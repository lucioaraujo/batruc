// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#include "core/FigureComposer.hpp"
#include "core/GeometryCurves.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>

namespace batruc {
namespace {

constexpr double kTwoPi = 6.283185307179586;
constexpr std::size_t kFigureVoices = 3;   // um pêndulo por voz

// splitmix32 local: a seed escolhe só as condições do modelo, nunca os ataques
std::uint32_t mix(std::uint32_t& state) noexcept {
    state += 0x9E3779B9u;
    std::uint32_t z = state;
    z = (z ^ (z >> 16)) * 0x85EBCA6Bu;
    z = (z ^ (z >> 13)) * 0xC2B2AE35u;
    return z ^ (z >> 16);
}
double unit(std::uint32_t& state) noexcept {
    return static_cast<double>(mix(state) >> 8) / 16777216.0;
}

// uma figura de um compasso: acento por passo (0 = sem ataque)
using Figure = std::vector<float>;

// peso métrico: tempo (a cada `beatSteps` passos), meio tempo, o resto
float metricWeight(const int step, const int beatSteps) noexcept {
    if (step == 0) return 1.0f;
    if (step % beatSteps == 0) return 0.95f;
    if (step % 2 == 0) return 0.84f;
    return 0.7f;
}

Figure rotate(const Figure& figure, const int shift) {
    const int n = static_cast<int>(figure.size());
    Figure out(figure.size(), 0.0f);
    for (int i = 0; i < n; ++i)
        out[static_cast<std::size_t>(((i + shift) % n + n) % n)] = figure[static_cast<std::size_t>(i)];
    return out;
}

} // namespace

const char* figureModelName(const FigureModel model) noexcept {
    switch (model) {
        case FigureModel::pendulum: return "pêndulos acoplados";
        case FigureModel::billiard: return "bilhar";
        case FigureModel::lissajous: return "Lissajous";
        case FigureModel::fourier: return "Fourier";
        case FigureModel::roulette: return "curvas rolantes";
    }
    return "";
}

bool composeFigures(const FigureComposerParameters& parameters,
                    FigureComposition& composition) {
    composition = {};
    if (parameters.voiceCount == 0 || parameters.voiceCount > kFigureVoices
        || parameters.barsPerSection < 1 || parameters.barsPerSection > 8
        || parameters.stepsPerBar < 4 || parameters.stepsPerBar > 32
        || !(parameters.sectionSeconds > 0.5 && parameters.sectionSeconds <= 60.0)
        || parameters.barsPerSection * parameters.stepsPerBar > 128)
        return false;

    const int bars = parameters.barsPerSection;
    const int steps = parameters.stepsPerBar;
    const double barSeconds = parameters.sectionSeconds / static_cast<double>(bars);
    const double stepSeconds = barSeconds / static_cast<double>(steps);
    // compassos em /8 (14 passos = 7/8) têm o tempo na colcheia
    const int beatSteps = steps % 4 == 0 ? 4 : 2;

    // ---- o modelo: três pêndulos acoplados, condições escolhidas pela seed
    std::uint32_t state = parameters.seed ^ 0x46494731u;   // "FIG1"
    CoupledPendulumParameters model;
    model.durationSeconds = parameters.sectionSeconds * static_cast<double>(kFormalCycleCount);
    model.sampleStepSeconds = 0.002;
    // v2 (12/11/2026, "mais variáveis, mais invenção"): período de ½ a 2
    // compassos, acoplamento e energia mais largos
    static constexpr double kPeriodRatios[6] = {0.5, 0.75, 1.0, 1.25, 1.5, 2.0};
    const double period = barSeconds * kPeriodRatios[mix(state) % 6u];
    model.lengthMetres = model.gravity * period * period / (kTwoPi * kTwoPi);
    model.springConstantNewtonsPerMetre = 0.15 + 2.35 * unit(state);   // troca de energia
    model.dampingPerSecond = 0.003 + 0.012 * unit(state);
    const double energy = 0.25 + 0.75 * unit(state);   // quão longe se balança
    for (std::size_t p = 0; p < 3; ++p) {
        const double sign = (mix(state) & 1u) ? 1.0 : -1.0;
        model.initialAnglesRadians[p] = sign * energy * (0.05 + 0.45 * unit(state));
        model.initialVelocitiesRadiansPerSecond[p] = (unit(state) - 0.5) * 0.8 * energy;
    }
    // a energia começa concentrada num pêndulo (a peça "acende" a partir dele)
    const std::size_t leader = mix(state) % 3u;
    model.initialAnglesRadians[leader] = std::copysign(0.3 + 0.35 * energy,
        model.initialAnglesRadians[leader]);
    // forma da frase: 0 A A' A B · 1 A B A B' · 2 A A A' B · 3 A A' B B'
    const int phraseForm = static_cast<int>(mix(state) % 4u);
    // ruptura: chance de, no compasso B, duas vozes trocarem de papel (Gramani)
    const double ruptureChance = 0.15 + 0.35 * unit(state);

    // ---- sinais por voz (valor e velocidade), amostrados a cada dt
    const double dt = model.sampleStepSeconds;
    std::array<std::vector<double>, kFigureVoices> value, rate;
    std::vector<double> times;   // instante de cada amostra
    std::array<double, kFigureVoices> low{}, high{};
    const bool pendulum = parameters.model == FigureModel::pendulum;
    std::ostringstream modelSummary;
    if (pendulum) {
        CoupledPendulumTrajectory trajectory;
        if (buildCoupledPendulumTrajectory(model, trajectory) != PendulumError::none
            || trajectory.samples.size() < 2)
            return false;
        for (std::size_t p = 0; p < kFigureVoices; ++p) {
            value[p].reserve(trajectory.samples.size());
            rate[p].reserve(trajectory.samples.size());
        }
        times.reserve(trajectory.samples.size());
        for (const auto& sample : trajectory.samples) {
            times.push_back(sample.timeSeconds);
            for (std::size_t p = 0; p < kFigureVoices; ++p) {
                value[p].push_back(sample.anglesRadians[p]);
                rate[p].push_back(sample.angularVelocitiesRadiansPerSecond[p]);
            }
        }
        modelSummary << "pendulos k=" << model.springConstantNewtonsPerMetre
                     << " T=" << period << "s lider=V" << (leader + 1);
    } else {
        // as condições da curva vêm de um sorteio próprio: as escolhas de
        // forma, ruptura e trastes seguem as mesmas do pêndulo
        std::uint32_t curveState = parameters.seed ^ 0x43525631u;   // "CRV1"
        GeometryCurve curve = GeometryCurve::billiardSquare;
        int fourierTerms = 1;
        int billiardPattern = 0;
        GeometryLissajousParameters lissajous;
        switch (parameters.model) {
            case FigureModel::billiard:
                billiardPattern = static_cast<int>(mix(curveState) % 7u);
                modelSummary << "bilhar padrao=" << billiardPattern;
                break;
            case FigureModel::lissajous:
                lissajous.xFrequency = 1 + static_cast<int>(mix(curveState) % 5u);
                do lissajous.yFrequency = 1 + static_cast<int>(mix(curveState) % 5u);
                while (lissajous.yFrequency == lissajous.xFrequency);
                lissajous.yPhaseTurns = 0.25 * unit(curveState);
                curve = GeometryCurve::lissajous;
                modelSummary << "lissajous " << lissajous.xFrequency << ":"
                             << lissajous.yFrequency;
                break;
            case FigureModel::fourier:
                curve = (mix(curveState) & 1u) ? GeometryCurve::fourierSquare
                                               : GeometryCurve::fourierTriangle;
                fourierTerms = 2 + static_cast<int>(mix(curveState) % 7u);
                modelSummary << (curve == GeometryCurve::fourierSquare
                    ? "fourier quadrada" : "fourier triangular")
                             << " termos=" << fourierTerms;
                break;
            case FigureModel::roulette:
            case FigureModel::pendulum: {
                static constexpr GeometryCurve kRoulettes[6] = {
                    GeometryCurve::epicycloid, GeometryCurve::hypocycloid,
                    GeometryCurve::cardioid, GeometryCurve::astroid,
                    GeometryCurve::ranunculoid, GeometryCurve::prolateTrochoid};
                static constexpr const char* kRouletteNames[6] = {
                    "epicicloide", "hipocicloide", "cardioide", "astroide",
                    "ranunculoide", "trocoide alongada"};
                const auto choice = mix(curveState) % 6u;
                curve = kRoulettes[choice];
                modelSummary << kRouletteNames[choice];
                break;
            }
        }
        // densidade: uma volta da curva gera E eventos na sombra da V2
        // (5 trastes nominais + retornos); a duração da volta é escolhida
        // para a V2 ter de 4 a 10 ataques por compasso, pela energia
        double lapEvents = 0.0;
        {
            constexpr int kProbe = 4000;
            std::vector<double> lap(kProbe);
            double lo = 1.0e9, hi = -1.0e9;
            for (int i = 0; i < kProbe; ++i) {
                Point2D point;
                if (!geometryCurvePoint(curve, static_cast<double>(i) / kProbe, 1.0, 0.0,
                                        fourierTerms, point, billiardPattern, lissajous))
                    return false;
                lap[static_cast<std::size_t>(i)] = point.y;
                lo = std::min(lo, point.y);
                hi = std::max(hi, point.y);
            }
            const double span = std::max(hi - lo, 1.0e-6);
            auto fret = [&](double y) {
                return static_cast<int>(std::clamp((y - lo) / span, 0.0, 0.999999) * 5.0);
            };
            for (std::size_t i = 2; i < lap.size(); ++i)
                if (fret(lap[i]) != fret(lap[i - 1])
                    || (lap[i] - lap[i - 1]) * (lap[i - 1] - lap[i - 2]) < 0.0)
                    lapEvents += 1.0;
        }
        const double targetPerBar = 2.0 + 8.0 * energy;
        const double lapSeconds = std::max(barSeconds * 0.25,
            std::max(lapEvents, 1.0) * barSeconds / targetPerBar);
        // a velocidade do percurso respira ao longo da peça
        const double waves = 1.0 + static_cast<double>(mix(curveState) % 2u);
        const double breathPhase = kTwoPi * unit(curveState);
        const double total = model.durationSeconds;
        const auto count = static_cast<std::size_t>(total / dt) + 2;
        for (std::size_t p = 0; p < kFigureVoices; ++p) {
            value[p].reserve(count);
            rate[p].reserve(count);
        }
        double progress = 0.0;
        Point2D previous{};
        times.reserve(count);
        for (std::size_t i = 0; i < count; ++i) {
            const double t = static_cast<double>(i) * dt;
            times.push_back(t);
            Point2D point;
            if (!geometryCurvePoint(curve, progress, 1.0, 0.0, fourierTerms, point,
                                    billiardPattern, lissajous))
                return false;
            const double shadows[3] = {point.x, point.y,
                                       0.7071067811865476 * (point.x + point.y)};
            const double before[3] = {previous.x, previous.y,
                                      0.7071067811865476 * (previous.x + previous.y)};
            for (std::size_t p = 0; p < kFigureVoices; ++p) {
                value[p].push_back(shadows[p]);
                rate[p].push_back(i == 0 ? 0.0 : (shadows[p] - before[p]) / dt);
            }
            previous = point;
            const double breath = 1.0 + 0.3 * std::sin(kTwoPi * waves * t / total + breathPhase);
            progress += dt / lapSeconds * breath;
        }
        modelSummary << " volta=" << lapSeconds << "s ondas=" << waves;
    }
    const std::size_t sampleCount = value[0].size();

    // trastes por voz: V1 âncora (poucos), V2 média, V3 fina
    static constexpr int kFretChoices[3][4] = {{3, 3, 4, 5}, {3, 4, 5, 6}, {4, 6, 7, 9}};
    for (std::size_t v = 0; v < kFigureVoices; ++v)
        composition.frets[v] = kFretChoices[v][mix(state) % 4u];
    composition.springConstant = model.springConstantNewtonsPerMetre;

    // faixa e velocidade máximas de cada sinal na peça inteira: os trastes
    // ficam fixos; a voz que perde energia cruza menos deles (no pêndulo, a
    // faixa é simétrica, ±amplitude)
    std::array<double, 3> speed{};
    for (std::size_t p = 0; p < kFigureVoices; ++p) {
        double amplitude = 0.0;
        double lo = 1.0e9, hi = -1.0e9;
        for (std::size_t i = 0; i < sampleCount; ++i) {
            amplitude = std::max(amplitude, std::abs(value[p][i]));
            lo = std::min(lo, value[p][i]);
            hi = std::max(hi, value[p][i]);
            speed[p] = std::max(speed[p], std::abs(rate[p][i]));
        }
        low[p] = pendulum ? -amplitude : lo;
        high[p] = pendulum ? amplitude : hi;
    }

    // ---- figura do modelo para um compasso (voz v, compasso absoluto)
    auto modelFigure = [&](std::size_t v, int absoluteBar) {
        Figure figure(static_cast<std::size_t>(steps), 0.0f);
        const int frets = composition.frets[v];
        const double span = std::max(high[v] - low[v], 2.0e-6);
        const double t0 = static_cast<double>(absoluteBar) * barSeconds;
        const double t1 = t0 + barSeconds;
        auto band = [&](double x0) {
            const double x = std::clamp((x0 - low[v]) / span, 0.0, 0.999999);
            return static_cast<int>(x * static_cast<double>(frets));
        };
        const auto& signal = value[v];
        for (std::size_t i = 1; i < sampleCount; ++i) {
            const double t = times[i];
            if (t < t0) continue;
            if (t >= t1) break;
            // nas curvas, o ponto de retorno da sombra (a tabela, a volta)
            // também é ataque, acentuado
            const bool turning = !pendulum && i >= 2
                && (signal[i] - signal[i - 1]) * (signal[i - 1] - signal[i - 2]) < 0.0;
            if (!turning && band(signal[i - 1]) == band(signal[i]))
                continue;
            // grade por papel: V1 âncora em colcheias; V2 em semicolcheias,
            // atraída para o tempo quando passa perto dele; V3 livre
            const double exact = (t - t0) / stepSeconds;
            int step = static_cast<int>(std::lround(exact));
            if (v == 0) {
                step = 2 * static_cast<int>(std::lround(exact / 2.0));
            } else if (v == 1) {
                const double beat = beatSteps * std::round(exact / beatSteps);
                if (std::abs(exact - beat) < 1.25) step = static_cast<int>(beat);
            }
            if (step >= steps) continue;   // pertence ao compasso seguinte
            const float velocity = turning ? 0.85f : static_cast<float>(std::min(1.0,
                std::abs(rate[v][i]) / std::max(speed[v], 1.0e-6)));
            const float accent = (0.32f + 0.68f * velocity) * metricWeight(step, beatSteps);
            auto& slot = figure[static_cast<std::size_t>(step)];
            slot = std::max(slot, std::min(1.0f, accent));
        }
        return figure;
    };

    std::ostringstream summary;
    summary << "forma=" << phraseForm << " energia=" << energy
            << " " << modelSummary.str() << " trastes";
    for (std::size_t v = 0; v < parameters.voiceCount; ++v)
        summary << " V" << (v + 1) << "=" << composition.frets[v];
    summary << " | ataques por ciclo:";

    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        // figuras da frase, por voz: A A' A B
        std::array<std::vector<Figure>, kFigureVoices> phrase;
        for (std::size_t v = 0; v < parameters.voiceCount; ++v) {
            const int firstBar = static_cast<int>(cycle) * bars;
            const Figure motif = modelFigure(v, firstBar);
            // deslocamento de A' tirado do próprio modelo: a posição do
            // pêndulo no início do 2º compasso, em passos pares
            const double probe = value[v][std::min(sampleCount - 1,
                static_cast<std::size_t>((static_cast<double>(firstBar + 1) * barSeconds)
                                         / model.sampleStepSeconds))];
            const double where = (probe - low[v])
                / std::max(high[v] - low[v], 1.0e-6);
            const int shift = 2 * static_cast<int>(std::clamp(where, 0.0, 0.999) * 4.0) + 2;
            // B: o compasso próprio do modelo (no fim da frase)
            const Figure other = modelFigure(v, firstBar + bars - 1);
            for (int bar = 0; bar < bars; ++bar) {
                // papel do compasso pela forma da frase sorteada
                static constexpr int kForms[4][4] = {
                    {0, 1, 0, 3},   // A A' A B
                    {0, 2, 0, 4},   // A B A B'
                    {0, 0, 1, 3},   // A A A' B
                    {0, 1, 2, 4}};  // A A' B B'
                const int role = kForms[phraseForm][bar % 4];
                Figure figure = role == 0 ? motif
                    : role == 1 ? rotate(motif, shift)
                    : role == 2 ? other
                    : role == 4 ? rotate(other, shift)
                    : modelFigure(v, firstBar + bar);
                if (bar % 4 == 3) {   // cadência: anacruse na voz 1, respiro na 3
                    if (v == 0) {
                        if (figure[static_cast<std::size_t>(steps - 2)] <= 0.0f)
                            figure[static_cast<std::size_t>(steps - 2)] = 0.55f;
                        if (figure[static_cast<std::size_t>(steps - 1)] <= 0.0f)
                            figure[static_cast<std::size_t>(steps - 1)] = 0.72f;
                    } else if (v == 2) {
                        for (int s = steps - steps / 4; s < steps; ++s)
                            figure[static_cast<std::size_t>(s)] = 0.0f;
                    }
                }
                phrase[v].push_back(std::move(figure));
            }
        }
        // ruptura (Gramani): no último compasso, V1 e V2 trocam de figura
        if (parameters.voiceCount >= 2 && bars >= 4 && unit(state) < ruptureChance) {
            std::swap(phrase[0][static_cast<std::size_t>(bars - 1)],
                      phrase[1][static_cast<std::size_t>(bars - 1)]);
            summary << " ruptura@c" << (cycle + 1);
        }
        // âncora: a voz 1 abre a frase no primeiro tempo
        if (!phrase[0].empty())
            phrase[0][0][0] = std::max(phrase[0][0][0], 0.92f);
        // encaixe: V2/V3 não batem junto com V1 — vão ao vizinho livre
        for (std::size_t v = 1; v < parameters.voiceCount; ++v)
            for (int bar = 0; bar < bars; ++bar) {
                auto& figure = phrase[v][static_cast<std::size_t>(bar)];
                const auto& anchor = phrase[0][static_cast<std::size_t>(bar)];
                for (int s = 0; s < steps; ++s) {
                    const auto si = static_cast<std::size_t>(s);
                    if (figure[si] <= 0.0f || anchor[si] <= 0.0f) continue;
                    const float accent = figure[si];
                    figure[si] = 0.0f;
                    for (const int d : {1, -1}) {
                        const int n = s + d;
                        if (n < 0 || n >= steps) continue;
                        const auto ni = static_cast<std::size_t>(n);
                        if (figure[ni] <= 0.0f && anchor[ni] <= 0.0f) {
                            figure[ni] = accent * 0.9f;
                            break;
                        }
                    }
                }
            }
        // células da seção
        for (std::size_t v = 0; v < parameters.voiceCount; ++v) {
            RhythmicCell& cell = composition.cells[cycle][v];
            cell.enabled = true;
            for (int bar = 0; bar < bars; ++bar)
                for (int s = 0; s < steps; ++s) {
                    const float accent = phrase[v][static_cast<std::size_t>(bar)][static_cast<std::size_t>(s)];
                    if (accent <= 0.0f) continue;
                    const double position = static_cast<double>(bar * steps + s)
                        / static_cast<double>(bars * steps);
                    insertRhythmicCellAttack(cell, position, std::clamp(accent, 0.05f, 1.0f));
                }
            if (!validRhythmicCell(cell))
                return false;
        }
        summary << " [";
        for (std::size_t v = 0; v < parameters.voiceCount; ++v)
            summary << (v ? "," : "") << static_cast<int>(composition.cells[cycle][v].count);
        summary << "]";
    }
    composition.summary = summary.str();
    return true;
}

} // namespace batruc

// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#pragma once

#include "core/CompositionCore.hpp"

#include <array>
#include <cstdint>
#include <string>

// FIGURAS A PARTIR DO MODELO (12/11/2026; AUDITORIA_MUSICAL_2026-11-12.md §7,
// primeiro lote). O núcleo antigo transforma cada voz num trem de pulsos
// iguais (cruzamentos de uma senoide). Esta camada fica entre o modelo físico
// e o evento: os três pêndulos acoplados que o Batruc já calcula viram
// FIGURAS rítmicas, MOTIVO e DESENVOLVIMENTO, entregues como células
// (`RhythmicCell`) — o agendador não muda.
//
// Tradução (analogia composicional declarada, não lei física):
//   - TRASTES: o ângulo de cada pêndulo é dividido em K faixas iguais entre
//     −A e +A (A = amplitude máxima da voz no trecho). Cruzar uma fronteira
//     gera um ataque. Perto do centro o pêndulo é rápido e as fronteiras
//     passam juntas (os ataques adensam); perto do extremo ele desacelera e
//     os ataques rareiam — figuras que aceleram e freiam dentro do balanço.
//   - ACENTO: a velocidade angular no cruzamento, normalizada.
//   - GRADE: os ataques são levados ao passo mais próximo (16 por compasso);
//     dois no mesmo passo viram um, com o acento maior.
//   - MOTIVO E FRASE: a seção tem 4 compassos, em A A' A B. A' é A girado
//     pelo deslocamento do modelo; B é o compasso próprio do modelo com
//     uma cadência (anacruse para a próxima seção).
//   - DESENVOLVIMENTO: a trajetória é contínua por toda a peça (7 ciclos ×
//     seção); a mola passa energia entre os pêndulos, e a voz que recebe
//     energia cruza mais trastes — a densidade de cada voz muda com a física.
//   - ENCAIXE: um ataque da voz 2 ou 3 no mesmo passo da voz 1 desloca-se
//     para o passo vizinho livre (dissociação, Gramani); a voz 1 ancora o
//     primeiro tempo de cada frase.
// OUTROS MODELOS (12/11/2026; reordenação, camada 3: "bilhar, Lissajous,
// Fourier, cicloide … a mesma tradução em trastes"). A tradução acima vale
// para qualquer trajetória: o modelo entrega um sinal por voz. Nas curvas
// do `GeometryCurves`, um ponto percorre a curva e as três vozes são as
// suas três SOMBRAS — V1 a projeção em x, V2 em y, V3 na diagonal. A
// velocidade do percurso respira ao longo da peça (±30 %, de 1 a 2 ondas
// nos 7 ciclos), e cada ponto de retorno de uma sombra (a tabela do bilhar,
// a volta da Lissajous) vira um ataque acentuado. O pêndulo continua
// exatamente como era.
// Determinístico: mesma seed, mesmas figuras. Sem alocação fora de
// `composeFigures`.

namespace batruc {

enum class FigureModel : std::uint8_t {
    pendulum,    // três pêndulos acoplados (o original)
    billiard,    // bilhar no quadrado
    lissajous,   // figura de Lissajous
    fourier,     // série de Fourier (quadrada ou triangular, poucos termos)
    roulette     // rolantes: epi/hipocicloide, cardioide, astroide, trocoides
};
constexpr int kFigureModelCount = 5;
const char* figureModelName(FigureModel model) noexcept;   // em português

struct FigureComposerParameters {
    std::uint32_t seed = 0;
    FigureModel model = FigureModel::pendulum;
    std::size_t voiceCount = 3;          // vozes com figura (1–3)
    double sectionSeconds = 8.0;
    int barsPerSection = 4;
    int stepsPerBar = 16;
};

struct FigureComposition {
    // células por ciclo formal e por voz (vozes além de voiceCount desligadas)
    std::array<std::array<RhythmicCell, kMaxVoices>, kFormalCycleCount> cells{};
    std::array<int, 3> frets{};          // K por voz
    double springConstant = 0.0;
    std::string summary;                 // descrição legível da realização
};

bool composeFigures(const FigureComposerParameters& parameters,
                    FigureComposition& composition);

} // namespace batruc

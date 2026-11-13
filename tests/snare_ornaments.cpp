// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Ornamentos da caixa (12/11/2026): cada um escreve ataques válidos; flam,
// drag e rufo vêm ANTES da principal; o ricochete perde força a cada rebote;
// o rulo enche o espaço com fusas; o aro marca GM 37; tudo determinístico.
#include "core/SnareOrnaments.hpp"

#include <cmath>
#include <cstdio>

using namespace batruc;

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) { std::printf("FALHOU: %s\n", what); ++failures; }
}
RhythmicCell backbeat() {   // caixa nos tempos 2 e 4 de quatro compassos
    RhythmicCell cell;
    cell.enabled = true;
    for (int bar = 0; bar < 4; ++bar)
        for (const int beat : {1, 3})
            insertRhythmicCellAttack(cell, (bar * 16 + beat * 4) / 64.0,
                                     beat == 3 ? 0.9f : 0.6f);
    return cell;
}
} // namespace

int main() {
    for (int k = 0; k < kSnareOrnamentCount; ++k) {
        auto cell = backbeat();
        SnareOrnamentParameters p;
        p.kind = static_cast<SnareOrnament>(k);
        p.amount = 1.0f;
        p.seed = 0x5EED2027u;
        const int changed = ornamentSnareCell(cell, p);
        std::printf("%-16s +%d (total %d)\n", snareOrnamentName(p.kind), changed,
                    static_cast<int>(cell.count));
        check(changed > 0, "o ornamento escreve algo");
        check(validRhythmicCell(cell), "a célula continua válida");
        auto again = backbeat();
        ornamentSnareCell(again, p);
        check(again.count == cell.count, "determinístico");
    }
    {   // flam: enfeite ~25 ms antes de cada acento, mais fraco
        auto cell = backbeat();
        SnareOrnamentParameters p; p.kind = SnareOrnament::flam; p.amount = 1.0f;
        ornamentSnareCell(cell, p);
        const double grace = (12 / 64.0) - 0.025 / 8.0;
        bool found = false;
        for (std::size_t i = 0; i < cell.count; ++i)
            if (std::abs(cell.positions[i] - grace) < 1e-6 && cell.strengths[i] < 0.5f) found = true;
        check(found, "flam: enfeite 25 ms antes do acento");
    }
    {   // ricochete: os rebotes depois do primeiro golpe perdem força
        auto cell = backbeat();
        SnareOrnamentParameters p; p.kind = SnareOrnament::ricochet; p.amount = 1.0f;
        ornamentSnareCell(cell, p);
        float previous = 2.0f; bool decaying = true; int rebounds = 0;
        for (std::size_t i = 0; i < cell.count; ++i) {
            if (cell.positions[i] <= 4 / 64.0 + 1e-9) continue;
            if (cell.positions[i] >= 12 / 64.0 - 1e-9) break;
            decaying = decaying && cell.strengths[i] < previous;
            previous = cell.strengths[i]; ++rebounds;
        }
        check(rebounds >= 3 && decaying, "ricochete: rebotes que perdem força");
    }
    {   // aro: notas fracas recebem GM 37
        auto cell = backbeat();
        SnareOrnamentParameters p; p.kind = SnareOrnament::rim; p.amount = 1.0f;
        ornamentSnareCell(cell, p);
        int rims = 0;
        for (std::size_t i = 0; i < cell.count; ++i) rims += cell.midiNotes[i] == 37;
        check(rims == 4, "aro: as quatro notas fracas viram GM 37");
    }
    // ornamento como propriedade da nota (13/11): a célula ESCRITA guarda uma
    // nota marcada; a TOCADA (expandida) é igual à inserção antiga
    for (const auto kind : {SnareOrnament::flam, SnareOrnament::drag,
                            SnareOrnament::ruff, SnareOrnament::roll}) {
        SnareOrnamentParameters p;
        p.kind = kind; p.amount = 1.0f; p.seed = 0x5EED2027u;
        auto legacy = backbeat();
        ornamentSnareCell(legacy, p);
        auto written = backbeat();
        const int marked = markSnareOrnaments(written, p);
        check(marked > 0, "marca notas");
        check(written.count == backbeat().count, "a célula escrita não ganha toques");
        check(validRhythmicCell(written), "célula escrita válida");
        SnareOrnament found;
        bool anyMark = false;
        for (std::size_t i = 0; i < written.count; ++i)
            anyMark = anyMark || (cellOrnament(written, i, found) && found == kind);
        check(anyMark, "a nota guarda o tipo do ornamento");
        RhythmicCell performed;
        expandSnareOrnaments(written, performed, p.sectionSeconds, p.stepsPerSection);
        bool same = performed.count == legacy.count;
        for (std::size_t i = 0; same && i < performed.count; ++i)
            same = std::abs(performed.positions[i] - legacy.positions[i]) < 1e-12
                && std::abs(performed.strengths[i] - legacy.strengths[i]) < 1e-6f;
        check(same, "expandir = inserção antiga (mesmo som)");
        bool noMarks = true;
        for (std::size_t i = 0; i < performed.count; ++i) noMarks = noMarks && performed.ornaments[i] == 0;
        check(noMarks, "a célula tocada não carrega marcas");
    }
    {   // fantasmas continuam sendo notas escritas
        auto cell = backbeat();
        SnareOrnamentParameters p; p.kind = SnareOrnament::ghostNotes; p.amount = 1.0f;
        check(markSnareOrnaments(cell, p) > 0 && cell.count > backbeat().count,
              "fantasmas: notas de verdade na célula escrita");
    }
    {   // ORN desligado no stem: batida normal (sem fantasmas, aro volta ao normal)
        auto cell = backbeat();
        SnareOrnamentParameters ghost; ghost.kind = SnareOrnament::ghostNotes; ghost.amount = 1.0f;
        SnareOrnamentParameters rim; rim.kind = SnareOrnament::rim; rim.amount = 1.0f;
        SnareOrnamentParameters roll; roll.kind = SnareOrnament::roll; roll.amount = 1.0f;
        markSnareOrnaments(cell, ghost);
        markSnareOrnaments(cell, rim);
        markSnareOrnaments(cell, roll);
        RhythmicCell off, on;
        expandSnareOrnaments(cell, off, 8.0, 64, false);
        expandSnareOrnaments(cell, on, 8.0, 64, true);
        check(off.count == backbeat().count, "desligado: volta à batida escrita (sem fantasmas)");
        bool noRim = true;
        for (std::size_t i = 0; i < off.count; ++i) noRim = noRim && off.midiNotes[i] != 37;
        check(noRim, "desligado: o aro volta ao toque normal");
        check(on.count > cell.count, "ligado: fantasmas, aro e rulo");
    }
    {   // ornamentos desligados (seção 0): a tocada é só a escrita
        auto cell = backbeat();
        SnareOrnamentParameters p; p.kind = SnareOrnament::roll; p.amount = 1.0f;
        markSnareOrnaments(cell, p);
        RhythmicCell performed;
        expandSnareOrnaments(cell, performed, 0.0, 64);
        check(performed.count == cell.count, "desligado: sem toques de enfeite");
    }
    {   // sem marcas, tocada = escrita
        const auto cell = backbeat();
        RhythmicCell performed;
        expandSnareOrnaments(cell, performed, 8.0, 64);
        check(performed.count == cell.count, "sem marcas, nada muda");
    }

    {   // as operações do Study levam o ornamento com a nota
        auto cell = backbeat();
        SnareOrnamentParameters p; p.kind = SnareOrnament::roll; p.amount = 1.0f;
        const int marked = markSnareOrnaments(cell, p);
        auto countMarks = [](const RhythmicCell& c) {
            int n = 0;
            for (std::size_t i = 0; i < c.count; ++i) n += c.ornaments[i] != 0;
            return n;
        };
        auto rotated = cell;
        check(rotateRhythmicCell(rotated, 0.25) && countMarks(rotated) == marked,
              "rotação mantém os ornamentos");
        auto spread = cell;
        // em 16 passos a caixa em 2 e 4 já está distribuída (nada a fazer);
        // em 12 a operação de fato move os ataques
        check(euclideanRedistributeRhythmicCell(spread, 12) && countMarks(spread) == marked,
              "redistribuição mantém os ornamentos");
        auto reduced = cell;
        if (reduceRhythmicCellDensity(reduced, 0.5, 0x5EEDu))
            check(countMarks(reduced) == static_cast<int>(reduced.count),
                  "redução mantém o ornamento das notas que ficam");
    }

    {   // idiomas da caixa por gênero (§4b)
        auto frevo = backbeat();
        applySnareIdiom(frevo, SnareIdiom::frevo, 4, 4, 4, 1u);
        int rolls = 0;
        for (std::size_t i = 0; i < frevo.count; ++i) {
            SnareOrnament k;
            if (cellOrnament(frevo, i, k) && k == SnareOrnament::roll) {
                ++rolls;
                // último tempo de um compasso par (2 ou 4): passos 28 e 60
                const long step = std::lround(frevo.positions[i] * 64);
                check(step == 28 || step == 60, "frevo: rulo no último tempo da frase");
            }
        }
        check(rolls == 2, "frevo: um rulo por frase de 2 compassos");
        RhythmicCell eighths; eighths.enabled = true;
        for (int s = 0; s < 64; s += 2) insertRhythmicCellAttack(eighths, s / 64.0, 0.6f);
        applySnareIdiom(eighths, SnareIdiom::maracatu, 4, 4, 4, 1u);
        int buzzes = 0;
        for (std::size_t i = 0; i < eighths.count; ++i) {
            SnareOrnament k;
            if (cellOrnament(eighths, i, k) && k == SnareOrnament::buzz) {
                ++buzzes;
                check(std::lround(eighths.positions[i] * 64) % 4 == 2, "maracatu: 2ª nota do tempo");
                check(eighths.strengths[i] >= 0.9f, "maracatu: acentuada");
            }
        }
        check(buzzes == 16, "maracatu: toque múltiplo em cada tempo");
        auto baiao = backbeat();
        applySnareIdiom(baiao, SnareIdiom::baiao, 4, 4, 4, 7u);
        int accents = 0;
        for (std::size_t i = 0; i < baiao.count; ++i)
            if (std::lround(baiao.positions[i] * 64) % 16 == 6 && baiao.strengths[i] >= 0.9f) ++accents;
        check(accents == 4, "baião: acento na 2ª colcheia do 2º tempo");
        check(validRhythmicCell(frevo) && validRhythmicCell(eighths) && validRhythmicCell(baiao),
              "idiomas: células válidas");
        RhythmicCell played;
        expandSnareOrnaments(eighths, played, 8.0, 64);
        check(played.count > eighths.count, "toque múltiplo expande em rebotes");
    }

    {   // microtempo medido (ISMIR 2020): 3ª e 4ª semicolcheias adiantadas
        RhythmicCell sixteenths; sixteenths.enabled = true;
        for (int s = 0; s < 64; ++s) insertRhythmicCellAttack(sixteenths, s / 64.0, 0.6f);
        auto tarol = sixteenths;
        const int moved = applyMicrotimingProfile(tarol, MicrotimingProfile::tarolBaqueSolto, 16, 1.0f);
        check(moved == 32, "microtempo: move a 3ª e a 4ª de cada tempo");
        check(std::abs(tarol.positions[2] - 0.47 / 16.0) < 1e-9
              && std::abs(tarol.positions[3] - 0.715 / 16.0) < 1e-9, "tarol: 0,47 e 0,715 do tempo");
        check(std::abs(tarol.positions[1] - 1.0 / 64.0) < 1e-12, "a 2ª semicolcheia fica");
        auto half = sixteenths;
        applyMicrotimingProfile(half, MicrotimingProfile::tarolBaqueSolto, 16, 0.5f);
        check(std::abs(half.positions[2] - (0.5 - 0.015) / 16.0) < 1e-9, "profundidade 50 %");
        check(validRhythmicCell(tarol) && validRhythmicCell(half), "microtempo: células válidas");
    }

    if (failures == 0) std::printf("snare_ornaments: OK\n");
    return failures == 0 ? 0 : 1;
}

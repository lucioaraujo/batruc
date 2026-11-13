// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Pauta tradicional (ScoreNotation). Critérios: cada compasso soma a duração
// do compasso; figuras e pontos corretos; ligadura através da barra; nota
// fora do tempo não cruza o tempo; armadura por modo; acidentes no compasso.
#include "core/ScoreNotation.hpp"

#include <cstdio>
#include <string>

using namespace batruc;

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) { std::printf("FALHOU: %s\n", what); ++failures; }
}
int barTotal(const NotatedBar& bar) {
    int total = 0;
    for (const auto& item : bar.items) total += item.steps;
    return total;
}
} // namespace

int main() {
    // armaduras
    check(keyFifthsForMode(0, 0) == 0, "Dó maior: 0");
    check(keyFifthsForMode(9, 1) == 0, "Lá menor: 0");
    check(keyFifthsForMode(2, 2) == 0, "Ré dórico: 0");
    check(keyFifthsForMode(7, 3) == 0, "Sol mixolídio: 0");
    check(keyFifthsForMode(10, 5) == -1, "Si♭ lídio: 1 bemol (Fá maior)");
    check(keyFifthsForMode(7, 0) == 1, "Sol maior: 1 sustenido");
    check(keyFifthsForMode(2, 1) == -1, "Ré menor: 1 bemol");
    check(keySignatureAlter(6, -1) == -1 && keySignatureAlter(3, 1) == 1,
          "si♭ em −1, fá♯ em +1");

    // semínima, colcheia pontuada + semicolcheia, mínima ligada à barra
    NotationParameters p;   // 4/4, 4 compassos, Dó maior
    std::vector<LineNote> notes = {
        {0, 4, 36, 0.8f},       // semínima
        {4, 7, 38, 0.7f},       // colcheia pontuada
        {7, 8, 40, 0.6f},       // semicolcheia
        {12, 20, 41, 0.7f},     // começa no tempo 4 e cruza a barra
        {21, 23, 43, 0.6f}};    // fora do tempo: não pode cruzar o tempo
    std::vector<NotatedBar> bars;
    check(notateLine(notes, p, bars), "notateLine");
    check(bars.size() == 4, "4 compassos");
    bool sums = true;
    for (const auto& bar : bars) sums = sums && barTotal(bar) == 16;
    check(sums, "cada compasso soma 16 semicolcheias");
    const auto& b1 = bars[0].items;
    check(!b1.empty() && !b1[0].rest && b1[0].figure == 4 && !b1[0].dotted, "semínima");
    check(b1.size() > 1 && b1[1].figure == 8 && b1[1].dotted, "colcheia pontuada");
    check(b1.size() > 2 && b1[2].figure == 16, "semicolcheia");
    check(b1.size() > 3 && b1[3].rest && b1[3].steps == 4, "pausa de semínima no tempo 3");
    check(b1.size() > 4 && b1[4].tieToNext, "nota do tempo 4 ligada à barra");
    check(!bars[1].items.empty() && bars[1].items[0].tieFromPrevious, "continua ligada no compasso 2");
    bool offbeatOk = true;
    for (const auto& item : bars[1].items) {
        const int beatStart = item.startStep / 4 * 4;
        if (item.startStep % 4 != 0 && item.startStep + item.steps > beatStart + 4)
            offbeatOk = false;
    }
    check(offbeatOk, "nada fora do tempo atravessa o tempo");

    // acidentes: Fá maior (−1); si natural leva ♮, o 2º si natural não repete
    NotationParameters f;
    f.keyFifths = -1;
    std::vector<LineNote> fNotes = {{0, 4, 47, 0.7f}, {4, 8, 47, 0.7f}, {8, 12, 46, 0.7f}};
    check(notateLine(fNotes, f, bars), "notateLine Fá maior");
    const auto& fb = bars[0].items;
    check(fb.size() >= 3 && fb[0].showAccidental && fb[0].alter == 0, "si natural com bequadro");
    check(fb.size() >= 3 && !fb[1].showAccidental, "segundo si natural sem repetir");
    check(fb.size() >= 3 && fb[2].showAccidental && fb[2].alter == -1, "si♭ volta com bemol");

    // 7/8: compasso de 14 semicolcheias
    NotationParameters seven;
    seven.beatsPerBar = 7;
    seven.beatUnit = 8;
    check(notateLine({{0, 6, 36, 0.7f}}, seven, bars), "notateLine 7/8");
    bool sevenSums = true;
    for (const auto& bar : bars) sevenSums = sevenSums && barTotal(bar) == 14;
    check(sevenSums, "7/8 soma 14");

    // MusicXML: 7 ciclos × 4 compassos, clave de Fá, armadura, transposição
    std::vector<NotatedBar> piece;
    NotationParameters key;
    key.keyFifths = -1;
    for (int cycle = 0; cycle < 7; ++cycle) {
        std::vector<NotatedBar> cycleBars;
        check(notateLine(notes, key, cycleBars), "notateLine ciclo");
        piece.insert(piece.end(), cycleBars.begin(), cycleBars.end());
    }
    MusicXmlParameters xml;
    xml.title = "Batruc · teste";
    xml.octaveTranspose = -1;
    const auto text = musicXmlForBars(piece, key, xml);
    auto count = [&](const std::string& needle) {
        std::size_t n = 0, at = 0;
        while ((at = text.find(needle, at)) != std::string::npos) { ++n; at += needle.size(); }
        return n;
    };
    std::size_t items = 0;
    for (const auto& bar : piece) items += bar.items.size();
    check(count("<measure number=") == 28, "MusicXML com 28 compassos");
    check(count("<note>") == items, "uma <note> por figura");
    check(text.find("<sign>F</sign><line>4</line>") != std::string::npos, "clave de Fá");
    check(text.find("<fifths>-1</fifths>") != std::string::npos, "armadura −1");
    check(text.find("<octave-change>-1</octave-change>") != std::string::npos, "transposição do baixo elétrico");
    check(count("<tie type=\"start\"/>") == count("<tie type=\"stop\"/>"), "ligaduras pareadas");
    check(text.find("light-heavy") != std::string::npos, "barra final");

    // bateria: chimbal em colcheias, caixa em 2 e 4 (flam no 2, rulo no 4),
    // bumbo em 1 e 3
    {
        NotationParameters drums;   // 4/4, 4 compassos
        std::vector<LineNote> handLine, feetLine;
        std::vector<DrumHit> hits;
        for (int bar = 0; bar < 4; ++bar)
            for (int eighth = 0; eighth < 8; ++eighth) {
                const int step = bar * 16 + eighth * 2;
                handLine.push_back({step, step + 2, 60, 0.7f});
                DrumHit hat; hat.step = step; hat.place = 39; hat.cross = true;
                hat.sticking = 1;
                hits.push_back(hat);
                if (eighth == 2 || eighth == 6) {
                    DrumHit snare; snare.step = step; snare.place = 35; snare.accent = true;
                    snare.ornament = eighth == 2 ? 1 : 4;
                    hits.push_back(snare);
                }
                if (eighth == 0 || eighth == 4) {
                    feetLine.push_back({step, step + 4, 60, 0.7f});
                    DrumHit kick; kick.step = step; kick.feet = true; kick.place = 31;
                    hits.push_back(kick);
                }
            }
        std::vector<NotatedBar> handBars, feetBars;
        check(notateLine(handLine, drums, handBars) && notateLine(feetLine, drums, feetBars),
              "bateria: escrita das duas vozes");
        MusicXmlParameters dx;
        dx.partName = "Bateria";
        const auto dtext = musicXmlForDrumStaff(handBars, feetBars, hits, drums, dx);
        auto dcount = [&](const std::string& needle) {
            std::size_t n = 0, at = 0;
            while ((at = dtext.find(needle, at)) != std::string::npos) { ++n; at += needle.size(); }
            return n;
        };
        check(dcount("<measure number=") == 4, "bateria: 4 compassos");
        check(dtext.find("<sign>percussion</sign>") != std::string::npos, "clave de percussão");
        check(dcount("<backup>") == 4, "bateria: segunda voz em cada compasso");
        check(dcount("<notehead>x</notehead>") == 32, "32 chimbais com cabeça ×");
        check(dcount("<grace slash=\"yes\"/>") == 4, "4 flams (notinha cortada)");
        check(dcount("<tremolo type=\"single\">3</tremolo>") == 4, "4 rulos (3 barras)");
        // o bumbo fica na voz dos pés; só a caixa forma acorde com o chimbal
        check(dcount("<chord/>") == 8, "caixa em acorde com o chimbal");
        check(dcount("<lyric") == 32, "manulação embaixo");
    }

    if (failures == 0) std::printf("score_notation: OK\n");
    return failures == 0 ? 0 : 1;
}

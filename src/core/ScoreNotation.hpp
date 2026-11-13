// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#pragma once

#include <cstdint>
#include <string>
#include <vector>

// PAUTA TRADICIONAL (13/11/2026; o autor: "achei que ia escrever a partitura
// como a pauta tradicional de notação musical"; "verifique os códigos já
// elaborados no maravi"; "avance"). Converte uma linha monofônica numa grade
// de semicolcheias em FIGURAS de pauta: compassos, notas e pausas, pontos,
// ligaduras e a grafia das alturas pela armadura.
//
// Origem (mesma autoria e licença, porte com crédito):
//   - ANTITOTEM `writeMusicXmlCaptureToFile` (src/app/Main.cpp): quantização
//     numa grade, decomposição gulosa em figuras com ponto e ligadura que
//     atravessa a barra de compasso;
//   - MARAVI `tools/musicxml_writer.py`: grafia dos acidentes pela armadura
//     (bemol em tom de bemóis, sustenido em tom de sustenidos) e a escrita
//     pensada tempo a tempo, para a pauta não sair "quadrada".
// Acréscimo daqui: a nota que começa fora do tempo não atravessa a fronteira
// do tempo, e em compassos pares a metade do compasso só é atravessada por
// quem começa no primeiro tempo (convenção de legibilidade da escrita
// rítmica). O passo é sempre a semicolcheia.
namespace batruc {

struct LineNote {
    int startStep = 0;     // em semicolcheias desde o início do trecho
    int endStep = 1;
    int midi = 36;
    float velocity = 0.7f;
};

struct NotationParameters {
    int beatsPerBar = 4;   // numerador
    int beatUnit = 4;      // denominador (4 ou 8)
    int bars = 4;
    int keyFifths = 0;     // armadura: + sustenidos, − bemóis
};

struct NotatedItem {
    bool rest = false;
    int startStep = 0;     // dentro do compasso
    int steps = 1;         // duração em semicolcheias (com o ponto)
    int figure = 4;        // 1 semibreve, 2 mínima, 4 semínima, 8 colcheia, 16 semicolcheia
    bool dotted = false;
    bool tieToNext = false;
    bool tieFromPrevious = false;
    int midi = 0;
    float velocity = 0.0f;
    // grafia
    int diatonic = 0;      // oitava × 7 + grau (dó = 0 … si = 6)
    int alter = 0;         // −1, 0, +1
    bool showAccidental = false;   // ♯/♭/♮ escrito antes da nota
};

struct NotatedBar {
    std::vector<NotatedItem> items;
};

// Armadura (quintas) do tom: tônica 0–11 e modo pelo índice de
// `kBassKeyScales` (maior, menor, dórico, mixolídio, frígio, lídio, menor
// harmônica, pentatônica maior/menor, blues).
int keyFifthsForMode(int tonic, int scale) noexcept;

// Grafia de uma altura MIDI: grau diatônico com oitava e alteração.
void spellMidi(int midi, int keyFifths, int& diatonic, int& alter) noexcept;

// Alteração que a armadura dá a um grau (0–6).
int keySignatureAlter(int degree, int keyFifths) noexcept;

// Escreve a linha em compassos. As notas devem ser monofônicas; sobreposições
// são encurtadas (a anterior termina onde a próxima começa). Devolve false
// para parâmetros inválidos.
bool notateLine(std::vector<LineNote> notes, const NotationParameters& parameters,
                std::vector<NotatedBar>& bars);

// MusicXML 4.0 (partwise) de uma parte com os compassos já escritos, em
// sequência (ex.: os 7 ciclos × 4 compassos). `octaveTranspose` = −1 declara
// a escrita do baixo elétrico (escrito uma 8ª acima do som), para o editor
// de partitura tocar na altura real. Sem dinâmica nem barras de colcheia: o
// editor agrupa as colcheias sozinho.
struct MusicXmlParameters {
    std::string partName = "Baixo";
    std::string title;
    double bpm = 120.0;
    int octaveTranspose = 0;
};
std::string musicXmlForBars(const std::vector<NotatedBar>& bars,
                            const NotationParameters& notation,
                            const MusicXmlParameters& xml);

// Bateria (13/11/2026): parte de percussão em duas vozes — mãos (hastes
// para cima) e pés (hastes para baixo) — com os toques de cada posição.
struct DrumHit {
    int step = 0;          // semicolcheia desde o início do trecho
    bool feet = false;     // voz dos pés (bumbo)
    int place = 35;        // posição na pauta (grau diatônico; mi4 = 30 na 1ª linha)
    bool cross = false;    // cabeça ×
    bool ghost = false;    // entre parênteses
    bool accent = false;
    int ornament = 0;      // 0 nenhum · 1 flam · 2 drag · 3 rufo · 4 rulo · 5 ricochete · 6 toque múltiplo
    int sticking = 0;      // 0 nenhuma · 1 direita · 2 esquerda
};
std::string musicXmlForDrumStaff(const std::vector<NotatedBar>& hands,
                                 const std::vector<NotatedBar>& feet,
                                 const std::vector<DrumHit>& hits,
                                 const NotationParameters& notation,
                                 const MusicXmlParameters& xml,
                                 const char* rightHand = "D",
                                 const char* leftHand = "E");

} // namespace batruc

// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#pragma once

#include "core/CompositionCore.hpp"

#include <cstdint>

// ORNAMENTOS DA CAIXA (12/11/2026; o autor: "faltam os ornamentos, rufos,
// ricochetes e outros elementos que são característicos da caixa"; "avance").
// Princípio da reordenação: a geração ESCREVE — cada ornamento vira ataques
// na célula, visíveis e editáveis no Study, com desfazer.
//
//   fantasmas — toques fracos (15–30 % do acento) em semicolcheias vazias,
//               fora dos tempos;
//   flam      — uma nota de enfeite ~25 ms antes de um acento;
//   drag      — duas notas rápidas (~60 e ~30 ms) antes da principal;
//   ruff      — três notas (~75, ~50, ~25 ms) antes da principal (rufo);
//   ricochete — depois do golpe, 3–8 rebotes que aceleram e perdem força
//               (a baqueta quicando: intervalo × 0,8 e força × 0,6 a cada
//               rebote; analogia com o quique, não simulação da baqueta);
//   rulo      — fusas contínuas do golpe até o próximo ataque (até um
//               tempo), com crescendo;
//   aro       — a nota leva a articulação de aro (GM 37, cross-stick).
// `amount` (0–1) é a proporção de ataques que recebe o ornamento; a escolha
// é determinística pela seed. Ataques que não cabem (limite da célula ou
// distância mínima) são pulados, nunca deslocam os existentes.
namespace batruc {

enum class SnareOrnament : std::uint8_t {
    ghostNotes, flam, drag, ruff, ricochet, roll, rim,
    buzz   // toque múltiplo (13/11/2026): rebotes rápidos numa nota só (PAS #4 curto)
};
constexpr int kSnareOrnamentCount = 8;

struct SnareOrnamentParameters {
    SnareOrnament kind = SnareOrnament::flam;
    double sectionSeconds = 8.0;
    int stepsPerSection = 64;     // semicolcheias na seção (4 compassos de 16)
    float amount = 0.5f;
    std::uint32_t seed = 0;
};

// Devolve quantos ataques foram acrescentados ou alterados.
int ornamentSnareCell(RhythmicCell& cell, const SnareOrnamentParameters& parameters) noexcept;

// ORNAMENTO COMO PROPRIEDADE DA NOTA (13/11/2026; o autor: "ficou muitos
// toques no gráfico … mantém uma só marcação (com algo que sinaliza que há
// um rufo ou outro ornamento)"; ESTUDO_ORNAMENTOS_CAIXA.md §6, etapa 2).
// Flam, drag, rufo, ricochete e rulo MARCAM a nota escrita
// (`cell.ornaments[i] = tipo + 1`), sem acrescentar toques; notas fantasmas
// (notas de verdade, fracas) e aro (articulação da própria nota) continuam
// escrevendo na célula como em `ornamentSnareCell`, e desde 13/11/2026 ficam
// MARCADOS (fantasma = nota acrescentada; aro = nota que virou GM 37), para
// o ORN do stem poder tirá-los.
int markSnareOrnaments(RhythmicCell& cell, const SnareOrnamentParameters& parameters) noexcept;

// Célula TOCADA a partir da escrita: os toques de cada nota marcada nascem
// aqui (mesmas figuras de `ornamentSnareCell`, uma nota de cada vez); a
// marca não segue para a célula tocada. Sem marcas, `performed == written`.
// `enabled` = false (ORN desligado no stem, 13/11/2026): a batida normal —
// sem enfeites, sem as notas fantasmas marcadas e com o aro de volta ao
// toque normal; as marcas continuam na célula escrita.
void expandSnareOrnaments(const RhythmicCell& written, RhythmicCell& performed,
                          double sectionSeconds, int stepsPerSection,
                          bool enabled = true) noexcept;

// O ornamento marcado numa nota (ou nullopt-like: false se não houver).
bool cellOrnament(const RhythmicCell& cell, std::size_t index, SnareOrnament& kind) noexcept;

const char* snareOrnamentName(SnareOrnament kind) noexcept;   // em português

// Vocabulário de caixa que a seed escolhe (gene "SNR1", 12/11/2026): 0 nenhum ·
// 1 fantasmas+flam · 2 fantasmas+drag · 3 ricochete · 4 rulo · 5 rufo+aro.
// Devolve o índice; `kinds` recebe os dois ornamentos (iguais = um só) e
// `amount` a quantidade (25–60 %). Usado pelo app e pelo render offline.
int seedSnareVocabulary(std::uint32_t seed, SnareOrnament (&kinds)[2], float& amount) noexcept;

// IDIOMA DA CAIXA POR GÊNERO (13/11/2026; ESTUDO_ORNAMENTOS_CAIXA.md §4b,
// a partir de Galvão, Per Musi 2023, CC BY 4.0). Só o que a fonte documenta:
//   frevo    — rulo de 1 tempo no último tempo de cada frase de 2 compassos,
//              acentuado (Saldanha 2008, p. 184, citado por Galvão);
//   maracatu — em cada tempo, a 2ª nota da caixa ganha toque múltiplo e
//              acento (padrão do tarol do baque virado);
//   baião    — acento na 2ª colcheia do 2º tempo de cada compasso e notas
//              fantasmas pontuando as síncopes.
// Escreve na célula (notas e marcas), como os outros ornamentos; o ORN do
// stem continua tirando os enfeites. Determinístico pela seed.
enum class SnareIdiom : std::uint8_t { frevo, maracatu, baiao };
constexpr int kSnareIdiomCount = 3;
const char* snareIdiomName(SnareIdiom idiom) noexcept;
int applySnareIdiom(RhythmicCell& cell, SnareIdiom idiom, int beatsPerBar, int beatSteps,
                    int bars, std::uint32_t seed) noexcept;

// MICROTEMPO MEDIDO (13/11/2026; ESTUDO_ORNAMENTOS_CAIXA.md §4c): Davies et
// al., ISMIR 2020 (CC BY 4.0), maracatu de baque solto. Perfis medianos das
// semicolcheias dentro do tempo (fração do tempo; a grade reta é 0,25 · 0,5 ·
// 0,75): tarol [0,25 · 0,47 · 0,715]; bombo agudo 3ª e 4ª [0,46 · 0,69].
// A operação ESCREVE o desvio nas posições (visível, com desfazer); só move
// ataques que estão na grade de semicolcheias (±30 % de passo). `depth` 0–1
// (1 = a mediana medida). Sem oscilação: o artigo não dá números para ela.
enum class MicrotimingProfile : std::uint8_t { tarolBaqueSolto, bomboBaqueSolto };
constexpr int kMicrotimingProfileCount = 2;
const char* microtimingProfileName(MicrotimingProfile profile) noexcept;
int applyMicrotimingProfile(RhythmicCell& cell, MicrotimingProfile profile,
                            int beatsPerSection, float depth) noexcept;

} // namespace batruc

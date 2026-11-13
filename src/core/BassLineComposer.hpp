// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#pragma once

#include "core/BassComposition.hpp"
#include "core/FigureComposer.hpp"

#include <cstdint>
#include <string>

// BAIXO COM VOZ PRÓPRIA (12/11/2026; AUDITORIA_MUSICAL_2026-11-12.md §7,
// item 5: "frases com contorno e harmonia (graus, notas de passagem,
// resposta nas lacunas), não só a derivação da agenda"). A geração escreve:
// o resultado é uma `BassPerformance` comum, com um evento por nota e o
// ciclo formal de cada uma, consumida pelo mesmo renderizador de baixo.
//
//   HARMONIA — a seed escolhe uma progressão de graus, um acorde por
//     compasso, no tom da seed (tônica × modo; o mesmo gene "KEY1" do app).
//     O acorde é a tríade empilhada na própria escala do modo (nas
//     pentatônicas e no blues, empilhamento na escala de 5–6 notas).
//   RITMO — o primeiro tempo de cada compasso leva a fundamental; parte
//     dos ataques do bumbo (V1) recebe o baixo junto (APOIO); nas lacunas
//     longas do bumbo o baixo responde no contratempo (RESPOSTA).
//   ALTURAS — tempo forte: fundamental ou quinta; demais: notas do acorde,
//     escolhidas perto da nota anterior (contorno conjunto); a última nota
//     antes de uma troca de acorde é uma nota de PASSAGEM, vizinha de
//     escala da próxima fundamental.
//   FORMA — os 7 ciclos seguem as frases do FigureComposer (o bumbo de cada
//     ciclo); o ciclo 3 respira (só tempos fortes), o ciclo 4 troca um acorde
//     pelo relativo, o ciclo 7 termina na dominante para voltar ao início.
// Determinístico: mesma seed e mesmas figuras, mesmo baixo.
namespace batruc {

struct BassLineParameters {
    std::uint32_t seed = 0;
    double sectionSeconds = 8.0;
    int barsPerSection = 4;
    int stepsPerBar = 16;
    int keyTonic = 0;     // 0–11
    int keyScale = 0;     // índice em kBassKeyScales
};

// tom da seed da composição — a mesma derivação do app (gene "KEY1")
void seedKey(std::uint32_t compositionSeed, int& tonic, int& scale) noexcept;

bool composeBassLine(const FigureComposition& figures,
                     const BassLineParameters& parameters,
                     BassPerformance& performance,
                     std::string* summary = nullptr);

} // namespace batruc

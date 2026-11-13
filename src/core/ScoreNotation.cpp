// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#include "core/ScoreNotation.hpp"

#include <algorithm>
#include <array>
#include <map>
#include <sstream>

namespace batruc {
namespace {

// grafia por sustenido e por bemol (MARAVI `SHARP_STEPS`/`FLAT_STEPS`):
// grau 0–6 (dó…si) e alteração por classe de altura
constexpr std::array<int, 12> kSharpDegree = {0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6};
constexpr std::array<int, 12> kSharpAlter = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0};
constexpr std::array<int, 12> kFlatDegree = {0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6};
constexpr std::array<int, 12> kFlatAlter = {0, -1, 0, -1, 0, 0, -1, 0, -1, 0, -1, 0};

// ordem dos acidentes da armadura, em graus (fá dó sol ré lá mi si)
constexpr std::array<int, 7> kSharpOrder = {3, 0, 4, 1, 5, 2, 6};
constexpr std::array<int, 7> kFlatOrder = {6, 2, 5, 1, 4, 0, 3};

// figuras possíveis em semicolcheias: (duração, figura, ponto)
struct Figure { int steps; int figure; bool dotted; };
constexpr std::array<Figure, 8> kFigures = {{
    {16, 1, false}, {12, 2, true}, {8, 2, false}, {6, 4, true},
    {4, 4, false}, {3, 8, true}, {2, 8, false}, {1, 16, false}}};

} // namespace

int keyFifthsForMode(const int tonic, const int scale) noexcept {
    if (tonic < 0 || tonic > 11) return 0;
    // distância da tônica do modo à tônica do maior relativo
    int toMajor = 0;
    switch (scale) {
        case 0: case 7: toMajor = 0; break;          // maior, pentatônica maior
        case 1: case 6: case 8: case 9: toMajor = 3; break;   // menores e blues
        case 2: toMajor = -2; break;                 // dórico (ré → dó)
        case 3: toMajor = 5; break;                  // mixolídio (sol → dó)
        case 4: toMajor = -4; break;                 // frígio (mi → dó)
        case 5: toMajor = 7; break;                  // lídio (fá → dó)
        default: toMajor = 0; break;
    }
    const int major = ((tonic + toMajor) % 12 + 12) % 12;
    // classe → quintas; Fá♯ (6 ♯) em vez de Sol♭, Ré♭ (5 ♭) em vez de Dó♯
    static constexpr std::array<int, 12> kFifths = {0, -5, 2, -3, 4, -1, 6, 1, -4, 3, -2, 5};
    return kFifths[static_cast<std::size_t>(major)];
}

int keySignatureAlter(const int degree, const int keyFifths) noexcept {
    const int d = ((degree % 7) + 7) % 7;
    if (keyFifths > 0) {
        for (int i = 0; i < std::min(keyFifths, 7); ++i)
            if (kSharpOrder[static_cast<std::size_t>(i)] == d) return 1;
    } else if (keyFifths < 0) {
        for (int i = 0; i < std::min(-keyFifths, 7); ++i)
            if (kFlatOrder[static_cast<std::size_t>(i)] == d) return -1;
    }
    return 0;
}

void spellMidi(const int midi, const int keyFifths, int& diatonic, int& alter) noexcept {
    const int pc = ((midi % 12) + 12) % 12;
    const int octave = midi / 12 - 1;
    const bool flats = keyFifths < 0;
    const int degree = flats ? kFlatDegree[static_cast<std::size_t>(pc)]
                             : kSharpDegree[static_cast<std::size_t>(pc)];
    alter = flats ? kFlatAlter[static_cast<std::size_t>(pc)]
                  : kSharpAlter[static_cast<std::size_t>(pc)];
    // a altura natural do grau que a armadura altera (ex.: si em Fá maior)
    // é escrita com bequadro; o grau continua o mesmo
    diatonic = octave * 7 + degree;
}

bool notateLine(std::vector<LineNote> notes, const NotationParameters& p,
                std::vector<NotatedBar>& bars) {
    bars.clear();
    if (p.beatsPerBar < 1 || p.beatsPerBar > 16 || (p.beatUnit != 4 && p.beatUnit != 8)
        || p.bars < 1 || p.bars > 16 || p.keyFifths < -7 || p.keyFifths > 7)
        return false;
    const int beatSteps = p.beatUnit == 4 ? 4 : 2;
    const int barSteps = p.beatsPerBar * beatSteps;
    const int total = barSteps * p.bars;

    // monofonia: ordem, recorte ao trecho, sobreposição encurtada
    std::stable_sort(notes.begin(), notes.end(),
        [](const LineNote& a, const LineNote& b) { return a.startStep < b.startStep; });
    std::vector<LineNote> line;
    for (auto note : notes) {
        note.startStep = std::clamp(note.startStep, 0, total);
        note.endStep = std::clamp(note.endStep, note.startStep + 1, total);
        if (note.startStep >= total) continue;
        if (!line.empty() && line.back().startStep == note.startStep) continue;   // mesma posição: fica a primeira
        if (!line.empty() && line.back().endStep > note.startStep)
            line.back().endStep = note.startStep;
        line.push_back(note);
    }

    bars.assign(static_cast<std::size_t>(p.bars), {});
    // limite de cada fatia: barra de compasso; fora do tempo, o próximo
    // tempo; em compasso par, a metade só é cruzada por quem começa no 1
    auto capAt = [&](const int position, const int remaining) {
        const int inBar = position % barSteps;
        int cap = std::min(remaining, barSteps - inBar);
        if (inBar % beatSteps != 0)
            cap = std::min(cap, beatSteps - inBar % beatSteps);
        else if (p.beatsPerBar % 2 == 0 && p.beatsPerBar >= 4 && inBar != 0
                 && inBar < barSteps / 2)
            cap = std::min(cap, barSteps / 2 - inBar);
        return std::max(cap, 1);
    };
    auto emit = [&](const bool rest, const int start, const int length,
                    const LineNote* source) {
        int position = start;
        int remaining = length;
        bool first = true;
        while (remaining > 0) {
            const int cap = capAt(position, remaining);
            Figure chosen = kFigures.back();
            for (const auto& figure : kFigures)
                if (figure.steps <= cap
                    // um ponto só quando o resto da duração o justifica
                    && (!figure.dotted || figure.steps <= remaining)) {
                    chosen = figure;
                    break;
                }
            NotatedItem item;
            item.rest = rest;
            item.startStep = position % barSteps;
            item.steps = chosen.steps;
            item.figure = chosen.figure;
            item.dotted = chosen.dotted;
            if (!rest && source != nullptr) {
                item.midi = source->midi;
                item.velocity = source->velocity;
                spellMidi(source->midi, p.keyFifths, item.diatonic, item.alter);
                item.tieFromPrevious = !first;
                item.tieToNext = remaining - chosen.steps > 0;
            }
            bars[static_cast<std::size_t>(position / barSteps)].items.push_back(item);
            position += chosen.steps;
            remaining -= chosen.steps;
            first = false;
        }
    };
    int cursor = 0;
    for (const auto& note : line) {
        if (note.startStep > cursor)
            emit(true, cursor, note.startStep - cursor, nullptr);
        emit(false, note.startStep, note.endStep - note.startStep, &note);
        cursor = note.endStep;
    }
    if (cursor < total)
        emit(true, cursor, total - cursor, nullptr);

    // acidentes: comparados à armadura e ao que já foi escrito no compasso
    // para o mesmo grau e oitava; nota ligada não repete o acidente
    for (auto& bar : bars) {
        std::map<int, int> current;   // diatônico → alteração em vigor
        for (auto& item : bar.items) {
            if (item.rest) continue;
            const auto found = current.find(item.diatonic);
            const int inForce = found != current.end() ? found->second
                : keySignatureAlter(item.diatonic % 7, p.keyFifths);
            item.showAccidental = !item.tieFromPrevious && item.alter != inForce;
            current[item.diatonic] = item.alter;
        }
    }
    return true;
}

namespace {
std::string xmlEscape(const std::string& text) {
    std::string out;
    for (const char c : text) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            default: out += c;
        }
    }
    return out;
}
const char* figureName(const int figure) {
    switch (figure) {
        case 1: return "whole";
        case 2: return "half";
        case 4: return "quarter";
        case 8: return "eighth";
        default: return "16th";
    }
}
} // namespace

std::string musicXmlForBars(const std::vector<NotatedBar>& bars,
                            const NotationParameters& n,
                            const MusicXmlParameters& x) {
    static constexpr char kSteps[7] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
    std::ostringstream out;
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        << "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 4.0 Partwise//EN\" "
           "\"http://www.musicxml.org/dtds/partwise.dtd\">\n"
        << "<score-partwise version=\"4.0\">\n";
    if (!x.title.empty())
        out << "<work><work-title>" << xmlEscape(x.title) << "</work-title></work>\n";
    out << "<identification><encoding><software>Batruc (RASGO)</software></encoding></identification>\n"
        << "<part-list><score-part id=\"P1\"><part-name>" << xmlEscape(x.partName)
        << "</part-name></score-part></part-list>\n<part id=\"P1\">\n";
    for (std::size_t b = 0; b < bars.size(); ++b) {
        out << "<measure number=\"" << (b + 1) << "\">\n";
        if (b == 0) {
            // divisions = 4: a semicolcheia é a unidade
            out << "<attributes><divisions>4</divisions><key><fifths>" << n.keyFifths
                << "</fifths></key><time><beats>" << n.beatsPerBar << "</beats><beat-type>"
                << n.beatUnit << "</beat-type></time><clef><sign>F</sign><line>4</line></clef>";
            if (x.octaveTranspose != 0)
                out << "<transpose><diatonic>0</diatonic><chromatic>0</chromatic><octave-change>"
                    << x.octaveTranspose << "</octave-change></transpose>";
            out << "</attributes>\n";
            out << "<direction placement=\"above\"><direction-type><metronome>"
                   "<beat-unit>quarter</beat-unit><per-minute>"
                << static_cast<int>(x.bpm + 0.5) << "</per-minute></metronome></direction-type>"
                << "<sound tempo=\"" << static_cast<int>(x.bpm + 0.5) << "\"/></direction>\n";
        }
        for (const auto& item : bars[b].items) {
            out << "<note>";
            if (item.rest) {
                out << "<rest/>";
            } else {
                const int degree = ((item.diatonic % 7) + 7) % 7;
                const int octave = item.diatonic >= 0 ? item.diatonic / 7 : (item.diatonic - 6) / 7;
                out << "<pitch><step>" << kSteps[degree] << "</step>";
                if (item.alter != 0) out << "<alter>" << item.alter << "</alter>";
                out << "<octave>" << octave << "</octave></pitch>";
            }
            out << "<duration>" << item.steps << "</duration>";
            if (!item.rest) {
                if (item.tieFromPrevious) out << "<tie type=\"stop\"/>";
                if (item.tieToNext) out << "<tie type=\"start\"/>";
            }
            out << "<voice>1</voice><type>" << figureName(item.figure) << "</type>";
            if (item.dotted) out << "<dot/>";
            if (!item.rest && item.showAccidental)
                out << "<accidental>" << (item.alter > 0 ? "sharp" : item.alter < 0 ? "flat" : "natural")
                    << "</accidental>";
            if (!item.rest && (item.tieFromPrevious || item.tieToNext)) {
                out << "<notations>";
                if (item.tieFromPrevious) out << "<tied type=\"stop\"/>";
                if (item.tieToNext) out << "<tied type=\"start\"/>";
                out << "</notations>";
            }
            out << "</note>\n";
        }
        if (b + 1 == bars.size())
            out << "<barline location=\"right\"><bar-style>light-heavy</bar-style></barline>\n";
        out << "</measure>\n";
    }
    out << "</part>\n</score-partwise>\n";
    return out.str();
}

std::string musicXmlForDrumStaff(const std::vector<NotatedBar>& hands,
                                 const std::vector<NotatedBar>& feet,
                                 const std::vector<DrumHit>& hits,
                                 const NotationParameters& n,
                                 const MusicXmlParameters& x,
                                 const char* rightHand, const char* leftHand) {
    static constexpr char kSteps[7] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
    const int beatSteps = n.beatUnit == 4 ? 4 : 2;
    const int barSteps = n.beatsPerBar * beatSteps;
    std::ostringstream out;
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        << "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 4.0 Partwise//EN\" "
           "\"http://www.musicxml.org/dtds/partwise.dtd\">\n"
        << "<score-partwise version=\"4.0\">\n";
    if (!x.title.empty())
        out << "<work><work-title>" << xmlEscape(x.title) << "</work-title></work>\n";
    out << "<identification><encoding><software>Batruc (RASGO)</software></encoding></identification>\n"
        << "<part-list><score-part id=\"P1\"><part-name>" << xmlEscape(x.partName)
        << "</part-name></score-part></part-list>\n<part id=\"P1\">\n";
    const auto writeHead = [&](const DrumHit& h, const NotatedItem& item, const int voice,
                               const bool chord, const bool grace, const int graceIndex) {
        out << "<note>";
        if (grace) out << "<grace" << (graceIndex < 0 ? " slash=\"yes\"" : "") << "/>";
        if (chord) out << "<chord/>";
        out << "<unpitched><display-step>" << kSteps[((h.place % 7) + 7) % 7]
            << "</display-step><display-octave>" << h.place / 7 << "</display-octave></unpitched>";
        if (!grace) out << "<duration>" << item.steps << "</duration>";
        out << "<voice>" << voice << "</voice><type>"
            << (grace ? (graceIndex < 0 ? "eighth" : "16th") : figureName(item.figure)) << "</type>";
        if (!grace && item.dotted) out << "<dot/>";
        out << "<stem>" << (voice == 1 ? "up" : "down") << "</stem>";
        if (h.cross) out << "<notehead>x</notehead>";
        else if (h.ghost && !grace) out << "<notehead parentheses=\"yes\">normal</notehead>";
        if (!grace && (h.accent || h.ornament == 4 || h.ornament == 6)) {
            out << "<notations>";
            if (h.accent) out << "<articulations><accent/></articulations>";
            if (h.ornament == 4) out << "<ornaments><tremolo type=\"single\">3</tremolo></ornaments>";
            if (h.ornament == 6)   // toque múltiplo: trêmulo não medido (buzz)
                out << "<ornaments><tremolo type=\"unmeasured\">0</tremolo></ornaments>";
            out << "</notations>";
        }
        if (!grace && !chord && h.sticking != 0)
            out << "<lyric number=\"1\"><text>" << (h.sticking == 1 ? rightHand : leftHand)
                << "</text></lyric>";
        out << "</note>\n";
    };
    const auto writeVoice = [&](const NotatedBar& bar, const int barIndex, const bool feetVoice) {
        const int voice = feetVoice ? 2 : 1;
        for (const auto& item : bar.items) {
            const int absolute = barIndex * barSteps + item.startStep;
            std::vector<const DrumHit*> chord;
            if (!item.rest && !item.tieFromPrevious)
                for (const auto& h : hits)
                    if (h.step == absolute && h.feet == feetVoice) chord.push_back(&h);
            if (chord.empty()) {
                out << "<note><rest/><duration>" << item.steps << "</duration><voice>" << voice
                    << "</voice><type>" << figureName(item.figure) << "</type>";
                if (item.dotted) out << "<dot/>";
                out << "</note>\n";
                continue;
            }
            // enfeites (flam/drag/rufo) da primeira nota do acorde que os tiver
            for (const auto* h : chord) {
                if (h->ornament < 1 || h->ornament > 3) continue;
                const int graces = h->ornament;   // 1 flam · 2 drag · 3 rufo
                for (int g = 0; g < graces; ++g)
                    writeHead(*h, item, voice, false, true, graces == 1 ? -1 : g);
                break;
            }
            for (std::size_t c = 0; c < chord.size(); ++c)
                writeHead(*chord[c], item, voice, c > 0, false, 0);
        }
    };
    const std::size_t barCount = std::max(hands.size(), feet.size());
    for (std::size_t b = 0; b < barCount; ++b) {
        out << "<measure number=\"" << (b + 1) << "\">\n";
        if (b == 0) {
            out << "<attributes><divisions>4</divisions><key><fifths>0</fifths></key><time><beats>"
                << n.beatsPerBar << "</beats><beat-type>" << n.beatUnit
                << "</beat-type></time><clef><sign>percussion</sign></clef></attributes>\n"
                << "<direction placement=\"above\"><direction-type><metronome>"
                   "<beat-unit>quarter</beat-unit><per-minute>" << static_cast<int>(x.bpm + 0.5)
                << "</per-minute></metronome></direction-type><sound tempo=\""
                << static_cast<int>(x.bpm + 0.5) << "\"/></direction>\n";
        }
        if (b < hands.size()) writeVoice(hands[b], static_cast<int>(b), false);
        if (b < feet.size()) {
            out << "<backup><duration>" << barSteps << "</duration></backup>\n";
            writeVoice(feet[b], static_cast<int>(b), true);
        }
        if (b + 1 == barCount)
            out << "<barline location=\"right\"><bar-style>light-heavy</bar-style></barline>\n";
        out << "</measure>\n";
    }
    out << "</part>\n</score-partwise>\n";
    return out.str();
}

} // namespace batruc

#!/usr/bin/env python3
"""Gera as quatro páginas do site do Batruc (index.html em português, a
canônica, e en/fr/es.html) a partir de um modelo único e dos textos de cada
idioma. Estrutura copiada do site do Trioio (13/11/2026); textos próprios.

    python3 gerar_paginas.py && python3 seo.py

Editar os TEXTOS aqui, não nos .html (eles são derivados).

Desde 14/11/2026 gera também Tutorial, LEARN e Instalação (tutorial*.html,
learn*.html, instalacao*.html), com o menu do cabeçalho no padrão do site do
Asterismus. Textos dessas páginas em textos_paginas.py. O LEARN vem do app:

    ./run --export-learn=website/learn_textos.json   (na raiz do projeto)

PUBLICADO = False enquanto não houver release: a página diz "em preparação",
leva noindex e não mostra a seção Baixar (links quebrados não vão ao ar).
"""

import html
import json
import pathlib

from textos_paginas import NAV, TUTORIAL, LEARN, INSTALACAO, IMAGENS_TUTORIAL, IMAGENS_LEARN, PASTA_IMAGENS, ALT

AQUI = pathlib.Path(__file__).parent
PORTAL = "https://rasgoinstruments.arquiviagem.net/"
REPO = "https://github.com/lucioaraujo/batruc"
BLOB = "https://github.com/lucioaraujo/batruc/blob/main"
PUBLICADO = True
VERSAO = "0.1.5"
RELEASE = "https://github.com/lucioaraujo/batruc/releases/tag/v" + VERSAO
DL = "https://github.com/lucioaraujo/batruc/releases/download/v" + VERSAO + "/"
# nomes amarrados ao CPACK_PACKAGE_FILE_NAME do CMakeLists (batruc-<versão>-<plataforma>)
PACOTES = [
    ("Windows", [("batruc-%s-windows-x64.exe" % VERSAO, "exe"), ("batruc-%s-windows-x64.zip" % VERSAO, "zip")]),
    ("macOS", [("batruc-%s-macos-universal.dmg" % VERSAO, "dmg")]),
    ("Linux", [("batruc-%s-linux-x86_64.deb" % VERSAO, "deb"), ("batruc-%s-linux-x86_64.AppImage" % VERSAO, "AppImage"),
               ("batruc-%s-linux-x86_64.tar.gz" % VERSAO, "tar.gz")]),
]

PAGINAS = [("index.html", "pt-BR", "PT"), ("en.html", "en", "EN"), ("fr.html", "fr", "FR"), ("es.html", "es", "ES")]

T = {
    "pt-BR": {
        "title": "Batruc — ritmo escrito por matemática e física",
        "desc": "Batruc: instrumento de composição rítmica da família RASGO. Pêndulos acoplados, bilhar, Lissajous e Fourier escrevem ritmos editáveis; caixa com ornamentos, baixo com voz própria, pauta e MusicXML.",
        "strip": "RASGO — família de instrumentos e pesquisa sonora",
        "skip": "Pular para o conteúdo",
        "langnav": "Idioma",
        "pill_prep": "Versão 0.1.0 — em preparação",
        "pill_pub": "Versão 0.1.2 — para Windows, macOS e Linux",
        "lead": "Batruc é um instrumento de composição rítmica. Modelos matemáticos e físicos — pêndulos acoplados, bilhar, figuras de Lissajous, séries de Fourier, curvas rolantes — escrevem ritmos que você ouve, edita, toca e leva para a partitura. A geração escreve; ela não filtra às escondidas.",
        "s_shot": "A interface",
        "shot_alt": "O Batruc em execução: o editor de ciclos com o estudo circular das vozes, o gráfico de onda e fluxo, e o painel de células.",
        "shot_cap": "A tela Compor: o editor de ciclos (Study), a vista vinculada de onda e fluxo, e as células de cada voz. Em quatro idiomas.",
        "s1": "Como funciona",
        "steps": [
            ("Escrever", "Cada seed escreve figuras para bumbo, caixa e chimbal a partir de um modelo físico, com forma de frase, rupturas e desenvolvimento em sete ciclos. Tudo fica editável no Study."),
            ("Tocar", "No modo RITMO toca exatamente o que está escrito; no GERATIVO entram as máscaras de campo, as leis de intensidade e a forma, com profundidade ajustável."),
            ("Ornamentar", "A caixa guarda os ornamentos na nota — fantasmas, flam, drag, rufo, ricochete, rulo, aro, toque múltiplo —, e o botão ORN de cada stem alterna entre a batida normal e a caixa completa."),
            ("Notar", "Uma pauta do baixo, em clave de Fá e no tom da seed, e uma pauta de bateria com sinais de ornamento e manulação, as duas exportáveis em MusicXML."),
        ],
        "s2": "Baixo e tom",
        "bass": "A seed escolhe um tom — doze tônicas e dez modos — e o baixo ganha voz própria: uma progressão de graus, apoio a parte dos ataques do bumbo, respostas nas lacunas e notas de passagem antes de cada troca de acorde. A percussão também pode seguir o tom.",
        "s3": "Ritmos e pesquisa",
        "research": "O Batruc trabalha com fontes citadas, não com rótulos prontos:",
        "facts": [
            "um catálogo de 1.150 performances de bateria do Groove MIDI Dataset (Google, CC BY 4.0), com origem, licença e atribuição guardadas em cada célula derivada; os rótulos de estilo descrevem performances, não definem tradições;",
            "os ornamentos de caixa partem do vocabulário dos 40 rudimentos internacionais da Percussive Arts Society, descritos com palavras próprias;",
            "a ginga da caixa no frevo, no maracatu e no baião usa só o que o estudo de Christiano Galvão (Per Musi, 2023) documenta;",
            "o microtempo do tarol vem de medições publicadas no maracatu de baque solto (ISMIR 2020).",
        ],
        "s4": "Créditos",
        "credits": "Lúcio de Araújo — concepção, composição, interface e código. Licença GNU AGPLv3 ou posterior; JUCE sob a via AGPLv3. Fontes e créditos completos:",
        "credits_link": "CREDITS_AND_SOURCES.md",
        "s5": "Estado",
        "state_prep": "A versão 0.1.0 está em preparação. O Batruc foi compilado, testado e tocado no Linux; os pacotes de Windows e macOS são construídos pela integração contínua e ainda não foram abertos em máquinas reais. O instrumento segue em desenvolvimento, aberto.",
        "state_pub": "A versão 0.1.2 é a atual; a primeira publicada foi a 0.1.0, em novembro de 2026. O Batruc foi compilado, testado e tocado no Linux; os pacotes de Windows e macOS são construídos e testados pela integração contínua e ainda não foram abertos em máquinas reais. O instrumento segue em desenvolvimento, aberto.",
        "contact": "Contato",
        "footer_credits": "Créditos",
        "footer_license": "Licença AGPLv3+",
        "dialog_title": "CONTATO",
        "dialog_copy_hint": "Copie o endereço de e-mail:",
        "dialog_copy": "Copiar e-mail",
        "dialog_close": "Fechar janela de contato",
        "copied": "E-mail copiado.",
        "fallback": "Selecione e copie o endereço acima.",
        "s_dl": "Baixar",
        "dl_intro": "Escolha o pacote do seu sistema.",
        "dl_os": {"Windows": "Windows 10 ou 11 — instalador (.exe) ou sem instalar (.zip)", "macOS": "macOS 10.15 ou mais novo, Intel e Apple Silicon", "Linux": ".deb para Ubuntu, Debian e Mint; AppImage ou .tar.gz para as outras distribuições"},
        "dl_warn": "Na primeira abertura, o Windows e o macOS mostram um aviso de segurança: é esperado, porque o app não tem a assinatura paga das duas empresas. O guia de instalação diz onde clicar.",
        "dl_install": "Guia de instalação",
        "dl_all": "Todos os pacotes e as notas da versão",
    },
    "en": {
        "title": "Batruc — rhythm written by mathematics and physics",
        "desc": "Batruc: a rhythm-composition instrument of the RASGO family. Coupled pendulums, billiard, Lissajous and Fourier write editable rhythms; a snare with ornaments, a bass with its own voice, staves and MusicXML.",
        "strip": "RASGO — a family of instruments and sound research",
        "skip": "Skip to content",
        "langnav": "Language",
        "pill_prep": "Version 0.1.0 — in preparation",
        "pill_pub": "Version 0.1.2 — for Windows, macOS and Linux",
        "lead": "Batruc is a rhythm-composition instrument. Mathematical and physical models — coupled pendulums, billiard, Lissajous figures, Fourier series, rolling curves — write rhythms that you hear, edit, play and take to the score. Generation writes; it does not filter behind your back.",
        "s_shot": "The interface",
        "shot_alt": "Batruc running: the cycle editor with the circular study of the voices, the wave and flow graph, and the cell panel.",
        "shot_cap": "The Compose screen: the cycle editor (Study), the linked wave and flow view, and each voice's cells. In four languages.",
        "s1": "How it works",
        "steps": [
            ("Write", "Each seed writes figures for kick, snare and hi-hat from a physical model, with phrase form, ruptures and development across seven cycles. Everything stays editable in the Study."),
            ("Play", "RHYTHM mode plays exactly what is written; GENERATIVE adds the field masks, intensity laws and form, at an adjustable depth."),
            ("Ornament", "The snare keeps its ornaments on the note — ghost notes, flam, drag, ruff, ricochet, roll, rim, buzz —, and each stem's ORN switch toggles between the plain beat and the full snare."),
            ("Notate", "A bass staff, in bass clef and in the seed's key, and a drum staff with ornament signs and sticking, both exportable as MusicXML."),
        ],
        "s2": "Bass and key",
        "bass": "The seed chooses a key — twelve tonics and ten modes — and the bass gets a voice of its own: a degree progression, support for part of the kick hits, answers in the gaps and passing notes before each chord change. The percussion can follow the key too.",
        "s3": "Rhythms and research",
        "research": "Batruc works with cited sources, not ready-made labels:",
        "facts": [
            "a catalogue of 1,150 drum performances from the Groove MIDI Dataset (Google, CC BY 4.0), with origin, licence and attribution kept in every derived cell; style labels describe performances, they do not define traditions;",
            "the snare ornaments start from the vocabulary of the Percussive Arts Society's 40 international rudiments, described in Batruc's own words;",
            "the snare grooves of frevo, maracatu and baião use only what Christiano Galvão's study (Per Musi, 2023) documents;",
            "the tarol microtiming comes from published measurements of maracatu de baque solto (ISMIR 2020).",
        ],
        "s4": "Credits",
        "credits": "Lúcio de Araújo — concept, composition, interface and code. Licence GNU AGPLv3 or later; JUCE under its AGPLv3 option. Full sources and credits:",
        "credits_link": "CREDITS_AND_SOURCES.md",
        "s5": "Status",
        "state_prep": "Version 0.1.0 is in preparation. Batruc has been built, tested and played on Linux; the Windows and macOS packages are built by continuous integration and have not yet been opened on real machines. The instrument remains in development, open.",
        "state_pub": "Version 0.1.2 is the current one; the first release was 0.1.0, in November 2026. Batruc has been built, tested and played on Linux; the Windows and macOS packages are built and tested by continuous integration and have not yet been opened on real machines. The instrument remains in development, open.",
        "contact": "Contact",
        "footer_credits": "Credits",
        "footer_license": "AGPLv3+ licence",
        "dialog_title": "CONTACT",
        "dialog_copy_hint": "Copy the email address:",
        "dialog_copy": "Copy email",
        "dialog_close": "Close contact window",
        "copied": "Email copied.",
        "fallback": "Select and copy the address above.",
        "s_dl": "Download",
        "dl_intro": "Choose the package for your system.",
        "dl_os": {"Windows": "Windows 10 or 11 — installer (.exe) or no install (.zip)", "macOS": "macOS 10.15 or later, Intel and Apple Silicon", "Linux": ".deb for Ubuntu, Debian and Mint; AppImage or .tar.gz for other distributions"},
        "dl_warn": "On first launch, Windows and macOS show a security warning: this is expected, because the app does not carry the two companies' paid signature. The installation guide says where to click.",
        "dl_install": "Installation guide",
        "dl_all": "All packages and release notes",
    },
    "fr": {
        "title": "Batruc — le rythme écrit par les mathématiques et la physique",
        "desc": "Batruc : instrument de composition rythmique de la famille RASGO. Pendules couplés, billard, Lissajous et Fourier écrivent des rythmes modifiables ; caisse claire avec ornements, basse à voix propre, portées et MusicXML.",
        "strip": "RASGO — famille d'instruments et recherche sonore",
        "skip": "Aller au contenu",
        "langnav": "Langue",
        "pill_prep": "Version 0.1.0 — en préparation",
        "pill_pub": "Version 0.1.2 — pour Windows, macOS et Linux",
        "lead": "Batruc est un instrument de composition rythmique. Des modèles mathématiques et physiques — pendules couplés, billard, figures de Lissajous, séries de Fourier, courbes roulantes — écrivent des rythmes que vous écoutez, modifiez, jouez et portez sur la partition. La génération écrit ; elle ne filtre pas en cachette.",
        "s_shot": "L'interface",
        "shot_alt": "Batruc en cours d'exécution : l'éditeur de cycles avec l'étude circulaire des voix, le graphique onde et flux, et le panneau des cellules.",
        "shot_cap": "L'écran Composer : l'éditeur de cycles (Study), la vue liée onde et flux, et les cellules de chaque voix. En quatre langues.",
        "s1": "Fonctionnement",
        "steps": [
            ("Écrire", "Chaque graine écrit des figures de grosse caisse, caisse claire et charleston à partir d'un modèle physique, avec forme de phrase, ruptures et développement sur sept cycles. Tout reste modifiable dans le Study."),
            ("Jouer", "Le mode RYTHME joue exactement ce qui est écrit ; le mode GÉNÉRATIF ajoute masques de champ, lois d'intensité et forme, avec une profondeur réglable."),
            ("Ornementer", "La caisse claire garde ses ornements sur la note — notes fantômes, fla, ra, rufo, ricochet, roulement, cercle, buzz —, et le bouton ORN de chaque stem alterne entre le rythme simple et la caisse complète."),
            ("Noter", "Une portée de basse, en clé de fa et dans la tonalité de la graine, et une portée de batterie avec signes d'ornement et doigté, toutes deux exportables en MusicXML."),
        ],
        "s2": "Basse et tonalité",
        "bass": "La graine choisit une tonalité — douze toniques et dix modes — et la basse prend une voix propre : une progression de degrés, l'appui d'une partie des frappes de grosse caisse, des réponses dans les creux et des notes de passage avant chaque changement d'accord. La percussion peut aussi suivre la tonalité.",
        "s3": "Rythmes et recherche",
        "research": "Batruc travaille avec des sources citées, pas avec des étiquettes toutes faites :",
        "facts": [
            "un catalogue de 1 150 interprétations de batterie du Groove MIDI Dataset (Google, CC BY 4.0), avec origine, licence et attribution conservées dans chaque cellule dérivée ; les étiquettes de style décrivent des interprétations, elles ne définissent pas des traditions ;",
            "les ornements de caisse claire partent du vocabulaire des 40 rudiments internationaux de la Percussive Arts Society, décrits avec des mots propres ;",
            "les grooves de caisse claire du frevo, du maracatu et du baião n'utilisent que ce que documente l'étude de Christiano Galvão (Per Musi, 2023) ;",
            "le microtiming du tarol vient de mesures publiées sur le maracatu de baque solto (ISMIR 2020).",
        ],
        "s4": "Crédits",
        "credits": "Lúcio de Araújo — conception, composition, interface et code. Licence GNU AGPLv3 ou ultérieure ; JUCE sous sa licence AGPLv3. Sources et crédits complets :",
        "credits_link": "CREDITS_AND_SOURCES.md",
        "s5": "État",
        "state_prep": "La version 0.1.0 est en préparation. Batruc a été compilé, testé et joué sous Linux ; les paquets Windows et macOS sont construits par l'intégration continue et n'ont pas encore été ouverts sur de vraies machines. L'instrument reste en développement, ouvert.",
        "state_pub": "La version 0.1.2 est l'actuelle ; la première publiée fut la 0.1.0, en novembre 2026. Batruc a été compilé, testé et joué sous Linux ; les paquets Windows et macOS sont construits et testés par l'intégration continue et n'ont pas encore été ouverts sur de vraies machines. L'instrument reste en développement, ouvert.",
        "contact": "Contact",
        "footer_credits": "Crédits",
        "footer_license": "Licence AGPLv3+",
        "dialog_title": "CONTACT",
        "dialog_copy_hint": "Copiez l'adresse e-mail :",
        "dialog_copy": "Copier l'e-mail",
        "dialog_close": "Fermer la fenêtre de contact",
        "copied": "E-mail copié.",
        "fallback": "Sélectionnez et copiez l'adresse ci-dessus.",
        "s_dl": "Télécharger",
        "dl_intro": "Choisissez le paquet de votre système.",
        "dl_os": {"Windows": "Windows 10 ou 11 — installateur (.exe) ou sans installation (.zip)", "macOS": "macOS 10.15 ou plus récent, Intel et Apple Silicon", "Linux": ".deb pour Ubuntu, Debian et Mint ; AppImage ou .tar.gz pour les autres distributions"},
        "dl_warn": "À la première ouverture, Windows et macOS affichent un avertissement de sécurité : c'est attendu, car l'application ne porte pas la signature payante des deux entreprises. Le guide d'installation indique où cliquer.",
        "dl_install": "Guide d'installation",
        "dl_all": "Tous les paquets et les notes de version",
    },
    "es": {
        "title": "Batruc — ritmo escrito por matemática y física",
        "desc": "Batruc: instrumento de composición rítmica de la familia RASGO. Péndulos acoplados, billar, Lissajous y Fourier escriben ritmos editables; caja con ornamentos, bajo con voz propia, pentagramas y MusicXML.",
        "strip": "RASGO — familia de instrumentos e investigación sonora",
        "skip": "Saltar al contenido",
        "langnav": "Idioma",
        "pill_prep": "Versión 0.1.0 — en preparación",
        "pill_pub": "Versión 0.1.2 — para Windows, macOS y Linux",
        "lead": "Batruc es un instrumento de composición rítmica. Modelos matemáticos y físicos — péndulos acoplados, billar, figuras de Lissajous, series de Fourier, curvas rodantes — escriben ritmos que escuchas, editas, tocas y llevas a la partitura. La generación escribe; no filtra a escondidas.",
        "s_shot": "La interfaz",
        "shot_alt": "Batruc en ejecución: el editor de ciclos con el estudio circular de las voces, el gráfico de onda y flujo, y el panel de células.",
        "shot_cap": "La pantalla Componer: el editor de ciclos (Study), la vista vinculada de onda y flujo, y las células de cada voz. En cuatro idiomas.",
        "s1": "Cómo funciona",
        "steps": [
            ("Escribir", "Cada seed escribe figuras de bombo, caja y hi-hat a partir de un modelo físico, con forma de frase, rupturas y desarrollo en siete ciclos. Todo queda editable en el Study."),
            ("Tocar", "El modo RITMO toca exactamente lo escrito; el GENERATIVO añade las máscaras de campo, las leyes de intensidad y la forma, con profundidad ajustable."),
            ("Ornamentar", "La caja guarda los ornamentos en la nota — notas fantasma, flam, drag, ruff, ricochet, redoble, aro, toque múltiple —, y el botón ORN de cada stem alterna entre el ritmo normal y la caja completa."),
            ("Notar", "Un pentagrama de bajo, en clave de fa y en la tonalidad de la seed, y un pentagrama de batería con signos de ornamento y digitación, ambos exportables a MusicXML."),
        ],
        "s2": "Bajo y tonalidad",
        "bass": "La seed elige una tonalidad — doce tónicas y diez modos — y el bajo gana voz propia: una progresión de grados, apoyo a parte de los golpes de bombo, respuestas en los huecos y notas de paso antes de cada cambio de acorde. La percusión también puede seguir la tonalidad.",
        "s3": "Ritmos e investigación",
        "research": "Batruc trabaja con fuentes citadas, no con etiquetas hechas:",
        "facts": [
            "un catálogo de 1.150 interpretaciones de batería del Groove MIDI Dataset (Google, CC BY 4.0), con origen, licencia y atribución guardadas en cada célula derivada; las etiquetas de estilo describen interpretaciones, no definen tradiciones;",
            "los ornamentos de caja parten del vocabulario de los 40 rudimentos internacionales de la Percussive Arts Society, descritos con palabras propias;",
            "el groove de la caja en el frevo, el maracatu y el baião usa solo lo que documenta el estudio de Christiano Galvão (Per Musi, 2023);",
            "el microtiming del tarol viene de mediciones publicadas sobre el maracatu de baque solto (ISMIR 2020).",
        ],
        "s4": "Créditos",
        "credits": "Lúcio de Araújo — concepción, composición, interfaz y código. Licencia GNU AGPLv3 o posterior; JUCE bajo su vía AGPLv3. Fuentes y créditos completos:",
        "credits_link": "CREDITS_AND_SOURCES.md",
        "s5": "Estado",
        "state_prep": "La versión 0.1.0 está en preparación. Batruc se ha compilado, probado y tocado en Linux; los paquetes de Windows y macOS los construye la integración continua y todavía no se han abierto en máquinas reales. El instrumento sigue en desarrollo, abierto.",
        "state_pub": "La versión 0.1.2 es la actual; la primera publicada fue la 0.1.0, en noviembre de 2026. Batruc se ha compilado, probado y tocado en Linux; los paquetes de Windows y macOS los construye y prueba la integración continua y todavía no se han abierto en máquinas reales. El instrumento sigue en desarrollo, abierto.",
        "contact": "Contacto",
        "footer_credits": "Créditos",
        "footer_license": "Licencia AGPLv3+",
        "dialog_title": "CONTACTO",
        "dialog_copy_hint": "Copia la dirección de correo:",
        "dialog_copy": "Copiar correo",
        "dialog_close": "Cerrar ventana de contacto",
        "copied": "Correo copiado.",
        "fallback": "Selecciona y copia la dirección de arriba.",
        "s_dl": "Descargar",
        "dl_intro": "Elige el paquete de tu sistema.",
        "dl_os": {"Windows": "Windows 10 u 11 — instalador (.exe) o sin instalar (.zip)", "macOS": "macOS 10.15 o más reciente, Intel y Apple Silicon", "Linux": ".deb para Ubuntu, Debian y Mint; AppImage o .tar.gz para las demás distribuciones"},
        "dl_warn": "En la primera apertura, Windows y macOS muestran un aviso de seguridad: es lo esperado, porque la app no lleva la firma de pago de las dos empresas. La guía de instalación dice dónde hacer clic.",
        "dl_install": "Guía de instalación",
        "dl_all": "Todos los paquetes y las notas de la versión",
    },
}



def rodape(lang):
    t = T[lang]
    e = lambda x: html.escape(x, quote=False)
    a = lambda x: html.escape(x, quote=True)
    return f"""<footer class="site-footer">
  <span>© Lúcio de Araújo · Batruc 2026 · RASGO</span>
  <nav>
    <a href="{REPO}">GitHub</a> ·
    <a href="{BLOB}/CREDITS_AND_SOURCES.md">{e(t["footer_credits"])}</a> ·
    <a href="{BLOB}/LICENSE">{e(t["footer_license"])}</a>
  </nav>
</footer>
<dialog class="contact-dialog" data-contact-dialog aria-labelledby="contact-title">
  <button class="dialog-close" type="button" data-contact-close aria-label="{a(t["dialog_close"])}">×</button>
  <h2 id="contact-title">{e(t["dialog_title"])}</h2>
  <p>{e(t["dialog_copy_hint"])}</p>
  <code data-contact-address></code>
  <button class="button button-primary" type="button" data-contact-copy>{e(t["dialog_copy"])}</button>
  <p class="contact-feedback" data-contact-feedback data-copied="{a(t["copied"])}" data-fallback="{a(t["fallback"])}" aria-live="polite"></p>
</dialog>"""

def pagina(arquivo, lang):
    t = T[lang]
    e = lambda s: html.escape(s, quote=False)
    a = lambda s: html.escape(s, quote=True)
    passos = "\n".join('    <li><h3>%s</h3><p>%s</p></li>' % (e(h), e(p)) for h, p in t["steps"])
    fatos = "\n".join("    <li>%s</li>" % e(f) for f in t["facts"])
    pacotes = "\n".join(
        '    <li><h3>%s</h3><p>%s</p><p class="dl-links">%s</p></li>' % (
            e(so), e(t["dl_os"][so]), " ".join('<a class="button" href="%s%s">%s</a>' % (DL, a(f), e(rot)) for f, rot in arqs))
        for so, arqs in PACOTES)
    baixar = f"""
<section class="section section-dark download" id="baixar">
  <h2>{e(t["s_dl"])}</h2>
  <p>{e(t["dl_intro"])}</p>
  <ul class="steps downloads">
{pacotes}
  </ul>
  <p class="example">{e(t["dl_warn"])}</p>
  <div class="contact-row">
    <a class="button button-primary" href="{BLOB}/INSTALL.md">{e(t["dl_install"])}</a>
    <a class="button" href="{RELEASE}">{e(t["dl_all"])}</a>
  </div>
</section>
""" if PUBLICADO else ""
    noindex = "" if PUBLICADO else '<meta name="robots" content="noindex" />\n'
    pill = t["pill_pub"] if PUBLICADO else t["pill_prep"]
    estado = t["state_pub"] if PUBLICADO else t["state_prep"]
    return f"""<!doctype html>
<html lang="{lang}">
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
{noindex}<title>{e(t["title"])}</title>
<meta name="description" content="{a(t["desc"])}" />
<link rel="icon" href="assets/batruc-b.svg" type="image/svg+xml" />
<link rel="stylesheet" href="styles.css" />
<script src="assets/contact.js" defer></script>
</head>
<body>
<div class="rasgo-strip"><a href="{PORTAL}">{e(t["strip"])}</a></div>
<a class="skip-link" href="#conteudo">{e(t["skip"])}</a>

{cabecalho(lang, "sobre")}

<main id="conteudo">
<section class="hero" id="top">
  <span class="status-pill">{e(pill)}</span>
  <h1>Batruc</h1>
  <p class="lead">{e(t["lead"])}</p>
</section>

<section class="section">
  <h2>{e(t["s_shot"])}</h2>
  <figure class="shot">
    <img src="assets/interface.jpg" alt="{a(t["shot_alt"])}" width="1920" height="1080" loading="lazy" />
    <figcaption>{e(t["shot_cap"])}</figcaption>
  </figure>
</section>
{baixar}
<section class="section">
  <h2>{e(t["s1"])}</h2>
  <ol class="steps">
{passos}
  </ol>
</section>

<section class="section section-dark">
  <h2>{e(t["s2"])}</h2>
  <p>{e(t["bass"])}</p>
</section>

<section class="section">
  <h2>{e(t["s3"])}</h2>
  <p>{e(t["research"])}</p>
  <ul class="facts">
{fatos}
  </ul>
</section>

<section class="section section-dark">
  <h2>{e(t["s4"])}</h2>
  <p>{e(t["credits"])} <a href="{BLOB}/CREDITS_AND_SOURCES.md">{e(t["credits_link"])}</a>.</p>
</section>

<section class="section">
  <h2>{e(t["s5"])}</h2>
  <p>{e(estado)}</p>
  <div class="contact-row">
    <button type="button" class="button button-primary" data-contact-trigger>{e(t["contact"])}</button>
    <a class="button" href="{REPO}">GitHub</a>
  </div>
</section>
</main>

{rodape(lang)}
</body>
</html>
"""


# ---- páginas internas (14/11/2026, padrão do site do Asterismus) ----------
SUFIXO = {"pt-BR": "", "en": "-en", "fr": "-fr", "es": "-es"}
SOBRE = {lang: arq for arq, lang, _ in PAGINAS}
CODIGO = {lang: c for _, lang, c in PAGINAS}
TIPOS = ("tutorial", "learn", "instalacao")


def arquivo_de(tipo, lang):
    return SOBRE[lang] if tipo == "sobre" else "%s%s.html" % (tipo, SUFIXO[lang])


def cabecalho(lang, atual):
    n = NAV[lang]
    a = lambda x: html.escape(x, quote=True)
    itens = [("sobre", arquivo_de("sobre", lang)), ("tutorial", arquivo_de("tutorial", lang)),
             ("learn", arquivo_de("learn", lang)), ("instalacao", arquivo_de("instalacao", lang))]
    if PUBLICADO:
        itens.append(("baixar", arquivo_de("sobre", lang) + "#baixar"))
    menu = "\n".join('      <a href="%s"%s>%s</a>' % (href, ' aria-current="page"' if tipo == atual else "", html.escape(n[tipo]))
                     for tipo, href in itens)
    langs = "\n".join('      <a href="%s"%s>%s</a>' % (arquivo_de(atual if atual != "baixar" else "sobre", l),
                                                       ' aria-current="true"' if l == lang else "", CODIGO[l])
                      for _, l, _ in PAGINAS)
    return f"""<div class="site-header">
  <a class="brand" href="{arquivo_de("sobre", lang)}" aria-label="Batruc">
    <img src="assets/batruc-b.svg" alt="" width="36" height="36" />
    <span>BATRUC</span>
  </a>
  <div class="header-nav">
    <nav class="page-nav" aria-label="{a(n["aria"])}">
{menu}
    </nav>
    <nav class="lang-switch" aria-label="{a(T[lang]["langnav"])}">
{langs}
    </nav>
  </div>
</div>"""


def figuras(nomes, lang):
    """Recortes reais do app na língua da página (assets/shots/<língua>/)."""
    from PIL import Image
    a = lambda x: html.escape(x, quote=True)
    saida = []
    for nome in nomes:
        caminho = "assets/shots/%s/%s.jpg" % (PASTA_IMAGENS[lang], nome)
        with Image.open(AQUI / caminho) as im:
            w, h = im.size
        saida.append('    <figure class="lesson-shot"><img src="%s" alt="%s" width="%d" height="%d" loading="lazy" /></figure>'
                     % (caminho, a(ALT[nome][lang]), w, h))
    return "\n".join(saida)


def paragrafos(texto):
    e = lambda x: html.escape(x, quote=False)
    blocos = [b.strip() for b in texto.split("\n\n") if b.strip()]
    return "\n".join("  <p>%s</p>" % "<br />".join(e(l) for l in b.split("\n")) for b in blocos)


def corpo_tutorial(lang):
    t = TUTORIAL[lang]
    e = lambda x: html.escape(x, quote=False)
    passos = []
    for n, (h, intro, itens) in enumerate(t["steps"], start=1):
        lista = "\n".join("      <li>%s</li>" % e(i) for i in itens)
        passos.append(f"""  <article class="lesson">
    <h2>{e(h)}</h2>
    <p>{e(intro)}</p>
    <ul>
{lista}
    </ul>
{figuras(IMAGENS_TUTORIAL.get(n, []), lang)}
  </article>""")
    return f"""<section class="hero hero-inner" id="top">
  <h1>{e(t["h1"])}</h1>
  <p class="lead">{e(t["lead"])}</p>
</section>
<section class="section lessons">
{chr(10).join(passos)}
</section>
<section class="section section-dark">
  <h2>{e(t["rule_h"])}</h2>
  <p>{e(t["rule"])}</p>
</section>"""


LEARN_CODIGO = {"pt-BR": "pt", "en": "en", "fr": "fr", "es": "es"}


def corpo_learn(lang):
    t = LEARN[lang]
    e = lambda x: html.escape(x, quote=False)
    topicos = [x for x in json.loads((AQUI / "learn_textos.json").read_text(encoding="utf-8")) if x["topic"] >= 1]
    indice = "\n".join('    <li><a href="#t%d">%s</a></li>' % (x["topic"], e(x[LEARN_CODIGO[lang]]["title"])) for x in topicos)
    fichas = "\n".join(f"""  <article class="lesson" id="t{x["topic"]}">
    <h2>{x["topic"]}. {e(x[LEARN_CODIGO[lang]]["title"])}</h2>
{paragrafos(x[LEARN_CODIGO[lang]]["body"])}
{figuras(IMAGENS_LEARN.get(x["topic"], []), lang)}
  </article>""" for x in topicos)
    return f"""<section class="hero hero-inner" id="top">
  <h1>{e(t["h1"])}</h1>
  <p class="lead">{e(t["lead"])}</p>
  <ol class="toc">
{indice}
  </ol>
</section>
<section class="section lessons">
{fichas}
</section>"""


def corpo_instalacao(lang):
    t = INSTALACAO[lang]
    e = lambda x: html.escape(x, quote=False)
    lista = lambda xs: "\n".join("    <li>%s</li>" % e(x) for x in xs)
    return f"""<section class="hero hero-inner" id="top">
  <h1>{e(t["h1"])}</h1>
  <p class="lead">{e(t["lead"])}</p>
</section>
<section class="section lessons">
  <article class="lesson">
    <h2>{e(t["win_h"])}</h2>
    <ol>
{lista(t["win"])}
    </ol>
  </article>
  <article class="lesson">
    <h2>{e(t["mac_h"])}</h2>
    <ol>
{lista(t["mac"])}
    </ol>
    <pre><code>{e(t["mac_cmd"])}</code></pre>
  </article>
  <article class="lesson">
    <h2>{e(t["lin_h"])}</h2>
    <ul>
{lista(t["lin"])}
    </ul>
  </article>
  <article class="lesson">
    <h2>{e(t["onde_h"])}</h2>
    <ul>
{lista(t["onde"])}
    </ul>
  </article>
</section>
<section class="section section-dark">
  <p>{e(t["estado"])}</p>
  <div class="contact-row">
    <a class="button button-primary" href="{BLOB}/INSTALL.md">{e(t["guia"])}</a>
    <a class="button" href="{RELEASE}">{e(T[lang]["dl_all"])}</a>
  </div>
</section>"""


def pagina_interna(tipo, lang):
    t = T[lang]
    meta = {"tutorial": TUTORIAL, "learn": LEARN, "instalacao": INSTALACAO}[tipo][lang]
    corpo = {"tutorial": corpo_tutorial, "learn": corpo_learn, "instalacao": corpo_instalacao}[tipo](lang)
    e = lambda x: html.escape(x, quote=False)
    a = lambda x: html.escape(x, quote=True)
    noindex = "" if PUBLICADO else '<meta name="robots" content="noindex" />\n'
    return f"""<!doctype html>
<html lang="{lang}">
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
{noindex}<title>{e(meta["title"])}</title>
<meta name="description" content="{a(meta["desc"])}" />
<link rel="icon" href="assets/batruc-b.svg" type="image/svg+xml" />
<link rel="stylesheet" href="styles.css" />
<script src="assets/contact.js" defer></script>
</head>
<body>
<div class="rasgo-strip"><a href="{PORTAL}">{e(t["strip"])}</a></div>
<a class="skip-link" href="#conteudo">{e(t["skip"])}</a>

{cabecalho(lang, tipo)}

<main id="conteudo">
{corpo}
<section class="section">
  <div class="contact-row">
    <button type="button" class="button button-primary" data-contact-trigger>{e(t["contact"])}</button>
    <a class="button" href="{REPO}">GitHub</a>
  </div>
</section>
</main>

{rodape(lang)}
</body>
</html>
"""

def main():
    for arquivo, lang, _ in PAGINAS:
        (AQUI / arquivo).write_text(pagina(arquivo, lang), encoding="utf-8")
    for tipo in TIPOS:
        for _, lang, _ in PAGINAS:
            (AQUI / arquivo_de(tipo, lang)).write_text(pagina_interna(tipo, lang), encoding="utf-8")
    print("gerar_paginas.py: %d páginas (PUBLICADO=%s)" % (len(PAGINAS) * (1 + len(TIPOS)), PUBLICADO))


if __name__ == "__main__":
    main()

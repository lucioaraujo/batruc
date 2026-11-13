/* Conteúdo educativo local da maquete Batruc.
 * Direção artística: Lúcio de Araújo. Implementação assistida por Codex.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
(function () {
  "use strict";

  const content = {
    pt: {
      learn: "Aprender", aboutLabel: "Sobre", close: "FECHAR ×", language: "IDIOMA DO CONTEÚDO",
      topics: [
        ["Fórmula e fase", "Uma fase organiza cada movimento: θᵢ(t) = 2π nᵢ t/T + φᵢ. n indica os ciclos na seção; φ desloca a fase. Em 4/4, a seção de quatro compassos tem 16 tempos e duração T = 960/BPM segundos. O ponto móvel mostra o valor atual da função e os marcadores indicam os ataques calculados."],
        ["Regras de evento", "A regra Centro dispara quando a fase cruza 0 ou π. Extremos dispara nas inversões, em π/2 ou 3π/2. A regra escolhe quando a voz ataca; a curva continua mostrando o movimento matemático. A intensidade do ataque é calculada separadamente pela lei I e por λ."],
        ["Superfície e geometria", "O plano XY combina x = sen(θ₁) e y = sen(θ₂). Campo livre mantém os eventos; ilhas e microlinhas filtram eventos por posição, convertendo os que ficam fora em pausas. O desenho geométrico é uma trilha visual; ao ativar a opção de fonte, o valor y calculado conduz a voz 1 e altera seus eventos. O bilhar quadrado usa reflexão especular e sete razões de voltas fechadas x:y; cada colisão vira um evento da V1 quando essa forma é escolhida como fonte. É um traçado de raios ideal, não uma reverberação de sala. As formas de Fourier usam uma soma finita de componentes harmônicos: triângulos e quadrados são aproximações, e a ordem H muda a curva e o erro RMS exibidos. A nefróide (epicicloide, R=2r) e a deltóide (hipocicloide, R=3r) mostram a roda rolando; também há cicloide e catenária normalizada. Triângulo e quadrado exibem área pelo método do cadarço e perímetro pela soma dos lados; esses valores ainda não modulam o som. Com a fonte geométrica ativa, y(t) pode atuar na V1."],
        ["Espaço acústico", "A geometria de uma sala altera os percursos e tempos das reflexões. Um bilhar ideal acompanha um raio: v′=v−2(v·n)n; comprimento do percurso dividido pela velocidade do som aproxima o atraso, e cada parede pode reduzir a energia. Essa aproximação funciona melhor em frequências altas, quando o comprimento de onda é pequeno diante da sala. Em frequências baixas, ondas interferem e formam modos próprios; uma resposta de sala precisa modelar esses modos ou usar uma resposta ao impulso. O bilhar atual é um operador composicional de eventos, não uma reverberação acústica. Ainda não há efeito de sala aplicado ao áudio."],
        ["Som e lei I", "A lei I calcula a intensidade do ataque, não a altura da nota nem uma medição de energia acústica. As oito funções são: uniforme, projeção em y, produto xy, diferença x²−y², centro gaussiano, bandas de cosseno, energia potencial U e energia cinética K. Nos dois últimos modelos, x e y são deslocamentos normalizados de dois osciladores abstratos de mesma massa; n₁ e n₂ são seus ciclos na seção ativa: U=(n₁²x²+n₂²y²)/(n₁²+n₂²) e K=1−U. A mistura é Iλ=(1−λ)+λf(x,y): λ=0 mantém intensidade uniforme; λ=1 aplica integralmente a função escolhida. É um mapeamento composicional inspirado em modelos físicos, não uma simulação nem uma medição de energia. Timbres e modelos sonoros continuam experimentais."],
        ["Gráfico, zoom e movimento", "Cada voz ocupa uma faixa própria. Ajuste Zoom para examinar um trecho, Amplitude Y para ampliar a altura visual e Centro do trecho para navegar. A roda do mouse amplia em torno do cursor; arrastar percorre a janela ampliada. O movimento da vista não altera o som. Alterações de fase e de parâmetros composicionais, quando disponíveis, mudam os eventos calculados."],
        ["Cenas, gravação e arquivos", "A seed identifica uma realização; Nova seed cria outra. Escolha uma pasta local para guardar e reabrir cálculos. A gravação captura o master estéreo no navegador e oferece o áudio e os dados da composição para salvar no dispositivo. Faça uma nova gravação para cada tomada que queira preservar; confira a pasta escolhida antes de fechar a página."]
      ],
      about: ["Sobre o Batruc", "Batruc é um instrumento generativo em desenvolvimento na família RASGO, criado por Lúcio de Araújo. Trucmetry é seu nome histórico. Esta página é uma maquete local para experimentar fórmulas, gráficos e composição; a versão principal está sendo desenvolvida em C++/JUCE.\n\nO código do Batruc é distribuído sob GNU AGPL-3.0-or-later. O JUCE mantém seus próprios termos de licença. Os modelos e sons são experimentais; a página não envia gravações a um servidor: os arquivos são tratados no navegador e salvos na pasta escolhida pelo usuário."]
    },
    en: {
      learn: "Learn", aboutLabel: "About", close: "CLOSE ×", language: "CONTENT LANGUAGE",
      topics: [
        ["Formula and phase", "A phase organizes each motion: θᵢ(t) = 2π nᵢ t/T + φᵢ. n is the number of cycles in a section; φ shifts the phase. In 4/4, a four-bar section has 16 beats and lasts T = 960/BPM seconds. The moving point shows the current function value; markers show calculated attacks."],
        ["Event rules", "Center triggers when phase crosses 0 or π. Extrema triggers at turning points, π/2 or 3π/2. The rule chooses when a voice attacks; the curve still shows the mathematical motion. Attack intensity is calculated separately by Law I and λ."],
        ["Surface and geometry", "The XY plane combines x = sin(θ₁) and y = sin(θ₂). Open field keeps all events; pads and microlanes filter events by position, turning those outside into rests. The geometric drawing is a visual path; with source mode on, the calculated y value drives voice 1 and changes its events. The square billiard uses specular reflection and seven closed x:y lap ratios; each collision becomes a voice 1 event when this shape is selected as its source. It is an ideal ray path, not room reverberation. Fourier shapes use a finite sum of harmonic components: triangles and squares are approximations, and order H changes the curve and the displayed RMS error. The nephroid (epicycloid, R=2r) and deltoid (hypocycloid, R=3r) show the rolling circle. The cycloid and normalized catenary are also available. Triangle and square show area by the shoelace formula and perimeter by summing edges; these values do not yet modulate sound. With geometry source on, y(t) can shape voice 1."],
        ["Acoustic space", "Room geometry changes reflection paths and arrival times. An ideal billiard follows a ray: v′=v−2(v·n)n; path length divided by the speed of sound approximates delay, and each wall can reduce energy. This approximation works best at high frequencies, when wavelength is small compared with the room. At low frequencies, waves interfere and form room modes; a room response needs a model of those modes or an impulse response. The current billiard is a compositional event operator, not acoustic reverberation. No room effect is applied to the audio yet."],
        ["Sound and Law I", "Law I calculates attack intensity, not note pitch or measured acoustic energy. Its eight functions are uniform, projection on y, product xy, difference x²−y², Gaussian centre, cosine bands, potential energy U and kinetic energy K. In the last two models, x and y are normalized displacements of two abstract equal-mass oscillators; n₁ and n₂ are their cycles in the active section: U=(n₁²x²+n₂²y²)/(n₁²+n₂²) and K=1−U. The blend is Iλ=(1−λ)+λf(x,y): λ=0 keeps intensity uniform; λ=1 applies the selected function fully. This is a compositional mapping inspired by physical models, not a simulation or an energy measurement. Timbres and sound models remain experimental."],
        ["Graph, zoom and motion", "Each voice has its own lane. Use Zoom to inspect a passage, Y Amplitude to enlarge its visual height, and Section Center to navigate. The mouse wheel zooms around the pointer; dragging moves through the enlarged window. View movement does not change sound. Phase and compositional parameters, when available, change the calculated events."],
        ["Scenes, recording and files", "A seed identifies one realization; New Seed creates another. Choose a local folder to save and reopen compositions. Recording captures the stereo master in the browser and offers the audio and composition data for saving on your device. Make a new recording for each take you want to keep; check the selected folder before closing the page."]
      ],
      about: ["About Batruc", "Batruc is a generative instrument in development within the RASGO family, created by Lúcio de Araújo. Trucmetry is its former name. This page is a local study for exploring formulas, graphs and composition; the main version is being developed in C++/JUCE.\n\nBatruc code is released under GNU AGPL-3.0-or-later. JUCE retains its own licence terms. Sound models are experimental. The page does not upload recordings to a server: files are handled in the browser and saved to the folder selected by the user."]
    },
    es: {
      learn: "Aprender", aboutLabel: "Acerca de", close: "CERRAR ×", language: "IDIOMA DEL CONTENIDO",
      topics: [
        ["Fórmula y fase", "La fase organiza cada movimiento: θᵢ(t) = 2π nᵢ t/T + φᵢ. n indica los ciclos de la sección; φ desplaza la fase. En 4/4, una sección de cuatro compases tiene 16 pulsos y dura T = 960/BPM segundos. El punto móvil muestra el valor actual de la función; las marcas muestran los ataques calculados."],
        ["Reglas de evento", "Centro dispara cuando la fase cruza 0 o π. Extremos dispara en los puntos de inversión, π/2 o 3π/2. La regla decide cuándo ataca una voz; la curva sigue mostrando el movimiento matemático. La intensidad del ataque se calcula por separado mediante la ley I y λ."],
        ["Superficie y geometría", "El plano XY combina x = sen(θ₁) e y = sen(θ₂). Campo libre conserva todos los eventos; islas y microlíneas los filtran por posición y convierten los exteriores en pausas. El dibujo geométrico es una trayectoria visual; al activar el modo fuente, el valor y calculado conduce la voz 1 y modifica sus eventos. El billar cuadrado usa reflexión especular y siete razones cerradas de vueltas x:y; cada choque se convierte en un evento de V1 cuando la figura es su fuente. Es un recorrido ideal de rayos, no una reverberación de sala. Las formas de Fourier usan una suma finita de componentes armónicos: triángulos y cuadrados son aproximaciones, y el orden H cambia la curva y el error RMS mostrado. La nefroide (epicicloide, R=2r) y deltoide (hipocicloide, R=3r) muestran la rueda rodante; también hay cicloide y catenaria normalizada. El triángulo y el cuadrado muestran el área por el método del cordón (shoelace) y el perímetro por suma de lados; todavía no modulan el sonido. Con la fuente geométrica activa, y(t) puede influir en V1."],
        ["Espacio acústico", "La geometría de una sala cambia los recorridos y tiempos de las reflexiones. Un billar ideal sigue un rayo: v′=v−2(v·n)n; la longitud del recorrido dividida por la velocidad del sonido aproxima el retardo, y cada pared puede reducir la energía. Esta aproximación funciona mejor en frecuencias altas, cuando la longitud de onda es pequeña frente a la sala. En frecuencias bajas, las ondas interfieren y forman modos propios; una respuesta de sala necesita modelar esos modos o usar una respuesta al impulso. El billar actual es un operador compositivo de eventos, no una reverberación acústica. Todavía no se aplica un efecto de sala al audio."],
        ["Sonido y ley I", "La ley I calcula la intensidad del ataque, no la altura de la nota ni una medición de energía acústica. Sus ocho funciones son: uniforme, proyección en y, producto xy, diferencia x²−y², centro gaussiano, bandas de coseno, energía potencial U y energía cinética K. En los dos últimos modelos, x e y son desplazamientos normalizados de dos osciladores abstractos de igual masa; n₁ y n₂ son sus ciclos en la sección activa: U=(n₁²x²+n₂²y²)/(n₁²+n₂²) y K=1−U. La mezcla es Iλ=(1−λ)+λf(x,y): λ=0 mantiene la intensidad uniforme; λ=1 aplica por completo la función elegida. Es un mapeo compositivo inspirado en modelos físicos, no una simulación ni una medición de energía. Los timbres y modelos sonoros siguen siendo experimentales."],
        ["Gráfico, zoom y movimiento", "Cada voz tiene su propia franja. Usa Zoom para examinar un pasaje, Amplitud Y para ampliar su altura visual y Centro del tramo para navegar. La rueda del ratón amplía alrededor del cursor; arrastrar recorre la ventana ampliada. El movimiento de la vista no cambia el sonido. La fase y los parámetros compositivos, cuando están disponibles, modifican los eventos calculados."],
        ["Escenas, grabación y archivos", "La semilla identifica una realización; Nueva semilla crea otra. Elige una carpeta local para guardar y volver a abrir composiciones. La grabación captura el master estéreo en el navegador y ofrece el audio y los datos de composición para guardarlos en el dispositivo. Haz una grabación nueva por cada toma que quieras conservar y comprueba la carpeta elegida antes de cerrar la página."]
      ],
      about: ["Acerca de Batruc", "Batruc es un instrumento generativo en desarrollo dentro de la familia RASGO, creado por Lúcio de Araújo. Trucmetry es su nombre anterior. Esta página es un estudio local para explorar fórmulas, gráficos y composición; la versión principal se desarrolla en C++/JUCE.\n\nEl código de Batruc se distribuye bajo GNU AGPL-3.0-or-later. JUCE conserva sus propios términos de licencia. Los modelos sonoros son experimentales. La página no sube grabaciones a un servidor: los archivos se procesan en el navegador y se guardan en la carpeta elegida por el usuario."]
    },
    fr: {
      learn: "Apprendre", aboutLabel: "À propos", close: "FERMER ×", language: "LANGUE DU CONTENU",
      topics: [
        ["Formule et phase", "La phase organise chaque mouvement : θᵢ(t) = 2π nᵢ t/T + φᵢ. n indique les cycles de la section ; φ décale la phase. En 4/4, une section de quatre mesures compte 16 temps et dure T = 960/BPM secondes. Le point mobile montre la valeur actuelle de la fonction ; les repères montrent les attaques calculées."],
        ["Règles d’événement", "Centre déclenche lorsque la phase traverse 0 ou π. Extrema déclenche aux points d’inversion, π/2 ou 3π/2. La règle choisit l’instant d’attaque ; la courbe montre toujours le mouvement mathématique. L’intensité est calculée séparément par la loi I et λ."],
        ["Surface et géométrie", "Le plan XY combine x = sin(θ₁) et y = sin(θ₂). Le champ libre conserve tous les événements ; les îlots et microlignes les filtrent par position et transforment les événements extérieurs en silences. Le dessin géométrique est une trajectoire visuelle ; en mode source, la valeur y calculée pilote la voix 1 et modifie ses événements. Le billard carré utilise la réflexion spéculaire et sept rapports fermés de tours x:y ; chaque collision devient un événement de V1 lorsque cette forme est sélectionnée comme source. C’est un trajet de rayons idéal, pas une réverbération de salle. Les formes de Fourier utilisent une somme finie de composantes harmoniques : triangles et carrés sont des approximations, et l’ordre H modifie la courbe et l’erreur RMS affichée. La néphroïde (épicycloïde, R=2r) et la deltoïde (hypocycloïde, R=3r) montrent le cercle roulant. La cycloïde et la chaînette normalisée sont également disponibles. Triangle et carré affichent l’aire par la formule de lacet et le périmètre par la somme des côtés ; ces valeurs ne modulent pas encore le son. Avec la source géométrique active, y(t) peut agir sur V1."],
        ["Espace acoustique", "La géométrie d'une salle modifie les trajets et les temps des réflexions. Un billard idéal suit un rayon : v′=v−2(v·n)n ; la longueur du trajet divisée par la vitesse du son approche le retard, et chaque paroi peut réduire l'énergie. Cette approximation convient mieux aux hautes fréquences, lorsque la longueur d'onde est petite par rapport à la salle. Aux basses fréquences, les ondes interfèrent et forment des modes propres ; une réponse de salle doit modéliser ces modes ou utiliser une réponse impulsionnelle. Le billard actuel est un opérateur compositionnel d'événements, pas une réverbération acoustique. Aucun effet de salle n'est encore appliqué au son."],
        ["Son et loi I", "La loi I calcule l’intensité de l’attaque, pas la hauteur de la note ni une mesure d’énergie acoustique. Ses huit fonctions sont : uniforme, projection sur y, produit xy, différence x²−y², centre gaussien, bandes cosinus, énergie potentielle U et énergie cinétique K. Dans les deux derniers modèles, x et y sont les déplacements normalisés de deux oscillateurs abstraits de même masse ; n₁ et n₂ désignent leurs cycles dans la section active : U=(n₁²x²+n₂²y²)/(n₁²+n₂²) et K=1−U. Le mélange est Iλ=(1−λ)+λf(x,y) : λ=0 garde l’intensité uniforme ; λ=1 applique entièrement la fonction choisie. C’est un mappage compositionnel inspiré de modèles physiques, pas une simulation ni une mesure d’énergie. Les timbres et modèles sonores restent expérimentaux."],
        ["Graphique, zoom et mouvement", "Chaque voix possède sa propre bande. Utilisez Zoom pour examiner un passage, Amplitude Y pour agrandir sa hauteur visuelle et Centre de section pour naviguer. La molette zoome autour du pointeur ; le glissement parcourt la fenêtre agrandie. Le mouvement de la vue ne change pas le son. La phase et les paramètres de composition, lorsqu’ils sont disponibles, modifient les événements calculés."],
        ["Scènes, enregistrement et fichiers", "La seed identifie une réalisation ; Nouvelle seed en crée une autre. Choisissez un dossier local pour sauvegarder et rouvrir les compositions. L’enregistrement capture le master stéréo dans le navigateur et propose l’audio et les données de composition à enregistrer sur l’appareil. Créez un nouvel enregistrement pour chaque prise à conserver et vérifiez le dossier choisi avant de fermer la page."]
      ],
      about: ["À propos de Batruc", "Batruc est un instrument génératif en développement au sein de la famille RASGO, créé par Lúcio de Araújo. Trucmetry est son ancien nom. Cette page est une étude locale pour explorer formules, graphiques et composition ; la version principale est développée en C++/JUCE.\n\nLe code de Batruc est distribué sous GNU AGPL-3.0-or-later. JUCE conserve ses propres conditions de licence. Les modèles sonores sont expérimentaux. La page n’envoie pas les enregistrements à un serveur : les fichiers sont traités dans le navigateur et enregistrés dans le dossier choisi par l’utilisateur."]
    }
  };

  const dialog = document.getElementById("guide-dialog");
  const tabs = document.getElementById("guide-tabs");
  const article = document.getElementById("guide-content");
  const title = document.getElementById("guide-title");
  const language = document.getElementById("guide-language");
  const langTags = { pt: "pt-BR", en: "en", es: "es", fr: "fr" };
  let selected = "topic-0";

  try {
    const interfaceLanguage = document.getElementById("interface-language").value;
    const saved = localStorage.getItem("batruc-guide-language");
    language.value = content[interfaceLanguage] ? interfaceLanguage
      : saved && content[saved] ? saved : "pt";
  } catch (_) { /* Armazenamento local pode estar indisponível em file://. */ }

  function render() {
    const locale = content[language.value] || content.pt;
    if (tabs.childElementCount === 0) {
      [...locale.topics, locale.about].forEach((_, index) => {
        const button = document.createElement("button");
        button.type = "button";
        button.addEventListener("click", () => {
          selected = index === locale.topics.length ? "about" : `topic-${index}`;
          render();
        });
        tabs.append(button);
      });
    }
    [...tabs.children].forEach((button, index) => {
      const aboutButton = index === locale.topics.length;
      button.textContent = aboutButton ? locale.aboutLabel : locale.topics[index][0];
      button.setAttribute("aria-pressed", String(aboutButton
        ? selected === "about" : selected === `topic-${index}`));
    });

    const isAbout = selected === "about";
    const index = Number(selected.slice("topic-".length));
    const entry = isAbout ? locale.about : locale.topics[index] || locale.topics[0];
    title.textContent = isAbout ? locale.aboutLabel : locale.learn;
    article.replaceChildren();
    const heading = document.createElement("h3");
    heading.textContent = entry[0];
    const paragraph = document.createElement("p");
    paragraph.textContent = entry[1];
    article.append(heading, paragraph);
    article.lang = langTags[language.value] || "pt-BR";
    document.getElementById("guide-learn").textContent = locale.learn.toLocaleUpperCase(langTags[language.value]);
    document.getElementById("guide-about").textContent = locale.aboutLabel.toLocaleUpperCase(langTags[language.value]);
    document.getElementById("guide-close").textContent = locale.close;
    document.getElementById("guide-close").setAttribute("aria-label", locale.close.replace(" ×", ""));
    document.querySelector("label[for=guide-language]").textContent = locale.language;
    language.setAttribute("aria-label", locale.language);
    tabs.setAttribute("aria-label", locale.learn);
  }

  function open(key) {
    selected = key;
    render();
    if (!dialog.open) dialog.showModal();
  }

  document.getElementById("guide-learn").addEventListener("click", () => open("topic-0"));
  document.getElementById("guide-about").addEventListener("click", () => open("about"));
  document.getElementById("guide-close").addEventListener("click", () => dialog.close());
  language.addEventListener("change", () => {
    const interfaceLanguage = document.getElementById("interface-language");
    interfaceLanguage.value = language.value;
    interfaceLanguage.dispatchEvent(new Event("change", { bubbles: true }));
  });
  document.addEventListener("batruc:languagechange", (event) => {
    if (content[event.detail?.language]) {
      language.value = event.detail.language;
      render();
    }
  });
  render();
})();

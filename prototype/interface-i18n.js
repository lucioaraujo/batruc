/* Localização progressiva da interface web do Batruc.
 * Direção artística: Lúcio de Araújo. Implementação assistida por Codex.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
(function () {
  "use strict";

  // Cada linha mantém o original PT e suas versões EN, ES e FR. A tabela é
  // aplicada a nós de texto/atributos explícitos; cálculos não são alterados.
  const rows = [
    ["Relação ecológica", "Ecological relation", "Relación ecológica", "Relation écologique"],
    ["Independente", "Independent", "Independiente", "Indépendante"],
    ["Sustentar", "Support", "Sostener", "Soutenir"],
    ["Imitar", "Imitate", "Imitar", "Imiter"],
    ["Afastar", "Avoid", "Alejar", "Écarter"],
    ["Competir", "Compete", "Competir", "Rivaliser"],
    ["V1 · início da cadeia relacional", "V1 · start of the relational chain", "V1 · inicio de la cadena relacional", "V1 · début de la chaîne relationnelle"],
    ["λ controla a força; memória = ½ ciclo e atraso de imitação = ¹⁄₁₆ ciclo.", "λ sets strength; memory = ½ cycle and imitation delay = ¹⁄₁₆ cycle.", "λ controla la fuerza; memoria = ½ ciclo y retraso de imitación = ¹⁄₁₆ ciclo.", "λ règle la force ; mémoire = ½ cycle et délai d’imitation = ¹⁄₁₆ cycle."],
    ["RASGO / INSTRUMENTO EM INCUBAÇÃO", "RASGO / INSTRUMENT IN DEVELOPMENT", "RASGO / INSTRUMENTO EN DESARROLLO", "RASGO / INSTRUMENT EN DÉVELOPPEMENT"],
    ["Estudo 01 — até doze vozes de fórmula e um módulo editável de baixo.", "Study 01 — up to twelve formula voices and an editable bass module.", "Estudio 01 — hasta doce voces de fórmulas y un módulo de bajo editable.", "Étude 01 — jusqu’à douze voix de formule et un module de basse éditable."],
    ["MATEMÁTICA", "MATHEMATICS", "MATEMÁTICAS", "MATHÉMATIQUES"],
    ["FÍSICA", "PHYSICS", "FÍSICA", "PHYSIQUE"],
    ["MÚSICA", "MUSIC", "MÚSICA", "MUSIQUE"],
    ["combinação linear de funções", "linear combination of functions", "combinación lineal de funciones", "combinaison linéaire de fonctions"],
    ["influência idealizada entre movimentos", "idealized interaction between motions", "influencia idealizada entre movimientos", "interaction idéalisée entre mouvements"],
    ["imitação transformada, encontro e variação", "transformed imitation, encounter and variation", "imitación transformada, encuentro y variación", "imitation transformée, rencontre et variation"],
    ["λ = 0: vozes independentes. Aumentar λ aproxima cada voz do movimento anterior.", "λ = 0: independent voices. Increasing λ draws each voice toward the previous motion.", "λ = 0: voces independientes. Al aumentar λ, cada voz se acerca al movimiento anterior.", "λ = 0 : voix indépendantes. Augmenter λ rapproche chaque voix du mouvement précédent."],
    ["IDIOMA", "LANGUAGE", "IDIOMA", "LANGUE"],
    ["SEED / 8 HEX", "SEED / 8 HEX", "SEED / 8 HEX", "SEED / 8 HEX"],
    ["NOVA SEED", "NEW SEED", "NUEVA SEED", "NOUVELLE SEED"],
    ["VARIAÇÃO", "VARIATION", "VARIACIÓN", "VARIATION"],
    ["A seed calcula o baixo a partir dos ataques e posições das vozes. Editar uma célula converte o baixo para o modo manual.", "The seed calculates the bass from voice attacks and positions. Editing a cell switches the bass to manual mode.", "La seed calcula el bajo a partir de los ataques y posiciones de las voces. Editar una celda cambia el bajo al modo manual.", "La seed calcule la basse à partir des attaques et positions des voix. Modifier une cellule passe la basse en mode manuel."],
    ["ESCOLHER PASTA", "CHOOSE FOLDER", "ELEGIR CARPETA", "CHOISIR UN DOSSIER"],
    ["GUARDAR COMO", "SAVE AS", "GUARDAR COMO", "ENREGISTRER SOUS"],
    ["Nome da composição", "Composition name", "Nombre de la composición", "Nom de la composition"],
    ["GUARDAR CÁLCULO", "SAVE COMPOSITION", "GUARDAR CÁLCULO", "ENREGISTRER LA COMPOSITION"],
    ["CÁLCULOS NA PASTA", "COMPOSITIONS IN FOLDER", "CÁLCULOS EN LA CARPETA", "COMPOSITIONS DANS LE DOSSIER"],
    ["Escolha uma pasta", "Choose a folder", "Elige una carpeta", "Choisissez un dossier"],
    ["ABRIR", "OPEN", "ABRIR", "OUVRIR"], ["APAGAR", "DELETE", "BORRAR", "SUPPRIMER"],
    ["Escolha uma pasta local para guardar e reabrir cálculos e gravações.", "Choose a local folder to save and reopen compositions and recordings.", "Elige una carpeta local para guardar y volver a abrir cálculos y grabaciones.", "Choisissez un dossier local pour enregistrer et rouvrir les compositions et les enregistrements."],
    ["Fórmulas e valores em movimento", "Formulas and values in motion", "Fórmulas y valores en movimiento", "Formules et valeurs en mouvement"],
    ["Relação entre cálculo e composição", "From calculation to composition", "Relación entre cálculo y composición", "Du calcul à la composition"],
    ["TRAÇADO / 8,0 S POR CICLO", "TRACE / 8.0 S PER CYCLE", "TRAZO / 8,0 S POR CICLO", "TRACÉ / 8,0 S PAR CYCLE"],
    ["Uma forma que se torna tempo", "A shape becoming time", "Una forma que se convierte en tiempo", "Une forme qui devient temps"],
    ["Construção", "Construction", "Construcción", "Construction"], ["Mudança", "Change", "Cambio", "Changement"],
    ["Expansão", "Expansion", "Expansión", "Expansion"], ["Pausa", "Pause", "Pausa", "Pause"], ["Retorno", "Return", "Retorno", "Retour"],
    ["02 / DESENHO GEOMÉTRICO", "02 / GEOMETRIC DRAWING", "02 / DIBUJO GEOMÉTRICO", "02 / TRACÉ GÉOMÉTRIQUE"],
    ["O instrumento traça a curva", "The instrument traces the curve", "El instrumento traza la curva", "L’instrument trace la courbe"],
    ["INSTRUMENTO DE TRAÇADO", "DRAWING TOOL", "INSTRUMENTO DE TRAZADO", "OUTIL DE TRACÉ"],
    ["TRAJETÓRIA · VOLTAS X:Y", "TRAJECTORY · LAPS X:Y", "TRAYECTORIA · VUELTAS X:Y", "TRAJECTOIRE · TOURS X:Y"],
    ["Bilhar quadrado: reflexão especular · v′=v−2(v·n)n · 6 colisões por ciclo", "Square billiard: specular reflection · v′=v−2(v·n)n · 6 collisions per cycle", "Billar cuadrado: reflexión especular · v′=v−2(v·n)n · 6 colisiones por ciclo", "Billard carré : réflexion spéculaire · v′=v−2(v·n)n · 6 collisions par cycle"],
    ["Compasso · circunferência", "Compass · circle", "Compás · circunferencia", "Compas · cercle"],
    ["Esquadro · triângulo", "Set square · triangle", "Escuadra · triángulo", "Équerre · triangle"],
    ["Régua e esquadro · quadrado", "Ruler and set square · square", "Regla y escuadra · cuadrado", "Règle et équerre · carré"],
    ["Compasso · espiral", "Compass · spiral", "Compás · espiral", "Compas · spirale"],
    ["Lápis · curva de Lissajous", "Pencil · Lissajous curve", "Lápiz · curva de Lissajous", "Crayon · courbe de Lissajous"],
    ["Fourier · triângulo", "Fourier · triangle", "Fourier · triángulo", "Fourier · triangle"],
    ["Fourier · quadrado", "Fourier · square", "Fourier · cuadrado", "Fourier · carré"],
    ["Roda externa · epicicloide (nefróide)", "Outer rolling circle · epicycloid (nephroid)", "Rueda exterior · epicicloide (nefroide)", "Cercle roulant extérieur · épicycloïde (néphroïde)"],
    ["Roda interna · hipocicloide (deltóide)", "Inner rolling circle · hypocycloid (deltoid)", "Rueda interior · hipocicloide (deltoide)", "Cercle roulant intérieur · hypocycloïde (deltoïde)"],
    ["Cicloide · roda sobre reta", "Cycloid · wheel rolling on a line", "Cicloide · rueda sobre una recta", "Cycloïde · roue roulant sur une droite"],
    ["Catenária · curva de uma corrente suspensa", "Catenary · hanging chain curve", "Catenaria · curva de una cadena suspendida", "Chaînette · courbe d’une chaîne suspendue"],
    ["2:4 · 6 impactos", "2:4 · 6 impacts", "2:4 · 6 impactos", "2:4 · 6 impacts"],
    ["4:2 · 6 impactos", "4:2 · 6 impacts", "4:2 · 6 impactos", "4:2 · 6 impacts"],
    ["2:6 · 8 impactos", "2:6 · 8 impacts", "2:6 · 8 impactos", "2:6 · 8 impacts"],
    ["6:2 · 8 impactos", "6:2 · 8 impacts", "6:2 · 8 impactos", "6:2 · 8 impacts"],
    ["4:4 · 8 impactos", "4:4 · 8 impacts", "4:4 · 8 impactos", "4:4 · 8 impacts"],
    ["4:6 · 10 impactos", "4:6 · 10 impacts", "4:6 · 10 impactos", "4:6 · 10 impacts"],
    ["6:4 · 10 impactos", "6:4 · 10 impacts", "6:4 · 10 impactos", "6:4 · 10 impacts"],
    ["Bilhar · quadrado com reflexões", "Billiard · reflective square", "Billar · cuadrado con reflexiones", "Billard · carré avec réflexions"],
    ["Usar y do desenho como função da voz 1", "Use drawing y as voice 1's function", "Usar y del dibujo como función de la voz 1", "Utiliser y du dessin comme fonction de la voix 1"],
    ["ORDEM H · COMPONENTES −H…+H", "ORDER H · COMPONENTS −H…+H", "ORDEN H · COMPONENTES −H…+H", "ORDRE H · COMPOSANTES −H…+H"],
    ["Percurso geométrico traçado pelo compasso, lápis ou esquadro", "Geometric path traced by compass, pencil or set square", "Recorrido geométrico trazado por compás, lápiz o escuadra", "Trajectoire géométrique tracée au compas, crayon ou à l’équerre"],
    ["01 / VOZES · TEMPO", "01 / VOICES · TIME", "01 / VOCES · TIEMPO", "01 / VOIX · TEMPS"],
    ["Funções convertidas em eventos sonoros", "Functions turned into sound events", "Funciones convertidas en eventos sonoros", "Fonctions converties en événements sonores"],
    ["Traçados das vozes e pontos de disparo", "Voice traces and trigger points", "Trazos de las voces y puntos de disparo", "Tracés des voix et points de déclenchement"],
    ["ZOOM", "ZOOM", "ZOOM", "ZOOM"], ["AMPLITUDE Y", "Y AMPLITUDE", "AMPLITUD Y", "AMPLITUDE Y"],
    ["CENTRO DO TRECHO", "SECTION CENTRE", "CENTRO DEL TRAMO", "CENTRE DE SECTION"],
    ["VER CICLO TODO", "SHOW FULL CYCLE", "VER CICLO COMPLETO", "VOIR LE CYCLE ENTIER"],
    ["evento sonoro", "sound event", "evento sonoro", "événement sonore"],
    ["A linha vertical é a posição atual no ciclo.", "The vertical line marks the current position in the cycle.", "La línea vertical marca la posición actual del ciclo.", "La ligne verticale indique la position actuelle dans le cycle."],
    ["MIXER / DOZE VOZES + BAIXO", "MIXER / TWELVE VOICES + BASS", "MIXER / DOCE VOCES + BAJO", "MIXEUR / DOUZE VOIX + BASSE"],
    ["Equilíbrio e espaço", "Balance and space", "Equilibrio y espacio", "Équilibre et espace"],
    ["MASTER · saída", "MASTER · output", "MASTER · salida", "MASTER · sortie"],
    ["TRIM DAS VOZES", "VOICE BUS TRIM", "TRIM DE VOCES", "TRIM DES VOIX"],
    ["AJUSTAR VOZES", "BALANCE VOICES", "EQUILIBRAR VOCES", "ÉQUILIBRER LES VOIX"],
    ["Controles independentes por voz", "Independent controls per voice", "Controles independientes por voz", "Contrôles indépendants par voix"],
    ["MÓDULO / BAIXO", "MODULE / BASS", "MÓDULO / BAJO", "MODULE / BASSE"],
    ["Escreva a linha, passo a passo", "Write the line, step by step", "Escribe la línea, paso a paso", "Écrivez la ligne, pas à pas"],
    ["Regra de duração", "Duration rule", "Regla de duración", "Règle de durée"],
    ["Manual por passo", "Manual per step", "Manual por paso", "Manuel par pas"],
    ["Sequência de Fibonacci", "Fibonacci sequence", "Secuencia de Fibonacci", "Suite de Fibonacci"],
    ["Progressão geométrica", "Geometric progression", "Progresión geométrica", "Progression géométrique"],
    ["Matéria sonora do baixo", "Bass sound material", "Materia sonora del bajo", "Matière sonore de la basse"],
    ["Perfil espectral", "Spectral profile", "Perfil espectral", "Profil spectral"],
    ["Modular alturas com LFO", "Modulate pitch with LFO", "Modular alturas con LFO", "Moduler la hauteur avec un LFO"],
    ["Forma do LFO", "LFO shape", "Forma del LFO", "Forme du LFO"],
    ["Profundidade", "Depth", "Profundidad", "Profondeur"], ["Frase / pausas", "Phrase / rests", "Frase / pausas", "Phrase / silences"],
    ["Loop exato", "Exact loop", "Loop exacto", "Boucle exacte"],
    ["Encaixe com as vozes", "Fit with voices", "Encaje con las voces", "Articulation avec les voix"],
    ["Convolução cíclica", "Cyclic convolution", "Convolución cíclica", "Convolution cyclique"],
    ["Mínimo de variação de altura", "Minimum pitch variation", "Variación mínima de altura", "Variation minimale de hauteur"],
    ["Corte VCF", "VCF cutoff", "Corte VCF", "Coupure VCF"], ["Ressonância", "Resonance", "Resonancia", "Résonance"],
    ["Envelopes do baixo", "Bass envelopes", "Envolventes del bajo", "Enveloppes de la basse"],
    ["AMPLITUDE · ADSR", "AMPLITUDE · ADSR", "AMPLITUD · ADSR", "AMPLITUDE · ADSR"],
    ["FILTRO · ADSR", "FILTER · ADSR", "FILTRO · ADSR", "FILTRE · ADSR"],
    ["Nível", "Level", "Nivel", "Niveau"], ["Ataque", "Attack", "Ataque", "Attaque"],
    ["Decaimento", "Decay", "Decaimiento", "Déclin"], ["Sustentação", "Sustain", "Sostenimiento", "Maintien"],
    ["Soltura", "Release", "Liberación", "Relâchement"], ["Andamento", "Tempo", "Tempo", "Tempo"],
    ["Fórmula de compasso", "Time signature", "Compás", "Chiffrage"],
    ["Modelo de movimento", "Motion model", "Modelo de movimiento", "Modèle de mouvement"],
    ["Quantidade de vozes", "Number of voices", "Cantidad de voces", "Nombre de voix"],
    ["Relação polirrítmica", "Polyrhythmic ratio", "Relación polirrítmica", "Rapport polyrythmique"],
    ["APLICAR RELAÇÃO", "APPLY RATIO", "APLICAR RELACIÓN", "APPLIQUER LE RAPPORT"],
    ["Receita de eventos", "Event recipe", "Receta de eventos", "Recette d’événements"],
    ["Campo da receita", "Recipe field", "Campo de la receta", "Champ de la recette"],
    ["Lei de intensidade", "Intensity law", "Ley de intensidad", "Loi d’intensité"],
    ["Mistura da lei", "Law blend", "Mezcla de la ley", "Mélange de la loi"],
    ["Intenção composicional", "Compositional intention", "Intención compositiva", "Intention compositionnelle"],
    ["Desenho da dinâmica", "Dynamics shape", "Diseño de dinámica", "Courbe de dynamique"],
    ["Forma de tocar", "Articulation", "Articulación", "Articulation"], ["Duração total", "Total duration", "Duración total", "Durée totale"],
    ["Matéria sonora", "Sound material", "Materia sonora", "Matière sonore"], ["Groove", "Groove", "Groove", "Groove"],
    ["▶ Tocar estudo", "▶ Play study", "▶ Tocar estudio", "▶ Lire l’étude"], ["■ Parar", "■ Stop", "■ Detener", "■ Arrêter"],
    ["● Gravar master", "● Record master", "● Grabar master", "● Enregistrer le master"],
    ["Áudio não gravado", "Audio not recorded", "Audio no grabado", "Audio non enregistré"],
    ["Pronto para ouvir. O áudio começa após seu gesto.", "Ready to listen. Audio starts after your gesture.", "Listo para escuchar. El audio comienza tras tu acción.", "Prêt à écouter. Le son démarre après votre action."],
    ["FORMA / 7 CICLOS", "FORM / 7 CYCLES", "FORMA / 7 CICLOS", "FORME / 7 CYCLES"],
    ["Construção, mudança, pausa, retorno", "Build, change, pause, return", "Construcción, cambio, pausa, retorno", "Construction, changement, pause, retour"],
    ["Modelos:", "Models:", "Modelos:", "Modèles :"],
    ["A direção do instrumento permanece C++/JUCE.", "The instrument's main development remains C++/JUCE.", "El desarrollo principal del instrumento continúa en C++/JUCE.", "Le développement principal de l’instrument reste en C++/JUCE."],
    ["funções senoidais combinadas por λ ou, quando selecionada, aproximação linear de três pêndulos angulares ligados por molas efetivas. Esta última não reproduz o pêndulo serial do artigo visto nas capturas. O baixo usa uma sequência editável de notas e durações; o LFO é opcional. Os sons são provisórios.", "sine functions combined by λ or, when selected, a linear approximation of three angular pendulums coupled by effective springs. The latter does not reproduce the serial pendulum in the article shown in the screenshots. The bass uses an editable sequence of notes and durations; the LFO is optional. The sounds are provisional.", "funciones sinusoidales combinadas por λ o, si se selecciona, una aproximación lineal de tres péndulos angulares unidos por resortes efectivos. Esta última no reproduce el péndulo serial del artículo mostrado en las capturas. El bajo usa una secuencia editable de notas y duraciones; el LFO es opcional. Los sonidos son provisionales.", "fonctions sinusoïdales combinées par λ ou, si cette option est choisie, une approximation linéaire de trois pendules angulaires couplés par des ressorts effectifs. Cette dernière ne reproduit pas le pendule en série de l’article visible dans les captures. La basse utilise une séquence de notes et de durées modifiable ; le LFO est facultatif. Les sons sont provisoires."],
    ["Batruc · Lúcio de Araújo · estudo local em HTML/JavaScript para investigar a interface. A direção do instrumento permanece C++/JUCE.", "Batruc · Lúcio de Araújo · local HTML/JavaScript study for interface research. The instrument's main development remains C++/JUCE.", "Batruc · Lúcio de Araújo · estudio local en HTML/JavaScript para investigar la interfaz. El desarrollo principal del instrumento continúa en C++/JUCE.", "Batruc · Lúcio de Araújo · étude locale en HTML/JavaScript pour explorer l’interface. Le développement principal de l’instrument reste en C++/JUCE."],
    ["Nenhum cálculo salvo nesta pasta", "No compositions saved in this folder", "No hay cálculos guardados en esta carpeta", "Aucune composition enregistrée dans ce dossier"],
    ["sem seed", "no seed", "sin seed", "sans seed"], ["leitura indisponível", "unreadable", "lectura no disponible", "lecture indisponible"],
    ["Escolha primeiro uma pasta local.", "Choose a local folder first.", "Elige primero una carpeta local.", "Choisissez d’abord un dossier local."],
    ["Permissão de leitura e escrita não concedida.", "Read and write permission was not granted.", "No se concedió permiso de lectura y escritura.", "L’autorisation de lecture et d’écriture n’a pas été accordée."],
    ["Arquivo de composição inválido ou versão não suportada.", "Invalid composition file or unsupported version.", "Archivo de composición no válido o versión no compatible.", "Fichier de composition invalide ou version non prise en charge."],
    ["Este navegador não oferece acesso à pasta local. Use um navegador Chromium recente.", "This browser does not support local folder access. Use a recent Chromium browser.", "Este navegador no permite acceder a carpetas locales. Usa una versión reciente de Chromium.", "Ce navigateur ne permet pas l’accès aux dossiers locaux. Utilisez une version récente de Chromium."],
    ["Complete os oito caracteres hexadecimais da seed antes de guardar.", "Enter all eight hexadecimal seed characters before saving.", "Completa los ocho caracteres hexadecimales de la seed antes de guardar.", "Saisissez les huit caractères hexadécimaux de la seed avant l’enregistrement."],
    ["Escolha a pasta local antes de guardar o cálculo.", "Choose the local folder before saving the composition.", "Elige la carpeta local antes de guardar el cálculo.", "Choisissez le dossier local avant d’enregistrer la composition."],
    ["Gravação não disponível neste navegador.", "Recording is not available in this browser.", "La grabación no está disponible en este navegador.", "L’enregistrement n’est pas disponible dans ce navigateur."],
    ["Gravação vazia; nenhum arquivo salvo.", "Empty recording; no file was saved.", "Grabación vacía; no se guardó ningún archivo.", "Enregistrement vide ; aucun fichier n’a été enregistré."],
    ["REC · saída master estéreo.", "REC · stereo master output.", "REC · salida master estéreo.", "REC · sortie master stéréo."],
    ["SENSOR · parado", "SENSOR · stopped", "SENSOR · detenido", "CAPTEUR · arrêté"],
    ["SENSOR · aguarda áudio", "SENSOR · waiting for audio", "SENSOR · esperando audio", "CAPTEUR · en attente de son"],
    ["SENSOR · vozes", "SENSOR · voices", "SENSOR · voces", "CAPTEUR · voix"],
    ["voz → baixo", "voice → bass", "voz → bajo", "voix → basse"],
    ["baixo / LFO", "bass / LFO", "bajo / LFO", "basse / LFO"],
    ["voz de referência · sem mistura anterior", "reference voice · no previous-voice blend", "voz de referencia · sin mezcla anterior", "voix de référence · sans mélange précédent"],
    ["oscilações por ciclo", "cycles per section", "oscilaciones por ciclo", "oscillations par cycle"],
    ["Transformação-base · voz", "Base transform · voice", "Transformación base · voz", "Transformation de base · voix"],
    ["pêndulo · θᵢ(t)", "pendulum · θᵢ(t)", "péndulo · θᵢ(t)", "pendule · θᵢ(t)"],
    ["Aplicada:", "Applied:", "Aplicada:", "Appliquée :"],
    ["valor x", "x value", "valor x", "valeur x"], ["VOZ", "VOICE", "VOZ", "VOIX"],
    ["ATIVA", "ACTIVE", "ACTIVA", "ACTIVE"], ["PAUSA", "REST", "PAUSA", "PAUSE"],
    ["CANAL", "CHANNEL", "CANAL", "CANAL"], ["VOL", "VOL", "VOL", "VOL"], ["PAN", "PAN", "PAN", "PAN"],
    ["voz", "voice", "voz", "voix"], ["baixo", "bass", "bajo", "basse"],
    ["sem baixo", "without bass", "sin bajo", "sans basse"], ["e baixo", "and bass", "y bajo", "et basse"],
    ["sem mistura anterior", "no previous-voice blend", "sin mezcla anterior", "sans mélange précédent"],
    ["MUDANÇA", "CHANGE", "CAMBIO", "CHANGEMENT"], ["EXPANSÃO", "EXPANSION", "EXPANSIÓN", "EXPANSION"], ["RETORNO", "RETURN", "RETORNO", "RETOUR"],
    ["CONSTRUÇÃO", "BUILD", "CONSTRUCCIÓN", "CONSTRUCTION"],
    ["pontos de contato", "contact pads", "zonas de contacto", "zones de contact"],
    ["campo livre", "open field", "campo libre", "champ libre"],
    ["faixas diagonais", "diagonal lanes", "bandas diagonales", "bandes diagonales"],
    ["Harmônicos", "Harmonics", "Armónicos", "Harmoniques"], ["Ímpares", "Odd", "Impares", "Impairs"],
    ["Inarmônicos", "Inharmonic", "Inarmónicos", "Inharmoniques"],
    ["Batimento lento", "Slow beating", "Batimiento lento", "Battement lent"],
    ["Seno original", "Original sine", "Seno original", "Sinus d’origine"],
    ["Inversão", "Inversion", "Inversión", "Inversion"], ["Retificação", "Rectification", "Rectificación", "Redressement"],
    ["Segundo harmônico", "Second harmonic", "Segundo armónico", "Deuxième harmonique"],
    ["Envelope de amplitude", "Amplitude envelope", "Envolvente de amplitud", "Enveloppe d’amplitude"],
    ["Quantização em 4 níveis", "4-level quantization", "Cuantización en 4 niveles", "Quantification à 4 niveaux"],
    ["Cúbica", "Cubic", "Cúbica", "Cubique"], ["Terceiro harmônico", "Third harmonic", "Tercer armónico", "Troisième harmonique"],
    ["Saturação suave", "Soft saturation", "Saturación suave", "Saturation douce"],
    ["Aplicar", "Apply", "Aplicar", "Appliquer"],
    ["Constante", "Uniform", "Constante", "Constante"], ["Projeção em y", "Projection on y", "Proyección en y", "Projection sur y"],
    ["Produto xy", "Product xy", "Producto xy", "Produit xy"], ["Diferença x² − y²", "Difference x² − y²", "Diferencia x² − y²", "Différence x² − y²"],
    ["Centro gaussiano", "Gaussian centre", "Centro gaussiano", "Centre gaussien"], ["Faixas de cosseno", "Cosine bands", "Bandas de coseno", "Bandes cosinus"],
    ["Energia potencial", "Potential energy", "Energía potencial", "Énergie potentielle"], ["Energia cinética", "Kinetic energy", "Energía cinética", "Énergie cinétique"],
    ["Campo livre", "Open field", "Campo libre", "Champ libre"], ["Quatro zonas de contato", "Four contact pads", "Cuatro zonas de contacto", "Quatre zones de contact"],
    ["Duas faixas diagonais", "Two diagonal lanes", "Dos bandas diagonales", "Deux bandes diagonales"],
    ["O campo move e deforma o recorte pela seed. As quatro ilhas pulsam e derivam em fases próprias com influência suave da vizinha; conectores transitórios incluem no campo o corredor entre ilhas próximas. As microlinhas derivam e respiram de modo assimétrico. Ataques que cruzam as bordas mudam de presença e intensidade; o traço acompanha o campo.", "The field shifts and deforms from the seed. Four islands pulse and drift at distinct phases with gentle neighbour influence; temporary bridges add corridors between nearby islands to the field. The microlanes drift and breathe asymmetrically. Attacks crossing their edges change presence and intensity; the trace follows the field.", "El campo se desplaza y deforma según la seed. Cuatro islas pulsan y derivan con fases propias e influencia suave de la vecina; conectores transitorios incorporan al campo los corredores entre islas próximas. Las microlíneas derivan y respiran de forma asimétrica. Los ataques que cruzan los bordes cambian de presencia e intensidad; el trazo sigue el campo.", "Le champ se déplace et se déforme selon la seed. Quatre îlots pulsent et dérivent avec des phases propres et une légère influence du voisin ; des connecteurs temporaires ajoutent au champ les couloirs entre îlots proches. Les microlignes dérivent et respirent de façon asymétrique. Les attaques qui franchissent leurs bords changent de présence et d’intensité ; le tracé suit le champ."],
    ["Escutar encontros", "Hear encounters", "Escuchar encuentros", "Écouter les rencontres"],
    ["Construir intensidade", "Build intensity", "Construir intensidad", "Construire l’intensité"],
    ["Rarefazer", "Thin out", "Reducir densidad", "Éclaircir"], ["Contrastar vozes", "Contrast voices", "Contrastar voces", "Contraster les voix"],
    ["Estável", "Steady", "Estable", "Stable"], ["Crescendo", "Crescendo", "Crescendo", "Crescendo"],
    ["Arco", "Arc", "Arco", "Arche"], ["Ondas", "Waves", "Ondas", "Ondes"],
    ["Curta", "Short", "Corta", "Courte"], ["Equilibrada", "Balanced", "Equilibrada", "Équilibrée"], ["Sustentada", "Sustained", "Sostenida", "Soutenue"],
    ["Seno — puro", "Sine — pure", "Seno — puro", "Sinus — pur"], ["Triângulo — suave", "Triangle — soft", "Triángulo — suave", "Triangle — doux"],
    ["Dente — brilhante", "Sawtooth — bright", "Diente — brillante", "Dent de scie — brillant"], ["Quadrada — oca", "Square — hollow", "Cuadrada — hueca", "Carrée — creuse"],
    ["Quatro apoios", "Four pulses", "Cuatro apoyos", "Quatre appuis"], ["Três pulsações", "Three pulses", "Tres pulsos", "Trois pulsations"],
    ["Contratempos", "Offbeats", "Contratiempos", "Contretemps"],
    ["Nenhum áudio gravado", "No audio recorded", "No hay audio grabado", "Aucun audio enregistré"],
    ["■ Finalizar e salvar", "■ Finish and save", "■ Finalizar y guardar", "■ Terminer et enregistrer"],
    ["↺ Reiniciar estudo", "↺ Restart study", "↺ Reiniciar estudio", "↺ Recommencer l’étude"],
    ["Nova regra aplicada aos próximos eventos.", "New rule applied to upcoming events.", "Nueva regla aplicada a los próximos eventos.", "Nouvelle règle appliquée aux prochains événements."],
    ["Estudo concluído. Compare a outra receita ou mude a fase.", "Study complete. Compare the other recipe or change the phase.", "Estudio terminado. Compara la otra receta o cambia la fase.", "Étude terminée. Comparez l’autre recette ou changez la phase."],
    ["Tocando. Fórmulas, traços e eventos compartilham o mesmo ciclo.", "Playing. Formulas, traces and events share the same cycle.", "Reproduciendo. Fórmulas, trazos y eventos comparten el mismo ciclo.", "Lecture en cours. Formules, tracés et événements partagent le même cycle."],
    ["Animação sem áudio: Web Audio indisponível neste navegador.", "Animation without audio: Web Audio is unavailable in this browser.", "Animación sin audio: Web Audio no está disponible en este navegador.", "Animation sans son : Web Audio n’est pas disponible dans ce navigateur."],
    ["Saída master silenciada.", "Master output muted.", "Salida master silenciada.", "Sortie master en sourdine."],
    ["Saída master recuperada.", "Master output restored.", "Salida master restaurada.", "Sortie master rétablie."],
    ["Solo desligado; todos os canais não silenciados voltaram.", "Solo off; all unmuted channels are back.", "Solo desactivado; vuelven todos los canales no silenciados.", "Solo désactivé ; tous les canaux non muets sont rétablis."],
    ["Trim das vozes ajustado pela leitura RMS atual; o ajuste fica salvo na composição.", "Voice trim adjusted from current RMS readings; the adjustment is saved with the composition.", "Trim de voces ajustado según la lectura RMS actual; el ajuste se guarda en la composición.", "Trim des voix ajusté d’après la mesure RMS actuelle ; le réglage est enregistré avec la composition."],
    ["O gesto contínuo vira notas quantizadas pela grade do compasso.", "A continuous gesture becomes notes quantized to the bar grid.", "El gesto continuo se convierte en notas cuantizadas a la cuadrícula del compás.", "Un geste continu devient des notes quantifiées sur la grille de mesure."],
    ["O gráfico mostra o início do ciclo de mudança; editar as células abaixo troca para o baixo manual. LFO, filtros e envelopes continuam modulando o som.", "The graph shows the start of the Change cycle; editing cells below switches the bass to manual mode. LFO, filters and envelopes continue shaping the sound.", "El gráfico muestra el inicio del ciclo de Cambio; al editar las celdas, el bajo pasa al modo manual. El LFO, los filtros y las envolventes siguen modulando el sonido.", "Le graphique montre le début du cycle Changement ; modifier les cellules passe la basse en mode manuel. Le LFO, les filtres et les enveloppes continuent de modeler le son."],
    ["O gesto contínuo vira notas quantizadas pela grade do compasso.", "A continuous gesture becomes notes quantized to the bar grid.", "El gesto continuo se convierte en notas cuantizadas a la cuadrícula del compás.", "Un geste continu devient des notes quantifiées sur la grille de mesure."],
    ["Digite oito caracteres hexadecimais para fixar a seed.", "Enter eight hexadecimal characters to lock the seed.", "Escribe ocho caracteres hexadecimales para fijar la seed.", "Saisissez huit caractères hexadécimaux pour fixer la seed."],
    ["A mudança da visualização não altera o som.", "Changing the view does not change the sound.", "Cambiar la vista no altera el sonido.", "Le changement de vue ne modifie pas le son."],
    ["Aplicação no compasso:", "Applied in the bar:", "Aplicación en el compás:", "Application dans la mesure :"],
    ["semicolcheias", "sixteenth notes", "semicorcheas", "doubles croches"],
    ["Pasta", "Folder", "Carpeta", "Dossier"],
    ["Retirar mute do master", "Unmute master", "Quitar mute del master", "Rétablir le master"],
    ["Mute master", "Mute master", "Silenciar master", "Couper le master"],
    ["Apagar", "Delete", "Borrar", "Supprimer"],
    ["Abrir", "Open", "Abrir", "Ouvrir"],
    ["Guardar", "Save", "Guardar", "Enregistrer"],
    ["Gravação · áudio e partitura", "Recording · audio and score", "Grabación · audio y partitura", "Enregistrement · audio et partition"],
    ["Sem baixo", "No bass", "Sin bajo", "Sans basse"],
    ["em mute", "muted", "en mute", "en sourdine"],
    ["✎ DESENHAR TRAÇO", "✎ DRAW STROKE", "✎ DIBUJAR TRAZO", "✎ DESSINER UN TRAIT"],
    ["✎ DESENHANDO · CLIQUE PARA SAIR", "✎ DRAWING · CLICK TO EXIT", "✎ DIBUJANDO · CLIC PARA SALIR", "✎ DESSIN · CLIQUER POUR QUITTER"],
    ["↺ Reiniciar estudo", "↺ Restart study", "↺ Reiniciar estudio", "↺ Recommencer l’étude"],
    ["■ Finalizar e salvar", "■ Finish and save", "■ Finalizar y guardar", "■ Terminer et enregistrer"],
    ["Solo desligado; todos os canais não silenciados voltaram.", "Solo off; all unmuted channels are back.", "Solo desactivado; vuelven todos los canales no silenciados.", "Solo désactivé ; tous les canaux non coupés sont rétablis."],
    ["Animação sem áudio: Web Audio indisponível neste navegador.", "Animation without audio: Web Audio is unavailable in this browser.", "Animación sin audio: Web Audio no está disponible en este navegador.", "Animation sans audio : Web Audio n’est pas disponible dans ce navigateur."]
  ];
  const translations = Object.fromEntries(rows.map(([pt, en, es, fr]) => [pt, { en, es, fr }]));
  const tags = { pt: "pt-BR", en: "en", es: "es", fr: "fr" };
  const selector = document.getElementById("interface-language");
  const textSources = new WeakMap();
  const attrSources = new WeakMap();
  let current = "pt";

  try {
    const stored = localStorage.getItem("batruc-interface-language");
    if (stored && tags[stored]) current = stored;
  } catch (_) { /* localStorage may be unavailable for file:// pages. */ }
  selector.value = current;

  function translateDynamic(source) {
    if (current === "pt") return source;
    const tr = (en, es, fr) => current === "pt" ? source : ({ en, es, fr })[current];
    let hit;

    const fixed = {
      "Plano de fase · θ₁ × θ₂": ["Phase plane · θ₁ × θ₂", "Plano de fase · θ₁ × θ₂", "Plan de phase · θ₁ × θ₂"],
      "02 / MOVIMENTO · FASE": ["02 / MOTION · PHASE", "02 / MOVIMIENTO · FASE", "02 / MOUVEMENT · PHASE"],
      "três equações diferenciais acopladas por vizinhança": ["three locally coupled differential equations", "tres ecuaciones diferenciales acopladas por vecindad", "trois équations différentielles couplées par voisinage"],
      "Ângulos dos três pêndulos e eventos de cruzamento": ["Three pendulum angles and crossing events", "Ángulos de los tres péndulos y eventos de cruce", "Angles des trois pendules et événements de croisement"],
      "Funções e vozes independentes": ["Functions and independent voices", "Funciones y voces independientes", "Fonctions et voix indépendantes"],
      "Três pêndulos ligados por molas · aproximação linear": ["Three spring-coupled pendulums · linear approximation", "Tres péndulos unidos por resortes · aproximación lineal", "Trois pendules couplés par ressorts · approximation linéaire"],
      "A seed calcula o baixo a partir dos ataques e posições das vozes. Editar uma célula converte o baixo para o modo manual.": ["The seed calculates the bass from voice attacks and positions. Editing a cell switches the bass to manual mode.", "La seed calcula el bajo a partir de los ataques y posiciones de las voces. Editar una celda cambia el bajo al modo manual.", "La seed calcule la basse à partir des attaques et positions des voix. Modifier une cellule passe la basse en mode manuel."],
      "O áudio usa uma linha calculada pelos eventos vocais; a grade é um molde para edição manual.": ["Audio uses a line calculated from voice events; the grid is a template for manual editing.", "El audio usa una línea calculada a partir de los eventos vocales; la cuadrícula es una plantilla para editar manualmente.", "Le son utilise une ligne calculée à partir des événements des voix ; la grille sert de modèle pour l’édition manuelle."],
      "Pausa": ["Rest", "Pausa", "Silence"],
      "O estudo terminou. Compare a outra receita ou mude a fase.": ["Study complete. Compare the other event rule or change the phase.", "Estudio terminado. Compara la otra regla de eventos o cambia la fase.", "Étude terminée. Comparez l’autre règle d’événements ou changez la phase."],
      "Estudo concluído. Compare a outra receita ou mude a fase.": ["Study complete. Compare the other event rule or change the phase.", "Estudio terminado. Compara la otra regla de eventos o cambia la fase.", "Étude terminée. Comparez l’autre règle d’événements ou changez la phase."],
      "Parado. O desenho permanece no início do ciclo.": ["Stopped. The drawing remains at the start of the cycle.", "Detenido. El dibujo permanece al inicio del ciclo.", "Arrêté. Le tracé reste au début du cycle."],
    };
    if (fixed[source]) return fixed[source][current === "en" ? 0 : current === "es" ? 1 : 2];
    if ((hit = source.match(/^(CONSTRUÇÃO|MUDANÇA|EXPANSÃO|PAUSA|RETORNO) · (C\d+(?:–\d+)?)$/))) {
      const names = { "CONSTRUÇÃO": ["BUILD", "CONSTRUCCIÓN", "CONSTRUCTION"], "MUDANÇA": ["CHANGE", "CAMBIO", "CHANGEMENT"], "EXPANSÃO": ["EXPANSION", "EXPANSIÓN", "EXPANSION"], "PAUSA": ["REST", "PAUSA", "PAUSE"], "RETORNO": ["RETURN", "RETORNO", "RETOUR"] };
      return `${names[hit[1]][current === "en" ? 0 : current === "es" ? 1 : 2]} · ${hit[2]}`;
    }
    if ((hit = source.match(/^H=(\d+) · (\d+) componentes$/)))
      return tr(`H=${hit[1]} · ${hit[2]} components`, `H=${hit[1]} · ${hit[2]} componentes`, `H=${hit[1]} · ${hit[2]} composantes`);
    if ((hit = source.match(/^ω₀ = (.+) rad\/s · energia total normalizada globalmente$/)))
      return tr(`ω₀ = ${hit[1]} rad/s · total energy normalized globally`, `ω₀ = ${hit[1]} rad/s · energía total normalizada globalmente`, `ω₀ = ${hit[1]} rad/s · énergie totale normalisée globalement`);
    if ((hit = source.match(/^Bilhar quadrado: reflexão especular · v′=v−2\(v·n\)n · voltas x:y=(\d+):(\d+) · (\d+) impactos\/ciclo · (trajeto e colisões alimentam a voz 1|percurso visual)$/))) {
      const relation = hit[4] === "percurso visual"
        ? tr("visual path", "recorrido visual", "trajectoire visuelle")
        : tr("path and collisions drive voice 1", "recorrido y colisiones alimentan la voz 1", "trajectoire et collisions alimentent la voix 1");
      return tr(`Square billiard: specular reflection · v′=v−2(v·n)n · laps x:y=${hit[1]}:${hit[2]} · ${hit[3]} impacts/cycle · ${relation}`,
        `Billar cuadrado: reflexión especular · v′=v−2(v·n)n · vueltas x:y=${hit[1]}:${hit[2]} · ${hit[3]} impactos/ciclo · ${relation}`,
        `Billard carré : réflexion spéculaire · v′=v−2(v·n)n · tours x:y=${hit[1]}:${hit[2]} · ${hit[3]} impacts/cycle · ${relation}`);
    }
    if ((hit = source.match(/^(.+) · (percurso visual|Y aproximado alimenta a voz 1|trajeto e colisões alimentam a voz 1)$/))) {
      const suffix = hit[2] === "percurso visual"
        ? tr("visual path", "recorrido visual", "trajectoire visuelle")
        : hit[2] === "trajeto e colisões alimentam a voz 1"
          ? tr("path and collisions drive voice 1", "recorrido y colisiones alimentan la voz 1", "trajectoire et collisions alimentent la voix 1")
          : tr("Approximate Y drives voice 1", "Y aproximada alimenta la voz 1", "Y approximatif alimente la voix 1");
      let formula = hit[1];
      formula = formula.replace("trajeto linear pelos 3 vértices · y = interpolação do percurso", tr("linear path through 3 vertices · y = path interpolation", "recorrido lineal por 3 vértices · y = interpolación del recorrido", "trajet linéaire par 3 sommets · y = interpolation du trajet"));
      formula = formula.replace("trajeto linear pelos 4 vértices · y = interpolação do percurso", tr("linear path through 4 vertices · y = path interpolation", "recorrido lineal por 4 vértices · y = interpolación del recorrido", "trajet linéaire par 4 sommets · y = interpolation du trajet"));
      formula = formula.replace("triângulo fechado", tr("closed triangle", "triángulo cerrado", "triangle fermé"));
      formula = formula.replace("quadrado fechado", tr("closed square", "cuadrado cerrado", "carré fermé"));
      formula = formula.replace("Epicicloide: R=2r · x=(3 cos θ−cos 3θ)/4 · y=(3 sen θ−sen 3θ)/4",
        tr("Epicycloid: R=2r · x=(3 cos θ−cos 3θ)/4 · y=(3 sin θ−sin 3θ)/4",
          "Epicicloide: R=2r · x=(3 cos θ−cos 3θ)/4 · y=(3 sen θ−sen 3θ)/4",
          "Épicycloïde : R=2r · x=(3 cos θ−cos 3θ)/4 · y=(3 sin θ−sin 3θ)/4"));
      formula = formula.replace("Hipocicloide: R=3r · x=(2 cos θ+cos 2θ)/3 · y=(2 sen θ−sen 2θ)/3",
        tr("Hypocycloid: R=3r · x=(2 cos θ+cos 2θ)/3 · y=(2 sin θ−sin 2θ)/3",
          "Hipocicloide: R=3r · x=(2 cos θ+cos 2θ)/3 · y=(2 sen θ−sen 2θ)/3",
          "Hypocycloïde : R=3r · x=(2 cos θ+cos 2θ)/3 · y=(2 sin θ−sin 2θ)/3"));
      formula = formula.replace("Cicloide: roda de raio r sobre reta · centro x=r(θ−π) · ponta x=r(θ−π−sen θ), y=−r cos θ",
        tr("Cycloid: radius-r wheel on a line · centre x=r(θ−π) · tip x=r(θ−π−sin θ), y=−r cos θ",
          "Cicloide: rueda de radio r sobre una recta · centro x=r(θ−π) · punta x=r(θ−π−sen θ), y=−r cos θ",
          "Cycloïde : roue de rayon r sur une droite · centre x=r(θ−π) · pointe x=r(θ−π−sin θ), y=−r cos θ"));
      formula = formula.replace("Catenária: y=cosh(x) · x=sen θ · normalizada",
        tr("Catenary: y=cosh(x) · x=sin θ · normalized",
          "Catenaria: y=cosh(x) · x=sen θ · normalizada",
          "Chaînette : y=cosh(x) · x=sin θ · normalisée"));
      formula = formula.replace("Bilhar quadrado: reflexão especular · v′=v−2(v·n)n · 6 colisões por ciclo",
        tr("Square billiard: specular reflection · v′=v−2(v·n)n · 6 collisions per cycle",
          "Billar cuadrado: reflexión especular · v′=v−2(v·n)n · 6 colisiones por ciclo",
          "Billard carré : réflexion spéculaire · v′=v−2(v·n)n · 6 collisions par cycle"));
      formula = formula.replace("componentes", tr("components", "componentes", "composantes"));
      formula = formula.replace("erro RMS relativo XY", tr("relative XY RMS error", "error RMS relativo XY", "erreur RMS relative XY"));
      return `${formula} · ${suffix}`;
    }
    if ((hit = source.match(/^Y aproximado alimenta a voz 1$/)))
      return tr("Approximate Y drives voice 1", "Y aproximada alimenta la voz 1", "Y approximatif alimente la voix 1");
    if ((hit = source.match(/^percurso visual$/)))
      return tr("visual path", "recorrido visual", "trajectoire visuelle");
    if ((hit = source.match(/^x = θ₁\(t\) · y = θ₂\(t\) · trajetória calculada nos sete ciclos$/)))
      return tr("x = θ₁(t) · y = θ₂(t) · trajectory calculated across seven cycles", "x = θ₁(t) · y = θ₂(t) · trayectoria calculada a lo largo de siete ciclos", "x = θ₁(t) · y = θ₂(t) · trajectoire calculée sur sept cycles");
    if ((hit = source.match(/^PÊNDULO \/ 7 CICLOS · (.+) S POR CICLO · (.+)$/)))
      return tr(`PENDULUM / 7 CYCLES · ${hit[1]} S PER CYCLE · ${hit[2]}`, `PÉNDULO / 7 CICLOS · ${hit[1]} S POR CICLO · ${hit[2]}`, `PENDULE / 7 CYCLES · ${hit[1]} S PAR CYCLE · ${hit[2]}`);
    if ((hit = source.match(/^TRAÇADO \/ (.+) S · (.+) SEMÍNIMAS\/CICLO · (.+)$/)))
      return tr(`TRACE / ${hit[1]} S · ${hit[2]} QUARTER NOTES/CYCLE · ${hit[3]}`, `TRAZO / ${hit[1]} S · ${hit[2]} NEGRAS/CICLO · ${hit[3]}`, `TRACÉ / ${hit[1]} S · ${hit[2]} NOIRES/CYCLE · ${hit[3]}`);
    if ((hit = source.match(/^(.+) volta$/)))
      return tr(`${hit[1]} cycle`, `${hit[1]} vuelta`, `${hit[1]} tour`);
    if ((hit = source.match(/^Geométrica: dₙ = min\((.+), arred\.\((.+)⁽ⁿ mod 8⁾\)\) semicolcheias\. Aplicação no compasso: (.*) semicolcheias\.$/)))
      return tr(`Geometric: dₙ = min(${hit[1]}, round(${hit[2]}⁽ⁿ mod 8⁾)) sixteenth notes. Bar application: ${hit[3]} sixteenth notes.`, `Geométrica: dₙ = min(${hit[1]}, redondear(${hit[2]}⁽ⁿ mod 8⁾)) semicorcheas. Aplicación en el compás: ${hit[3]} semicorcheas.`, `Géométrique : dₙ = min(${hit[1]}, arrondi(${hit[2]}⁽ⁿ mod 8⁾)) doubles-croches. Application à la mesure : ${hit[3]} doubles-croches.`);
    if ((hit = source.match(/^Manual: dₙ = duração escolhida no passo\. Aplicação no compasso: (.*) semicolcheias\.$/)))
      return tr(`Manual: dₙ = duration chosen for each step. Bar application: ${hit[1]} sixteenth notes.`, `Manual: dₙ = duración elegida para cada paso. Aplicación en el compás: ${hit[1]} semicorcheas.`, `Manuel : dₙ = durée choisie pour chaque pas. Application à la mesure : ${hit[1]} doubles-croches.`);
    if ((hit = source.match(/^Fibonacci: dₙ = min\((.+), F₍ₙ mod 8₎₊₁\) semicolcheias\. Aplicação no compasso: (.*) semicolcheias\.$/)))
      return tr(`Fibonacci: dₙ = min(${hit[1]}, F₍ₙ mod 8₎₊₁) sixteenth notes. Bar application: ${hit[2]} sixteenth notes.`, `Fibonacci: dₙ = min(${hit[1]}, F₍ₙ mod 8₎₊₁) semicorcheas. Aplicación en el compás: ${hit[2]} semicorcheas.`, `Fibonacci : dₙ = min(${hit[1]}, F₍ₙ mod 8₎₊₁) doubles-croches. Application à la mesure : ${hit[2]} doubles-croches.`);
    if ((hit = source.match(/^Manual: dₙ = duração escolhida no passo\.$/)))
      return tr("Manual: dₙ = duration chosen for each step.", "Manual: dₙ = duración elegida para cada paso.", "Manuel : dₙ = durée choisie pour chaque pas.");
    if ((hit = source.match(/^A duração é calculada pela lei selecionada; arrastar uma nota altera apenas a altura\. No modo de desenho, o traço cria ou altera os passos atravessados\.$/)))
      return tr("Duration follows the selected rule; dragging a note changes pitch only. In drawing mode, the stroke creates or changes the steps it crosses.", "La duración sigue la regla seleccionada; arrastrar una nota solo cambia la altura. En modo de dibujo, el trazo crea o modifica los pasos que atraviesa.", "La durée suit la règle choisie ; faire glisser une note ne change que sa hauteur. En mode dessin, le trait crée ou modifie les pas traversés.");
    if ((hit = source.match(/^Ciclo (\d+), (Construção|Mudança|Expansão|Pausa|Retorno), (\d+) voz\(es\) (sem baixo|e baixo)$/))) {
      const sections = { "Construção": ["Build", "Construcción", "Construction"], "Mudança": ["Change", "Cambio", "Changement"], "Expansão": ["Expansion", "Expansión", "Expansion"], "Pausa": ["Rest", "Pausa", "Silence"], "Retorno": ["Return", "Retorno", "Retour"] };
      const section = sections[hit[2]][current === "en" ? 0 : current === "es" ? 1 : 2];
      const bass = hit[4] === "e baixo" ? tr("and bass", "y bajo", "et basse") : tr("without bass", "sin bajo", "sans basse");
      return tr(`Cycle ${hit[1]}, ${section}, ${hit[3]} voice(s) ${bass}`, `Ciclo ${hit[1]}, ${section}, ${hit[3]} voz(ces) ${bass}`, `Cycle ${hit[1]}, ${section}, ${hit[3]} voix ${bass}`);
    }
    if ((hit = source.match(/^C(\d+): (\d+) voz\(es\) (sem baixo|e baixo)$/)))
      return tr(`C${hit[1]}: ${hit[2]} voice(s) ${hit[3] === "e baixo" ? "and bass" : "without bass"}`, `C${hit[1]}: ${hit[2]} voz(ces) ${hit[3] === "e baixo" ? "y bajo" : "sin bajo"}`, `C${hit[1]} : ${hit[2]} voix ${hit[3] === "e baixo" ? "et basse" : "sans basse"}`);
    if ((hit = source.match(/^(\d+(?:[.,]\d+)?) passos por compasso$/)))
      return tr(`${hit[1]} steps per bar`, `${hit[1]} pasos por compás`, `${hit[1]} pas par mesure`);
    if ((hit = source.match(/^(\d+(?:[.,]\d+)?) semitons$/)))
      return tr(`${hit[1]} semitones`, `${hit[1]} semitonos`, `${hit[1]} demi-tons`);
    if ((hit = source.match(/^ENTRA C(\d+)$/)))
      return tr(`ENTERS C${hit[1]}`, `ENTRA C${hit[1]}`, `ENTRE EN C${hit[1]}`);
    if ((hit = source.match(/^(\d+(?:[.,]\d+)?) oscilações por ciclo$/)))
      return tr(`${hit[1]} cycles per section`, `${hit[1]} oscilaciones por ciclo`, `${hit[1]} oscillations par cycle`);
    if ((hit = source.match(/^Voz (\d+) ativa neste ciclo$/)))
      return tr(`Voice ${hit[1]} active in this cycle`, `Voz ${hit[1]} activa en este ciclo`, `Voix ${hit[1]} active dans ce cycle`);
    if ((hit = source.match(/^Voz (\d+) em pausa; retorna no ciclo (\d+)$/)))
      return tr(`Voice ${hit[1]} rests; returns in cycle ${hit[2]}`, `Voz ${hit[1]} en pausa; vuelve en el ciclo ${hit[2]}`, `Voix ${hit[1]} en pause ; revient au cycle ${hit[2]}`);
    if ((hit = source.match(/^Voz (\d+) entra no ciclo (\d+)$/)))
      return tr(`Voice ${hit[1]} enters in cycle ${hit[2]}`, `Voz ${hit[1]} entra en el ciclo ${hit[2]}`, `Voix ${hit[1]} entre au cycle ${hit[2]}`);
    if ((hit = source.match(/^Nota do passo (\d+)$/)))
      return tr(`Note at step ${hit[1]}`, `Nota del paso ${hit[1]}`, `Note au pas ${hit[1]}`);
    if ((hit = source.match(/^Duração do passo (\d+)$/)))
      return tr(`Duration at step ${hit[1]}`, `Duración del paso ${hit[1]}`, `Durée du pas ${hit[1]}`);
    if ((hit = source.match(/^Traço cresce com a execução; cursor fora da janela\. Y não muda o volume\.$/)))
      return tr("Trace grows during playback; cursor is outside the window. Y does not change volume.", "El trazo crece durante la reproducción; el cursor está fuera de la ventana. Y no cambia el volumen.", "Le tracé se développe pendant la lecture ; le curseur est hors de la fenêtre. Y ne change pas le volume.");
    if (source === "Traço revelado pelo cálculo em tempo real; a forma muda com cada transformação.")
      return tr("Trace revealed by real-time calculation; its shape changes with each transformation.", "Trazo revelado por el cálculo en tiempo real; la forma cambia con cada transformación.", "Tracé révélé par le calcul en temps réel ; sa forme change avec chaque transformation.");
    if (source === "Cursor fora da janela; arraste para percorrer. Y não muda o volume.")
      return tr("Cursor outside the window; drag to navigate. Y does not change volume.", "Cursor fuera de la ventana; arrastra para recorrer. Y no cambia el volumen.", "Curseur hors de la fenêtre ; faites glisser pour parcourir. Y ne change pas le volume.");
    if (source === "Arraste para percorrer. A escala Y é visual; não muda o volume.")
      return tr("Drag to navigate. The Y scale is visual; it does not change volume.", "Arrastra para recorrer. La escala Y es visual; no cambia el volumen.", "Faites glisser pour parcourir. L’échelle Y est visuelle ; elle ne change pas le volume.");
    if ((hit = source.match(/^A duração é calculada pela lei selecionada; arrastar uma nota altera apenas a altura\. No modo de desenho, o traço cria ou altera os passos atravessados\.$/)))
      return tr("Duration follows the selected rule; dragging a note changes pitch only. In drawing mode, the stroke creates or changes the steps it crosses.", "La duración sigue la regla seleccionada; arrastrar una nota solo cambia la altura. En modo de dibujo, el trazo crea o modifica los pasos que atraviesa.", "La durée suit la règle choisie ; faire glisser une note ne change que sa hauteur. En mode dessin, le trait crée ou modifie les pas traversés.");
    if ((hit = source.match(/^Repetições do passo (\d+)$/)))
      return tr(`Repeats at step ${hit[1]}`, `Repeticiones del paso ${hit[1]}`, `Répétitions du pas ${hit[1]}`);
    if ((hit = source.match(/^Intenção do passo (\d+)$/)))
      return tr(`Intent at step ${hit[1]}`, `Intención del paso ${hit[1]}`, `Intention du pas ${hit[1]}`);
    if ((hit = source.match(/^Altura derivada da seed no passo (\d+)$/)))
      return tr(`Seed-derived pitch at step ${hit[1]}`, `Altura derivada de la seed en el paso ${hit[1]}`, `Hauteur issue de la seed au pas ${hit[1]}`);
    if ((hit = source.match(/^Mute (.+)$/))) return tr(`Mute ${hit[1]}`, `Silenciar ${hit[1]}`, `Couper ${hit[1]}`);
    if ((hit = source.match(/^Solo (.+)$/))) return tr(`Solo ${hit[1]}`, `Solo ${hit[1]}`, `Solo ${hit[1]}`);
    if ((hit = source.match(/^Cálculo “(.+)” salvo em (.+)\.$/)))
      return tr(`Composition “${hit[1]}” saved in ${hit[2]}.`, `Cálculo “${hit[1]}” guardado en ${hit[2]}.`, `Composition « ${hit[1]} » enregistrée dans ${hit[2]}.`);
    if ((hit = source.match(/^Apagar “(.+)” da pasta (.+)\?$/)))
      return tr(`Delete “${hit[1]}” from folder ${hit[2]}?`, `¿Borrar “${hit[1]}” de la carpeta ${hit[2]}?`, `Supprimer « ${hit[1]} » du dossier ${hit[2]} ?`);
    if ((hit = source.match(/^Composição local “(.+)” carregada\.$/)))
      return tr(`Local composition “${hit[1]}” loaded.`, `Composición local “${hit[1]}” cargada.`, `Composition locale « ${hit[1]} » chargée.`);
    if ((hit = source.match(/^“(.+)” reaberta · seed (.+)\.$/)))
      return tr(`“${hit[1]}” reopened · seed ${hit[2]}.`, `“${hit[1]}” reabierta · seed ${hit[2]}.`, `« ${hit[1]} » rouverte · seed ${hit[2]}.`);
    if ((hit = source.match(/^Arquivo (.+) removido de (.+)\.$/)))
      return tr(`File ${hit[1]} removed from ${hit[2]}.`, `Archivo ${hit[1]} eliminado de ${hit[2]}.`, `Fichier ${hit[1]} supprimé de ${hit[2]}.`);
    if ((hit = source.match(/^Pasta ativa: (.+)\. Os arquivos ficam no seu computador\.$/)))
      return tr(`Folder active: ${hit[1]}. Files stay on your computer.`, `Carpeta activa: ${hit[1]}. Los archivos permanecen en tu ordenador.`, `Dossier actif : ${hit[1]}. Les fichiers restent sur votre ordinateur.`);
    if ((hit = source.match(/^(.+): (\d+) cálculo\(s\) encontrado\(s\)\.$/)))
      return tr(`${hit[1]}: ${hit[2]} composition(s) found.`, `${hit[1]}: ${hit[2]} cálculo(s) encontrado(s).`, `${hit[1]} : ${hit[2]} composition(s) trouvée(s).`);
    if ((hit = source.match(/^Não foi possível abrir a pasta: (.*)$/)))
      return tr(`Could not open folder: ${hit[1]}`, `No se pudo abrir la carpeta: ${hit[1]}`, `Impossible d’ouvrir le dossier : ${hit[1]}`);
    if ((hit = source.match(/^Não foi possível ler a pasta: (.*)$/)))
      return tr(`Could not read folder: ${hit[1]}`, `No se pudo leer la carpeta: ${hit[1]}`, `Impossible de lire le dossier : ${hit[1]}`);
    if ((hit = source.match(/^Não foi possível guardar o cálculo: (.*)$/)))
      return tr(`Could not save composition: ${hit[1]}`, `No se pudo guardar el cálculo: ${hit[1]}`, `Impossible d’enregistrer la composition : ${hit[1]}`);
    if ((hit = source.match(/^Não foi possível abrir o cálculo: (.*)$/)))
      return tr(`Could not open composition: ${hit[1]}`, `No se pudo abrir el cálculo: ${hit[1]}`, `Impossible d’ouvrir la composition : ${hit[1]}`);
    if ((hit = source.match(/^Não foi possível apagar o cálculo: (.*)$/)))
      return tr(`Could not delete composition: ${hit[1]}`, `No se pudo borrar el cálculo: ${hit[1]}`, `Impossible de supprimer la composition : ${hit[1]}`);
    if ((hit = source.match(/^Áudio e partitura salvos em (.+) · ([\d.,]+) MB\.$/)))
      return tr(`Audio and score saved in ${hit[1]} · ${hit[2]} MB.`, `Audio y partitura guardados en ${hit[1]} · ${hit[2]} MB.`, `Audio et partition enregistrés dans ${hit[1]} · ${hit[2]} Mo.`);
    if ((hit = source.match(/^A pasta não aceitou a gravação \((.*)\); downloads preparados\.$/)))
      return tr(`The folder rejected the recording (${hit[1]}); downloads are ready.`, `La carpeta rechazó la grabación (${hit[1]}); las descargas están preparadas.`, `Le dossier a refusé l’enregistrement (${hit[1]}) ; les téléchargements sont prêts.`);
    if ((hit = source.match(/^ Master ([\d.,]+) MB · download iniciado; TXT disponível\.$/)))
      return tr(` Master ${hit[1]} MB · download started; TXT available.`, ` Master ${hit[1]} MB · descarga iniciada; TXT disponible.`, ` Master ${hit[1]} Mo · téléchargement lancé ; TXT disponible.`);
    if ((hit = source.match(/^SENSOR · vozes (.+) · baixo (.+)$/)))
      return tr(`SENSOR · voices ${hit[1]} · bass ${hit[2]}`, `SENSOR · voces ${hit[1]} · bajo ${hit[2]}`, `CAPTEUR · voix ${hit[1]} · basse ${hit[2]}`);
    if ((hit = source.match(/^RMS · vozes (.+) · baixo (.+) · ajuste (.+)$/)))
      return tr(`RMS · voices ${hit[1]} · bass ${hit[2]} · trim ${hit[3]}`, `RMS · voces ${hit[1]} · bajo ${hit[2]} · ajuste ${hit[3]}`, `RMS · voix ${hit[1]} · basse ${hit[2]} · trim ${hit[3]}`);
    if ((hit = source.match(/^Aplica (.+) às vozes para aproximar os níveis RMS medidos\. É uma compensação estática, não automação contínua\.$/)))
      return tr(`Apply ${hit[1]} to voices to bring measured RMS levels closer. This is static compensation, not continuous automation.`, `Aplica ${hit[1]} a las voces para acercar los niveles RMS medidos. Es una compensación estática, no una automatización continua.`, `Appliquer ${hit[1]} aux voix pour rapprocher les niveaux RMS mesurés. Il s’agit d’une compensation statique, pas d’une automation continue.`);
    if (source === "Trim das vozes ajustado pela leitura RMS atual; o ajuste fica salvo na composição.")
      return tr("Voice trim adjusted from the current RMS reading; the setting is saved in the composition.", "Ajuste de voces actualizado según la lectura RMS actual; el ajuste se guarda en la composición.", "Trim des voix ajusté d’après la mesure RMS actuelle ; le réglage est enregistré dans la composition.");
    if (source === "Nova regra aplicada aos próximos eventos.")
      return tr("New rule applied to upcoming events.", "Nueva regla aplicada a los próximos eventos.", "Nouvelle règle appliquée aux prochains événements.");
    if (source === "Tocando. Fórmulas, traços e eventos compartilham o mesmo ciclo.")
      return tr("Playing. Formulas, traces and events share the same cycle.", "En reproducción. Las fórmulas, los trazos y los eventos comparten el mismo ciclo.", "Lecture en cours. Formules, tracés et événements partagent le même cycle.");
    if (source === "Saída master silenciada.") return tr("Master output muted.", "Salida master silenciada.", "Sortie master coupée.");
    if (source === "Saída master recuperada.") return tr("Master output restored.", "Salida master restaurada.", "Sortie master rétablie.");
    if (source === "Digite oito caracteres hexadecimais para fixar a seed.")
      return tr("Enter eight hexadecimal characters to set the seed.", "Escribe ocho caracteres hexadecimales para fijar la seed.", "Saisissez huit caractères hexadécimaux pour définir la seed.");
    if ((hit = source.match(/^Seed ([A-F0-9]{8}): (\d+) vozes · (.+)$/))) {
      const labels = {
        center: ["centre crossings", "pasos por el centro", "passages par le centre"],
        turn: ["turning points", "puntos de inversión", "points d’inversion"],
        unrestricted: ["open field", "campo libre", "champ libre"],
        "adhesive-pads": ["four contact pads", "cuatro zonas de contacto", "quatre zones de contact"],
        "diagonal-lanes": ["two diagonal lanes", "dos bandas diagonales", "deux bandes diagonales"],
        uniform: ["uniform", "uniforme", "uniforme"],
        "projection-y": ["projection on y", "proyección en y", "projection sur y"],
        "product-xy": ["product xy", "producto xy", "produit xy"],
        "quadratic-difference": ["difference x² − y²", "diferencia x² − y²", "différence x² − y²"],
        "gaussian-centre": ["Gaussian centre", "centro gaussiano", "centre gaussien"],
        "cosine-bands": ["cosine bands", "bandas de coseno", "bandes cosinus"],
        "potential-energy": ["potential energy", "energía potencial", "énergie potentielle"],
        "kinetic-energy": ["kinetic energy", "energía cinética", "énergie cinétique"]
      };
      let rest = hit[3].replace(" ciclos em V2 · ", tr(" cycles on V2 · ", " ciclos en V2 · ", " cycles en V2 · "))
        .replace(". O baixo segue as vozes.", tr(". Bass follows the voices.", ". El bajo sigue las voces.", ". La basse suit les voix."))
        .replace(" transformação(ões), ", tr(" transformation(s), ", " transformación(es), ", " transformation(s), "))
        .replace(" ciclo(s) e ", tr(" cycle(s) and ", " ciclo(s) y ", " cycle(s) et "))
        .replace(" altura(s) variam.", tr(" pitch(es) vary.", " altura(s) varían.", " hauteur(s) varient."));
      if (current !== "pt")
        for (const [code, names] of Object.entries(labels)) rest = rest.replaceAll(` · ${code} · `, ` · ${names[current === "en" ? 0 : current === "es" ? 1 : 2]} · `);
      return tr(`Seed ${hit[1]}: ${hit[2]} voices · ${rest}`, `Seed ${hit[1]}: ${hit[2]} voces · ${rest}`, `Seed ${hit[1]} : ${hit[2]} voix · ${rest}`);
    }
    if ((hit = source.match(/^Aplicada: (.+?) · (.+)$/))) {
      const names = { "Seno original": ["Original sine", "Seno original", "Sinus d’origine"], "Inversão": ["Inversion", "Inversión", "Inversion"], "Retificação": ["Rectification", "Rectificación", "Redressement"], "Segundo harmônico": ["Second harmonic", "Segundo armónico", "Deuxième harmonique"], "Envelope de amplitude": ["Amplitude envelope", "Envolvente de amplitud", "Enveloppe d’amplitude"], "Quantização em 4 níveis": ["4-level quantization", "Cuantización en 4 niveles", "Quantification à 4 niveaux"], "Cúbica": ["Cubic", "Cúbica", "Cubique"], "Terceiro harmônico": ["Third harmonic", "Tercer armónico", "Troisième harmonique"], "Triangular": ["Triangle", "Triangular", "Triangulaire"], "Saturação suave": ["Soft saturation", "Saturación suave", "Saturation douce"] };
      const name = names[hit[1]]?.[current === "en" ? 0 : current === "es" ? 1 : 2] || hit[1];
      return tr(`Applied: ${name} · ${hit[2]}`, `Aplicada: ${name} · ${hit[2]}`, `Appliquée : ${name} · ${hit[2]}`);
    }
    if ((hit = source.match(/^Seed ([A-F0-9]{8}):/))) return source;
    if ((hit = source.match(/^Mute retirado de (.+)\.$/)))
      return tr(`Unmuted ${hit[1]}.`, `Mute quitado de ${hit[1]}.`, `Sourdine retirée de ${hit[1]}.`);
    if ((hit = source.match(/^(.+) em mute\.$/)))
      return tr(`${hit[1]} muted.`, `${hit[1]} en mute.`, `${hit[1]} en sourdine.`);
    if ((hit = source.match(/^Solo ativo em (\d+) canal\(is\)\.$/)))
      return tr(`Solo active on ${hit[1]} channel(s).`, `Solo activo en ${hit[1]} canal(es).`, `Solo actif sur ${hit[1]} canal(aux).`);
    if ((hit = source.match(/^Três pêndulos pequenos acoplados por molas efetivas: κ = (.+), ζ = (.+)\. As vozes 1–3 seguem θᵢ\(t\); no ciclo 5, a fase se sustenta sem impor silêncio\.$/)))
      return tr(`Three small pendulums coupled by effective springs: κ = ${hit[1]}, ζ = ${hit[2]}. Voices 1–3 follow θᵢ(t); in cycle 5, phase is held without imposing silence.`, `Tres péndulos pequeños acoplados por resortes efectivos: κ = ${hit[1]}, ζ = ${hit[2]}. Las voces 1–3 siguen θᵢ(t); en el ciclo 5, la fase se mantiene sin imponer silencio.`, `Trois petits pendules couplés par des ressorts effectifs : κ = ${hit[1]}, ζ = ${hit[2]}. Les voix 1–3 suivent θᵢ(t) ; au cycle 5, la phase est maintenue sans imposer de silence.`);
    if ((hit = source.match(/^λ médio = (.+)\. λ = 0 mantém as vozes independentes; aumentar λ transmite mais movimento da voz anterior para a seguinte\.$/)))
      return tr(`Mean λ = ${hit[1]}. λ = 0 keeps voices independent; increasing λ transfers more motion from each voice to the next.`, `λ medio = ${hit[1]}. λ = 0 mantiene las voces independientes; al aumentar λ se transmite más movimiento de una voz a la siguiente.`, `λ moyen = ${hit[1]}. λ = 0 garde les voix indépendantes ; augmenter λ transmet davantage de mouvement d’une voix à la suivante.`);
    if ((hit = source.match(/^ENTRADAS · C1: V1 \+ baixo · C2: V1–V(\d+) \+ baixo · C3–5: todas \+ baixo · C6: V1–V(\d+) \+ baixo · C7: todas \+ baixo\. FASE: avança em C3–4, sustenta em C5 e retorna em C6–7\.$/)))
      return tr(`ENTRIES · C1: V1 + bass · C2: V1–V${hit[1]} + bass · C3–5: all + bass · C6: V1–V${hit[2]} + bass · C7: all + bass. PHASE: advances in C3–4, holds in C5 and returns in C6–7.`, `ENTRADAS · C1: V1 + bajo · C2: V1–V${hit[1]} + bajo · C3–5: todas + bajo · C6: V1–V${hit[2]} + bajo · C7: todas + bajo. FASE: avanza en C3–4, se mantiene en C5 y vuelve en C6–7.`, `ENTRÉES · C1 : V1 + basse · C2 : V1–V${hit[1]} + basse · C3–5 : toutes + basse · C6 : V1–V${hit[2]} + basse · C7 : toutes + basse. PHASE : avance en C3–4, se maintient en C5 et revient en C6–7.`);
    if (source === "O gráfico mostra o baixo calculado dos ataques vocais no início da seção Mudança. A grade abaixo é um molde de edição: ao editar uma célula, o baixo passa ao modo manual.")
      return tr("The graph shows the bass calculated from voice attacks at the start of the Change section. The grid below is an editing template: editing a cell switches the bass to manual mode.", "El gráfico muestra el bajo calculado a partir de los ataques vocales al inicio de la sección Cambio. La cuadrícula es una plantilla de edición: al editar una celda, el bajo pasa al modo manual.", "Le graphique montre la basse calculée à partir des attaques vocales au début de la section Changement. La grille est un modèle d’édition : modifier une cellule passe la basse en mode manuel.");
    if (source === "Clique em passo vazio para inserir; arraste o corpo para mudar altura e a borda direita para mudar duração. No modo de desenho, o traço cria ou altera os passos atravessados.")
      return tr("Click an empty step to add a note; drag its body to change pitch and its right edge to change duration. In drawing mode, the stroke creates or changes the steps it crosses.", "Haz clic en un paso vacío para insertar una nota; arrastra el cuerpo para cambiar la altura y el borde derecho para cambiar la duración. En modo de dibujo, el trazo crea o modifica los pasos atravesados.", "Cliquez sur un pas vide pour ajouter une note ; faites glisser son corps pour changer la hauteur et son bord droit pour changer la durée. En mode dessin, le trait crée ou modifie les pas traversés.");
    return null;
  }

  function localized(source) {
    if (translations[source]) return current === "pt" ? source : translations[source][current] || source;
    return translateDynamic(source);
  }

  function applyText(node) {
    if (node.nodeType !== Node.TEXT_NODE || !node.nodeValue.trim()) return;
    const remembered = textSources.get(node);
    const visible = node.nodeValue.trim();
    const original = remembered && visible === remembered.output
      ? remembered.source : visible;
    const translated = localized(original);
    if (translated === null) {
      textSources.delete(node);
      return;
    }
    const leading = node.nodeValue.match(/^\s*/)?.[0] || "";
    const trailing = node.nodeValue.match(/\s*$/)?.[0] || "";
    textSources.set(node, { source: original, output: translated });
    const next = leading + translated + trailing;
    if (node.nodeValue !== next) node.nodeValue = next;
  }

  function applyAttribute(element, attribute) {
    if (!element?.getAttribute) return;
    const value = element.getAttribute(attribute);
    if (value === null) return;
    let originals = attrSources.get(element);
    if (!originals) { originals = {}; attrSources.set(element, originals); }
    const remembered = originals[attribute];
    const source = remembered && value === remembered.output ? remembered.source : value;
    const translated = localized(source);
    if (translated === null) {
      delete originals[attribute];
      return;
    }
    originals[attribute] = { source, output: translated };
    if (value !== translated) element.setAttribute(attribute, translated);
  }

  function applyAttributes(root = document.body) {
    const elements = [root, ...root.querySelectorAll("*")];
    for (const element of elements)
      for (const attribute of ["aria-label", "title", "placeholder"])
        applyAttribute(element, attribute);
  }

  function translate(root = document.body) {
    const walker = document.createTreeWalker(root, NodeFilter.SHOW_TEXT);
    const nodes = [];
    while (walker.nextNode()) nodes.push(walker.currentNode);
    nodes.forEach(applyText);
    applyAttributes(root);
  }

  function apply(root = document.body) {
    translate(root);
    document.documentElement.lang = tags[current];
    document.getElementById("guide-language").value = current;
    document.dispatchEvent(new CustomEvent("batruc:languagechange", { detail: { language: current } }));
  }

  selector.addEventListener("change", () => {
    current = tags[selector.value] ? selector.value : "pt";
    try { localStorage.setItem("batruc-interface-language", current); } catch (_) {}
    apply();
  });
  document.addEventListener("batruc:languagechange", (event) => {
    if (event.detail?.language && tags[event.detail.language]) {
      current = event.detail.language;
      selector.value = current;
    }
  });
  const observer = new MutationObserver((records) => {
    for (const record of records) {
      if (record.type === "characterData") applyText(record.target);
      if (record.type === "attributes") applyAttribute(record.target, record.attributeName);
      for (const added of record.addedNodes) {
        if (added.nodeType === Node.TEXT_NODE) applyText(added);
        else if (added.nodeType === Node.ELEMENT_NODE) translate(added);
      }
    }
  });
  observer.observe(document.body, {
    childList: true, characterData: true, subtree: true, attributes: true,
    attributeFilter: ["aria-label", "title", "placeholder"]
  });
  apply();
  globalThis.BatrucI18n = {
    get language() { return current; },
    apply,
    t(source) { return localized(String(source)) ?? String(source); }
  };
})();

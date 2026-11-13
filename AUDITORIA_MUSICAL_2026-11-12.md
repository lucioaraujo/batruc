# Auditoria musical do Batruc — do conceito ao som (12/11/2026)

Pedido do autor (12/11): "o codex não tá dando conta, preciso que faça uma
auditoria que parta dos conceitos, do que foi discutido, do que foi feito, mas
ainda precisa ser melhorado pois carece de resultados musicais, de composição
generativa interessantes, o que está pendente e ou que precisa ser alterado";
"esse até agora é o instrumento que menos gostei do resultado, precisamos
melhorar"; "faça uma investigação aprofundada, desde o início do
desenvolvimento".

Autoria da auditoria: Claude Code. É leitura e diagnóstico, não decisão. As
propostas da §7 esperam o autor; nada foi alterado no comportamento musical.

## 1. Método e fontes

- **Documentos lidos:**
  - `README.md`: identidade, música, princípios, contrato conceito → som;
  - `TAREFAS.md` (11.121 linhas): os 333 títulos datados, de 10/10 a 12/11,
    e todos os relatos do autor localizáveis;
  - `OPERACOES_GENERATIVAS.md`, `RITMOS_E_CONTAGENS.md` e
    `MOVIMENTO_GRAFICO_E_SOM.md`, já lidos em 9–10/11 no estudo da família
    (`RASGO_MODULAR/dossies/ESTUDO_familia_rasgo_para_o_modular.md` §4.3).
- **Código lido:** `src/core/CompositionCore.{hpp,cpp}` (`buildEventPlan`,
  `seededRecipeSelection`) e `src/core/BassComposition.cpp`.
- **Medidas:** contagens no diário. Linhas que declaram escuta pendente:
  35. Linhas sobre paridade HTML/JUCE: 124. Versões de schema de cena
  citadas: 33. Iterações de logo: 14 (V1–V14). Linhas sobre MIDI/GMD: 314.
- **Limite:** não ouvi o instrumento. As conclusões sobre o som vêm do código
  e dos relatos do autor, não de escuta.

## 2. O conceito — o que o Batruc prometeu ser

Do README, nas palavras do projeto:

- "compor com conceitos matemáticos e físicos: tornar fórmulas, teorias,
  axiomas, problemas e movimentos em regras audíveis de forma, ritmo, alturas,
  timbre, dinâmica e relação entre vozes";
- "um instrumento musical sobretudo de ritmo";
- "O objetivo é música com desenvolvimento, não apenas uma demonstração de
  polirritmia. Investigar ataques, articulações, acentos, pausas, motivos,
  entradas, saídas, diálogo entre vozes e forma de longa duração";
- "O sequenciador torna essas propostas tocáveis; não deve reduzir o
  instrumento a uma grade de disparos";
- o contrato: **conceito → modelo → estado/evento → mapeamento → realização →
  escuta**, separando resultado matemático, simulação física e analogia
  composicional.

O conceito é forte e original, e é o mais exigente da família.

## 3. Linha do tempo — o que foi feito

| Período | Foco dominante |
|---|---|
| 10–11/10 | Abertura, consulta ao Atlas, corpo sonoro (bateria, percussão, acústica) |
| 24/10 | Hipótese "máquina de composição matemática e física"; maquete HTML animada; fórmulas combináveis; intenção |
| 25/10 | **Explosão de amplitude** (~70 registros num dia): renomeação Trucmetry → Batruc, baixo com ADSR/VCF/LFO, geometria triangular, Pascal/Fibonacci/Fourier, polirritmia, forças e ruptura, ecologia das vozes, Gramani, cenas, WAV, banco local |
| 26–28/10 | **Paridade HTML/JUCE**: duas implementações do mesmo instrumento conferidas número a número; forma em sete ciclos; A/B offline |
| 29/10 | Cicloide, catenária, bilhar, anéis, epiciclos, relações ecológicas |
| 30/10 | **Logo** (14 versões); auditoria JUCE: "repetição, baixo e saturação" |
| 01–02/11 | CPU, lentidão ao trocar seed, pêndulo triplo, Lissajous, Stengel, Poincaré, redesenho do layout em três campos, Orbit/Study |
| 03–07/11 | Layout, Mixer, editor do Study, Learn, textos, fonte MIDI |
| 08–11/11 | **MIDI GM / Groove MIDI Dataset**: importar baterias humanas, timbres por nota GM, CC4, polifonia, pickups por seed, metadados, recombinação por compasso (~85 registros só em 11/11) |
| 12/11 | Trajetória de pan desenhada por stem; registro de derivados MIDI (schema 57) |

## 4. O que o motor realmente faz (evidência no código)

1. **O ritmo "matemático" é um trem de pulsos regulares.** Em
   `buildEventPlan`, cada voz é uma senoide com `cyclesPerSection` ciclos por
   seção, e os ataques caem nos cruzamentos do zero (fases 0 e ½) ou nos
   extremos (¼ e ¾). Resultado: **2 ataques igualmente espaçados por ciclo**,
   deslocados pela fase. Duas vozes dão uma polirritmia N:M de pulsos
   isócronos, e nada dentro do ciclo: nem figura, nem motivo, nem síncope,
   nem pausa interna.
2. **As fórmulas e as geometrias filtram ou ponderam esses pulsos, não os
   compõem.** O campo XY usa as fases das vozes 1 e 2 (uma Lissajous); as
   leis de intensidade, os modos de padrão (`adhesivePads`, `diagonalLanes`),
   o bilhar, a cicloide e o Fourier mudam **intensidade e aceitação** de
   ataques que já estão na grade. A matemática aparece no gráfico mais do que
   no ritmo.
3. **A seed escolhe pouco no que se ouve.**
   - O ciclo da voz 2 sai de **5 valores** (1, 2, 3, 5, 7), mais uma fase entre
     101, uma regra entre 2 e um padrão entre 3.
   - Muitos genes novos (família sonora entre 17, groove, intenção,
     dinâmica, compasso, chip-8, geometria, Lissajous) alteram cor e
     visualização; quase nenhum altera a estrutura rítmica.
   - Daí o relato do autor (11/11): "as seeds eram previsíveis".
4. ~~Teto de densidade de 65 eventos por seção~~ — **correção (12/11): erro
   de leitura.** `kMaxEventsPerSection = 65'536`; o separador de milhar do
   C++ cortou o número na busca. Não há limite prático de densidade. A
   escassez vem da regra de dois ataques por ciclo (item 1), não de um teto.
5. **A forma é um molde fixo:** sete ciclos (Construção, Mudança, Pausa,
   Retorno) iguais para toda seed, sem desenvolvimento de material.
6. **O baixo deriva da agenda das vozes,** numa grade de passos (durações 1,
   2, 4, 8 e 16; repetições 1, 2, 4 e 8), com modos de frase (loop,
   respiração, cadência, pergunta e resposta). Herda a pouca variedade das
   vozes. O autor relatou em 11/11 que ele "voltava a soar igual"; uma causa
   foi corrigida, a raiz não.
7. **O corpo sonoro é sintético e parecido entre si:** ressonadores modais e
   ruído, sem samples. O autor, em 08/11: "mudar a família quase não muda o
   som". As diferenças foram ampliadas, mas o próprio diário diz que "não há
   sample acústico nem garantia" de fidelidade.
8. **A virada para o MIDI (08–12/11) deslocou o centro.** Importar uma
   bateria humana do GMD e variar seus acentos ("a seed preserva o groove
   MIDI", "não desloca os ataques da fonte") é tocar um groove pronto com
   leves variações. Pode soar melhor que a grade de pulsos, mas **não é a
   composição matemática e física** que define o instrumento, e consumiu os
   últimos cinco dias.

## 5. Diagnóstico — por que falta resultado musical

Em ordem de peso:

1. **O núcleo gera pulsos, não música.** A cadeia prometida (conceito → modelo
   → evento → mapeamento) para no primeiro mapeamento possível: "cruzou o
   zero, toca". Não existe a camada que transforma o modelo em **figuras
   rítmicas, motivos, frases e forma**, justamente o que o README chama de
   objetivo.
2. **Amplitude no lugar de profundidade.** Dezenas de conceitos entraram como
   "primeira prova" (Fourier, cicloide, catenária, bilhar, Lissajous, Stengel,
   Poincaré, anéis, pêndulos, ecologia, Gramani), cada um com "escuta
   pendente" (35 registros). Nenhum foi levado até soar bem antes do
   próximo começar.
3. **Infraestrutura consumindo o tempo de composição:**
   - duas implementações mantidas em paridade (HTML e JUCE);
   - 57 versões de formato de cena em um mês;
   - 14 versões de logo;
   - auditorias de layout, tooltips em 3 línguas e manifestos de proveniência
     MIDI.

   O próprio autor já tinha corrigido a prioridade (25/10): "o eixo do Batruc
   é o instrumento e a composição a partir dos conceitos matemáticos/físicos";
   o sistema de registros "fica subordinado a esse trabalho".
4. **Validação por teste, não por escuta.** Fixtures de paridade e de schema
   passam (18/18), mas medem consistência, não música. O diário repete "isso
   não substitui a escuta"; a escuta raramente aconteceu, e quando aconteceu
   ("pouca diferença", "quase não muda o som", "previsíveis") os ajustes
   foram locais.
5. **Timbre fraco para um instrumento de ritmo.** Bateria sintética
   indistinta tira o corpo de qualquer boa ideia rítmica.
6. **Documentação inchada.** O `TAREFAS.md` tem 11 mil linhas e o README
   quase 2,8 mil: fica difícil ver o que importa, inclusive para quem
   desenvolve.

## 6. O que vale preservar

- **O conceito** e o contrato conceito → som, com honestidade de nome (o
  melhor texto de método da família).
- **Os modelos já calculados** (pêndulos acoplados, bilhar, epiciclos,
  cicloide, catenária, campos) como fontes de **trajetórias e eventos**, a
  reaproveitar com uma tradução musical melhor.
- **Relações ecológicas** (apoio, imitação, afastamento, competição) e as
  **funções do baixo** (apoio, resposta nas lacunas, primeiro plano).
- **Operações sobre células** (rotação, redistribuição euclidiana, interlocking,
  densidade, recombinação, contorno de intensidade) e o **Study** como
  editor: são ferramentas de composição de verdade.
- A **pesquisa de contagens** (Gramani, Rítmica; polígonos inscritos; casos
  Tool/Meshuggah) e a **bibliografia** (Senn 2016 contra a humanização
  aleatória; Hove 2014 sobre o grave).
- A estabilidade técnica: áudio sem cliques, cenas reproduzíveis, testes.

## 7. O que precisa mudar — proposta para o autor

**Processo (vale imediatamente):**

- **Congelar a infraestrutura:** nada de schema, logo, layout, paridade ou
  metadados novos até o Batruc soar bem.
- **Uma implementação só:** o JUCE. A maquete HTML vira arquivo histórico; a
  paridade deixa de ser trabalho.
- **Portão de escuta:** cada mudança musical vem com um A/B gravado (antes e
  depois, a mesma seed), e só segue com o OK do autor.
- **Diário curto:** registros novos com no máximo uma tela; o histórico atual
  fica como arquivo.

**Música (o trabalho principal, em lotes pequenos):**

1. **Uma camada de composição entre o modelo e o evento.** O modelo (fórmula,
   pêndulo, bilhar, órbita) passa a gerar **figuras**: células de ataques com
   subdivisões internas, acentos, durações e pausas, tiradas da forma da curva
   (velocidade, curvatura, inversões, distância entre vozes). Não mais "cruzou
   o zero = um pulso".
2. **Motivo e desenvolvimento:** cada seed define um motivo rítmico curto e o
   desenvolve com as operações que já existem: rotação, aumento e
   diminuição, inversão de papéis (Gramani), interlocking, densidade,
   recombinação. A forma deixa de ser um molde fixo de sete ciclos e passa a
   ser um percurso do material (exposição, variação, ruptura, retorno).
3. **Seed que muda o que se ouve:** ampliar o espaço rítmico (métrica,
   subdivisão, quiálteras, figuras, densidade por voz, quem conduz), não só
   cor e gráfico. Medir a diversidade entre seeds por números (ataques,
   intervalos entre ataques, síncopes) antes de pedir escuta.
4. ~~Subir o teto de eventos~~ (não há teto prático; ver §4, item 4).
5. **Baixo com voz própria:** frases com contorno e harmonia (graus, notas de
   passagem, resposta nas lacunas), não só a derivação da agenda.
6. **Corpo sonoro:** decidir entre melhorar a síntese por papel (bumbo,
   caixa, chimbal, toms com modelos dedicados) ou usar samples com licença
   clara. Sem isso, nenhum ritmo convence.
7. **Lugar do MIDI:** o GMD fica como **material de estudo e referência**
   (comparação, ensino, análise), não como o motor. O centro volta à
   composição a partir dos conceitos.

**Primeiro lote sugerido, para validar o caminho:** um único modelo (por
exemplo, o pêndulo duplo ou o bilhar) gerando figuras e motivo para duas
vozes e baixo, com corpo sonoro de bateria melhorado só para bumbo, caixa e
chimbal. Três seeds gravadas em A/B contra o Batruc atual. Se não soar melhor,
paramos e repensamos antes de seguir.

## 8. Pendências que continuam valendo

- A escuta de tudo o que está marcado "escuta pendente" deixa de ser
  prioridade: o motor vai mudar.
- Abertos e úteis:
  - o editor de trajetória de pan (12/11): inspeção visual;
  - o registro de derivados MIDI (12/11, schema 57): os parâmetros
    detalhados por operação e a verificação de licença e atribuição.
- O `midis/groove/` e o `.zip` do GMD não estão em commit. A licença (CC BY
  4.0, Google LLC) e a exigência de atribuição já estão registradas em
  `midis/README.md`.
- A pasta `--archive-smoke/` na raiz do Batruc, de 30/10, guarda um render de
  teste da maquete web (`batruc-5EED2027-web-offline.*`). O nome com "--"
  sugere um argumento mal passado. Decidir com o autor se fica no arquivo ou
  se é removida.

## 9. Execução

- **Lote 1 (12/11), camada modelo → figuras:** feito no core e no render
  offline; A/B gravado e aguardando a escuta do autor. Detalhes no ponto de
  retomada do `TAREFAS.md`.


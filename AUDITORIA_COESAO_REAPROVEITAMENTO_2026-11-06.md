# Batruc — auditoria de coesão e reaproveitamento

**Data:** 2026-11-06
**Estado:** diagnóstico técnico-musical; decisões de prioridade registradas,
implementação incremental pendente
**Objetivo:** recuperar código e pesquisa já investidos, reduzir mecanismos
paralelos ou inacessíveis e transformar o Batruc num instrumento coerente de
composição rítmica, geometria editável, síntese percussiva e forma longa.

## 1. Síntese do diagnóstico

O problema principal do Batruc não é falta de recursos. O instrumento já tem
um núcleo incomum e valioso: células rítmicas editáveis, geometria que pode
agendar eventos, oito vozes, relações ecológicas, desenvolvimento por
passagens, importação MIDI, transformações de baixo, cenas versionadas,
gravação, metrônomo, Mixer e proteção de saída.

O problema é que esse trabalho cresceu em **ilhas**:

- alguns mecanismos estão testados apenas no core e não chegam ao músico;
- alguns aparecem como gráficos, mas não alteram a agenda sonora;
- outros atuam no áudio, porém não mostram origem, transformação e resultado;
- funções equivalentes foram implementadas no protótipo web e no JUCE, criando
  duas fontes que já divergem;
- a interface conserva componentes antigos ocultos, como Áreas/Stengel;
- célula, motivo, passagem, seção, duração da peça e cena ainda não formam um
  único modelo composicional.

Portanto, a direção saudável não é adicionar mais botões. É estabelecer uma
espinha dorsal única:

```text
FONTE
manual · seed · MIDI
  ↓
CÉLULAS RÍTMICAS
ataques · força · microtempo · proveniência
  ↓
OPERAÇÕES
rotação · densidade · interlocking · retrógrado · recombinação
  ↓
FRASES E FORMA
motivo · seção · repetição · desenvolvimento · coda · duração
  ↓
PERFORMANCE
acento · gesto · memória · resposta · variação limitada
  ↓
CORPOS SONOROS
excitação · ressonador · ruído · envelope · processamento
  ↓
MIXER · BAIXO · SAÍDA · GRAVAÇÃO
```

Gráficos, áudio, cenas e exportação devem ler esse mesmo plano. Um gráfico não
deve manter uma segunda composição implícita.

## 2. Evidência examinada

- núcleo C++ em `src/core/`, host JUCE em `src/app/Main.cpp`, renderer offline,
  testes e protótipo web;
- README, tarefas, operações generativas, bibliografia e auditorias locais;
- governança e inventário transversal do RASGO;
- pesquisas `CRI-SEQ-001`, `CRI-DRF-001`, `CRI-PER-001` e `CRI-BAS-001`;
- precedentes do Antitotem, Aquorbium, Rasgo Modular, Rasgo Synth Studio e
  Trioio, sem criar dependência entre instrumentos.

Dimensão atual que merece atenção: `Main.cpp` tem aproximadamente **15,6 mil
linhas** e mistura componentes gráficos, estado de cena, importação, gravação,
composição, navegação e parte da coordenação de áudio. `CompositionCore.cpp`
tem aproximadamente **2,8 mil linhas**. Esse tamanho não é um defeito por si,
mas hoje dificulta saber qual mecanismo é canônico e torna cada mudança de
layout arriscada.

## 3. Código valioso subutilizado

| Ativo existente | Estado observado | Potencial perdido | Decisão recomendada |
| --- | --- | --- | --- |
| `algorithmicNextSeed()` | **Recuperado em 06/11:** o botão nativo agora usa a busca contrastiva já existente; o startup continua aleatório | A operação deixou de ser código subutilizado e passou a variar vozes, ciclos, fase, regra, padrão e intensidade a partir da realização atual | Preservar o caminho contrastivo como ação principal; oferecer depois “nova aleatória” como ação secundária e documentar as dimensões traváveis |
| `RhythmicCell` + editor Study | É a melhor fonte comum atual; recebe gesto manual e MIDI e já chega ao renderer | Ainda não constitui motivo/frase/seção, não guarda proveniência MIDI e não oferece histórico de operações | Promover a célula a unidade canônica de composição, com ID, origem, operação e undo |
| `MidiRhythmImport` | Núcleo bounded e fluxo visível concluídos | Importação termina na aplicação; fonte/licença/recorte e transformação não entram na cena | Acrescentar `CellProvenance` e sidecar/manifesto antes de incorporar corpus externo |
| `invertEventPlanVoices()` | Core + teste, sem uso no app | Operação musical pronta permanece invisível | Adaptar para operação de célula/frase, com prévia A/B e desfazer; não operar diretamente em agenda efêmera |
| `invertChordVoicing()`, `invertBassPitchContour()`, `retrogradeBassScore()`, `recombineBassBars()`, `recalculateBassScore()`, `inversePhaseRotation()` | Várias funções existem somente em testes; variantes de performance de baixo já chegam à UI | Duas famílias de operação, uma acessível e outra abandonada | Unificar em `OperationPlan`; preservar apenas operações que atuem no modelo canônico e remover duplicatas após migração |
| `buildPendulumEventPlan()` | Usado no renderer offline; a janela nativa mostra estudo pendular, mas não usa o mesmo plano sonoro | Visual e render offline podem contar histórias diferentes | Ou ligar o plano pendular ao modelo canônico com chave explícita, ou rotular o estudo como análise e retirá-lo do fluxo principal |
| geometria/Fourier/bilhar/Lissajous | `geometryCurveEventPositions()` e V1 podem afetar eventos; grande parte das demais vistas é analítica | Pesquisa forte aparece como decoração para várias vozes | Generalizar fonte geométrica por voz/célula com orçamento limitado; mostrar no inspetor quais destinos são realmente afetados |
| `PassageDevelopment` | Atua em ataques de voz e baixo, com cinco papéis derivados da passagem | Forma fixa de sete ciclos e repetição implícita escondem a potência do mecanismo | Transformar papéis em seções editáveis de `PiecePlan`, preservando o algoritmo como proposta seeded |
| relações ecológicas | Plano bounded e comportamento de resposta já existem no core/offline e no baixo | O músico não vê quem escuta, responde ou resiste | Expor relações no nível de frase, não como mais um efeito; desenhar setas/estado e permitir desligar ou congelar |
| cenas schema 40 | Persistência ampla e compatibilidade histórica são investimentos sólidos | Serialização, captura de UI e modelo musical vivem no monólito; MIDI ainda perde proveniência | Extrair `CompositionDocument` e codecs de cena; manter leitura 1–40 num adaptador legado |
| famílias sonoras | 17 famílias distribuem matérias pelos oito stems; em 06/11 a família 8-bit ganhou `chip8` e controles de duty, clock, bits e ruído persistidos | As demais ainda são sobretudo combinações estilizadas; no chip faltam LFSR e envelope dedicados | Chamar de **paletas iniciais** e construir editor de timbre por blocos antes de multiplicar famílias |
| proteção de saída e gravação | `OutputSafety`, REC e sidecar já existem | Medidores por stem e comparação A/B ainda não comprovam a consequência das operações | Reusar a telemetria para medidores reais e normalização perceptualmente comparável no A/B |

## 4. Código incoerente, defasado ou improdutivo

### 4.1 Áreas/Stengel: pesquisa preservável, interface encerrada

A UI foi ocultada, mas continuam no `Main.cpp` a classe gráfica, três sliders,
rótulos, combo, botão, layout, tradução, callbacks e rotinas de atualização.
Isso é código inacessível, não um recurso em incubação. A relação matemática
pura e seus testes podem permanecer como registro de pesquisa; a camada JUCE
deve ser removida. Se o cálculo não ganhar um mapeamento musical documentado,
também deve sair do build normal e permanecer somente no histórico ou num
teste de pesquisa.

Não substituir Stengel por outro painel abstrato. O sucessor útil é
**Morfologia rítmica**, descrito na seção 7.

### 4.2 Duas aplicações que competem pela verdade

O protótipo web e o JUCE repetem seleção seeded, geometria, baixo, cenas e
interface. Cinco testes da suíte completa já registram divergências de
paridade, principalmente no baixo, curva cycloid e hard-pan. Como a decisão
vigente coloca o JUCE como instrumento prioritário, continuar desenvolvendo os
dois em paralelo desperdiça tempo.

Decisão recomendada:

- congelar o web como **protótipo histórico e referência visual**;
- manter somente testes de contratos que ainda tenham valor documental;
- tornar C++ a fonte canônica de composição e áudio;
- se uma interface web voltar a ser necessária, consumir um formato/engine
  exportado, não reimplementar a composição em JavaScript.

### 4.3 Viewport usado como paginação

Editor, Mixer e Baixo agora cabem e se comportam como páginas em 1080×800, mas
internamente continuam empilhados num documento alto e o cabeçalho apenas muda
o offset do `Viewport`. Funciona visualmente, porém mantém cálculo de posições,
visibilidade, animação e chrome preso ao modelo antigo.

Substituição recomendada: `PageHost` com três componentes filhos, somente um
visível e participante do layout. Isso reduz repaints fora da tela, elimina
coordenadas globais frágeis e permite que cada módulo tenha testes de bounds
próprios.

### 4.4 Monólito de interface e estado

O `Main.cpp` contém pelo menos cinco responsabilidades que devem ser separadas
sem alterar o som:

1. `CompositionDocument` — estado autoral editável;
2. `SceneCodec` — leitura/escrita e compatibilidade de schemas;
3. `EditorPage`, `MixerPage`, `BassPage` — apresentação e gestos;
4. `RecordingController` — REC, fila e sidecar;
5. `EngineCoordinator` — prepara planos fora do callback e publica nas
   fronteiras musicais.

A extração deve ser incremental e coberta por testes. Não reescrever tudo de
uma vez.

### 4.5 Limite histórico de 12 versus instrumento de oito vozes

O armazenamento de doze slots ajuda a ler cenas antigas, mas vaza para loops,
arrays, testes e decisões seeded. A camada musical deve trabalhar com oito
vozes; o codec legado converte cenas antigas para oito e preserva os dados
extras somente como payload de compatibilidade, sem fazê-los participar da
composição.

### 4.6 Nomes e componentes que perderam a função

`geometryButton_` agora mostra MIDI; `geometryModeBox_` e
`geometryAreaModeButton_` continuam declarados embora não devam reaparecer.
Esses resíduos aumentam o risco de ligar o controle errado. Renomear ao extrair
a página (`midiShortcutButton_`) e remover componentes obsoletos.

## 5. Conceitos RASGO que realmente fortalecem o Batruc

O reaproveitamento deve copiar e adaptar inteligência estrutural a partir de
commit conhecido, nunca ligar o Batruc a uma pasta irmã.

### Antitotem — transformação com memória

- A pesquisa `CRI-SEQ-001` separa **passo, ciclo e frase** e descreve acento,
  probabilidade, condição, ratchet, ghost, microtiming causal, glitch
  estrutural, rotação, influência euclidiana e morph.
- A deriva do Antitotem preserva âncora, momentum, histórico e retorno, em vez
  de somar aleatoriedade independente.

Aplicação Batruc: usar histórico de operações sobre células e frases. A
variação deve mover-se a partir de um estado audível, poder retornar e nunca
destruir silenciosamente a edição manual.

### Aquorbium — geometria como sequenciador

O `GeometricSequencer` já prova uma arquitetura com padrão euclidiano, rotação,
cinco lanes, gate, acento e intenção contínua. Não copiar sua ecologia ou sua
sonoridade. Adaptar a separação entre forma compartilhada, fase por lane,
evento e influência humana.

Aplicação Batruc: o polígono deve ser uma fonte/transformação de células, não
apenas uma animação. Cada vértice ou setor precisa corresponder a ataque,
força, duração ou relação declarada.

### Rasgo Modular — vocabulário de operação explícita

`PHRASE`, `DECISION`, `FUNCTION`, `EUCLID`, `SWITCH`, `VARIA`, `MUDA`,
`EVOLUI` e `CRUZA` oferecem uma boa separação de responsabilidades. O Batruc
não precisa virar rack modular; deve adotar a clareza fonte → operação →
destino, os limites bounded, o congelamento real em zero e as travas por
dimensão.

### Rasgo Synth Studio — identidade, forma e duração

`CompositionIdentity`, `GenerativeArc` e `DurationTarget` distinguem identidade
da seed, gramática de seções e ajuste de duração sem cortar arbitrariamente a
obra. O Batruc já pediu 65 ciclos, 4:20 e motivos repetidos até 5:00.

Aplicação Batruc: copiar somente o contrato e, se depois for necessário, uma
versão fixada/adaptada. A forma do Batruc deve operar em ciclos e células,
permitir edição manual e expor introdução, desenvolvimento, transição, clímax,
retorno e coda.

### Trioio — escuta como memória autoral

O Trioio registra aceitar/transformar/rejeitar por resposta, seção ou trecho,
com esquecimento limitado e proveniência do que soou.

Aplicação Batruc: primeiro implementar A/B honesto de operação e permitir ao
músico guardar “manter”, “transformar” ou “voltar”. Não importar um sistema de
aprendizagem inteiro antes de o Batruc produzir versões realmente distintas.

### Performance Engine — gesto específico da matéria

`HitGesture` e `PatternGesture` são mais adequados ao Batruc do que aplicar uma
morfologia tonal genérica. Acento pode correlacionar velocidade, dureza,
brilho, ruído, duração e espaço, mas cada corpo sonoro traduz a intenção de
modo próprio.

## 6. Arquitetura de coesão proposta

### 6.1 `CompositionDocument`

Fonte única editável, sem JUCE e sem DSP:

```text
CompositionDocument
  seed/origem/manual
  meter/tempo/groove
  CellLibrary[até limite bounded]
  VoicePlan[8]
  PiecePlan
  TimbrePlan[8]
  BassPlan
  OperationHistory
  Provenance
```

### 6.2 `CellOperation`

Uma operação contém:

```text
tipo · fonte · destino · escopo · intensidade · seed opcional
parâmetros travados · estado anterior · estado resultante
```

Primeira lista, toda audível e reversível: rotação, deslocamento de fase,
densidade, força, interlocking, retrógrado, troca entre vozes, recombinação de
trechos e influência euclidiana.

### 6.3 `PiecePlan`

Separa motivo e obra:

- ciclos do motivo;
- número de repetições ou ciclos totais;
- duração-alvo;
- seções com papel, comprimento e transformações;
- regra de finalização em fronteira musical;
- proposta seeded, manual ou híbrida.

### 6.4 publicação segura no áudio

Toda edição é validada e preparada fora do callback. O engine recebe snapshot
imutável/bounded na próxima fronteira escolhida. Gráficos leem o mesmo snapshot
publicado e podem mostrar, em outra cor, a edição ainda não aplicada.

## 7. Substituição saudável para Áreas: Morfologia rítmica

Em vez de três sliders longos e uma relação sem consequência musical, usar um
inspetor compacto ligado ao gráfico principal:

- **Operação** — botão/menu: Rotacionar, Rarear/Densificar, Intercalar,
  Espelhar, Recompor;
- **Quantidade** — knob grande com unidade contextual (passos, %, graus ou
  ataques), não um slider genérico;
- **Escopo** — voz, seleção, célula, frase ou seção;
- **Influência** — knob 0–100%; zero é bypass bit a bit;
- **Aplicar / Desfazer / Comparar** — ações explícitas;
- **Origem → Resultado** — miniatura dupla no próprio Study;
- **Destino musical** — rótulo obrigatório: tempo, força, gate, timbre ou
  relação.

O gesto direto no círculo continua prioritário. O inspetor serve para operações
exatas, seedadas ou repetíveis, não para substituir a manipulação gráfica.

## 8. Plano de recuperação por prioridade

### P0 — reduzir incoerência sem mudar a identidade sonora

1. [concluído em 06/11] ligar `Nova seed` nativo a
   `algorithmicNextSeed()` e cobrir determinismo/contraste por teste;
2. [concluído em 06/11] remover toda a UI inacessível de Stengel e manter o
   cálculo puro somente como pesquisa/teste histórico; o atalho já é MIDI;
3. declarar o web congelado como protótipo histórico;
4. extrair as três páginas para um `PageHost` verdadeiro;
5. manter a suíte atual e registrar as cinco regressões já conhecidas sem
   ajustar fixtures para escondê-las.

**Pronto quando:** as três páginas não dependerem de coordenadas verticais
globais. As parcelas de Stengel oculto e troca de seed por distância musical
já estão concluídas e validadas.

### P1 — uma única composição para gráfico, áudio e cena

1. criar `CompositionDocument` e codec de cena separado;
2. migrar `RhythmicCell`, Study e MIDI para `CellLibrary` com proveniência;
3. criar histórico bounded de operações e undo/redo;
4. expor inicialmente rotação, densidade, força e interlocking;
5. fazer gráfico e renderer consumirem o mesmo snapshot.

**Pronto quando:** a mesma edição for audível, visível, reversível e persistir
ao salvar/reabrir.

### P2 — forma longa e desenvolvimento

1. criar `PiecePlan` com motivo, ciclos totais e duração-alvo;
2. adaptar `PassageDevelopment` como gramática de seções;
3. acrescentar introdução, desenvolvimento, transição, clímax, retorno e coda;
4. mostrar duração estimada antes de tocar;
5. permitir seed/manual/híbrido.

**Pronto quando:** 8 ciclos repetidos até 5:00, 65 ciclos e uma peça de 4:20
forem representáveis, terminarem musicalmente e reabrirem de modo determinista.

### P3 — timbre e performance percussiva

1. transformar as 17 famílias em paletas documentadas;
2. criar `TimbrePlan` por voz: excitação, corpo/ressonador, espectro, ruído,
   envelope, saturação e modulação;
3. adaptar `HitGesture`/`PatternGesture` para acento multidimensional;
4. acrescentar glitch estrutural antes de efeitos de áudio;
5. implementar medidores reais e A/B com ganho comparável.

**Pronto quando:** uma célula idêntica produzir famílias claramente distintas,
o usuário puder criar/salvar timbre próprio e cada macro mostrar consequência
sonora mensurável.

### P4 — escuta e memória

Somente depois de P1–P3: guardar versões aprovadas, comparar trechos e usar
preferências do músico com peso e esquecimento limitados. Não chamar isso de
aprendizado enquanto for apenas um banco de escolhas.

## 9. O que preservar, aprofundar ou encerrar

### Preservar e integrar agora

- `RhythmicCell`, Study e importador MIDI;
- geometria que realmente agenda eventos;
- passagem/desenvolvimento;
- baixo especializado e suas operações já ligadas;
- cenas, gravação, metrônomo, Mixer e segurança de saída;
- seed determinística e algoritmo contrastivo.

### Aprofundar por pesquisa aplicada

- microtiming com memória, não jitter independente;
- influência euclidiana sobre célula existente;
- interlocking entre vozes;
- morfologia de hit/padrão;
- forma longa e duração-alvo;
- síntese percussiva original por blocos;
- escuta autoral por versão/trecho.

### Encerrar ou arquivar

- UI Stengel sem mapeamento musical;
- desenvolvimento paralelo do protótipo web;
- gráficos que não declarem se são fonte, transformação ou análise;
- famílias estilísticas tratadas como simples troca de nomes;
- novas funções matemáticas sem destino musical e teste A/B.

## 10. Limites desta auditoria

Esta passagem foi uma auditoria de fonte, documentação e integrações. Ela não
constitui escuta musical, teste prolongado de performance, revisão linha a
linha de todas as 23 mil linhas de C++ nem autorização para copiar código de
instrumentos irmãos. Cada adaptação exige commit de origem, licença,
proveniência, teste local e tradução para a identidade do Batruc.

As correções de layout e MIDI desta sessão foram compiladas e capturadas; as
decisões arquiteturais acima ainda são plano. A ordem P0→P4 evita perder mais
tempo em recursos que não chegam à composição.

## 11. Adendo de direção musical — 08/11/2026

**Objetivo:** responder à escuta do autor: seeds semelhantes, famílias pouco
distintas, stems cuja função não se reconhece e percussão sem intenção musical.
Esta revisão examinou o fluxo atual de seed, MIDI, células, renderer, Mixer e
baixo. É uma auditoria de código e conceito; não houve nova escuta, build nem
teste de GUI nesta etapa.

### Decisão de identidade

O Batruc deve funcionar primeiro como **instrumento de composição e execução
rítmica**: a pessoa escolhe ou escreve uma célula, entende a função de cada
stem, transforma o material e ouve a consequência no mesmo tempo musical.
Geometria, relações ecológicas e seed são a gramática autoral de transformação
desse material. Uma combinação matematicamente válida de ataques não basta para
constituir um groove; a referência musical precisa ser audível antes da
transformação. O baixo responde a esse mesmo arranjo, em vez de parecer um
instrumento paralelo.

Separar cinco decisões que hoje se confundem: **quando** cada papel ataca
(ritmo), **qual** peça ou articulação toca (mapa de instrumentos), **como** o
ataque é executado (força, microtempo, duração e acento), **com que corpo
sonoro** ele soa (kit/paleta) e **como** essas células evoluem (forma). Um MIDI
de bateria fornece sobretudo as três primeiras. Não contém o som de um bumbo
ou a identidade de um samba. O rótulo editorial de estilo do GMD não é uma
gramática completa nem validação cultural da tradição.

### Onde o fluxo atual se rompe

1. Sem MIDI ativo, `applySeedInput()` limpa as células e os ataques voltam a
   derivar dos ciclos, fases e regras de cada voz em `buildEventPlan()`. O seed
   escolhe muitos parâmetros independentes, mas não parte de um motivo rítmico
   com funções de bateria. **Inferência musical:** isso ajuda a explicar a
   sensação de ataques fortuitos, mesmo com determinismo e relações formais.
2. Com MIDI, o importador preserva notas GM em oito papéis e a seed varia apenas
   a força dos ataques, sem deslocar suas posições. Esse caminho é uma base
   musical mais clara. Porém `applyImportedMidiRhythm()` redefine a família
   para Original/manual e atribui modelos genéricos aos oito stems. A escolha
   posterior de uma família mantém esses mesmos modelos.
3. `soundFamilyVoiceSounds()` devolve exatamente o mesmo mapa de oito modelos
   para as 17 famílias. `setChannelTimbreProfile()` altera cauda, ruído,
   inclinação espectral e ganho/duração do corpo. A família chega ao renderer,
   mas seu nome não seleciona um kit nem uma articulação estilística própria.
   Assim, a pequena diferença percebida pelo autor é compatível com o código;
   só a comparação por escuta poderá medir o quanto cada perfil se distingue.
4. `seededRecipeSelection()` ainda calcula `soundFamilyIndex` e `patternIndex`,
   e `algorithmicNextSeed()` dá 180 pontos ao primeiro e 70 ao segundo. O host
   preserva a família e força **Campo livre**. Portanto, a busca contrastiva
   premia diferenças que a aplicação descarta. O tooltip da seed e o P0 de
   `TAREFAS.md` também ficaram desatualizados.
5. O desenho prepara `displayPlan_` a partir da receita e das células atuais;
   o áudio conserva seu próprio estado e recebe alterações por slots/atomics.
   Isso é uma arquitetura legítima para tempo real, mas hoje não há um
   `PlaybackSnapshot` identificável que prove que vista, áudio e cena exibem
   a mesma versão musical. `commitRhythmicCellEdit()` ainda pede reset da forma
   durante a reprodução. **Hipótese a testar:** parte da pausa/retomada
   relatada pelo autor pode vir desse reset, além da atualização na fronteira
   de compasso; não há medição de latência nesta auditoria.
6. A redução formal de vozes é desativada no callback quando há fonte MIDI:
   `setFormalVoiceReductionEnabled(!midiSourceActive_)`. Logo, a afirmação
   anterior de que C1 toca só V1 também com MIDI está defasada na documentação.

### Arquitetura mínima para um instrumento tocável

Não é necessário substituir o core. Reusar `RhythmicCell`, nota GM por ataque,
`EventPlan`, renderer, BassPerformance e cenas, dando a eles um contrato único:

```text
Fonte (manual | MIDI com proveniência | proposta algorítmica)
  → Células e papéis V1–V8
  → Operações explícitas e reversíveis
  → Arranjo com gesto, kit e relação do baixo
  → Agenda de execução versionada
  → Áudio, gráfico, cena e exportação da mesma versão
```

Nesse contrato, uma seed pode propor variações **limitadas por papel**: alterar
acento e densidade de chimbal com limites audíveis, oferecer deslocamento ou
resposta de caixa, preservar âncoras de bumbo e o motivo escolhido. Cada
transformação deve informar o que manteve e o que mudou. Família sonora é
outra escolha, travável pelo músico; o contraste da próxima seed só deve usar
dimensões efetivamente aplicadas. Forma longa, pesquisa geométrica e novas
famílias entram depois de uma célula curta funcionar musicalmente.

### Próxima fatia de desenvolvimento e critério de pronto

1. Selecionar **um** MIDI de bateria 4/4 já preservado no GMD, guardar caminho,
   licença/atribuição e janela de dois compassos; escolher uma versão `beat`,
   não `fill`. Chamar a experiência de *estudo de bateria 4/4* até validá-la
   musicalmente, sem prometer um gênero pelo nome da família.
2. Mostrar, para cada ataque, a cadeia nota GM → stem/função → posição/força
   → evento agendado → modelo sonoro. Confirmar por solo que V1 é bumbo, V2
   caixa e V3/V4 são chimbais; conferir o primeiro compasso com todos os stems.
3. Fazer um kit de referência com bumbo, caixa e chimbal claramente distintos,
   nível comparável e articulações mínimas. Só depois comparar duas paletas no
   **mesmo** padrão e mesmo volume. Os modelos de tons, pratos e acessórios
   exigem ensaio próprio antes de receber rótulos estilísticos.
4. Corrigir a pontuação da próxima seed, o tooltip e a documentação; oferecer
   primeiro uma variação reversível da célula de referência, com posições e
   papéis preservados. Medir o intervalo entre edição e áudio; eliminar reset
   ou silêncio inesperado antes de acrescentar operações.
5. Pedir escuta do autor com a célula original, a variação e os stems isolados.
   O marco só está pronto quando ele reconhece o padrão, a função de cada stem,
   a mudança de timbre e a ação do controle sem depender de explicação técnica.

**Ordem revista:** essa fatia musical precede a extração completa de páginas,
`PiecePlan`, novas famílias e Learn. Correções de sobreposição continuam
necessárias para a escuta e edição, mas não substituem esse marco sonoro.

### Primeira execução do marco — 08/11/2026

A investigação do GMD revelou uma falha de classificação mais fundamental que
o perfil de timbre: o corpus foi gravado no Roland TD-11, cujo [mapa oficial](https://magenta.withgoogle.com/datasets/groove)
usa 22/26 para bordas de chimbal fechado/aberto e 58 para aro de tom grave.
O importador anterior enviava 22/26 para V8 e 58 para V7 como se o corpus fosse
GM puro. O catálogo local usa agora um mapa TD-11 explícito; MIDI externo
continua GM. O tempo inicial do arquivo passa a ajustar o BPM do app quando
está na faixa do controle. Isso corrige função e andamento dos stems antes de
qualquer tentativa de
“samba” ou “rock” por paleta. A busca contrastiva deixou de pontuar família,
máscara e Chip 8-bit preservados pelo host. O tooltip da seed foi atualizado.

Uma renderização offline do estudo de dois compassos está documentada em
`midis/README.md`; mostra o trajeto nota → stem → agenda → WAV e preserva um
mesmo ganho para mix e stems isolados. **Limite:** ela comprova roteamento e
sinal, não musicalidade, correspondência de gênero ou paridade exata com todos
os parâmetros da GUI. O próximo portão continua sendo escuta no aplicativo.

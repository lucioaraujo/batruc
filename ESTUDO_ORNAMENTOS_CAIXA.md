# Batruc — estudo dos ornamentos da caixa e de notação para execução

Estado: dossiê de pesquisa, primeira versão (13/11/2026). Ainda não
implementado; serve de base para a tabela de rudimentos e para a notação.

O autor: "os ornamentos precisam ser investigados em estudos de ritmos,
percussão, baterias, caixa"; "como o batruc pode incorporar esses recursos
(algo como sistemas de notação) para facilitar a execução". Aprovou ("sim") a
sequência: dossiê → tabela de rudimentos (ornamento como propriedade da
nota) → faixa de notação com manulação → exportação.

## 1. Por que o estudo

O lote de 12/11 (`SnareOrnaments`) escreveu sete ornamentos como toques
soltos na célula: notas fantasmas, flam, drag, rufo, ricochete, rulo e aro.
Funciona no som, mas tem dois problemas:

- **No gráfico:** cada toque vira um ponto. O autor: "ficou muitos toques no
  gráfico".
- **No vocabulário:** é pequeno e inventado no código. A tradição da caixa
  tem um vocabulário organizado, com nomes, manulações e uma escrita
  própria. Este dossiê reúne esse vocabulário, para o Batruc partir dele.

## 2. Os 40 rudimentos internacionais (PAS, 1984)

**Fonte:** folha *Percussive Arts Society International Drum Rudiments*,
lida na versão oficial em PDF (pas.org).

- A PAS revisou os 26 rudimentos americanos codificados em 1933 pela
  National Association of Rudimental Drummers e acrescentou 14. O resultado
  são 40, organizados por um comitê presidido por Jay Wanamaker.
- A folha indica o estudo "aberto (lento) → fechado (rápido) → aberto
  (lento)", ou num andamento moderado de marcha.
- **Direitos:** "Copyright © 1984 by the Percussive Arts Society. All Rights
  Reserved". A folha não é reproduzida aqui. Usamos os nomes e os conceitos,
  que são vocabulário comum do ofício. As figuras abaixo são descrições
  próprias e devem ser conferidas com a folha antes de virarem tabela no
  código. As marcadas **a conferir** são de memória e precisam de checagem.

Convenções desta tabela:
- manulação em D/E (direita/esquerda; na folha, R/L);
- `e` minúsculo é nota de enfeite (grace note);
- `>` é acento;
- `*` indica que o rudimento está entre os 26 originais, como na folha.

### I. Rulos (15)

**A. Rulos de toques simples**

| # | nome (PAS) | figura (descrição própria) |
|---|---|---|
| 1 | Single Stroke Roll * | toques alternados D E D E … |
| 2 | Single Stroke Four | quatro toques alternados, os três primeiros levando ao quarto |
| 3 | Single Stroke Seven | sete toques alternados levando ao último |

**B. Rulos de rebotes múltiplos**

| # | nome (PAS) | figura (descrição própria) |
|---|---|---|
| 4 | Multiple Bounce Roll | cada toque solta vários rebotes; é o rulo "fechado" (*buzz*) |
| 5 | Triple Stroke Roll | D D D E E E |

**C. Rulos de toques duplos (abertos)**

| # | nome (PAS) | figura (descrição própria) |
|---|---|---|
| 6 | Double Stroke Open Roll * | D D E E … |
| 7 | Five Stroke Roll * | 2 pares + 1 toque acentuado (DD EE D>) |
| 8 | Six Stroke Roll | acento, 2 pares, acento (D> EE DD E>) |
| 9 | Seven Stroke Roll * | 3 pares + 1 acentuado |
| 10 | Nine Stroke Roll * | 4 pares + 1 acentuado |
| 11 | Ten Stroke Roll * | 4 pares + 2 acentuados |
| 12 | Eleven Stroke Roll * | 5 pares + 1 acentuado |
| 13 | Thirteen Stroke Roll * | 6 pares + 1 acentuado |
| 14 | Fifteen Stroke Roll * | 7 pares + 1 acentuado |
| 15 | Seventeen Stroke Roll | 8 pares + 1 acentuado |

### II. Diddles (4)

| # | nome (PAS) | figura (descrição própria) |
|---|---|---|
| 16 | Single Paradiddle * | D E D D · E D E E |
| 17 | Double Paradiddle * | D E D E D D · E D E D E E |
| 18 | Triple Paradiddle | D E D E D E D D · … |
| 19 | Single Paradiddle-diddle | D E D D E E |

### III. Flams (11)

| # | nome (PAS) | figura (descrição própria) |
|---|---|---|
| 20 | Flam * | enfeite + principal, mãos opostas (eD, dE) |
| 21 | Flam Accent * | flam + 2 toques simples, em tercina |
| 22 | Flam Tap * | flam + toque da mesma mão (eD D · dE E) |
| 23 | Flamacue * | flam, acento no 2º toque, termina em flam |
| 24 | Flam Paradiddle * | paradiddle com flam no 1º toque |
| 25 | Single Flammed Mill | "moinho" = paradiddle invertido (D D E D · E E D E) com flam no 1º toque de cada grupo — conferido¹ |
| 26 | Flam Paradiddle-diddle * | paradiddle-diddle com flam no 1º toque |
| 27 | Pataflafla | grupo de 4 com flams no 1º e no 4º toque; flams seguidos com a mesma mão; as versões americana e francesa diferem — conferido² |
| 28 | Swiss Army Triplet | tercina D D E (ou E E D) com flam no 1º; tudo em rebote, por isso rápido — conferido³ |
| 29 | Inverted Flam Tap | toques duplos com flams entre eles, começando no meio do duplo: o toque vem antes do acento com a mesma mão (o inverso do flam tap) — conferido⁴ |
| 30 | Flam Drag | flam seguido de drag — **a conferir** (a busca de 13/11 não achou a manulação em texto) |

### IV. Drags (10)

| # | nome (PAS) | figura (descrição própria) |
|---|---|---|
| 31 | Drag * | dois enfeites (um duplo) + principal (ee D); em português, "rufo" |
| 32 | Single Drag Tap * | drag + toque (eeD E) |
| 33 | Double Drag Tap * | dois drags + toque |
| 34 | Lesson 25 * | drag + toque + toque acentuado |
| 35 | Single Dragadiddle | paradiddle com o 1º toque de cada grupo trocado por drag; uma fonte o descreve como duplo rebatido em fusas — fontes divergem⁵ |
| 36 | Drag Paradiddle #1 * | acento, drag, paradiddle |
| 37 | Drag Paradiddle #2 * | acento, dois drags, paradiddle |
| 38 | Single Ratamacue * | drag + 3 toques, acento no último |
| 39 | Double Ratamacue * | dois drags + 3 toques |
| 40 | Triple Ratamacue * | três drags + 3 toques |

Conferência de 13/11/2026 (fontes de ensino; a folha PAS segue como
referência final):
¹ Drumeo, "Single Flammed Mill" — https://www.drumeo.com/beat/single-flammed-mill/ ·
² freedrumlessons, "Pataflafla" — https://freedrumlessons.com/drum-lessons/pataflafla.php ·
³ Halftime Magazine, "Swiss Army Triplets" — https://halftimemag.com/sectionals/swiss-army-triplets.html ·
⁴ Drumeo, "Inverted Flam Tap" — https://www.drumeo.com/beat/inverted-flam-tap/ ·
⁵ drumlessons.com, "Single Dragadiddle" — https://www.drumlessons.com/drum-lessons/drum-set-rudiments/single-dragadiddle/

### O que isso dá ao Batruc

- **Quatro famílias claras.** Rulo e rebote (sustentação), diddle (motor de
  manulação), flam (espessura do ataque) e drag (impulso antes do ataque).
- **Os ornamentos de 12/11 se encaixam nelas:**
  - flam = #20;
  - drag = #31;
  - o nosso "rufo" de três enfeites é um *ruff*, um drag ampliado (não
    está na lista; é variante europeia);
  - o "rulo" é #4 (fechado) ou #6 (aberto);
  - o "ricochete" não é rudimento PAS: é técnica de rebote livre;
  - "aro" é articulação, não rudimento (§4).
- **Rudimento não é só ornamento.** Muitos são **figuras completas de
  manulação** (paradiddle, ratamacue), e cabem também na geração de ritmo,
  não só no enfeite de uma nota.

## 3. Métodos de estudo de caixa (referências, não copiadas)

- **George Lawrence Stone, *Stick Control for the Snare Drummer* (1935):**
  exercícios de manulação sobre figuras simples; a ideia central é a
  combinatória de D/E. Ver também, do mesmo autor, *Accents and Rebounds*
  (1961).
- **Charley Wilcoxon, *The All-American Drummer* — 150 Rudimental Solos:**
  solos rudimentares; o ano não foi confirmado na busca. Do mesmo autor,
  *Modern Rudimental Swing Solos* (1946, segundo uma fonte).
- **José Eduardo Gramani:** já está na biblioteca do Batruc
  (`RITMOS_E_CONTAGENS.md`, `BIBLIOGRAFIA_DE_ESTUDO.md`); dissociação e
  ciclos sobrepostos.

Para o Batruc o que importa é o **método**, não o conteúdo protegido:
- Stone mostra que a manulação é um parâmetro composicional independente
  das alturas e dos acentos;
- Wilcoxon mostra o rudimento usado como frase, não como exercício.

## 4. A caixa no samba: articulações e função

**Fonte principal lida:** Arthur Teles Leppaus, "A caixa surda:
possibilidades de adaptação dos instrumentos típicos da percussão no samba
para bateria", XXIX Congresso da ANPPOM, Pelotas, 2019. Baseado na
dissertação do autor (LEPPAUS, 2018a); cita Pellon (2003), Barsalini (2009,
2014), Cunha (2015), Aquino (2014) e Borgovoni (2014).

**O que interessa ao Batruc:**
- **Caixa surda, 1929–1950:** caixa sem esteira, imitando tamborim,
  repinique e outros instrumentos de percussão. Depois dos anos 1950 vem o
  "samba de prato". O timbre da caixa era o centro da levada.
- **Articulações com baqueta (cinco):**
  1. toque no centro da pele;
  2. *rim shot* (aro e pele juntos, com o corpo da baqueta);
  3. *rim shot* com a ponta da baqueta;
  4. *stick shot* fechado (uma baqueta sobre a outra, a de baixo pressionando
     a pele; um só ataque);
  5. *stick shot* aberto (a de baixo solta; vários ataques por rebote).
- **Articulações com a mão:** *slap* na borda; toque abafado no centro com
  as pontas dos dedos; toque aberto na borda; polegar; bloco de dedos aberto
  ou fechado; pressão do indicador; **rulo de dedos**.
- **Sinais usados no artigo** (tabela da figura 6): `+` para toque fechado
  ou *stick shot* fechado, `o` para toque aberto no centro. Os demais sinais
  estão em figuras-imagem e não foram transcritos.
- **"Condução implícita":** uma mão faz a levada acentuada e a outra
  completa toda a grade de semicolcheias. É a figura central do samba na
  caixa, e o Batruc pode gerá-la (uma voz dá o desenho e a outra preenche).
- **Forma da execução:** "levada + variação + levada + variação", ciclo
  alternado comum na percussão do samba. Conversa direto com os 7 ciclos
  formais do Batruc.

**Ainda não pesquisado** (próximas leituras; frevo, maracatu e baião
avançaram em §4b):
- a caixa de escola de samba (caixa em cima, "tarol");
- maracatu e frevo;
- a definição de "rufo", "rulo", "repique", "chamada" e "virada" em fontes
  brasileiras;
- a dissertação Leppaus (2018a) e o trabalho de Barsalini.

A busca achou trabalhos da UNIRIO sobre notação de percussão de altura
indefinida na roda e na bateria da escola de samba, ainda não lidos.

## 4b. A caixa por gênero brasileiro: frevo, maracatu, baião (13/11/2026)

**Fonte principal lida:** Christiano Lima Galvão, "Panorama dos modos de
execução do baião, do maracatu e do frevo na bateria em gravações de Hermeto
Pascoal e Egberto Gismonti entre 1965 e 2003", *Per Musi* (UFMG), 2023, DOI
10.35699/2317-6377.2023.45902, licença CC BY 4.0. O artigo transcreve Airto
Moreira, João Palma, Robertinho Silva, Zé Eduardo Nazário, Nenê, Alfredo
Dias Gomes e Márcio Bahia.

Idiomas da caixa citados no artigo (resumo próprio; as figuras do artigo não
são reproduzidas):

- **Frevo:**
  - "caixa com a célula rítmica característica, acentos e rulo de 1 tempo
    de duração no final da frase de dois compassos" (citando Saldanha 2008,
    p. 184);
  - numa variação, o chimbal com o pé abre junto com o rulo do último tempo;
  - noutra, a condução fica dividida: mão direita no chimbal, esquerda na
    caixa.
- **Maracatu de baque virado:**
  - a caixa reproduz o padrão da caixa de guerra (tarol), com **toque
    múltiplo na segunda nota** e **acento na segunda nota**, em mãos
    alternadas;
  - o bumbo faz a célula da alfaia.
- **Baião:**
  - a caixa "preenche os espaços acentuando a segunda colcheia do 2º
    tempo", e esse preenchimento "dá um balanço todo especial";
  - aro de caixa com **dois timbres** (agudo com a ponta da baqueta, grave
    com o meio do corpo, numa hipótese do autor);
  - **notas fantasmas** pontuando síncopes que lembram o bacalhau do zabumba.
- **Recursos recorrentes** (conclusão do artigo): notas fantasmas na caixa,
  toque múltiplo, subdivisões do xaxado no chimbal com o pé, uníssonos com
  os pés e os gêneros também em 3/4, 5/8 e 7/8.

**O que isso dá ao Batruc** (proposta, não implementada):
- Cada família de estilo pode ter um **vocabulário de caixa** próprio, em
  vez do sorteio genérico de hoje (gene "SNR1"). Exemplos:
  - frevo: rulo de 1 tempo no fim da frase de 2 compassos;
  - maracatu: toque múltiplo com acento na 2ª nota;
  - baião: fantasmas e aro em dois timbres.

  É a parte "caixa" da **ginga por família** pedida pelo autor (ver
  TAREFAS); o microtempo e os acentos são a outra parte.
- O "toque múltiplo" é o rulo fechado (rudimento PAS #4) aplicado a uma
  nota só, e já cabe no modelo de ornamento por nota.

**Leituras seguintes:**
- Saldanha, L. V., *Frevendo no Recife* (tese, Unicamp, 2008), p. 184: a
  célula do frevo;
- Santos, C. de O.; Resende, T., *Batuque book maracatu: baque solto e
  baque virado* (2005), pp. 39–87: os baques;
- Braga, T., *A caixa clara na bateria* (2011);
- Thalwitzer, T., *Microrhythmic Analyses of Caixa Recordings from
  Maracatu-Nação* (Universidade de Viena): o servidor devolveu um arquivo
  vazio em 13/11; tentar de novo;
- Oliveira (2024), transcrição da bateria da Mangueira (revista UBM);
- Carvalho, S. S. (UFPB, 2021), técnica de caixa na música popular
  brasileira.

## 4c. Microtempo da caixa (tarol): a outra metade da ginga (13/11/2026)

**Fonte lida:** M. Davies, M. Fuentes, J. Fonseca, L. Aly, M. Jerónimo e
F. Bonini Baraldi, "Moving in time: computational analysis of microtiming
in Maracatu de baque solto", *ISMIR 2020* (Montreal). Zenodo 4245554,
licença CC BY 4.0.

**Material:** gravações com microfones de contato, uma faixa por músico, de
um terno de maracatu de baque solto (Zona da Mata Norte, PE). Os
pesquisadores relatam cerca de 45 000 ataques anotados, com a Porca e o
Gonguê grave como referência de tempo.

**Resultado:**
- Perfil mediano do **tarol** nas semicolcheias do tempo:
  **[0,25 · 0,47 · 0,715]**, contra [0,25 · 0,50 · 0,75] da grade reta. A
  3ª semicolcheia chega cerca de 3 % do tempo **adiantada**, e a 4ª cerca
  de 3,5 %. A 120 BPM, isso dá uns 15 a 17 ms.
- Bombo agudo: [0,46 · 0,69] (sem a 2ª semicolcheia).
- O microtempo do tarol é **dinâmico**: oscila acima e abaixo da grade ao
  longo da peça, mais que o do bombo.
- O resultado depende de qual instrumento serve de referência de tempo. A
  Porca chega a tocar até 20 ms adiantada e vai se alinhando aos poucos.
- Os perfis diferem bastante dos da música sul-americana mais estudada
  (samba, candombe). O artigo compara com a caixa do samba em gráfico, sem
  números no texto.

**Cuidados:** é o baque **solto** (maracatu rural), não o baque virado do
§4b. São medianas de um grupo, um retrato e não uma regra do gênero.

**O que isso dá ao Batruc** (proposta, não implementada): a ginga como
**desvio visível** (camada 2 da reordenação) pode usar perfis medidos como
este, em vez de um "swing" genérico. Por exemplo, um perfil "tarol de baque
solto" desloca a 3ª e a 4ª semicolcheia de cada tempo, com uma oscilação
lenta em torno da mediana. Seguindo a preferência do autor por restrições
leves, seria um único perfil por idioma, com a profundidade ajustável.

A tese de Thalwitzer (Viena, 2013, sobre caixas do baque virado; registro
em utheses.univie.ac.at/detail/25147) seguiu sem download: o servidor
devolveu um arquivo vazio em duas tentativas.

## 5. Sistemas de notação que servem à execução

1. **Pauta de percussão (padronização PAS):** Norman Weinberg, *Guide to
   Standardized Drumset Notation* (Percussive Arts Society, 1998; 56 p.).
   Trata de cabeças de nota, posição na pauta, pratos e notação de
   improviso; o conteúdo detalhado não foi consultado. Convenções de uso
   geral na escrita de caixa:
   - nota pequena cortada antes da principal = flam; duas notas pequenas =
     drag;
   - barras cortando a haste = rulo medido (1 barra = colcheias, 2 =
     semicolcheias); `z` ou 3 barras = rulo fechado (*buzz*);
   - `>` = acento; nota entre parênteses = nota fantasma;
   - cabeça `×` ou com círculo = aro / *cross-stick*;
   - letras D/E (R/L) acima ou abaixo = manulação.
2. **TUBS — Time Unit Box System:** grade de caixinhas, uma por unidade de
   tempo; a marca na caixa diz **como** tocar (aberto, abafado, aro…).
   Criado por Philip Harland e James Koetting (UCLA, início dos anos 1960)
   para polirritmias africanas. Fontes registram que a pauta é preferida no
   ensino e o TUBS na análise. **O Study e o piano roll do Batruc já são
   TUBS.**
3. **Sinais de articulação da percussão brasileira:** `+` e `o` (Leppaus);
   a tabela completa é a próxima leitura.
4. **Contagem falada:** "1 e & a"; as sílabas e a contagem do Gramani. As
   sílabas do konnakol (solkattu, Índia do Sul) são uma possibilidade ainda
   não pesquisada.

## 6. Proposta para o Batruc (a confirmar com o autor por etapa)

1. **Tabela de rudimentos no núcleo**, derivada da §2 depois de conferida.
   Cada rudimento guarda:
   - família;
   - lista de toques, cada um com deslocamento, força relativa, mão
     (D/E) e articulação (centro, aro, *rim shot*, *stick shot*, fantasma);
   - se é **enfeite** (antes da nota, em ms: flam ~20–40 ms) ou **figura
     medida** (na grade: paradiddle, rulos medidos).
2. **O ornamento vira propriedade da nota** (a opção 2 combinada; feita em
   13/11/2026 com os 5 ornamentos de 12/11; ver TAREFAS):
   - a nota guarda o rudimento e a mão inicial;
   - os toques nascem na hora de tocar;
   - a célula não lota, editar a nota leva o ornamento junto, e o gráfico
     mostra um ponto com sinal.
3. **Faixa "PARTITURA DA CAIXA · 16 TEMPOS"**, como a do baixo, em TUBS:
   - uma caixinha por passo;
   - sinal do ornamento sobre a nota (flam, drag, barras do rulo, `>`,
     `( )`, `×`);
   - linha de **manulação D/E** embaixo, com contagem opcional.
4. **Rudimento também como gerador:** a seed pode escolher figuras de
   manulação (paradiddle, ratamacue) e a "condução implícita" do samba para
   escrever a caixa, além de enfeitar.
5. **Articulações como timbre:** o corpo sonoro da caixa ganha articulações
   distintas (centro, borda, *rim shot*, *stick shot*, abafado), ligadas às
   notas GM já usadas (37, 38, 40…).
6. **Exportação MusicXML de percussão,** para MuseScore e impressão.

## Fontes consultadas

- Percussive Arts Society, International Drum Rudiments (página e PDF oficial):
  https://pas.org/resources/rudiments ·
  https://pas.org/wp-content/uploads/2024/04/pas-rudiments.pdf
- PAS, "The history of the 40 International Drum Rudiments — 20 years later":
  https://pas.org/publication-articles/the-history-of-the-40-international-drum-rudiments-20-years-later-the-adoption-debate-continues/
- Leppaus, A. T. "A caixa surda…", ANPPOM 2019:
  https://anppom.org.br/anais/anaiscongresso_anppom_2019/5629/public/5629-20667-1-PB.pdf
- Weinberg, N. *Guide to Standardized Drumset Notation* (catálogo):
  https://www.halleonard.com/product/6620063/guide-to-standardized-drumset-notation
- Time unit box system: https://en.wikipedia.org/wiki/Time_unit_box_system
- George Lawrence Stone: https://en.wikipedia.org/wiki/George_Lawrence_Stone
- Charles Wilcoxon: https://en.wikipedia.org/wiki/Charles_Wilcoxon
- Glossário de samba (caixa): https://viradadrums.com/glossary-of-samba-terminology/
- Davies et al., "Moving in time…", ISMIR 2020 (CC BY 4.0):
  https://zenodo.org/records/4245554
- Galvão, C. L. *Per Musi*, 2023 (CC BY 4.0):
  https://periodicos.ufmg.br/index.php/permusi/article/download/45902/39542/177703

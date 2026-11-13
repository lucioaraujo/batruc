# Batruc — ritmos, contagens e geradores de eventos

Estado: especificação de pesquisa para a próxima camada composicional; ainda
não implementada no app JUCE.

## Princípio

O número selecionado no controle atual é a quantidade de voltas de uma voz
contínua ao longo da seção. Ele determina a trajetória e seus cruzamentos; não
é o número de pulsos de um compasso nem a quantidade de ataques de um padrão.
O gerador rítmico deve produzir uma lista de posições de ataque separada dessa
trajetória. Depois, uma regra explícita pode usar a posição da curva para
modular intensidade, altura ou timbre. Isso permite ouvir a mesma curva com
ritmos diferentes e o mesmo ritmo com órbitas diferentes.

Cada gerador declara sua unidade, domínio, regra de transformação, rotação,
acentuação e pausas. O resultado preparado deve ser finito, ordenado, limitado
e reprodutível. Desenho e áudio devem ler a mesma agenda; a animação não deve
calcular ou disparar eventos por conta própria.

## Estrutura e interpretação

A agenda de eventos descreve a estrutura calculada; não determina sozinha como
ela deve soar. Sobre essa estrutura, a realização pode articular groove, swing,
intenção, acentos, dinâmica, articulação, fraseado, microtemporização e balanço
entre vozes. O intérprete deve poder moldar essas dimensões sem perder de vista
as posições, agrupamentos, rotações e pausas que o algoritmo produziu.

A referência do autor à interpretação barroca serve como analogia de projeto:
uma notação pode registrar relações e ainda deixar espaço para decisões de
realização. Não implica equivalência histórica entre o Barroco, a prática de
Gramani e tradições rítmicas culturalmente situadas. No instrumento, presets e
controles devem declarar o que alteram (por exemplo, intensidade, deslocamento
temporal ou articulação), manter a estrutura original consultável e permitir
comparação e recuperação de versões. Expressividade não deve ser reduzida a
sorteios independentes nem a um único controle genérico chamado “humanize”.

Na geração, função e intenção também podem atuar como restrições: um baixo de
resposta pode ocupar lacunas deixadas por outra voz; um baixo de apoio pode
reter pontos de referência; uma voz em primeiro plano pode construir contorno
e deixar repousos. O algoritmo registra quais candidatos criou e por que
escolheu um deles. Depois, swing, acentuação e articulação realizam a frase.
Esse percurso conserva três representações consultáveis: material calculado,
decisão composicional e eventos efetivamente tocados. Escolher uma função é
decisão do músico, não inferência automática do significado de uma fórmula.

### Inverter vozes como transformação composicional

Primeiro experimento: voz A toca um ostinato e voz B desenvolve uma série;
num limite de compasso escolhido, A passa a executar a série e B assume o
ostinato. Trocam-se agendas de ataques e pausas, conservando a identidade
sonora e o registro de cada canal. Acentos e articulação são opções explícitas
da troca. Guardar origem e destino de cada material, seed, seção, período e
momento de aplicação; oferecer retorno A/B. A operação não é inversão de
altura, sinal matemático ou simples permuta de timbres.

O primeiro cálculo precisa verificar que ambos os materiais cabem na duração
comum, que o teto de eventos por voz é respeitado e que pausas declaradas
continuam pausas. Para o baixo, a altura escrita continua ligada à sua célula;
se a série rítmica recebida exige mais ataques que as alturas disponíveis, a
regra de repetição ou a revisão humana deve ser definida antes de tocar. Não
preencher lacunas silenciosamente. O valor musical dessa troca será avaliado
pela escuta da relação entre as vozes, não pela simples mudança de contagem.

O core já oferece a primeira operação pura para agendas de vozes existentes,
`invertEventPlanVoices`. Ela troca o canal de destino de cada evento das duas
vozes e preserva o plano original. Sua aceitação pelo renderer e sua
reversibilidade foram verificadas localmente; controles JUCE e a integração
com a partitura de baixo continuam pendentes.

## Primeira implementação recomendada: padrão euclidiano

Cabe no transporte atual de duração `T` e não exige mudar a fórmula de compasso.
O padrão `E(k,n,r)` distribui `k` ataques tão uniformemente quanto possível
em `n` posições iguais, depois gira o padrão por `r` posições. A referência
matemática descreve justamente essa distribuição de onsets.

| Parâmetro | Contrato inicial |
| --- | --- |
| `n` — posições | inteiro de 1 a 64 por seção |
| `k` — ataques | inteiro de 0 a `n` |
| `r` — rotação | inteiro de 0 a `n−1` |
| tempo de cada posição | `T/n` segundos |
| posição `j` | início da posição: `tⱼ = jT/n`, `j ∈ [0,n)` |
| limite | no máximo 64 ataques por voz por seção neste modo |

As posições são relativas à seção, não a semínimas. Em 4/4 e quatro
compassos, `n=16` alinha as posições à pulsação; `n=32` dá duas posições por
tempo; `n=64`, quatro. Outros `n` produzem ciclos regulares atravessando a
seção e devem ser identificados como subdivisões/ciclos, não como compassos
tradicionais. Para reduzir densidade, `k=0` é silêncio e `k=n` é ataque em
todas as posições. A rotação muda o ponto de partida sem mudar os intervalos
cíclicos.

Na primeira entrega, o padrão será selecionável por voz, mas `k`, `n` e `r`
serão editados no painel de fórmula/ritmo, nunca inferidos do controle de
ciclos da órbita. A curva, lista de ataques e rótulo `E(k,n,r)` devem aparecer
juntos. Acentos e pausas terão controles próprios na etapa seguinte; por ora,
as posições sem ataque são pausas estruturais e os ataques têm intensidade
uniforme antes das leis de dinâmica já disponíveis.

### Contrato de cálculo

Antes da rotação, definir o bit da posição `j` por
`bⱼ = floor((j+1)k/n) − floor(jk/n)`, para `j = 0…n−1`. Com divisão inteira,
`bⱼ` só pode ser 0 ou 1, e a soma dos bits é exatamente `k`. Rotação positiva
`r` desloca os ataques `r` slots para a direita:
`b'ⱼ = b[(j−r) mod n]`. Para `k=0`, não há eventos; para `k=n`, todos os slots
têm evento. Converter cada índice atacado em `tⱼ = jT/n`, ordenar e aplicar
depois o limite global de eventos do core e os filtros geométricos já
existentes. O filtro pode remover ataques, nunca criar slots adicionais.

## Camadas seguintes — não confundir com `E(k,n)`

### Agrupamentos aditivos e métrica

Uma lista como `[2,3,2,3]` expressa grupos sucessivos e seus inícios podem
receber acentos. Para que esses grupos definam compasso, cada grupo precisa
ter uma unidade (por exemplo, colcheia) e a duração da seção deve ser a soma
dos grupos. O transporte atual fixa quatro compassos de 4/4, por isso a
implementação de compassos aditivos exige primeiro tornar compasso e duração
da seção selecionáveis. Até essa alteração, agrupamentos só podem ser tratados
como ciclos de acento dentro da seção fixa e precisam ser nomeados dessa forma.

### Sieves modulares

Um sieve escolhe posições por classes residuais e operações de conjuntos.
Representação inicial candidata: união, interseção ou complemento de condições
`j mod m ∈ R`, sobre um domínio de posições finito. Registrar a expressão
canônica, módulos, resíduos, operação e período resultante. A expressão deve
ser visível e editável; um “preset Xenakis” sem esses dados não explica o
processo. A fonte de Xenakis descreve classes residuais e operações booleanas
como materiais para construção de sieves; a referência histórica serve para
estudo, sem copiar procedimentos de uma edição específica.

### Sequências de durações

Uma lista positiva como `[1,2,3,5,8]` pode definir proporções de durações; a
soma é normalizada para a duração escolhida. Pausa deve ser um evento/tipo
explícito, não duração zero, para preservar validação e legibilidade. Esta
camada muda durações e fraseado, não apenas posições de ataques. Registrar a
sequência, regra de normalização, número de repetições e tratamento do
remanescente no final da seção.

## Ordem de implementação

1. Criar o tipo de padrão euclidiano e o cálculo puro de slots, preservando o
   gerador atual como modo `órbita/eventos geométricos`.
2. Acrescentar controles próprios de `k`, `n` e rotação, inicialmente para uma
   voz selecionada; não reaproveitar `ciclos por seção`.
3. Mostrar slots, ataques e pausas no gráfico temporal e manter a fórmula
   `E(k,n,r)` legível. A agenda mostrada deve ser exatamente a que o renderer
   toca.
4. Fixar e documentar versão de seed antes de incluir parâmetros novos no
   mapeamento determinístico. Seeds antigas continuam carregando a receita
   anterior.
5. Só depois avaliar acentos, padrões aditivos sob métrica variável, sieves e
   sequências de duração. Cada camada precisa de limite de cálculo e escuta
   comparativa própria.

## Fontes e limites da pesquisa

- José Eduardo Gramani, [*Rítmica* (1988)](https://editoraperspectiva.com.br/produtos/ritmica/).
  A editora descreve o livro como propostas para responder com sensibilidade,
  recorrendo à razão quando houver dúvida sobre a referência métrica; tratá-lo
  apenas como leitura rítmica dá aproveitamento parcial. Estudos acadêmicos
  relacionam os materiais a séries, ostinatos, contrapontos, assimetrias e
  polirritmias. Isso orienta a pesquisa, mas não autoriza copiar exercícios:
  criar relações e regras próprias e manter escuta e execução corporal no
  centro. Fontes de apoio: [Fiaminghi, OPUS (2018)](https://www.anppom.com.br/revista/index.php/opus/article/view/opus2018c2405)
  e [Ribeiro e Fiaminghi, Revista Vórtex (2019)](https://periodicos.unespar.edu.br/vortex/article/view/2880).
- José Eduardo Gramani, [*Rítmica viva: a consciência musical do ritmo*
  (1ª edição, 1996)](https://loja.editoraunicamp.com.br/DynamicItems/Catalog/9df60672-cef2-40d2-8e39-696835e957ab20pp_Ritmica_viva_W65.pdf).
  É outro livro. A ficha da edição da Editora da Unicamp de 2008 informa a
  primeira edição de 1996; a editora o apresenta como propostas abertas de
  leitura a duas ou mais vozes, em que a escuta e a realização importam além
  da contagem. Para o Batruc, investigar contrapontos entre baixo e vozes
  calculadas, deixando interpretação e gesto editáveis. Consulta de pesquisa;
  nenhum exercício, página ou conteúdo editorial foi incorporado ao código.
- Francisco Gabriel da Silva Lima e Luiz Henrique Fiaminghi,
  [“Gramani, o método ‘anti-metódico’ de rítmica: a assimetria no estudo e na
  composição rítmica musical”](https://www.udesc.br/arquivos/udesc/id_cpmenu/10570/50_15657985001175_10570.pdf),
  texto de três páginas hospedado pela UDESC. Os autores defendem que a leitura
  dos dois livros deve abrir associação, dissociação, inversão de papéis entre
  vozes, criação e improvisação, em vez de terminar na execução correta de
  exercícios. Esta é a leitura analítica do texto, não uma nova regra atribuída
  literalmente a Gramani. No Batruc, comparar o mesmo baixo calculado como
  apoio, resposta e linha em primeiro plano; a mudança de papel deve aparecer
  no arranjo e ser avaliada pela escuta. Sem reproduzir exercícios ou páginas.
- Bianca Thomaz Ribeiro e Luiz Henrique Fiaminghi, [“O ritmo além da regra: o
  conceito de time line e rítmica aditiva em Gramani”](https://editoraartemis.com.br/catalogo/post/o-ritmo-alem-da-regra-o-conceito-de-time-line-e-ritmica-aditiva-em-gramani),
  capítulo 13 de *Estudos Latino-Americanos sobre Música*, vol. I (2020), DOI
  `10.37572/EdArt_14910092013`; cópia integral consultada em
  `~/Downloads/o-ritmo-alem-da-regra-o-conceito-de-time-line-e-ritmica-aditiva-em-gramani.pdf`.
  A contribuição útil ao Batruc é pensar o ostinato como tempo moldado e
  referência cíclica, em diálogo com outras linhas, em vez de tratá-lo como
  metrônomo neutro. O capítulo demonstra agrupamentos e rotações como
  `3+3+2`, padrões de 12 pulsos e séries por adição gradual de valores longos
  e curtos, sobrepostos a ostinatos em contraponto. **Limite de atribuição:**
  os autores apresentam a relação entre Gramani, *time lines* africanas e
  rítmica aditiva como perspectiva analítica deles; afirmam que Gramani não
  declarou essas referências nem escreveu usando esses conceitos. Não
  transformar padrões culturalmente situados em presets universais sem
  contexto, fonte e escuta. O artigo também insiste na dimensão performativa,
  gestual e sensível, além da notação e da contagem.

- Godfried Toussaint, [“The Euclidean Algorithm Generates Traditional Musical
  Rhythms”](https://archive.bridgesmathart.org/2005/bridges2005-47.html):
  distribuição dos ataques tão uniforme quanto possível; a terminologia e os
  exemplos devem ser tratados com contexto cultural, sem afirmar que o
  algoritmo substitui as tradições musicais citadas.
- [Iannis Xenakis — Sieve Theory](https://www.iannis-xenakis.org/en/sieve-theory/):
  contextualiza classes residuais, operações e transformações. O instrumento
  deverá explicar sua própria expressão computável e evitar tratar qualquer
  filtro modular como reconstrução da teoria completa.
- José L. Besada, [“Xenakis’ Sieve Theory: A Remnant of Serial
  Music?”](https://mtosmt.org/issues/mto.22.28.2/mto.22.28.2.besada.php):
  análise histórica e conceitual que alerta para não reduzir a teoria a um
  gerador genérico sem declarar a abordagem composicional.

### Sequência de escuta contextual no Learn — protótipo de conteúdo

A primeira sequência aplicada ao corpus local começa por uma fonte MIDI
identificada, não por um preset que prometa representar um gênero:

1. Conferir rótulo de origem, tipo `beat/fill`, compasso e BPM; manter o MIDI
   original distinguível das células importadas e das alterações feitas no
   Batruc.
2. Ouvir o trecho por stem e localizar, pela própria escuta, marcação, guia,
   resposta, pausas e sobreposições. Os nomes V1–V8 são lanes práticas do
   instrumento; não substituem os nomes e papéis dos instrumentos da fonte.
3. Alterar uma camada ou uma regra de intensidade, comparar com a fonte e
   descrever o que mudou na função musical, não só na contagem de ataques.
4. Ao usar rótulos brasileiros do GMD, tratá-los como metadados de performances
   de bateria daquele dataset. O GMD não documenta nesses rótulos uma tradição
   completa nem autoriza generalizar os eventos como padrão universal.

Para contextualizar um estudo futuro de **Maracatu Nação**, o parecer do
[IPHAN sobre o inventário e o registro](https://bcr.iphan.gov.br/wp-content/uploads/tainacan-items/65968/66968/Maracatu-Nacao_de-Parecer-do-Conselho_.pdf)
descreve o batuque, as alfaias e os idiofones e afirma que os baques têm
peculiaridades e identidades próprias entre as nações. Portanto, a atividade
deve comparar fontes contextualizadas e grupos identificados, sem ensinar uma
célula MIDI como “o” baque. Como referência de pedagogia corporal e contextual
para Ijexá/Afoxé, a videoaula pública da
[Paraná Cultura](https://www.prcultura.pr.gov.br/Pagina/Percussao-em-Casa-Introducao-ao-Ritmo-Ijexa)
articula prática corporal, contratempo, subdivisão, instrumentos e escrita;
ela não licencia a apropriação de um toque nem substitui as comunidades que o
transmitem.

No app, esta sequência é uma orientação textual para usar o importador, Mixer,
solo/mute e comparação manual. Não há ainda catálogo de corpus navegável,
detector automático de papéis, demonstração sonora por estilo nem exercício
autoral de Maracatu ou Ijexá. Escuta, autorização e revisão por músicos com
conhecimento situado continuam necessárias antes de ampliar esses conteúdos.

Esta especificação não reivindica validade composicional ou didática antes de
implementação, visualização e escuta. O autor deve poder comparar e transformar
as receitas; o algoritmo oferece uma estrutura, não uma interpretação musical
obrigatória.

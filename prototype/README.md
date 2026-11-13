# Batruc — estudo local 01

Maquete reversível de interface e audição para os primeiros problemas de
composição do Batruc. Abrir [index.html](index.html) diretamente no navegador
e clicar em **Tocar estudo**. Não requer servidor, instalação ou rede.

Os botões **Aprender** e **Sobre** abrem o caderno local em `guide.js`, com sete
tópicos de composição e textos em PT/EN/ES/FR. O seletor global em
`interface-i18n.js` traduz rótulos e opções, acompanha o caderno e trata também
mensagens dinâmicas de arquivo, mixer, vozes, reprodução, traçado e edição do
baixo. A medição automatizada no Chrome conferiu overflow e interseção entre
controles em viewports de 390 a 1440 px; essa verificação não substitui uma
revisão visual humana e funcional em navegador. Nomes de arquivos/pastas e
detalhes técnicos de erros do navegador permanecem como fornecidos. Em telas
estreitas, os controles geométricos passam a uma coluna e as ferramentas do
caderno podem quebrar em mais de uma linha.
O tópico da Lei I define as oito funções implementadas, a mistura por λ e
separa explicitamente a analogia composicional de uma medição de energia
acústica ou de uma simulação física. O tópico geométrico descreve a soma
finita de Fourier, o controle de ordem H e a influência da fonte y nos eventos
de V1.

Este protótipo local não é ainda a edição web de experimentação destinada a
usuários. A direção agora prevê dois produtos: o instrumento completo C++/JUCE
para download multiplataforma e uma edição web mais básica. A edição web deverá
preservar receitas e relações composicionais comuns; a forma de compartilhar o
core com o aplicativo nativo ainda será investigada. Hospedagem e publicação
seguem como etapas separadas.

A tela exibe de duas a oito vozes, cada uma com sua fórmula, número de
oscilações, coeficiente de mistura e gráfico. Cada voz seguinte combina sua
função senoidal com uma parte ajustável da função anterior. Mudar o número de
ciclos ou `λ` recalcula fórmulas dependentes, gráficos e eventos.

Há também uma pista de baixo editável, com uma nota ou pausa por passo e
duração individual. O editor gráfico permite inserir notas com clique, mudar
altura arrastando verticalmente e ajustar duração pela borda direita; os
seletores abaixo oferecem edição precisa. O número de passos segue o compasso
(12 em 3/4, 16 em 4/4, 20 em 5/4 e 14 em 7/8). No modo manual, a linha se
repete por compasso; no modo seeded, o calendário de quatro compassos segue
ataques vocais e o gráfico pré-visualiza o primeiro compasso do primeiro ciclo
de mudança. Uma nota termina no próximo ataque para manter a pista monofônica. O LFO
senoidal, triangular ou quadrado é opcional e desloca as alturas escritas em
semitons. A faixa do baixo no gráfico principal mostra o resultado, e o mixer
tem canal próprio. O baixo oferece seis matérias provisórias: subgrave senoidal,
triângulo redondo, dente filtrado, quadrada filtrada, dente brilhante e pluck
de decaimento rápido. São modelos Web Audio para comparar caráter, não timbres
de qualidade final nem instrumentos acústicos simulados.

O desenho é também uma entrada composicional: o modo **Desenhar traço** aceita
um gesto contínuo no editor de baixo, interpola as alturas atravessadas e as
quantiza aos passos do compasso. O gráfico liga os pontos da frase para tornar
visível esse percurso. O modo de edição isolada continua disponível para
inserir, remover e ajustar notas.

Cada passo pode retrigar 1, 2, 4 ou 8 vezes e ser normal, ghost ou acentuado.
Além das pausas escritas no padrão, o fraseado pode inserir respirações a cada
dois compassos, uma cadência de silêncio no quarto compasso ou alternar chamada
e resposta entre compassos. São comportamentos determinísticos e visíveis na
faixa do baixo; o loop exato permanece como padrão. Esses gestos partem de
ideias do Navalha 2 — REPEAT, STUTTER ×4, BURST ×8, TRACE LOOP e GAP — e foram
traduzidos para eventos musicais; a maquete não repete fragmentos de gravação.

As durações também podem ser calculadas: modo manual, sequência de Fibonacci
limitada ao número de semicolcheias do compasso (`dₙ = min(P, F₍ₙ mod 8₎₊₁)`)
ou progressão geométrica limitada a esse compasso (`dₙ = min(P, arred(r⁽ⁿ mod 8⁾))`),
em que `P` é o número de semicolcheias do compasso. A razão `r` pode
ser 1,20, 1,50, o número áureo ou 2. A interface exibe a expressão e os valores
resultantes no compasso. O gate ainda termina no próximo ataque, para a linha
permanecer monofônica; as durações longas atravessam passos escritos como
pausa.

A mesa de canais fica logo abaixo do gráfico para aproximar a audição e a
visualização dos controles de mixagem. A interface reduz o corpo tipográfico
das equações e amplia a altura útil do gráfico.

Durante a reprodução, as curvas não aparecem inteiras de uma vez: cada faixa
revela a função calculada até o instante atual. O trecho futuro permanece em
branco; pausada, a vista mostra o ciclo completo. Cada transformação aplicada
à voz altera a forma traçada.

Há um segundo gráfico, independente do gráfico temporal das vozes. No modo de
funções, ele mostra o percurso geométrico: circunferência de compasso, triângulo ou
quadrado traçado por esquadros, espiral, curva de Lissajous, nefróide
(epicicloide, `R=2r`), deltóide (hipocicloide, `R=3r`), arco cicloidal com
retorno horizontal contínuo ou catenária normalizada. Para epicicloide e hipocicloide, mostra a
circunferência fixa, a roda rolante, o raio até o ponto traçador e o percurso;
as razões entre raios estão fixas. As duas curvas novas usam parametrizações
contínuas. Com a fonte geométrica ligada, `y(t)` de todas essas formas alimenta
eventos/intensidade da V1.
No triângulo e no quadrado, o painel também calcula área normalizada pelo
método do cadarço e perímetro pela soma dos lados; esses números são
informativos e ainda não modulam o som.
Durante a
execução, o percurso se revela no mesmo ciclo da música; o cursor indica a
ponta do instrumento de desenho e, no compasso/espiral, o braço parte do
centro. O caminho completo aparece com o transporte parado. O painel mostra a
equação ou regra geométrica ativa. A opção “Usar y do desenho como função da
voz 1” conecta o eixo vertical ao gerador; sem ela, o desenho é apenas visual.
As opções Fourier reconstroem os percursos fechados de triângulo ou quadrado
por coeficientes complexos `cₙ`, de `n=−H` a `+H`. O controle `H` vai de 1 a
32 (3–65 componentes); o canvas sobrepõe a referência pontilhada, a trajetória
aproximada e os círculos/vetores em rotação. O eixo `y` aproximado pode alimentar
a voz 1. A leitura de erro é RMS relativo bidimensional, calculada localmente
em 512 amostras contra a curva geométrica de referência; não corresponde às
métricas nem aos valores publicados pelo Matharium. A aproximação, os vetores,
as curvas rolantes e o áudio continuam em avaliação musical.
Os rótulos Math Art, Sound Design e Mathematical Sonification situam as três
frentes de pesquisa, não uma alegação de validação ou acabamento sonoro.

Cada voz matemática escolhe sua transformação: seno, inversão, retificação,
segundo harmônico, envelope de amplitude ou quantização. A transformação entra
na função antes da mistura `λ` com a voz anterior; a expressão correspondente
fica visível no cartão.

A máquina de seeds usa oito dígitos hexadecimais e um controle de variação. Uma
combinação de seed, fórmulas manuais e nível de variação reproduz os mesmos
ciclos, transformações, fases, misturas e linha de baixo. No modo seeded, a
linha seleciona ataques vocais ao longo de quatro compassos; posições e
intensidades das vozes calculam alturas e dinâmicas sem impor escala
predefinida. O gráfico pré-visualiza o primeiro compasso do primeiro ciclo de
mudança. Editar uma célula ou desenhar no piano roll muda para o padrão manual.
Composições antigas sem o campo que identifica o modo seeded permanecem no
modo manual. O botão Nova Seed restaura a geração vocal.

**Escolher pasta** abre um diretório local indicado pelo usuário. **Guardar
cálculo** grava nele um `.batruc.txt` UTF-8 (JSON legível e versionado) com
nome, seed, fórmulas, parâmetros, padrão do baixo e estado do mixer. O seletor
lista esses cálculos para reabertura ou exclusão. A pasta ativa fica apenas na
memória da sessão; depois de recarregar a página, basta selecioná-la de novo. Não há banco de
composições no armazenamento do navegador nem envio a servidor. O acesso direto
à pasta depende de o navegador oferecer a API de arquivos; se ela faltar ou a
permissão for recusada, a interface informa que não pode usar o diretório.

**Gravar master** captura a saída estéreo processada do instrumento. Se a
reprodução estiver parada, inicia o estudo junto com a gravação; se já estiver
tocando, grava a partir do momento do clique. Ao concluir os sete ciclos, o
transporte para e a gravação é finalizada. Com uma pasta escolhida, o áudio
WebM/Opus ou Ogg/Opus e o TXT de cálculo são gravados nela. Sem acesso à pasta,
o áudio é baixado pelo navegador e o TXT fica disponível em um link.
**Finalizar e salvar** encerra apenas a gravação e mantém o transporte em
andamento; **Parar** encerra o transporte e salva o trecho gravado. A gravação
é local; a maquete não envia áudio a um servidor. Esse arquivo comprimido serve
para prévia, não como master PCM sem perdas para entrega final. O TXT UTF-8 usa
JSON versionado e nome correspondente ao áudio; identifica o arquivo e seu
MIME type, e reúne seed, fórmulas e parâmetros, agenda inicial de eventos e
revisões de configuração feitas durante a captura.
O plano inicial descreve a agenda completa calculada no começo; cada revisão
indica o tempo aproximado a partir do qual a nova configuração afeta os eventos.
Por limite de tamanho, o histórico guarda até 512 revisões. O TXT é um registro
computacional da composição, não uma partitura convencional nem um substituto
para áudio sem perdas.

A receita **A** dispara nas passagens pelo centro; a **B**, nos extremos do
movimento combinado. A construção apresenta V1 no ciclo 1, metade das vozes
selecionadas no ciclo 2 e o conjunto completo no ciclo 3. O baixo participa de
todos os ciclos. No ciclo 5, todas as vozes permanecem disponíveis e a fase é
sustentada, sem silêncio formal oculto. No retorno, metade do conjunto atua no
ciclo 6 e todas as vozes no ciclo 7. A quantidade intermediária é
`ceil((N+1)/2)`, em que `N` é a contagem selecionada. O desvio de fase das vozes
não físicas avança nos ciclos 3–4 e retorna gradualmente ao valor inicial nos
ciclos 6–7. Cada voz
tem volume, panorama, mute e solo no mixer; solos múltiplos são permitidos. Há
mute master e volume de saída com contenção suave de picos. A duração total
varia de 28 a 84 s, em sete ciclos.

Durante a reprodução, medidores leem RMS do barramento das fórmulas e do baixo
em separado. O botão `AJUSTAR VOZES` propõe uma compensação pelo desnível atual
e a aplica ao trim de −12 a +24 dB, que fica salvo com a composição. É um ajuste
estático explícito; a leitura não é LUFS e não automatiza o ganho ao longo da
música.

A faixa formal numera cada ciclo; o rótulo acessível informa seção, quantidade
de vozes ativas e presença do baixo. O texto sob a faixa resume o mapa de
entradas para a contagem de vozes selecionada. Cada cartão de fórmula indica
se a voz está ativa, em pausa ou em qual ciclo ela entra ou retorna.

O painel acrescenta intenção (encontros, construção, rarefação ou contraste),
curva de dinâmica, swing de reto a ternário, articulação curta/equilibrada/
sustentada, quatro formas de onda básicas e quatro mapas de acento de groove.
Andamento ajustável (40–240 BPM) e compassos 3/4, 4/4, 5/4 e 7/8 organizam a
grade de swing e acentos; a duração global permanece independente e a grade
visual mostra pulsos e barras.
Essas escolhas são modelos audíveis provisórios, não estilos de execução
autênticos nem simulações de instrumentos acústicos. As famílias já têm
precedentes internos no RASGO para acentos, articulação, groove e duração; o
mapa de fontes está no [README do projeto](../README.md#repertório-já-existente-na-família-rasgo).

### Forma de sete ciclos e movimento pendular

A forma tem sete ciclos: 1–2 constroem, 3–4 mudam, 5 é uma pausa e 6–7
retornam. No ciclo 5 apenas V1 gera eventos: as vozes 2–12 e o baixo são
recolhidos. Notas do baixo iniciadas no ciclo anterior têm seu gate encerrado
na fronteira da pausa. No modelo pendular, o painel mostra `θ₁ × θ₂`: as seções
são coloridas por grupo de ciclos, as fronteiras ficam marcadas e o cursor
acompanha o cálculo. O painel cresce nesse modo, sem deformar a relação entre
os eixos. Os ângulos usam uma escala comum normalizada pelo maior valor do
percurso. As três coordenadas continuam calculadas durante o quinto
ciclo. A pausa é uma decisão de orquestração, não uma interrupção da dinâmica
calculada. Ao voltar ao modo de funções, o painel recupera a geometria. Nos
cartões pendulares, cada voz mostra seu ângulo e sua velocidade angular; a
energia total aparece uma vez no painel de relações, sem ser atribuída
individualmente aos três pêndulos.

O seletor **Três pêndulos ligados por molas · aproximação linear** oferece um
modelo idealizado de três pêndulos individuais, com acoplamento efetivo entre
vizinhos, comprimento `L`, coeficiente adimensional `κ` e amortecimento `ζ`.
As equações de ângulo pequeno alimentam os traçados e os eventos das três
primeiras vozes; os controles de ciclos, mistura e transformação dessas vozes
ficam ocultos enquanto o modelo está ativo. Este modelo não reproduz o
pêndulo serial mostrado nas capturas do artigo de P. Prasad, nem valida os
resultados do artigo. As capturas fornecidas pelo autor são a proveniência da
pista; a referência bibliográfica e as equações completas permanecem por
confirmar.
O gráfico amplia horizontalmente de 1× a 8× e verticalmente a amplitude de
0,5× a 3×. O controle “Centro do trecho” permanece ativo; ao movê-lo em 1×,
o gráfico passa a 2× e enquadra o trecho escolhido. Também é possível
recentrar e percorrer com arraste ou roda do mouse. Esses controles mudam a
janela de visualização, não o tempo nem o volume do áudio.

O Caustic 8BitSynth é referência para expressão executável: fórmula inteira
dependente de `t` produz diretamente uma waveform bytebeat. A maquete ainda
não avalia fórmulas livres; suas equações são as funções senoidais especificadas
no código. Definir um avaliador próprio e limitado é próximo passo de pesquisa,
não capacidade já implementada.

`score.js` combina as funções e gera os eventos com limites finitos; `app.js`
desenha, controla o transporte e produz sons percussivos provisórios com Web
Audio. A interface HTML/Canvas serve à pesquisa de gesto e layout; sua
qualidade sonora não define a meta do instrumento RASGO, que busca áudio de
excelência, recursos aprofundados e aprendizado claro. Ela não substitui a
direção C++/JUCE. A oscilação é idealizada; não há simulação de acoplamento
físico, síntese percussiva final, registro de performance ou exportação de
partitura.

Verificação rápida da lógica, se Node.js estiver disponível:

```sh
node prototype/test-score.cjs
```

Executar o comando acima a partir de `BATRUC/`. A avaliação musical e a
escuta pelo autor ainda estão pendentes.

Arquivos e sons desta maquete são autorais do projeto. Não há biblioteca,
fonte, amostra ou mídia externa incorporada. Licença do projeto:
AGPL-3.0-or-later em [../LICENSE](../LICENSE). O diretório permanece local
e excluído do Git nesta fase. Direção artística de Lúcio de Araújo;
implementação desta maquete com assistência Codex em 2026-10-24.

O painel geométrico inclui um bilhar em quadrado com sete razões de voltas
fechadas. A seleção modifica as colisões e os eventos da V1 quando a fonte
geométrica está ativa; o gráfico marca paredes, percurso, impactos e posição
do traçador. O modelo é uma trajetória especular ideal, não uma simulação de
reverberação. A regressão `node prototype/test-score.cjs` verifica as sete
contagens, a ordem dos impactos, a reprodução determinística e os eventos
musicais resultantes.

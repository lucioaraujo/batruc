# Batruc — operações de composição generativa

Estado em 01/11/2026: contrato de pesquisa com operações puras no core C++ e
uma primeira forma longa automática no renderer JUCE; ainda não há editor
manual de arranjo.

## Unidade, escala e memória

Uma operação tem **fonte**, **tipo**, **escopo**, **parâmetros**, **seed quando
houver escolha**, **invariantes**, **resultado** e **versão anterior**. Ela pode
atuar sobre célula, compasso, frase, seção ou peça. Cada escala tem uma fronteira
temporal própria; a troca durante a reprodução só entra nessa fronteira depois
de preparar a nova agenda fora do callback. O gráfico e a partitura precisam
mostrar a relação entre fonte e resultado, inclusive pausas e material que não
mudou.

Uma célula conserva identidade suficiente para reaparecer. Uma frase combina
células, dá direção e repouso. A seção desenvolve ou suspende frases. A peça
organiza seções, retornos e contraste, podendo guardar motivos e decisões
de escuta para as próximas seções. A mesma operação muda de efeito conforme
o escopo: inverter três notas de uma célula é diferente de fazer o contorno
de uma frase inteira descer; recombinar dois compassos é diferente de cruzar
duas seções. O usuário escolhe o escopo e pode travar o que deve sobreviver.

O renderer JUCE organiza sete ciclos locais em uma forma (construção, mudança,
pausa e retorno). Na reprodução longa, cada forma de sete ciclos é agora uma
passagem de uma macroforma de oito papéis: A → B → A′ → C → B′ → pausa → C′ →
retorno A. A mesma seed fornece o motivo-base; memória determinística modula
articulação e acentuação de cada papel. A abertura A/B/A′/C é comum; a seed
seleciona uma entre quatro ordens para B′, pausa e C′, sempre terminando em
retorno A. A/B e A′/retorno compartilham o mesmo
calendário de eventos, favorecendo o reconhecimento; as outras passagens
redistribuem peso entre vozes ou rarefazem a textura. O baixo recebe os mesmos
pesos temporais das vozes. O status indica `P#/8` e o papel vigente. Cada
passagem tem os sete ciclos formais (28 compassos em 4/4); as oito passagens
ocupam 224 compassos antes de reiniciar a macroforma.

## Gesto espacial calculado para percussão

## Variação calculada por stem

O controle **VAR** do Mixer interpola entre o stem-base (`v=0`) e a
transformação calculada integral (`v=1`), de modo independente para cada voz.
Em cada evento, o motor monta
`C = clamp(0,55·y + 0,25·x + 0,20·sin(2π·r·t + φ), −1, 1)`, onde `x` e `y`
são coordenadas normalizadas do percurso, `t` é o progresso na seção e `r` e
`φ` são derivados deterministicamente da seed e do índice da voz. O peso
eventual se torna `W = (1−v) + v·Wforma·(1 + 0,20·C)`. A altura combina a
memória de passagem com `round(2·C)` e limita o deslocamento resultante a dois
semitons, interpolando a razão de frequência com `v`. A profundidade também
escala pan calculado e amortecimento percussivo. Assim, as vozes compartilham
seed, forma e percurso, mas não precisam repetir a mesma transformação: cada
canal tem fase derivada própria e profundidade independente.

O controle não cria uma fonte genérica de ruído: toda diferença é explicável
por percurso, seed e forma e pode ser reproduzida. Cenas schema 36 guardam o
valor por stem; versões anteriores usam `v=1` para preservar o comportamento
antigo. As ideias de profundidade local/cache de Asterismus e de variação
seeded por passo de Rasgo Modular foram consultadas, sem cópia de código ou
dependência. A implementação ainda requer escuta comparativa de vários stems,
seeds e profundidades.

## Campo XY móvel e Lupa de foco

As três superfícies usam um relógio contínuo `τ = número_da_seção + t`, para
que a transformação não recomece a cada ataque. A seed define `φ` e `r`, e
`θ(τ)=2πrτ+φ`. Ilhas e microlinhas recebem translação limitada, rotação e uma
variação lenta de largura/raio. O motor consulta o campo na posição e no tempo
de cada ataque; portanto, a borda em movimento libera ou suprime eventos de
verdade. O desenho usa o mesmo `patternFieldPose/Allows/Weight` do core.

No campo livre, uma Lupa virtual percorre o plano e mantém todos os eventos,
mas altera seu peso por distância: `W=0,68+0,52·exp(−d²/(2ρ²))`. É uma
acentuação espacial composta para o Batruc, não uma afirmação sobre acústica
nem uma máscara de silêncio. O conceito de foco móvel foi observado na Lupa
do Aquorbium (documentação local `../AQUORBIUM/docs/DESENVOLVIMENTO.md`, seção
“Para usar a Lupa”); sua agência ecológica e o gesto de arrastar não foram
copiados. Nenhum código Aquorbium foi transferido.

As ilhas adesivas e as microlinhas são micropatterns musicais: sua geometria
faz ataques de diferentes trajetórias entrar ou sair do campo, alterando a
densidade e as relações temporais entre stems. Em campo livre, o foco não
precisa recalcular os tempos; o calendário preparado permanece e cada ataque
recebe seu peso no instante em que toca. Para conter o custo em tempo real, o
JUCE consulta a próxima amostra de ataque dinâmico das máscaras, em vez de
percorrer todos os stems a cada amostra de áudio. A deriva lenta, entre 0,25 e
0,625 ciclos por seção formal, continua governada por seed e trajetória.

As quatro ilhas recebem deformações radiais escolhidas por seed,
independentemente para cada organismo. A biblioteca tem 28 perfis
limitados: (0) harmônicos de segunda/terceira ordem; (1) W de quatro lóbulos;
(2) domo gaussiano periodizado; (3) lóbulo racional inspirado na bruxa de
Agnesi; (4) softplus; (5) GELU; (6) seno absoluto com ondulações; (7) batimento
de harmônicos próximos; (8) tangente limitada; (9) seno com fase modulada,
chirp circular; (10) onda cúbica; (11) logaritmo natural; (12) dobra refletida;
(13) setores côncavos; (14) anamorfose angular; (15) silhueta afilada
(“foguete”); (16) interferência seno/cosseno; (17) arco seno de cosseno;
(18) arco cosseno refletido; (19) produto seno/cosseno; (20) cosseno composto;
(21) rebote amortecido; (22) parábola; (23) dobra/reflexo reforçado;
(24) curva recíproca; (25) logaritmo de base 10; (26) cosseno amortecido;
(27) oval suavemente convexo. Cada perfil
mantém a curva fechada e o raio positivo. Gauss, Agnesi, softplus, GELU e log
são adaptações periódicas/radiais, não gráficos cartesianos literais; “chirp”
é modulação de fase angular periódica, não varredura de frequência no tempo.
“Reflexo” descreve simetria geométrica, não uma reflexão óptica calculada.
“Rebote amortecido” é um perfil radial periódico com essa aparência; não
simula uma colisão com perda de energia.
O perfil harmônico anterior continua variando lentamente com seed e percurso.
Cada organismo tem um raio próprio
`Rᵢ=0,38+0,027sin(0,61θ+φ+iγ)+0,013sin(0,19θ+φ+(i+1)γ)`, com
`γ=2π/φ_golden²≈2,399963`, onde `φ_golden=(1+√5)/2`; o segundo termo transmite uma pulsação fraca entre ilhas vizinhas. Cada centro também deriva em X/Y com duas componentes determinísticas, uma própria e outra da vizinha; a amplitude máxima combinada é 0,036 unidade por eixo. Quando ilhas vizinhas se aproximam, surge um conector temporário: `b=clamp(0,018−g/2; 0; 0,045)`, com `g=d−Rᵢ(α)−Rⱼ(α+π)` e `d=|cᵢ−cⱼ|`. O corredor entre centros passa a fazer parte do campo que aceita e pesa ataques; quando as fronteiras se afastam, o conector desaparece. O mesmo segmento e raio são usados no desenho e no teste de distância. O ângulo da deformação varia suavemente por ilha.
É um acoplamento determinístico e periódico, sem memória de eventos passados:
“organismo” nomeia o agente composicional, não uma simulação biológica.
Uma janela de oito bits da seed escolhe por módulo o perfil de cada ilha. A
mesma função de contorno alimenta o desenho e a distância da máscara nos dois
motores; lóbulos, concavidades, cúspides e rebotes alteram quais ataques passam
e o peso dos ataques próximos à borda. A geometria não escolhe alturas nem cria
eventos por si só. O movimento atual tem desvios periódicos e limitados; ainda
não é um sistema caótico sensível às condições iniciais no sentido do efeito
borboleta.

### Pesquisa futura: topologia, variedades e desvios

Topologia algébrica — em particular homologia e cohomologia —, variedades,
fibrados vetoriais (feixes de fibras) e cálculo diferencial ainda não estão
implementados como operações do instrumento. São hipóteses para novos
experimentos, com possíveis
relações composicionais:

- um **homeomorfismo** é uma transformação bijetiva, contínua e com inversa
  contínua. Preserva a estrutura topológica — como conectividade, número de
  componentes e existência de buracos —, mas não preserva comprimentos,
  ângulos, curvatura, áreas ou espectro acústico. No Batruc, pode fundamentar
  a transformação de contornos ou micropatterns mantendo a identidade dos
  percursos e a conectividade enquanto a forma muda. A deformação só afeta o
  som se uma regra explícita converter posição, velocidade, curvatura ou
  distância em ataques, pausas, acentos, alturas ou timbre; preservar topologia
  não implica preservar composição ou som;
- distinguir `S²` de `S³` evita uma confusão comum: `S²` é a superfície esférica
  bidimensional usual; `B³` é a bola sólida tridimensional cujo bordo é `S²`;
  `S³` é a esfera tridimensional (a hipersuperfície de uma bola em quatro
  dimensões), não uma bola sólida comum. A conjectura de Poincaré afirma que
  toda 3-variedade fechada e simplesmente conexa é homeomorfa a `S³`. Grigori
  Perelman provou a conjectura no programa de fluxo de Ricci com cirurgia,
  apoiado no trabalho de Richard Hamilton e também resolvendo a conjectura de
  geometrização de Thurston. Isso é um teorema de classificação de espaços,
  não uma fórmula para calcular o som de uma sala;
- possível experimento Batruc, ainda não implementado: usar trajetórias numa
  superfície `S²` ou num toro para relacionar fase, fechamento e retorno; ou
  modelar raios numa sala 3D com superfícies refletoras. Esses são problemas
  composicionais/acústicos diferentes. A conjectura de Poincaré pode orientar
  perguntas sobre conectividade e laços, mas não determina as trajetórias nem
  os modos acústicos; para estes são necessários domínio, condições de
  contorno e um modelo de propagação apropriados;
- componentes conectadas podem organizar regiões ou agrupamentos de vozes;
  classes de homologia podem descrever ciclos e buracos persistentes; uma
  representação de cohomologia pode avaliar ou etiquetar esses ciclos para
  controlar motivos, pausas, separações e reencontros. Invariantes como números
  de Betti exigem uma discretização estável antes de afetar a música;
- numa variedade ou superfície curva, tangente, normal, curvatura e comprimento
  de arco podem informar articulação, acento, duração e densidade; distância
  geodésica pode relacionar eventos sem reduzir o espaço a uma grade plana;
- num feixe (fibrado) vetorial, o percurso pode ser a base e cada fibra guardar
  estados locais de voz, fase, timbre ou dinâmica. Uma conexão pode transportar
  esses estados ao longo do percurso; a holonomia ao fechar um ciclo poderia
  produzir retorno transformado. Isso exige definir a base, as fibras e a regra
  de transporte antes de mapear qualquer parâmetro musical;
- curvas implícitas `F(x,y)=0` e regiões `F(x,y)≤0` podem gerar interseções,
  cortes e máscaras. Polos, singularidades e custo de cálculo precisam de
  limites explícitos;
- uma rede de teoremas pode ligar geometria, análise, álgebra e dinâmica por
  hipóteses e invariantes compartilhados. Cada conexão precisa ser matematicamente
  válida no domínio usado; a composição pode mapear o invariante para forma,
  fase, ritmo ou transição, sem apresentar uma analogia artística como prova;
- o teorema de Stokes relaciona a integral de uma forma sobre a fronteira à
  integral de sua derivada exterior no domínio; no caso vetorial usual, liga
  circulação de contorno a rotacional sobre uma superfície orientada. Pode
  informar relações entre percurso fechado e campo, desde que orientação e
  grandeza mapeada estejam definidas;
- a recorrência de Poincaré diz, sob hipóteses de medida preservada e medida
  finita, que quase todo estado retorna arbitrariamente perto do inicial; não
  promete repetição exata ou prazo curto. Pode inspirar retornos de células,
  mas as hipóteses do sistema escolhido precisam ser verificadas;
- análise de estruturas pode tratar ilhas como nós e conectores transitórios
  como arestas: grau, componentes, ciclos e duração das ligações podem orientar
  densidade, alternância ou reencadeamento. Derivadas do contorno podem estimar
  inclinação, curvatura e deformação, desde que a amostragem não confunda
  cúspides intencionais com instabilidade numérica;
- um estudo de desvio/efeito borboleta deve comparar trajetórias com condições
  iniciais próximas, limitar divergência e definir o resultado musical. A
  deriva atual é periódica e reproduzível, não é caos comprovado;
- reflexão geométrica, reflexão óptica e projeção anamórfica são fenômenos
  distintos. Animação ou truque de cinema só terá papel composicional quando
  alterar uma variável sonora por regra explícita.

### Classificar o espaço antes de compor com ele

“Espaço” não designa uma única estrutura. Um mesmo domínio pode receber várias
estruturas, e elas respondem a perguntas diferentes:

| Estrutura | O que distingue | Possível operação no Batruc |
| --- | --- | --- |
| Topológica | conectividade, ciclos e buracos; preservada por deformações contínuas | número de regiões/ciclos pode organizar grupos, pausas e retornos |
| Diferenciável | quais coordenadas e transformações são suaves | derivadas, tangentes e singularidades controlam continuidade de gesto |
| Métrica/Riemanniana | distância, ângulo, comprimento e curvatura | geodésicas e curvatura podem orientar percurso, duração e acento |
| Espectral | autovalores e modos de um operador, como o Laplaciano | modos de membrana/sala podem alimentar ressonadores ou mapas de alturas |
| Simplética/Hamiltoniana | estrutura de espaço de fase e evolução que preserva a forma simplética | relações posição–momento e conservação de energia para movimentos mecânicos |
| Fibrada/conexão | estados locais sobre uma base e seu transporte | fase/timbre de uma voz pode ser transportado; holonomia transforma o retorno |
| Quântica | estados em espaço de Hilbert, amplitudes complexas, operadores e evolução | interferência entre caminhos e distribuição de escolha em modelo computacional |

O **número de Chern** oferece uma ligação precisa entre geometria, topologia e
física quântica. Para uma banda isolada definida sobre uma superfície fechada
orientada `Σ` no espaço de parâmetros, o primeiro número de Chern é o
invariante topológico integral
`C₁=(1/(2π))∫_Σ Ω_B ∈ ℤ`, com a convenção de sinal fixada pela orientação e
pela definição da conexão. A fase de Berry é a holonomia geométrica acumulada
num percurso; o número de Chern classifica globalmente o fibrado da banda. Não são
sinônimos: a fase depende do caminho, enquanto o inteiro é um invariante global
sob as hipóteses do modelo. Esse inteiro não é a integral de uma curva
qualquer: requer uma banda isolada sobre uma superfície fechada apropriada. O
valor persiste sob deformações suaves enquanto o gap fica aberto; uma
degenerescência/fechamento de gap pode mudar a classe.

Para estados normalizados `|u(λ)⟩`, a conexão de Berry pode ser escrita
`A(λ)=i⟨u|∇_λu⟩`; a curvatura é `Ω_B=dA` (ou o rotacional de `A` em três
coordenadas). A conexão muda com a escolha de fase local do autovetor, enquanto
a curvatura e o número de Chern são invariantes de gauge. Stokes relaciona a
integral da conexão no contorno à integral da curvatura na superfície, quando
há uma escolha de gauge regular no patch. Em malha discreta, o método de
Fukui–Hatsugai–Suzuki usa produtos de elos entre autovetores vizinhos para
calcular o inteiro de modo invariante a gauge; é candidato para um protótipo,
mas ainda precisa de auditoria numérica independente. Em termos geométricos,
`Ω_B` é uma 2-forma e a integral é sobre a superfície de parâmetros inteira,
não sobre a curva temporal percorrida por uma execução.

### Geometria rítmica: forma do ciclo e percurso

Geometria rítmica trata uma célula temporal como espaço estruturado. Para um
ciclo dividido em `N` posições, cada evento pode ser um ponto `pᵢ` num círculo
ou num toro de fases; distância circular, ângulo, simetria, vizinhança e
trajetória definem relações entre ataques, pausas e acentos. A composição pode
operar sobre essa estrutura por rotação, reflexão, dilatação/contração,
translação modular, sobreposição de ciclos e deformação contínua. A seed
registra a construção, enquanto parâmetros composicionais controlam densidade,
assimetria e intensidade da transformação.

No **Study Mode — Rhythmic Geometry**, o objetivo é analisar música concreta:
partir de uma faixa/caso musical, decompor a polirritmia por instrumento ou
camada, explicitar ciclos, compassos, pulsos, subdivisões e acentos, e então
transformar essa leitura em gráficos didáticos sincronizados. O modo também
deve comparar a leitura com células de clave, rumba clave, bossa nova e
tresillo, mantendo variantes/orientações identificadas e ciclos completos. O
usuário deve poder ver ataques,
acentos, pausas, contagem e subdivisões; rodar ou espelhar uma célula; e
sobrepor duas ou mais linhas para observar polirritmias, coincidências,
deslocamentos e fases de retorno. A tela pode combinar anéis concêntricos para
níveis de subdivisão, raios/eventos para ataques e uma linha móvel de tempo; a
mesma grade deve alimentar a reprodução, para a animação representar eventos
reais e não ser decorativa. Cada linha deve poder ser silenciada/isolada e
receber variação calculada independente. Em um breakdown, cada pista representa
uma função musical (por exemplo guitarra, bumbo/caixa, toms), não apenas um
padrão abstrato; a forma da música deve mostrar quando a relação ou a
instrumentação muda.

O material observado em @rhythmicgeometry também aponta para um segundo uso,
separado do breakdown de faixas: **Orbit**, um estudo generativo de razões
rítmicas. A captura fornecida mostra quatro ciclos em `3:4:5:7`; cada camada
percorre o círculo em velocidade/fase próprias, e o rastro acumulado registra
visualmente a história da relação. No Batruc, isso pode se tornar um modo
compositivo: desenhar cada ciclo como órbita/polígono de cor própria, acumular
o caminho numa camada de persistência ajustável e permitir limpar, congelar,
esmaecer e comparar rastros. O rastro deve derivar da trajetória matemática
dos eventos, com velocidade e fase ligadas ao mesmo relógio do áudio; não é uma
onda de forma ornamental independente.

É necessário definir claramente o significado de cada razão: eventos por ciclo
comum, frequências de osciladores ou durações relativas. A representação
visual, o número de ataques por camada e o instante de realinhamento dependem
dessa escolha. Exibir a convenção, a fase inicial e o cálculo de retorno; não
inferir o período comum apenas pelo desenho. O usuário pode editar as razões e
as fases, silenciar/solo camadas e ouvir como a figura muda com o groove.

**Estado atual do Batruc:** o preset `3:4:5:7` já existe no core JUCE e no
protótipo web; ele aplica os ciclos às quatro primeiras vozes e alimenta a
composição. No JUCE, o painel de percurso agora tem dois estudos rítmicos
transitórios, alimentados pelas vozes ativas, seus overrides de ciclo, fases e
transporte: **Orbit** mostra anéis/fases por voz e o traço resultante da soma
vetorial normalizada; **Study** representa pulsos em uma janela comum e mostra
o LCM da grade limitada. Razões inteiras preservam o período exato até 512
divisões; razões fracionárias são aproximadas numa janela máxima de 32 seções,
e essa condição aparece na legenda. O cálculo e o desenho rodam na interface,
fora do callback de áudio; não alteram o som nem criam outro sequenciador.
Esses modos observam a composição gerada pelo Batruc; não transcrevem uma faixa
nem reproduzem os algoritmos internos de Rhythmic Geometry. Uma inspeção na
janela confirmou Orbit e Study visíveis, sem sobreposição aparente ao
gráfico/mixer; Study mostrou 512 divisões aproximadas para a configuração de
12 vozes observada. A validação do caso simples `3:4`, retorno de fase e
playback continua pendente.

Riff tem uma função diferente: escrever células contra um compasso e mostrar
deslocamento, acentos e retorno. O Batruc já possui calendários de vozes,
transformações seeded, forma longa e edição especializada do baixo, mas ainda
não possui editor geral de células por stem nem visualização dedicada de frase
contra compasso. Essa lacuna permanece aberta; não tratar a forma de sete ciclos
como equivalente a um Cell Sequencer.

Cada gráfico de breakdown precisa expor os dados que sustentam a interpretação:
fórmula de compasso, unidade de subdivisão, contagem por ciclo, duração de cada
camada, fase inicial, pontos de acento e regra de realinhamento. A legenda deve
distinguir o ciclo local de uma pista do ciclo comum de todas as pistas. Ao
declarar que a figura “volta ao primeiro tempo”, o sistema deve demonstrar o
alinhamento pela contagem/aritmética modular e pela referência auditiva, não
deduzi-lo apenas pela simetria do desenho.

**Polígonos inscritos no círculo:** para uma camada com `N` divisões iguais,
desenhar o polígono regular de `N` vértices dentro do círculo; cada vértice
marca uma posição da subdivisão, cada lado representa o avanço até a próxima e
o playhead percorre a borda na ordem temporal. Mostrar linhas radiais do centro
aos vértices e rótulos de contagem selecionáveis. Sobrepor polígonos de `N` e
`M` lados em raios concêntricos distintos torna visível a polirritmia `N:M`;
interseções de vértices indicam coincidências, e o retorno conjunto mostra o
ciclo de alinhamento. Para subdivisões hierárquicas, anéis internos/externos
podem separar pulso, subdivisão e microdivisão, com contraste visual discreto.
Eventos de clave/tresillo e outras células não uniformes são pontos/acento
independentes sobre a grade, ligados em sua própria ordem; não deformar a
notação para fazê-los parecer um polígono regular. Rotação do polígono é fase
inicial, espelhamento é transformação explícita e a contagem deve permanecer
sincronizada com o áudio.

**Estudo de caso 3:7 — Tool, “Invincible”:** a descrição fornecida pelo autor
propõe a guitarra como ciclo de sete pontos (septágono) e os toms como ciclo de
três; bumbo/caixa sustentam uma camada de acentos, primeiro nos pontos 1, 4, 6
e depois 1, 3, 5, enquanto o motivo de três passa dos toms para a caixa. Para
visualizar o mecanismo, usar um círculo de fase comum com o septágono externo e
um triângulo interno. Dividir o tempo comum em `lcm(7,3)=21` passos mínimos,
marcar em pistas separadas os eventos `k mod 7` e `k mod 3`, e iluminar os
retornos coincidentes. A mudança de acentos é uma transformação de camada; a
troca toms→caixa é uma troca de timbre/orquestração, não uma mudança da relação
3:7. Permitir pausar, desacelerar, contar em 7 e em 3, isolar cada pista,
alternar a fase inicial e ouvir a sequência completa de 21 passos. A frase
“triângulo dentro do círculo dentro do septágono” pode ser a leitura visual da
interação entre ciclo 3, órbita/fase circular e ciclo 7, desde que a interface
mostre claramente qual figura corresponde a cada camada.

Esse caso é uma hipótese de análise baseada na descrição compartilhada pelo
autor, não uma transcrição verificada da gravação. Antes de publicar um gráfico
didático, conferir áudio, andamento, subdivisão, orientação, acentos e contagem
com a faixa original; registrar fonte, trecho temporal, autoria da análise,
grau de confiança e leituras alternativas. O gráfico explica uma leitura
musical e não deve se apresentar como a única análise possível.

**Estudo de caso — Meshuggah, “Pitch Black”:** captura fornecida pelo autor do
post @rhythmicgeometry mostra a afirmação de que o riff agrupa tercinas num
padrão repetido de dez notas, traçando um polígono antes de retornar ao primeiro
tempo após quatro compassos. Para o Batruc, representar a camada de dez ataques
sobre uma grade ternária e compará-la à camada de pulso/compasso; destacar cada
ataque e a evolução do polígono ao longo do tempo. O enunciado “retorna após
quatro compassos” fica registrado como afirmação do post, a verificar contra a
gravação e a unidade métrica adotada: por exemplo, em 4/4 com três subdivisões
por pulso, quatro compassos correspondem a 48 subdivisões e não coincidem
automaticamente com um ciclo de dez. O Study Mode deve mostrar essa conta e
explicar qualquer outra convenção que produza o retorno descrito.

**Estudo Orbit `3:4:5:7`:** outra captura fornecida pelo autor apresenta quatro
ritmos orbitando a velocidades distintas; as linhas sobrepostas deixam um
rastro que evolui como uma figura geométrica. Tratar como demonstração
generativa, não análise de faixa. O Batruc pode explorar a relação entre cada
trilha de eventos, seu polígono/fase e o desenho acumulado, com controles de
persistência do rastro e audição por camada. A fórmula que converte as razões
em relógios/eventos precisa acompanhar a visualização e tornar explícito o
realinhamento escolhido.

**Estudo de frase/células — Meshuggah, “ObZen”:** a captura fornecida descreve
três células rítmicas distintas numa frase de 68 semicolcheias, isto é, `17/4`,
enquanto a bateria mantém uma grade de `4/4`; o riff gira em relação a essa
grade. O autor observa no esquema as partes A, B, C e D. Isso é um exemplo
interessante de organização em seções, não requisito geral do Batruc. Caso
inspire uma ferramenta Riff, convém separar nomes de seção da identidade das
células: só as letras não dizem se cada parte introduz uma célula nova, repete
uma anterior ou combina várias. Não deduzir os comprimentos internos das três
células nem o mapeamento A–D só pela captura. Com semicolcheia igual nas duas camadas,
uma frase tem 17 tempos de semínima e a bateria tem 4; o primeiro alinhamento
natural de início de frase e início de compasso ocorre após `lcm(17,4)=68`
tempos de semínima — quatro frases de 17/4 ou dezessete compassos de 4/4. Isso
é distinto de um reset de arranjo imposto pelo compositor.

Esse exemplo aponta para investigar um **Riff/Cell Sequencer de composição**,
distinto do sequenciador atual do baixo: células nomeadas e editáveis poderiam
ser organizadas em seções opcionais, encadeadas, repetidas, deslocadas, invertidas
ou recombinadas em frases
multivozes; cada voz pode manter seu próprio ciclo contra uma grade comum. A
seed pode propor ordem e transformações, mas a sequência deve ser visível,
editável, reproduzível e convertível em áudio/partitura. O editor de baixo
existente continua especializado em notas, alturas e durações da linha de
baixo; não o tratar como editor geral de células de todas as vozes.

As células servem primeiro como objetos de estudo e pontos de partida para
transformações autorais, não como presets que alegam representar integralmente
uma tradição. Clave tem organização em ciclo de dois compassos, direção 3–2 ou
2–3 e variantes como son/rumba; bossa nova requer padrões de acompanhamento e
variações documentados em vez de ser reduzida a uma única “bossa clave”. O
tresillo pode funcionar como padrão cíclico e também como camada rítmica sobre
uma pulsação binária. O modo deve mostrar a fonte, a contagem usada e o contexto
de cada transcrição; não deve fundir nomes, células e práticas culturais.

Isso oferece uma ponte entre geometria animada e som: cruzar uma região pode
disparar ou suprimir um evento; a distância ao ponto seguinte pode definir
duração; curvatura ou velocidade podem modular acento/timbre; a simetria pode
gerar inversão ou resposta entre vozes. Cada mapeamento precisa ser audível,
editável e reproduzível. Essa geometria de padrões no tempo é distinta da
geometria de Berry: o número de Chern exige uma família de estados próprios,
uma superfície fechada de parâmetros e uma banda isolada; não surge apenas por
desenhar um percurso rítmico sobre um círculo.

**Experimento conceitual Batruc:** tomar as fases das vozes V1/V2 como
coordenadas periódicas de um toro `T²`; definir uma matriz Hermitiana pequena
`H(φ₁,φ₂)` de acoplamento; verificar que uma banda permanece isolada; calcular
curvatura e `C` numa grade limitada. O percurso musical pode acumular fase
geométrica e alterar relações de entrada/saída, enquanto `C` classifica regimes
globais de acoplamento. Isso seria um modelo quântico-inspirado calculado no
software, não um efeito quântico físico no áudio. Sem espectro, gap,
autovetores e integral devidamente verificados, não se deve exibir um Chern
number.

Essas camadas não formam uma lista única de “tipos de espaço”: por exemplo,
um domínio pode ser topológico, diferenciável, métrico e espectral ao mesmo
tempo. Geometria local também não determina sozinha a estrutura global. O
resultado de Gordon–Webb–Wolpert dá domínios não isométricos com os mesmos
espectros de Laplace; portanto, espectro não identifica sempre a forma. Para o
Batruc, isso abre uma pergunta composicional: duas geometrias visualmente
diferentes podem compartilhar uma paleta de modos, mas organizar os eventos de
forma distinta?

Em física teórica, os caminhos mais próximos são: (1) mecânica Hamiltoniana e
geometria simplética para trajetórias e conservação; (2) equações de onda e
problemas espectrais para modos de salas, membranas e corpos; (3) fibrados e
conexões para transportar fase/estado interno; (4) espaços de Hilbert e
operadores unitários para interferência e evolução quântica. O Batruc já tem
um estudo visual de pêndulo e campos XY, mas ainda não tem solver espectral de
sala/membrana nem simulação quântica. Uma versão “quântica” deve ser declarada
como modelo computacional, expor estado, operador, regra de medida e
reprodutibilidade; usar apenas palavras como superposição ou incerteza não
constitui modelo.

Primeiro experimento recomendado: comparar duas formas com conectividade
distinta e duas com espectro semelhante; calcular poucos modos em malha pequena
fora do callback de áudio, visualizar nós/antinós e converter modo ou fluxo em
eventos por uma regra documentada. Medir CPU e conferir acusticamente antes de
conectar o solver ao instrumento em tempo real.

### Redes neurais como ferramenta composicional

Redes neurais não são uma classe de espaço geométrico. São operadores que podem
aprender relações sobre dados estruturados. Redes geométricas e GNNs poderiam
receber como grafo os nós/arestas dos micropatterns e stems, e calcular
probabilidades de aproximação, afastamento, pausa ou mudança de matéria. Para
quatro ilhas, regras explícitas são hoje mais econômicas, auditáveis e
reproduzíveis; uma GNN só se justifica quando houver exemplos suficientes ou
relações mais numerosas que as regras manuais não descrevam bem.

Uma rede informada por física (PINN) poderia aproximar uma solução de equação
diferencial ou ajudar a inferir parâmetros, penalizando resíduos da equação e
condições de contorno. Isso não garante por si só estabilidade, conservação,
precisão acústica ou generalização. Para o Batruc, um primeiro uso seria offline
como aproximação comparada a um solver numérico de referência, nunca como
substituição automática do solver no callback de áudio.

Um módulo neural generativo deve funcionar offline ou com inferência pequena e
bounded, sem rede externa; declarar corpus/licenças, entradas, pesos, seed,
temperatura e critérios de avaliação; permitir congelar/desligar o modelo; e
exportar decisões como operações compreensíveis. A rede complementa as
estruturas matemáticas escolhidas pelo compositor, não as mascara.

### Mecânica estatística como motor de escolha

O fator de Boltzmann pode ponderar estados candidatos por energia:
`pᵢ = gᵢ exp(−Eᵢ/(kBT))/Z`, com `Z=Σⱼgⱼ exp(−Eⱼ/(kBT))`. No instrumento,
uma versão adimensional com `kB=1` seria `pᵢ=gᵢ exp(−Cᵢ/τ)/Z`, onde `Cᵢ` é
um custo composicional explícito (por exemplo, colisão de
registros, afastamento da célula ou excesso de densidade), e `gᵢ` o número de
caminhos que levam ao estado. `τ` é temperatura composicional adimensional e
controlaria concentração versus dispersão de escolhas. A seleção seria
reprodutível por seed e limitada a um conjunto finito. Esse parâmetro não é
temperatura física nem mede emoção, groove ou “organicidade”.

A distribuição de velocidades de Maxwell é outra distribuição:
`f(v)=4π(m/(2πkBT))^(3/2)v² exp(−mv²/(2kBT))`, para `v≥0` em um gás ideal
clássico tridimensional em equilíbrio. Ela combina fator `v²` e decaimento
exponencial da energia cinética.
Pode inspirar um perfil de velocidade de partículas/gestos, mas não deve ser
confundida com o peso de Boltzmann sobre estados discretos. Primeiro ensaio
possível: comparar Boltzmann sobre custos de transição entre células com
escolha uniforme, mantendo o mesmo seed e medindo variedade, repetição,
limites rítmicos e qualidade de escuta. Nenhuma dessas distribuições está
implementada no motor generativo atual.

As microlinhas agora são duas membranas assimétricas: `y₊=0,55x+g(x)+δ₀` e
`y₋=−0,55x−g(x)+δ₁`, onde
`g(x)=(1−x²)(b₀+a·sin(πx)+c·sin(2πx))`. Cada faixa tem largura
`hᵢ=h+0,008sin(0,29θ+φ+2,17i)` e deslocamento próprio
`δᵢ=0,024sin(0,23θ+0,41φ+2,17i)`. O envelope mantém as pontas da deformação
harmônica fixas, enquanto a deriva independente faz as duas faixas aproximarem
ou afastarem seus percursos. O core
calcula contorno, inclinação e distância assinada à borda. A visualização JUCE
chama as mesmas funções de contorno e inclinação; o motor JS porta os mesmos
coeficientes e acompanha o campo no `geometry-trace`. Assim, desenho e seleção
de ataques usam a mesma geometria em cada saída, embora os calendários de
eventos continuem com implementações próprias. Isso é uma abstração musical de
campos de micropatterning, não uma simulação de adesão celular nem reprodução
de um protocolo experimental.


## Afinidade gradual nas bordas

As máscaras não comutam mais diretamente entre ataque pleno e silêncio. Para a
distância assinada aproximada `d` à borda (radial nas ilhas, perpendicular nas
faixas; negativa dentro e positiva fora), a faixa de
afinidade usa `η=0,055` e `w(d)=1−s(clamp((d+η)/(2η),0,1))`, com
`s(q)=q²(3−2q)`. Assim, a intensidade cai suavemente perto do limite e chega a
zero além da faixa. O scheduler admite somente eventos com `d<η`; o peso aplica
a transição. O mesmo cálculo está no core JUCE e no gerador JS, e a interface
desenha uma auréola discreta ao redor da região de influência. Campo livre
mantém a Lupa gaussiana já descrita. A faixa de afinidade é uma decisão
composicional do Batruc, não um parâmetro biológico medido.

A animação do plano XY é uma leitura temporal da mesma órbita; o gráfico de
vozes e a curva Fourier permanecem camadas diferentes. Fourier só atua no
material sonoro quando a curva é escolhida como fonte geométrica da V1. Na
ausência dessa opção, a curva Fourier explica/ilustra a forma, mas não muda o
som. O campo, por outro lado, já altera a seleção ou a intensidade dos ataques.

O Mixer JUCE oferece movimento de panorama por canal percussivo, derivado da
posição do evento na trajetória: `p = clamp(p₀ + m·s·x, −1, 1)`. `p₀` é o
panorama-base; `m` é a profundidade definida pelo músico; `x ∈ [−1,1]` é a
coordenada horizontal; `s` interpreta intenção e intensidade dinâmica. A
intensidade amplia o gesto ao construir, é contida ao rarefazer, aproxima os
ataques do centro ao encontrar e alterna lados ao contrastar. A pausa formal e
os canais inativos suspendem o gesto e conduzem o pan de volta ao ponto-base
com suavização. Esse mapeamento é uma decisão composicional autoral do Batruc,
sem equivalência universal presumida entre trajetória e posição estéreo. A
validação deve comparar profundidade zero e movimento, em mono/estéreo, e ouvir
o retorno nas pausas.

O mesmo fluxo alimenta o amortecimento da percussão: o intervalo entre ataques
define uma densidade `q`; a intensidade dinâmica `I` e a intenção `S` controlam
`d = clamp(0,35·q·(1,10 − 0,50·I)·S, 0, 0,42)`. O coeficiente extra
`exp(−d/(0,22·Fs))` modula modos e ruído a cada amostra. A resposta se acumula
somente após ataques percussivos; na pausa formal o histórico é limpo e o
coeficiente retorna a 1. Esse cálculo complementa o controle global **Cauda**,
que permanece como base de decaimento. Construir/rarefazer, encontrar e
contrastar são pesos expressivos do Batruc, definidos para produzir relações
audíveis e revisáveis; não codificam afirmações acústicas universais. A
validação precisa ouvir densidades diferentes e o retorno depois da pausa.

Essa é uma primeira organização calculada, não uma escolha manual de trechos.
O modelo de dados futuro para editor deve representar cada segmento como
`{identidade do motivo, trecho-fonte, operação, intensidade, duração,
parâmetros travados}`. A′ referencia A e guarda apenas as operações/deltas,
em vez de duplicar notas; retorno A reaplica o motivo-base com memória acumulada
limitada. A mesma seed, a mesma ordem e os mesmos parâmetros precisam
reproduzir forma e partitura. A cena salva uma fotografia do estado; um banco
futuro deve poder guardar uma forma inteira como preset nomeado, mas não é
pré-requisito para a geração.

Essa hierarquia segue as entradas do [Atlas RASGO](../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md)
“Instrumentos e ambientes livres para composição algorítmica matemática”,
“Gramani: dois livros e realização rítmica no Batruc” e “Operações generativas
entre instrumentos RASGO para o Batruc”: compor padrões, preservar identidade
motívica, criar associação/dissociação de vozes e distinguir passo, motivo,
forma e histórico. A referência do Studio é direção composicional por
frase/seção; o [Antitotem](../ANTITOTEM/docs/PESQUISA_SEQUENCER_GENERATIVO.md)
fornece precedente para transformações de motivo e histórico. São princípios
estudados; o código do Batruc é próprio.

## Vocabulário de operações

| Operação | Contrato musical | Primeira implementação |
| --- | --- | --- |
| Inversão de vozes | Trocar agendas rítmicas entre dois canais, preservando timbre e registro de cada canal | `invertEventPlanVoices` no core; interface pendente |
| Inversão de acorde | Mover sucessivamente a voz mais grave acima da superior, preservando classes de altura | `invertChordVoicing` no core |
| Inversão de alturas | Espelhar o contorno em torno de eixo MIDI declarado; rejeitar notas fora da faixa | `invertBassPitchContour` no core |
| Inversão de tempo | Nomear a operação concreta: retrógrado de notas completas, com início e duração espelhados na seção | `retrogradeBassScore` no core |
| Inversão de ritmo | Investigar retrógrado de intervalos, complemento de ataque/pausa ou troca entre vozes como operações distintas | troca entre vozes no core; outras pendentes |
| Inversão de algoritmo | Calcular inversa somente quando o domínio e a função a permitem; uma rotação de fase tem inversa modular | `inversePhaseRotation` no core; sem inversor universal |
| Recombinação | Selecionar trechos de duas fontes identificadas e proteger as fronteiras | `recombineBassBars` escolhe quatro compassos de dois planos e limita sustentações nas bordas |
| Recálculo | Alterar uma receita e reconstruir o resultado, conservando a versão anterior se a nova falhar | `recalculateBassScore` no core |

As funções acima atuam em dados preparados; ainda não comandam o som do app
JUCE. O baixo nativo ainda não foi ligado ao renderer. A transformação não
deve receber o nome de “inversão” se apenas alterar ganho ou sortear parâmetros.

## Variação durante a peça

### Contrato pendente para variação de funções

A interface deve permitir variar uma função sem confundir quatro ações
diferentes: escolher outra família matemática, alterar seus parâmetros,
transformar o resultado da função ou trocar o destino musical do resultado.
Cada operação precisa mostrar **fonte → transformação → destino**, declarar se
usa seed e indicar seu escopo: célula, compasso, frase, seção ou peça.

Uma seed pode reproduzir a proposta inicial e as decisões probabilísticas, mas
não deve apagar edição manual. Travas independentes devem preservar função,
parâmetros, ritmo, timbre ou forma; intensidade zero precisa congelar a
dimensão; desfazer e A/B devem recuperar estados completos. A variação deve ser
calculada fora do callback e aplicada numa fronteira musical segura. Este é o
contrato de produto; o seletor de função atualmente visível no Study ainda não
implementa esse sistema.

O Rasgo Modular oferece precedentes internos úteis, consultados em leitura:

- **VARIA / MotionEngine:** movimento lento e correlacionado de parâmetros,
  intensidade de zero a dois, congelamento real em zero, proteção de mixer e
  master e atualização em taxa baixa. No Batruc, investigar curvas de
  fórmula, fase, densidade e matéria em escala de frase; recalcular agendas
  afetadas antes da fronteira musical. O campo deve ter limites por parâmetro
  e permitir congelar uma dimensão sem parar as outras.
- **MUDA / EVOLUI / CRUZA / PatchGenetics:** mutação do estado atual,
  acumulação de passos pequenos e recombinação de parâmetros compatíveis;
  travas preservam partes escolhidas. Para o Batruc, candidatos de receita
  podem conservar fórmula, seed, voz ou forma, variando somente o que o músico
  liberou. O próprio Modular registra que mutações sem proteger parâmetros
  críticos podem silenciar um patch; limites e escuta são parte da operação.
- **PHRASE, DECISION, FUNCTION, EUCLID e SWITCH:** motivo com pausa/cadência/
  retorno; escolha com memória; função controlável; distribuição de ataques;
  roteamento entre fontes. Servem como perguntas de design para frase,
  decisão e troca de relações, cada uma com domínio e destino musical claros.

O [Rasgo Synth Studio](../RASGO_SYNTH_STUDIO/README.md) já explora peça
generativa, direção por frase/seção e uma célula compartilhada entre core e FM.
A memória composicional entre faixas consta como pendência no Studio; no
Batruc, a ideia é conservar a identidade de uma célula ao transformá-la
em vozes, frases e seções próprias, com um plano de peça audível e editável.

No [Antitotem](../ANTITOTEM/src/core/SimpleSequencer.h), regras de passos
podem agir sobre o motivo: pausa pontual, rotação, repetição interna,
inversão de altura e retrógrado da ordem. O compasso é distinguido do loop
de passos. Isso ajuda a especificar escopo e orçamento de microeventos sem
importar o sequenciador para o Batruc.

No [Trioio](../TRIOIO/README.md), o músico julga uma resposta inteira, seção
ou trecho; aceitação, transformação e rejeição alimentam memória de escolhas.
Para o Batruc, a comparação entre versões deve guardar intenção declarada,
operações executadas e consequência escutada. A avaliação humana não vira uma
nota automática de qualidade.

Essas são referências internas de pesquisa. Nenhum código dos instrumentos
irmãos foi copiado para o Batruc nesta etapa. Qualquer adaptação futura
precisa fixar origem, versão, arquivo, licença, créditos e testes próprios.

## Próximo ensaio musical

Escolher uma célula curta de baixo, gerar original, inversão de altura e
retrógrado; recombinar dois compassos e recalcular a mesma célula com outra
fórmula de duração. Depois construir uma frase de duas vozes que passe de
apoio a resposta, retornar à célula reconhecível e organizar sete ciclos
com construção, mudança, expansão e retorno. Comparar os materiais com a mesma
seed e registrar o que se reconhece pela escuta sem olhar o gráfico. O plano
de peça, editor e renderização ainda precisam ser implementados.

## Contrato da forma longa

A forma longa deve separar quatro quantidades hoje confundidas: células por
ciclo, ciclos do motivo, repetições/transformações do motivo e ciclos totais
ou duração-alvo da peça. Assim, o músico poderá pedir **65 ciclos**, definir
**4:20**, ou repetir um motivo de **oito ciclos** até a peça alcançar **5:00**.
O encerramento acontece numa fronteira musical declarada e a interface mostra
antes de tocar a duração estimada e a duração efetiva resultante.

O plano de peça organiza introdução, desenvolvimento(s), transição, clímax,
retorno e coda. Cada seção declara comprimento, transformação, densidade e
fontes permitidas. Há três origens equivalentes: construção manual, proposta
reproduzível por seed e modo híbrido. A seed propõe e reproduz; nunca bloqueia
edição posterior. Silêncio continua possível como escolha explícita de célula,
voz ou seção, mas nenhuma etapa formal impõe uma região morta escondida.

## Corpus rítmico e recombinação

O [Groove MIDI Dataset](https://magenta.tensorflow.org/datasets/groove), CC BY
4.0, foi escolhido como primeiro corpus externo candidato. O pacote oficial
[MIDI-only (3,11 MB)](https://storage.googleapis.com/magentadata/datasets/groove/groove-v1.0.0-midionly.zip)
contém os MIDI sem os WAVs; SHA-256 publicado: `651cbc524ffb891be1a3e46d89dc82a1cecb09a57c748c7b45b844c4841dcc1e`.
O importador é offline e converte MIDI escolhido pelo usuário e metadados em
células internas de 1–16 compassos. Cada derivado deverá guardar fonte,
versão, licença, identificação da performance, seed e operações aplicadas. O
corpus não é necessário para tocar o instrumento e não há rede, parsing ou
alocação no callback. O app não baixa o conjunto; nenhum arquivo GMD está no
repositório até a data deste registro.

Primeiro vocabulário sobre células importadas: recortar, rotacionar, deslocar
fase, recombinar, intercalar vozes, preservar ou quantizar microtiming, alterar
densidade/intensidade e redistribuir ataques. O resultado deve entrar na mesma
agenda que alimenta som e gráficos, permanecer editável no Study e permitir
comparação A/B com a origem. Lakh não será embarcado devido à proveniência
individual inconsistente; E-GMD fica adiado por repetir o material simbólico
do GMD em uma expansão de áudio desnecessária para esta etapa.

Fontes adicionais para investigação, ainda não aprovadas para incorporação:

| Fonte | Uso possível | Estado de direitos |
| --- | --- | --- |
| [Lucerne Groove Research Library](https://www.grooveresearch.ch/) | Microtiming e relação bateria/baixo; comparação de realizações | Verificar licença de dados, MIDI e gravações separadamente |
| [Drum Patterns Dataset, Universidade de Trento](https://zenodo.org/records/18395007) | Variações humanas de dez padrões para estudar transformação | Confirmar licença dos dados no registro e no pacote |
| [DrumMachinePatterns](https://github.com/stephenhandley/DrumMachinePatterns) | Padrões textuais catalogados por estilo, candidatos a conversão em célula | Licença do repositório não resolve direitos sobre os livros/origem editorial |
| [POP-ROK](https://github.com/patchbanks/WaivOps-POP-ROK) | Referência de padrões pop/rock e dados simbólicos | CC BY 4.0 declarado; WAV de 12,6 GB não é necessário ao fluxo MIDI/síntese |

Essas fontes alimentam estudo e protótipos locais enquanto proveniência e
termos são auditados; nenhuma entra no pacote do Batruc nesta etapa.

## Leis matemáticas como material de composição

As leis de intensidade existentes devem operar sobre uma mesma célula e
coordenadas geométricas. O músico precisa ver nome/fórmula e curva, editar
parâmetros, combinar duas leis, recalcular preservando os eventos-base e
comparar o resultado anterior e o novo por A/B com desfazer. O núcleo usa
`blendedIntensityFormulaValue()`: mistura convexa limitada entre duas leis.
Os seletores A/B, peso, persistência e comparação A/B estão ligados ao
renderer. A análise `I` do gráfico de ondas alterna para uma vista das curvas
A, B e do multiplicador misturado aplicado aos ataques, usando as fases e a
janela temporal atuais. Essa curva descreve o fator de intensidade da fórmula;
não representa envelope ou forma de onda de áudio. Edição livre de coeficientes
e expressões ainda não existe: por ora o usuário escolhe leis nomeadas e
combina duas delas.

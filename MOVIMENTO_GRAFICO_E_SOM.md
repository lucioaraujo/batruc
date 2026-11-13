# Batruc — deslocamento gráfico como material de composição

Estado: estudo de função e gesto. O app JUCE desloca a **vista** das curvas
para a esquerda ou para a direita e possui uma primeira automação rítmica
independente da voz 2. As transformações composicionais mais gerais abaixo
seguem especificadas para desenvolvimento posterior.

## O que o movimento pode calcular

Uma curva que se desloca pode produzir música se sua posição e sua velocidade
forem parâmetros da receita e entrarem no cálculo da agenda. Para uma
coordenada horizontal normalizada `s` e tempo musical `t`, o primeiro modelo
é `gᵢ(s,t) = Aᵢ sen[2π(mᵢs − vᵢt) + φᵢ]`. O sinal de `vᵢ` determina a direção;
`|vᵢ|` determina quantos comprimentos de onda atravessam a tela por seção.
Esta expressão adapta a forma padrão de uma onda viajante
`y(x,t)=A sen(kx∓ωt+φ)`, cuja velocidade de fase é `ω/k`.

| Cálculo da imagem | Parâmetro musical possível | Evidência visual/auditiva a exigir |
| --- | --- | --- |
| deslocamento `dᵢ(t)=vᵢt+φᵢ` | fase e direção da frase | mesma posição aparece na curva e na agenda |
| velocidade relativa `vᵢ−vⱼ` | periodicidade dos encontros | pontos de encontro marcados e ataques associados |
| cruzamentos `gᵢ(s,t)=gⱼ(s,t)` | ataques, diálogo ou pausas | coincidências finitas e audíveis, com limite de eventos |
| gradiente `∂g/∂s` | articulação ou brilho | inclinação normalizada, sem saltos de ganho |
| curvatura ou aceleração `∂²g/∂s²` | tensão, dinâmica ou densidade | função legível, saturação limitada e comparação A/B |
| soma `gᵢ+gⱼ` | reforço/cancelamento | mostrar as curvas originais e a resultante |

Os eixos, unidades e normalizações devem aparecer na interface. Se `s` é
posição de tela e `t` é tempo de seção, isso é uma analogia composicional de
onda viajante; não representa automaticamente uma corda, ar ou outro material.
O modelo físico só pode receber parâmetros como massa, tensão e velocidade
de propagação quando essas grandezas participarem das equações e tiverem
unidades coerentes.

## Primeira experiência composicional proposta

Usar duas vozes com a mesma forma e velocidades `v₁` e `v₂`, inclusive sinais
opostos. O usuário edita velocidade e direção. O sistema calcula o percurso
das curvas e escolhe eventos quando a diferença entre elas cruza zero em
sentido declarado. Os mesmos eventos entram na partitura computacional e no
gráfico; um teto de eventos por seção impede explosões quando as curvas ficam
próximas. Comparar `v₂=0`, `v₂=v₁` e `v₂=−v₁` com os mesmos timbres e BPM
mostra se o gesto gera diferença musical relevante.

Em seguida, experimentar uma lei de intensidade baseada na distância
`D(t)=|g₁-g₂|`: intensidade maior no encontro (`1−D/2`) ou no afastamento
(`D/2`). Esses dois mapeamentos produzem sentidos musicais diferentes a partir
do mesmo movimento. O músico deve escolher, ouvir e poder alterar o resultado.

## Epicíclos, espirais, fractais e grandezas do traço

Uma soma de Fourier pode ser desenhada como epicíclos. Para coeficientes
complexos `cₙ = rₙe^{iφₙ}`, a ponta percorre
`z(t)=Σ cₙe^{inω₀t}`; cada círculo tem raio `rₙ`, fase inicial `φₙ` e
velocidade angular `nω₀`. Em coordenadas,
`x(t)=Σrₙcos(nω₀t+φₙ)` e `y(t)=Σrₙsin(nω₀t+φₙ)`. Velocidade e aceleração
seguem por derivação termo a termo. Para uma série realmente reconstruída, os
coeficientes pertencem à curva de referência; mudar os raios ou frequências
gera uma nova trajetória, não uma aproximação fiel. A trajetória, os
epicíclos e os eventos musicais devem compartilhar o mesmo relógio.

Fibonacci pode definir outra receita: pesos `rₙ ∝ 1/Fₙᵖ`, velocidades
`ωₙ = Fₙω₀` ou intervalos entre eventos. São escolhas diferentes e precisam
ser nomeadas. Múltiplos inteiros de `ω₀` voltam a se repetir; uma razão
irracional como `φ` não tem período comum exato no modelo ideal, mas isso não
garante um resultado musical. A espiral de Fibonacci feita por quadrados e
arcos é uma aproximação geométrica; a espiral áurea exata é logarítmica,
`r(θ)=ae^{bθ}`. O instrumento deve identificá-las separadamente.

Uma matriz móvel `M(t)` transforma uma trajetória `u(t)=M(t)v(t)`. Sua
velocidade é `u̇=Ṁv+Mv̇`; determinante, escala, cisalhamento e rotação podem
alterar área, direção, cruzamentos e densidade de eventos. Iterações fractais
como `zₖ₊₁=zₖ²+c` ou sistemas de funções iteradas podem gerar pontos e células,
desde que o número de iterações e a saída sejam limitados. São sistemas
matemáticos de composição, não modelos físicos por si só.

O próprio desenho oferece medidas calculáveis: comprimento do traço
`L=∫||r′(t)||dt`, área orientada
`A=½∫(x dy−y dx)` e volume por secções
`V=∫A(s)ds`. Podem alimentar duração, densidade, acento, forma ou timbre; cada
receita deve mostrar unidades, normalização e a regra musical escolhida. Área
não é automaticamente volume, e nenhuma dessas medidas determina sozinha um
som.

**Estado no JUCE:** a soma Fourier e a animação do traço estão em camadas
separadas; ainda não há cadeia de epicíclos giratórios, fórmula visível de
velocidade/aceleração ou receitas Fibonacci/fractais com eventos. A geometria
do traço deve chegar ao áudio por eventos e medidas calculados, não por um
desenho decorativo. Próxima pesquisa: epicíclos Fourier inspecionáveis, receitas
Fibonacci com controle de pesos/frequências, transformações de matriz em
movimento e medidas de comprimento/área/volume com limites de cálculo.

## Estado do app e próximo incremento

- A vista temporal agora pode fluir nos dois sentidos com o tempo tocado no
  centro. Esse fluxo serve à leitura e não altera a agenda sonora.
- `Movimento V2`, no Mixer, usa `φ₂(b) = (φ₂,base + b·Δφ₂) mod 0,5`, com
  `b` igual ao número de compassos concluídos e `Δφ₂` entre 0 e 0,125 volta
  por compasso. A agenda correspondente entra no próximo compasso. Em cada
  troca, os tempos de ataque da voz 2 mudam; os tempos das outras vozes
  permanecem. O gráfico mostra a agenda ativa. A variação é por compassos,
  não um arrasto contínuo dentro do compasso. O áudio não é replanejado no
  callback: seleciona uma das 9.696 agendas preparadas.
- A Lei I ganhou oito opções. Duas usam um modelo de osciladores de mesma
  massa: `U=(n₁²x²+n₂²y²)/(n₁²+n₂²)` e `K=1−U`. São intensidades
  normalizadas, não energia acústica medida.
- Próximo incremento de código: levar velocidade e direção editáveis às
  demais vozes, calcular suas agendas fora do callback de áudio e mostrar
  os parâmetros na fórmula. A seed v1 permanece intacta.

Fonte física de partida: [OpenStax, *University Physics*, §16.2 — Mathematics
of Waves](https://openstax.org/books/university-physics-volume-1/pages/16-2-mathematics-of-waves)
para onda viajante e direção de propagação; [§16.5 — Interference of
Waves](https://openstax.org/books/university-physics-volume-1/pages/16-5-interference-of-waves)
para superposição. Os mapeamentos musicais da tabela são hipóteses autorais do
Batruc, a avaliar por cálculo, interface e escuta.

## Referência de pesquisa: spring-coupled triple pendulum

O autor identificou o perfil do Instagram [@integral_killer_](https://www.instagram.com/integral_killer_/)
como origem do carrossel e forneceu capturas de um artigo intitulado
*Spring-coupled triple pendulum: Lagrangian structure, equilibria, and
full-rotation dynamics of the bob*, atribuído na imagem a P. Prasad e datado de
12 Oct 2026. O perfil é a
fonte de publicação do carrossel conforme identificação do autor do Batruc;
isso não confirma que o titular da conta seja autor do artigo. A identificação
completa, o periódico ou repositório e o endereço bibliográfico original ainda
não foram confirmados; as capturas e o perfil são a proveniência da pista, não
uma fonte bibliográfica validada. A página do Instagram não pôde ser carregada
pela pesquisa web nesta sessão.

As páginas visíveis mostram um pêndulo serial com três coordenadas, geometria
e energia, matriz de massa, equações de movimento, equilíbrios, modos lineares
e espectro, além de uma transição para rotação completa. Isso inspira a
pesquisa de vozes com movimento autônomo: cada coordenada conserva uma
trajetória própria, enquanto o acoplamento cria dependência entre elas. A
maquete web implementou uma exploração relacionada, mas distinta: três
pêndulos individuais ligados em cadeia por molas efetivas, com equações
lineares de ângulo pequeno. Não é a geometria serial visível nas capturas e
seus parâmetros não foram extraídos do artigo.

Para uma futura receita de três vozes, usar coordenadas angulares `θ₁, θ₂,
θ₃` e velocidades `ωᵢ = dθᵢ/dt`. Partir do sistema mecânico declarado no artigo,
com parâmetros e unidades explícitos, e integrar sua equação de movimento fora
do callback de áudio. Derivar candidatos musicais de grandezas nomeadas:

| Grandeza mecânica | Possível controle no Batruc | Regra a tornar explícita |
| --- | --- | --- |
| ângulo `θᵢ` | fase ou posição da voz `i` | converter volta mecânica em volta de fase musical |
| velocidade angular `ωᵢ` | densidade ou duração entre ataques | cruzar limiares com histerese, com teto de eventos |
| extensão da mola | acoplamento rítmico entre vozes | escolher se aproximação ou afastamento cria encontro, atraso ou pausa |
| energia por pêndulo | intensidade ou dinâmica da voz | normalizar e preservar a transferência total calculada |
| modos normais / autovetores | proporções de movimento entre vozes | expor frequências e relações de fase do modo selecionado |
| separatriz / rotação completa | transição formal | mapear a passagem de oscilação a volta completa para uma mudança audível |

A primeira comparação deve manter a mesma seed e a mesma matéria sonora em
três condições: pêndulos independentes (`k=0`), acoplamento fraco e acoplamento
mais forte. Mostrar ao mesmo tempo `θᵢ(t)`, extensão de cada mola, energia e
marcadores de ataque. Limitar duração, passo de integração, erro numérico e
número de eventos; gerar a trajetória antes da execução para manter o áudio
determinístico e sem alocação no callback. O modelo serial completo permanece
como proposta de pesquisa; a aproximação implementada não deve ser apresentada
como reprodução ou validação do artigo.

### Implementação exploratória na maquete web — 24/10/2026

O seletor de movimento oferece três osciladores angulares individuais com
acoplamento linear efetivo entre vizinhos. `L` define `ω₀² = g/L`; `κ`
controla o acoplamento adimensional e `ζ` o amortecimento. A integração é
limitada e ocorre antes da reprodução. Os ângulos das três primeiras vozes
alimentam gráficos e eventos; o painel secundário mostra a trajetória animada
no plano de fase `θ₁ × θ₂`, colorida pelos grupos formais, com fronteiras de
ciclo e cursor. O painel aumenta de altura nesse modo e mantém a mesma escala
nos eixos. A regra formal de entradas também vale aqui: C1 toca V1; C2 adiciona
parte das vozes e o baixo; C3 libera o conjunto; C5 mantém só V1 audível embora
as três coordenadas sigam calculadas; C6 reintroduz parte do conjunto e C7
completa o retorno. Na máquina senoidal, o desvio de fase cresce nos ciclos
3–4 e volta ao valor inicial em 6–7. É uma receita de exploração composicional
baseada em aproximação linear, sem pretensão de equivaler ao sistema serial do
artigo.
A qualidade musical e a fidelidade física ainda não foram validadas por
escuta ou comparação numérica com a fonte.

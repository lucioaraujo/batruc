# Batruc — desenvolvimento

> Até 13/11/2026 este arquivo era o `README.md`. Foi renomeado na preparação
> para publicação: a apresentação pública passou a ser o novo
> [`README.md`](README.md), e este é o diário de desenvolvimento, de pesquisa
> e de decisões.

Nome adotado em 25/10/2026: **Batruc**. Nome anterior: **Trucmetry**, mantido
como nome histórico do mesmo instrumento. Fonte atual:
`RASGO/PROJETOS_AUTORAIS/BATRUC/`; o diretório anterior foi preservado em
`RASGO/RASGO_ARQUIVO/.local-preservation-20261025/TRUCMETRY-pre-rename/`.
Criação e desenvolvimento são a finalidade; autoria, licença e identidade
permanecem próprias. Ler [as instruções RASGO](../../AGENTS.md) e a continuidade
local antes de trabalhar. Organização e validações no
[registro da migração](../../RASGO_DOCUMENTATION/architecture/MIGRACAO_PROJETOS_AUTORAIS.md).
Aplicar o [procedimento de recursos](../../../PROCEDIMENTO_SEGURANCA_RECURSOS.md)
antes de tarefas controladas; publicação continua uma decisão separada.


Instrumento da família RASGO em incubação.
Criado em: 2026-10-10. Atualizado em: 2026-11-13.
Autoria e direção artística: Lúcio de Araújo.
Licença: GNU AGPL-3.0-or-later — texto integral em [LICENSE](LICENSE).
Base tecnológica definida pelo autor: C++ e JUCE.

No Linux, `./run` inicia a compilação **Release** otimizada quando disponível;
usa Debug apenas como alternativa de desenvolvimento.

O código autoral do Batruc, incluindo o protótipo local, segue GNU
AGPL-3.0-or-later, em alinhamento com os demais instrumentos RASGO. O arquivo
[LICENSE](LICENSE) contém o texto integral. Dependências e componentes de
terceiros, inclusive JUCE na implementação nativa, mantêm suas próprias
licenças e avisos; a licença do Batruc não as substitui.

O nome histórico **Trucmetry** permanece registrado nesta documentação e no
histórico de continuidade. Batruc é o nome atual do instrumento, do diretório,
do aplicativo e dos novos arquivos. Cenas antigas `.trucmetry.txt` e os
formatos internos `trucmetry-scene` e `trucmetry-composition` continuam
legíveis; as novas gravações usam `.batruc.txt` e os identificadores `batruc-*`.
Esta compatibilidade de leitura não muda os arquivos antigos no disco.

Estado em 2026-11-05: a hipótese de máquina de composição matemática e física
ganhou uma [maquete local de interface e audição](prototype/README.md). A
interface nativa organiza o instrumento em três campos contínuos: **Gráficos e
cálculos** (incluindo geometria e percursos), **Mixer** (até oito stems, com os
indicadores de fase dentro dos canais) e **Baixo**. A composição pode usar uma
seed reproduzível ou modo manual sem seed; neste, os controles dos canais passam
a indicar de 1 a 32 toques por seção, distribuídos segundo a fase de cada stem.
O Mixer oferece a ação explícita **Modo manual**; em modo manual, o mesmo botão
gera uma nova seed aleatória para voltar à composição seeded.
Cada abertura do app escolhe uma seed nova e evita as 64 últimas seeds
aplicadas, mantendo esse histórico nas preferências locais. **Nova seed** e o
retorno do modo manual também evitam esse histórico. Seeds digitadas pelo
músico e seeds explícitas de cenas continuam sendo respeitadas; elas passam a
contar para a próxima escolha automática.
Essa contagem é aplicada como ciclos no agendador matemático existente e ainda
respeita regras e máscaras que podem filtrar ataques. A interface combina
fórmulas e gráficos de até oito vozes com transformações selecionáveis. A
contagem manual repinta o gráfico principal mesmo com o transporte parado; no
áudio, mudanças de ataques, ciclo, fase e BPM entram no próximo bloco, mantendo
a posição musical atual.
Quando o MIDI importado tem um rótulo editorial conhecido no catálogo GMD local,
a seed usa esse rótulo como pista inicial para a família sonora e o groove; a
escolha continua editável e não desloca os ataques da fonte. O app reconhece
compassos 3/4, 4/4, 5/4 e 7/8 na importação e preserva o compasso escolhido ao
trocar de seed enquanto o MIDI permanece ativo. Rótulos desconhecidos não
forçam uma associação, e o mapa não certifica autenticidade estilística. Ao
trocar a seed com MIDI carregado, o limite de vozes também se ajusta para não
cortar a lane mais alta que contém ataques. A seed posiciona as lanes de apoio
no estéreo com limites por papel, mantém bumbo e caixa no centro e define
movimento panorâmico moderado nos outros stems; esses valores continuam
editáveis no Mixer.
Também há variação, intenção, curva dinâmica,
andamento e compasso, swing, groove, articulação, timbre provisório, duração
ajustável, um módulo de baixo com linha editável, fórmulas de duração, doze
matérias sonoras virtuais no JUCE, LFO, envelopes ADSR de amplitude e filtro,
gate e nível próprio, além de gravação do master. No painel JUCE do Baixo, os
envelopes ADSR e parâmetros VCF (corte, abertura e ressonância) usam knobs com
leitura numérica; nível e Gate continuam sliders. Tempos de envelope e
frequência de corte recebem taper de controle próprio, sem alterar valores
salvos nem o som enquanto o controle não é movido. A escuta crítica do autor
ainda está pendente. A maquete inclui
mixer com volume, panorama, mute/solo por canal, mute/volume master e zoom gráfico; são recursos
de exploração, não substitutos da qualidade sonora e profundidade didática
exigidas do instrumento final. A fonte ativa das pendências é
[TAREFAS.md](TAREFAS.md). A paridade integral entre HTML e JUCE é acompanhada
na [matriz de paridade](PARIDADE_HTML_JUCE.md); ela inclui composição, forma,
geometria, mixer, arquivo, gravação e conteúdo didático, não apenas o baixo.

Em 2026-11-03, a tela JUCE passou a seguir parte da organização visual do
[wireframe aprovado](PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg): cabeçalho
compacto com a marca, cartão principal para o Editor de Ciclos, cartão da
vista vinculada Onda/Fluxo e rodapé fixo com créditos. Study ocupa a área
principal à esquerda; o inspetor
lateral mostra ciclos por voz e a variação. Onda/Fluxo e os parâmetros de tempo,
intensidade e metrônomo ficam à direita. Os controles do percurso ficam acima
do gráfico, e os controles/título/traços da onda usam faixas distintas.
Em janela mais alta, as duas vistas crescem com o viewport. A logo foi reduzida
e os créditos permanecem no rodapé. O Baixo recebeu linhas separadas para
knobs, variação, LFO e percurso, com altura de módulo suficiente para o último
controle; os atalhos de Mixer e Baixo mostram o começo de cada módulo abaixo
do cabeçalho fixo. Capturas do app em 1080×800 e 1440×900 estão registradas na
[auditoria de layout](AUDITORIA_LAYOUT_COMPOSICIONAL_2026-11-02.md). Em
04/11/2026, o Study ganhou células rítmicas editáveis por voz, com até 32
ataques, persistência de cena e atualização de áudio no próximo bloco. O botão
**EUCLID** redistribui a célula selecionada em uma grade de 8 a 32 passos,
preservando dados de notas MIDI e permitindo desfazer a transformação.
O Mixer mantém oito stems. Cada coluna contém nível, mute/solo, pan, movimento,
fórmula, timbre, regra de evento, acoplamento, fase, ciclos e variação próprios.
Na importação MIDI GM, V7 reúne os tambores de mão (60–66) e V8 os acessórios
(54/56/58/67–77) e demais notas; essa separação conserva ataques simultâneos
entre as duas funções. O mapa Roland TD-11 mantém a nota 58 no V5 como aro de
tom grave.
Os parâmetros do timbre 8-bit também aparecem na coluna da voz que o utiliza.
Uma faixa de **Baixo** no alto do Mixer oferece ligar/desligar, nível, mute,
solo, panorama e medidor de sinal. Nível e mute/solo compartilham o estado da
página Baixo; o panorama do canal se soma ao panorama de cada nota. A cena
schema 50 preserva esse panorama.
Em janelas menores, role sobre o fundo do Mixer para alcançar a parte inferior
das colunas; a rolagem permanece dentro do Mixer e não muda de página.
O acoplamento mistura a curva de intensidade da voz anterior com a voz atual
(0 a 0,8); V1 não tem voz anterior e por isso seu controle fica desativado.
Ele altera a força dos ataques, sem deslocar seus instantes. O slider de nível
é um controle; a barra no cabeçalho de cada stem mostra o RMS real após o
fader da voz, antes do pan e do master.

Cada coluna do Mixer também tem um gráfico curto de trajetória panorâmica. No
modo **Desenho**, arraste para editar oito pontos cíclicos ao longo da seção; o
eixo horizontal acompanha o progresso musical e a linha vertical marca a
posição de execução. Os pontos são interpolados entre ataques: o pan-base define
o centro, **MOV** limita a abertura e **VAR** escala o movimento. Em **Campo**,
o deslocamento volta a seguir o X dos ataques. A curva manual só atua em
matérias percussivas, atualiza no próximo bloco de áudio e fica salva na cena
(schema 56); cenas anteriores continuam usando o percurso automático do campo.
Duplo clique sobre o gráfico desativa o desenho sem apagar seus pontos.

Em 09/11/2026, a tela Compor foi redistribuída em três cartões independentes:
campo móvel de ataques e células à esquerda, Study maior ao centro e Vista
vinculada à direita. O campo XY, antes acionado pelo botão CAMPO na área
central, agora fica sempre visível à esquerda e usa o progresso do transporte;
o botão foi retirado. Acima dele, uma prévia separada mostra a curva escolhida
no seletor geométrico e responde também à curva definida pela seed. Um cursor
percorre essa curva no mesmo tempo musical do campo. A máscara
do campo continua sob escolha do músico: as seeds partem de Campo livre, e
Ilhas/Microlinhas são selecionadas no rodapé. A grade de 16 posições amplia
suas células conforme a largura disponível. O círculo de Study mantém
linhas e anéis mais legíveis e os seus gestos continuam ligados à mesma
geometria de desenho. O gráfico da Vista vinculada é menor que o Study e
permite navegação visual por zoom e arraste; os controles de tempo e
intensidade associados afetam o som.

No aplicativo JUCE, a saída passa por um limitador estéreo local com lookahead
de 3 ms e teto nominal de −1 dBFS. A gravação master recebe o mesmo sinal após
essa proteção. O rodapé informa picos amostrais antes/depois e a redução máxima
do limitador na sessão. O detector estima true peak por reconstrução polifásica
4×, ainda sem certificação EBU, e não calcula loudness BS.1770. O guard tem
teste sintético para teto amostral, link estéreo, entrada não finita e seno
intersample; validar gravação WAV, latência e transientes no dispositivo segue
pendente.

No rodapé, o JUCE exibe também **CPU AUDIO**, a carga suavizada do callback em
relação ao orçamento de cada bloco de áudio. 100% corresponde a usar todo o
tempo disponível do bloco; leituras acima disso indicam que o callback excedeu
esse orçamento. A medida cobre a renderização JUCE, baixo, proteção de saída e
encaminhamento dos blocos de gravação no callback; não representa o uso total
de CPU do sistema.

O campo **Gráficos e cálculos** inclui um metrônomo de estudo independente do
transporte musical. Ele pode tocar com a composição parada, acompanha BPM e
compasso, oferece de 1 a 8 subdivisões por tempo, cinco timbres sintetizados
(madeira, digital, sino, aro e grave), acento no primeiro tempo, swing opcional
nível próprio antes do master/limitador e botão TAP, que estima o BPM pela
mediana das últimas cinco batidas. Vem desligado para evitar cliques
inesperados; quando ligado, entra também na gravação do master. **ENT** arma
uma contagem de um compasso antes de iniciar o transporte; só atua com o
metrônomo ligado e pode ser cancelada pelo botão principal. A opção é salva nas
cenas JUCE a partir do schema 51; cenas anteriores abrem com contagem desligada.
O HTML não oferece contagem de entrada.

Na janela JUCE, a marca, a seed e os botões **Editor · Mixer · Baixo** ficam no
cabeçalho fixo. Esses botões levam diretamente a cada campo; a barra de rolagem
vertical não aparece. A roda do mouse ainda permite alcançar os controles que
ficam abaixo da altura visível no Mixer e no Baixo. Transporte, estado do
master, créditos e medidor CPU AUDIO permanecem fixos na base. A animação
invalida a área atualmente visível, inclusive nos painéis abaixo do editor.

Em 05/11/2026, o Study passou a distinguir clique e arraste: clique curto no
anel ou numa fatia vazia cria um ataque; arrastar o anel desloca a fase;
arrastar um vértice muda seu tempo; `Shift` altera sua intensidade; clique
direito ou duplo clique remove um ponto manual; `Alt` devolve a voz à fonte
calculada. O seletor de função permanece visível nessa vista. Mixer e Baixo
recuperaram a largura integral depois de uma regressão de layout, e o painel
Sobre foi separado das coordenadas dos módulos. Essas correções passaram por
build Release e auditoria de bounds em 1080/1440, mas os gestos, o modal em
1920×1080 e a experiência musical ainda precisam de confirmação humana. A
lista consolidada e os critérios de encerramento estão no início de
[TAREFAS.md](TAREFAS.md).

Ainda em 05/11, a suspensão automática de C5 foi removida: a forma atual toca
continuamente, C5 chama-se **Expansão**, o baixo participa de C1 e C5 e suas
sustentações não são cortadas nessa fronteira. O Mixer oferece 17 famílias de
timbre — rock, jazz, eletrônica, latina, experimental, percussão, sonoplastia,
efeitos especiais, 8-bit, 16-bit, metal, avant-garde, samba, folk, funk,
computer music e Rasgo Modular. As famílias acrescentam cor de espectro, ruído
e cauda às vozes sem alterar ritmo, ataques ou articulação. A ordem de bateria
é estável em todas: V1 bumbo, V2 caixa, V3 chimbal fechado, V4 chimbal aberto,
V5 tons, V6 pratos, V7 percussão de mão e V8 apoio. Em arranjos MIDI, a família
colore os timbres sem substituir o papel importado de cada lane. São receitas
sintetizadas, não amostras nem reproduções fiéis de gêneros ou tradições. O
perfil **8-bit** aplica clock digital e quantização com resolução distinta
para bumbo, caixa, chimbal e demais papéis, depois da síntese percussiva de
cada stem. Isso preserva a função musical de cada voz enquanto muda sua
matéria de saída; ainda não há modelos digitais originais separados para cada
peça. O renderer offline usa o mesmo mapa e registra o índice da família no manifesto
JSON. Os knobs ADSR/VCF do Baixo foram ampliados e reorganizados em duas linhas.

Composições novas abrem com a família **Percussão** e esses oito papéis já
atribuídos. O balanço inicial usa 0,45 para V3 e 0,50 para V4, com os demais
canais em 1,00; é um ponto de partida sujeito à escuta, não uma correção
automática da dinâmica de cada ritmo. Cenas salvas recuperam seus próprios
timbres e níveis. A importação MIDI respeita o mix já ajustado pelo músico.

No Mixer, o seletor superior **Global** é o material de reserva: só soa nos
stems cujo seletor individual está em “Global”. A família aplica cor de
espectro, ruído e cauda por stem sem substituir o papel instrumental; por
exemplo, mudar a família de um arranjo MIDI mantém bumbo no stem de bumbo. O
renderer ancora as fundamentais percussivas para que o bumbo não herde a altura
arbitrária da voz matemática. São sínteses originais estilizadas, sem kits ou
samples e sem pretensão de reproduzir fielmente Rock, Samba, Funk ou outra
tradição. A articulação e a escolha de fontes sonoras apropriadas ainda exigem
desenvolvimento e escuta comparativa do autor.

Em 08/11/2026, a escuta do autor indicou que as famílias ainda soavam parecidas
e que alguns stems pareciam trocados. A investigação confirmou que os perfis
anteriores mudavam sobretudo diferenças discretas de cauda e espectro, e que
o renderer reconhecia a nota GM de cada ataque mas continuava usando o modelo
sonoro fixo da lane. A versão Release recompilada passou a variar também o
corpo e a duração do ruído no ataque, aumentando o contraste numérico entre
perfis. Quando
um MIDI GM está ativo, cada nota escolhe seu modelo sintetizado dentro da
lane: por exemplo, a nota 37 aciona uma aproximação de aro pelo modelo
`brushes` no V2 e a 38 aciona caixa.
O modelo de tom/membrana agora tem fundamental ancorada. Isso corrige a
atribuição por nota; ainda falta confirmar por escuta se cada peça e família
é reconhecível. V3/V4 têm durações de ruído diferentes, mas usam a mesma
família de síntese de chimbal. V5 ainda não afina cada tom segundo sua nota;
V6 ainda compartilha o modelo ride entre crash e ride. V7/V8 precisam de
modelos próprios para acessórios e efeitos.

Em 08/11, a leitura do corpus GMD ganhou o mapa Roland TD-11 declarado pela
fonte: notas 22/26 agora chegam aos stems de chimbal e 58 ao stem de tom.
O importador lê o mapa de tempo em todas as pistas, inclusive quando a pista
vem depois da bateria num MIDI formato 1. Integra cada intervalo de ticks com o
microssegundo por semínima vigente e converte o tempo resultante em posições
relativas dos ataques. O BPM equivalente do recorte ajusta a duração do ciclo
se estiver na faixa 20–220; o controle do app continua livre para mudar essa
duração sem desfazer a proporção temporal do mapa importado. Sem metaevento de
tempo, mantém-se o BPM atual e a distribuição métrica original.
Quando a pista do canal de bateria contém CC4, o importador preserva a posição
do pedal de chimbal por ataque (0 aberto, 127 fechado) e o renderer varia
continuamente a duração, o ruído e o equilíbrio modal. MIDI sem CC4 mantém as
articulações indicadas pelas notas GM; CC4 pode estar em outra pista do mesmo
arquivo e é associado pelo tempo absoluto. A orientação de valores segue a
[implementação MIDI Roland TD-27](https://static.roland.com/assets/media/pdf/TD-27_MIDI_Imple_eng04_W.pdf);
o módulo precisa estar configurado para transmitir o pedal como CC4.
Um estudo de dois compassos, com rastreio de ataques e WAVs isolados por stem,
está documentado em [midis/README.md](midis/README.md). O app Release compilou;
a escuta no aplicativo continua pendente.

O mapa de V1–V8 é uma organização interna do Mixer, não a numeração das pistas
do arquivo MIDI. O importador lê a percussão do canal MIDI 10 e classifica cada
ataque pelo número da nota, com mapa GM ou TD-11 conforme a fonte. O código
atual desativa a redução formal de vozes quando há fonte MIDI, permitindo
V1–V8 desde C1; ainda falta confirmar essa execução por escuta no app.
O mapa detalhado e o resultado dos testes estão
em [TAREFAS.md](TAREFAS.md); a proveniência do corpus está em
[midis/README.md](midis/README.md).

A forma longa continua tecnicamente baseada num motivo de sete ciclos. A etapa
seguinte separa ciclos do motivo, ciclos totais e duração-alvo: o músico poderá,
por exemplo, pedir 65 ciclos; definir 4:20; ou repetir um motivo de oito ciclos
até a peça alcançar 5:00. Introdução, desenvolvimentos, transições, clímax e
coda serão seções editáveis, propostas opcionalmente pela seed e sempre
alteráveis pelo músico. O contrato está registrado em [TAREFAS.md](TAREFAS.md)
e [OPERACOES_GENERATIVAS.md](OPERACOES_GENERATIVAS.md).

Para alimentar recombinação e criação sem transformar o instrumento num
reprodutor de presets, o primeiro corpus externo escolhido é o
[Groove MIDI Dataset](https://magenta.tensorflow.org/datasets/groove), CC BY
4.0; o pacote [MIDI-only oficial (3,11 MB)](https://storage.googleapis.com/magentadata/datasets/groove/groove-v1.0.0-midionly.zip)
contém o material simbólico sem os WAVs. O importador local lê arquivos MIDI
offline e os divide em células editáveis. Quando o caminho corresponde a uma
linha do `info.csv` local do GMD, a prévia associa estilo, tipo, compasso, BPM
e divisão do conjunto; a cena schema 55 preserva esses metadados separados da
licença e da atribuição declaradas. A associação é pelo caminho do catálogo,
não por checksum do dataset; os direitos permanecem não verificados. MIDI
externo sem correspondência não recebe rótulo editorial inferido. O importador
continua sem produzir um manifesto completo de origem/licença. Ao reabrir a
cena, o resumo e o tooltip da fonte recuperam também os metadados e o estado de
direitos. A seed mantém a fonte imutável e varia deterministicamente a dinâmica
por ataque e, em notas simultâneas, por camada. Na frase derivada, pode retirar
uma parcela limitada de ataques fracos em lanes de apoio densas, preservando
bumbo, caixa, downbeats e ataques fortes; as velocities ainda variam com a seed.
Também pode acrescentar pickups
discretos onde há articulações de apoio adequadas. Assim, a seed cria espaço
entre papéis sem apagar a base estrutural do ritmo. A rotação temporal é uma
edição explícita no Study.
O Study agora oferece operações explícitas sobre células — interlocking,
redução de densidade, microtiming, recombinação entre células e contornos de
intensidade — com desfazer/refazer e mantendo a identidade sonora da voz
selecionada. Em importações MIDI de vários compassos, **Recombinar por compasso**
também intercala os ataques do compasso correspondente de outra voz, compasso a
compasso, usando a seed e preservando a matéria sonora da voz selecionada. Só
fica disponível quando ambas as vozes têm ataques suficientes em ao menos um
compasso. **Equilibrar densidade com Vx** adapta, compasso a compasso, as lanes
de apoio V3–V8: quando uma tem material bem mais denso que a parceira, reduz seus
ataques para perto de 1,25× a contagem da voz mais esparsa, preservando downbeat
e ataques mais fortes. Compassos sem duas vozes ativas ficam intactos. Undo/redo
trata o par como uma única edição. Essas operações editam células escolhidas
pelo músico. A geração MIDI por seed também varia a densidade das lanes de apoio
com limites por papel; transformações globais sobre a peça ainda estão
pendentes. Nenhum
dataset é necessário para o funcionamento normal do instrumento; Lakh não será
embarcado por ter proveniência individual inconsistente, e E-GMD fica adiado
porque sua expansão de áudio não é necessária para esta primeira etapa.

O núcleo offline do importador está em `src/core/MidiRhythmImport.*`. Ele lê
Standard MIDI File formato 0/1 com resolução PPQ, reconhece o canal 10 de
percussão General MIDI, preserva microtiming e velocity e agrupa os instrumentos
em oito lanes: bumbo, caixa, chimbal fechado, chimbal aberto, toms, pratos,
percussão de mão e outros. A saída já usa `RhythmicCell`, portanto foi projetada
para permanecer editável no Study em vez de criar um sequenciador paralelo.
Há limites explícitos de arquivo, tracks e eventos e nenhum parsing pertence ao
callback de áudio. Em 05/11/2026, depois da limpeza segura do cache APT elevar o
espaço livre de `/` acima do piso operacional de 5 GiB, o núcleo, o teste e o
aplicativo Release foram compilados em série. O teste focado
`batruc_midi_rhythm_import` passou. A interface JUCE expõe **Importar MIDI**
no Study: seleciona um arquivo local, mostra prévia e só aplica a célula após
confirmação. O resumo após importar conta somente lanes com ataques — as oito
continuam habilitadas internamente para impedir eventos gerados em lanes vazias
— e o tooltip informa o compasso e se o BPM do arquivo foi aplicado ou mantido.
O app não baixa o GMD automaticamente. A página oficial oferece
o pacote MIDI-only de 3,11 MB, sem os WAVs do pacote completo de 4,76 GB; o
download e a atribuição CC BY 4.0 ficam sob decisão do usuário. Nenhum arquivo
do corpus é baixado pelo aplicativo. Em 06/11/2026, o pacote MIDI-only oficial
foi guardado como fonte externa local em `midis/`, com o SHA-256 oficial
confirmado e extraído em `midis/groove/`; a cópia foi comparada arquivo a
arquivo com o ZIP e confere integralmente. O corpus não foi ligado ao binário
nem autorizado automaticamente para publicação. A política local e os limites de outros
materiais examinados estão em [midis/README.md](midis/README.md).

Desde 08/11/2026, aplicar seed com MIDI preserva as células importadas como
fonte imutável e produz variação determinística de acentos, escalada pela
profundidade da receita (fator até 0,52) e ponderada pelo papel de cada lane.
Desde 11/11, cada nota GM simultânea no mesmo ataque também recebe uma
variação determinística de velocity independente, sem alterar sua nota, posição
ou CC4. Com variação zero, as velocities importadas permanecem intactas.
Em 11/11, o app passou também a derivar frases MIDI com densidade por papel de
lane: em material de apoio denso pode retirar até 10–24% dos ataques fracos,
com a taxa limitada por instrumento; bumbo, caixa, downbeats e ataques fortes
permanecem (as velocities podem variar). Onsets com notas simultâneas e CC4
são mantidos como um grupo. Essa redução é determinística por seed e variação, e nunca modifica
as células-fonte. A seed também pode acrescentar pickups discretos entre ataques
de chimbal fechado, percussão de mão ou acessórios já presentes. Ela pode
inserir até dois pickups de tom discretos no último compasso quando há toms na
fonte, e até dois pulsos fracos entre ataques consecutivos de ride GM 51/59. O
ride não é criado a partir de crashes nem de campanas. Os gestos reutilizam
notas GM locais; bumbo e caixa permanecem intactos, enquanto ataques de apoio
podem ser rarefeitos dentro dos limites descritos. O número e a posição dependem
da seed e do nível de variação. A resposta
musical ainda precisa ser ouvida em diferentes ritmos e estilos. Lanes vazias permanecem habilitadas para não deixar eventos
gerados por fórmula aparecerem por baixo do arranjo MIDI. A cena guarda nome,
caminho, janela em compassos, SHA-256 do arquivo, licença e atribuição declaradas e as
oito células-fonte, versão da fonte declarada pelo usuário e até 64 entradas de
histórico (schema 47). Schema 48 preserva também a nota GM de cada ataque MIDI
nas células editáveis e na fonte imutável; a nota segue com o evento ao renderer
e permite escolher articulações sintetizadas, mantendo compatibilidade com
cenas schema 1–47. O histórico identifica importação, seed, edição por voz e
desfazer/refazer, mas não é uma ficha completa dos parâmetros de cada edição.
O app conserva os direitos como não verificados;
os textos são declarações do usuário, não uma validação jurídica. Ao reabrir,
outra seed pode recalcular a variação sem reler nem alterar o arquivo original.
No seletor **Ritmos / MIDI**, **Remover MIDI** desvincula a fonte da cena e
reconstrói o ritmo da seed atual; o arquivo no disco não é apagado. A próxima
seed passa então a compor sem MIDI. O schema 49 guarda também a seed independente
do baixo e seu nível de variação (0–3); cenas anteriores continuam legíveis.
Em 11/11, o agendamento do baixo passou a combinar essa seed independente com
a seed principal da composição, para que a nova composição também varie a linha
sem apagar o valor próprio do baixo. A mudança compilou em Release, mas ainda
aguarda escuta comparativa.
Em 11/11, a escuta do autor apontou que mudar a seed dedicada do baixo não
produzia alteração perceptível. A conversão anterior podia arredondar ou limitar
seeds diferentes às mesmas notas. O gerador agora escolhe, a partir da seed
combinada, uma transposição limitada para a frase inteira e mantém
contorno, pausas e variações por evento. É uma mudança no gerador do canal de
baixo. Nos níveis de variação 1–3, cada ciclo formal também passa a propor ao
menos um contratempo em uma subdivisão livre próxima de um ataque-fonte, quando
há espaço rítmico; a seed escolhe a posição. Os ataques existentes e os pulsos
fortes da percussão ficam preservados. Ainda requer escuta comparativa após
recompilar.
Em 11/11, foi acrescentada uma resposta curta e de baixa intensidade num
contratempo livre do primeiro compasso, também escolhida pela seed combinada.
Isso altera desde o início a frase que antes começava seguindo os mesmos
ataques da percussão. Nível 0 mantém o comportamento sem essa resposta; o
resultado sonoro ainda precisa ser confirmado por escuta.
Também removi o papel de pausa que surgia periodicamente após sete ciclos de
forma. As passagens longas continuam variando densidade e balanço, mas não
silenciam um ciclo inteiro; a célula suavizada em A' também não zera mais os
ataques.
O Study agora oferece microtiming por célula, redução de densidade,
interlocking com outra célula MIDI e contornos de intensidade, todos com
preservação dos ataques simultâneos e undo/redo. Recombinação entre células,
frases ou compassos e o manifesto completo de licença/atribuição ainda não
estão implementados.

Na janela JUCE a partir da largura mínima de 1080 px, o gráfico de ondas e a
geometria/percursos aparecem lado a lado. O gráfico de ondas usa 40% da largura
da janela, e a coluna de geometria recebe o espaço restante; com o campo de
padrão ativo, o percurso recebe 74% da coluna geométrica. Os controles desse
campo foram distribuídos em duas linhas com limites calculados para não invadir
os vizinhos ao redimensionar. A coluna geométrica oferece seleção de curva,
estudos Orbit/Study e pêndulo, vínculo opcional `V1 ← y(t)` e alternância entre percurso e campo de
fases. Triângulo e quadrado permitem arrastar vértices e deslocar as formas no
próprio gráfico. Fourier, bilhar e Lissajous expõem seus controles conforme a
curva selecionada. A opção **Áreas** troca o percurso pela relação de Stengel
dentro da mesma coluna: knobs `x`, `y` e `z` e o gráfico interativo atualizam
`w`, `S` e o resíduo. O estudo continua visual e didático, sem efeito sonoro
nesta versão; as barras representam magnitudes, não a figura espacial das
cevianas. Assim, Stengel pertence ao primeiro campo de Gráficos e Cálculos e
deixa de ocupar uma seção separada após a linha de forma. Mixer e Baixo ficam
nas seções roláveis seguintes. A interface solicita atualização de animação a 60 Hz;
tarefas de manutenção são limitadas a 20 Hz. Isso não garante 60 quadros por
segundo, pois a GUI depende da thread da interface e do compositor. `CPU AUDIO`
mede o callback de áudio, não o custo de pintura, GPU ou uso total do sistema.
O conteúdo rolável termina no fim do módulo do Baixo, com espaço reservado para
o rodapé fixo; não há uma cauda vazia de altura fixa depois dele.

Cada gráfico animado recebe um título próprio na tela JUCE: vozes · onda/fluxo,
percurso geométrico, Orbit/Study, campo de padrão, pêndulo, energia, LFO e
percurso do baixo, e piano roll. O título identifica a vista; não significa que
todo estudo gráfico gere ou module áudio. A diferença entre cálculos já
composicionais e vistas ainda didáticas está registrada em
[TAREFAS.md](TAREFAS.md) e no [Atlas RASGO](../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md).

No modo `Orbit`, o ponto móvel de cada voz também é uma alça: arrastá-lo muda
a fase da fórmula daquela voz e atualiza o controle `φ fórmula` correspondente;
`Shift` + arraste vertical sobre o ponto altera `ciclos/forma` em passos de
0,25 e atualiza o controle da voz. A fase desloca a curva de acentuação sem
reposicionar ataques; mudanças de ciclo atualizam a sequência no próximo bloco
de áudio. `Study` mostra os ataques como vértices de polígonos sobre o
ciclo comum. Arrastar um anel altera a fase rítmica da voz pelo controle
`Δφ ataque`; os ataques são recalculados pela agenda usual e, durante a
reprodução, a mudança entra no próximo bloco de áudio. Esse gesto
desloca a figura inteira da voz enquanto ela usa a regra por fórmula. Um clique
curto no anel ou numa fatia vazia insere um ataque e habilita uma célula manual;
movimento acima de 5 px no anel passa a ser interpretado como arraste de fase.
Arrastar um vértice muda somente seu ataque; `Shift` + arraste radial muda sua
força; botão direito ou duplo clique no vértice remove; `Alt` + clique no anel
restaura a regra por fórmula.
Cada célula contém até 64 ataques ordenados por seção, com distância mínima
entre vizinhos. O diâmetro do vértice indica força. A agenda audível atualiza no
próximo bloco, sem alocação no callback; o gráfico Onda/Fluxo e a cena, ao ser
salva, recebem as mesmas posições. O campo geométrico pode atenuar os ataques no
momento da execução. Uma nova seed limpa as células Study editadas manualmente;
quando há MIDI importado, recompõe os ataques desde as células-fonte preservadas
e aplica a variação determinística da seed.
À direita do polígono, **Células · 16** mostra os ataques das vozes na mesma
janela temporal. Clicar numa divisão liga ou desliga o ataque daquela voz:
na primeira edição de uma voz por fórmula, seus ataques correntes são
convertidos em célula manual. A grade oferece passos quantizados; o arraste
dos vértices continua disponível para posições livres entre esses passos.

Na curva de Lissajous, as alças `Ax`, `Ay` e `φ` também são arrastáveis e
atualizam os controles correspondentes e o traço. Para que a ordenada da curva
influencie a composição sonora, ative `V1 ← y(t)`.

O baixo tem modos de relação com as vozes: independente, seguir ataques e
responder após a voz. No painel **Baixo**, a caixa hexadecimal e **Novo baixo**
controlam uma seed própria. Ela escolhe alturas e respiros sobre os ataques
recebidos da percussão sem alterar o MIDI nem a seed da composição. O seletor
de nível vai de **0 · Âncoras** a **3 · Ampla**: 0 não acrescenta respiros
probabilísticos aos ataques admitidos (pausas da forma/frase ainda valem);
níveis maiores aumentam desvios de altura e a chance de respiros, mantendo os
apoios de pulso da V1. A variação de altura manual continua sendo outro
controle. Nos níveis 1–3, o gerador aplica microtiming determinístico aos
ataques não ancorados e compõe ao menos um contratempo em subdivisão vazia por
ciclo formal quando há um ataque-fonte próximo; a seed escolhe a posição.
Nos níveis 2–3, também pode antecipar a pulsação seguinte em semicolcheia; cada
compasso limita esses novos ataques a 2 ou 3, respectivamente. O nível 1 fica
em até um contratempo por compasso. Níveis maiores aumentam a probabilidade de
outras respostas. Os apoios fortes da V1 ficam fixos. Assim a
seed pode criar novas posições rítmicas, e não apenas atrasar as notas que já
existiam. Em níveis 1–3, cada ciclo formal escolhe ao menos um contratempo
quando há subdivisão livre perto de um ataque-fonte; níveis maiores ampliam as
outras respostas e variações. O Mixer tem uma faixa própria do baixo com nível,
pan, ativação, mute, solo e medidor, ligada aos mesmos parâmetros da página
Baixo. A linha gerada é substituída quando o músico edita o piano roll;
**Novo baixo** volta à linha calculada. Essas regras ainda precisam de
conferência auditiva por ritmo; não há arranjos de baixo específicos para cada
estilo do catálogo. A paridade completa com o HTML continua na fila de
[TAREFAS.md](TAREFAS.md).

No motor JUCE, as vozes matemáticas podem usar 18 matérias virtuais: seis
harmônicas/modais (seno, triângulo, dente de serra, quadrada, sino e membrana),
onze percussivas (bumbo, caixa, chimbal fechado, chimbal aberto, aro da caixa,
prato, conga, escovas, chocalho, agogô e bloco de madeira) e a matéria digital
Chip 8-bit. A paleta percussiva usa perfis modais, inarmonicidade, excitação de
ruído e uma queda de frequência no bumbo; cada voz pode herdar o
seletor global ou receber um override no Mixer. São modelos sintetizados e
estilizados, não gravações nem reproduções medidas dos instrumentos. O ritmo
continua vindo dos cálculos do Batruc; ainda não há receitas de conjunto por
estilo. As doze matérias originais mantêm os índices persistidos desde o
schema 37; Chip 8-bit foi acrescentado em 06/11, o chimbal aberto e o aro da
caixa em 11/11,
sem renumerá-las. Os controles próprios do Chip persistem nas cenas a partir
do schema 41. Escuta comparativa, variação de dinâmica e resposta mono/estéreo
continuam pendentes.

O protótipo tem um segundo painel de desenho: no modo de funções, mostra
trajetórias de compasso, esquadros, espiral e Lissajous; no modo pendular, mostra
o plano de fase `θ₁ × θ₂`, colorido pelos grupos da forma em sete ciclos. O
JUCE acompanha o traço pelo transporte e desenha os vetores giratórios nos
estudos de Fourier. Opcionalmente, no modo de funções, o eixo Y do desenho pode
alimentar a primeira voz. A interface enquadra essa pesquisa como Math Art,
Sound Design e sonificação matemática.

No painel de geometria da GUI JUCE, **Estudo físico · pêndulo triplo** alterna
para uma visualização do solver RK4: três massas ligadas por molas efetivas,
rastros, ângulos e curvas de energia local. A seed gera parâmetros e condições
iniciais reproduzíveis; o movimento e o cursor do gráfico acompanham o
transporte. É um estudo visual do modelo, separado da fonte de áudio ao vivo:
não representa o render offline tocado nem muda a composição atual. O tooltip
e o rodapé do gráfico informam esse limite.

Os gráficos de execução também mostram o valor de cada voz no cursor atual.
No campo XY, a trajetória acompanha o arco formal de fase; o cursor acrescenta
uma cauda atenuada, vetor de velocidade e projeções nos eixos para tornar
legíveis posição e direção do movimento. Esses recursos são visuais e não
alteram a geração sonora.

O JUCE executa a forma de sete ciclos sem inserir um ciclo de pausa automática.
A textura cresce por entradas progressivas, C5 é Expansão e C6 reduz parte das
vozes antes do retorno completo em C7; o arco de fase percorre o som, a curva e
o ponteiro das órbitas. A sequência pode repetir em performance ou tocar uma
passada finita; a maquete web permanece finita. O modo de reprodução longa
organiza oito passagens dessa forma sobre a mesma seed: A, B, A′ e C abrem a
forma; a segunda metade contém B′, uma passagem de Expansão, C ou C′ e o retorno
A, em ordem definida pela seed. Todas as passagens mantêm eventos ativos. A
camada de passagem não insere silêncio automático.
Cada papel modifica a articulação comum às vozes e ao baixo; o calendário e a
identidade da seed ficam preservados. A seed e a memória de passagem determinam
as escolhas internas. O status mostra a passagem e seu papel. Ainda não existe
editor manual de trechos: a ordem é gerada, não escolhida em uma lista, e cada
passagem longa contém os sete ciclos formais (28 compassos em 4/4); as oito
passagens somam 224 compassos antes de a sequência retornar. O banco de cenas
salva estados inteiros, não arranjos de trechos.

No canal de baixo, o controle `RESPIROS` encurta a duração do gate em até 30%,
mas conserva todos os ataques calculados. Pausas locais continuam disponíveis
somente ao escolher explicitamente um modo de frase que as prevê.

O baixo seeded nativo agora recalcula os eventos vocais em cada ciclo formal e
acompanha o arco de fase da forma. A fase-base individual das vozes usa agora a
mesma distribuição escalonada e o mesmo desvio determinístico por seed da web;
cenas nativas schema 23 migram bases antigas sem perder sua leitura. O
mapeamento determinístico seed→receita agora é comum: ciclo/fase de V2, regra
de eventos, campo espacial, contagem de vozes e lei/profundidade de intensidade;
a variação compartilhada cobre ciclos, acoplamentos, fases e transformações.
Em cada nova volta completa de sete ciclos, a memória formal também desloca o
registro do baixo em um ou dois semitons, invertendo a direção quando a
passagem o determina. A primeira volta mantém as alturas da seed. A mudança é
aplicada aos eventos tocados e não impõe uma escala nem altera o material
escrito no editor. As vozes ressonantes recebem deslocamentos próprios de
registro a cada passagem, com a mesma memória e transição de frequência
suavizada; a afinação de cada voz conserva as proporções de seus modos.
As agendas regulares da web resolvem cruzamentos e extremos analiticamente como
o core JUCE. Fixtures de cinco seeds, variação e uma agenda fracionária passaram;
execução e escuta A/B continuam pendentes. Os eventos geométricos de V1 e a
nuvem do mapa XY ainda usam a agenda-base; o cursor XY acompanha a fase de V2.
A comparação visual e auditiva continua pendente na
[matriz HTML/JUCE](PARIDADE_HTML_JUCE.md).

O estudo web e o painel JUCE incluem reconstrução de triângulo e quadrado por
séries de Fourier truncadas, com ordem `H` ajustável, curva de referência,
aproximação, cadeia de círculos/vetores e erro RMS relativo XY. No JUCE, o
traçado acompanha o transporte e os epiciclos usam buffer fixo. O `y`
reconstruído pode alimentar V1. A comparação visual e auditiva entre as saídas
continua pendente; método e métrica estão no
[registro de continuidade](TAREFAS.md#registro--25102026-primeiro-instrumento-de-reconstrução-de-fourier).

O mesmo painel oferece **Orbit** e **Study** para os ciclos musicais ativos.
Orbit desenha a fase de cada voz em seu anel e um rastro resultante da soma de
vetores; Study distribui os pulsos numa janela comum e informa o LCM limitado.
Ambos seguem transporte, ciclos e fases da composição. O arraste de fase e
ciclos em Orbit e a edição das células em Study alteram a composição. Razões
fracionárias e LCMs acima do limite são aproximados e identificados na legenda.
A referência
conceitual oficial de Orbit/Study/Riff está em
[BIBLIOGRAFIA_DE_ESTUDO.md](BIBLIOGRAFIA_DE_ESTUDO.md); o Batruc não reproduz
código nem identidade de outro produto.

O gráfico temporal JUCE também alterna entre o sinal composto de cada voz,
sua derivada numérica em relação ao tempo normalizado da seção e sua integral
numérica acumulada. Essas curvas incluem as transformações e os acoplamentos
visíveis no sinal; derivada e integral são normalizadas por voz. Cursor e
marcadores de ataque continuam no mesmo eixo temporal. Esta leitura é
diagnóstica: não muda os eventos nem o áudio. A derivada e a integral ainda não
foram promovidas a fontes composicionais selecionáveis.

Correção sobre seeds no baixo: antes desta atualização, a seed não alterava as
alturas das células. A maquete agora deriva, de modo reproduzível, novas alturas
para algumas células ativas, usando apenas as classes de nota já escritas e um
raio de até sete semitons. A nota-base permanece editável e visível; a altura
derivada aparece no editor. Pausas e demais atributos da célula são mantidos.
No JUCE, o controle de variação também foi ligado à seed com a mesma regra de
hash e seleção de alturas; 0% preserva a escrita e o editor indica a nota
calculada. Os piano-rolls mostram a altura efetiva como barra e trajetória; se
a seed alterar a nota, um conector e o rótulo `escrita>calculada` identificam a
origem. A correspondência exata e a audição comparativa ainda precisam de
conferência manual.

O painel nativo do baixo agora inclui um piano-roll gráfico de 150 px integrado
à receita: clicar em passo vazio cria nota, arrastar altera altura ou duração,
botão direito marca pausa e Shift+botão direito limpa a célula. O modo **Desenhar
traço** interpola alturas pelos passos cruzados, cria notas nos passos vazios e
preserva durações das notas já existentes. A grade
numerada permanece como alternativa para selecionar e alternar
nota/pausa/vazio. A duração exibida acompanha a lei selecionada; as barras são
limitadas pelo próximo evento e pelo fim do compasso. Durante a reprodução, um
playhead atravessa a barra exibida seguindo o transporte compartilhado; ao
pausar, fica congelado na posição alcançada. É uma referência visual do tempo,
sem interferir na geração do áudio. A interface JUCE compilou
em 26/10/2026, mas ainda falta conferir visualmente o painel e comparar a
edição, as pausas e as seeds com o HTML. Com Fibonacci ou duração geométrica
ativa, o seletor de duração por passo e o redimensionamento pela borda ficam
desativados nos dois destinos; a altura continua editável. Assim, o gesto não
indica uma mudança que a lei ativa ignoraria.

A primeira camada nativa da máquina de fórmulas permite selecionar leis de
intensidade — constante, projeção de `y`, produto `xy` e diferença `x²−y²` —
com mistura `λ` entre intensidade uniforme e valor calculado. Os ataques
continuam nos instantes preparados; a lei altera a força de excitação de cada
evento. O gráfico mostra essa diferença pelo tamanho dos marcadores. Seed e
cenas guardam lei e `λ`; fórmulas livres e transformações temporais ainda
precisam ser desenvolvidas.

O Mixer JUCE e a maquete web permitem as mesmas dez transformações de
intensidade por voz:
`sin(θ)`, inversão, retificação, combinação harmônica, modulação de amplitude,
quantização, `sin³(θ)`, terceiro harmônico, onda triangular e saturação suave.
As expressões aparecem na prévia; os quatro acréscimos são limitados a
`[−1,1]` e mudam a intensidade, sem recalcular a agenda de ataques. Cenas no
esquema 4 guardam os novos índices; esquemas 1–3 mantêm suas seis opções
anteriores. O esquema 5 também guarda ciclos e deslocamento rítmico por canal.
O gráfico nativo agora desenha a transformação ativa, sua fase e o acoplamento
com a voz anterior; quando o percurso geométrico alimenta V1, a curva também
parte desse percurso. O desenho representa a função normalizada, não o timbre
acústico do ressonador.
No Mixer, os oito stems ativos podem ter ciclos próprios (0,5–16 por seção) e uma fase
de ataque independente; em reprodução, os novos valores entram no próximo bloco
de áudio.
O renderer calcula os próximos cruzamentos em curso sem alocar memória nem
reconstruir a agenda no callback. A forma geométrica ainda pode filtrar eventos,
por isso “ciclos/forma” não equivale sempre à quantidade final de ataques.
O Mixer agora inclui uma máquina de relações polirrítmicas: presets `4:3`,
`5:4`, `6:4`, `7:4`, `8:6`, `10:4` e `10:7`, além de combinações de três,
quatro e cinco vozes (`2:3:5`, `3:4:5`, `4:5:7`, `3:4:5:7` e `3:4:5:7:8`).
Aplicar um preset coloca seus valores nos canais V1 em diante e ativa ao menos
as vozes necessárias; os demais canais conservam seus ajustes. A relação é de
ciclos dentro da seção de 16 tempos (quatro compassos), não uma contagem direta
de notas: cada ciclo parte de dois eventos antes das máscaras e regras de
ataque. Assim, `4:3`, `6:4` e `8:6` têm a mesma proporção reduzida, mas densidades
diferentes. Fases, regras Centro/Extremos, máscaras, mute e solo permitem
transformar e ouvir cada camada; o limite de composição ativa é oito canais.
O esquema 6 também salva a regra de ataque por voz: herdar a receita, cruzar o
centro ou usar pontos de inversão. A pesquisa rítmica distingue os dois livros
de Gramani, *Rítmica* (1988) e *Rítmica viva: a consciência musical do ritmo*
(1996), como referências de sensibilidade, assimetria, ostinatos, contraponto
e realização musical, sem transpor seus exercícios para o instrumento.

Na versão JUCE em desenvolvimento, “Aprender” e “Sobre” compartilham um painel
dentro da janela principal; nove tópicos aparecem em abas, sem abrir janelas
separadas. “Aprender” combina textos de referência com um explorador visual de
ritmos binários: escolha de 3 a 16 posições, navegação por número, edição das
células e sorteio de padrões. O preset alterna entre um compasso 4/4 de 16
semicolcheias e um compasso 3/4 com tercinas (nove posições ao longo de três
tempos); a prévia usa as durações correspondentes. A opção de três posições
também apresenta a tercina em uma batida: oito padrões de ataque/pausa. O texto
distingue os dois cálculos: no compasso 3/4 com tercinas, são nove posições e
512 padrões possíveis. O explorador
não altera a composição. O botão **Ouvir** toca uma prévia sintetizada em pulso
de metrônomo; **Praticar** oculta um padrão para ouvir e reconstruir nas células,
com conferência da resposta. **Percurso** propõe cinco etapas de 4, 6, 8, 12 e
16 posições; cada acerto libera a próxima e cada erro permite repetir a audição
e tentar novamente. Sequências didáticas ligadas a ritmos e contextos musicais
começam com uma trilha de escuta: confira a origem, o compasso e o BPM do MIDI;
ouça os stems em separado; identifique marcação, guia e resposta; altere uma
camada e compare. Os rótulos brasileiros do GMD descrevem performances de
bateria, não definem tradições. A pesquisa de Maracatu Nação e Ijexá/Afoxé e
seus limites estão em [RITMOS_E_CONTAGENS.md](RITMOS_E_CONTAGENS.md); amostras
GMD localizáveis estão em [midis/README.md](midis/README.md). Sequências
didáticas completas e demonstrações guiadas continuam pendentes. A resposta da
interface e a audição ainda precisam de validação manual.

## Modernização de 12–13/11/2026 — como usar

A auditoria musical de 12/11 ([AUDITORIA_MUSICAL_2026-11-12.md](AUDITORIA_MUSICAL_2026-11-12.md))
reordenou o instrumento: **a geração escreve ritmo editável; o que é escondido
vira efeito nomeado**. O que isso mudou na tela:

- **Modo RITMO / GERATIVO** (primeiro botão do cabeçalho):
  - RITMO toca exatamente o que está escrito;
  - GERATIVO acrescenta as máscaras, as leis e a forma; a profundidade é o
    controle **GER %** no rodapé (padrão 30 %, dinâmica leve, nada some).
- **Seeds** escrevem figuras em V1–V3 a partir do modelo físico (pêndulos
  acoplados), com forma de frase, ruptura e desenvolvimento nos 7 ciclos;
  sorteiam também o **tom** (12 tônicas × 10 modos) e o vocabulário da caixa.
- **Caixa:**
  - ornamentos (fantasmas, flam, drag, rufo, ricochete, rulo, aro) pelo menu
    RITMO do Study → Ornamentar;
  - cada ornamento fica **na nota** e aparece no Study como um ponto com
    sinal;
  - o botão **ORN** de cada stem do Mixer liga e desliga os recursos de
    caixa (desligado: a batida normal).
- **Notação:**
  - **pauta do baixo** na aba Baixo (clave de Fá, armadura do tom da seed,
    4 compassos), com clique direito → MusicXML da peça;
  - **pauta de bateria** no Study (botão ♩ PAUTA), com sinais de ornamento
    e manulação D/E, e clique direito → MusicXML.
- **Baixo:** o botão **FRASE** liga um baixo com voz própria (progressão de
  graus, apoio ao bumbo, respostas, notas de passagem). Fica desligado por
  padrão até a escuta do autor.
- **Render offline** (`batruc_offline_render`), para A/B:
  - `--figuras`, `--ritmo` e `--profundidade`;
  - `--baixo-frase`, `--afinar` (percussão no tom);
  - `--modelo pendulo|bilhar|lissajous|fourier|rolante|seed`;
  - `--caixa` (o vocabulário de caixa da seed, como no app).

Estudos de apoio:
- [ESTUDO_ORNAMENTOS_CAIXA.md](ESTUDO_ORNAMENTOS_CAIXA.md): rudimentos PAS,
  caixa no samba, notação;
- código de partitura portado com crédito do ANTITOTEM
  (`writeMusicXmlCaptureToFile`) e do MARAVI (`musicxml_writer.py`,
  `score_preview.py`).

Continuidade e pendências: [TAREFAS.md](TAREFAS.md).

## Identidade

O centro do instrumento é compor com conceitos matemáticos e físicos: tornar
fórmulas, teorias, axiomas, problemas e movimentos em regras audíveis de forma,
ritmo, alturas, timbre, dinâmica e relação entre vozes. Gráficos animados
mostram o cálculo e seu percurso no tempo; seeds e arquivos locais preservam
realizações que mereçam ser retomadas. O diretório de arquivo apoia esse
trabalho composicional, não o substitui.

Batruc é um instrumento musical sobretudo de ritmo, voltado à composição
e à performance generativa por construção geométrica. O desenho técnico
permite investigar divisões de tempo, polirritmias, proporções, trajetórias,
fases, encontros e transformações entre vozes.

### Marca gráfica

Proposta de logo escolhida pelo autor em 29/10/2026: símbolo de pulso coral,
arcos de ressonância/órbita e pontos de evento, acompanhado do lettering
geométrico “BATRUC”. A referência quadrada aprovada, sem transparência nem
degradê, está em
[`prototype/assets/brand/references/batruc-logo-selected-01.png`](prototype/assets/brand/references/batruc-logo-selected-01.png).
O lettering foi recortado dessa referência sem redesenho e é uma imagem raster,
não uma fonte tipográfica. Por decisão do autor, o instrumento web exibe apenas
esse lettering recortado em
[`prototype/assets/brand/active/batruc-wordmark-original-crop-v01.png`](prototype/assets/brand/active/batruc-wordmark-original-crop-v01.png).
Os símbolos, lockups e propostas anteriores foram preservados e organizados
sob `prototype/assets/brand/`: `references/` guarda originais e entradas,
`active/` o lettering usado no site, `studies/` os símbolos vetoriais,
`archive/` as exportações raster históricas e `proposals/` estudos de
alternativas. O mesmo lettering agora é incorporado ao executável JUCE via
`BinaryData`, substituindo o título digitado com a fonte do sistema; assim a
marca não depende de um arquivo externo na instalação. Ícones próprios da
aplicação JUCE ainda não foram configurados. O inventário e as regras de localização estão em
[`prototype/assets/brand/README.md`](prototype/assets/brand/README.md).
O favicon e os ícones web (Apple touch e PWA) são derivados do B do lettering,
sem novo símbolo; o cabeçalho já os declara. Ícones da aplicação JUCE nativa
seguem pendentes.

O nome histórico **Trucmetry** reunia a ideia de construção e medida do tempo;
Batruc dá nome ao instrumento atual e à sua identidade própria. Não são dois
projetos nem duas versões ativas.

Compasso de desenho, régua e esquadros são ferramentas de criação propostas,
não controles já implementados. Seno, cosseno, tangente, ângulos, comprimentos
e áreas são materiais de investigação; seu significado musical depende de
mapeamentos explícitos, não de uma equivalência natural entre medida e som.

O desenho deve participar da composição: dividir, girar, refazer ou liberar
uma restrição pode transformar uma relação rítmica. A contribuição própria
será investigada pelo gesto de construção, suas consequências e sua memória;
geometria sonora, sequenciadores gráficos e polirritmia têm antecedentes.
Não há declaração de novidade absoluta nem intenção de clonar uma referência.

### Campo geométrico de pesquisa

A forma circular é uma porta de entrada visual, não o limite do instrumento.
A pesquisa pode atravessar construções planas e espaciais: polígonos e
poliedros, curvas paramétricas, espirais, mosaicos, formas fractais, superfícies
e sólidos gerados por rotação, extrusão ou outras regras explícitas. As
operações — girar, deformar, combinar, intersectar, projetar e cortar — devem
ser manipuláveis e deixar visível o que foi preservado ou transformado.

Cortes de volumes podem revelar secções ao longo de um eixo ou de um plano
móvel. O instrumento pode acompanhar grandezas calculáveis como área da secção,
perímetro, centroide, simetria, interseções e variação do volume; essas medidas
podem gerar candidatos a eventos, densidade, intensidade, articulação, timbre
ou forma. Não há mapeamento natural entre uma grandeza geométrica e um
parâmetro musical: cada receita deve declarar a regra usada, mostrar seus
limites e permitir avaliar a consequência na escuta. Distinguir a geometria
calculada de uma analogia composicional e não apresentar uma visualização
espacial como simulação física sem um modelo físico definido.

Trigonometria, geometria e outros ramos da matemática entram no instrumento
tanto para efetuar cálculos como para compor música generativamente. Podem
calcular ângulos, distâncias, fases, posições, proporções, decomposição de
forças, oscilações e trajetórias dentro de um modelo físico definido. Seus
resultados podem então organizar ou recalcular eventos, células, ritmos,
alturas, timbres ou forma. Exibir uma fórmula ou gráfico, por si só, não basta:
cada mecanismo deve permitir ao músico inspecionar e escolher a relação,
entender os limites do cálculo, ouvir o resultado, compará-lo com bypass e
decidir se contribui à composição. O cálculo físico descreve o modelo adotado;
seu uso musical é uma decisão composicional, sem correspondência universal
presumida entre fenômeno e gesto musical.

#### Frentes de cálculo que entram na composição

O instrumento organiza cada frente como **modelo → medida → destino musical
→ profundidade/limites → comparação**. Álgebra e aritmética modular descrevem
recombinações, razões e ciclos; trigonometria e geometria descrevem fases,
curvas, colisões, áreas e transformações; álgebra linear organiza vetores,
matrizes de transformação e acoplamentos entre vozes; cálculo diferencial e
integral descreve velocidade, curvatura, acumulação e memória; estatística e
probabilidade descrevem distribuição de eventos, correlação, regressão e
escolhas condicionadas. Na física, os modelos de movimento e oscilação podem
alimentar ritmo e dinâmica; acústica, filtros, envelopes e ressonadores devem
alterar o som produzido. Na composição, tudo isso precisa chegar a células,
frases, forma, pausas, acentos, alturas ou timbres por uma regra exposta.

Essas frentes não devem virar painéis isolados. Uma trajetória pode fornecer
posição e eventos; sua velocidade, aceleração e curvatura podem alimentar
articulação, intensidade ou duração; uma matriz pode transformar essas medidas
entre vozes; probabilidade pode decidir entre operações previamente declaradas;
e o resultado segue pelo baixo e pelos canais do mixer. Cada destino deve ser
selecionável e limitado. O mixer continua responsável por nível, panorama,
mute/solo e master; os cálculos composicionais atuam antes dele, salvo uma
opção explícita de mixagem adaptativa. Assim o desenho, a partitura, a animação
e o áudio podem compartilhar a mesma fonte de cálculo sem confundir análise
visual com efeito sonoro.

**Primeira integração estatística no baixo:** para cada ciclo formal, os eventos
das vozes fornecem pares normalizados `(x,y)`. O core calcula a correlação de
Pearson `ρxy = cov(x,y)/√(var(x)var(y))` e a inclinação da regressão linear de
`y` sobre `x`, comprimida por `tanh` para permanecer em `[-1,1]`. Se alguma
variância for nula, o respectivo descritor vale zero. Esses dois descritores
entram com pesos limitados no alvo que escolhe o próximo deslocamento de altura
do baixo; portanto descrevem a relação espacial dos eventos e podem mudar a
linha calculada, sem impor escala ou progressão de acordes. O gráfico do
percurso mostra `ρ` e a inclinação comprimida `β̃`; o manifesto e a partitura
registram ambos. Correlação e regressão são descritores da associação dos
eventos, não prova de causalidade. A calibração musical e a comparação
auditiva desta influência ainda estão pendentes.

O mesmo percurso entre `V1(t)` e `V2(t)` agora fornece medidas cinemáticas por
diferenças finitas centrais, com `h=1/2048`: `ṽ=tanh(||r′(t)||/8,8858)` e
`κ̃=tanh(κ(t)/4)`, onde
`κ=(x′y″−y′x″)/(x′²+y′²)^(3/2)`. São medidas normalizadas da trajetória
musical, não velocidade em m/s nem curvatura de um objeto físico. `ṽ` altera
em até ±15% as durações geradas pelas leis Fibonacci/geométrica; durações
manuais permanecem intactas. `|κ̃|` pode acrescentar até 6% ao acento do baixo.
O gráfico e a partitura registram `ṽ` e `κ̃`; o tooltip explica as fórmulas.
Assim, cálculo diferencial passa a afetar articulação/duração e dinâmica antes
do mixer. A escuta ainda precisa confirmar se os pesos são musicalmente úteis.

Frentes seguintes: (1) usar características próprias das roletas — cuspidez,
fechamento, razão entre raios e posição do traçador — na escolha de eventos;
(2) álgebra linear para matrizes de transformação e acoplamento entre canais;
(3) distribuição estatística/probabilidade com seed e limites para variação de
células; (4) acústica e modelos de materiais ligados de forma audível ao VCF,
ADSR, LFO e ressonadores; (5) análise de forma e memória entre frases e ciclos.
Cada etapa deve compartilhar os mesmos dados entre desenho, score e renderer,
e manter um bypass reproduzível.

O percurso de investigação será incremental: começar por uma construção
bidimensional que explique a relação matemática; depois investigar projeções,
secções e sólidos quando a nova dimensão acrescentar uma decisão composicional
audível. A interface radial pode permanecer junto a gráficos cartesianos,
projeções e vistas de secção, cada uma adequada ao objeto e à operação em foco.
O instrumento não precisa forçar todo conceito a caber num círculo.

#### Triângulo: medidas, partições e trajetórias

O triângulo oferece uma primeira família de cálculos geométricos verificáveis.
Pitágoras relaciona os lados de um triângulo retângulo (`a²+b²=c²`); Herón
calcula sua área a partir dos três lados (`A=√(s(s−a)(s−b)(s−c))`). A área
calculada pelas coordenadas e a área de Herón também permitem conferir duas
rotas de cálculo independentes. [Pitágoras](https://mathworld.wolfram.com/PythagoreanTheorem.html),
[Herón](https://mathworld.wolfram.com/HeronsFormula.html).

Stewart relaciona os lados e uma ceviana. Com `a=BC`, `b=CA`, `c=AB` e `D` a
uma fração `t` do caminho `B→C`, `AD²=(1−t)c²+t b²−t(1−t)a²`. O caso `t=1/2`
é a fórmula da mediana de Apolônio: `mₐ²=(2b²+2c²−a²)/4`. Isso permite
transformar o deslocamento do ponto de divisão em comprimento de trajetória,
ou usar esse comprimento para controlar duração, energia, densidade ou timbre.
O mapeamento musical continua sendo uma escolha composicional. Os nomes variam:
Apolônio costuma nomear o caso da mediana, enquanto algumas fontes também
chamam a relação geral de Stewart de teorema de Apolônio.
[Stewart](https://mathworld.wolfram.com/StewartsTheorem.html),
[medianas](https://users.math.uoc.gr/~pamfilos/eGallery/problems/Stewart.html).

Ceva oferece uma condição de encontro para três cevianas. Se `tA=BD/BC`,
`tB=CE/CA` e `tC=AF/AB`, as três linhas concorrem quando
`[tA/(1−tA)]·[tB/(1−tB)]·[tC/(1−tC)]=1`. No Batruc, a condição pode orientar
convergência, bifurcação ou tensão entre trajetórias; cabe ao compositor
escolher se o encontro causa uníssono, acento, pausa ou mudança de camada. O
código calcula o resíduo logarítmico da condição, sem ainda ligá-lo ao áudio.
[Encyclopedia of Mathematics — Ceva](https://encyclopediaofmath.org/wiki/Ceva_theorem).

A reta de Euler contém circuncentro `O`, centroide `G` e ortocentro `H`; em
triângulos não equiláteros eles são colineares e `OH=3·OG`. No equilátero os
três centros coincidem, então a direção da reta fica indefinida. A posição dos
centros pode mover pontos de controle, modular fase ou aproximar/afastar
eventos. O núcleo calcula resíduos de alinhamento e proporção, sem tratar
“equilíbrio geométrico” como uma tradução musical natural.
[Euler line — MathWorld](https://mathworld.wolfram.com/EulerLine.html).

Na versão de áreas do teorema estendido das escadas cruzadas (Stengel),
`1/S + 1/z = 1/(x+z) + 1/(y+z)`, com `S=w+x+y+z`; `w,x,y,z` são as quatro
regiões da configuração de cevianas. Assim, dados `x,y,z`, pode-se calcular
`S=1/(1/(x+z)+1/(y+z)−1/z)` e então `w=S−x−y−z`, desde que todas as áreas
resultantes sejam positivas. O núcleo já calcula `w` e o resíduo relativo da
igualdade. O painel permite ajustar `x`, `y` e `z` pelos sliders compactos ou
arrastando suas barras; `w` é a saída calculada. Ainda não mostra a disposição
geométrica exata das regiões: as barras codificam valores numa escala comum e
não devem ser interpretadas como a configuração espacial das cevianas. O
teorema clássico de escadas cruzadas usa comprimentos recíprocos de segmentos
paralelos; é relacionado, mas distinto desta fórmula de áreas.
[Stengel: enunciado estendido e corolário de áreas](https://en.wikipedia.org/wiki/Crossed_ladders_problem#Extended_crossed_ladders_theorem),
[MathWorld: escadas cruzadas clássicas](https://mathworld.wolfram.com/CrossedLaddersTheorem.html),
[exemplo numérico da versão de áreas](https://math.stackexchange.com/questions/5128041/what-is-the-area-of-the-triangle-formed-by-three-cevians-when-three-other-areas).

O núcleo `src/core/TriangleGeometry.*` inicia essa base: valida triângulos não
degenerados; calcula área por coordenadas e Herón, lados, medianas de
Apolônio, compactação de área, desvio de Pitágoras, raios e centros da reta de
Euler; avalia comprimento de ceviana por Stewart, o resíduo de Ceva e resolve
a quarta região da relação de áreas estendida de Stengel. O painel JUCE
`Geometria` permite editar `x`, `y` e `z` tanto pelos sliders quanto por
manipulação direta do gráfico, recalcula `w` e apresenta `S` e o resíduo da
relação. Nenhum valor altera o áudio nesta etapa.

O mesmo painel também contém um modo `Curvas · Fourier`, portado da maquete:
compasso, triângulo, quadrado, espiral, Lissajous e aproximações de Fourier de
triângulo/quadrado. `H` varia de 1 a 32 e o erro RMS relativo é comparado a
512 amostras da referência geométrica. As funções puras estão em
`src/core/GeometryCurves.*`, sem dependência de JUCE. Nesta etapa, os percursos
são calculados e desenhados no painel. A opção de percurso usa `y(t)` para
calcular os ataques e acentos da V1. Em curvas periódicas, `Centro` agenda os
cruzamentos de `y=0` e `Extremos` agenda inversões locais; a coordenada `x(t)`
do evento também alimenta as máscaras geométricas e as leis de intensidade.
Forma e ordem Fourier geram tabelas limitadas na interface, publicadas sem
corrida e adotadas pelo áudio na próxima barra. Ciclos e fase da V1 deslocam a
agenda no renderer. A espiral conserva sua trajetória/acento, mas usa ataques
regulares porque seu raio muda ao longo da seção e não oferece uma tabela
periódica comum. Seleção, ordem e ativação continuam guardadas nas cenas JUCE
esquema 10 (esquemas anteriores continuam aceitos). Os candidatos vêm de
amostragem com resolução fixa; o arredondamento para amostras pode deslocar um
evento em até aproximadamente meia amostra.

Uma próxima família de pesquisa conecta combinatória, ondas e proporções:
diagonais rasas do triângulo de Pascal somam Fibonacci; linhas normalizadas de
Pascal podem servir como filtros FIR binomiais; e a série da onda triangular
usa harmônicos ímpares com amplitude proporcional a `1/n²` e sinais alternados.
Esses fatos sugerem experimentos independentes para acentuação, suavização e
síntese, mas não provam que um padrão Fibonacci soe orgânico ou que um filtro
binomial seja “perfeito”. A possível relação entre teoremas geométricos e
parâmetros musicais deve ser exposta como receita selecionável e comparável,
nunca como equivalência natural entre geometria e groove.

#### Modelagem animada por sobreposição de polígonos

No gráfico JUCE, os estudos de triângulo e quadrado podem ser manipulados como
duas formas convexas sobrepostas. A primeira conserva a geometria escolhida; a
segunda recebe de 3 a 8 vértices conforme a seed. Cada forma é transformada por
`p′ᵢ(t)=R(θᵢ(t))pᵢ+dᵢ(t)`, com
`θᵢ(t)=2π(cᵢt+φᵢ)` e `d₁(t)` conduzido pelo ciclo/fase de V2, enquanto `d₂(t)`
acompanha V1. A seed escolhe as fases iniciais de modo reproduzível. A
interseção convexa é calculada por recorte de polígonos; áreas são medidas pela
fórmula do cadarço. `H_area=−Σpᵣ ln(pᵣ)/ln(3)` mede a distribuição normalizada
entre `A\B`, `A∩B` e `B\A`; não é entropia termodinâmica nem uma propriedade do
som.

Arrastar vértices remodela a forma, sob a condição de preservar convexidade;
arrastar seu interior desloca a forma em relação à outra. Duplo clique
reconstrói os polígonos a partir da geometria e seed atuais. Rastros mostram a
translação; vetores mostram a orientação; o preenchimento evidencia a região
de interseção. O modelo é uma camada visual/analítica e ainda não recalcula os
eventos de áudio. Em cenas JUCE schema 37, vértices e translações manuais são
salvos e restaurados depois da seed/geometria; cenas 1–36 continuam aceitas. A
web ainda não tem editor equivalente. Próxima decisão composicional: definir
um mapeamento selecionável de interseções, área ou `H_area` para eventos,
preservando comparação com o percurso anterior; a reabertura funcional ainda
precisa ser conferida no app.

### Equilíbrio, gravidade e escultura

Equilíbrio, peso, apoio, braço de alavanca, suspensão e balanço podem ajudar a
compor relações de tensão, compensação e mudança. Em um modelo estático de
corpo rígido, equilíbrio exige resultante de forças e de torques nula; se a
resultante deixa de ser nula, o modelo dinâmico pode descrever aceleração e
movimento. A gravidade pode participar do cálculo de torque e da energia
potencial, desde que massa, posição, apoio e condições do modelo estejam
definidos. [OpenStax: equilíbrio estático](https://openstax.org/books/university-physics-volume-1/pages/12-1-conditions-for-static-equilibrium).

A escultura móvel oferece uma referência artística concreta: partes articuladas
e suspensas mudam de relação ao se mover, enquanto a composição pode depender
de equilíbrio, distribuição de formas e resposta ao ar. O trabalho de
Alexander Calder é apresentado pela National Gallery of Art em termos de
esculturas móveis e estáveis, equilíbrio por ensaio e transformação visual
produzida pelo movimento. [NGA: Calder’s Balancing Acts](https://www.nga.gov/educational-resources/counting-art/calders-balancing-acts).

No Batruc, possíveis receitas podem tratar vozes ou objetos como elementos
de uma construção: massa/valor atribuído, distância ao apoio, forças, torque e
deslocamento. Um equilíbrio pode gerar sustentação, distribuição ou repouso;
uma perturbação pode conduzir a rotação, queda, aceleração ou novo equilíbrio.
Como hipóteses composicionais, essas mudanças poderiam modular densidade,
acentos, duração, altura, timbre, suspensão e retorno. A pessoa deve poder
ver o apoio, os vetores, a contribuição de cada elemento e a soma que causa o
movimento, e ouvir separadamente cada relação escolhida.

Há duas vias que a interface deve nomear corretamente: (1) **simulação física**,
que calcula uma dinâmica especificada com parâmetros e unidades coerentes; e
(2) **analogia escultórico-composicional**, que usa ideias de peso, tensão ou
equilíbrio como regras musicais sem alegar simular um objeto físico. Um estado
musical de equilíbrio não precisa ser silêncio, assim como uma dissonância ou
instabilidade não é, por natureza, queda. Cada mapeamento deve ser explícito,
editável e avaliado pela escuta. A referência escultórica informa a pesquisa;
não implica copiar formas, composições ou identidade visual de uma obra.

### Matéria, modos e criação de timbres

O estudo escultórico pode avançar da forma para a matéria que vibra. Um modelo
de objeto sonoro deve relacionar geometria e dimensões, apoios/contornos,
densidade, rigidez elástica e perdas por amortecimento. O material não determina
sozinho o som: a forma e as condições de contorno alteram os modos; o modo como
o objeto é excitado e o ponto de contato alteram quais modos recebem energia.
Madeiras também podem ter propriedades direcionais e variar com a amostra e a
umidade. [Modalys: propriedades de objetos e materiais](https://support.ircam.fr/docs/Modalys/3.4.0/co/object_properties_general.html)
e [estudo sobre propriedades acústicas da madeira](https://pubmed.ncbi.nlm.nih.gov/22280703/).

Cada modo pode ser pensado como uma componente ressonante com frequência,
intensidade e decaimento próprios. Em alguns corpos, as frequências se alinham
aproximadamente a múltiplos de uma fundamental e formam uma série harmônica;
em barras, placas ou outros corpos, os modos podem ser inarmônicos. O
amortecimento pode variar com a frequência: se os modos agudos decaem mais
rápido, o timbre escurece durante a sustentação. Assim, propriedades materiais
podem afetar tanto o espectro inicial quanto sua mudança no tempo. [Modalys
documenta as perdas e os parâmetros de densidade, elasticidade e geometria](https://support.ircam.fr/docs/Modalys/3.4.0/co/object_properties_general.html).

Receitas de timbre podem permitir comparações controladas: mesma forma com
materiais diferentes; mesmo material em formas ou espessuras diferentes; mesmo
corpo percutido, friccionado ou excitado em pontos distintos; ou diferentes
curvas de amortecimento no mesmo espectro modal. A visualização pode revelar
modos e suas amplitudes, frequências, relações harmônicas e decaimentos; a
escuta testa quais diferenças são composicionalmente úteis. Síntese modal,
cordas/guias de onda, modelos de placas ou uma combinação híbrida são famílias
a avaliar, não engines adotadas.

Para materiais reais, identificar fonte e condições dos parâmetros; para
valores inventados ou abstraídos, nomear o resultado como material virtual ou
modelo estilizado, não como reprodução fiel de madeira, metal ou pedra. O
exciter (baqueta, dedo, arco, sopro, fricção), o local e a força de contato
também fazem parte da receita. Essa dimensão conecta os conceitos de escultura,
equilíbrio e gravidade à composição: uma perturbação pode mudar simultaneamente
movimento, energia de excitação e espectro, caso o modelo realmente calcule
essas relações. Caso contrário, declarar o vínculo como mapeamento composicional.

### Força, deformação, transferência e ruptura

Uma família de processos pode encadear ação e consequência: uma força desloca
um elemento; ligações transferem parte do movimento ou da energia; o material
deforma e armazena energia; ao atingir um limite, pode mudar de regime ou
romper. A sequência é uma possibilidade composicional e só é uma simulação
física quando cada elo segue equações, parâmetros e unidades declarados.

| Fenômeno | Relação física de referência | Possível decisão composicional |
| --- | --- | --- |
| Mola/elástico | `F=−kx`; `Eₑ=½kx²` no regime linear | deslocamento da fase, acúmulo de tensão e liberação como ataque |
| Massa, força e peso | `ΣF=ma`; perto da Terra `P=mg` | resultante controla aceleração/densidade; distinguir massa de peso |
| Amortecimento | `m ẍ+c ẋ+kx=F(t)` | energia perdida controla cauda, escurecimento ou duração |
| Alavanca | `τ=r×F`; braço e apoio determinam torque | proporções entre vozes, acentos e pontos de apoio da forma |
| Pressão/tensão | pressão `p=F/A`; tensão interna depende do estado de deformação | área de contato e esforço podem mudar densidade, timbre ou limiar |
| Deformação | `ε≈ΔL/L` em pequenas deformações | transformação gradual de trajetória, altura ou espectro |
| Empuxo | `Fᵦ=ρfluido g Vdeslocado` | sustentação, suspensão e oposição ao peso em modelo de corpo imerso |
| Magnetismo | força depende de carga/corrente/campo; `F=q(v×B)` é o caso magnético de uma carga em movimento | atração/repulsão entre elementos, aproximações, afastamentos e encontros |
| Radiação | vibração acopla-se ao meio e irradia energia; pressão EM ideal pode ser `I/c` (absorção) ou `2I/c` (reflexão) | projeção sonora, perda de energia, brilho ou mudança espacial, conforme o modelo |
| Ruptura | critério de falha depende do material e do estado de tensão | limiar dispara corte, pausa, ruído/transiente, mudança de matéria ou nova seção |

Pressão externa e tensão interna não são sinônimos; massa e peso também não.
“Radiação” deve dizer se significa emissão acústica de um corpo vibrante ou
radiação eletromagnética. Magnetismo precisa de carga, corrente, dipolo ou
campo definidos: uma atração genérica é uma regra abstrata, não uma simulação
magnética. Para elasticidade e ruptura, o limite depende do material e da
geometria; um limiar arbitrário deve ser chamado de modelo virtual.

No instrumento, o percurso pode ser uma máquina de estados com memória:
**ação → força resultante → movimento/deformação → energia armazenada ou perdida
→ limite → ruptura/liberação → consequência e novo equilíbrio**. Histerese e
um estado explícito de ruptura impedem que a condição oscile rapidamente junto
ao limiar. O cálculo deve limitar deslocamento, energia, passos de integração
e eventos; a ruptura precisa declarar para onde vai a energia armazenada
(transiente, cauda, dissipação ou silêncio). A analogia composicional pode
transformar essa cadeia em tensão, espera, impulso, pausa, rasgo e retorno,
sem alegar que a música reproduz literalmente a matéria.

Esta profundidade — fonte, derivação, unidades/hipóteses, validade, processo,
consequência composicional e validação — segue o método transversal de
[aprofundamento conceitual da família RASGO](../../RASGO_DOCUMENTATION/GOVERNANCA_E_TRANSVERSALIDADE.md#82-eixo-de-colagem-corte-e-acontecimento).

## Música, geração e experiência de tempo

O objetivo é música com desenvolvimento, não apenas uma demonstração de
polirritmia. Investigar ataques, articulações, acentos, pausas, motivos,
entradas, saídas, diálogo entre vozes e forma de longa duração. Timbre e
ressonância devem dar corpo à organização rítmica; melodia e harmonia podem
participar sem deslocar esse centro.

A geração pode propor e transformar construções, conservando memória e
possibilidade de intervenção humana. Não deve apagar silenciosamente escolhas
do músico nem se limitar a randomizadores independentes. Autonomia, edição,
interrupção e retorno devem coexistir.

Questões de tempo a explorar: medido/vivido, ciclo/irreversibilidade,
repetição/diferença, espera, suspensão, encontro e vestígio. São perguntas
artísticas, ainda sem bibliografia filosófica consolidada. Precisam produzir
consequências na escuta; nenhum termo filosófico certifica profundidade.

Diretriz do autor para toda a família RASGO: cada instrumento deve buscar
excelência de saída de áudio, aprofundamento e variedade de recursos, e valor
didático. No batruc, isso significa que a fórmula, o gráfico, o gesto e a
consequência sonora devem ensinar relações matemáticas, físicas e musicais com
clareza, sem virar uma demonstração superficial. A maquete HTML/Web Audio é
apenas um estudo de interação; seus sons simples não definem o padrão de áudio
do instrumento final.

Excelência também envolve precisão, estabilidade, segurança e legibilidade. A
experiência deve continuar musicalmente significativa sem olhar o desenho.
Cálculos e testes corretos não substituem escuta, uso e avaliação do autor.

O sequenciamento é uma camada de execução e edição, não o conceito inteiro do
Batruc. A hipótese central é uma máquina de composição generativa em que
conceitos matemáticos/físicos e suas transformações geram materiais, relações
entre vozes e forma. O sequenciador torna essas propostas tocáveis; não deve
reduzir o instrumento a uma grade de disparos. Esta distinção orienta a
pesquisa, mas ainda precisa ser concretizada e avaliada em composições.

### Superfícies de afinidade — primeira prova

A primeira prova inspirada pelos micropadrões de adesão desenha a trajetória
paramétrica das fases das duas vozes no plano XY. O músico escolhe entre campo
livre, quatro ilhas de contato ou duas microlinhas diagonais. Cada evento recebe
as coordenadas `(sin θ₁, sin θ₂)` no instante do ataque. A borda tem uma faixa
de afinidade de 0,055 unidade normalizada: a intensidade desce com uma curva
suave na transição e o evento é suprimido além dela. Assim, a superfície
espacial altera a seleção temporal e a dinâmica dos ataques. A interface
mostra a superfície, o percurso completo atenuado, o trecho já percorrido, os
ataques aceitos por voz e a posição atual.

As quatro ilhas começam com centros nos eixos `±0,58`; as microlinhas
partem de `y = ±0,55x` e largura normalizada `0,16`. Agora o campo muda de
posição e orientação ao longo da forma. Uma fase `θ = 2πrt + φ(seed)` conduz
translação limitada a `±0,12`, rotação de até `0,16 rad`, respiração da largura
das faixas e variação do raio das ilhas. Os eventos são avaliados no instante
do ataque contra o mesmo campo em movimento; a afinidade perto da borda muda
gradualmente a intensidade. Em **Campo livre**, a Lupa é um foco móvel: não bloqueia
eventos, mas pesa sua intensidade entre `0,68` e `1,20` conforme a distância ao
foco. Assim, a superfície aberta também atua no som sem virar máscara de
silêncio. Cada ilha agora pulsa e deriva pelo campo com fase própria derivada
da seed, recebe uma pequena contribuição da vizinha seguinte e orienta seu
contorno de forma independente. Isso produz quatro percursos próprios de
expansão, contração e deslocamento; o encontro da trajetória musical com cada
organismo pode mudar a densidade de ataques de maneira diferente ao longo do
ciclo. Quando ilhas vizinhas se aproximam, um conector temporário torna o
corredor entre elas parte do campo sonoro; ao se afastarem, a ligação some. As
microlinhas também deixam de ser um
par perfeitamente espelhado: cada faixa respira em largura e deriva alguns
centésimos de unidade, mantendo o envelope da deformação preso às extremidades.
Esse movimento relativo permite encontros e afastamentos entre as membranas e
altera quais ataques atravessam cada faixa.

O traçado do campo e a consulta no motor usam as mesmas funções puras; a seed
determina trajetória e velocidade da máscara, e a reprodução reinicia a mesma
órbita a cada nova forma. Mudanças afetam a geração dos eventos, não somente a
animação. O movimento é uma composição geométrica autoral, não uma simulação
biológica ou de micropatterning. O músico ainda escolhe entre famílias de
campo; dentro das ilhas, a seed escolhe um perfil diferente para cada uma.
Há 28 perfis polares inspirados por curva W, domo gaussiano, bruxa de
Agnesi, logaritmos naturais e de base 10, curva recíproca, softplus, GELU,
seno/cosseno/arcos seno e cosseno, seno
absoluto, parábola, batimentos entre harmônicos, ondulações, tangente limitada,
onda com fase modulada (chirp circular), cubo, simetria/reflexo, concavidade,
anamorfose angular, rebote amortecido, cosseno amortecido, oval de convexidade
suave e silhueta afilada apelidada “foguete”,
além da forma harmônica anterior. São adaptações fechadas
e limitadas para deformar uma borda radial, não cópias literais dos gráficos
cartesianos. O mesmo contorno decide quais eventos passam pela máscara e com
que peso; portanto, a forma não é apenas animação decorativa.

Micropatterning continua como eixo ativo desta pesquisa. A página da ibidi e
sua proveniência estão no [Atlas RASGO](../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md#micropadrões-de-adesão-como-referência-para-campos-composicionais--24-out-2026).
O próximo aprofundamento pode comparar máscaras binárias com campos graduais
de afinidade, alterar tamanho/espaçamento/forma das zonas e acompanhar como
uma trajetória atravessa, acumula tensão junto a uma fronteira ou muda de
regime. A adesão celular permanece o fenômeno de origem; forças, deformações,
rupturas e regras de evento do Batruc serão modelos composicionais próprios,
sem alegar simular células.

### Ecologia, percepção e organicidade das vozes

A investigação ecológica do Aquorbium é uma referência direta para o problema
biológico e orgânico que o autor quer aprofundar no Batruc. Consultar
[`ECOSYSTEM_DESIGN.md`](../AQUORBIUM/ECOSYSTEM_DESIGN.md) e
[`ECOLOGICAL_MUSIC_DESIGN.md`](../AQUORBIUM/ECOLOGICAL_MUSIC_DESIGN.md): ali,
organismos não respondem a um observador total, mas ao que percebem; desejo
altera tendências sem escrever eventos; indivíduos da mesma espécie mantêm
diferenças; papéis, memória, presença e mudanças do meio produzem consequências
coletivas.

No Batruc, essa pesquisa pode informar uma máquina de relações entre vozes
e materiais calculados: cada voz conserva estado e memória de frase; percebe
somente eventos acessíveis segundo regras explícitas; responde com atração,
afastamento, imitação, disputa, apoio, pausa ou mudança de densidade; e altera
seu comportamento gradualmente, sem perder sua identidade de fórmula, registro
e timbre. A forma deve emergir de relações audíveis — encontro, ocupação de um
nicho, fuga, persistência, escassez, ruptura e retorno — em vez de uma camada
aleatória de “humanização”. A intenção do músico define quais relações são
possíveis e quais têm prioridade.

Esta aproximação conversa com micropatterning, mas não o substitui: superfícies
de afinidade oferecem condições espaciais locais; a ecologia acrescenta
percepção, memória e resposta entre agentes ao longo do tempo.

O primeiro protótipo está no core em `src/core/EcologicalRelations.hpp/.cpp`.
`buildEcologicalEventPlan` deriva um plano bounded de outro plano, sem alterar
a fonte. Cada voz pode declarar uma voz-fonte, força e janela de memória, e
escolher entre apoio (reforça intensidade), imitação (aproxima o ataque do
atraso declarado), afastamento (desloca ataques próximos) ou competição
(reduz intensidade quando há proximidade). A cada evento, no máximo o ataque
anterior da fonte pode ser percebido; eventos simultâneos não reagem entre si.
O rastro opcional informa evento percebido, idade da memória e valores antes e
depois. Regras e ordem temporal são validadas, a quantidade de eventos é
limitada e a função é determinística, sem sorteio ou trabalho em tempo real.

O HTML oferece essas relações por voz e usa o plano transformado antes da
geração do baixo. O JUCE também oferece quatro modos ecológicos no seletor de
relação do baixo seedado. Ele prepara uma cópia transformada do plano fora do
callback e entrega a performance ao renderer no próximo compasso. As forças
vêm dos acoplamentos λ das vozes. O primeiro teste confirma que a performance
adiada não soa antes da fronteira e é aceita quando o compasso reinicia.

A integração JUCE ainda é global para a cadeia do baixo, não tem seletor por
voz, não transforma baixo manual e não mostra o rastro causal. Tampouco foi
feita comparação auditiva entre HTML e JUCE; os testes atuais validam cálculo,
geração e agendamento, não equivalência de timbre. Os documentos e o código do
Aquorbium permanecem fonte de referência do próprio instrumento; Batruc
deriva receitas novas, sem copiar implementação nem transferir sua identidade
aquática.

No protótipo web, a primeira integração já oferece relações de eventos por voz
(V2–V12): independente, apoio, imitação, afastamento e competição. A seleção
faz parte da composição local salva; cada voz responde à anterior, a força usa
o acoplamento λ, e memória/atraso inicial são ½ e ¹⁄₁₆ de ciclo. A
transformação ocorre antes da geração do baixo, que recebe também a intensidade
resultante. JUCE ainda não aplica essa receita, e a interface web ainda não
mostra o rastro causal por evento; a audição comparativa segue pendente.

### Créditos, ajuda e idiomas — primeira camada nativa

A janela JUCE inclui crédito de rodapé com autoria, RASGO e licença do
Batruc; “Sobre” informa versão, autoria/direção, licença do projeto,
licenciamento próprio do JUCE e o estado experimental do som. “Aprender” e
“Sobre” compartilham um painel na própria janela. “Aprender” oferece nove
tópicos — oito referências textuais e um explorador visual de padrões — ainda
sem constituir um curso completo. Os temas cobrem fórmula/fase, regras de
evento, superfície XY, espaço acústico, leis de intensidade, zoom, cenas,
contagens rítmicas e baixo/Mixer. A seleção oferece português,
inglês, espanhol e francês. A localização
espanhola é parcial e usa inglês como fallback para strings sem tradução; não
se deve considerar PT/EN/ES/FR como cobertura integral. O idioma vale durante
a sessão; persistência
entre aberturas, tradução integral de conteúdo futuro e revisão humana das
traduções ficam pendentes.

A janela nativa segue a linguagem visual da maquete web: papel `#ece9dd`, tinta
`#132a28`, terracota `#c65132`, verde-petróleo `#167d77`, papel de gráfico
`#f1f1e8`; títulos usam a família serifada Georgia e rótulos a monoespaçada
DejaVu Sans Mono. Os controles e traços mantêm bordas retas e contrastes da
página. As fontes não são empacotadas nesta prova; a substituição depende das
fontes disponíveis em cada sistema e ainda requer revisão visual multiplataforma.

### Gestos da primeira fatia

A arquitetura decidida para implementar esses gestos usa um único transporte
em amostras e uma cena preparada imutável por versão da receita. O recorte
inicial declara 120 BPM, 4/4 e quatro compassos por seção de oito segundos;
as razões 3:4 são oscilações nessa seção. Mudanças de regra, superfície e fase
entram no compasso seguinte; mudanças de razão, BPM ou tamanho da seção entram
no início da seção seguinte. O plano de eventos nasce da fórmula, passa pelo
filtro espacial e só depois recebe articulação e swing, preservando a distinção
entre instante matemático e ataque tocado. O protótipo nativo agora prepara
9.696 agendas para dezesseis quantidades inteiras de ciclos da segunda voz (1–16),
duas regras, três superfícies e 101 posições da fase `φ₂` (0–0,5 volta em
passos de 0,005), com um único renderizador e contador de
amostras. A escolha durante reprodução entra no próximo compasso; durante
pausa, preserva a posição e as caudas. Fórmula e gráficos mostram a receita
ativa até a troca, enquanto o estado indica a mudança pendente. Cenas ganharam
uma primeira memória de sessão: guardar/recuperar até 16 configurações de
regra, superfície, ciclos/fases, BPM, lei de intensidade e número de vozes. O
ciclo da voz 2 pode ser selecionado de 1 a 16, incluindo contagens pares e
ímpares. A seed hexadecimal mantém o mapeamento v1 para os ciclos antigos
{2, 3, 4, 6, 8}, preservando as realizações existentes; as contagens novas são
selecionáveis manualmente enquanto a seed não ganhar uma versão explícita. A
seed também escolhe a lei, `λ` e o número de vozes. Alterar controles manualmente apaga o rótulo de
seed. Cenas preservam seed e parâmetros durante esta sessão. O mapeamento v1
continua ligado ao banco fixo: não gera fórmulas livres nem receitas
arbitrárias, e sua equivalência entre versões ainda precisa ser versionada.
O host JUCE exporta e abre composições `.batruc.txt` versionadas, incluindo
receita, seed, parâmetros globais e 12 slots de canal no formato de cena
(compatibilidade de dados; o host ativa até oito stems). Os campos por canal
incluem nível, panorama, mute, solo, transformação e acoplamento. O arquivo é validado antes de aplicar
e a operação ocorre fora do callback de áudio. A captura/restauração das cenas
em memória também preserva esses controles. O menu Cenas permite escolher um
banco de pasta local e navegar até 64 arquivos válidos. A leitura não é
recursiva, examina no máximo 256 arquivos de até 64 KiB e ordena os válidos
encontrados por data. O caminho da pasta fica nas preferências locais do app;
se ela não existir mais, escolha outra. As preferências guardam somente o
caminho, enquanto cenas e áudios permanecem nos arquivos da pasta escolhida.
A exportação de cenas e a gravação sugerem esse diretório e não substituem
arquivos existentes. O esquema 8 preserva a leitura dos esquemas 1–7; cenas
1–6 carregam os controles do baixo nos valores padrão, enquanto cenas 7
preservam seus parâmetros de áudio.
O botão `REC` grava o master local como WAV PCM 24-bit e cria um TXT com a
cena inicial. Durante a captura, fica vermelho, passa a mostrar `STOP mm:ss` e
atualiza o tempo uma vez por segundo. O limite padrão é 5 minutos; a caixa ao
lado lista 1, 2, 3, 5, 10 ou 15 minutos, e a escolha fica nas preferências
locais. Ao atingir o limite, a captura para sozinha. WAV e TXT são fechados em
segundo plano; a conclusão normal aparece no status, sem interromper a sessão
com uma janela modal. A escrita usa fila limitada fora do callback e reporta
blocos perdidos; o fluxo ainda precisa de validação manual.
Fórmulas livres, expressividade e ritmos avançados continuam pendentes. O JUCE agora tem um
deslocamento de fase da fórmula por canal: ele altera a intensidade dos ataques
sem mover seus instantes. O contrato e a ordem de execução estão em
[TAREFAS.md](TAREFAS.md#ordem-de-execução--aproximação-progressiva-à-maquete-web).

### Contagens e ritmos avançados

O seletor de 1–16 descreve **voltas da segunda voz ao longo da seção atual**;
não é um seletor de compassos, subdivisões ou ciclos rítmicos tradicionais.
Ritmos avançados pedem outros modelos e deverão ser uma camada composicional
própria: distribuição euclidiana de `k` ataques em `n` pulsos; agrupamentos
aditivos e assimétricos (por exemplo, `2+3+2+3`); sieves modulares com
periodicidade declarada; e sequências de durações proporcionais, que podem
incluir valores de Fibonacci. Essas referências descrevem estruturas distintas
e não devem ser reduzidas a “mais opções de ciclos”. Próximo passo: desenhar
receitas editáveis para esses modelos, com contagem/pulso explícitos, acentos,
pausas, duração, limite de eventos e visualização por voz, antes de incorporá-las
ao motor. Referências: [Toussaint, *The Euclidean Algorithm Generates
Traditional Musical Rhythms*](https://archive.bridgesmathart.org/2005/bridges2005-47.html),
[Xenakis, Sieve Theory](https://www.iannis-xenakis.org/en/sieve-theory/) e
[Besada, estudo da teoria de sieves e *Psappha*](https://mtosmt.org/issues/mto.22.28.2/mto.22.28.2.besada.php).
O contrato dos geradores e a primeira especificação `E(k,n,r)` estão em
[RITMOS_E_CONTAGENS.md](RITMOS_E_CONTAGENS.md).

O host nativo seleciona de duas a oito vozes preparadas no mesmo transporte.
Vozes adicionais têm ciclos, fases, panoramas e perfis de ressonância virtual
próprios; a seleção por seed também determina quantas tocam. O painel Mixer
oferece nível, mute e solo por canal; o nível muda com uma rampa curta, eventos
silenciados não se acumulam e vozes recém-ativadas começam com estados limpos.
O panorama estéreo pode ser editado por canal. A matéria sonora global oferece
18 modelos sonoros: seis formas harmônicas/modais, onze percussivos (bumbo,
caixa, chimbal fechado, chimbal aberto, aro da caixa, ride, conga, escovas,
chocalho, agogô e bloco de madeira) e Chip 8-bit,
sintetizados por ressonância modal, ruído ou síntese quantizada conforme o modelo.
Não são gravações nem uma implementação de osciladores Web Audio. Cada canal
pode herdar o perfil global ou escolher uma exceção sonora própria; a cena
guarda essa escolha. Além do panorama-base, **Movimento** aplica aos perfis
percussivos `p = clamp(p₀ + m·s·x, −1, 1)`: `p₀` é o pan-base, `m` é a
profundidade por canal (0–1), `x` é a coordenada horizontal do evento e `s`
depende da intenção e da intensidade já moldada pela curva dinâmica. Construir
abre o movimento com a intensidade; rarefazer o contém; encontrar aproxima os
eventos do centro; contrastar alterna o lado dos ataques. Os ganhos usam pan
de potência constante. Canal inativo retorna ao pan-base com rampa de 5 ms. O
movimento pode ser zerado e não gera modulação quando o canal usa timbre não
percussivo. Cenas schema 37 persistem a profundidade; cenas
anteriores abrem com movimento desligado.

Cada stem tem também **VAR** (0–100%) para dosar as diferenças calculadas a
partir da seed. O cálculo por evento combina coordenadas X/Y do percurso com
uma fase e uma taxa determinísticas derivadas da seed e do índice do canal;
isso altera o peso do ataque em até ±20% e escolhe deslocamentos de altura
limitados a dois semitons. A memória da macroforma continua somada a esse
resultado. VAR escala conjuntamente o contraste calculado, o deslocamento de
altura, o pan móvel e o amortecimento por densidade do canal. Em 0%, o canal
mantém eventos, alturas e controles expressivos em seus valores-base; em 100%,
usa integralmente os deltas calculados. O valor é independente por canal e
persiste em cenas schema 37; cenas antigas abrem com 100% para preservar o
comportamento anterior. A operação usa o próprio percurso e a seed, sem
aleatoriedade por evento. As referências de Asterismus e Rasgo Modular
orientaram os princípios de profundidade local por ator e atualização
determinística em fronteiras de evento; nenhum código desses projetos foi
copiado nem adicionado como dependência.

Os mesmos eventos percussivos também recalculam a cauda modal conforme a
densidade de ataques. Para intervalo `Δt` e intensidade normalizada `I`,
`q = clamp((0,55 − Δt)/(0,55 − 0,08), 0, 1)` e
`d = clamp(0,35·q·(1,10 − 0,50·I)·S, 0, 0,42)`. `S` expressa a intenção:
construir `0,70`, rarefazer `1,20`, encontrar `0,85`; contrastar alterna
`0,70/1,20` por subdivisão. Um coeficiente adicional
`g = exp(−d/(0,22·Fs))` atenua os modos e a cauda de ruído. Golpes densos e
menos intensos encurtam mais a cauda; a intenção e a curva dinâmica mudam essa
relação. Ataques espaçados não recebem amortecimento adicional; o controle
**Cauda** continua definindo o decaimento-base. É uma regra de sound design do
Batruc, não uma simulação medida de material ou sala.

O Mixer também oferece **Cauda**, que escala o amortecimento modal, e
**Fundamental**, que controla o primeiro modo nos timbres harmônicos. As caudas
tinham duração e primeiro modo suficientes para acumular energia audível; os
novos valores iniciais são 0,35× e 0,55, respectivamente. Cenas da sessão
guardam panorama, seleção sonora, movimento de panorama e esses dois ajustes.
O botão `REC` grava o master estéreo em WAV PCM 24-bit na taxa do dispositivo,
com limite padrão ajustável de 5 minutos (opções de 1 a 15) e TXT companheiro
contendo a cena do início. Durante a gravação, fica vermelho e mostra `STOP`
com o tempo decorrido; ao alcançar o limite, para automaticamente. A fila de
áudio tem cerca de dois segundos; saturação é sinalizada ao finalizar. STOP
transfere a drenagem da fila e a gravação do TXT para uma tarefa de segundo
plano; o estado “Finalizando WAV…” mantém outra captura desativada até terminar.
O callback não escreve no disco. Exportação de stems isolados ainda está
pendente. Ciclos próprios, fase rítmica que mova ataques, articulação, duração
e timbre ainda não são editáveis individualmente para cada voz. O controle
`φ fórmula` do JUCE desloca apenas a transformação de intensidade, e as cenas
de esquema 2 guardam esse valor por canal; cenas de esquema 1 carregam com fase
de fórmula zero. O módulo de baixo nativo inclui LFO contínuo com destino
selecionável, relação rítmica com as vozes, composição por células, matérias
sonoras, perfil espectral e VCF/ADSR. A maquete web ainda não oferece os três
destinos contínuos do LFO, e a comparação sonora geral dos motores permanece
aberta.

Cada voz ativa ganha um círculo de fase e uma faixa própria no gráfico
temporal, com curva e ataques na mesma linha. O botão de seta alterna ondas
deslocando-se à esquerda, à direita e linha temporal fixa; no fluxo, o instante
tocado permanece no centro. `+`, `−` ou roda do mouse ampliam a vista até uma
janela de um segundo. Arrastar um trecho ampliado ativa a linha fixa para
percorrer a seção; clique duplo restaura a vista inteira. A navegação visual
não altera a composição nem o áudio. O estudo para ampliar o deslocamento como
**parâmetro composicional** está em
[MOVIMENTO_GRAFICO_E_SOM.md](MOVIMENTO_GRAFICO_E_SOM.md).

O primeiro controle de deslocamento **com consequência sonora** está no Mixer:
`Movimento V2` acrescenta de 0 a 0,125 volta por compasso à fase-base da
segunda voz. A cada compasso, o app escolhe uma das agendas já preparadas;
os ataques de V2 mudam, enquanto os tempos da voz 1 e das demais vozes
permanecem nos seus planos. A fase-base continua editável no controle `φ₂`.
O gráfico mostra a agenda ativa, mas a passagem entre agendas é discreta, no
compasso; ainda não há velocidade/direção independente editável para todas as
vozes. O controle de movimento é guardado nas cenas de sessão e começa em
zero, preservando o comportamento anterior.

A Lei I oferece oito funções de intensidade: quatro iniciais, Gauss no centro,
bandas cosseno e energia potencial/cinética de dois osciladores idealizados. O
mesmo cálculo é usado no renderer e nos marcadores do gráfico; `λ` mistura o
resultado com intensidade uniforme. Os modelos físicos e as unidades estão
descritos no guia Aprender do app e em
[MOVIMENTO_GRAFICO_E_SOM.md](MOVIMENTO_GRAFICO_E_SOM.md).

Na Lei I, `x` e `y` são deslocamentos normalizados das duas trajetórias e
`n₁`, `n₂` são seus ciclos por seção. As opções de energia calculam
`U=(n₁²x²+n₂²y²)/(n₁²+n₂²)` e `K=1−U`. É um modelo idealizado de dois
osciladores harmônicos independentes, sem unidades físicas nem integração de
equações no app; portanto, a leitura é uma energia normalizada para a
composição, não medição de energia acústica ou de um pêndulo real. A lei muda
a intensidade dos eventos selecionados, não os seus tempos. Ela oferece um
primeiro exemplo concreto da cadeia conceito–modelo–mapeamento–som. Swing,
groove, intenção e dinâmica já estão disponíveis no JUCE; o estado de paridade
e suas limitações estão registrados em [TAREFAS.md](TAREFAS.md).

O host nativo oferece BPM entre 20 e 220 e compassos 3/4, 4/4, 5/4 e 7/8. A
seção tem quatro compassos; seu comprimento é recalculado pelo andamento e pela
fórmula. A mudança de andamento preserva a fase normalizada quando está em pausa
e entra no próximo início de seção durante a reprodução; o plano de ataques
continua armazenado como posições relativas da seção. Cenas guardam e restauram
BPM e compasso.

O primeiro núcleo nativo da linha de baixo calculada está em
`src/core/BassComposition.hpp/.cpp`. Ele prepara uma partitura de quatro
compassos no compasso selecionado, com 16 posições normalizadas por compasso,
notas e pausas explícitas,
alturas escritas e alturas derivadas da seed, e durações manuais, Fibonacci ou
geométricas. A interpretação é uma etapa separada que aplica swing, intenção,
intensidade e repetições, preservando a partitura de origem. O plano tem limite
de 64 notas e 512 ataques interpretados. O mapeamento nativo da seed do baixo
tem versão própria e ainda não garante identidade de resultado com a maquete
web. Groove por frase, harmonia e escolha de modelo físico para o baixo seguem
na fila.

No percurso seed, o baixo já usa campo médio das vozes, memória integral,
primeira e segunda diferenças e rota determinística para escolher deslocamentos
de altura. A análise agora acrescenta `ρxy` (correlação de Pearson das
coordenadas dos eventos) e `β̃=tanh(cov(x,y)/var(x))` (inclinação comprimida
da regressão de `y` sobre `x`). Seus pesos são limitados e participam do alvo
de movimento entre ciclos; o gráfico e a partitura registram esses valores.
Isso acrescenta uma relação estatística ao percurso sem converter notas numa
escala fixa. É um modelo composicional próprio, não uma previsão científica do
que uma voz “deveria” fazer. Sua consequência sonora precisa ser comparada por
escuta em seeds e relações distintas.

Uma linha calculada não precisa ocupar sempre a função de apoio: ela pode
responder a outra voz, dividir o pulso ou assumir o primeiro plano. Essas
funções devem ser escolhas de composição e interpretação com efeito audível,
sem alterar silenciosamente as notas e pausas da partitura calculada.

A primeira interação composicional se organiza em quatro gestos curtos:

1. **Construir:** escolher a receita (passagens pelo centro ou extremos), as
   razões/ciclos das duas vozes e sua fase. Isso cria uma versão identificável
   da construção e a conserva como ponto de retorno.
2. **Percorrer:** iniciar ou interromper a execução. O relógio musical determina
   eventos e posição no ciclo; a animação apenas representa esse estado no
   instante correspondente. Redesenhar a tela nunca adianta, atrasa ou agenda
   eventos sonoros.
3. **Deslocar:** editar explicitamente uma relação da construção, inicialmente
   fase ou razão/ciclo. Durante execução, a mudança fica pendente e entra no
   próximo limite musical escolhido (no primeiro recorte, início do próximo
   compasso); não reescreve a versão guardada. A interface indica o valor atual
   e o valor pendente até a troca.
4. **Recuperar:** selecionar uma versão guardada e aplicá-la no mesmo tipo de
   limite musical. A versão que estava tocando também é guardada, para permitir
   retorno; recuperar não apaga o histórico nem reinicia o relógio da peça.

O estado é composto pela receita, parâmetros, seed/variação, instante de
aplicação e relação com a versão anterior. Edição produz nova versão; transporte
e desenho leem uma versão ativa imutável até a fronteira de aplicação. Fora da
execução, a alteração pode ser aplicada imediatamente. Esta regra é comum à
edição JUCE e à web, ainda que cada interface ofereça outro gesto de entrada.
O protótipo atual não implementa esse histórico versionado nem a troca
quantizada; a especificação não deve ser confundida com recurso existente.

### Princípio: tornar audíveis as relações

Cálculos, funções, algoritmos e leis físicas não são música por si mesmos. O
instrumento os torna musicalmente consequentes ao escolher quais relações
preservar e como traduzi-las em parâmetros e decisões de composição. A
pergunta orientadora não é apenas “que som corresponde a esta fórmula?”, mas
“o que esta regra faz musicalmente, e como podemos ouvir e transformar esse
efeito?”.

Cada proposta generativa articula quatro camadas:

1. **Modelo:** conceito, axioma, problema, função, algoritmo ou modelo físico,
   com domínio e limites declarados.
2. **Tradução musical:** relações preservadas e seus efeitos em altura, ritmo,
   duração, timbre, intensidade, articulação, vozes, pausas e interação.
3. **Forma:** como o processo começa, se desenvolve, cria contraste, suspende,
   retorna ou deixa vestígios, incluindo as decisões e intervenções disponíveis
   ao músico.
4. **Interpretação e realização:** como músicos e produtores dão caráter à
   estrutura por meio de groove, swing, intenção, acentuação, articulação,
   fraseado, microtemporização, dinâmica e balanço entre vozes. Essas escolhas
   devem ser audíveis, editáveis e, quando ligadas a uma seed ou versão, poder
   ser recuperadas; não devem surgir como variação aleatória opaca.

A comparação proposta pelo autor com a interpretação barroca orienta esta
camada: a notação pode fixar alturas, durações e relações, enquanto deixa
espaço para decisões de realização. É uma analogia de projeto sobre agência
interpretativa, não uma equivalência histórica entre práticas barrocas,
Gramani e tradições rítmicas culturalmente situadas. No Batruc, a estrutura
gerada deve continuar legível enquanto groove, swing, intenção e gesto podem
transformar sua execução.

#### Batuque, corpo e relações rítmicas

“Batuque” tem usos históricos e musicais diversos e também nomeia tradições
religiosas vivas. No Rio Grande do Sul, Batuque designa uma religião afro-
brasileira específica; suas nações e cosmologias não são intercambiáveis com as
do Candomblé, da Umbanda ou do Tambor de Mina. A literatura etnomusicológica
também analisa como a palavra foi usada para racializar sonoridades e
gestualidades negras. Por isso, o Batruc não tratará “batuque” como sinônimo
universal de percussão, preset de gênero ou licença para reproduzir toques
sagrados fora de contexto.

Como pesquisa composicional, interessa investigar mecanismos gerais que podem
ser descritos sem alegar equivalência cultural: pulso e subdivisão; acentos e
contratempos; densidade e silêncio; intertravamento entre partes; chamada e
resposta; repetição com memória; microtemporização e gesto de ataque. Cada
mecanismo deve ser editável por voz, audível em solo e conjunto, ter seed
recuperável e revelar quais eventos foram deslocados, reforçados ou omitidos.
Elasticidade temporal deve ser limitada e musicalmente legível, não ruído
aleatório apresentado como “humanização”.

Termos como *levada*, *clave*, *toque* e *ginga* não serão usados como rótulos
intercambiáveis. Caso uma pesquisa futura trate de tradição ou prática
identificável, ela deverá nomear o contexto e suas fontes, distinguir
observação de abstração, e buscar diálogo com praticantes antes de incorporar
material ritual ou chamar um resultado de representação dessa prática.

Referências de partida: Ari Pedro Oro, “Religiões afro-brasileiras do Rio
Grande do Sul: passado e presente” ([Estudos Afro-Asiáticos, 2002](https://www.scielo.br/j/eaa/a/MHgZxZM6Nw5qzMqZHyy7dQg/));
Cauê Fraga Machado, “Lugares e objetos de memória no batuque gaúcho”
([Religião & Sociedade, 2015](https://www.scielo.br/j/rs/a/xvvJFTyMdTWqq8V9HZTHmBF/?lang=pt));
Marcos dos Santos, *Perspectivas etnomusicológicas sobre Batuque:
racialização sonora e ressignificações em diáspora* ([tese, UFBA, 2020](https://bdtd.ibict.br/vufind/Record/UFBA-2_8ca0992b80ed656481f649e51d40e41a));
Norton F. Corrêa, “Os tambores iorubá no Brasil”
([Revista África(s), 2014](https://revistas.uneb.br/africas/article/view/2405)).
São pontos de partida, não substitutos de escuta situada ou diálogo com as
comunidades envolvidas.

### Intenção e gesto dentro do cálculo

O músico pode declarar **função da voz**, **direção da frase** e **relação com
outras vozes** como entradas da receita. O gerador produz alternativas
limitadas, avalia restrições verificáveis e mostra o que mudou. A realização
posterior continua livre para articular a frase tocada. “Semântico” aqui é o
sentido musical atribuído por quem compõe e escuta; o algoritmo trabalha com
consequências observáveis dessa intenção, sem afirmar que consegue medir seu
significado ou qualidade artística.

Para o baixo, uma receita pode manter a sequência de durações Fibonacci e
comparar três funções declaradas pelo músico: **apoio** favorece pontos de
referência e continuidade; **resposta** procura lacunas de outra voz e deixa
espaço para ela; **primeiro plano** favorece um contorno reconhecível e
variações de frase. Devem permanecer visíveis a fórmula, as notas candidatas,
a posição escolhida, as pausas e a relação com a voz de referência. A função
do baixo é uma escolha composicional editável.

Um primeiro algoritmo de pesquisa pode preparar até oito variantes da mesma
frase e compará-las com medidas legíveis: ocupação de lacunas, coincidências
escolhidas, continuidade de contorno, saltos de altura e espaço para a
respiração. Os pesos e limites dependem da função declarada. A seed desempata
escolhas de modo reproduzível, sem sortear cada ataque isoladamente. As medidas
ajudam a navegar possibilidades; a escuta e a escolha musical continuam com o
autor.

Para dar continuidade orgânica, o estado da frase inclui seu ponto de partida,
energia, direção, repouso previsto, eventos anteriores e gestos do músico.
Curvas de dinâmica, microtemporização e articulação atravessam grupos de notas
e pausas. A interface deve permitir comparar a partitura calculada, a
alternativa escolhida e a realização tocada. Durante a reprodução, recalcular
alternativas fora do callback de áudio e aplicar a escolha numa fronteira
musical definida, preservando a versão ativa.

**Inversão de vozes** é uma transformação relacional a investigar. Na primeira
experiência, duas vozes mantêm seus instrumentos, registros e identidades
sonoras, enquanto trocam os papéis rítmicos de **ostinato** e **série em
desenvolvimento**, incluindo posições de ataque e pausas declaradas. O músico
decide se acentos e articulação acompanham o material trocado ou ficam com a
voz. A operação preserva as duas receitas de origem, seus períodos e a
proveniência da troca; aplica-se numa fronteira musical e pode ser desfeita.
No gráfico, as duas linhas mostram quem executa cada papel antes e depois.
Outra investigação poderá trocar funções sem trocar as agendas, por exemplo
fazendo o baixo passar de apoio a primeiro plano pela relação de dinâmica,
fraseado e espaço entre vozes. São operações diferentes, com nomes e resultados
audíveis diferentes.

A passagem atribuída a Gramani em [Lima e Fiaminghi,
UDESC](https://www.udesc.br/arquivos/udesc/id_cpmenu/10570/50_15657985001175_10570.pdf)
relaciona inversões de vozes à criação de novas associações e à solicitação da
sensibilidade musical (*Rítmica*, 1988, p. 12, conforme citação nesse texto).
Esta operação do Batruc é uma hipótese autoral inspirada nessa leitura;
não pretende reconstruir um exercício do livro.

O primeiro código dessa operação já existe no core como
`invertEventPlanVoices`: prepara outra agenda para duas vozes, troca a
propriedade de seus eventos, mantém a receita sonora dos canais e deixa a
agenda original intacta para A/B ou retorno. Ele valida limites e ordena os
eventos depois da troca. Ainda não resolve o encaixe de alturas do baixo, a
edição da operação na interface nem a aplicação durante a execução do app.

A família de inversão, recombinação, recálculo e variação em escalas de célula
até peça está descrita em
[OPERACOES_GENERATIVAS.md](OPERACOES_GENERATIVAS.md). O core já prepara
inversão de acordes, espelho de alturas do baixo, retrógrado temporal,
recombinação por compassos, recálculo transacional de receita e inversa de
rotação de fase. São operações de dados; o painel e a forma de peça inteira
continuam em desenvolvimento.

A biblioteca de estudo proposta pelo autor, com dados bibliográficos
conferidos, correções de atribuição e experimentos ligados ao instrumento,
está em [BIBLIOGRAFIA_DE_ESTUDO.md](BIBLIOGRAFIA_DE_ESTUDO.md). Ela conecta
geometria rítmica, composição algorítmica, modelagem física, DSP, groove,
expressão e cognição sem tratar teoria como receita automática de música.

O gráfico pode revelar o percurso e o cálculo; a escuta deve revelar suas
consequências musicais. Uma tradução é composicionalmente promissora quando
produz diferenças perceptíveis de fraseado, densidade, tensão, repouso,
contraste ou forma, que o músico possa comparar, editar e escolher. Isso é um
critério de investigação, não uma garantia automática de qualidade. Toda
conexão deve ser identificada como consequência matemática, simulação de um
modelo físico definido ou analogia composicional. A ferramenta deve explicar
qual relação foi mantida e não sugerir que a matemática ou a física contenham
uma música pronta.

### Contrato entre conceito, cálculo e decisão musical

Cada receita deve tornar rastreável a cadeia **conceito → modelo → estado ou
evento → mapeamento → realização → escuta**. Para cada elo, mostrar a entrada,
a operação, a saída e o que permanece invariável. Separar três tipos de
relação: resultado matemático calculado; simulação física com modelo, unidades
e condições declaradas; e analogia composicional escolhida pelo autor. Um
mapeamento pode ser musicalmente fértil sem ser uma lei da natureza, desde que
seja apresentado como escolha e possa ser alterado.

Uma mesma saída do modelo pode alimentar papéis musicais independentes. Por
exemplo, o deslocamento pode selecionar alturas numa voz, a velocidade pode
modular a intensidade de outra e os pontos de inversão podem gerar ataques de
percussão. O painel deve explicitar a origem de cada valor, a transformação, a
faixa de destino, o limite/saturação e o parâmetro musical afetado. Assim, a
pessoa pode manter a física e trocar a composição, ou mudar o modelo mantendo
uma tradução musical para comparar resultados.

Para modelos dinâmicos, registrar estado inicial, parâmetros, passo ou método
de integração, duração, condições de contorno e critério de estabilidade; para
funções fechadas, registrar domínio, amostragem, normalização e tratamento de
singularidades. A agenda estrutural resultante é determinística para os mesmos
parâmetros e seed. Intenção, função da voz e gesto também podem participar da
escolha dessa agenda; a realização posterior pode alterar relações temporais
declaradas, energia, envelopes e agrupamentos. Cada decisão oferece comparação
A/B, preserva o estado anterior e não reescreve a fórmula fonte.

**Exemplo de pesquisa: pêndulo duplo idealizado.** O estado angular e a
velocidade angular são calculados pelo modelo dinâmico escolhido. Uma receita
de demonstração pode mapear ângulo para altura contínua ou grau de escala,
velocidade para intensidade, extremos para articulação sustentada e passagens
por regiões para ataques; cada opção precisa declarar escala, limiar e
quantização. Groove/swing pode então deslocar os ataques por uma regra
controlável, e a intenção pode desenhar uma curva de energia por frase. Os
gráficos mostram simultaneamente o percurso físico calculado, os eventos
estruturais e os desvios interpretativos. Isto é uma proposta de experimento,
não uma equivalência física-musical nem uma receita já validada pela escuta.

## Corpo sonoro: bateria, percussão e acústica

Requisito do autor registrado em 2026-10-11: bons sons de bateria e percussão,
módulos geradores com dinâmica, variação tímbrica e acústica. A precisão dos
eventos ou a qualidade dos gráficos não bastam para atender a esse requisito.
É direção musical; nenhum destes geradores está implementado no batruc.

Famílias a investigar, sem fechar um catálogo: bumbo, caixa, toms,
chimbal/pratos; peles, madeiras, metais, sinos e objetos percussivos
experimentais. Buscar vozes reconhecíveis quando desejado e liberdade de
transformação, sem limitar a paleta a uma bateria eletrônica fixa.

Cada voz deve ser tocada, não apenas disparada. Propostas de exploração:

- Intensidade do gesto alterando não só ganho, mas ataque, brilho, proporção
  de ruído/corpo e resposta de ressonância, conforme o modelo da voz.
- Articulação: acento, nota-fantasma, toque abafado/aberto, posição e tipo
  de impacto quando pertinentes; transições suaves entre estados.
- Matéria tímbrica: afinação, decaimento, excitação, amortecimento e modos
  de ressonância. Material, dimensão ou tensão só serão apresentados como
  controles físicos se o modelo sustentar essa interpretação.
- História entre golpes: repetição com nuances, caudas, reexcitação e
  abafamento; investigar, por exemplo, a relação chimbal aberto/fechado.
  Não substituir expressividade por sorteios independentes a cada ataque.
- Evolução por frase/seção: variar articulação e timbre mantendo ou
  transformando uma identidade, com escolhas editáveis pelo músico.

Separar duas camadas de acústica: a do corpo sonoro (ressonância,
amortecimento e resposta ao gesto) e a do ambiente (reflexões, reverberação,
posição e distância). Investigar também acoplamentos entre corpos como
hipótese posterior, sem prometer simulação física completa. Manter a leitura
rítmica e a possibilidade de comparar som seco e espacializado.

Síntese própria, modelagem física/modal e soluções híbridas são caminhos a
avaliar, não escolhas técnicas já fechadas. Se amostras ou impulsos externos
forem considerados, exigir origem, autorização/licença e créditos específicos.
Consultar o inventário RASGO antes de criar ou adaptar geradores.

Critério musical de avaliação: ouvir variações controladas da mesma frase,
de notas-fantasma a acentos, e conferir diferenças de articulação e timbre,
equilíbrio entre vozes, clareza das polirritmias e comportamento das caudas.
Contrastes precisam ser audíveis sem depender da animação; escuta do autor
complementa, mas não substitui, testes de níveis, estabilidade e recursos.
Vozes simultâneas, ressonadores e caudas terão limites explícitos.

## Baixo: cálculo, interpretação e corpo sonoro

O baixo do Batruc parte da partitura calculada já existente em
`src/core/BassComposition.hpp/.cpp`. Seu próximo estágio deve conservar três
responsabilidades distintas, inspiradas pela pesquisa interna do RASGO e
adaptadas à identidade matemática/visual deste instrumento:

1. **Função/cálculo:** fórmula, célula, altura, duração, pausas, repetição e
   papel da linha na frase. Seed e operações transformam esta camada de forma
   reproduzível.
2. **Interpretação:** gate, acento, nota-fantasma/abafada, slide, swing,
   microtiming, curva de energia, pressão e memória de frase. A fórmula pode
   sugerir esses valores; a escuta e a escolha do músico continuam visíveis.
3. **Corpo sonoro:** fonte ou ressonador, ADSR de amplitude, filtro e sua
   envoltória, modulação opcional, saturação e nível. O mesmo score deve poder
   ser comparado por mais de uma matéria, sem trocar sua lógica composicional.

Referências locais úteis: no Rasgo Synth Studio, `BassFunctionPlan`,
`BassPhrasePlan`, `BassPerformancePlan` e `BassBodyAdapters` separam essas
camadas; `AcidBasslineVoice` combina oscilador, filtro ressonante, decaimentos
distintos de amplitude/filtro, acento e slide; `BassPhraseTimbre` muda o
espectro por frase e transiente. O `Envelope` do Rasgo Modular reúne uma forma
AD/ASR, VCA de áudio e saída de controle; seu `Filter` oferece saídas low/band/
high/all e modulação de parâmetros. Antitotem oferece um contorno ADSR digital
e formas LFO próprias. Aquorbium contém o protótipo `BiomaBassVoice`, uma
possível pesquisa de matéria, hoje mantida em stand-by. Os projetos de origem
não foram alterados; detalhes, limites e licenças pendentes estão registrados
no Atlas RASGO e em `TAREFAS.md`.

### O que o Batruc pode acrescentar

- Fazer a **fórmula também desenhar e governar a articulação**: curvas de
  ataque, decaimento, cutoff e LFO aparecem junto da linha e do cursor de
  execução; cada mapeamento mostra unidades, limites, evento produzido e
  permite intervenção.
- Criar um **campo de acento/intenção** calculado por evento ou frase. Um valor
  coerente pode alimentar em proporções editáveis dinâmica, duração de gate,
  abertura do filtro e transiente. Pausas, acentos e mudanças tímbricas passam
  a resultar da mesma causa composicional, sem randomizações independentes.
- Comparar a mesma partitura entre corpos contrastantes — sub/fundamental,
  espectro aditivo, voz filtrada com slide, ou ressonador/modal — e preservar a
  fundamental grave enquanto se explora o movimento dos harmônicos superiores.
  Cada corpo será um modelo sonoro identificado, não uma promessa de réplica
  acústica ou de gênero musical.
- Tratar LFO como modulador do baixo, com fase e taxa legíveis no gráfico; a
  curva poderá seguir divisões de tempo ou permanecer livre. Swing altera
  primeiro os eventos e só depois poderá deformar a fase do LFO como opção.

No JUCE, o LFO do baixo agora modula continuamente a execução, em vez de apenas
amostrar sua curva no início de cada nota. O painel escolhe altura (±0–12
semitons), corte do filtro (±0–1200 Hz, com limites seguros) ou amplitude
(profundidade máxima de 65%), além de forma triangular/senoidal/quadrada e
período de 1/2/4 ciclos formais. A fase segue o transporte, inclusive nas
seções de pausa; mudanças de alvo, forma, período e profundidade entram durante
a execução. A curva mostra a fase e o alvo selecionado; na altura, o readout
indica a nota contínua estimada em MIDI. Cenas schema 32 guardam o alvo, a
retrogradação, o espelhamento de alturas e a recombinação entre ciclos; esquemas
anteriores continuam abrindo com altura e tempo direto e sem recombinação.
Esses incrementos são nativos JUCE e ampliam a diferença atual para a maquete
web; não são contabilizados como paridade. A compilação e os testes focados
cobrem os três destinos de modulação, retrogradação, espelhamento e
recombinação entre ciclos, mas ainda faltam inspeção do painel e escuta pelo
dispositivo.

No mesmo painel, **Retrogradar tempo** reflete as posições de cada evento dentro
de seu ciclo formal, sem mudar alturas nem durações. A forma pode voltar ao
tempo direto pelo mesmo botão. Como a operação desloca os ataques contra as
vozes, ela é uma transformação composicional deliberada, não um controle de
humanização ou swing.

**Inverter altura 42** espelha as notas ao redor de F♯2: `m′=84−m`. O operador
mantém a tessitura MIDI 24–60, não altera ritmo ou duração e pode ser desligado
para recalcular a linha original da mesma seed. É uma operação declarada sobre
alturas, não uma afirmação de que toda inversão produza harmonia tonal.

**Recombinar ciclos** preserva a agenda de ataques e sua relação com as vozes.
Em cada compasso de cada ciclo, a operação escolhe um ciclo doador a partir da
seed e transfere altura, dinâmica e duração do evento doador de fase mais
próxima. O doador é sempre um ciclo formal diferente; se não tiver evento no
compasso, o ataque mantém seu material. Desativar o botão recompõe o material
base da seed e mantém as demais operações ativas. O gráfico de percurso mostra
o deslocamento do doador em `REC Δn`. Cenas schema 32 guardam a escolha; a
operação só se aplica a linhas geradas pela seed, não à edição manual.

Primeira integração nativa: `src/core/BassAudio.hpp/.cpp` recebe uma
`BassPerformance` já preparada, sincroniza seus eventos ao ciclo principal e
mistura três componentes senoidais em um dos quatro perfis espectrais. Há envelope de
amplitude para o VCA, filtro passa-baixas de um polo (VCF) e envelope separado
para a abertura do filtro. O painel `Baixo` expõe ativação, Mute/Solo, nível,
ADSR-A, frequência de corte, intensidade do envelope de filtro e duração do gate.
O baixo participa do mesmo conjunto Solo dos oito stems ativos. O gate
padrão ocupa 86% da duração do evento, deixando espaço audível para o release
antes do próximo ataque; 100% produz articulação ligada. O ADSR-F tem agora
ataque, decaimento, sustentação e soltura independentes do ADSR-A. O mixer
oferece trim de nível separado para as vozes de fórmula, antes da soma com o
baixo; o master aplica saturação suave para conter picos quando várias vozes
somam. Um sensor RMS separado de vozes e baixo, disponível durante a execução,
sugere um ajuste estático do trim; o botão aplica essa compensação sem alterar
o ganho nota a nota. RMS orienta, mas não substitui a audição nem mede LUFS.
No modo manual, a linha usa as células editáveis; com seed ativa, o
calendário de quatro compassos é gerado a partir dos eventos vocais, suas
posições x/y e intensidade, sem escala predefinida. O editor JUCE de uma barra
mostra o primeiro compasso como resumo. A voz começa desligada.

**Percurso de alturas por ciclo (31/10):** além da projeção local de `x/y`, a
seed escolhe a relação inicial entre quatro percursos: resposta exponencial ao
campo, contraposição trigonométrica, órbita angular e ruptura condicionada
à distância da origem (`hypot(x,y)`). Energia e direção anterior podem mudar
a relação no ciclo seguinte. Em cada um dos sete ciclos, o core pesa o campo
e a energia dos eventos das vozes, o último deslocamento e a função formal
de mudança, pausa ou retorno. Escolhe um passo limitado e guarda a posição
para o ciclo seguinte. O mesmo cálculo alimenta o áudio seeded e o resumo do
editor JUCE. Não há escala ou progressão de acordes imposta por essa camada;
ela produz continuidade e bifurcações de alturas relativas. A escolha fica
determinística para a mesma seed e a mesma agenda. Ainda faltam escuta,
controle autoral das relações e desenvolvimento entre passagens completas.
O JUCE agora expõe o percurso calculado numa faixa própria: altura relativa,
integral limitada, rota matemática, densidade após respiros e ciclo ativo vêm
do mesmo `BassPerformance` que o áudio consome; não são uma animação paralela.

**Integrais indefinidas como pesquisa composicional e primeiro protótipo:** se `f(t)` for uma lei de
força, gesto ou densidade, a família de primitivas `F(t) + C` pode produzir
estados de acumulação com origens distintas. O parâmetro `C` pode escolher
uma posição inicial de fase, altura ou energia; a derivada descreve a
variação instantânea. Para um ciclo estável, subtrair a média de `f` antes
de integrar impede deriva acumulada involuntária. Se a deriva for desejada,
ela precisa de limites, reinício e ligação audível com eventos. Composição
possível: a área acumulada desloca o próximo ataque, altera a resposta de
outra voz ou aciona uma mudança formal ao cruzar um limiar. Isso é uma
hipótese de mapeamento do Batruc, não um uso musical atribuído a Vygodski.
Cálculos integrais do *Aide-mémoire de mathématiques supérieures* seguem
como referência de estudo; verificar as páginas e fórmulas da edição francesa
antes de importar exemplos específicos.

O primeiro protótipo executável usa o campo normalizado `fₙ=yₙ−xₙ` das vozes
e uma integral discreta com fuga `Iₙ=clamp(0,72 Iₙ₋₁+fₙ,−1,5,1,5)`. A seed
fornece a constante inicial `C`; a fuga e o limite impedem deriva silenciosa.
Diferença primeira e curvatura discreta participam da escolha do deslocamento
de altura, da abertura de respiros e da dinâmica do baixo. O mecanismo é uma
tradução composicional autoral, não uma fórmula retirada do livro. O gráfico
mostra `I`, `Δ`, densidade e as rotas EXP/SIN/ORB/RUP usadas na preparação.
O compositor pode ajustar a retenção da memória entre 0 e 95% e a força dos
respiros entre 0 e 200%; a probabilidade final de lacuna continua limitada a
30% e ataques fortes são preservados. Esses parâmetros e os sete estados
calculados são guardados na cena esquema 27. A cena exportada e a partitura
companheira de uma gravação registram offset, integral, derivada, curvatura,
rota e densidade, permitindo relacionar fórmula, evento e áudio depois da
execução.
O próprio gráfico é também um controle bidimensional: o eixo horizontal edita
a retenção da memória e o vertical edita a força dos respiros. Durante o
arraste há prévia M/R; o plano musical é recomposto ao soltar, para manter a
interface responsiva. Os sliders equivalentes continuam disponíveis.

O gráfico principal publica também o estado do compositor entre passagens:
índice `P`, memória acumulada, rotação das oito células, direção ou inversão e
célula de repouso. Esses valores vêm da mesma função que pondera ataques nas
vozes e no baixo. Ao gravar, o TXT companheiro anota esse estado em cada evento
da agenda inicial e inclui a equação da memória na partitura matemática.
Na agenda inicial gravada, cada evento vocal inclui o peso de passagem
calculado e um indicador de evento audível, ao lado dos estados registrados
para o baixo.

**Matemática de inspiração quântica — pesquisa, sem implementação:** amplitudes
complexas de caminhos candidatos podem interferir antes da escolha: a chance
de um caminho é proporcional ao módulo quadrado da soma de suas amplitudes,
depois de normalização. Fases ligadas às vozes poderiam favorecer ou cancelar
respostas; a seed fixa a escolha para permitir repetição de uma composição.
O modelo deve mostrar amplitudes, fases, probabilidades e resultado no gráfico.
Isso seria uma regra composicional inspirada no formalismo quântico, não uma
simulação física de partículas nem uma fonte automática de qualidade musical.
Antes do código: definir estados, operadores, normalização, memória musical e
um teste que distinga interferência de sorteio ponderado comum.

**Tractriz e pseudoesfera — pesquisa geométrica:** uma tractriz pode ser
parametrizada por `x=a(t−tanh t), y=a sech(t)`, para `t≥0`; a rotação em torno
da assíntota gera a pseudoesfera, com curvatura gaussiana constante negativa
`K=−1/a²` na região regular. No Batruc, a distância percorrida pelo ponto
arrastado pode influir no intervalo entre ataques, a tensão do arrasto na
articulação e o ângulo de rotação no espaço estéreo. A curva e a superfície
devem aparecer como trajetórias animadas ligadas aos mesmos eventos sonoros.
O gesto da **Lupa** do Aquorbium também informa a superfície aberta do Batruc:
um foco se desloca pelo campo e muda a saliência do que atravessa sua região.
No Batruc, essa relação é refeita como acentuação calculada por distância, sem
tomar posse do gesto do usuário, copiar código ou importar dependências.
Uma pseudoesfera visual, sozinha, não determina a acústica de uma sala: para
alegar modos e reverberação físicos é necessário especificar domínio,
condições de contorno, material e solver de ondas. Referências matemáticas:
[UCI Mathematics](https://webapps.math.uci.edu/~vmm/Surface/pseudosphere/pseudosphere.html)
e [Brown Mathematics](https://www.math.brown.edu/tbanchof/balt/ma106/dtext65a.html).
Ainda não há esse gerador no áudio nem na interface.

**Paleta e máscara generativas — direção de desenvolvimento:** a seed deve
escolher e relacionar famílias de som, além de ritmo e alturas. O primeiro
passo nativo atribui deterministicamente caráter global, perfil de cada voz,
matéria e perfil espectral do baixo dentre os modelos já implementados; cada
controle continua editável. Isso ainda não equivale às famílias pedidas pelo
autor: bateria, percussão, xilofone/lâminas, objetos, natureza e sonoplastia;
nem aos corpos graves baixo acústico, fretless, Moog, piano grave, tuba, fagote,
sax barítono e voz baixo. Cada família precisará de articulação, síntese ou
amostras com licença/proveniência auditadas, registro e escuta próprios.
O LFO deve evoluir de três formas e poucos destinos para uma matriz legível
de oscilador, fase, sincronização, profundidade e destino por voz.

Os campos já se deslocam e se orientam com a seed. O próximo aprofundamento é
usar distância assinada para deformação, divisão, recombinação e memória de
fronteira, além de uma máscara de pausa que suprima/libere eventos. Uma futura
edição manual de forma deve preservar o contrato: o gráfico desenha o mesmo
campo consultado pelos eventos, com limites legíveis; variação apenas visual
não conta como composição.

Cenas de esquema 7 guardam ativação, Mute/Solo e os oito parâmetros originais do
baixo; esquema 8 também guarda as células editáveis e a configuração da
receita; esquema 9 acrescenta o trim das vozes e os quatro parâmetros do ADSR-F. Esquemas
anteriores carregam controles ainda inexistentes nos valores padrão. O mesmo
conteúdo acompanha as cenas de início de gravação.

Registro da primeira prova de áudio: naquele estágio havia uma voz monofônica
com síntese aditiva simples, sem validação de timbre. Esse retrato não descreve
o estado atual: o host JUCE agora tem até oito stems ativos e 18 modelos
sonoros. A escuta crítica e a aceitação dos timbres ainda estão pendentes; a
fila e os critérios atuais ficam em [TAREFAS.md](TAREFAS.md).

## Marco técnico inicial — 24/10/2026 (histórico)

Projeto aberto por autorização do autor em 2026-10-10. A maquete web em
[prototype/index.html](prototype/index.html) continua um estudo de interação,
sem síntese definitiva. Em 2026-10-24 foi criado e compilado o core C++17,
com cálculo de eventos por fase e renderer virtual de oito modos ressonantes
por voz. A aplicação JUCE foi compilada como standalone local com duas a oito
vozes, mixer por canal, oito leis de intensidade, cenas, seed determinística,
BPM, círculos de fase por voz e faixas temporais animadas. Inclui duas regras de evento
(cruzamentos do centro/extremos) e três superfícies de
seleção de eventos: livre, ilhas adesivas e microlinhas diagonais. A posição
XY combina as fases das duas primeiras vozes; vozes adicionais compartilham o
filtro XY e acrescentam seus próprios eventos. Uma segunda visualização anima
o percurso e marca ataques por intensidade. A nova versão ainda não foi aberta
nem ouvida. Timbre, níveis,
sincronização percebida, estabilidade e valor composicional continuam sem
validação musical.

C++ e JUCE foram escolhidos. Direção arquitetural proposta: cálculo,
construções, tempo musical e geração em um core próprio, independente da
interface; JUCE como host de áudio/MIDI e superfície gráfica para o instrumento
completo, instalável e multiplataforma. O autor também escolheu manter uma
edição em página web para que usuários experimentem o instrumento sem instalar
o aplicativo. Essa edição poderá ter escopo mais básico, mas deve preservar a
ideia central: mostrar as regras e os desenhos e permitir ouvir suas
consequências composicionais. Não será apresentada como equivalente em recursos
ou qualidade sonora à edição nativa.

Princípio de arquitetura para as duas edições: compartilhar o mesmo vocabulário
de receitas, relações e decisões composicionais; buscar concordância de
resultados para uma mesma receita e parâmetros. A direção preferida é compilar
o core C++ para navegador; se o experimento de viabilidade falhar, a alternativa
é uma implementação web limitada às receitas acordadas, mantendo casos de
referência para comparar as saídas. O WebView do JUCE é uma interface web
embutida em um aplicativo nativo, não deve ser confundido com a distribuição do
instrumento como página executada diretamente no navegador. [Visão oficial de
plataformas do JUCE](https://juce.com/features/) e [documentação do
WebBrowserComponent](https://docs.juce.com/develop/classjuce_1_1WebBrowserComponent.html).

#### Especificação inicial do core e das superfícies — 2026-10-24

Escolhas para a primeira fatia, sujeitas a build, medição e revisão:

- **C++17**, sem extensões, no core portátil. Essa base coincide com o
  `FunctionGenerator`, `EuclidClock`, `Decision` e `Phrase` do Modular que
  foram priorizados como referências; o core não dependerá desses projetos.
- **JUCE 9.0.3 fixado exatamente** para o aplicativo standalone C++/JUCE.
  A cópia local `RASGO/third_party/JUCE-9.0.3` foi verificada no tag `9.0.3`,
  commit `be29c81`, com árvore limpa. CMake rejeita outra versão. A licença
  dessa dependência permanece a indicada em sua árvore (AGPLv3/comercial),
  sem substituir a licença do Batruc.
- **Edição web:** tentar compilar o mesmo core C++17 para WebAssembly com
  Emscripten; a página terá interface HTML/Canvas e áudio via Web Audio
  AudioWorklet. JUCE não será dependência do alvo web. A API AudioWorklet para
  C/C++/Wasm existe no Emscripten, mas desempenho, navegadores suportados,
  tamanho e requisitos de hospedagem precisam ser provados no protótipo.
- **Dependência JUCE externa e fixada**, sem copiar JUCE para a árvore do
  Batruc nem depender do diretório de outro instrumento. Build local usa
  CMake e `.local-build/native/linux-debug-juce`; a página usa
  `.local-build/web/`. A cópia vizinha `RASGO/third_party/JUCE` (9.0.1) não foi
  alterada nem usada.
- **Primeiro formato:** aplicativo standalone em Linux, Windows e macOS; sem
  plugin VST/AU/AAX nesta fatia. Distribuição universal de macOS e instaladores
  por sistema passam por portões próprios de toolchain, teste e pacote.

O core atual pode ser compilado sem JUCE, como biblioteca estática, a partir de
`RASGO/PROJETOS_AUTORAIS/BATRUC/`:

```sh
cmake -S . -B .local-build/native/linux-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build .local-build/native/linux-debug --target batruc_core --parallel 2
```

O standalone JUCE local pode ser compilado com JUCE 9.0.3 já disponível no
arquivo:

```sh
cmake -S . -B .local-build/native/linux-debug-juce \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBATRUC_BUILD_JUCE_APP=ON \
  -DBATRUC_JUCE_SOURCE_DIR="$PWD/../../third_party/JUCE-9.0.3"
cmake --build .local-build/native/linux-debug-juce \
  --target batruc_app --parallel 2
```

O build foi concluído em 2026-10-24. Isso valida compilação e link do core,
host e módulos JUCE; não valida reprodução, dispositivo, interface em execução,
som, desempenho em tempo real ou composição. A edição web permanece pendente.

```text
parâmetros e receita versionada
  -> core matemático/geometria C++17
  -> plano limitado de eventos musicais
       ├─> renderer de vozes e corpos percussivos -> saída técnica -> JUCE/host nativo
       └─> renderer Wasm/AudioWorklet -> Web Audio -> navegador
  -> projeção temporal/geométrica -> interface JUCE ou HTML/Canvas
```

O core calcula relações e eventos sem conhecer widgets, desenho, dispositivo,
arquivos ou rede. A camada de áudio recebe eventos já datados; a interface não
é o relógio. O host de cada plataforma fornece o relógio de áudio, sample rate,
buffer, transporte e timestamp; a visualização consulta esse estado e não agenda
eventos. Receita ativa é imutável durante uma frase. Mudanças vêm como comandos
limitados e são aplicadas na fronteira musical escolhida. Para evitar cópia ou
liberação de memória no callback, planos são preparados fora dele em slots
fixos; a troca e a devolução do slot usam fila limitada com confirmação.

**Contrato do evento:** tempo absoluto/posição musical, ID da voz, intensidade,
articulação, matéria/modelo sonoro e parâmetros dimensionados. O planejador
decide *quando* ocorre; o renderer decide *como soa*. Intensidade (0–1) afeta
excitação/transiente e nível, não apenas ganho. Articulação (normal, ghost,
accent, aberto/abafado) molda envelope e resposta. Matéria escolhe um modelo
sonoro identificado; parâmetros só recebem nomes físicos quando o modelo os
sustenta. Uma nova excitação pode somar energia ao corpo ainda ressoando;
abafamento por grupo aplica fade controlado à cauda ativa. Isso não significa
que a maquete web atual já ofereça esses modelos.

**Tetos iniciais para provar, não números de desempenho já medidos:** 12 vozes
matemáticas; duração máxima de 84 s por realização; até 65.536 eventos por
seção e 262.144 eventos preparados no total;
até 64 comandos pendentes; geometria normalizada finita em `[-1,1]` e até 4.096
pontos desenhados por trajetória/ciclo (amostragem visual pode decimar); pool
fixo de 48 corpos ressonantes simultâneos, com cauda limitada a 20 s e fade de
saída. Valores não finitos, domínio inválido, excesso de eventos ou comando
fora da capacidade são recusados com estado/telemetria legível. Se o pool de
áudio saturar, somente uma cauda já em liberação pode ser encerrada com fade;
nenhum ataque novo substitui silenciosamente outro ataque ativo. Se não houver
cauda liberável, o ataque excedente é recusado e contado na telemetria. Esses tetos
devem ser medidos em pior caso e avaliados pela escuta antes de virarem
contratos definitivos.

**Callback de áudio:** nenhum `new`/liberação de heap, lock, I/O, rede, parsing,
geração de fórmula, cálculo geométrico sem limite ou espera. `prepare()` aloca
voz/pool/buffers e prepara DSP fora do callback. O callback consome planos e
comandos de capacidade fixa, produz áudio float finito, aplica fades e publica
telemetria compacta. Segurança final de saída (mute/fade, detecção de não
finitos e teto de pico) fica após o master criativo, conforme a política RASGO
de [saída de áudio](../../RASGO_DOCUMENTATION/architecture/SAIDA_AUDIO_COMUM.md).

Este contrato ainda é **especificação**. Os tetos, a API C++, o transporte e a
paridade Wasm/nativa dependem de teste de pior caso, comparação de saídas e
escuta; nenhum build ou renderer foi criado nesta etapa.

Nenhum código de instrumento irmão ou de terceiro foi incorporado. Reutilização
futura exige versão/commit, licença, créditos e adaptação à identidade local;
não criar dependência de build sobre pastas de instrumentos irmãos. JUCE e
outras ferramentas externas seguem a localização neutra prevista na governança.

O relógio visual não agenda áudio. Toda implementação futura precisa de
limites de eventos, memória e CPU; tratar degenerações geométricas e valores
não finitos; evitar alocação, locks, rede e I/O no callback de áudio. A escolha
de biblioteca gráfica ou técnica de renderização não foi fechada.

## Origem, referências e direitos

O [brainstorm original](../../0_BRAINSTORM/BATRUC.md) permanece exclusivamente
no HD, sem movimentação ou cópia integral para este projeto. É o registro de
origem e da sondagem; este README é a fonte ativa do conceito do projeto,
com as [tarefas e continuidade](TAREFAS.md) como ponto operacional de retomada.
O brainstorm não deve acompanhar uma futura distribuição do instrumento.

Referências iniciais para estudar, não dependências adotadas:

- [IanniX](https://github.com/buzzinglight/IanniX): percursos e sequenciamento gráfico.
- [RTG](https://github.com/ninioArtillero/RTG): relações entre geometria e padrões rítmicos.
- [Kig](https://apps.kde.org/kig/) e [JSXGraph](https://github.com/jsxgraph/jsxgraph): construção geométrica interativa.
- [Paper.js](https://github.com/paperjs/paper.js): operações sobre caminhos vetoriais.
- [Polyrhythm Metronome](https://github.com/hhyyg/polyrhythm-metronome-pub): prática de polirritmia e agendamento no navegador.
- [ossia score](https://github.com/ossia/score): composição interativa e organização de acontecimentos.

### Vygodski — geometria, séries e equações diferenciais

O autor encontrou uma cópia física doada de *Aide-mémoire de mathématiques
supérieures*, de Marc (Mark Yakovlevich) Vygodski. A edição francesa é da
Éditions Mir, Moscou, 1973, traduzida por Guy Der-Mégréditchian; o catálogo da
Bibliothèque nationale de Tunisie registra 861 páginas e inclui gráficos
([ficha bibliográfica](https://www.bibliotheque.nat.tn/CATG/detailstatic.aspx?RSC_BASE=SYRACUSE&RSC_DOCID=647492&TITLE=aide-memoire-de-mathematiques-superieures)).
O sumário de uma edição correspondente percorre geometria analítica plana e
espacial, cálculo diferencial e integral, curvas, séries, Fourier, integrais
múltiplas, equações diferenciais e curvas notáveis — entre elas espirais,
cicloides, epicicloides, hipocicloides e catenária
([sumário consultável](https://mirtitles.org/2022/06/02/mathematical-handbook-higher-mathematics-vygodsky/)).
O autor também forneceu o [scan integral da edição francesa no Internet
Archive](https://dn720004.ca.archive.org/0/items/m.-vygodski-aide-memoire-de-mathematiques-superieures-mir-1973/M.%20Vygodski%20-%20%20Aide-me%CC%81moire%20de%20mathe%CC%81matiques%20supe%CC%81rieures%20-%20Mir%20-%201973.pdf).
O arquivo digital tem 863 páginas PDF; isso não contradiz diretamente as 861
páginas do registro bibliográfico, pois o scan inclui folhas preliminares e
elementos editoriais. O PDF tem camada de texto pesquisável, além das páginas
escaneadas.

Para o Batruc, esta é uma referência matemática de consulta e estudo: pode
fundamentar geradores de curvas, aproximações de Fourier, integrais de área e
volume e sistemas de movimento descritos por equações diferenciais. Ela não é
um tratado de composição generativa nem de acústica; os mapeamentos para ritmo,
gesto, timbre e forma continuam sendo decisões autorais do instrumento. A
cópia física veio de uma caixa de livros doados; edição, estado de conservação
e localização atual da cópia ainda não foram registrados além do que consta
na ficha bibliográfica.

#### Recorte prioritário: cálculo diferencial e movimento

O sumário da edição consultada também percorre derivadas, diferenciais,
curvatura, séries, equações diferenciais de primeira e segunda ordem e o método
de Euler. Para o Batruc, isso permite tratar uma fórmula como processo e não
apenas como contorno estático.

| Cálculo | Equação | Possível papel composicional |
|---|---|---|
| Taxa de mudança | `v(t)=f′(t)` | Sinal de `v` pode indicar subida/descida; magnitude limitada pode dirigir densidade de ataques ou articulação. |
| Aceleração | `a(t)=f″(t)` | Mudanças de aceleração podem marcar impulso, repouso, acento ou transição formal. |
| Velocidade do traço | `s′(t)=√(x′²+y′²)` | Distingue movimento rápido de lento; sua integral fornece distância percorrida e pode parametrizar o avanço musical pelo arco. |
| Curvatura planar | `κ(t)=|x′y″−y′x″|/(x′²+y′²)^(3/2)` | Curvas mais fechadas podem modular timbre ou acento; requer teto e tratamento especial perto de velocidade zero/cúspides. |
| Oscilador amortecido | `m·x″+c·x′+k·x=F(t)` | Um modelo massa–mola pode conduzir desvios de fase, tempo e intensidade; precisa de integração numérica estável e parâmetros limitados. |

As setas para ritmo e som são hipóteses de composição autorais, não
consequências automáticas do cálculo. Cada quantidade precisa ser normalizada,
ter faixa e curva de resposta documentadas e poder ser ouvida isoladamente.
Para sistemas físicos, o passo numérico altera estabilidade e caráter: não
usar Euler explícito indiscriminadamente em osciladores; comparar integradores
e verificar energia/estabilidade antes de ligar a áudio em tempo real.

#### Tempo maleável: deformar o percurso e fechar o ciclo

O tempo maleável do Batruc pode ser formulado como uma transformação de fase
monótona. Para a posição nominal `u∈[0,1]` de um evento, escolha um campo de
movimento limitado `g(u)` e uma profundidade `λ`; defina
`w(u)=exp(λg(u))` e
`τ(u)=∫₀ᵘw(v)dv / ∫₀¹w(v)dv`. A agenda executa o evento em `T·τ(u)`.
Como `w>0`, a ordem dos eventos é preservada; `τ(0)=0` e `τ(1)=1`, então o
compasso começa e termina no mesmo lugar e a deformação não acumula deriva
entre ciclos. A derivada `τ′` descreve onde o tempo se expande ou se comprime;
a segunda derivada descreve como essa elasticidade muda. `λ=0` recupera a
grade uniforme.

Esse mapa precisa ser distinto do swing: swing redistribui uma subdivisão
local; `τ` molda o fluxo ao longo de uma frase ou ciclo. Um campo comum mantém
as vozes juntas; mapas por voz permitem autonomia rítmica, mas precisam
preservar limites de seção e oferecer um acoplamento explícito. A intensidade,
a curvatura ou a velocidade de outra voz podem alimentar `g` com atraso
causal e profundidade limitada, fazendo a interação alterar a colocação dos
eventos sem reescrever o relógio global. Os controles devem mostrar o mapa e
seu efeito na agenda, e permitir desligar a deformação.

#### Recorte prioritário: “Some Remarkable Curves”

A parte final do handbook lista, entre outras, lemniscata de Bernoulli, espiral
de Arquimedes, espiral logarítmica, cicloide, epicicloide/hipocicloide,
tractriz e catenária (seções 503–517 na edição inglesa consultada). É um recorte
promissor para o segundo painel gráfico do Batruc: as curvas são percursos
parametrizados e suas mudanças de forma podem conduzir eventos e fraseado. As
equações abaixo são fórmulas matemáticas usuais para orientar o estudo, não
transcrição do texto do livro; a forma exata e a numeração devem ser conferidas
na cópia francesa do autor.

| Curva | Equação de referência | Propriedade que pode virar regra composicional |
|---|---|---|
| Cicloide | `x=a(t−sen t)`, `y=a(1−cos t)` | O parâmetro avança com velocidade não uniforme; o arco pode orientar aceleração/repouso. A propriedade tautócrona vale apenas no modelo ideal sem atrito. |
| Epicicloide | `x=(R+r)cos t−r cos((R+r)t/r)`, `y=(R+r)sen t−r sen((R+r)t/r)` | A razão `R/r` controla lóbulos e cúspides; razões inteiras oferecem períodos e contagens conectáveis a ciclos e acentos. |
| Nefróide (epicicloide `R=2r`) | `x=(3cos θ−cos 3θ)/4`, `y=(3sen θ−sen 3θ)/4` | Dois lóbulos/cúspides; a posição da roda externa e do ponto traçador pode conduzir a V1. É uma primeira razão fixa, não um controle geral de `R/r`. |
| Deltóide (hipocicloide `R=3r`) | `x=(2cos θ+cos 2θ)/3`, `y=(2sen θ−sen 2θ)/3` | Três cúspides e simetria ternária; pode propor agrupamento/acentuação ternária sem impor compasso 3/4. |
| Lemniscata de Bernoulli | `(x²+y²)²=a²(x²−y²)` | Dois lóbulos simétricos sugerem alternância entre regiões, resposta e inversão de percurso. |
| Espiral de Arquimedes | `r=a+bθ` | O raio cresce linearmente: útil para acumular densidade, registro ou intensidade por volta. |
| Espiral logarítmica | `r=a·e^(bθ)` | O raio cresce multiplicativamente; útil para expansões/contrações graduais, limitadas e normalizadas. |
| Catenária | `y=a·cosh(x/a)` | Curva de equilíbrio de corrente flexível sob gravidade uniforme; pode servir de envelope de tensão/relaxamento, sem pretender simular uma corda sonora. |

O mapeamento deve ser explícito e desligável. Uma primeira gramática a testar:
`x(t),y(t)` para posição das vozes; comprimento de arco para avanço temporal;
curvatura limitada para acento ou timbre; cúspides e cruzamentos para eventos
de forma; simetria para inversão/resposta; razão entre frequências do traço
para polirritmia. Se o parâmetro `t` for usado diretamente como relógio, a
velocidade do traço também passa a afetar o ritmo; se o arco `s(t)` for o
relógio, o percurso tende a avançar em velocidade geométrica uniforme. O
instrumento deverá permitir escolher conscientemente entre esses dois modos.
Curvaturas em cúspides e singularidades precisam de saturação/limites para
evitar picos de áudio ou eventos sem limite.

Primeira etapa implementada nas duas saídas: nefróide (`R=2r`) e deltóide
(`R=3r`) são trajetórias selecionáveis; o painel mostra a circunferência fixa,
a roda rolante e o raio até o ponto traçador. Com “Usar y do desenho como
função da voz 1”, `y(t)` entra no cálculo de eventos/intensidade da V1. A razão
entre raios permanece fixa.

Ampliação seguinte, também nos dois motores: um arco de cicloide normalizado,
seguido por retorno horizontal na base para fechar o percurso sem salto, e a
catenária `y=cosh(x)` percorrida nos dois sentidos. Ambas estão no seletor,
mostram a equação e podem alimentar `y(t)` da V1. A
catenária é parametrizada como traço contínuo de ida e volta; não se apresenta
como simulação de uma corrente física. Ainda falta comparar os desenhos e a
resposta sonora durante execução.

#### Leitura visual do scan: polígonos, curvas e cortes

Foi feita uma inspeção visual por amostragem das páginas escaneadas, além da
consulta à camada de texto. As figuras são diagramas didáticos estáticos de
construção: mostram eixos, pontos, raios, tangentes, círculos geradores,
diretrizes e posições sucessivas. Não são gráficos de áudio nem animações
prontas. Entre as páginas conferidas estão a soma vetorial pela regra do
polígono (p. 126), a espiral de Arquimedes (p. 775), a geração da cicloide por
rolamento (p. 788), epicicloide/hipocicloide (p. 801) e catenária/tractriz
(p. 824; paginação impressa).

Na geometria espacial, as ilustrações mostram sólidos e suas secções planas:
cone de duas nappes e cortes que produzem cônicas (p. 216), paraboloide
hiperbólico com secções em diferentes direções (p. 219) e superfícies de
revolução geradas por curvas (p. 226). O tratamento é útil para pensar
polígonos como percursos fechados, decomposição em segmentos, interseções,
vértices e simetrias; e cortes como operações que transformam uma superfície
em contornos dependentes da posição e orientação do plano. O livro não propõe
essas operações como técnicas de composição musical: a tradução para ritmo,
altura, timbre e forma continua sendo hipótese autoral do Batruc.

Para o painel gráfico, o próximo experimento é tornar os vértices do polígono
editáveis e atualizar perímetro e área à medida que o traço muda; depois, (b)
um plano móvel atravessando um sólido parametrizado, exibindo a curva de
intersecção e sua área projetada. A cicloide contínua agora existe como curva
parametrizada, mas seu círculo gerador ainda não é animado. Os
valores derivados devem permanecer vinculados ao desenho e ser mapeados a
eventos por regras selecionáveis, limitadas e desligáveis. O vínculo
implementado é `y(t)` → eventos/intensidade da V1. Triângulo e quadrado agora
calculam área pelo método do cadarço (shoelace) e perímetro pela soma das
arestas; os valores normalizados aparecem no painel em HTML e JUCE. Esses
descritores ainda não modulam o som. Curvatura e intersecções de sólidos
continuam sem cálculo no instrumento.

### Caustic 8BitSynth: precedente de fórmula executável — 2026-10-24

O manual do [Caustic 3](https://adistancia.fam.unam.mx/uploads/docs/Caustic3.0.0manual.pdf)
confirma a máquina **8BitSynth**: um contador temporal `t` alimenta expressões
aritméticas e bitwise; a saída é calculada em 8 bits e retorna como áudio. Há
duas expressões A/B com mistura, teclado de operadores, altura e volume. É um
gerador de waveform bytebeat para experimentação sonora, não um editor geral
de algoritmos composicionais, nem uma ferramenta que desenhe qualquer função
como gráfico e converta diretamente seu contorno em notas.

Isso dá um precedente concreto para tornar as fórmulas executáveis no
batruc. A hipótese própria permanece diferente: expressões visíveis devem
produzir relações entre vozes, ritmos e forma; o gráfico deve mostrar o cálculo
que conduz os eventos e o gesto do músico deve poder transformá-lo. Antes de
implementar um avaliador de expressões, definir linguagem limitada, semântica,
limites de custo e mapeamentos de saída. Nenhum código ou fórmula do Caustic
foi copiado. O workspace não contém fonte nem documentação local do Caustic;
a consulta foi ao manual público.

Em 06/11/2026 entrou a primeira matéria sonora 8-bit própria do Batruc. O novo
`SoundCharacter::chip8` parte de uma fonte harmônica tipo pulso e aplica
sample-and-hold a 8 kHz, quantização de amplitude assinada em 8 bits e uma
excitação curta de ruído digital. Ele aparece na matéria global, no inspetor
por stem. A família **8-bit** atualmente mantém os papéis percussivos estáveis
de V1–V8 e aplica um perfil curto/escuro; ela não seleciona automaticamente
`chip8`, pois isso substituiria bumbo, caixa e chimbais pelo mesmo oscilador.
Essa associação precisa ser redesenhada com modelos digitais por papel. O
valor foi anexado ao enum para não
reinterpretar cenas antigas. O inspetor do stem expõe `DUTY` (12,5–87,5%),
`CLOCK` (1–24 kHz), `BITS` (2–8) e `RUÍDO` (0–100%); os quatro parâmetros são
aplicados no callback sem alocação e persistem no schema de cena 41. A
auditoria de layout e round-trip passou em 1080×800 e 1440×900. Ainda não é um
avaliador bytebeat: LFSR configurável, envelope dedicado e fórmulas A/B
permanecem como evolução explícita, não como capacidades implícitas.

### Teoria dos anéis e área de anéis geométricos

“Ring” em teoria dos anéis (francês *théorie des anneaux*) é uma estrutura
algébrica com operações de soma e produto; não é o mesmo que um anel geométrico
(*anneau*) no plano. Ambos podem informar o Batruc por caminhos distintos.

Para uma célula cíclica de `N` passos, represente os pesos dos eventos por
`P(x)=Σ pᵢxⁱ` no quociente `R[x]/(xᴺ−1)`. Multiplicar por `xᵏ` gira a célula em
`k` passos; somar polinômios sobrepõe camadas; multiplicá-los faz convolução
circular, combinando cada evento com os deslocamentos do outro padrão. A
identidade entre convolução cíclica e produto nesse anel quociente é padrão em
DSP e pode ser calculada pela DFT/FFT ([Tolimieri, An Algorithms for Discrete
Fourier Transform and Convolution, cap. 6](https://ece.uprm.edu/~domingo/INEL8396/TOLIMIERI%20-%20Algorithms%20for%20Discrete%20Fourier%20Transform%20and%20Convolution%20%281989%29.pdf);
para fundamentos de anéis e polinômios, ver as [notas de Álgebra Abstrata do
MIT](https://math.mit.edu/~mckernan/Teaching/12-13/Spring/18.703/book.pdf)).

Isso oferece ao Batruc operações explícitas para rotação, sobreposição e
recombinação de células e uma ponte matemática entre padrão rítmico e filtro
FIR. A operação não determina por si só a música: os coeficientes precisam de
uma semântica (presença, contagem, intensidade ou probabilidade), faixa e
normalização. Com coeficientes binários em `F₂`, soma significa XOR e duas
batidas coincidentes se cancelam; esse comportamento só deve ser oferecido
como operação escolhida, não como mistura padrão. A primeira operação
implementada é o modo opcional **Convolução cíclica** do encaixe do baixo com
as vozes: as intensidades dos ataques formam a célula de entrada; três taps
ponderados `(0, 0,50)`, `(k₁, 0,32)` e `(k₂, 0,18)` circulam por
`R[x]/(xᴺ−1)`. A seed e o ciclo determinam `k₁` e `k₂`; os pesos coincidentes
se acumulam, todo passo alcançado por ao menos um tap gera um ataque, e a soma
normalizada pelo pico modula sua intensidade. A fonte de maior contribuição
fornece posição e intensidade para calcular a altura.
O modo está disponível na página web e no JUCE. É uma regra de composição
discreta e limitada, não uma simulação física nem uma garantia de consonância.

Se “anneau” designar a figura geométrica, o [teorema de Mamikon para anéis
ovais](https://www.cco.caltech.edu/~mamikon/VisualCalc.html) traz outro
experimento: um segmento tangente de comprimento fixo `L`, com uma extremidade
no ponto de tangência, movendo-se ao longo de uma curva convexa fechada suave,
varre área `πL²`, independente da forma dessa curva. Se `a` designa uma corda
simétrica inteira, tangente no meio, então `L=a/2` e a mesma área escreve-se
`πa²/4`. A hipótese requer tangência e comprimento invariantes; não vale para
qualquer deformação arbitrária. No Batruc, poderia comparar trajetórias com a
mesma área varrida e contornos diferentes, sem confundir essa área invariante
com intensidade ou energia sonora.

### Bilhar, espaço e acústica

O primeiro caso agora implementado nas versões web e JUCE é um bilhar quadrado
periódico. Um ponto percorre o interior e reflete pela lei especular
`v′=v−2(v·n)n`; a trajetória, as seis colisões por ciclo e o ponto móvel são
animados. Com “usar y do desenho” ativo, as colisões agendam eventos da voz 1,
e o filtro espacial consulta as coordenadas reais `(x,y)` do bilhar. Assim, a
geometria participa do cálculo composicional em vez de ser apenas decorativa.
Este é um primeiro caso fechado; ainda não há editor de polígonos nem controles
de velocidade contínua, direção ou material. Há agora sete razões discretas
de voltas no espaço desdobrado (`2:4`, `4:2`, `2:6`, `6:2`, `4:4`, `4:6`,
`6:4`), todas com contagens pares para fechar também a direção após o ciclo.
Cada escolha altera declive, ordem e quantidade de colisões (seis a dez), e
fica salva na cena JUCE e no cálculo local da página.

Esse módulo ilustra um princípio mais amplo da matemática generativa do
Batruc: cada conceito deve tornar-se um operador explícito sobre relações
musicais. Recorrências podem transformar uma célula em outra; combinatória e
aritmética modular podem distribuir eventos e acentos; transformações de fase
podem deslocar vozes; convolução cíclica pode recombinar células; Fourier pode
construir trajetórias e espectros; sistemas diferenciais podem produzir
movimento com memória. Cada operador precisa declarar seus dados de entrada,
regra, limites e saídas (altura, tempo, duração, intensidade, timbre ou
posição), permitir recombinação e manter a seed reproduzível. Uma fórmula
isolada ou figura animada só é composicional quando altera eventos ou som de
modo observável.

O bilhar é um modelo geométrico de raios; não representa, por si só, a
propagação completa de ondas numa sala.
Para tornar o espaço audível, uma etapa acústica separada pode modelar
reflexões iniciais com atraso e ganho dependentes da distância e da absorção
das paredes, e então uma cauda reverberante controlada por difusão. Uma etapa
posterior de modos próprios exigiria resolver a equação de onda/Helmholtz para
o contorno e as condições de fronteira. Polígonos com vértices e curvas
Fourier podem servir de contorno, desde que a simulação detecte interseções e
trate trajetórias que atinjam vértices como casos singulares.

### Repertório já existente na família RASGO

Antes de portar componentes para o batruc, consultar as fontes originais:
[RasgarAccentField](../RASGO_SYNTH_STUDIO/rasgo-synth-core/src/engine/RasgarAccentField.hpp)
separa atributos de acento e traduz para velocidade, duração, microaltura e
timbre dentro de limites; [GrooveDialogue](../RASGO_SYNTH_STUDIO/rasgo-synth-core/src/engine/GrooveDialogue.hpp)
e [TemporalGrooveImprint](../RASGO_SYNTH_STUDIO/rasgo-synth-core/src/engine/TemporalGrooveImprint.hpp)
relacionam padrões, acentos, durações e deslocamentos; [GrooveRepeat](../RASGO_SYNTH_STUDIO/rasgo-synth-core/src/engine/GrooveRepeat.hpp)
faz subdivisões limitadas com decrescendo; [DurationTarget](../RASGO_SYNTH_STUDIO/rasgo-synth-core/src/engine/DurationTarget.hpp)
faz ajuste limitado de duração antes do render. O estudo de
[groove e swing do Studio](../RASGO_SYNTH_STUDIO/docs/GROOVE_RHYTHM.md)
alerta que groove não é jitter uniforme e que padrões culturais exigem pesquisa
e escuta próprias. São referências internas; não são dependências de build nem
aprovação desses mapeamentos para o batruc.

### Pista do autor: metrônomos circulares e bateria — 2026-10-10

O autor encontrou na internet diversos metrônomos circulares com separação
das peças da bateria e estudos de polirritmia avançada. Esta é uma pista
relatada pelo autor: nomes, URLs, funcionamento, código e licenças dos exemplos
ainda não foram fornecidos ou verificados. Não atribuir a esses exemplos
capacidades ou direitos de reutilização sem consulta às fontes.

Potencial a investigar: representar vozes como bumbo, caixa, chimbal e
percussões em círculos ou anéis, tornando observáveis subdivisões, acentos,
pausas, deslocamentos de fase e pontos de encontro. Distinguir diferentes
subdivisões sobre um ciclo comum de ciclos com durações diferentes; a imagem
circular, sozinha, não define polirritmia, polimetria ou independência temporal.
Patterning é uma referência visual interessante para observar anéis por voz e
loops independentes. A decisão do autor é desenvolver código e interação
próprios para o Batruc: não copiar código proprietário ou sem licença livre.
Uma fonte de código livre só poderá ser adaptada após identificar e fixar a
versão, auditar licença, arquivos, dependências e proveniência, e reconstruir
a expressão musical segundo a identidade do Batruc.

No batruc, investigar a passagem de visualização/estudo para composição:
o gesto geométrico altera relações entre vozes, articulação, timbre e forma;
a geração propõe transformações com memória e possibilidade de intervenção.
Não copiar a interface de um metrônomo nem reduzir o instrumento a cliques.
O fluxo visual do gráfico representa o estado musical; o controle `Movimento
V2` agenda mudanças rítmicas da segunda voz no compasso seguinte.

Próxima comparação: localizar os exemplos do autor e observar o que é somente
representação, o que pode ser editado/tocado e quais transformações têm
consequência sonora. Registrar fontes e licenças antes de qualquer adaptação.

## Consulta ao Atlas RASGO — 2026-10-10

Pesquisa local no [Atlas](<../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md>),
no [inventário de fontes](../../RASGO_DOCUMENTATION/architecture/INVENTARIO_FONTES_ATLAS.md)
e no [inventário global de módulos](../../RASGO_DOCUMENTATION/architecture/INVENTARIO_GLOBAL_MODULOS.md).
O registro abaixo distingue referências, implementações nos instrumentos de
origem e hipóteses para o batruc. Não houve incorporação de código,
auditoria completa de licenças, compilação ou teste musical nesta consulta.

| Referência no Atlas | Potencial a investigar no batruc |
|---|---|
| Grids, §45 | Navegação em campos de densidade e padrões; a construção geométrica pode propor uma posição ou transformação, sem importar os mapas do Grids. |
| Stages / Tides, §§41 e 44 | Segmentos e ciclos deformáveis; investigar aceleração, desaceleração e evolução de percursos. |
| OpenMusic / Chaos / Constraints, §§53–55 | Estruturas musicais, regras, restrições e sistemas dinâmicos; transformação generativa além de randomizadores independentes. |
| ossia score, §99 | Estados, eventos, condições e processos para organizar entradas, transformações, pausas e retornos. |
| ChucK, §69 | Tempo como parte da estrutura de execução; referência de método, não adoção de outra linguagem no core C++. |
| OMax, §63 | Memória performativa, reconhecimento e recombinação de motivos, respostas e retornos transformados. |
| Separação tempo/sequência, §119.1 | Relógio, condição de disparo e organização dos acontecimentos como responsabilidades distintas. |

O [aprofundamento de ossia e OpenMusic](../../RASGO_DOCUMENTATION/architecture/CNMAT_OSSIA_FREESOUND_POTENCIALIDADES.md)
é fonte complementar para cenários e representação musical. Nenhuma dessas
referências foi adotada como dependência.

### Código próprio existente a estudar

- Rasgo Modular: [EuclidClock](../RASGO_MODULAR/src/dsp/EuclidClock.hpp),
  [Logic](../RASGO_MODULAR/src/dsp/Logic.hpp),
  [TrigSeq](../RASGO_MODULAR/src/dsp/TrigSeq.hpp),
  [Abacus](../RASGO_MODULAR/src/dsp/Abacus.hpp) e
  [Drift](../RASGO_MODULAR/src/dsp/Drift.hpp). O inventário registra distribuição
  euclidiana, divisões/multiplicações, lógica de eventos, padrões multipista,
  aritmética e deriva com memória. EuclidClock teve consulta parcial da fonte;
  os demais foram localizados pelo inventário, não revalidados em execução.
- Antitotem: [SimpleSequencer](../ANTITOTEM/src/core/SimpleSequencer.cpp),
  registrado no Atlas §2.1 como fonte de swing/groove, subdivisões e ratchets.
  A sobreposição de sequenciadores e um ratchet ternário não demonstram,
  por si sós, coordenação polirrítmica 3:2 ou 5:4.
- Trioio: [RhythmField](../TRIOIO/src/core/RhythmField.cpp) e
  [OracleMemory](../TRIOIO/src/core/OracleMemory.cpp), registrados nas
  atualizações do Atlas de 8 out. 2026. A consulta parcial confirmou o campo
  densidade/síncope e a construção de memória recombinatória. Esses códigos
  usam estruturas dinâmicas; não tratá-los como prontos para um callback de
  áudio. Uma adaptação exige arquitetura, limites e validação próprios.

Consulta não autoriza copiar automaticamente esses módulos. O próximo estudo
de fonte deve fixar commit, arquivos, licença, créditos e dependências antes
de decidir entre adaptação de código e implementação independente. Os
instrumentos de origem permanecem intactos e não serão dependências de build.

### Tempo, identidade e lacunas

A [Arquitetura Temporal RASGO](../../RASGO_DOCUMENTATION/GOVERNANCA_E_TRANSVERSALIDADE.md#8-vocabulário-temporal-transversal)
distingue tempo cronológico/percebido e grade física/pulso percebido. Suas
camadas — tempo estrutural, grade, performance, evento, relação e história —
podem orientar o projeto sem impor um motor comum. Entropia, tensão e memória
são dimensões de investigação, não controles já implementados no batruc.

Na consulta ao Atlas principal não foi localizado um núcleo dedicado à
construção musical com compasso de desenho, régua, esquadros e cálculo
geométrico. IanniX, RTG e as ferramentas geométricas listadas acima pertencem
à sondagem local do batruc; não foram encontrados como entradas desse
Atlas. A relação entre construção, percurso e consequência musical ainda
precisa ser especificada e testada; essa lacuna não prova novidade absoluta.

Há também uma divergência documental a verificar antes de eventual uso do
Grids: a atualização do Trioio no Atlas indica MIT, enquanto o inventário de
fontes distingue firmware AVR/GPL-3.0 de STM32F/MIT. Não atribuir uma licença
única à família nem resolver essa divergência sem conferir arquivos e versão.

Direção de investigação: aproveitar o conhecimento temporal e composicional
já existente, mas fazer o gesto geométrico produzir relações audíveis entre
vozes, com desenvolvimento e memória. Ainda é hipótese de projeto, não uma
conquista musical validada. Próximo passo: a situação curta descrita em
[TAREFAS.md](TAREFAS.md), antes de escolher componentes ou ampliar a interface.

## Condições de reutilização e licença

A sondagem local distingue licenças indicadas de arquivos efetivamente
consultados. Antes de incorporar material, fixar versão e auditar código,
dependências e assets. Fontes e conteúdos externos mantêm seus próprios
direitos; a licença deste projeto não os relicencia.

O projeto autoral é licenciado sob GNU Affero General Public License,
versão 3 ou, à escolha do usuário, qualquer versão posterior, sem garantia.
SPDX: `AGPL-3.0-or-later`. Copyright © 2026 Lúcio de Araújo.
O texto padrão FSF de LICENSE foi copiado literalmente do arquivo local
`TRIOIO/LICENSE`; isso não incorpora código ou conteúdo musical do Trioio.

O [licenciamento oficial do JUCE](https://github.com/juce-framework/JUCE/blob/master/LICENSE.md)
oferece AGPLv3 ou licença comercial. A direção é usar a opção AGPL; a versão
efetivamente adotada deverá ter licença e dependências documentadas. Nenhum
JUCE foi baixado ou incorporado nesta etapa.

## Governança, acesso e próximos passos

Criação, pesquisa e desenvolvimento são a finalidade. Documentação e arquivo
acompanham as materializações de forma proporcional, preservando autoria,
proveniência e versões; não substituem o fazer musical.

Este projeto segue [as instruções RASGO](../../AGENTS.md), a
[governança transversal](../../RASGO_DOCUMENTATION/GOVERNANCA_E_TRANSVERSALIDADE.md)
e a governança do [VM_STUDIO_ARCHIVE](../../../VM_STUDIO_ARCHIVE.md).
Antes de qualquer build, render ou tarefa controlada, aplicar o
[procedimento de segurança de recursos](../../../PROCEDIMENTO_SEGURANCA_RECURSOS.md).
Registrar objetivo, mudanças, validações, limitações e próximo passo na
documentação local; checksums e preservação congelada só quando proporcionais
ao marco. Não alterar outros instrumentos para desenvolver o batruc.

Em 25/10/2026, o autor autorizou o primeiro commit da fonte Batruc no
repositório central privado `lucioaraujo/rasgo-instruments`. O commit
`b663d66` foi enviado à branch `batruc/initial`; a branch `main` não foi
alterada. Essa decisão versiona o estado de incubação para continuidade e
colaboração; não autoriza divulgação pública, distribuição de binários ou
publicação de áudio. A disponibilidade jurídica do nome não foi confirmada.
Builds, brainstorm, gravações e outros derivados locais continuam fora do
controle de versão.

Em 25/10/2026 foi acrescentado ao baixo um seletor de perfil espectral
equivalente na maquete web e no JUCE: três componentes senoidais com razões
harmônicas `1:2:3`, ímpares `1:3:5`, inarmônicas `1:√2:√3` ou batimento lento
`1:1,003:0,997`. DaisySP, Faust e Pure Data foram avaliados como tecnologias
livres, mas nenhuma delas entrou como dependência. As fontes, licenças e
próximos experimentos estão em [BIBLIOGRAFIA_DE_ESTUDO.md](BIBLIOGRAFIA_DE_ESTUDO.md);
a alteração e a limitação de validação auditiva estão em [TAREFAS.md](TAREFAS.md).

Próximo foco: ouvir a maquete com os controles expressivos, registrar quais
relações são úteis ao autor e especificar uma linguagem de fórmulas executáveis
para o core C++/JUCE, com custo e saídas limitados. O Caustic 8BitSynth e as
referências internas da família RASGO estão registrados acima e em
[TAREFAS.md](TAREFAS.md).

Incremento de paridade HTML/JUCE: o JUCE agora expõe swing global de 50–67%.
Ele desloca os ataques das vozes no escalonador e os respectivos marcadores no
gráfico; o baixo combina esse valor com o ajuste local já existente. Mudanças
entram na próxima barra e são guardadas em cenas esquema 14. Cenas
anteriores continuam sendo lidas como swing reto. Essa implementação ainda
precisa de validação manual de escuta e resposta visual.

O JUCE agora também oferece as quatro levadas presentes na maquete: reta,
ternária, aditiva e de contratempo. Elas alteram a intensidade dos eventos das
vozes e do baixo e o tamanho dos marcadores no gráfico; as cenas esquema 15 as
preservam, e os esquemas anteriores abrem em levada reta. **Atualização de
25/10/2026:** o JUCE agora permite
selecionar 3/4, 4/4, 5/4 e 7/8. O compasso altera a duração das quatro barras da
seção, a malha de swing e os acentos de groove das vozes e do baixo; a grade do
gráfico acompanha as barras e subdivisões. A seleção é salva em cenas esquema
17; cenas anteriores abrem em 4/4. Em seguida, o editor do baixo passou a exibir
12, 16, 20 ou 14 passos e salvar 20 células no esquema 18, preservando a leitura
de cenas antigas com 16. A edição de pausas ainda é mais simples que no HTML e
falta validar visualmente e por escuta os quatro compassos e comparar as saídas.

O editor nativo do baixo agora oferece uma grade de até 20 posições, mostrando
12, 16, 20 ou 14 passos em 3/4, 4/4, 5/4 ou 7/8. O clique percorre célula vazia,
nota e pausa explícita; as cenas novas guardam 20 células no esquema 18 e as
anteriores com 16 continuam legíveis. A execução usa passos de semicolcheia e
limita o módulo a quatro barras. Ainda falta conferir o editor e a equivalência
por escuta com a página web. Para abrir o build local de Linux, use `./run` na
raiz do projeto; o script não recompila automaticamente.

As escolhas de intenção e dinâmica da maquete também chegaram ao JUCE:
construir/rarear/encontrar/contrastar e estável/crescendo/arco/ondas. As mesmas
curvas de expressão modulam os eventos das vozes e do baixo, e o gráfico indica
a intensidade pelo tamanho dos marcadores. O esquema de cena 16 registra essas
escolhas; cenas anteriores usam encontrar/arco. No JUCE, a curva reinicia a
cada seção de quatro compassos, enquanto a maquete percorre sua forma de sete
ciclos. As escolhas afetam o próximo bloco de áudio, sem esperar o fim da
seção. A compilação passou; falta conferir audição e resposta visual na
interface.

O estudo geométrico também inclui um bilhar ideal dentro de um quadrado. A
razão de voltas `x:y` escolhe entre sete percursos fechados, com seis a dez
colisões por ciclo; quando a fonte geométrica está ligada, elas se tornam
eventos da voz 1 e suas coordenadas conduzem a função. O gráfico anima o traço,
as paredes e os impactos. Isto é um operador de composição baseado em reflexão
especular, não uma reverberação: tamanho do recinto, absorção por frequência,
difusão e resposta impulsiva ainda precisam de um modelo acústico separado.
Rebuild e suíte JUCE passaram em 12/12; regressões nativas e web cobrem razões,
fechamento, reprodução determinística e efeito no calendário/áudio calculado.
Inspeção visual e escuta A/B continuam pendentes.

Para auditar localmente a recombinação entre ciclos do baixo com a mesma seed,
o render offline aceita `--recombine-bass`; combine com `--stems` para salvar
vozes e baixo separados. O JSON registra regra e offset doador. Em 01/11/2026,
três seeds mostraram stems e agendas de vozes idênticos entre controle e
candidato, enquanto o stem do baixo mudou. O baixo conservou 23, 98 e 96
ataques; respectivamente, mudaram 12/12/5, 49/67/9 e 49/68/19
alturas/intensidades/durações.
O render A/B está em `.local-build/offline-recombine-audit-20261101` (evidência
local, não publicada). Esses renders confirmam alteração do sinal e preservação
das vozes nessas três seeds, mas não substituem escuta humana ou inspeção do painel;
não declara validação musical.

O render C++ também aceita `--coupled-pendulum` para uma experiência offline:
RK4 calcula três pêndulos lineares de pequeno ângulo, acoplados por molas
efetivas. Os estados de V1–V3 geram ataques; a energia local de cada pêndulo,
incluindo sua parcela da energia das molas, modela os acentos, e
`sin(θ₁)`/`sin(θ₂)` passam à geração do baixo. Condições iniciais e
parâmetros físicos variam deterministicamente com a seed e são registrados no
JSON. Exemplo de saída já renderizada:
`.local-build/physics-pendulum-20261101/5EED2027-pendulum-transfer.wav`.
É uma receita exploratória do renderizador offline, ainda sem controle ou
gráfico na aplicação em tempo real; o modelo idealizado não reproduz o
pêndulo serial do artigo nem serve como medição acústica.

O renderizador offline aceita também `--voice-call-response`: seleciona
deterministicamente uma quota próxima de 42% das chamadas elegíveis de V1 na
forma completa de sete ciclos e as copia para V2 com atraso de uma semínima a
120 BPM. A seed escolhe quais chamadas respondem. Os ciclos em que V2 está
formalmente ausente não entram na quota; a resposta pode atravessar a fronteira
entre ciclos. Os ataques derivados aparecem no manifesto JSON e alimentam a
derivação do baixo. Sem a opção, a renderização mantém a agenda anterior. É um
protótipo offline, ainda sem controle na interface JUCE ao vivo. O A/B numérico
com seed `5EED2027` confirmou duas respostas em cinco chamadas elegíveis,
preservou as contagens das demais vozes e alterou os stems de vozes e baixo; a
escuta crítica continua pendente. Exemplo:
`./.local-build/native/linux-release-juce/batruc_offline_render 5EED2027 /tmp/batruc-call-response.wav --voice-call-response --stems`.

Para experimentar articulação matemática no baixo, o renderer aceita
`--logistic-bass`. O mapa logístico percorre os eventos graves em ordem de
tempo absoluto: a seed define o estado inicial, `r=3.82`, 12 iterações de
aquecimento e profundidade de acento `0.80` controlam a recorrência. O estado
de cada nota altera a intensidade, a abertura do filtro e o comprimento da
articulação (`0,28 + 0,72x` do gate configurado). Posições, alturas, durações
escritas e número de ataques permanecem iguais. O JSON registra os parâmetros,
o estado e o multiplicador de intensidade de cada ataque; a abertura e o gate
são derivados desse mesmo estado no renderizador. O A/B inicial, limitado à
intensidade, está em `.local-build/logistic-bass-20261102/iteration-02/`; o
A/B com timbre e articulação está em
`/tmp/batruc-logistic-articulation-control-20261102.wav` e
`/tmp/batruc-logistic-articulation-final-on-20261102.wav`. No JUCE,
`MAPA LOGÍSTICO` ativa a mesma transformação, desligada por padrão e salva
desde o schema 39. A recorrência ocorre uma vez ao preparar o plano, nunca no
callback. A faixa inferior do gráfico do percurso mostra os estados do plano
efetivamente publicado ao áudio. Para ouvir a transformação com clareza,
use **Solo** no stem do baixo durante a comparação com a mesma seed. Na mistura
completa, outras vozes podem mascarar a diferença. A escuta crítica continua
pendente. Exemplo do renderer:
`./.local-build/native/linux-release-juce/batruc_offline_render 5EED2027 /tmp/batruc-logistic-bass.wav --logistic-bass --stems`.

## Coesão e reaproveitamento — auditoria de 06/11/2026

A auditoria [AUDITORIA_COESAO_REAPROVEITAMENTO_2026-11-06.md](AUDITORIA_COESAO_REAPROVEITAMENTO_2026-11-06.md)
cruzou o core, a interface, o protótipo web, os testes e pesquisas dos
instrumentos irmãos. O diagnóstico é que o Batruc não precisa acumular mais
funções isoladas: precisa fazer células, operações, forma, gesto, timbre,
gráficos, cenas e áudio lerem um único documento de composição.
O adendo de **08/11/2026**, na seção 11, revê o fluxo musical e antecipa um
marco de escuta: um ritmo 4/4 de referência, funções V1–V8 audíveis, kit
coerente e variação reversível antes de ampliar a arquitetura.

Foram priorizados: ligar o algoritmo de próxima seed contrastante já existente;
remover definitivamente a UI inacessível de Áreas/Stengel; congelar o web como
protótipo histórico; substituir o Viewport empilhado por páginas reais; promover
`RhythmicCell`/Study/MIDI a biblioteca canônica de células com proveniência e
undo; e construir `PiecePlan` para ciclos totais, duração-alvo e seções. A
substituição proposta para Áreas é **Morfologia rítmica**, com rotação,
densidade, força, interlocking, escopo, aplicar/desfazer e comparação audível.
O primeiro gesto de densidade já está no menu **RITMO** do Study: reduz a
célula do stem selecionado para aproximadamente 75% ou 50% dos ataques,
priorizando velocities altas, mantendo um ataque no tempo zero quando existe e
preservando notas simultâneas como um grupo. A transformação é local à célula,
reversível por undo e não altera a fonte MIDI. O menu também pode intertravar a
célula selecionada com outra célula MIDI: onsets coincidentes são
levados ao ponto médio livre mais próximo no ciclo do parceiro, dentro de um
limite de deslocamento. A contagem e os grupos de notas simultâneas são
preservados, e o resultado pode ser desfeito. Se não houver intervalo livre
próximo, aquele ataque permanece no lugar. **Recombinar com Vx** seleciona metade
dos ataques de cada célula, escolhida pela seed, e combina os tempos resultantes
na voz selecionada. Ataques coincidentes se fundem; os tempos do parceiro usam
as notas e a matéria sonora da voz selecionada, com intensidade reduzida. A
fonte MIDI permanece intacta e a operação entra no undo/redo. Recombinação de
frases ou compassos inteiros continua pendente.
O submenu **Desenho de intensidade** também aplica crescendo, decrescendo ou
acentos alternados somente ao stem selecionado, preservando tempos e notas.

A primeira recuperação já foi aplicada: **Nova seed** no host JUCE passou a
usar `algorithmicNextSeed()` e, portanto, procura uma realização musicalmente
contrastante em relação à atual. A inicialização do aplicativo continua
aleatória; uma ação futura poderá expor separadamente “nova aleatória”. O teste
de receita seeded confirma determinismo e contraste da proposta seguinte.

Em 07/11, a receita seeded foi ampliada sem alterar os índices legados de
ciclo, fase, regra e padrão. A lei A agora pode escolher qualquer uma das oito
leis. Cada seed gera groove, compasso, intenção, dinâmica, profundidade de
variação, movimento cíclico de V2, fonte e forma geométricas, acoplamentos,
fases e transformações por stem. A máscara começa sempre em **Campo livre**.
A seed aplica sua família sonora no Mixer, e o músico pode editar os
timbres depois. O baixo e os parâmetros Chip 8-bit continuam sob controle do
músico. Reaplicar a mesma seed reconstrói os valores gerados e limpa células
Study editadas manualmente; com MIDI importado, recalcula as células desde a
fonte preservada. Regras locais também voltam aos genes da seed. Controles de
execução/mixagem — como BPM, swing, pan, mute/solo e ganhos — permanecem decisões
do músico. O seletor de família fica visível no Mixer; desde o schema 48 a cena preserva
a família, a fonte MIDI, seu SHA-256, os campos declarados de direitos e a nota
GM por ataque. O MIDI informa eventos; não fornece samples. O renderer ainda
sintetiza as articulações e não foi validado por escuta.

A segunda recuperação removeu definitivamente da interface o estudo de
Áreas/Stengel que permanecia oculto: componente gráfico, knobs, seletor e
layout legado saíram de `Main.cpp`. O cálculo matemático puro permanece no
core e nos testes como pesquisa histórica, sem disputar espaço ou sugerir uma
função musical inexistente.

Em 06/11, a navegação começou a expressar o fluxo **Compor → Estudar →
Tocar**: “Editor” passou a “Compor” e “Mixer” a “Tocar”; Study continua como
modo de edição dentro do campo geométrico. É uma primeira orientação, não três
documentos musicais separados. O objetivo é que célula, fonte, transformação,
cálculo, forma e realização sonora continuem sendo o mesmo material ao mudar
de atividade.

As leis de intensidade agora podem ser combinadas. A e B recebem as mesmas
coordenadas e ciclos; o peso `b` mistura suas saídas, e `λ` controla a
profundidade geral da lei. A combinação afeta os ataques do renderer e a força
visual dos pontos no campo. **A/B** alterna entre A isolada e a mistura sem
regenerar eventos, fase ou seed. Cenas schema 42 guardavam A, B e `b`; schema 43
acrescenta a família sonora, schema 44 os dados da fonte MIDI, schema 45 seu
SHA-256/estado de direitos, schema 46 licença/atribuição declaradas, schema 47
versão declarada da fonte/histórico de operações, schema 48 notas GM por
ataque e schema 49 seed e nível de variação independentes do baixo. Schema 52
registra CC4 por ataque; schema 53 acrescenta chocalho, agogô e bloco de madeira,
e schema 54 preserva várias notas simultâneas no mesmo stem
sem renumerar os modelos anteriores. Cenas anteriores seguem pelo caminho
legado.
vista de intensidade desenha A, B e a mistura com as equações ativas;
a seleção de ataque também marca e informa o multiplicador de campo aplicado.
Ainda falta um histórico de operações/desfazer mais amplo que o toggle de
comparação.

Os outros instrumentos são referências e fontes possíveis de cópia fixada,
nunca dependências: Antitotem contribui memória e transformação em camadas;
Aquorbium, separação entre geometria e eventos; Modular, vocabulário explícito
de operações; Studio, identidade/forma/duração; Trioio, escuta e memória de
escolhas. Cada adaptação futura exige origem, commit, licença, créditos e testes
próprios no Batruc.

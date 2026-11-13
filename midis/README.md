# Batruc — corpus MIDI local

Esta pasta reúne fontes MIDI usadas na pesquisa e no desenvolvimento do
Batruc. Ela não é, por si só, um diretório de assets publicáveis: cada corpus,
célula ou derivado precisa conservar origem, versão, licença, atribuição,
recorte e transformações antes de entrar numa cena, pacote ou release.

## Fonte presente

- `groove-v1.0.0-midionly.zip` — pacote MIDI-only oficial do
  [Groove MIDI Dataset](https://magenta.tensorflow.org/datasets/groove), versão
  1.0.0, licença CC BY 4.0 declarada pela fonte;
- SHA-256:
  `651cbc524ffb891be1a3e46d89dc82a1cecb09a57c748c7b45b844c4841dcc1e`;
- estado: fonte externa local, preservada no ZIP original; não é necessária
  para executar o instrumento e não foi incorporada ao binário.

A pasta `groove/` é a cópia extraída desse ZIP. Em 08/11/2026, comparei todos
os 1.189 arquivos regulares: caminhos e bytes coincidem integralmente; a
verificação CRC do ZIP também passou. O ZIP original permanece preservado. A
A cópia extraída aparece no catálogo local com filtros por estilo editorial,
beat/fill, compasso, faixa de BPM e densidade (ataques no primeiro compasso);
a correspondência com
`info.csv` também preenche a prévia com estilo, BPM, tipo beat/fill, compasso e
divisão. Cenas schema 55 guardam esses dados separados da licença/atribuição,
e o resumo da fonte os recupera ao reabrir a cena.
Arquivos fora do catálogo não recebem esses rótulos; a associação local é pelo
caminho e não certifica que o MIDI não foi modificado. O SHA-256 da cena
identifica os bytes importados, mas ainda não é comparado com um inventário
checksum do GMD.

Na composição, rótulos conhecidos do `info.csv` orientam a família sonora e o
groove iniciais quando uma seed é aplicada; os ataques preservados do MIDI não
são deslocados por essa escolha. A regra é um mapa editorial conservador para
rock, jazz, funk, samba, baião e a família `latin/`, não uma classificação
musical validada nem uma afirmação de autenticidade. Rótulos sem regra deixam a
seleção da seed prevalecer; o músico pode editar família e groove. A importação
aplica compassos 3/4, 4/4, 5/4 ou 7/8 que o Batruc oferece. Em outras métricas,
mantém o compasso de trabalho atual e informa essa diferença no tooltip.
Ao gerar outra seed, o Batruc também mantém o limite de vozes acima da lane MIDI
mais alta que tenha ataques; assim a seed não silencia por contagem V5–V8.

### Recorte brasileiro já presente no GMD

A conferência local de `groove/info.csv` identificou 57 entradas cujo rótulo
de estilo contém `brazilian`; todos os 57 caminhos MIDI listados existem na
cópia extraída. Distribuição: 32 baião, 5 samba, 5 songo, 3 ijexá, 3
samba-reggae, 2 bossa, 2 maracatu, 1 frevo e 4 `latin/brazilian` sem subtipo.
Há padrões `beat` e `fill`. São rótulos editoriais do GMD para performances
de bateria; não certificam autenticidade, contexto ou abrangência das
tradições. Servem como amostras para escuta e análise, não como definições de
gênero nem substituto de fontes contextualizadas.

Para localizar materiais no seletor **Importar MIDI**, estes arquivos `beat`
foram conferidos no `info.csv` e existem na cópia extraída:

| Rótulo GMD | BPM | Compasso | Arquivo relativo a `midis/groove/` |
| --- | ---: | --- | --- |
| `latin/brazilian-baiao` | 95 | 4/4 | `drummer1/session1/145_latin-brazilian-baiao_95_beat_4-4.mid` |
| `latin/brazilian-samba` | 89 | 4/4 | `drummer5/session1/3_latin-brazilian-samba_89_beat_4-4.mid` |
| `latin/brazilian-ijexa` | 108 | 4/4 | `drummer5/session2/19_latin-brazilian-ijexa_108_beat_4-4.mid` |
| `latin/brazilian-maracatu` | 96 | 4/4 | `drummer5/session1/6_latin-brazilian-maracatu_96_beat_4-4.mid` |
| `latin/brazilian-frevo` | 133 | 4/4 | `drummer5/session2/22_latin-brazilian-frevo_133_beat_4-4.mid` |

São performances de bateria incluídas no GMD, não transcrições completas das
formações instrumentais nem modelos de um toque ou grupo específico. Use-as
para observar ataques, dinâmica e distribuição entre peças; para estudar
tradições, acrescente fontes contextualizadas e compare versões sem declarar
uma célula como padrão universal. O GMD exige atribuição CC BY 4.0; registre
fonte e transformação em qualquer derivado.

Desde 07/11/2026, cenas Batruc schema 47 salvam a referência local da importação
(nome, caminho, SHA-256, janela em compassos e versão declarada), as oito
células-fonte agrupadas por lane, licença/atribuição declaradas e até 64
registros de operações. Os direitos continuam não verificados. Isso não
incorpora o ZIP ou o arquivo MIDI original, não guarda todos os metadados nem
as notas fora da janela importada (schema 48 preserva as notas das células) e
não substitui o manifesto detalhado de parâmetros por operação,
licença verificada e proveniência completa pendente em `TAREFAS.md`. O caminho é uma referência
local e pode deixar de existir se o arquivo for movido; as células guardadas na cena continuam
disponíveis para a seed mesmo assim. Para publicação ou redistribuição, manter
a fonte intacta e comprovar direitos/proveniência de cada material e derivado.

### Leitura musical e timbres no Batruc

O importador interpreta o canal MIDI 10 como percussão General MIDI e agrupa
os ataques pelo **número da nota**, não pela ordem das pistas do arquivo. O
[mapa GM de percussão](https://musescore.org/sites/musescore.org/files/2024-10/General%20MIDI%20Standard%20Percussion%20Set%20Key%20Map.pdf)
fundamenta a classificação em oito lanes internas: V1 bumbo (35–36), V2
caixa/aro (37–40), V3 chimbal fechado/pedal (42/44), V4 chimbal aberto (46),
V5 tons (41/43/45/47/48/50), V6 pratos (49/51–53/55/57/59), V7 tambores
de mão (60–66) e V8 acessórios (54/56/58/67–77) e demais notas. Os nomes são papéis
funcionais para organizar o Mixer; não pressupõem que a pista 1 de qualquer
MIDI seja bumbo. Cada ataque de uma lane preserva até quatro notas GM distintas
no mesmo instante, com velocity, pedal de chimbal e estado de síntese próprios.
Cópias repetidas da mesma nota se consolidam pela maior velocity. Uma quinta
nota distinta no mesmo instante, ou um ataque que exceda a capacidade da lane
ou o espaçamento mínimo, aparece como **PERDA MIDI**. O schema 54 da cena salva
as notas simultâneas tanto na referência da fonte quanto na célula editável; o
traço do renderizador offline registra cada nota no mesmo tempo musical.

**Correção Roland TD-11 — 08/11/2026.** O [mapa oficial do Groove MIDI
Dataset](https://magenta.withgoogle.com/datasets/groove) declara três notas
fora da interpretação GM usada antes: 22 = chimbal fechado de borda, 26 =
chimbal aberto de borda e 58 = aro do tom grave. O catálogo local `groove/`
e arquivos escolhidos dentro dessa pasta usam agora a opção TD-11 do
importador: 22 vai para V3, 26 para V4 e 58 para V5. Arquivos externos
continuam com o mapa GM. A nota original permanece na célula e na cena;
o renderer também a reconhece no stem correspondente. O importador lê CC4 no
canal de bateria, associa o último valor vigente ao ataque pelo tempo absoluto
(mesmo quando controlador e notas estão em pistas diferentes) e o renderer usa
esse valor para variar a articulação do chimbal. Arquivos sem CC4 continuam
usando as articulações das notas GM. Cenas schema 52 ou posteriores preservam
CC4 na fonte e nas células; schema 54 acrescenta as notas simultâneas e suas
articulações próprias. A associação foi validada por teste focal; ouvir um
arquivo real com CC4 no Batruc ainda depende de conferência auditiva.

### Estudo de referência: dois compassos de bateria 4/4

- Fonte GMD v1.0.0: `groove/drummer1/eval_session/8_rock-groove8_65_beat_4-4.mid`;
  `info.csv` declara `beat`, 4/4, 65 BPM e o rótulo editorial `rock/groove8`.
- SHA-256 do arquivo MIDI:
  `3dff4fa146063ac75d0f58d83f47f217efaf3e25cca8721ff0c2dfbd13a56c0e`.
  O [dataset oficial](https://magenta.withgoogle.com/datasets/groove) declara
  CC BY 4.0 por Google LLC; registrar atribuição e verificar os direitos de
  qualquer derivado antes de distribuição.
- Importação dos primeiros dois compassos com mapa TD-11: 30 ataques
  agendados, sendo 8 em V1 (bumbo), 5 em V2 (caixa), 16 em V3 (chimbal
  fechado) e 1 em V4 (chimbal aberto). Um ataque simultâneo foi fundido no
  mesmo stem; 195 notas do arquivo longo ficaram fora da janela. Os 30
  eventos constam em `/tmp/batruc-td11-reference-20261108-v3-trace.tsv`.
  O tempo MIDI lido no arquivo é 65 BPM, igual ao índice do catálogo.
- Renderização local: `/tmp/batruc-td11-reference-20261108-v3-mix.wav` e
  `-V1.wav` a `-V4.wav` com o mesmo ganho de saída. São artefatos de teste,
  não assets do instrumento. O mix tem pico digital aproximado de 0,778;
  todos os quatro stems isolados têm sinal. Não houve escuta humana nem teste
  de GUI nesta etapa.

Em 09/11, o renderizador de referência foi recompilado e o mesmo trecho
reprocessado em `/tmp/batruc-stem-audit-20261109-*`. O traço mantém V1=36
(bumbo), V2=38/40 (caixa), V3=22/42/44 (chimbal fechado), V4=26 (aberto):
não houve troca de lanes nesse caso. No áudio isolado, V1 concentra 99,9%
da energia medida em 35–250 Hz; V3 concentra 93,8% em 2–12 kHz. O RMS dos
trechos ativos de V3 foi aproximadamente 7 dB maior que o de V1, indício
de balanço a examinar por escuta, sem equivaler a uma medida de loudness
perceptivo. A hipótese de inversão inspirada por Gramani não se aplica a
esse caminho do app: a operação de troca de vozes está isolada em teste.

Comando de reprodução do diagnóstico, a partir da pasta Batruc, com outro
prefixo de saída para não sobrescrever os arquivos locais:

```bash
./.local-build/native/linux-release-juce/batruc_midi_reference_render \
  midis/groove/drummer1/eval_session/8_rock-groove8_65_beat_4-4.mid \
  65 /tmp/batruc-estudo-4-4
```

Este estudo não certifica o rótulo `rock`: ele testa função dos stems, ataque
e som produzido. O próximo passo é ouvir mix e stems no app, comparar com a
fonte e melhorar as peças sonoras que ainda não cumprem seu papel.

### Segundo recorte técnico: beat rotulado samba, dois compassos

- Fonte GMD v1.0.0:
  `groove/drummer5/session1/3_latin-brazilian-samba_89_beat_4-4.mid`;
  `info.csv` declara `beat`, 4/4, 89 BPM e o rótulo editorial
  `latin/brazilian-samba`. SHA-256 do MIDI:
  `8031a990b4bc02b242fbfde370ff75c462e3b6945298088e1db16ffe6e3e78e7`.
  A proveniência e a licença do corpus são as indicadas acima; este recorte
  permanece apenas no estudo local.
- Janela inicial de dois compassos, mapa Roland TD-11: 63 notas preservadas,
  sendo V1=17 (nota 36), V2=34 (33×38 e 1×37), V3=8 (nota 44) e V5=4
  (nota 43). O traço do plano está em
  `/tmp/batruc-samba-gmd-two-bars-20261110-v3-trace.tsv` e o mix e stems
  isolados em arquivos WAV com o mesmo prefixo. Não houve colisão de notas
  nem perda dentro da janela; 2261 ataques pertencem ao restante do MIDI
  longo e ficaram fora do recorte por escolha.
- A versão anterior da célula, limitada a 32 ataques por stem, descartava
  duas notas do V2 nesse recorte. A capacidade interna passou a 64; o
  controle manual de 1–32 toques permanece como gesto distinto. O teste de
  importação cobre 65 ataques numa lane e exige que só o 65º seja recusado.
- Todos os quatro stems presentes geraram áudio não nulo. No renderizador
  offline, o RMS de V2 no trecho completo foi −30,13 dBFS, cerca de 9 dB
  acima do V1 (−39,18 dBFS); a contagem de ataques também é duas vezes maior.
  Essa medida não equivale ao balanço final do app, que tem faders e master
  próprios. Ela identifica uma comparação de nível e síntese para a escuta.

O rótulo do dataset não prova autenticidade de samba. O ensaio confirma o
caminho MIDI → células → agenda → áudio deste recorte; a identidade musical
e o balanço perceptivo ainda exigem escuta do autor no instrumento.

### Comparação A/B de famílias com a mesma célula

O renderizador local aceita `--family` (índice −1 a 16), `--gain` (ganho de
saída fixo de 0,01 a 1) e `--mix-only`. Com uma família indicada, o padrão é
ganho fixo de 0,7; sem família, o comando antigo mantém normalização por
pico. Para comparar famílias, usar o mesmo arquivo, BPM e ganho, mudando
somente o índice e o prefixo de saída. Exemplo, a partir da pasta Batruc:

```bash
./.local-build/native/linux-release-juce/batruc_midi_reference_render \
  midis/groove/drummer1/eval_session/8_rock-groove8_65_beat_4-4.mid \
  65 /tmp/batruc-ab-rock --family 0 --gain 0.7 --mix-only
./.local-build/native/linux-release-juce/batruc_midi_reference_render \
  midis/groove/drummer1/eval_session/8_rock-groove8_65_beat_4-4.mid \
  65 /tmp/batruc-ab-8bit --family 8 --gain 0.7 --mix-only
```

Em 11/11/2026, o par Rock/8-bit sobre esse recorte teve os mesmos 30
eventos, RMS do mix −45,96/−46,63 dBFS e zero amostras cortadas. Num segundo
beat GMD rotulado samba, os 63 eventos também coincidiram e o RMS foi
−45,70/−46,37 dBFS. A medição motivou corrigir uma queda anterior de quase
10 dB no processamento da família 8-bit. Esses números são RMS da janela
inteira do renderizador de referência: não igualam loudness percebido nem
representam os faders e o master do app. A escuta musical permanece pendente.

### Comparação A/B de seeds sobre MIDI importado

O mesmo renderizador aceita `--seed` com oito dígitos hexadecimais e
`--variation` de 0 a 1. Sem `--seed`, mantém o comportamento original. Com
seed, aplica `seededMidiPhraseCells()` à cópia em memória das células: o arquivo
MIDI-fonte não é modificado; as lanes geradas e o traço registram as contagens
antes/depois e os ataques agendados. Use o mesmo BPM, família e ganho e mude
somente a seed para comparar a frase. A prévia isola a variação rítmica e de
velocity; não aplica o perfil estéreo seedado do Mixer nem substitui a escuta no
app.

Exemplo para os dois beats acima, com a família Samba (índice 12), ganho fixo e
todas as lanes presentes:

```bash
./.local-build/native/linux-release-juce/batruc_midi_reference_render \
  midis/groove/drummer5/session1/3_latin-brazilian-samba_89_beat_4-4.mid \
  89 /tmp/batruc-samba-seed-a --family 12 --gain 0.7 \
  --seed 5EED2027 --variation 1
./.local-build/native/linux-release-juce/batruc_midi_reference_render \
  midis/groove/drummer5/session1/3_latin-brazilian-samba_89_beat_4-4.mid \
  89 /tmp/batruc-samba-seed-b --family 12 --gain 0.7 \
  --seed 5EED2028 --variation 1
```

Em 12/11/2026 gerei pares técnicos para samba (89 BPM) e baião (95 BPM) em
`/tmp/batruc-midi-seed-audition-verified-20261112/`. Nos dois pares, o diff do
traço encontrou ataques/velocities distintos entre seeds; no samba, a lane de
tons passou de quatro ataques-fonte para cinco em ambas as realizações, mas a
posição escolhida diferiu. No baião, contagens por lane coincidiram, enquanto
ataques de chimbal foram rarefeitos/repostos em posições diferentes. Isso
confirma variação determinística no material local, não sua qualidade musical;
uma repetição do samba com a seed `5EED2027` produziu mix WAV e TSV bit a bit
idênticos. Os WAVs ainda precisam de escuta comparativa do autor.

O importador lê o mapa de tempo absoluto em todas as pistas, inclusive quando
fica numa pista posterior à bateria em MIDI formato 1. Integra os intervalos de
ticks com o microssegundo por semínima vigente; os ataques passam a ocupar
posições relativas ao tempo decorrido no recorte, mantendo a forma do mapa de
andamento. O BPM equivalente do recorte ajusta a duração do ciclo se estiver na
faixa do controle (20–220 BPM); depois o músico pode alterar a duração geral sem
apagar a relação temporal importada. Sem metaevento de tempo, o BPM atual
permanece e os ataques mantêm suas posições métricas. O recorte continua
limitado à janela de 1, 2 ou 4 compassos escolhida.

A assinatura inicial de compasso também é lida pelo tempo absoluto entre todas
as pistas formato 1, não pela ordem em que as pistas aparecem no arquivo. Só é
aplicada quando ocorre até o primeiro ataque de bateria; uma assinatura que
começa depois dele não redefine retroativamente a janela inicial. Um compasso
escolhido manualmente no diálogo de importação tem prioridade. Mudanças de
compasso após o primeiro ataque ainda não são reproduzidas.

Desde a correção de 08/11/2026, a nota MIDI preservada no schema 48 seleciona
o material sintetizado de cada ataque dentro da lane, em vez de apenas mudar
parâmetros de excitação enquanto o modelo fixo do canal continua tocando.
As 17 famílias do Mixer mantêm a função da lane e variam ataque/corpo,
ruído, espectro e cauda. O teste de core compara aro (37) e caixa (38) no
mesmo V2 e passou; a percepção auditiva ainda deve ser confirmada no app.
Há limites: V5 não diferencia a altura dos tons por nota; V6 ainda usa um
modelo de prato comum para crash e ride; V7/V8 precisam de peças mais
específicas. O código atual desativa a redução formal de vozes quando existe
fonte MIDI, para manter todos os stems ativos desde C1. Falta confirmar esse
comportamento por escuta no aplicativo, com solo de V2–V8, conforme
[TAREFAS.md](../TAREFAS.md).

## Pistas acadêmicas brasileiras verificadas em 08/11/2026

- **CInBalada — Centro de Informática da UFPE:** dissertação e artigo descrevem
  uma base de cerca de duzentos padrões MIDI de percussão, com categorias de
  instrumento, métrica, estilo e papel. Samba e maracatu aparecem entre as
  referências; muitos padrões foram transcritos manualmente de partituras.
  A dissertação está no [repositório UFPE](https://repositorio.ufpe.br/bitstream/123456789/2596/1/arquivo5181_1.pdf)
  e o artigo em [acesso aberto](https://doi.org/10.1007/BF03192563). O download
  da coleção MIDI não foi localizado; o antigo URL de exemplos mencionado no
  artigo não abriu nesta verificação. A licença do artigo não deve ser
  presumida como licença dos arquivos da base.
- **BRID — UFRJ, UFAM e colaboradores:** [Zenodo](https://zenodo.org/records/14051323)
  disponibiliza gravações e anotações de ritmos brasileiros, não uma coleção
  de MIDI. É candidato a referência de escuta/análise; verificar as condições
  do registro antes de criar derivados.
- **Banco de Partituras Brasileiras — Funarte:**
  [partiturasbrasileiras.funarte.gov.br](https://partiturasbrasileiras.funarte.gov.br/)
  oferece partituras PDF de acordo com as autorizações de cada obra. Pode
  orientar estudo/transcrição, mas não é fonte MIDI direta.

Essas fontes não foram baixadas nem incorporadas. O GMD continua sendo o único
corpus MIDI de pesquisa atualmente preservado nesta pasta; a busca de um
repositório universitário baixável e licenciado de padrões brasileiros
permanece aberta.

## Materiais que permanecem externos

O conjunto `<disco-local>/AUDIOS_RASGO/Zoutros/drumsmidi` foi
inspecionado em 06/11/2026 e permanece no local de origem. Ele mistura
transcrições de músicas conhecidas, padrões genéricos sem licença comprovada,
duplicatas e pacotes antigos de software/bancos proprietários. Nada desse
conjunto foi copiado para esta pasta.

- arquivos com nomes de músicas ou artistas: somente referência privada; não
  incorporar nem redistribuir;
- padrões genéricos como `44rock03.mid`, `44swing5.mid`, `54groov2.mid` e
  semelhantes: candidatos a teste local do importador, mas não a corpus do
  Batruc até que autoria e licença sejam comprovadas;
- `00001.mid` a `00006.mid`: contêm aviso interno de copyright de Jim Dooley;
- bancos Battery/Absynth: não usar como assets ou dependências sem licença
  explícita; pacotes identificados como crack/password removal não devem ser
  executados, incorporados ou distribuídos.

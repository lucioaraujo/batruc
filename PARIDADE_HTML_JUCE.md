# Paridade integral — HTML e JUCE

Este documento acompanha a paridade do Batruc como instrumento completo. A
paridade significa que cada versão oferece a mesma capacidade composicional,
controles equivalentes, persistência compatível e resposta musical coerente;
não basta que haja um controle com nome parecido.

### Estado das relações ecológicas entre vozes — 29/10/2026

O HTML já permite escolher relações independente, apoio, imitação,
afastamento e competição em V2–V12. A escolha é serializada na composição e
entra na geração de eventos antes da derivação do baixo; o acoplamento λ
controla força, com memória de ½ ciclo e atraso de imitação de ¹⁄₁₆ de ciclo.
V1 permanece independente como raiz da cadeia.

No JUCE, o seletor de relação agora oferece também apoio ecológico, imitação,
afastamento e competição. Para o baixo gerado pelo seed, o plano das vozes é
transformado no preparo da composição, fora do callback de áudio; os valores
λ por voz são usados como forças. A nova performance de baixo é publicada pelo
handoff existente e entra no próximo compasso, sem substituir dados enquanto o
callback os lê. Há teste que confirma o adiamento até essa fronteira.

A granularidade ainda difere: no JUCE a relação é selecionada globalmente para
a cadeia do baixo, enquanto no HTML cada voz pode ter sua própria relação. No
JUCE, os modos ecológicos não transformam uma linha de baixo manual. O cálculo
numérico tem fixture C++/JS, e a geração JUCE tem testes de composição e de
fronteira de compasso; não houve audição comparativa nem validação visual da
interface. A nota global continua em 69% até fechar essas diferenças e medir
os demais domínios.

## Estimativa provisória — 27/10/2026

Estimativa atual: **69,0% alcançado; aproximadamente 31,0% falta**. Os pesos
somam 100 e refletem a importância composicional e funcional de cada área. A
nota de cada área estima quanto da capacidade equivalente está implementado;
comparação auditiva, interação em execução e persistência ainda pendentes
reduzem a nota. Rubrica: 0 = ausente; 50 = camada parcial; 75 = capacidade
principal implementada, com diferenças/validação pendentes; 100 = equivalência
verificada nos dois alvos.

| Área | Peso | Nota provisória | Pontos ponderados |
|---|---:|---:|---:|
| Geração, seeds e recombinação | 10 | 85 | 8,50 |
| Vozes, fórmulas, fases e transformações | 12 | 70 | 8,40 |
| Ritmo, compassos e expressão | 10 | 70 | 7,00 |
| Polirritmia | 6 | 55 | 3,30 |
| Forma de sete ciclos | 10 | 75 | 7,50 |
| Gráfico temporal, zoom e foco | 7 | 75 | 5,25 |
| Escala vertical | 3 | 80 | 2,40 |
| Geometria e patterning | 10 | 75 | 7,50 |
| Fourier e epiciclos | 5 | 75 | 3,75 |
| Baixo | 10 | 73 | 7,30 |
| Mixer | 6 | 65 | 3,90 |
| Gravação e banco local | 7 | 40 | 2,80 |
| Learn, tutorial, Sobre e idiomas | 4 | 35 | 1,40 |
| **Total** | **100** |  | **69,00 = 69,0%** |

É um instrumento de acompanhamento, não uma certificação. As notas devem
subir quando a mesma receita for comparada nas duas saídas e as diferenças
forem verificadas; alteração de código sozinha não encerra uma área.

O metrônomo sintetizado de estudo foi acrescentado somente ao JUCE (nov. 2026),
com controle independente do transporte e gravação pelo master. O protótipo
HTML ainda não oferece esse módulo; portanto, ele não aumenta a estimativa de
paridade até existir uma implementação web equivalente e ambas serem conferidas.

## Verificação A/B sem interação manual

Para evitar abrir janelas, repetir cliques ou produzir capturas de tela, a
verificação de áudio deve ser executada por um único comando local e usar uma
receita determinística compartilhada. O fluxo de trabalho é:

1. Fixar seed, variação, BPM, compasso, número de vozes, fórmulas, parâmetros
   do baixo, mixer e duração do trecho. Exportar também o plano de eventos e a
   configuração de cada versão para um relatório legível.
2. Renderizar a receita pelo core JUCE e pela página HTML em Chromium headless
   isolado, usando DevTools por pipes do processo. Para comparação técnica, o
   HTML usa `OfflineAudioContext` e exporta PCM WAV; `MediaRecorder` continua
   sendo um recurso de gravação da aplicação, não o caminho A/B.
3. Alinhar pelo primeiro ataque e comparar duração, RMS/true peak, silêncio,
   clipping, transientes e correlação. Guardar os dois WAVs e o relatório em
   `.local-build/parity-audio/`; são derivados descartáveis, nunca fontes.
4. Se os indicadores detectarem diferença material, gerar um único resumo dos
   trechos divergentes para escuta dirigida. Não tirar screenshots; não elevar
   a porcentagem de paridade até os critérios sonoros e funcionais passarem.

Estado: existe um primeiro renderizador offline do core JUCE em
`src/tools/offline_render.cpp`. Ele prepara sete fases, aplica as escolhas
seeded de número de vozes, fase, padrão, intensidade, transformações, acoplamentos
e perfil de oscilador do baixo, gera vozes e baixo, escreve WAV PCM16 estéreo e
um manifesto JSON ao lado do áudio. O restante dos parâmetros usa uma receita
de referência fixa e ainda não reproduz todo o estado configurável do app JUCE.
O confronto da seed `5EED2027` já pareou esse render com o grafo HTML: os campos
da receita e a agenda coincidem, mas o resultado acústico continua divergente.

`tools/render_web.cjs SEED_HEX [diretório]` automatiza o render HTML local em
Chromium headless isolado por pipes DevTools, sem porta loopback e sem depender
de o navegador habitual estar aberto. Um gancho interno constrói o grafo em
`OfflineAudioContext`, renderiza PCM estéreo e exporta WAV, manifesto e
partitura. A opção `--stems` exporta adicionalmente os grupos `-voices.wav` e
`-bass.wav`, roteados antes do master para diagnóstico. `BATRUC_RENDER_SECONDS`
aceita 28, 42, 56, 70 ou 84 s; o pipeline comparativo mantém 56 s para
corresponder ao render core atual. O renderer
nativo e o HTML ainda têm implementações DSP distintas: receita igual não
significa equivalência acústica.

O renderer JUCE também aceita `--stems` e escreve os mesmos grupos ao lado do
WAV completo. Os stems são comparações de fonte: a cadeia de master/trim não é
idêntica entre os motores, portanto RMS isolado não deve ser usado como ajuste
automático de ganho.

O comparador local `tools/compare_audio.py` aceita WAV e formatos que o FFmpeg
decodifique, com limites de tamanho, duração, taxa de amostragem e timeout. Ele
informa duração, RMS, pico, clipping, silêncio, início de som, alinhamento pelo
primeiro ataque, correlação do envelope RMS em janelas de 10 ms e diferenças
dos planos de eventos, alturas, durações e coordenadas-fonte do baixo.
Com os dois manifestos, também confronta seed, forma, BPM/compasso, vozes,
fase, padrões, lei de intensidade, transformações, acoplamentos, balanço rítmico
e perfil do oscilador; divergências aparecem no relatório como campos com
valores HTML/JUCE para revisão.

`tools/render_compare.sh` produz primeiro o WAV JUCE e depois o WAV HTML. Assim,
uma falha do navegador não impede o artefato nativo, e a cadeia recusa
sobrescrever saídas existentes. O Chromium habitual pode permanecer fechado:
a captura usa um perfil temporário e isolado. Se o sandbox impedir o próprio
Chromium headless, a captura precisa executar num contexto autorizado para
esse processo.

```sh
sh tools/render_compare.sh 5EED2026
```

`tools/render_compare.sh` orquestra WAV JUCE, WAV HTML e relatório numa
chamada. No confronto offline da seed `5EED2027` (56 s, 47 eventos), os 22
campos da receita coincidem e os 47 ataques alinham com erro máximo de 0,05
ms. A auditoria de origem encontrou e corrigiu um `phaseAmount` fixo em 0,20 no
renderer JUCE do baixo: o instrumento passa agora a calcular as mesmas 22
alturas e coordenadas-fonte do HTML (diferença máxima de coordenadas 0,000001;
intensidade de origem do baixo, 0,0000005). Restam duas durações de cauda na
última borda com diferença máxima de 8,54 ms.

A auditoria dos stems encontrou um bug no render offline JUCE: ele acumulava os
buffers de baixo entre blocos. Após limpá-los antes de cada chamada, alinhei o
contrato de mixer da comparação aos defaults do app: master 0,72, trim de vozes
+16,5 dB, volumes unitários, panoramas distribuídos igualmente, ganho do baixo
0,20, cauda modal 0,35 e fundamental 0,55. O comparador verifica agora esses
valores; os 28 campos da receita coincidem.

Com os controles equiparados, o fullmix HTML mede RMS 0,01041, pico 0,53015 e
90,33% de silêncio; o JUCE mede RMS 0,01766, pico 0,20551 e 51,46% de silêncio.
Ambos não clipam; a correlação de envelope é 0,551. Os stems de voz medem RMS
0,00687 no HTML e 0,02442 no JUCE, com correlação 0,478. Os stems de baixo
medem RMS 0,00657 e 0,00311, com correlação 0,989. A cauda modal e os masters
continuam acusticamente diferentes.

O renderer pode testar `decayScale` de 0,15 a 1,5, preservando 0,35 como default
real do app. Um render de diagnóstico em 0,15 aproximou o fullmix (RMS 0,01162,
correlação 0,660) e melhorou a correlação de voz a 0,618, mas reduziu o pico e
continua com caudas mais longas que o HTML. Nenhuma mudança de default foi
aplicada; a escolha exige escuta dirigida. O RMS JUCE 0,41847 pertencia ao
renderer com buffers acumulados e não representa uma saída musical válida.

Uma varredura adicional em `0,20`, `0,25` e `0,30` deu correlações de envelope
fullmix de 0,630, 0,601 e 0,575; no stem de vozes, 0,573, 0,536 e 0,505.
Assim, `0,15` é apenas o melhor ponto da faixa medida, não uma equivalência: seu
stem de vozes ainda tem RMS 0,01592 (HTML 0,00687) e silêncio 74,11% (HTML
91,66%). A inspeção do código explica por que reduzir `decayScale` não basta:
o HTML cria osciladores por evento, faz varreduras de frequência e encerra cada
evento com um release exponencial; o JUCE excita modos ressonantes persistentes,
acumula a energia entre ataques e decai cada modo continuamente. Esse desvio de
arquitetura de síntese deve ser resolvido antes de usar a cauda global como
calibração de paridade. Relatórios e WAVs adicionais estão em
`.local-build/parity-audio/decay-sweep-20261028/`.

A estimativa geral permanece em **69,0%**. Os 47 eventos seguem alinhados em
ataque até 0,05 ms, mas duas durações finais diferem até 8,54 ms. Falta rever a
síntese modal, caudas e balanço do baixo por escuta dirigida. O primeiro WebM
silencioso foi preservado e não entra na comparação. Métricas não substituem
escuta nem certificam equivalência sonora.

## Matriz de trabalho

| Área | HTML | JUCE | Estado da paridade |
|---|---|---|---|
| Geração, seeds e recombinação | Seed de 8 dígitos; seleção de receita, campo, vozes, lei e variação | Mesmo mapeamento v1 e gerador determinístico | O contrato seed→receita e suas saídas agora correspondem. Fixtures cruzadas verificam cinco seeds, a variação de oito vozes e todos os ataques de cinco vozes em uma agenda analítica fracionária. Pêndulo/geometria, execução e escuta A/B continuam pendentes. |
| Vozes, fórmulas, ciclos, fases, acoplamentos e transformações | 0,5–16 ciclos, passos 0,25; fases-base e arco de sete ciclos | Mesma faixa/bases; receita de V2, fases-base, jitter, acoplamentos e transformações determinísticos | As agendas regulares usam eventos analíticos com as mesmas fases, regra e recorte do JUCE; vetores de variação seeded também correspondem. Diferenças em execução, caminhos pendulares/geométricos e resultado acústico ainda precisam de comparação. |
| Ritmo, BPM, compassos, swing, groove, intenção e dinâmica | Implementado | Implementado | As fixtures de baixo cruzam swing, quatro grooves, quatro dinâmicas, quatro intenções, frases e relações em calendários seeded. Duração arbitrária da web, aplicação em execução e resultado acústico ainda requerem confronto. |
| Máquina de polirritmia | 12 presets (2–5 vozes), aplicação sobre os primeiros canais | Tabela canônica no core; 12 presets (2–5 vozes), aplicação sobre os primeiros canais | A tabela nativa foi movida para `CompositionCore.hpp`; fixture verifica aplicação, preservação dos ciclos não cobertos e limites. O smoke web aplicou 5:4 em transporte e confirmou recálculo do plano do baixo. Corrigi o preset JUCE para atualizar overrides sem callbacks de edição manual, preservar a seed e recompilar o baixo uma vez; `batruc_app` compilou. Falta validar esse fluxo ao vivo no JUCE. Salvamento e conferência auditiva seguem pendentes. |
| Forma composicional de sete ciclos (2+2+1+2) | Passada finita e arco progressivo | Loop/uma passagem; arco progressivo em áudio, curva, ponteiro, eventos temporais e cursor XY | O arco formal de fase e a quantidade ativa de vozes agora têm funções canônicas no core, espelhadas no HTML. Fixture compara contagens e primeiro/último ataques de V1–V3 em todos os sete ciclos; execução contínua, loop versus passada finita e divergências de relógio ainda precisam de comparação. |
| Gráfico temporal, zoom e foco | Slider de zoom 1×–8×, escala Y e foco | Botões/roda/arraste, escala Y e slider de foco | O slider nativo acompanha zoom, arraste e reset; ao mover em 1×, abre a janela em 2× como na web. Falta comparação visual em execução e em janelas menores. |
| Escala vertical do gráfico | 0,5×–3× | 0,5×–3× nesta atualização | Código alinhado; falta inspeção visual da interface. |
| Geometria e campo/patterning | Compasso, polígonos, espiral, Lissajous, nefróide (`R=2r`), deltóide (`R=3r`), arco cicloidal com retorno, catenária normalizada e bilhar quadrado com sete razões | As mesmas curvas e razões; vínculo `y(t)`/colisões → eventos V1 e métricas pela fórmula do cadarço/soma das arestas | Fixture cruzada compara 162 posições das 12 curvas e sete bilhares, além de 222 vetores/raios dos epiciclos; erro máximo C++/JS `8,9×10⁻¹⁶`. O bilhar também tem regressão de fechamento, colisões e diferença de áudio calculado. Métricas de área/perímetro continuam informativas. Falta inspeção visual e escuta em execução; acústica de sala continua pesquisa separada. |
| Estudo de reconstrução por Fourier/epiciclos | Implementado | Curva, ordem H, erro RMS e cadeia de epiciclos | O JUCE agora desenha a cadeia de vetores em movimento com armazenamento fixo; falta comparar coeficientes, raio/fase dos círculos, ponto final, zoom e continuidade durante transporte. |
| Baixo: cálculo, interação rítmica, editor, síntese e expressão | Implementado e ativo por padrão | Seed gera eventos por voz e ciclo formal; “Seguir ataques” e “Responder” usam referência vocal por ciclo; ativo por padrão em novas cenas | Fixtures estáticas cruzam 9.168 agendas. No render A/B 5EED2027 os 22 pitches e coordenadas de origem coincidem após correção da fase fixa; ataques em todos os canais alinham até 0,05 ms. Duas caudas na borda final diferem até 8,54 ms. Sob o mesmo ganho de controle, o stem JUCE tem RMS menor que o HTML mas envelope correlacionado (0,989); revisar nível por escuta, sem transposição automática. Cenas que armazenem o baixo desligado continuam respeitando esse estado. |
| Mixer por canal, master, mute/solo, panorama, trim e sensor RMS | RMS separado; mute/solo por canal, baixo e master validados por render funcional | RMS separado; host tem trim do barramento de voz, master e sugestão de equilíbrio, além de roteamento mute/solo/pan testado no core | Smoke HTML mediu baseline RMS 0,01845; mute V1 0,01586; solo V1 0,00969; solo baixo 0,01296; mute master 0. O teste C++ cobre roteamento, pan extremo e sugestão RMS de trim: ausência de sinal, ajuste de +6,02 dB, limite superior e inferior. App recompilado; suíte atual 14/14. Sensor/trim/master não foram exercitados através da janela JUCE; faltam execução real, persistência após reabrir cena e loudness por escuta. |
| Gravação de áudio e documentação da composição | Opus em WebM/Ogg e TXT JSON schema 1 com agenda/revisões | WAV PCM estéreo 24-bit e TXT `-partitura.txt` no mesmo contrato versionado; agenda de sete ciclos e revisões aproximadas; “Abrir composição” recupera a configuração inicial do TXT nativo | Smoke real no Chromium headless: 3,78 s de MediaRecorder, WebM/Opus baixado com 44.216 bytes, igual ao blob; decodificação confirmou sinal (pico 0,329; RMS 0,0195). TXT JSON de 77.318 bytes baixado, schema 1 e nome do WAV correspondente. Banco de composições passou guardar/listar/abrir/apagar com File System Access simulado. Pasta/permissão real, captura nativa, reabertura dos pares e importação sem perda entre HTML/JUCE continuam pendentes. |
| Banco local de cenas/composições | Pasta escolhida; guardar/abrir/apagar composição | Banco de cenas nativo; salvar/exportar/abrir e apagar cena; abre TXT nativo de gravação como configuração inicial | Smoke em Chromium headless validou escolha da pasta (mock File System Access), salvar/listar, reabrir após mudar a seed e apagar, incluindo JSON schema 1. Isto valida o fluxo web contra a API simulada, não permissões reais do sistema. Composições web ainda não são importadas no JUCE; pêndulo/duração não têm mapeamento fiel. Falta smoke nativo e validação com pasta real. |
| Learn, tutorial, Sobre, créditos e idiomas | Seletor global PT/EN/ES/FR; mensagens de arquivo, mixer, vozes, reprodução, traçado e editor; sete tópicos e Sobre em quatro idiomas | Learn com sete tópicos e Sobre em PT/EN/ES/FR; seletor global | Completei no JUCE o tópico “Superfície e geometria” com os percursos disponíveis, vínculo opcional `y(t)→ataques V1`, aproximação harmônica por ordem H e distinção entre métricas de área/perímetro e modulação sonora. PT/EN/ES/FR cobertos pela localização existente. P4 avança em conteúdo, mas não fecha: arquivos web e JUCE seguem incompatíveis, pasta física/gravação nativa e reabertura cruzada não foram validadas; faltam conferência visual, revisão humana das quatro traduções e estados dos modais. |

## Próxima sequência de fechamento

**Atualização P5 — editor de polígonos JUCE:** cenas schema 37 preservam os
vértices e translações manuais do estudo convexo de triângulo/quadrado; leitura
de cenas schema 1–36 permanece suportada. A página web ainda não oferece este
editor, portanto o incremento nativo não aumenta a estimativa de paridade. A
reabertura real e a inspeção visual ainda precisam ser conferidas no app.

O inventário abaixo converte o saldo estimado de 34,9 pontos ponderados nas
cinco frentes solicitadas, cerca de sete pontos cada. São metas de aceitação,
não crédito automático: cada capacidade precisa passar pelos critérios
descritos antes de elevar a estimativa.

| Lote | Meta | Escopo e critério de aceite | Estado |
|---|---:|---|---|
| P1 — seed, fase e receitas | ≈7 pp | Igualar mapa seed→receita, bases, variação, fases e migração de cenas; comparar eventos de uma fixture comum. | Cálculo e fixtures coincidem. Smoke real do Chromium trocou seed `5EED2030→A1B2C3D4` e fase para 0,25 durante o transporte: ambos os cursores foram rebaseados após a janela comprometida, sem ataque repetido, e o transporte seguiu ativo. Falta testar seed/fase na interface JUCE, reabrir as mesmas cenas nos dois apps e comparar a escuta A/B. P1 segue aberto. |
| P2 — ritmo, polirritmia, forma e baixo | ≈7 pp | Conferir BPM/compassos/swing, aplicação das polirritmias por canal, sete ciclos, pausas, transições e relações/alturas/durações do baixo com a mesma seed. | Seis fixtures nativas/cross-engine continuam passando. No mesmo smoke de reprodução web, apliquei 5:4: o cálculo ativo passou a ciclos 5 e 4, agendou novo evento fora do horizonte já comprometido e recalculou o plano do baixo. Isso cobre execução web e dependência composicional calculada, não audição nem JUCE ao vivo. Falta validar ritmo/polirritmia/forma/baixo durante transporte no JUCE, comparar agenda e ouvir os stems; a diferença de duração de ciclo fracionário web segue documentada. P2 segue aberto. |
| P3 — mixer e balanço | ≈7 pp | Comparar ganho efetivo, mute/solo, pan, trims, master e sensor por canal, incluindo baixo contra vozes. | Mute/solo HTML medidos por render; o teste nativo de roteamento do mixer (`batruc_mixer_routing`) foi reexecutado e passou. A cobertura do core valida mute/solo, faders e pan; a medição web anterior continua registrada. P3 não fecha: falta exercitar trim/master/sensor pela interface JUCE, persistência ao reabrir cena, confirmar loudness percebida e decidir/calibrar se o sensor equilibra canal escolhido ou barramento vocal agregado. Sem aceite funcional/auditivo não atribuo pontos. |
| P4 — arquivos, gravação e conteúdo | ≈7 pp | Igualar guardar/abrir/apagar e recuperação; conferir áudio/partitura e inventariar Learn, tutorial, Sobre, créditos e os quatro idiomas. | Banco web passou smoke com File System Access simulado; WebM/Opus e TXT foram gravados/decodificados em smoke local. O importador JUCE explica que a importação web está pendente. Neste avanço, o tópico geométrico do Learn JUCE foi alinhado ao escopo da página e traduzido nas quatro línguas. Permanecem pasta física/permissões, gravação nativa, recuperação dos pares, contrato comum e conferência visual/revisão de conteúdo. |
| P5 — gráficos, geometria e Fourier | ≈7 pp | Comparar traçado, pontos/eventos, zoom/escala, coeficientes, raios/fases dos epiciclos e resposta durante transporte. | Reexecutei `batruc_triangle_geometry`, `batruc_geometry_curves` e `batruc_geometry_curve_parity`; 3/3 passaram (0,24 s), confirmando cálculos geométricos e coordenadas cruzadas existentes. As 162 posições e 222 vetores/raios coincidem com erro máximo `8,9×10⁻¹⁶`. Ainda falta comparar as interfaces renderizadas durante transporte, zoom/escala, continuidade dos traços e influência sonora; teste do core não fecha P5. |

### Lacunas concretas identificadas

- **Forma/fase:** renderer, curva, ponteiros orbitais e eventos temporais comuns
  já representam o arco; o cursor acompanha V2 no mapa XY, mas nuvem de eventos
  e eventos geométricos de V1 ainda usam a agenda preparada. O baixo JUCE agora
  recalcula sua agenda para cada ciclo formal. A fase-base individual seeded
  agora segue o escalonamento `(i−1)/16` e o jitter determinístico da web,
  incluindo wrap modular e migração das cenas antigas. A seleção seed→receita
  tem fixture cruzada e não é mais uma divergência conhecida. Falta comparar
  ataques e mudanças em execução nos dois apps, além de reinício/loop com uma
  receita comum; cenas antigas ainda precisam ser reabertas em cada alvo.
- **Geração e mudanças em tempo real:** mapa de seed, vetores de variação e
  agenda regular têm fixtures comuns. O smoke Chromium passou trocando seed,
  fase e polirritmia durante reprodução; cada revisão respeitou o corte de
  130 ms e o transporte permaneceu ativo. A nova polirritmia mudou também o
  plano de eventos do baixo. O equivalente ainda não foi exercitado no JUCE;
  migração de cenas, caminhos de pêndulo/geometria e resultado acústico com a
  mesma receita permanecem pendentes.
- **Geometria/Fourier:** existem implementações em ambos; falta inventário
  controle a controle e correspondência de forma, animação, fórmulas, erro e
  influência sonora declarada.
- **Baixo/mixer:** a presença de módulos não prova equilíbrio nem interação.
  O JUCE agora consulta os ataques vocais do ciclo formal correspondente e sua
  posição depois do swing ao seguir/responder. Faltam audição comparativa e
  verificação de que mudanças de timbre, relação rítmica e mute/solo têm a
  mesma consequência nos dois alvos. Ambos agora medem RMS separado e permitem
  aplicar uma compensação estática sugerida; faltam conferência visual, escuta
  A/B e calibração musical.
- **Arquivos e conteúdo:** o TXT nativo de gravação restaura a configuração
  inicial. O formato de composição web tem campos sem equivalente JUCE
  (especialmente dinâmica pendular e duração configurável por ciclo); uma
  conversão parcial descartaria dados. Confirmado no código: o arquivo salvo
  pela página usa `format: "batruc-composition"`, `schema_version: 1` e um
  `config` camelCase; o importador JUCE aceita `batruc-scene`/`trucmetry-scene`
  ou o `initial_configuration` de `batruc-recording-score`, cujo objeto interno
  segue o schema nativo. Portanto, “Abrir cena” não aceita hoje a composição
  web. O TXT companheiro da gravação HTML também não é aceito: seu
  `initial_configuration` é o mesmo objeto camelCase, sem o formato e os
  campos exigidos pelo schema JUCE. O contrato comum permanece pendente, assim
  como abertura real de pastas, captura nativa e o inventário final do material
  de aprendizagem/traduções.

**Portões de validação final:** a paridade não chega a 100% sem abrir os dois
alvos, aplicar uma receita/seed idêntica, comparar a reprodução e ouvir os
masters; persistência exige reabrir os arquivos criados. Quando o ambiente não
permitir essa comparação, o lote correspondente fica implementado mas pendente
de aceite, sem declarar equivalência.

## Leis de duração da agenda seeded — 27/10/2026

O core JUCE agora aplica à agenda ligada às vozes as mesmas leis de duração
usadas pelo HTML: duração manual por célula, sequência Fibonacci e potência
geométrica arredondada. As três leis respeitam o limite do compasso. A etapa
de reprodução não recalcula a duração de eventos que já vieram da agenda
seeded, o que evita deslocar o ordinal depois da compactação de ataques. Foram
comparadas as quatro razões expostas na interface: 1,20, 1,50, φ≈1,618 e 2,00.

A fixture de ponta a ponta agora cobre 9.168 agendas HTML/JUCE; 576 casos novos
cruzam quatro seeds, duas variações, quatro frases, três relações e seis
combinações de lei/razão. Comparam quantidade, ciclo, ataque, MIDI, duração
final (já limitada por próximo ataque, pausa formal e borda de compasso) e
intensidade.

Arquivos alterados: `src/core/BassComposition.hpp/.cpp`, `src/app/Main.cpp`,
`tests/seeded_bass_fixture.cpp`, `tests/compare_seeded_voice_bass.cjs`,
`tools/test_seeded_voice_bass_parity.sh`, `tools/test_seeded_bass_parity.sh`,
este documento e `TAREFAS.md`.

**Validação:** `batruc_app`, `batruc_seeded_bass_fixture` e
`batruc_bass_composition_test` compilaram. CTest seletivo passou 2/2; a fixture
confirmou 9.168 agendas. `node --check` e `node prototype/test-score.cjs`
passaram; scripts shell passaram em `sh -n`. Não houve abertura das interfaces,
renderização ou escuta A/B. Estimativa atual: 69,0%; o avanço reflete apenas
esta dimensão funcional, não valida envelope, timbre ou equivalência acústica.

**Estado após esta etapa:** as curvas principais de amplitude e material foram
ajustadas no código; falta medir sua resposta nos dois motores e conferir por
renderização e escuta A/B.

## Curvas de release e material pluck — 27/10/2026

Alinhei duas diferenças entre a síntese JUCE e o HTML. O release de amplitude
do ADSR nativo agora é exponencial, como a rampa do Web Audio; o coeficiente é
calculado uma vez ao iniciar o release e aplicado por amostra, sem exponencial
por sample no callback. O release da envoltória de filtro continua linear, como
no HTML. O material pluck agora chega ao nível 0,12 no intervalo configurado
pela duração do evento (entre 12 e 120 ms), em vez de usar sempre uma constante
de tempo de 120 ms.

Arquivos alterados: `src/core/BassAudio.hpp/.cpp`, este documento e
`TAREFAS.md`. Build local de `batruc_app` concluiu com sucesso. Não rodei testes,
renders ou escuta A/B nesta etapa. Naquele ponto a síntese JUCE seguia
monofônica; a implementação do pool posterior está registrada abaixo. Portanto
a equivalência acústica não foi estabelecida e a estimativa de paridade não
muda.

**Próximo passo:** expor uma medição automatizada de resposta por parâmetro
(amp ADSR, filtro ADSR, cutoff/Q e materiais), então comparar WAVs determinísticos
e escutar densidades altas, releases longos e trocas de seed.

## Vozes simultâneas e caudas no baixo JUCE — 27/10/2026

O renderer nativo saiu do estado monofônico e usa agora um pool fixo de oito
vozes de baixo. Cada voz mantém os próprios osciladores, envelope, filtro,
articulação e gate; assim, um ataque novo não apaga imediatamente o release da
nota anterior. A reciclagem é determinística e prioriza slots desligados,
depois vozes em release com menor nível, usando idade como desempate. Uma troca
da agenda inicia o release de todas as vozes ativas e mantém seus gates
proporcionais à nova duração de seção. O callback segue sem alocação e sem
locks. Se o pool estiver cheio, o slot reciclado guarda o último valor emitido
e aplica um crossfade residual linear de 4 ms para reduzir cliques de troca.

Arquivos alterados: `src/core/BassAudio.hpp/.cpp` e `TAREFAS.md`. O alvo
`batruc_app` compilou localmente. Não rodei testes nem renders. A web não limita
em oito suas notas sobrepostas; densidades acima da capacidade nativa podem
ter resultado diferente. O fade de reciclagem não foi medido em áudio. A
estimativa permanece 69,0% até comparação de áudio.

**Próximo passo:** construir uma medição reprodutível das respostas ADSR, VCF e
de cada matéria, então renderizar as duas versões com a mesma receita e ouvir
os pontos de maior diferença.

## Registro desta fatia — relação do baixo com ataques por ciclo — 26/10/2026

“Seguir ataques” e “Responder” no JUCE deixavam de consultar o mesmo material
vocal que originava cada linha: usavam uma agenda de voz única, sem a fase formal
do ciclo e sem o swing já aplicado às vozes. Agora a preparação do baixo monta,
fora do callback de áudio, uma referência de ataques para cada um dos sete
ciclos, aplicando os ciclos/fases da forma 2+2+1+2 e a transformação de swing
da agenda vocal. Cada evento seeded do baixo consulta a referência marcada para
seu ciclo; a resposta usa essa relação para deslocar o ataque. Eventos manuais
sem marca de ciclo continuam usando uma referência comum. A etapa de síntese
continua limitada a uma agenda preparada, sem busca de eventos nem alocação no
callback.

O alvo `batruc_app` compilou com dois jobs, sem erros. Preflight: 10 GiB
disponíveis, swap 161 MiB/2 GiB, 983 GiB livres no volume do projeto e 7,8 GiB
em `/`; nenhum processo pesado ativo. Não rodei testes nem abri ou ouvi a
interface. Falta comparar as três relações e ouvir resposta, swing e transições
com a mesma seed nos dois alvos. A estimativa permanece em 65,1% até essa
verificação.

## Revisão da receita seeded do baixo JUCE — 27/10/2026

A revisão comparativa do código encontrou divergências de execução no caminho
de baixo derivado das vozes: o JUCE reaplicava operações de swing,
quantização, variação de altura e relação rítmica a eventos que já tinham sido
calculados; a expressão final usava o progresso local de cada ciclo. A agenda
de gravação JUCE ainda multiplicava a intensidade vocal por um fator de fórmula
ausente do agendador HTML, embora esse fator não pertença à intensidade que
seleciona o slot do baixo. O caminho seeded preserva os ataques preparados,
evita os processamentos duplicados e usa o progresso global da forma na
expressão. As pausas de frase contam o ciclo formal. A compactação de eventos
preserva notas coincidentes no tempo local quando pertencem a ciclos diferentes.

A regra de colisão é uma função pura do core, usada pelos dois pontos de
compactação nativa e coberta por teste unitário. Extraí também para o core a
preparação de slots, alturas seeded e relações baixo/vozes. A fixture cruzada
HTML/JUCE percorre 5.472 combinações: quatro métricas (3/4, 4/4, 5/4 e 7/8),
quatro frases (loop, respiração, cadência e chamada/resposta), três relações
do baixo (independente, seguir e responder), quatro seeds (A1B2C3D4, 00000001,
5EED2026 e FFFFFFFF), fases formais 0, 0,125 e 0,25, e BPM de 97, 100, 120,
160 e 220. O caso 97 BPM é omitido em 5/4 porque quatro compassos excederiam o
limite de 12 s da configuração web. O teste cruza também a variação de altura
do baixo em 0% e 100%, mantendo a variação global em zero para isolar esse
controle. Em todos os casos há quatro vozes e swing de 57%; quantidade, ciclo,
instante e altura MIDI coincidem em todos os ataques. Para alinhar eventos
sobre fronteiras numéricas, os dois motores aplicam a mesma margem de
arredondamento ao calcular subdivisão; a web também normaliza a atribuição do
ciclo formal nessas fronteiras. A interface web agora tem o controle “Mínimo
de variação de altura”; o valor efetivo é o máximo entre esse mínimo e a
variação global, igual à regra do JUCE. O cálculo de ciclos, fases,
acoplamentos e transformações da variação global foi extraído para
`realizeSeedVariation()` no core e é usado pelo JUCE. A fixture HTML/JUCE
compara esses parâmetros para quatro seeds e quatro níveis (0%, 25%, 50% e
100%), cobrindo as 12 vozes. Outra fixture ponta a ponta recalcula as agendas
do baixo a partir das vozes variadas e confere 8.592 receitas. A matriz de
4.752 varia quatro seeds, variação global 25%/50%/100%, quatro frases, três
relações do baixo, compassos 3/4, 4/4, 5/4 e 7/8, BPM 97/120/220 e fases
0/0,125/0,25; 5/4 a 97 BPM é omitido pelo limite de duração da seção. A matriz
complementar de 2.304 casos cruza quatro seeds, variação 50%/100%, frases loop
e chamada-resposta, três relações, 2/4/8/12 vozes, swing 0,50/0,57/0,67 e
grooves reto/ternário/aditivo/contratempo em 4/4 a 120 BPM. Quantidade, ciclo,
tempo, MIDI, duração e intensidade coincidem. Outros 1.536 casos cruzam as
quatro dinâmicas e intenções com quatro grooves, duas frases e três relações.
A duração usa a receita manual de dois passos, incluindo cortes por próximo
ataque, pausa formal e borda de compasso. Leis de duração Fibonacci/geométrica,
envelope, timbre, edição e igualdade acústica continuam fora das fixtures.

Validação local: `batruc_app` e `batruc_seeded_bass_fixture` compilaram; CTest
passou 10/10, incluindo `batruc_seeded_voice_bass_parity` com 8.592 agendas.
Tolerâncias: 1e-7 s para tempo/duração e 1e-6 para intensidade. Não renderizei
nem escutei os dois alvos. Preflight: 11 GiB de memória disponível, swap 112
MiB/2 GiB, 983 GiB livres no volume do projeto; nenhum processo pesado ativo.
A estimativa anterior subiu de 66,6% para 67,1% pelas agendas originais, 67,9%
pela cobertura seeded e 68,4% com duração/intensidade nos eventos. A comparação
das quatro curvas de dinâmica e intenções eleva a estimativa para 68,9%, sem
certificar timbre nem igualdade acústica.

## Registro desta fatia — sensor RMS do mixer — 26/10/2026

Ambos os alvos agora medem o RMS dos barramentos de voz e baixo durante a
execução e oferecem um botão que aplica ao trim das vozes a diferença medida,
com limites de −12 a +24 dB. A web usa analisadores Web Audio; o JUCE mede as
fórmulas antes da soma e renderiza temporariamente o baixo em memória pré-alocada
para medir seu barramento separado, sem alocação no callback. A medição JUCE é
suavizada; em ambos os alvos o ajuste usa `20·log10(baixo/vozes)`. A ação aplica
um valor fixo e persistível; não há ganho automático flutuando durante a
execução. O sensor orienta o ajuste, mas não substitui a escuta.

Não abri as interfaces nem fiz escuta A/B. A estimativa geral permanece em
65,1% até verificar os valores, a apresentação e o equilíbrio musical nas duas
saídas.

## Registro desta fatia — continuação, 26/10/2026

O JUCE ganhou controle de variação global da seed (0–100%). A seed recalcula
deterministicamente os ciclos, o acoplamento, a fase da fórmula e a transformação
de cada voz, sempre a partir de uma base não cumulativa. A cena schema 23 guarda
a quantidade de variação e a base por voz e do baixo; versões anteriores continuam aceitas.
O estado reaberto mantém a realização salva. A variação do baixo combina o
controle geral com o controle específico por máximo, e seu algoritmo ainda não
é idêntico ao da web. A fase-base individual e o jitter de seed agora seguem a
mesma regra; a escolha seed→receita e a aplicação da fase global/formal ainda
diferem. A estimativa provisória subiu de 63,5% para 65,1% pelos itens de seed e
recombinação que agora têm controle e persistência nativos; não é aceite final.

Preflight: 10 GiB disponíveis, swap 174 MiB/2 GiB, 983 GiB livres no volume do
projeto, 11 GiB livres em `/`; nenhum build ou processo de áudio pesado ativo.
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` compilou sem erros. Não rodei testes automatizados nem
abri ou ouvi o instrumento. O próximo passo é alinhar as fases com a web e
comparar seeds comuns, incluindo a contribuição do baixo.

O JUCE recebeu o controle de amplitude vertical do gráfico temporal, igualando
a faixa do HTML (0,5× a 3×). É um controle visual: não altera o som. A escala
afeta tanto as curvas quanto os marcadores de ataque. O executável compilou;
falta abrir a interface para confirmar o encaixe visual e comparar com a web.

Build local: `cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` — concluído em 26/10/2026. Nenhum teste automatizado,
execução gráfica ou audição foi feito nesta fatia.

## Registro da progressão formal — 26/10/2026

O JUCE agora percorre sete ciclos de quatro compassos e repete o arco na
performance: C1 e C5 deixam V1; C2 e C6 ativam `ceil((N+1)/2)` vozes; C3, C4 e
C7 ativam todas as `N` vozes. O baixo não dispara em C1 e C5, acompanhando as
pausas da forma web. A mudança de contagem acontece no limite do compasso e a
entrada/saída das vozes usa a transição suave de nível do renderer. A faixa
inferior mostra os sete ciclos e destaca o atual; o status nomeia ciclo e seção.

A forma web continua sendo uma execução finita de sete ciclos; o JUCE repetia
o arco para uso performático. O botão `7×/∞` que acrescenta a passada finita no
JUCE está descrito no registro de 26/10 abaixo. A animação progressiva de fase
ainda não foi portada. Falta validar em execução as entradas, pausas, transições
e a parada/reinício da forma.

## Registro do foco do gráfico e correção UTF-8 — 26/10/2026

O controle de centro do trecho visível foi acrescentado ao JUCE. Ele acompanha
zoom por botões/roda, arraste e reset; mover o centro em zoom 1× abre uma janela
de 2×, seguindo a interação do HTML. Rótulo e dica são localizados nos quatro
idiomas da interface. O estudo de Fourier está presente nas duas saídas; a
comparação da experiência continua pendente.

O autor confirmou que o problema de acentuação UTF-8 na aba do baixo foi
corrigido. A conversão explícita de rótulos e opções, registrada em
`TAREFAS.md`, compilou no binário novo. A compilação desta fatia também verifica
a integração do controle de foco. Inspeção em execução e comparação visual
HTML/JUCE ainda não foram feitas.

Preflight: 9,2 GiB disponíveis, swap 12 MiB/2 GiB, 983 GiB livres no disco do
projeto, 7,9 GiB livres em `/` e nenhum processo pesado. O alvo `batruc_app`
compilou com dois jobs; `git diff --no-index --check` não apontou whitespace
nos três arquivos alterados. Binário
`4b549a5c905260056db36ba8cc8e58387e2f96129ba1da2796bbe554cbfb1b7c`.

## Registro do modo de reprodução da forma — 26/10/2026

O transporte JUCE ganhou seleção entre loop contínuo e uma passagem finita dos
sete ciclos. A passagem finita encerra após 28 compassos (sete ciclos de quatro
compassos), volta o transporte para o início quando iniciada novamente e
finaliza uma gravação ativa ao terminar a forma. O padrão continua em loop para
uso performático. O HTML permanece finito por padrão; esta opção nativa oferece
os dois usos sem alterar os cálculos composicionais.

O alvo `batruc_app` compilou com dois jobs após preflight (8,2 GiB disponíveis,
swap 12 MiB/2 GiB, 983 GiB livres no disco do projeto, 7,9 GiB livres em `/`,
sem processo pesado). Binário SHA-256
`74f056d7284046c70633cfe77f082b6c84d3af24657a9279d12972266495137d`.
Não houve teste automatizado nem execução da interface. Conferir manualmente a
seleção, a parada no fim, o reinício e o caso de gravação ativa antes de fechar
esta equivalência.

## Registro de exclusão no banco local JUCE — 26/10/2026

O submenu do banco agora permite escolher uma cena arquivada e apagá-la após
confirmação; após a exclusão, a lista é atualizada. A implementação impede
remover arquivo cujo diretório-pai não seja a pasta de banco ativa. A operação
web já removia o arquivo selecionado com confirmação. Esta fatia aproxima a
gestão local de arquivos, mas não unifica os formatos: a composição web guarda
`mathematical_partitura` e mixer completo; o arquivo nativo é uma cena JUCE.

Build local do `batruc_app` concluído com dois jobs após preflight (7,7 GiB
disponíveis, swap 12 MiB/2 GiB, 983 GiB livres no volume do projeto, 7,9 GiB
livres em `/`, sem processo pesado). Binário SHA-256
`78f77dc14ffd4fa4ef2decb1fbb0280e649b7864a5c612f79bf3ceab6418e649`.
Não houve teste automatizado nem validação em janela; testar seleção, cancelar,
confirmar e atualização do banco continua pendente.

## Registro do arco de fase formal no renderer JUCE — 26/10/2026

O valor do controle de fase existente agora também conduz o arco da forma nas
vozes V2–V12. C1–C2 preservam a fase-base; C3–C4 avançam até a profundidade
selecionada; C5 sustenta; C6–C7 retornam à base. O renderer resolve os
cruzamentos com a inclinação da fase por ciclo, de modo que os ataques e os
valores usados para intensidade acompanhem o movimento. O cálculo usa estado
fixo e continua sem alocação ou lock no callback.

O JUCE mantém os deslocamentos-base de suas receitas e soma o arco; a origem
das fases não é idêntica à regra seeded do HTML. A visualização nativa ainda
não desenha a trajetória e os pontos de ataque no gráfico/orbitas continuam
por conferir. `batruc_app` compilou após preflight (7,3 GiB disponíveis, swap
12 MiB/2 GiB, 983 GiB livres no volume do projeto, 7,9 GiB livres em `/`, sem
processo pesado). Binário SHA-256
`c4512121368f40a18541f278d3119086b105c292de08a1f73edf047b9d12939c`.
Não rodei testes automatizados, não abri a janela nem ouvi a saída.

## Registro do inventário e da curva de fase — 26/10/2026

O saldo provisório foi desdobrado em quatro lotes de cerca de dez pontos:
geração/forma, geometria/Fourier, vozes/baixo/mixer e arquivos/conteúdo. Os
critérios e as lacunas observáveis estão em “Próxima sequência de fechamento”.
Após os avanços de código registrados abaixo, a matriz passa a estimar 63,5%
implementados e 36,5% restantes. O inventário não concede pontos sem aceite.

A curva funcional JUCE agora recebe o mesmo arco de fase do renderer e os
ponteiros das órbitas também acompanham o movimento. O registro posterior
detalha o recálculo dos marcadores temporais; origem seeded e mapa XY continuam
com diferenças. O alvo `batruc_app` compilou após preflight
(7,0 GiB disponíveis, swap 12 MiB/2 GiB, 983 GiB livres no volume do projeto,
7,9 GiB em `/`, sem processo pesado). Binário SHA-256
`deb4ca177ccb62ab5b6f030e8a9c1e95cb1f78a95625cf663abe383d3d202b80`.
Nenhum teste automatizado, inspeção gráfica ou audição foi feito.

## Registro do traçado geométrico e epiciclos no JUCE — 26/10/2026

O painel nativo de curvas acompanha o progresso do transporte: mostra a curva
completa como referência discreta, revela o trecho já percorrido, desloca o
marcador de lápis e desenha a haste radial nas curvas de compasso/espiral. Em
triângulo e quadrado de Fourier, a função do core agora expõe a cadeia
`0,+1,−1,...` em buffer fixo, e o painel desenha círculos e vetores giratórios
na ponta do traço. A mesma fórmula continua sendo a fonte tanto do traço quanto
da visualização; habilitar `y(t)` como fonte da V1 permanece uma escolha
separada do usuário.

Preflight antes do build: 9,2 GiB disponíveis, swap 12 MiB/2 GiB, 983 GiB
livres no volume do projeto, 7,9 GiB livres em `/`, sem processo pesado. O alvo
`batruc_app` compilou com dois jobs. Binário SHA-256
`62ce48654f65eb10d6e0de6a06d3904819dee34a14e7fd25a512acef900575f8`.
`git diff --no-index --check` não apontou whitespace nos arquivos alterados.
Não rodei testes, não abri a janela e não comparei visualmente com a web; logo,
esta implementação não fecha por si só os dez pontos do lote P2.

## Registro dos eventos do arco no gráfico temporal — 26/10/2026

Para vozes com cruzamentos regulares, o gráfico temporal agora recalcula os
instantes de passagem/extremo pela fase e velocidade efetivas do ciclo formal,
e aplica a mesma seleção do campo de patterning antes de desenhar o marcador.
V1 quando usa eventos geométricos mantém sua tabela própria; o painel XY ainda
mostra a agenda preparada. O arco sonoro, a curva e os ataques regulares agora
compartilham a mesma inclinação matemática. Build local `batruc_app` passou com
dois jobs após preflight (9,5 GiB disponíveis, swap 12 MiB/2 GiB, 983 GiB livres
no volume do projeto, 7,9 GiB em `/`, sem processo pesado). Binário SHA-256
`5535befef388d8282c80326a950ae60fab09f882489b8be3536243cdb00554cc`.
Não rodei testes nem comparei a interface/áudio; resta confirmar visual e
auditivamente com uma receita idêntica nos dois alvos.

## Registro do cursor XY seguindo o arco — 26/10/2026

O marcador que mostra o estado atual no mapa de patterning JUCE agora usa a
fase efetiva da voz 2, em correspondência com o arco do renderer. A nuvem de
eventos continua ligada à agenda preparada e permanece como diferença visual a
fechar. Build local `batruc_app` passou com dois jobs após preflight (9,2 GiB
disponíveis, swap 12 MiB/2 GiB, 983 GiB livres no volume do projeto, 7,9 GiB
em `/`, sem processo pesado). Binário SHA-256
`f3e66cd8d5c2ddbf593583c86b9e712c94e10f725425bff510f5c7f39b5416bc`.
Não rodei testes nem abri a janela; comparar a posição durante as sete seções
continua pendente.

## Registro de sincronismo do baixo, transições e layout — 26/10/2026

Ao aplicar uma seed, o JUCE agora calcula a linha do baixo a partir dos ataques
e coordenadas x/y dos eventos vocais na receita selecionada. As alturas não são
limitadas a escala predefinida. O modo “Seguir ataques” alinha cada nota ao
evento vocal mais próximo e descarta coincidências após o realinhamento. A linha
gerada é guardada nas cenas. O cálculo do baixo ainda não é o mesmo da versão
HTML; duração, relação de resposta e execução A/B seguem para conferência.

Uma nova performance de baixo não zera a fase nem o envelope ativos. A pausa e
a retomada recebem uma rampa de 5 ms. O fechamento do mixer esconde também os
controles V2/polirritmia que ficavam como fragmentos; a altura mínima da janela
agora comporta os controles inferiores do painel. O build `batruc_app` passou
após preflight (8,8 GiB disponíveis, swap 169 MiB/2 GiB, 983 GiB no volume do
projeto, 7,9 GiB em `/`, sem processo pesado). Hash SHA-256:
`12dfffb46a6de722ca0f9b48b55f2f71fd1409f049c3b731f04f26c26ec3f1b1`.
Não abri o app nem ouvi o áudio; o critério de ausência de clique e de
sincronismo precisa da validação auditiva do autor.

## Integração da seed vocal no baixo HTML/JUCE — 26/10/2026

A origem da dessincronia era uma projeção incorreta: no JUCE, ataques das quatro
barras eram dobrados para uma célula de uma barra e depois repetidos; na web, a
seed alterava somente as alturas da grade escrita, enquanto o sequenciador do
baixo ignorava os ataques vocais. Agora os dois calculam uma agenda de baixo
para os quatro compassos a partir dos eventos reais das vozes. Tempo e escolha
de ataques vêm do plano vocal; alturas e dinâmica usam posição geométrica e
intensidade; o algoritmo é determinístico por seed e não limita as notas a uma
escala predefinida.

O padrão de relações oferece modos independente, seguir e responder. Swing,
frase/pausas e leis de duração transformam o calendário seeded. Na página web,
o gráfico mostra o primeiro compasso do primeiro ciclo de mudança e a edição de
uma célula troca explicitamente para o modo manual; arquivos antigos continuam
no modo manual quando não possuem a nova marca de modo. No JUCE, o editor de
uma barra é um resumo; a agenda tocada contém quatro barras.

O JUCE também troca a agenda pendente no limite do compasso, preservando fase,
filtro e envelope do sintetizador; início e pausa usam fade de 5 ms. Compilação
`batruc_app` concluída em 26/10/2026 após preflight; hash SHA-256
`a77ee24be7e3792ab9e9c31d5044b939a0eb4fb8a3fa68dfb9f3684d6fde6c07`.
`node --check prototype/score.js` e `node --check prototype/app.js` passaram.
Não houve execução gráfica nem audição. Sobreposições conhecidas no fechamento
do mixer foram corrigidas, mas o layout completo permanece sem inspeção em
execução.
# Validação funcional de agenda e áudio offline — 28/10/2026

Executei render A/B real com `OfflineAudioContext` (HTML) e o renderizador do
core JUCE para duas seeds, em 48 kHz estéreo e 56 s. A seed `5EED2028` pareou
77/77 eventos; `5EED2029` pareou 20/20. Nas duas, receita (28 campos), ataques,
altura, duração, intensidade e dados de origem/candidato do baixo coincidiram
dentro dos limites do comparador. O maior desvio de ataque foi 0,05 ms. Os
relatórios e WAVs preservados estão em
`.local-build/parity-audio/parity-audit-fixed-20261028/` e
`.local-build/parity-audio/parity-audit-second-seed-20261028/`.

Essa execução encontrou uma inconsistência na borda do ciclo: um piso de
25 ms aplicado depois dos limites de pausa/ciclo podia prolongar uma nota para
fora do ciclo. Ajustei a duração final no HTML e no fixture de comparação para
respeitar a borda formal. O teste cruzado de 9.168 agendas voltou a passar.
Também corrigi em `tools/render_compare.sh` o nome do manifesto HTML passado
ao comparador, que antes apontava para um arquivo inexistente.

O pareamento de eventos não significa que os dois timbres soam iguais. Para
`5EED2028`, a correlação dos envelopes foi 0,600; RMS HTML/JUCE 0,00747/0,01340;
silêncio 82,81%/41,97%. Para `5EED2029`, correlação 0,840; RMS
0,00333/0,00530; silêncio 94,38%/84,84%. Não houve clipping em nenhum render.
Os resultados sustentam a agenda composicional em duas seeds, mas mostram que
resposta sonora e distribuição de silêncio seguem diferentes e precisam de
avaliação dedicada.

**Validação:** `node --check prototype/score.js` e
`node prototype/test-score.cjs` passaram; `ctest --test-dir
.local-build/native/linux-debug-juce --output-on-failure` passou 10/10 após a
correção, incluindo 9.168 agendas seeded, operações generativas, geometria do
triângulo e curvas/Fourier. Isso é validação funcional automatizada e análise
objetiva de áudio. Não fiz audição subjetiva: os WAVs estão disponíveis para
escuta dirigida. A estimativa global de paridade fica em 69%, pois a execução
não fecha diferenças de síntese, gravação/arquivos nem inspeção funcional de
todos os controles.

**Próximo passo:** validar controles de mixer, gravação/abrir/salvar e a
resposta dos parâmetros de timbre com testes funcionais dedicados; depois
repetir A/B por voz isolada. Uma pessoa precisa ouvir os WAVs para julgar
cliques, intenção, balanço e qualidade musical.

## Início alinhado e diagnóstico por stems — 28/10/2026

O render offline HTML acrescentava 70 ms a todos os eventos (`audioStart=0.07`),
enquanto o JUCE começava em zero. Retirei esse deslocamento apenas do caminho
OfflineAudioContext; o player em tempo real continua antecipando a agenda para
compensar a latência do dispositivo. O A/B da seed `5EED2031` agora registra
início em 0 s nos dois lados e mantém 302/302 eventos coincidentes, com erro
máximo de ataque de 0,05 ms.

Ampliei `tools/render_compare.sh` com `--stems`, que exporta e compara mix,
vozes e baixo em relatórios separados. Na seed `5EED2031`, a correlação de
envelope foi 0,527 para vozes e 0,993 para o baixo. O RMS das vozes foi
0,01716 HTML e 0,05257 JUCE; o baixo foi 0,01756 e 0,00817. Os picos não
cliparam nesses stems. A seed `5EED2030` apresentou o mesmo padrão: correlação
0,507 nas vozes e 0,984 no baixo. Isso aponta para modelos sonoros diferentes
nas vozes matemáticas e diferenças de ganho no baixo; não é falha do calendário
seeded. O usuário ouviu os mixes `5EED2028` e confirmou que os timbres diferem.

O código explica a diferença: o HTML usa um oscilador Web Audio por evento,
com varredura de frequência e envelope de amplitude; o JUCE excita oito modos
de ressonância persistentes, com decaimento modal e ruído de ataque. São
arquiteturas sonoras diferentes. Ainda não alterei esse caráter sem um desenho
de equivalência explícito; a comparação de stems torna a decisão verificável.
Stems isolados são material de diagnóstico e não equivalem ao master limitado.

Adicionei `batruc_bass_audio_response`, que renderiza uma nota pelo core JUCE e
confirma sinal diferente ao variar ataque, decay, sustain, release, cutoff,
ressonância e material. A suíte passou 11/11. Isso confirma que esses controles
afetam o render do núcleo; a ligação interativa dos controles e a equivalência
com os parâmetros HTML ainda precisam de validação funcional.

Artefatos do diagnóstico: `.local-build/parity-audio/stem-audit-20261028/` e
`.local-build/parity-audio/stem-audit-offset-fixed-20261028/`.
**Próximo passo:** criar um caminho de síntese de vozes matemáticas comum ou
um modo de compatibilidade entre os dois motores, preservando a opção modal do
JUCE; medir os controles do mixer e testar gravação/reabertura. A estimativa
global segue em 69%.

## Fechamento de paridade seeded do baixo — 07/11/2026

Alinhei o protótipo JavaScript ao agendador C++ para o caminho relacional do
baixo: memória entre ciclos, deslocamento de alturas, respirações deterministas,
intensidade de origem separada da ponderação visual, arredondamentos float,
curvatura e duração cinemática. A intensidade final também é limitada a 1.0,
como no plano C++ de performance.

A fixture agora emite o slot de origem para diagnóstico. A matriz seeded por
voz cobre 8.592 casos, pois cruza 2, 4 e 8 vozes; 12 vozes excedem o contrato
atual do aplicativo. Os três testes de baixo e o de curva `cycloid` passaram.
CTest Release completo passou 18/18 em 73,73 s; `node --check` e
`node prototype/test-score.cjs` passaram. Isso fecha a paridade dos eventos
medidos por essas fixtures; não declara equivalência acústica entre os motores
nem substitui escuta comparativa no aplicativo.

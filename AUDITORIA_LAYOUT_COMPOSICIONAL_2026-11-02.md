# Batruc — auditoria de layout e proposta de editor composicional

Data: 02/11/2026. Estado auditado: app JUCE Release local, sem tocar nem
modificar uma cena do autor. Referências de método: `RASGO_DOCUMENTATION/
METODOLOGIA_DE_DESENVOLVIMENTO.md` (regra de recuperação) e
`RASGO_DOCUMENTATION/design/INTERFACES_E_LAYOUTS.md` (áreas semânticas,
regimes responsivos, ausência de colisões).

## Critério artístico e operacional

O gesto central deve ser desenhar e transformar relações de ataques entre até
oito vozes, ouvir o efeito e compreender o retorno do ciclo. Os gráficos são
superfícies de composição. Um traço que não altera o som deve ser identificado
como estudo; uma curva que altera eventos deve mostrar fonte, destino,
quantidade e resultado. O usuário precisa distinguir seed, edição manual e
transformação aplicada antes de tocar.

## Evidência observada

Usei `--audit-layout=/tmp/batruc-layout-audit-20261102` para capturar topo,
Mixer e Baixo em 1080×800 e 1440×900. O comando retornou `captured: true` e
fechou a janela. As seis imagens estão nesse diretório temporário e são
evidência de inspeção, não material de publicação. Ainda faltam captura nos
perfis 960×600/1024×650, DPI 125–200%, interação por mouse/teclado e
verificação de todas as opções de idioma e modo.

| Região | Achado concreto | Efeito no uso |
|---|---|---|
| Topo 1080 | O gráfico de ondas mede 432×360; o percurso mede cerca de 408×360 e o campo auxiliar fica com cerca de 144 px. Rótulos, fórmulas e controles comprimem a faixa acima e abaixo dos desenhos. | Dados e ações têm pouco espaço legível; é difícil saber qual gesto muda o som. |
| Topo 1440 | O mesmo gráfico mantém altura 360 px e a página principal segue baseada em 800 px. Na captura há uma faixa vazia aproximada de 120 px entre a linha de ciclos e o rodapé. | Maximizar aumenta a janela, mas não dá protagonismo proporcional aos gráficos. |
| Percurso | No modo inicial, o maior gráfico geométrico mostra quase só uma circunferência. Orbit e Study têm mais informação, mas dependem de seleção; a edição atual em Study altera fase de voz, não vértices independentes. | A superfície principal promete composição gráfica mais profunda do que permite hoje. |
| Mixer 1080 | Oito stems têm cerca de 120 px cada e mais de dez controles empilhados. As legendas de alguns parâmetros são pequenas; o rodapé cobre a última faixa quando se entra na seção pelo atalho. | Leitura lenta e alta carga visual durante performance. |
| Mixer 1440 | Há uma faixa vertical alta de faders sem medidores/eventos associados; o início do Baixo aparece sob o rodapé fixo. | Espaço é consumido sem ampliar o contexto rítmico. |
| Baixo 1440 | A linha dos botões de recombinação, inversão e retrógrado em `panelY+578` cruza a segunda fileira de knobs ADSR-F, que ocupa `panelY+538..618`. A vista LFO em `+612..650` também cruza essa fileira; o gráfico de percurso inicia em `+654` enquanto os knobs VCF vão até `+702`. | Colisão real de bounds e de leitura: controles e gráficos se encobrem. Bloqueia o aceite desse painel. |
| Baixo 1080 | O piano roll é legível, mas a faixa F+C mede apenas 74 px; as fórmulas, valores e curva competem ali. O baixo gráfico fica subordinado aos controles de síntese. | A relação entre lei, eventos escritos e resultado sonoro não fica clara. |

O código confirma a origem estrutural: `kPerformancePageHeight=800`,
`functionGraphBounds()` fixa 360 px de altura, `kBassSectionHeight=790` e
grande parte dos componentes recebe coordenadas absolutas em `resized()`.
Largura e altura são tratadas de modo diferente: a janela cresce, mas a
hierarquia visual quase não muda. A inspeção não prova colisão em todos os
estados; as acima foram vistas ou decorrem diretamente de bounds declarados.

## Proposta: mesa de composição geométrica

Manter a página única e os três campos já escolhidos pelo autor — Gráficos e
Cálculos, Mixer, Baixo —, com cabeçalho e rodapé fixos. O primeiro campo vira
uma mesa de composição, ocupando a altura útil real da janela. Na largura
completa, cerca de 60% da área visual vai para um **Editor de Ciclos** e 40%
para uma **vista vinculada**; no compacto, as duas vistas se empilham, sem
reduzir alvos ou fontes por escala uniforme. O espaço excedente aumenta os
gráficos, não os botões.

O Editor de Ciclos mostra um círculo comum com até oito anéis ou polígonos por
voz. Cada vértice é um ataque real; arrastar muda sua posição temporal na
mesma agenda que o áudio usa. Clicar no espaço cria ou remove um ataque,
conforme o modo explícito. Pausa, intensidade e probabilidade têm marcas
diferentes. O ponteiro mostra voz, passo/tempo, deslocamento, força e efeito
esperado antes de soltar. A edição deve ter teclado, valor numérico, desfazer,
restaurar seed e salvar na cena. A operação deve ser limitada em eventos e
publicada ao áudio pela fronteira segura já usada pelo instrumento. O modelo
atual ainda não permite editar cada onset independentemente: esse contrato de
dados é a primeira mudança funcional necessária.

A vista vinculada apresenta, em escolha local **sempre visível**, Onda/Fluxo,
Orbit, Study, Fourier/curva ou campo de máscaras. Ela usa os mesmos eventos e
playhead do editor principal. A relação fonte→destino aparece ao lado da
vista: por exemplo, “curva y(t) → ataques V1” ou “fase V3 → deslocamento de
3/16”. Orbit/Study ficam identificados como estudo quando apenas observam;
quando um gesto muda fase, o desenho e a agenda respondem juntos. Stengel
fica como estudo opcional no inspetor de cálculo, com dimensão contida e
legenda clara sobre sua ausência de efeito sonoro atual. O gráfico de ondas
mostra as oito vozes selecionáveis sem monopolizar a tela; fórmulas e zoom
ficam numa barra própria fora da área de dados.

O wireframe original desta hierarquia está em
[PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg](PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg).
É uma proposta visual de operação, sem copiar o desenho de Rhythmic Geometry;
os vértices da figura ilustram o contrato ainda a implementar, não uma captura
do instrumento atual. A marca no cabeçalho é o lettering aprovado em
`prototype/assets/brand/active/batruc-wordmark-original-crop-v01.png`,
usado em escala menor; a linha “matemática · física · música” sai. O rodapé
continua a apresentar Batruc, Lúcio de Araújo, Rasgo e GNU AGPL-3.0-or-later.

O Mixer vira uma faixa de oito stems com nível, pan, mute/solo, medidor e
miniatura rítmica alinhados em cada coluna. Parâmetros de fórmula, ciclos,
fase e variação aparecem no inspetor do stem selecionado, mantendo controle
direto sem oito cópias simultâneas de cada slider. A miniatura reflete o
polígono/agenda do primeiro campo. O Baixo separa piano roll e lei de duração
como gráficos grandes; ADSR/VCF, LFO e transformações ficam em grupos com
linhas reservadas, sem interseções. O painel pode ocupar mais altura se
necessário, mas a rolagem termina no seu último controle, sem cauda vazia.

### Hierarquia sugerida da tela completa

```text
┌ Cabeçalho compacto: BATRUC · seed · transporte secundário · cenas ┐
│ Toolbar de composição: voz · célula · editar · desfazer · fórmula   │
│ ┌ Editor de Ciclos: vértices/pausas/acentos ┐ ┌ Vista vinculada ┐ │
│ │  playhead + retorno + escuta de cada gesto │ │ onda/orbit/study │ │
│ └─────────────────────────────────────────────┘ └─────────────────┘ │
│ linha de seções A/B/C + estado da seed e mapeamento sonoro          │
├ Mixer: oito stems + miniaturas + inspetor do stem escolhido        ┤
├ Baixo: piano roll/lei à esquerda + controles agrupados à direita   ┤
└ Rodapé fixo: tocar · gravar/tempo · nível · pico · CPU · estado      ┘
```

## Ordem de implementação e aceite

1. **Recuperar a superfície:** substituir coordenadas concorrentes por áreas
   semânticas calculadas a partir da largura e da altura útil; separar
   explicitamente barras, gráficos, controles e rodapé. Corrigir as colisões
   do Baixo antes de acrescentar recursos. Não alterar som nesta etapa.
2. **Dar área aos gráficos:** Editor de Ciclos como foco, vista vinculada ao
   lado, gráfico do Baixo ampliado e espaços vazios redistribuídos. Validar
   topo, Mixer e Baixo em completo, compacto e emergência.
3. **Ligar gesto a composição:** modelo limitado de onset por voz no core,
   edição no círculo, atualização audível, cena/undo e equivalência com a
   linha temporal. Comparar saída antes/depois da edição.
4. **Afinar performance:** foco/teclado, alvos confortáveis, estados ativos,
   idioma, contraste, tempo de resposta, medidores e teste humano em monitor
   único e duplo. Só aceitar a versão quando nenhum controle visível cobrir
   outro e o músico conseguir explicar o efeito de cada gráfico ativo.

Critérios mensuráveis: nenhum par de controles visíveis de funções diferentes
tem interseção de bounds ou de hit targets; nenhum gráfico editável é coberto
por toolbar ou rodapé; o topo aproveita a altura adicional em 1440×900; o
editor principal ganha espaço real ao maximizar; uma ação no gráfico atualiza
som, desenho e valor persistido; modo compacto preserva controles essenciais.

## Continuidade — 03/11/2026

O redesenho amplo continua como especificação, mas já há um segundo incremento
real na tela JUCE. Mantive Study como vista inicial e recolhi o campo de padrão
lateral por padrão; reposicionei a segunda fileira de controles para não cruzar
o percurso; separei a linha de toolbar, título e traços de Onda/Fluxo; removi o
título redundante e a duplicação dos rótulos Intenção/Dinâmica; e reorganizei a
faixa de lei, intensidade, intenção, dinâmica e seções para eliminar colisões.
O percurso do Study passou a começar abaixo da fileira de controles e termina
antes da fórmula, sem cobrir botões. Em janelas mais altas, as vistas de onda e
percurso aumentam e as linhas de fórmula/controles/seções descem para usar o
espaço vertical. Corrigi uma regressão de inicialização do tamanho responsivo:
o cálculo agora usa a altura do viewport, não a altura total da página rolável;
o relatório inclui bounds/visibilidade do percurso em 1080 px.

Validação: build Release JUCE serial; auditoria via
`--audit-layout=/tmp/batruc-layout-final-corrected-20261103`, capturas
`1080-top.png`, `1440-top.png`, Mixer e Baixo. `audit-result.json` informa
`captured: true`; os bounds do Study em 1080 são 445×284×607×339. Inspecionei
as capturas de 1080×800 e 1440×900: o desenho reaparece em largura mínima, e a
janela alta ocupa a faixa vertical até o rodapé. Uma tentativa direta de
auditoria falhou uma vez com SIGSEGV; a execução seguinte sob GDB e a execução
direta posterior terminaram normalmente. A intermitência ainda não foi
isolada. Esta revisão não prova fluidez em hardware do autor nem elimina toda
a densidade visual do cabeçalho. `./run` aponta para o binário Release recém-
compilado; não havia cópia antiga em execução durante a auditoria.

Continuam pendentes e não devem ser confundidos com esta correção: nova
distribuição semântica de toda a página; Editor de Ciclos com edição independente
por vértice/ataque; modelo comum de gesto, áudio, desenho, undo e persistência;
auditoria/intervenção nas colisões do Baixo; inspeção com o autor em uso real e
investigação da antiga falha de inicialização relatada em 02/11. O wireframe
segue uma proposta, não uma captura do app.

## Revisão estrutural do JUCE — 03/11/2026

A hierarquia do primeiro ecrã foi invertida conforme a proposta: Study ocupa
a coluna principal à esquerda (aproximadamente 60%); Onda/Fluxo ocupa a coluna
vinculada à direita (aproximadamente 40%). O cálculo de bounds dos dois painéis
usa a largura real; ao maximizar, a altura útil amplia os gráficos e desloca
fórmula e seções até o rodapé. A legenda lateral do Study relaciona cada cor à
voz e à contagem de ataques. A marca aprovada foi reduzida, sem alterar a
autoria/créditos. No Baixo, knobs ADSR/VCF, variação, LFO e percurso agora
ocupam faixas consecutivas sem cruzar bounds; a seção cresceu para conter o
último controle. O salto pelo cabeçalho e rodapé posiciona Mixer e Baixo abaixo
do cabeçalho fixo.

Validação local: build Release serial e auditoria visual
`--audit-layout=/tmp/batruc-layout-gate-20261103` com `captured: true` em
1080×800 e 1440×900. Foram inspecionados topo, início de Mixer, início de
Baixo e parte final do Baixo. O modo de auditoria registra bounds do Study em
1080 px. As capturas mostram os elementos reorganizados sem as colisões
identificadas no Baixo. Não foi executada verificação humana de edição/escuta,
nem auditoria exaustiva de todos os estados, DPI e idiomas.

O coredump de 03/11 apontou para `juce::Component::centreWithSize` durante
a criação da janela. Substituí essa consulta ao monitor por bounds iniciais
determinísticos; duas inicializações diretas seguidas do modo de auditoria
terminaram com `captured: true`. A correção elimina esse ponto específico do
caminho de inicialização; ainda falta observação em sessões longas no monitor
do autor. Release final: SHA-256
`c7a922b9e8d7367202804b8bdead6744431ea38fb08165b1912943f714acbc1e`.

Pendências de produto que continuam separadas do layout atual: edição de cada
ataque no círculo ligada ao áudio/undo/cena; medidores de áudio por stem;
resposta visual e sonora a cada gesto. O usuário aprovou o princípio do
wireframe, não estas funções como já concluídas.

## Mixer com inspetor do stem — 03/11/2026

O Mixer deixou de repetir fórmula, timbre, regra de evento, acoplamento,
fase, ciclos e variação em oito colunas estreitas. Cada stem tem um botão de
seleção com estado ativo/inativo/mudo, miniatura rítmica, nível, mute/solo,
pan e movimento. O inspetor único apresenta os controles do stem selecionado;
a fórmula de prévia acompanha a seleção. Reduzi a altura do módulo de 760 para
675 px e reservei faixas distintas para seleção, fader, controles diretos e
inspetor. A síntese, as regras da agenda e a persistência de cena não foram
alteradas; o stem selecionado é apenas estado de interface nesta etapa.

Validação: preflight com 9,4 GiB de memória disponível, 890 MiB de 2 GiB de
swap em uso e 979 GiB livres no disco de trabalho; build Release serial;
auditoria final `--audit-layout=/tmp/batruc-mixer-bounds-zero-20261103` com
`captured: true`. As capturas de Mixer em 1080×800 e 1440×900 foram inspecionadas
com V1 e V2 selecionados; a troca alterou botão, cor, controles e fórmula.
O espaço acima do inspetor foi reduzido em 25 px e há 4 px entre botão do stem
e fader; ambos os ajustes aparecem nas capturas finais. A auditoria de bounds
detectou duas interseções entre rótulos e sliders da linha superior, corrigidas
antes do último build. O JSON final informa `mixer_overlaps` igual a zero
para V1 e V2 nas duas larguras. SHA-256 do Release:
`49a04e6f437b76ba5984a534aa86a1031684455367a943e07fe3b93b332a1f0b`.
Ainda faltam medidores de áudio reais por
stem, edição de onset individual no gráfico, teste humano contínuo e matriz de
DPI/idiomas. A contagem de bounds cobre controles visíveis do Mixer nesses
estados; não prova ausência de colisão em todos os estados do instrumento.

## Aplicação visual do SVG aprovado — 03/11/2026

Objetivo: aproximar a primeira tela JUCE do wireframe aprovado pelo autor,
sem apresentar como prontas as ações de ataque, pausa e acento por vértice.
O `src/app/Main.cpp` agora dispõe dois cartões brancos nas proporções do SVG:
Editor de Ciclos à esquerda e Vista Vinculada à direita. O cabeçalho fixo tem
72 px nas larguras validadas, com a marca aprovada; o rodapé mantém os créditos
em faixa escura. A geometria circular cresce no cartão esquerdo, com uma coluna
de leitura de ciclos/vozes e controle de variação. A onda, os parâmetros
musicais e o metrônomo ocupam o cartão direito; em 1440×900 há uma faixa de
prévia dos oito stems antes do transporte. O Mixer e o Baixo continuam abaixo,
acessíveis pela rolagem e pelos atalhos do cabeçalho/rodapé.

O desenho e o teste de clique dos anéis passam a usar o mesmo centro e raio;
isto corrige o desfasamento de 10 px que havia entre a pintura e a área de
interação. Os modos Study, Orbit, curvas e Áreas reservam a coluna lateral
para que a variação não cruze o gráfico.

Validação local: preflight de recursos (9,6 GiB de RAM disponível, 890 MiB de
swap em uso), build Release serial sem aviso de compilação e auditoria
`--audit-layout=/tmp/batruc-svg-layout-release-20261103`. O JSON retornou
`captured: true`, e as capturas `1080-top.png` e `1440-top.png` foram
inspecionadas visualmente. A contagem de colisões do Mixer foi zero com V1 e
V2 selecionados nas duas larguras. SHA-256 do Release:
`ef3e0ea2bcfbe265f9e16edb70b262df5b232b313b782ec506c4325535565ace`.
Não houve ensaio humano de áudio/clique, DPI e idiomas, nem auditoria visual de
todos os estados de Orbit/curvas/Áreas. Próximo passo: implementar edição
individual de ataques no Editor de Ciclos ligada à agenda sonora e às cenas.

## Correção de navegação e acabamento — 03/11/2026

O autor confirmou que a primeira tela deveria ficar sem barra de rolagem
visível, com acesso ao Mixer e ao Baixo pelo cabeçalho, e pediu aparência mais
próxima do SVG. O cabeçalho agora tem Editor, Mixer e Baixo lado a lado, com
cor ativa visível; a barra vertical foi ocultada e a paleta de fundo/cartões
foi aproximada da maquete. Os controles mais profundos de Mixer e Baixo
continuam acessíveis pela roda do mouse. O rodapé de créditos permanece.

Capturas de 1080×800 e 1440×900 em
`/tmp/batruc-navigation-final-20261103` mostraram os três destinos sem
colisão no cabeçalho e sem barra visível; `audit-result.json` registra
`captured: true` e zero colisões de bounds do Mixer testado. Build Release
serial concluído, SHA-256
`fc6134351109a0ebfaf13bdcdb30eb94a7733d6374f996783e65b2d59209c8df`.
As capturas do modo diagnóstico posicionam a vista programaticamente; os
cliques ainda precisam de confirmação direta no desktop do autor.

## Continuidade da auditoria — 05/11/2026

As revisões posteriores ocultaram a barra vertical, restauraram a largura
integral de Mixer e Baixo e separaram os bounds dos painéis flutuantes dos
bounds dos módulos. A auditoria final disponível em
`/tmp/batruc-layout-fixed-20261105` registrou `captured: true` e zero
interseções entre controles visíveis do Mixer para V1/V2 em 1080 e 1440; as
capturas de Mixer e Baixo em 1440×900 foram inspecionadas. O Study ganhou
criação, deslocamento, intensidade e remoção direta de pontos manuais.

Isso não encerra a auditoria. O painel Sobre ainda precisa ser reconfirmado em
1920×1080 depois da última correção de camadas; DPI, idiomas, todos os modos
gráficos e estados dos painéis continuam fora da matriz completa. A navegação
também não é uma paginação verdadeira: a barra não aparece, mas controles
profundos ainda dependem da roda do mouse. Knobs, sliders, caixas e botões não
possuem ainda escala e linguagem visual uniformes. A fonte viva dessas
pendências e o critério de encerramento estão no topo de
[TAREFAS.md](TAREFAS.md).

## Fechamento da navegação visual — 06/11/2026

Editor, Mixer e Baixo foram compactados para caber individualmente entre o
cabeçalho e o rodapé em 1080×800. A roda sobre o fundo não percorre mais o
documento; os três destinos são abertos pelo cabeçalho. O Mixer mantém oito
stems e o inspetor numa tela, e o Baixo foi redistribuído em duas colunas com
os onze knobs ADSR/VCF em duas linhas maiores. O rodapé de créditos permanece.

O fluxo MIDI está visível no Study e no atalho inferior. Áreas/Stengel saiu da
superfície musical, pois não tinha ação sonora compreensível. A auditoria
`/tmp/batruc-pages-audit` capturou Editor, Mixer e Baixo; as três imagens
1080×800 foram inspecionadas e o detector do Mixer reportou zero interseções
para V1/V2 em 1080 e 1440.

Essa entrega fecha a aparência sem rolagem, mas não a dívida arquitetural: os
módulos ainda estão empilhados internamente num `Viewport`. A auditoria de
coesão recomenda migrá-los gradualmente para um host de páginas que mantenha
somente o módulo ativo no layout e na animação. DPI, todos os idiomas e ensaio
humano prolongado continuam pendentes.

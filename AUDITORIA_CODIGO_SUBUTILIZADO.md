# Auditoria de código não conectado e subaproveitado

**Data:** 29/10/2026
**Escopo:** leitura estática do C++/JUCE, da página web, do CMake e dos testes.
**Limite:** não inclui inspeção visual nem audição; não declara equivalência
funcional com base apenas na presença de uma implementação.

## Critério

- **Em uso:** há caminho de chamada da interface ou do gerador até o recurso.
- **Parcial:** há chamada em execução, mas a saída não governa o som ou a
  composição de ponta a ponta.
- **Isolado:** o código compila, mas não há chamada de produção; pode haver
  cobertura unitária. Não é removido nesta auditoria.
- **Não verificado:** a leitura estática não basta para confirmar o
  comportamento em execução.

## Inventário

| Núcleo | Estado encontrado | O que o código já faz | Lacuna concreta |
|---|---|---|---|
| `CompositionCore` e `GeometryCurves` | Em uso | Gera planos de eventos; curvas, pontos de cruzamento/extremos, trajetórias de bilhar e Fourier participam do caminho composicional da voz 1. Os cálculos de curva têm fixture numérica C++/JS. | A trajetória de bilhar altera eventos; ainda não calcula reflexões acústicas, atrasos, perdas ou resposta de sala no áudio. A fixture matemática não valida a animação nem a audição. |
| Métricas geométricas de polígonos | Parcial | `geometryCurvePolygonMetrics` calcula área e perímetro de triângulo/quadrado e o painel JUCE as consulta. | Essas grandezas são informativas: não recalculam eventos, durações, alturas, intensidade, timbre ou baixo. Não foi encontrado caminho equivalente explícito no `prototype/score.js`. |
| `TriangleGeometry` | Isolado | Calcula métricas de triângulos (incluindo Heron, medianas, centros e linha de Euler), Stewart, Ceva e a relação da escada cruzada. Está compilado e tem teste unitário. | Busca no app e no protótipo não encontrou chamada de produção. Hoje é uma pequena biblioteca matemática, não um operador do instrumento. |
| `EcologicalRelations` | Parcial, em ambos os alvos | A regra C++/JS tem fixture numérica equivalente. HTML configura relação por voz e aplica a transformação antes de gerar o baixo. JUCE oferece modos ecológicos globais para o baixo seedado; transforma os eventos no preparo da composição e publica a nova performance para aplicação no próximo compasso. | Granularidade JUCE é global, não por voz; não afeta linha manual; não há rastro causal visível nem audição comparativa HTML/JUCE. |
| Solver do pêndulo acoplado | Parcial | RK4 calcula três coordenadas angulares, velocidades e energia cinética/potencial por pêndulo; meia energia de cada mola é atribuída às vozes adjacentes. `batruc_offline_render --coupled-pendulum` converte estados e energia local em eventos, acentos e entradas para o baixo. | Ainda não está integrado à GUI nem ao transporte JUCE ao vivo; parâmetros seedados não são editáveis e falta trajetória visível. Aproximação linear de ângulo pequeno com molas efetivas. |
| `GenerativeOperations` | Parcial | Implementa inversão de acorde, espelhamento de alturas, retrogradação temporal, recombinação score a score, recombinação de eventos entre ciclos, recálculo e inversão de fase. Retrogradação, espelhamento e recombinação entre ciclos afetam a performance JUCE, são desligáveis por recálculo e ficam salvos nas cenas; testes cobrem limites e preservação dos eventos. | Ainda não há encadeamento/histórico de comparação nem vínculo com as operações web. Inversão de acorde e recombinação score a score continuam sem controle no fluxo JUCE. |
| Baixo e renderer modal | Em uso | O baixo tem gerador de score e síntese próprios, controles de ADSR/filtro/LFO, seed e relações com o plano das vozes em caminhos já implementados. | A integração precisa ser medida por fluxos comuns de composição: o baixo ainda deve responder a mais aspectos da estrutura, não apenas compartilhar seed/ataques. Não confundir controle exposto com influência musical eficaz. |
| Espaço acústico | Conceito documentado, não implementado como efeito | O material didático distingue raio geométrico, modos de sala e difração. O bilhar atual é um operador de eventos. | Não há sala definida por geometria com reflexão/atraso/atenuação no sinal; o ambiente/reverb corrente não representa uma sala derivada do polígono. |

## Direção de aproveitamento

Não recomendo apagar nem reescrever esses módulos antes de integrá-los em
pequenos fluxos comparáveis. A maior oportunidade está em transformar
matemática que hoje aparece isolada em **operações composicionais explicáveis**:

1. **Relações entre vozes — prioridade alta.** A primeira integração já está
   nos dois alvos e aplica o plano ecológico fora do callback, com entrada do
   baixo JUCE na fronteira de compasso. Fechar a diferença de granularidade
   (relação por voz no JUCE), expor rastro causal e comparar auditivamente as
   mesmas composições antes de ampliar influência para altura/timbre e
   percussão.
2. **Operações generativas acessíveis.** Encadear inversão, retrogradação,
   recombinação e recalculação como ações sobre células/compassos, não apenas
   como funções de baixo. Definir cada transformação, o que preserva e como
   desfaz; compartilhar entradas/saídas e fixtures entre HTML e JUCE. As
   operações musicais devem atuar no plano completo quando a regra pedir, e
   permitir aplicar uma transformação a uma voz sem congelar as outras.
3. **Geometria que deixa consequência audível.** Fazer área, perímetro,
   proporção, curvatura, cortes e interseções produzirem descritores
   normalizados e auditáveis. Mapear esses descritores para regras explícitas
   de densidade, duração, acento, registro, espectro ou forma. Oferecer
   pré-escuta/comparação A/B e não impor uma relação universal entre uma medida
   e um parâmetro musical.
4. **Espaço como etapa separada.** Reutilizar o bilhar como gerador de tempos de
   chegada e direções; definir dimensões/unidades e coeficientes das paredes;
   então implementar atraso/atenuação limitados e, em outro modelo, modos ou
   convolução. A visualização deve mostrar raios e colisões calculados. Não
   chamar o desenho de reverberação enquanto ele não afetar o áudio.
5. **Reduzir duplicação após fixtures.** Onde HTML e C++ recalculam a mesma
   regra, manter entradas e saídas em fixtures compartilhadas e gerar
   comparações automatizadas. Não fundir os renderers de áudio: cada um pode
   manter síntese própria, desde que o comportamento composicional seja
   comparável e as diferenças de timbre sejam declaradas.

## Ordem sugerida de trabalho

1. Completar a paridade das relações ecológicas: o primeiro caminho JUCE já
   publica o baixo seedado para aplicação no compasso seguinte. Próximos passos:
   persistir relação/força na cena JUCE, oferecer seleção por voz comparável ao
   HTML, mostrar o rastro causal e fazer A/B auditivo. A configuração,
   persistência e aplicação web antes do baixo já estão ligadas.
2. Expor as operações restantes sobre células e planos, com undo/comparação e
   preservação de voz independente. Retrogradação e espelhamento de alturas
   estão conectados; inversão de acorde e recombinação ainda precisam entrar
   na interface e ser comparadas musicalmente.
3. Escolher duas grandezas geométricas úteis e torná-las operadores sonoros
   controláveis, em vez de ativar simultaneamente todos os teoremas.
4. Fazer modelo de sala por raios com atraso/atenuação, depois validar se o
   modelo modal é necessário para as baixas frequências.
5. Revisar cálculos restantes e remover apenas código cuja função tenha sido
   substituída ou recusada como direção do instrumento.

## Validação e retomada

As classificações “isolado” significam sem referência de produção encontrada
na busca estática, não prova de que o recurso seja inútil. A fixture ecológica
foi comparada entre C++ e JS: 10 rastros causais e 10 eventos finais coincidem
dentro de 2e-6. A composição web oferece quatro relações nas vozes 2–12,
persiste a escolha, transforma eventos antes de gerar o baixo e transmite a
alteração de intensidade ao baixo. A relação padrão permanece independente.
A aplicação JUCE, o rastro causal visível e a audição comparativa continuam
pendentes; não se declara paridade funcional ou sonora nesta etapa.

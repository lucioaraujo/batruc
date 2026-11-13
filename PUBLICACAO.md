# Batruc — prontidão para publicação

Segue a estratégia da família
([`ESTRATEGIA_DE_PUBLICACAO.md`](../../RASGO_DOCUMENTATION/ESTRATEGIA_DE_PUBLICACAO.md)):
não se pula da camada 0 para a 4. O modelo prático é o caminho já feito pelo
Rasgo Modular (`../RASGO_MODULAR/PUBLICACAO.md`).

Decisão do autor (13/11/2026): "avance em todas as etapas para a
publicação, vamos acertando os detalhes do software no caminho".

## Camadas

| Camada | Critério | Estado |
|---|---|---|
| 0. Pesquisa local | estado documentado; nada apresentado como release | ✅ `DESENVOLVIMENTO.md` (antigo README), `TAREFAS.md`, auditorias e estudos |
| 1. Candidato publicável | build e execução multiplataforma, licença, créditos, documentação e limitações revisados | 🟡 Linux ✅; CMake, CPack, scripts e workflow prontos; **Windows e macOS nunca compilados** (falta o repositório próprio para a CI rodar) |
| 2. Release do instrumento | pacote por plataforma, testes, evidência de validação | ❌ falta a CI nos três sistemas e a sessão de escuta documentada |
| 3. Página editorial | texto, autoria, imagens, links e estado correspondem à release | ❌ falta a página no portal da família |
| 4. Publicação | portal, repositório e página liberados juntos | ❌ |

## Gate editorial — item a item

| Item | Estado |
|---|---|
| nome, descrição curta e estado | ✅ `README.md` público em 4 línguas (13/11); o diário virou `DESENVOLVIMENTO.md` |
| autoria, créditos, fontes e licença | ✅ `LICENSE`, [`CREDITS_AND_SOURCES.md`](CREDITS_AND_SOURCES.md) |
| instalação e plataformas | ✅ [`INSTALL.md`](INSTALL.md), com a matriz honesta (só Linux testado) |
| testes e validações realmente executados | 🟡 28 testes no Linux; escuta do autor em lotes; falta a CI nos 3 sistemas |
| screenshots com origem autorizada | ❌ fazer, a partir de execução real |
| links (repositório, documentação, release) | ❌ dependem do repositório próprio |
| ausência de áudio privado, gravações e brainstorm | ✅ escutas em `.local-build/` (fora do git) |
| contato oficial | ✅ `rasgo.instruments@gmail.com` (já no CPack) |
| versão publicada = página editorial | ❌ depende da página |
| correção e retirada | 🟡 seguir o procedimento do Rasgo Modular (copiar para cá na release) |
| nome disponível | 🟡 nenhum conflito em web, marcas, software musical, domínios e GitHub (13/11); **falta a busca oficial no INPI e na WIPO** |

## O que falta, separado por quem resolve

### Depende de mim (código e documento)

- [x] CMake: Windows com runtime estático e `/bigobj`; macOS Universal 2
  (10.15); ícone; instalação; CPack (13/11)
- [x] `packaging/linux/` e `.github/workflows/package.yml` (porte do Modular)
- [x] `.deb` gerado e inspecionado localmente
- [x] `INSTALL.md`, `CHANGELOG.md`, `CREDITS_AND_SOURCES.md`, este
  documento
- [x] texto "Sobre" do app nas 4 línguas: versão 0.1.0 candidata, recursos atuais, crédito do GMD, "testado no Linux; Windows e macOS ainda não" (13/11)
- [x] README público curto em 4 línguas (13/11)
- [ ] corrigir o que a CI apontar no Windows e no macOS (o Modular precisou
  de cinco correções na primeira rodada)
- [ ] screenshots de execução real
- [ ] página do Batruc no portal da família (preparada localmente; o
  upload para o HostGator é do autor)

### Depende do autor

- [x] **Catálogo Ritmos / MIDI:** decidido (a), embutir (13/11).
  - O app procura o catálogo nas pastas de instalação.
  - O CMake instala o GMD com a licença dele.
  - `packaging/obter-gmd.sh` baixa e confere o SHA-256; a CI roda esse
    passo antes do build.
  - Conferido: o `.deb` tem 1.150 MIDIs e o binário instalado os carrega.
- [x] **README:** o diário virou `DESENVOLVIMENTO.md` (`git mv`, com o
  histórico preservado), e o README público entrou em 4 línguas (13/11).
- [ ] **Repositório próprio** `lucioaraujo/batruc`: privado primeiro (a
  CI em repositório privado consome cota) ou público direto (minutos
  gratuitos).
- [x] **Versão** da primeira release: 0.1.0 (13/11).
- [ ] **Busca de marca** no INPI (classes 9, 15 e 41).
- [ ] **Protótipo web em HTML:** vai junto ou fica como histórico.
- [ ] **Sessão de escuta final** documentada.
- [ ] Revisão do espanhol por um falante nativo (desejável).

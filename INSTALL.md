# Batruc — instalação / installation

> **Estado (13 nov. 2026):** versão 0.1.0 em preparação, ainda **sem
> release publicada**. Os pacotes abaixo saem da integração contínua quando
> o Batruc tiver repositório próprio.
> *English:* version 0.1.0 is being prepared; no release has been published
> yet.

## Matriz de plataformas

Cada combinação está marcada com o que foi **realmente feito**, não com o
que deveria funcionar.

| Plataforma | Build | Execução | Áudio verificado | Pacote |
|---|---|---|---|---|
| Linux x86-64 | ✅ local | ✅ local | ✅ em hardware real | ✅ `.deb` gerado localmente (CPack) |
| Windows x86-64 | ❌ nunca compilado | ❌ | ❌ | previsto: instalador `.exe` (NSIS) e `.zip` |
| macOS (Universal 2) | ❌ nunca compilado | ❌ | ❌ | previsto: `.dmg` |

O workflow `.github/workflows/package.yml` (porte do Rasgo Modular) compila,
testa e empacota nos três sistemas. Hoje ele está **inerte**: o GitHub só lê
workflows na raiz do repositório, e o Batruc ainda vive no monorepo da
família. Até ele rodar, nada se afirma sobre Windows e macOS.

## Português

### Instalar (quando houver release)

- **Ubuntu, Debian e Mint:** `sudo apt install ./batruc-<versão>-linux-x86_64.deb`.
- **Outras distribuições:**
  - **AppImage:** marque como executável e abra;
  - **`.tar.gz`:** descompacte e rode `./install.sh`, que instala em
    `~/.local`, sem root.
- **Windows e macOS:** a primeira abertura mostra os avisos de segurança
  esperados para software sem assinatura paga. O passo a passo será o mesmo
  do Rasgo Modular (ver o `INSTALL.md` dele) e entra aqui quando esses
  pacotes forem testados.

### Compilar a partir do código

Requisitos:
- CMake 3.22 ou mais novo e um compilador C++17;
- o checkout do **JUCE 9.0.3** (a versão exata é exigida);
- no Linux, as bibliotecas de desenvolvimento de ALSA, FreeType, Fontconfig
  e X11.

```sh
# só o motor e os testes
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 2
ctest --test-dir build

# o app
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
      -DBATRUC_BUILD_JUCE_APP=ON -DBATRUC_JUCE_SOURCE_DIR=/caminho/para/JUCE
cmake --build build --target batruc_app --parallel 2

# pacote (.deb no Linux, .exe no Windows, .dmg no macOS)
cd build && cpack -C Release
```

No Linux, `./run` na pasta do projeto compila e abre a versão Release
quando disponível.

### Onde o Batruc guarda as coisas

- **Preferências:** `~/.config/RASGO/Batruc.settings` no Linux; pasta
  equivalente do sistema no Windows e no macOS.
- **Cenas (`.batruc.txt`), gravações WAV e MusicXML:** onde você escolher
  salvar.
- **Catálogo Ritmos / MIDI:** os pacotes levam o Groove MIDI Dataset (1.150
  performances, CC BY 4.0, com a licença dele):
  - Linux: em `<prefixo>/share/batruc/midis/groove`;
  - macOS: em `Batruc.app/Contents/Resources/midis`;
  - Windows: ao lado do `.exe`, em `midis`.

  Para compilar com o catálogo, rode antes `packaging/obter-gmd.sh`, que
  baixa o arquivo oficial e confere o SHA-256.

### Licença

GNU AGPL-3.0-or-later ([`LICENSE`](LICENSE)). Créditos e fontes em
[`CREDITS_AND_SOURCES.md`](CREDITS_AND_SOURCES.md).

## English

- **Status:** no release yet. Linux is the only platform built and played.
  Windows and macOS have never been compiled; the CI workflow runs once
  Batruc has its own repository.
- **Build from source:** see the commands above (CMake ≥ 3.22, C++17,
  JUCE 9.0.3 exactly; `-DBATRUC_BUILD_JUCE_APP=ON
  -DBATRUC_JUCE_SOURCE_DIR=…`).
- **License:** AGPL-3.0-or-later.

## Français / Español

- **Français :** aucune version publiée pour l’instant ; seul Linux a été
  compilé et testé. Compilation : voir les commandes ci-dessus. Licence
  AGPL-3.0-or-later.
- **Español:** todavía no hay versión publicada; solo Linux se ha compilado
  y probado. Compilación: ver los comandos de arriba. Licencia
  AGPL-3.0-or-later.

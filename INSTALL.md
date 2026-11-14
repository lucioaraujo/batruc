# Batruc — instalação / installation

> **Estado (14 nov. 2026):** versão 0.1.1 (a primeira publicada foi a
> 0.1.0). Os pacotes saem da integração contínua do repositório
> `lucioaraujo/batruc`.
> *English:* version 0.1.1 (the first release was 0.1.0); packages are
> built by CI.

## Matriz de plataformas

Cada combinação está marcada com o que foi **realmente feito**, não com o
que deveria funcionar.

| Plataforma | Build | Testes (28) | Abertura | Áudio tocado | Pacote |
|---|---|---|---|---|---|
| Linux x86-64 | ✅ local e CI (Ubuntu 22.04) | ✅ local e CI | ✅ local; CI em Debian 12, Ubuntu 24.04, Fedora e Arch | ✅ em hardware real | `.deb`, AppImage, `.tar.gz` |
| Windows x86-64 | ✅ CI (clang-cl) | ✅ CI | ❌ nunca aberto num Windows real | ❌ | instalador `.exe` (NSIS) e `.zip` |
| macOS (Universal 2) | ✅ CI | ✅ CI | ❌ nunca aberto num Mac real | ❌ | `.dmg` (assinatura ad-hoc) |

O workflow `.github/workflows/package.yml` (porte do Rasgo Modular) compila,
testa e empacota nos três sistemas, e testa a abertura do pacote Linux em
quatro distribuições. As rodadas e o que cada uma corrigiu estão em
[`PUBLICACAO.md`](PUBLICACAO.md).

## Português

### Instalar, passo a passo

Os avisos de segurança abaixo **são esperados**: não indicam defeito nem
vírus e só aparecem na primeira vez. Os pacotes ficam na
[página de releases](https://github.com/lucioaraujo/batruc/releases).

#### Windows

1. Baixe `batruc-<versão>-windows-x64.exe`.
2. Abra o arquivo. O Windows mostra uma janela azul **"O Windows protegeu o
   computador"** (ou outra mensagem dizendo que o programa não é
   reconhecido). Clique em **"Mais informações"** e depois em
   **"Executar assim mesmo"**.
3. Se o Windows pedir permissão de administrador, clique em **"Sim"**.
4. Clique em **"Avançar"** até o fim. Depois disso, o Batruc fica no
   **Menu Iniciar**.

**Sem instalar:** baixe o `.zip`, descompacte-o e abra `Batruc.exe`. O
aviso do passo 2 aparece do mesmo jeito na primeira vez.

#### macOS

1. Baixe `batruc-<versão>-macos-universal.dmg`. O mesmo arquivo serve para
   Mac Intel e Apple Silicon.
2. Abra o `.dmg` e **arraste o Batruc para a pasta Aplicativos**.
3. Abra o Batruc em Aplicativos. Na primeira vez, o macOS **bloqueia** o app
   e diz que **a Apple não pôde confirmar que ele está livre de software
   malicioso** (ou "não é possível verificar o desenvolvedor", ou "está
   danificado"). Isso **não indica defeito nem vírus**: o pacote tem
   assinatura ad-hoc, sem Developer ID nem notarização. Clique em **"OK"**
   ou **"Concluído"**, **não** em "Mover para o Lixo".
4. Libere o app uma vez, de um destes dois jeitos:
   - **Pelos Ajustes:** **Ajustes do Sistema → Privacidade e Segurança**,
     na parte de segurança, clique em **"Abrir Mesmo Assim"** ao lado de
     "Batruc foi bloqueado…", confirme e clique em **"Abrir"**. No macOS 14
     ou anterior há um atalho: no Finder, botão direito no app →
     **"Abrir"** → **"Abrir"**.
   - **Pelo Terminal**, se o botão não aparecer: cole a linha abaixo e
     aperte Enter (se nada aparecer, deu certo). Ela só retira a marca de
     quarentena que o macOS põe em todo arquivo baixado.

     ```sh
     xattr -dr com.apple.quarantine "/Applications/Batruc.app"
     ```

Se ainda assim não abrir, escreva para **rasgo.instruments@gmail.com** com
a versão do sistema e a mensagem que aparece.

#### Linux

- **Ubuntu, Debian e Mint:** dê dois cliques no `.deb` ou use
  `sudo apt install ./batruc-<versão>-linux-x86_64.deb`.
- **Outras distribuições:** use o **AppImage** (marque como executável e
  abra) ou o **`.tar.gz`** (descompacte e rode `./install.sh`, que instala
  em `~/.local`, sem root). Requer x86-64 e glibc 2.35 ou mais nova.

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

### Installing, step by step

The security warnings below **are expected**: they do not mean anything is
wrong, and they only appear the first time. Packages are on the
[releases page](https://github.com/lucioaraujo/batruc/releases).

#### Windows

1. Download `batruc-<version>-windows-x64.exe` and open it.
2. Windows shows a blue **"Windows protected your PC"** box. Click
   **"More info"**, then **"Run anyway"**. If asked for administrator
   permission, click **"Yes"**.
3. Click **"Next"** to the end. Batruc is then in the **Start menu**.

**Without installing:** unzip the `.zip` and open `Batruc.exe`.

#### macOS

1. Download `batruc-<version>-macos-universal.dmg` (Intel and Apple
   Silicon), open it and **drag Batruc into Applications**.
2. The first time, macOS **blocks** the app, saying Apple could not confirm
   it is free of malicious software. This is **not a defect or a virus**:
   the package is ad-hoc signed, without Developer ID or notarisation.
   Click **"OK"** or **"Done"**, **not** "Move to Trash".
3. Allow it once: **System Settings → Privacy & Security → "Open Anyway"**
   (on macOS 14 or earlier: right-click the app → **Open → Open**). If the
   button does not appear, run this in Terminal (no output means it
   worked):

   ```sh
   xattr -dr com.apple.quarantine "/Applications/Batruc.app"
   ```

If it still does not open, write to **rasgo.instruments@gmail.com**.

#### Linux

- **Ubuntu, Debian and Mint:** double-click the `.deb`, or run
  `sudo apt install ./<file>.deb`.
- **Other distributions:** the **AppImage** (make it executable, then open
  it) or the **`.tar.gz`** (unpack it and run `./install.sh`, which installs
  for your user only). x86-64, glibc 2.35+.

### Build from source

See the commands above (CMake ≥ 3.22, C++17, JUCE 9.0.3 exactly;
`-DBATRUC_BUILD_JUCE_APP=ON -DBATRUC_JUCE_SOURCE_DIR=…`).

### License

AGPL-3.0-or-later.

## Français / Español

- **Français :** paquets pour Linux, Windows et macOS (compilés et testés en
  CI ; seul Linux a été joué sur une vraie machine). Installation pas à pas :
  voir la section anglaise. Compilation : voir les commandes ci-dessus. Licence
  AGPL-3.0-or-later.
- **Español:** paquetes para Linux, Windows y macOS (compilados y probados
  en CI; solo Linux se ha tocado en una máquina real). Instalación paso a
  paso: ver la sección en inglés. Compilación: ver los comandos de arriba. Licencia
  AGPL-3.0-or-later.

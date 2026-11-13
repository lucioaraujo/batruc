#!/bin/sh
# Instala o Batruc na sua pasta pessoal (~/.local), sem root.
#   ./install.sh            instala (ou atualiza)
#   ./install.sh --remove   remove o que este script instalou
# Outro destino: PREFIX=/opt/rasgo ./install.sh
set -eu
AQUI=$(cd "$(dirname "$0")" && pwd)
PREFIX=${PREFIX:-"$HOME/.local"}
APPS="$PREFIX/share/applications"
ICONES="$PREFIX/share/icons/hicolor"

if [ "${1:-}" = "--remove" ]; then
    rm -f "$PREFIX/bin/batruc" \
          "$APPS/batruc.desktop" \
          "$ICONES/scalable/apps/batruc.svg" \
          "$ICONES/512x512/apps/batruc.png"
    echo "Batruc removido de / removed from $PREFIX."
    echo "Preferências / settings: ~/.config/RASGO/Batruc.settings (cenas ficam onde você salvou / scenes stay where you saved them)"
    exit 0
fi

mkdir -p "$PREFIX/bin" "$APPS" "$ICONES/scalable/apps" "$ICONES/512x512/apps"
cp "$AQUI/bin/batruc" "$PREFIX/bin/"
cp "$AQUI/share/icons/hicolor/scalable/apps/batruc.svg" "$ICONES/scalable/apps/"
cp "$AQUI/share/icons/hicolor/512x512/apps/batruc.png" "$ICONES/512x512/apps/"
# caminho absoluto no Exec: ~/.local/bin nem sempre está no PATH do menu
sed "s|^Exec=.*|Exec=\"$PREFIX/bin/batruc\"|" \
    "$AQUI/share/applications/batruc.desktop" > "$APPS/batruc.desktop"
command -v update-desktop-database >/dev/null 2>&1 && update-desktop-database "$APPS" 2>/dev/null || true
echo "Batruc instalado em / installed to $PREFIX/bin/batruc"
echo "(no menu de aplicativos / in the application menu)"

#!/usr/bin/env python3
"""Recortes das capturas reais do app para o Tutorial e o LEARN do site
(14/11/2026).

As capturas vêm da auditoria de layout, uma pasta por língua:

    for l in 0 1 2 3; do
        BATRUC_AUDIT_LANGUAGE=$l ./run --audit-layout=<dir>/$l
    done
    python3 tools/recortar_capturas_site.py <dir>

(0 pt, 1 en, 2 es, 3 fr, a ordem do enum UiLanguage). Saída em
website/assets/shots/<pt|en|es|fr>/<nome>.jpg. As coordenadas são da janela
de 1920×1080; se o layout mudar, conferir com uma grade antes de recortar.
"""
import pathlib
import sys

from PIL import Image

RAIZ = pathlib.Path(__file__).resolve().parent.parent
SAIDA = RAIZ / "website" / "assets" / "shots"
LINGUAS = {"0": "pt", "1": "en", "2": "es", "3": "fr"}
LARGURA_MAX = 1400

# nome: (captura, (x0, y0, x1, y1))
RECORTES = {
    "cabecalho": ("1920-top.png", (300, 0, 1920, 75)),  # aparado à esquerda
    "barra": ("1920-top.png", (0, 990, 1100, 1033)),
    "study": ("1920-top.png", (350, 95, 1225, 960)),
    "study-botoes": ("1920-top.png", (355, 155, 1100, 255)),
    "campo": ("1920-top.png", (20, 95, 335, 905)),
    "vista": ("1920-top.png", (1245, 95, 1900, 760)),
    "vista-controles": ("1920-top.png", (1245, 95, 1900, 330)),
    "mixer-stems": ("1920-mixer.png", (50, 230, 960, 550)),
    "mixer-canais": ("1920-mixer.png", (50, 570, 960, 1000)),
    "mixer-baixo": ("1920-mixer.png", (420, 160, 1500, 220)),
    "baixo": ("1920-bass-top.png", (40, 90, 1880, 540)),
    "pauta-bateria": ("1920-drum-staff.png", (360, 160, 1220, 980)),
}


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    base = pathlib.Path(sys.argv[1])
    total = 0
    for pasta, lingua in LINGUAS.items():
        destino = SAIDA / lingua
        destino.mkdir(parents=True, exist_ok=True)
        for nome, (captura, caixa) in RECORTES.items():
            imagem = Image.open(base / pasta / captura).convert("RGB").crop(caixa)
            if nome == "cabecalho":
                # o bloco de botões se alinha à direita e muda de largura com a
                # língua: apara o fundo vazio à esquerda, com 12 px de folga
                cinza = imagem.convert("L")
                colunas = [x for x in range(cinza.width)
                           if min(cinza.getpixel((x, y)) for y in range(10, 60)) < 120]
                if colunas:
                    imagem = imagem.crop((max(0, colunas[0] - 12), 0, imagem.width, imagem.height))
            if imagem.width > LARGURA_MAX:
                altura = round(imagem.height * LARGURA_MAX / imagem.width)
                imagem = imagem.resize((LARGURA_MAX, altura), Image.LANCZOS)
            arquivo = destino / (nome + ".jpg")
            imagem.save(arquivo, quality=84, optimize=True, progressive=True)
            total += arquivo.stat().st_size
    print("recortes: %d imagens, %.1f MB" % (len(LINGUAS) * len(RECORTES), total / 1e6))


if __name__ == "__main__":
    main()

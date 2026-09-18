# Linguagens de descrição de hardware — 21/09/2026

Apresentação Beamer: `main.tex`; versão para apresentação: `main.pdf`.
Os 35 slides seguem a sequência `page_01.tex` a `page_35.tex`, na ordem
da apresentação. `page_01.tex` contém os dados da capa.
Páginas antigas não utilizadas foram preservadas em
`arquivo/paginas_antigas_nao_utilizadas.zip`.

As imagens estão em `images/`; fontes e créditos estão em
`images/CREDITOS.md`. As imagens ficam incorporadas ao PDF, sem depender
de acesso à internet durante a apresentação.

## Gerar PDF

```sh
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```

## Simular a porta AND

Requer GHDL com suporte a VHDL-2008; GTKWave para visualizar as ondas.

```sh
cd exemplos
ghdl -a --std=08 porta_and.vhd tb_porta_and.vhd
ghdl -e --std=08 tb_porta_and
ghdl -r --std=08 tb_porta_and --assert-level=error --vcd=and.vcd
gtkwave and.vcd
```

Esperado: quatro combinações verificadas sem erro e término em 40 ns.
`mux2.vhd`, `ff_d.vhd`, `contador4.vhd` e `controle_esteira.vhd`
são os demais modelos usados nos slides.

O Quartus Prime suporta VHDL. Selecione VHDL-2008 para os exemplos
que usam `process(all)`. O testbench é destinado ao simulador, não à síntese.


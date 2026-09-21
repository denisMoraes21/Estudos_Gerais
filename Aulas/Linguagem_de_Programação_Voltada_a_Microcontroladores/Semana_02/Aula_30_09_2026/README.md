# Aula 30/09/2026 — Depuração e testes

Semana 02 de Linguagem de Programação Voltada a Microcontroladores.

## Conteúdo e materiais

28 slides: objetivos, especificação do monitor, categorias de erros, avisos do compilador, método de investigação, rastreamento, `printf`, GDB, correções de limites, casos de teste, validação, `assert`, regressão, prática e exercícios com conferência.

Os exemplos C11 executam no computador; não precisam de placa. `main.pdf` está pronto para apresentação. Para editar, use `main.tex`, `page_*.tex`, `ppgee_style.sty` e `images/`. Na pasta da aula:

```sh
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```

## Executar o monitor e os testes

Na pasta da aula, com GCC:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 exemplos/monitor.c exemplos/regras.c -o /tmp/aula30_monitor
/tmp/aula30_monitor
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/testes.c exemplos/regras.c -o /tmp/aula30_testes
/tmp/aula30_testes
```

Saídas esperadas:

```text
Ocorrencias: 2
```

```text
6 testes de contagem passaram
6 testes de validacao passaram
```

O arquivo `regras.h` declara a interface; `regras.c` contém a implementação compartilhada pelo monitor e pelos testes. Os comandos compilam os dois arquivos `.c` necessários em cada programa. Essa organização reutiliza a separação entre interface e implementação da aula anterior.

O contrato de `contar_no_limite` exige armazenamento válido para a quantidade informada. Quando a quantidade é zero, nenhum elemento é acessado e `NULL` é aceito. `temperatura_valida` adota a faixa didática inclusiva de -40 a 125 °C. Os testes de entrada inválida verificam essa função de validação; não passam ponteiros inválidos ao processamento.

Não defina `NDEBUG`: ele desativa `assert`. A suíte inclui uma verificação de pré-processamento para impedir execução enganosa com as asserções desativadas. Arquivos `.c` contêm somente exemplos corretos; fragmentos intencionalmente defeituosos estão em `fragmentos_incorretos.txt`.

## Roteiro de depuração com GDB

Compile com `-g -O0` usando o comando anterior e inicie:

```sh
gdb /tmp/aula30_monitor
```

Dentro do GDB:

```text
break contar_no_limite
run
print quantidade
print limite
print valores[1]
list
next
print total
backtrace
continue
quit
```

Na primeira parada, quantidade = 3, limite = 30 e valores[1] = 30. Execute `next` conforme as linhas exibidas, até inicializar `total`, antes de examiná-lo. O total após cada leitura é 0, 1 e 2. Use `display i` somente quando `i` estiver no escopo. GDB precisa estar instalado; rastreamento manual e mensagens são alternativas para a atividade.

Opcionalmente, em um computador com suporte aos sanitizadores:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 -fsanitize=address,undefined exemplos/testes.c exemplos/regras.c -o /tmp/aula30_sanitizado
/tmp/aula30_sanitizado
```

## Prática e exercícios

Os slides contêm prática guiada, exercícios e conferência. Consulte o [gabarito](gabarito.md) para respostas, resultados esperados e orientações de condução.

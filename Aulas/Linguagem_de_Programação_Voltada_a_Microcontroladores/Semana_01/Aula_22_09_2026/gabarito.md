# Gabarito — 22/09/2026

## Atividade guiada: nível

Entrada: nível de 0 a 100%. Processamento: comparar nível com 20%.
Saída: estado do sinalizador. Para esta atividade, assume-se leitura válida.

```text
INÍCIO
  LER nível
  SE nível < 20 ENTÃO
    sinalizador ← LIGADO
  SENÃO
    sinalizador ← DESLIGADO
  FIM_SE
  ESCREVER sinalizador
FIM
```

Fluxo: início → ler nível → decisão “nível < 20?”; ramo Sim → ligar;
ramo Não → desligar; reunir os ramos → exibir estado → fim.
Respostas: 0 → ligado; 19 → ligado; 20 → desligado; 100 → desligado.

## Exercícios

1. Entradas: total = 14 e capacidade = 4. Contar quantos grupos completos
   cabem no total (3); obter a sobra (2). Saídas: 3 caixas completas e 2 peças.
   Algoritmo alternativo: retirar grupos de 4 enquanto houver pelo menos 4
   peças, contar cada grupo e informar a quantidade restante.
2. Temperatura = 30: pela regra liga, mas o algoritmo errado desliga.
3. Início → ler botão → “pressionado?”; Sim → ligar; Não → desligar;
   reunir ramos → fim do ciclo. Não deixar a saída indefinida no ramo Não.
4. a) Imperativa; b) procedural; c) orientada a objetos. As categorias podem
   coexistir: uma função procedural também pode conter comandos imperativos.

Critérios: entradas/saídas corretas, comparação adequada, cobertura dos ramos,
equivalência entre texto e desenho e resultados de mesa justificados.

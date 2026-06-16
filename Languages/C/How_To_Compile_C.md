# How to compile C file

## Simple compilation

- gcc main.c -o main

## Compilation with warnings

- gcc -Wall -Wextra -Wpedantic main.c -o main

## Compilation with C version target

- gcc -std=c11 main.c -o main

## How to know C version from my linux environment

- gcc -dM -E - < /dev/null | grep __STDC_VERSION__

199409L → C95

199901L → C99

201112L → C11

201710L → C17



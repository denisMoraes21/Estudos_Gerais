#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(realpath "$SCRIPT_DIR/../../Makefile/CM7")"

cd "$PROJECT_DIR" || exit 1

STM32_Programmer_CLI -c port=SWD -w build/teste_blink_02_CM7.elf -v -run
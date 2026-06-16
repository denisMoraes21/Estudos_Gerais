# Primeiros passos

## Atualizar ambiente

- sudo apt update
- sudo apt install git wget flex bison gperf python3 python3-pip cmake ninja-build ccache libffi-dev libssl-dev dfu-util

## Clonar ESP-IDF

- mkdir -p ~/esp
- cd ~/esp
- git clone --recursive https://github.com/espressif/esp-idf.git

# Instalar a ferramenta
Fazer a instalação conforme o microcontrolador utilizado.

- cd esp-idf
- ./install.sh esp32

# Ativar o ambiente

- source export.sh

# Setar alvo

- idf.py set-target esp32

# Construir o projeto

- idf.py build

# Gravar projeto no microcontrolador alvo

- idf.py -p /dev/ttyUSB0 -b 115200 flash monitor

# Limpar build

- idf.py clean





#!/usr/bin/env python3
"""Exercita os sketches no computador com GPIO e relógio substituídos.

Não compila para AVR nem verifica características elétricas da placa.
"""
from pathlib import Path
import subprocess
import tempfile

LESSON = Path(__file__).resolve().parents[1]
MOCK = r"""
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>
constexpr int LOW = 0, HIGH = 1;
constexpr int INPUT = 0, OUTPUT = 1, INPUT_PULLUP = 2;
constexpr int LED_BUILTIN = 13;
int levels[20] = {};
int modes[20] = {};
unsigned long clockMs = 0;
std::vector<unsigned long> waits;
std::vector<int> ledWrites;
void pinMode(int pin, int mode) {
    modes[pin] = mode;
    if (mode == INPUT_PULLUP) levels[pin] = HIGH;
}
int digitalRead(int pin) { return levels[pin]; }
void digitalWrite(int pin, int value) {
    assert(modes[pin] == OUTPUT);
    levels[pin] = value;
    if (pin == LED_BUILTIN) ledWrites.push_back(value);
}
unsigned long millis() { return clockMs; }
void delay(unsigned long ms) { waits.push_back(ms); clockMs += ms; }
struct SerialMock {
    unsigned long baud = 0;
    std::vector<int> values;
    void begin(unsigned long value) { baud = value; }
    void println(int value) { values.push_back(value); }
} Serial;
"""

TESTS = {
    "pisca": r"""
    setup();
    assert(levels[LED_BUILTIN] == LOW);
    ledWrites.clear();
    loop();
    assert((ledWrites == std::vector<int>{HIGH, LOW}));
    assert((waits == std::vector<unsigned long>{500, 500}));
    assert(clockMs == 1000);
""",
    "botao_led": r"""
    setup();
    assert(modes[2] == INPUT_PULLUP && Serial.baud == 9600);
    for (int input : {HIGH, LOW, HIGH}) {
        levels[2] = input;
        loop();
        assert(levels[LED_BUILTIN] == (input == LOW ? HIGH : LOW));
        assert(Serial.values.back() == (input == LOW));
    }
    assert((waits == std::vector<unsigned long>{100, 100, 100}));
""",
    "sinalizador": r"""
    setup();
    assert(modes[2] == INPUT_PULLUP && modes[3] == INPUT_PULLUP);
    assert(levels[LED_BUILTIN] == LOW);
    for (int request : {HIGH, LOW}) {
        for (int permission : {HIGH, LOW}) {
            levels[2] = request;
            levels[3] = permission;
            loop();
            const bool expected = request == LOW && permission == LOW;
            assert(levels[LED_BUILTIN] == (expected ? HIGH : LOW));
        }
    }
    // Release each button from the active state independently.
    for (int released : {2, 3}) {
        levels[2] = LOW; levels[3] = LOW; loop();
        assert(levels[LED_BUILTIN] == HIGH);
        levels[released] = HIGH; loop();
        assert(levels[LED_BUILTIN] == LOW);
    }
    assert(waits.empty());
""",
    "pisca_sem_bloqueio": r"""
    setup();
    assert(levels[LED_BUILTIN] == LOW);
    clockMs = 499; loop(); assert(levels[LED_BUILTIN] == LOW);
    clockMs = 500; loop(); assert(levels[LED_BUILTIN] == HIGH);
    clockMs = 999; loop(); assert(levels[LED_BUILTIN] == HIGH);
    clockMs = 1000; loop(); assert(levels[LED_BUILTIN] == LOW);
    // Exercise unsigned subtraction across host unsigned long wrap.
    aceso = false;
    clockMs = ULONG_MAX - 100;
    setup();
    clockMs = 398; loop(); assert(levels[LED_BUILTIN] == LOW);
    clockMs = 399; loop(); assert(levels[LED_BUILTIN] == HIGH);
    assert(waits.empty());
""",
}


def main():
    with tempfile.TemporaryDirectory(prefix="aula-gpio-") as directory:
        folder = Path(directory)
        for name, checks in TESTS.items():
            sketch = LESSON / "exemplos" / name / f"{name}.ino"
            source = folder / f"{name}.cpp"
            binary = folder / name
            source.write_text(MOCK + sketch.read_text() + "\nint main() {\n"
                              + checks + '\nstd::cout << "OK\\n";\n}\n')
            subprocess.run(["g++", "-std=c++11", "-Wall", "-Wextra", "-Wpedantic",
                            "-Werror", str(source), "-o", str(binary)], check=True)
            result = subprocess.run([str(binary)], check=True, capture_output=True, text=True)
            print(f"{name}: {result.stdout.strip()}")
    print("4 sketches verificados no computador; validação física ainda depende da placa.")


if __name__ == "__main__":
    main()

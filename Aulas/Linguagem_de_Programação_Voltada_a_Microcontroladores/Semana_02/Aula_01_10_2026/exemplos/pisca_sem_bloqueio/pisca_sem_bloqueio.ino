const unsigned long INTERVALO_MS = 500;
unsigned long anterior = 0;
bool aceso = false;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    anterior = millis();
}
// Cada ciclo consulta o tempo, sem esperar parado.

void loop() {
    unsigned long agora = millis();
    if (agora - anterior >= INTERVALO_MS) {
        anterior = agora;
        aceso = !aceso;
        if (aceso) digitalWrite(LED_BUILTIN, HIGH);
        else digitalWrite(LED_BUILTIN, LOW);
    }
    // Outras tarefas curtas podem ser chamadas aqui.
}

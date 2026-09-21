const int BOTAO = 2;

void setup() {
    pinMode(BOTAO, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.begin(9600);
}

void loop() {
    bool pressionado = (digitalRead(BOTAO) == LOW);
    if (pressionado) digitalWrite(LED_BUILTIN, HIGH);
    else digitalWrite(LED_BUILTIN, LOW);
    Serial.println(pressionado);
    delay(100);
}

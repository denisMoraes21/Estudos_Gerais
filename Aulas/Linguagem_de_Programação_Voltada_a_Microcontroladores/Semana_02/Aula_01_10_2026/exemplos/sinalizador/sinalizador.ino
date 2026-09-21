const int SOLICITACAO = 2;
const int PERMISSAO = 3;

bool autorizar(bool solicitacao, bool permissao) {
    return solicitacao && permissao;
}

void setup() {
    pinMode(SOLICITACAO, INPUT_PULLUP);
    pinMode(PERMISSAO, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

// Entradas ativas em baixo: pressionado = LOW.
// Modelo didatico: a saida e apenas o LED integrado.

void loop() {
    bool pedido = (digitalRead(SOLICITACAO) == LOW);
    bool permitido = (digitalRead(PERMISSAO) == LOW);
    bool ligar = autorizar(pedido, permitido);
    if (ligar) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

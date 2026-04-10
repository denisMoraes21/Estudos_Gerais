from flask import Flask, request, jsonify
from functools import wraps

USERNAME = "admin"
PASSWORD = "1234"

def check_auth(username, password):
    return username == USERNAME and password == PASSWORD

def authenticate():
    return jsonify({"erro": "Autenticação necessária"}), 401, {
        "WWW-Authenticate": 'Basic realm="Login Required"'
    }

def requires_auth(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        auth = request.authorization

        if not auth or not check_auth(auth.username, auth.password):
            return authenticate()

        return f(*args, **kwargs)
    return decorated

app = Flask(__name__)

@app.route("/")
def home():
    return "Servidor rodando!"

@app.route("/dados", methods=["POST"])
@requires_auth
def receber_dados():
    data = request.get_json()

    if not data:
        return jsonify({"erro": "Nenhum JSON enviado"}), 400

    nome = data.get("nome", "desconhecido")

    return jsonify({
        "mensagem": "Dados recebidos com sucesso",
        "nome": nome,
        "dados": data
    }), 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)


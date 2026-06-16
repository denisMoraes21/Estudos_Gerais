public class TreinadorJedi {
    String titulacao;
    String nome;

    TreinadorJedi() {
        this("Mestre", "Obi-Wan Kenobi");
    }

    TreinadorJedi(String titulacao, String nome) {
        this.titulacao = titulacao;
        this.nome = nome;
    }

    String getDescricao() {
        return String.format(
            "%s %s", 
            this.titulacao, 
            this.nome);
    }
}

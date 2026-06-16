public class IniciadoJedi {
    String nome;
    String especie;
    int anoNascimento;

    public IniciadoJedi() {
        this("Luke Skywalker", "Humano", 19);
    }

    public IniciadoJedi(String nome, String especie, int anoNascimento) {
        this.nome = nome;
        this.especie = especie;
        this.anoNascimento = anoNascimento;
    }

    String getDescricao() {
      return String.format(
            "%s (especie=%s, nascimento=%s)", 
            this.nome, 
            this.especie, 
            this.getAnoNascimento());
    }

    String getAnoNascimento() {
        String description = this.anoNascimento < 0 ? "ABY" : "DBY";
        return String.format(
            "%d %s", 
            Math.abs(this.anoNascimento),
            description,
            Math.abs(this.anoNascimento));
    }
}
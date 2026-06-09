import java.util.ArrayList;


public class SessaoJedi {
    String nome;
    TreinadorJedi treinador;
    ArrayList<IniciadoJedi> iniciados;

    public SessaoJedi(String nome, TreinadorJedi treinador) {
        this.nome = nome;
        this.treinador = treinador;
        this.iniciados = new ArrayList<IniciadoJedi>();
    }

    public void addIniciado(IniciadoJedi iniciado) {
        if (!this.iniciados.contains(iniciado)){
            this.iniciados.add(iniciado);
        }
    }
    
    IniciadoJedi getIniciado(String nome) {
        for (IniciadoJedi iniciado : this.iniciados) {
            if (iniciado.nome.equals(nome)) {
                return iniciado;
            }
        }
        return null;
    }

    double getMediaAnoNascimento() {
        double media = 0;
        for (IniciadoJedi iniciado : this.iniciados) {
            media += iniciado.anoNascimento;
        }
        return media / this.iniciados.size();
    }

    String getDescricao() {
        String sessao_text = String.format("--> SESSÃO %s (Treinador: %s) \n", this.nome, this.treinador.getDescricao());
        for (int i = 0; i < this.iniciados.size(); i++) {
            sessao_text += String.format("  - Iniciado %d: %s \n", i + 1, this.iniciados.get(i).getDescricao());
        }
        return sessao_text; 
    }
}
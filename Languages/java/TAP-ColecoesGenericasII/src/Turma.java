import java.util.ArrayList;

public class Turma {
    String nome;
    String professor;
    int numAlunos;
    boolean acessivel;
    ArrayList<Integer> horarios;

    public Turma() {
        horarios = new ArrayList<>();
    }

    public Turma(String nome, String professor, int numAlunos, boolean acessivel) {
        this();
        this.nome = nome;
        this.professor = professor;
        this.numAlunos = numAlunos;
        this.acessivel = acessivel;
    }

    public void addHorario(int horario) {
        horarios.add(horario);
    }

    public String getHorariosString() {
        String result = "";
        String[] dias = {
                "segunda",
                "terça",
                "quarta",
                "quinta",
                "sexta"
            };

        for (int i = 0; i < horarios.size(); i++) {
            int horario = horarios.get(i);

            String dia = dias[(horario - 1) / 7];
            int hora = 8 + 2 * ((horario - 1) % 7);

            result += String.format("%s %dhs", dia, hora);

            if (i < horarios.size() - 1) {
                result += ", ";
            }
        }
        return result;
    }

    public String getDescricao() {
        String turma = String.format("Turma: %s\n", this.nome);
        String professor = String.format("Professor: %s\n", this.professor);
        String numAlunos = String.format("Número de Alunos: %d\n", this.numAlunos);
        String horarios = String.format("Horário: %s\n", getHorariosString());
        String acessibilidade = String.format("Acessível: %s", this.acessivel ? "sim" : "não");
        return turma + professor + numAlunos + horarios + acessibilidade;
    }
}

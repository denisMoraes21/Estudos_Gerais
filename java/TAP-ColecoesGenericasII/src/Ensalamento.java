import java.util.ArrayList;


public class Ensalamento {
    ArrayList<Turma> turmas;
    ArrayList<Sala> salas;
    ArrayList<TurmaEmSala> ensalamento;

    public Ensalamento() {
        this.turmas = new ArrayList<>();
        this.salas = new ArrayList<>();
        this.ensalamento = new ArrayList<>();
    }

    public Ensalamento(ArrayList<Turma> turmas, ArrayList<Sala> salas, ArrayList<TurmaEmSala> ensalamento) {
        this.turmas = turmas;
        this.salas = salas;
        this.ensalamento = ensalamento;
    }   

    public void addSala(Sala sala) {
        salas.add(sala);
    }

    public void addTurma(Turma turma) {
        turmas.add(turma);
    }

    public Sala getSala(Turma turma) {
        for (TurmaEmSala tes : ensalamento) {
            if (tes.turma == turma) {
                return tes.sala;
            }
        }
        return null;
    }

    public boolean salaDisponivel(Sala sala, int horario) {
        for (TurmaEmSala tes : ensalamento) {
            if (tes.sala == sala) {
                for (int h : tes.turma.horarios) {
                    if (h == horario) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    public boolean salaDisponivel(Sala sala, ArrayList<Integer> horarios) {
        for (int horario : horarios) {
            if (!salaDisponivel(sala, horario)) {
                return false;
            }
        }
        return true;
    }

    public boolean turmaEnsalada(Turma turma) {
        for (TurmaEmSala tes : ensalamento) {
            if (tes.turma == turma) {
                return true;
            }
        }
        return false;
    }

    public boolean alocar(Turma turma, Sala sala) {
        if (sala.capacidade < turma.numAlunos) {
            return false;
        }
        if (turma.acessivel && !sala.acessivel) {
            return false;
        }
        if (!salaDisponivel(sala, turma.horarios)) {
            return false;
        }
        ensalamento.add(new TurmaEmSala(turma, sala));
        return true;
    }

    public void alocarTodas() {
        for (Turma turma : turmas) {
            if (!turmaEnsalada(turma)) {
                for (Sala sala : salas) {
                    if (alocar(turma, sala)) {
                        break;
                    }
                }
            }
        }
    }

    public int getTotalTurmasAlocadas() {
        int count = 0 ;
        for (TurmaEmSala tes : ensalamento) {
            if (tes.sala != null) {
                count++;
            }
        }
        return count;
    }

    public int getTotalEspacoLivre() {
        int totalEspacoLivre = 0;
        for (TurmaEmSala tes : ensalamento) {
            if (tes.turma != null && tes.sala != null) {
                totalEspacoLivre += tes.sala.capacidade - tes.turma.numAlunos;
            }
        }
        return totalEspacoLivre;
    }

    public String relatorioResumoEnsalamento() {
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("Total de Salas: %d\n", salas.size()));
        sb.append(String.format("Total de Turmas: %d\n", turmas.size()));
        sb.append(String.format("Turmas Alocadas: %d\n", getTotalTurmasAlocadas()));
        sb.append(String.format("Espaços Livres: %d\n\n", getTotalEspacoLivre()));
        return sb.toString();
    }

    public String relatorioTurmasPorSala() {
        StringBuilder sb = new StringBuilder();
        sb.append(this.relatorioResumoEnsalamento());
        for (Sala sala : salas) {
            sb.append("--- ")
            .append(sala.getDescricao())
            .append(" ---\n");
            for (TurmaEmSala tes : ensalamento) {
                if (tes.sala == sala) {
                    sb.append("\n");
                    sb.append(tes.turma.getDescricao());
                    sb.append("\n");
                }
            }
        }
        return sb.toString();
    }

    public String relatorioSalasPorTurma() {
        StringBuilder sb = new StringBuilder();

        sb.append(relatorioResumoEnsalamento());
        sb.append("\n");

        for (Turma turma : turmas) {

            sb.append(turma.getDescricao());

            Sala sala = getSala(turma);

            if (sala != null) {
                sb.append("\nSala: ");
                sb.append(sala.getDescricao());
            } else {
                sb.append("\nSala: SEM SALA");
            }

            sb.append("\n\n");
        }

        return sb.toString();
    }
}

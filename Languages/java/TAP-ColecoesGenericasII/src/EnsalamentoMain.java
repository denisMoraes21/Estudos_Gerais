public class EnsalamentoMain {
    public static void main(String[] args){
        Ensalamento ensalamento = new Ensalamento();
        ensalamento.addSala(new Sala(6, 101, 50, true));
        ensalamento.addSala(new Sala(6, 102, 100, true));
        ensalamento.addSala(new Sala(6, 203, 50, false));
        ensalamento.addSala(new Sala(6, 204, 100, false));  
        Turma turma1 = new Turma("Técnicas de Programação", "Horácio Fernandes", 50, false);
        turma1.addHorario(4);
        turma1.addHorario(18);   
        turma1.addHorario(32);  

        System.out.println("Hello, World!");
    }
}

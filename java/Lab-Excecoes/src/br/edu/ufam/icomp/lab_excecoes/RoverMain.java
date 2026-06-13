package br.edu.ufam.icomp.lab_excecoes;
public class RoverMain {
    public static void main(String[] args) {
        Caminho caminho = new Caminho(10);

        try {

            caminho.addCoordenada(new Coordenada(32, 30, 2));
            caminho.addCoordenada(new Coordenada(35, 40, 5));
            caminho.addCoordenada(new Coordenada(38, 30, 8));
            caminho.addCoordenada(new Coordenada(30, 36, 6));
            caminho.addCoordenada(new Coordenada(40, 36, 6));
            caminho.addCoordenada(new Coordenada(33, 31, 4));

        } catch (RoverException e) {

            System.out.println(e.getMessage());
            caminho.reset();

        }

        System.out.println(caminho);    
    }
}

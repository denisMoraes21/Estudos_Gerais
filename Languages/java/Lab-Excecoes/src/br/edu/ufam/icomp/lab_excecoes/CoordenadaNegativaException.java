package br.edu.ufam.icomp.lab_excecoes;

public class CoordenadaNegativaException extends RoverCoordenadaException {
    
    public CoordenadaNegativaException(){

    }
    public CoordenadaNegativaException(String menssagem) {
        super(menssagem);
    }

    public String getMessage() {
        return "Coordenada com valor negativo";
    }

}

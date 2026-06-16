package br.edu.ufam.icomp.lab_excecoes;

public class CoordenadaForaDosLimitesException extends RoverCoordenadaException {
    
    public CoordenadaForaDosLimitesException() {

    }

    public CoordenadaForaDosLimitesException(String menssagem) {
        super(menssagem);
    }
    
    public String getMessage() {
        return "Coordenada com valores fora dos limites";
    }
    
}

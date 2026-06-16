package br.edu.ufam.icomp.lab_excecoes;

public class RoverCoordenadaException extends RoverException {
    
    public RoverCoordenadaException() {

    }
    public RoverCoordenadaException(String message) {
        super(message);
    }  
    
    public String getMessage() {
        return "Exceção geral de coordenada do rover";
    }
}

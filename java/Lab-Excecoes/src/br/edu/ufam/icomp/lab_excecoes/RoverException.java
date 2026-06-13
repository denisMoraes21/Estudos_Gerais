package br.edu.ufam.icomp.lab_excecoes;

public class RoverException extends Exception {
    
    public RoverException() {

    }

    public RoverException(String mensagem) {
        super(mensagem);
    }

    public String getMessage() {
        return "Exceção geral do rover";
    }
    
}

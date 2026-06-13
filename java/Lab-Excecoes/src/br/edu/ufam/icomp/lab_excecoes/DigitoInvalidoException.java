package br.edu.ufam.icomp.lab_excecoes;

public class DigitoInvalidoException extends RoverCoordenadaException {

    public DigitoInvalidoException() {

    }

    public DigitoInvalidoException(String mensagem) {
        super(mensagem);
    }

    public String getMessage() {
        return "Digito da coordenada inválido";
    }
    
}

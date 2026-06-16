package br.edu.ufam.icomp.lab_excecoes;

public class RoverCaminhoException extends RoverException {
    public RoverCaminhoException() {

    }

    public RoverCaminhoException(String menssagem) {
        super(menssagem);
    }

    public String getMessage() {
        return "Exceção geral de caminho do rover";
    }
}

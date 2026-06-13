package br.edu.ufam.icomp.lab_excecoes;

public class TamanhoMaximoExcedidoException extends RoverCaminhoException {

    public TamanhoMaximoExcedidoException() {

    }

    public TamanhoMaximoExcedidoException(String mensagem) {
        super(mensagem);
    }

    public String getMessage() {
        return "Quantidade máxima de coordenadas excedida";
    }
    
}

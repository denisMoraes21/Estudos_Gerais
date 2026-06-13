package br.edu.ufam.icomp.lab_excecoes;

public class DistanciaEntrePontosExcedidaException extends RoverCaminhoException {
    
    public DistanciaEntrePontosExcedidaException() {

    }

    public DistanciaEntrePontosExcedidaException(String mensagem) {
        super(mensagem);
    }

    public String getMessage() {
        return "Distância máxima entre duas coordenadas vizinhas excedida";
    }

}

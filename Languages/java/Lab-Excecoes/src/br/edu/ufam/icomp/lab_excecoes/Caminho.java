package br.edu.ufam.icomp.lab_excecoes;

public class Caminho {
    private Coordenada[] caminho;
    private int tamanho;

    public Caminho(int maxTam) {
        this.caminho = new Coordenada[maxTam];
        this.tamanho = 0;
    }

    public int tamanho() {
        return this.tamanho;
    }

    public void addCoordenada(Coordenada coordenada)
            throws TamanhoMaximoExcedidoException,
                   DistanciaEntrePontosExcedidaException {

        if (tamanho >= this.caminho.length) {
            throw new TamanhoMaximoExcedidoException();
        }

        if (tamanho > 0) {
            Coordenada ultima = this.caminho[tamanho - 1];

            if (ultima.distancia(coordenada) > 15.0) {
                throw new DistanciaEntrePontosExcedidaException();
            }
        }

        this.caminho[tamanho] = coordenada;
        tamanho++;
    }

    public void reset() {
        this.caminho = new Coordenada[this.caminho.length];
        tamanho = 0;
    }

    @Override
    public String toString() {

        String retorno = "Dados do caminho:\n";
        retorno += "  - Quantidade de pontos: " + tamanho + "\n";
        retorno += "  - Pontos:\n";

        for (int i = 0; i < tamanho; i++) {
            retorno += "    -> " + this.caminho[i] + "\n";
        }

        return retorno;
    }
}

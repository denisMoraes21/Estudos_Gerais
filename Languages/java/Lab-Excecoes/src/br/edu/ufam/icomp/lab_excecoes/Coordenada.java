package br.edu.ufam.icomp.lab_excecoes;

public class Coordenada {
    private int posX;
    private int posY;
    private int digito;

    public Coordenada(int posX, int posY, int digito) 
    throws CoordenadaNegativaException,
    CoordenadaForaDosLimitesException,
    DigitoInvalidoException
    
    {

        if (posX < 0 || posY < 0) {
            throw new CoordenadaNegativaException();
        }

        if (posX > 30000 || posY > 30000) {
            throw new CoordenadaForaDosLimitesException();
        }

        if (((posX + posY) % 10) != digito) {
            throw new DigitoInvalidoException();
        }

        this.posX = posX;
        this.posY = posY;
        this.digito = digito;
    }

    public int getPosX() {
        return this.posX;
    }

    public int getPosY() {
        return this.posY;
    }

    public double distancia(Coordenada coordenada) {
        int dx = this.posX - coordenada.posX;
        int dy = this.posY - coordenada.posY;

        return Math.sqrt(dx * dx + dy * dy);
    }

    public String toString() {
        return posX + ", " + posY;
    }
}

package br.edu.icomp.ufam.lab_heranca;


public abstract class FormaGeometrica {
    public int posX;
    public int posY;

    public FormaGeometrica(int posX, int posY) {
        this.posX = posX;
        this.posY = posY;
    }

    abstract public double getArea();

    abstract public double getPerimetro();

    public String getPosString() {
        return "posição (" + posX + ", " + posY + ")";
    }
}
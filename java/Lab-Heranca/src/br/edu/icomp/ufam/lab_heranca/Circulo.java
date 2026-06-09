package br.edu.icomp.ufam.lab_heranca;
import java.util.Locale;

public class Circulo extends FormaGeometrica {
    public double raio;
    
    public Circulo(int posX, int posY, double raio) {
        super(posX, posY);
        this.raio = raio;
    }

    @Override
    public double getArea() {
        return Math.PI * raio * raio;
    }

    @Override
    public double getPerimetro() {
        return 2 * Math.PI * raio;
    }

    public String toString() {
        return "Círculo na " + getPosString()
            + " com raio de " + raio + "cm"
            + " (área=" + getArea() + "cm2, perímetro="
            + getPerimetro() + "cm)";
    }
}

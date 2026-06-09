package br.edu.icomp.ufam.lab_heranca;

public class FormasMain {
    public static void main(String[] args){
        Circulo circulo1 = new Circulo(32, 87, 6.0);
        System.out.println(circulo1.toString());
        Retangulo retangulo1 = new Retangulo(12, 45, 5.0, 3.0);
        System.out.println(retangulo1.toString());
        Quadrado quadrado1 = new Quadrado(23, 56, 4.0);
        System.out.println(quadrado1.toString());
    }
}

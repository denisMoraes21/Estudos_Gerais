package br.edu.ufam.icomp.lab_encapsulamento;

public class GISMain {
    public static void main(String[] args) {
        Localizavel[] vetorLocalizaveis = new Localizavel[4];

        vetorLocalizaveis[0] = new Celular(55, 92, 99999999);
        vetorLocalizaveis[1] = new CarroLuxuoso("ABC-1234");
        vetorLocalizaveis[2] = new Celular(55, 11, 988887777);
        vetorLocalizaveis[3] = new CarroLuxuoso("XYZ-9876");

        for (int i = 0; i < vetorLocalizaveis.length; i++) {
            System.out.println(vetorLocalizaveis[i].getPosicao());
        }
    }
}

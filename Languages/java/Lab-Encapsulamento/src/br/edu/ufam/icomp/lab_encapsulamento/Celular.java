package br.edu.ufam.icomp.lab_encapsulamento;

import java.util.Random;

public class Celular implements Localizavel {
    private int codPais;
    private int codArea;
    private int numero;

    public Celular(int codPais, int codArea, int numero) {
        setCodPais(codPais);
        setCodArea(codArea);
        setNumero(numero);
    }

    final public void setCodPais(int codPais) {
        if (codPais >= 1 && codPais <= 1999) {
            this.codPais = codPais;
        }
        else {
            this.codPais = -1;
        }
    }

    public int getCodPais() {
        return codPais;
    }

    final public void setCodArea(int codArea) {
        if (codArea >= 10 && codArea <= 99) {
            this.codArea = codArea;
        }
        else {
            this.codArea = -1;
        }
    }

    public int getCodArea() {
        return codArea;
    }

    final public void setNumero(int numero) {
        if (numero >= 10000000 && numero <= 999999999){
            this.numero = numero;
        }
        else {
            this.numero = -1;
        }
    }

    public int getNumero() {
        return numero;
    }

    public Posicao getPosicao() {
        Random rand = new Random();
        double valorMinimoLat = -3.160000;
        double valorMaximoLat = -2.960000;
        double valorMinimoLon = -60.120000;
        double valorMaximoLon = -59.820000;
        double valorMinimoAlt = 15.0;
        double valorMaximoAlt = 100.0;

        double latitude = valorMinimoLat + (valorMaximoLat - valorMinimoLat) * rand.nextDouble();
        double longitude = valorMinimoLon + (valorMaximoLon - valorMinimoLon) * rand.nextDouble();
        double altitude = valorMinimoAlt + (valorMaximoAlt - valorMinimoAlt) * rand.nextDouble();

        return new Posicao(latitude, longitude, altitude);
    }

    public double getErroLocalizacao() {
        return 50.0;
    }
}


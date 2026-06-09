package br.edu.ufam.icomp.lab_encapsulamento;

import java.util.Random;

public class CarroLuxuoso extends Carro implements Localizavel{
    public CarroLuxuoso(String placa) {
        super(placa);
    }

    @Override
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

    @Override
    public double getErroLocalizacao() {
        return 15.0;
    }


}

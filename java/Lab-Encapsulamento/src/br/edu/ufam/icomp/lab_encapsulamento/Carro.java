package br.edu.ufam.icomp.lab_encapsulamento;

public class Carro {
    protected String placa;

    public Carro(String placa) {
        this.setPlaca(placa);
    }

    public void setPlaca(String placa) {
        this.placa = placa;
    }

    public String getPlaca() {
        return this.placa;
    }
}

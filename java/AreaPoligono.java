import java.util.ArrayList;
import java.util.Scanner;


public class AreaPoligono {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in).useLocale(java.util.Locale.US);

        ArrayList<Double> x = new ArrayList<>();
        ArrayList<Double> y = new ArrayList<>();

        // Leitura dos valores de x
        double valor;

        while (true) {
            valor = sc.nextDouble();

            if (valor == -1) {
                break;
            }

            x.add(valor);
        }

        while (true) {
            valor = sc.nextDouble();

            if (valor == -1) {
                break;
            }

            y.add(valor);
        }

        double soma = 0.0;

         for (int i = 0; i < x.size() - 1; i++) {
            soma += (x.get(i) * y.get(i + 1))
                  - (x.get(i + 1) * y.get(i));
        }

        double area = Math.abs(soma) / 2.0;

        System.out.printf("%.4f%n", area);

        sc.close();
    }
}

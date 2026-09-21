#include <stdbool.h>
#include <stdio.h>

int main(void) {
    double pressao_bar = 2.5;
    bool comando = true;
    bool falha = false;
    bool permitido = (pressao_bar >= 2.0) && (pressao_bar <= 4.0)
                  && comando && !falha;
    printf("Pressao %.1f bar: permitido=%d\n", pressao_bar, permitido);

    pressao_bar = 1.9;
    permitido = (pressao_bar >= 2.0) && (pressao_bar <= 4.0)
             && comando && !falha;
    printf("Pressao %.1f bar: permitido=%d\n", pressao_bar, permitido);

    pressao_bar = 4.0;
    permitido = (pressao_bar >= 2.0) && (pressao_bar <= 4.0)
             && comando && !falha;
    printf("Pressao %.1f bar: permitido=%d\n", pressao_bar, permitido);
    return 0;
}

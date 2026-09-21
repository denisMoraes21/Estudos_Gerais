#include <limits.h>
#include <stdio.h>

int main(void) {
    printf("Bits por byte: %d\n", CHAR_BIT);
    printf("Bytes de int: %zu\n", sizeof(int));
    printf("Limites de int: %d a %d\n", INT_MIN, INT_MAX);
    printf("Bytes de double: %zu\n", sizeof(double));
    return 0;
}

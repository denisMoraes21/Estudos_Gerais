#include <stdio.h>

int main(void)
{
    printf("Hello World !\n");
    puts("Hello World !");
    printf("Your age: ");
    int age;
    scanf("%d", &age);
    printf("Your age is : %d\n", age);
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "olá!");
    printf("%s\n", buffer);
    return 0;
}

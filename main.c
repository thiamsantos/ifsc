#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

int main(int argc, char *argv[]) {
    int ip[4], ipbin[4][8];
    int mask[4], maskbin[4][8];
    int net[4], netbin[4][8];
    int bcast[4], bcastbin[4][8];
    int wildcard[4];

    int bitsrede, bitshosts, bits;
    int qtdeips, qtdehosts;

    int i, u, n;
    int resto;
    char p;

    setlocale(LC_ALL, "Portuguese");

    inputdados:
    printf("\nDigite o endereco IP: ");
    scanf("%d %c %d %c %d %c %d", &ip[0], &p, &ip[1], &p, &ip[2], &p, &ip[3]);

    digitarmascara:
    printf("\nDigite a máscara de sub-rede: ");
    scanf("%d %c %d %c %d %c %d", &mask[0], &p, &mask[1], &p, &mask[2], &p, &mask[3]);

    for (i=3; i>=0; --i) {
        if (mask[i]==0  || mask[i]==128 || mask[i]==192 || mask[i]==224 || mask[i]==240 || mask[i]==248 || mask[i]==252 || mask[i]==254 || mask[i]==255) {
            i++;
            i--;
        } else {
            printf("\nVocê digitou um valor invalido para máscara de rede.\nOs valores permitidos são: 0 | 128 | 192 | 224 | 240 | 248 | 252 | 254 | 255\n");
            i = 3;
            goto digitarmascara;
        }
    }

    for (i=3 ; i>=0 ; i--) {
        net[i] = ip[i] & mask[i];
    }

    for (i=3 ; i>=0 ; i--) {
        wildcard[i] = 255 - mask[i];
    }

    printf("\n\nO ip digitado em forma binária: \n");
    for(i = 0; i <= 3; i++) {
        n = ip[i];
        for(u = 7; u >= 0; u--) {
            resto = n % 2;
            n /= 2;
            ipbin[i][u] = resto;
            printf("%d", ipbin[i][u]);
        }
        if (i<30) {
            printf(".");
    }    }

    printf("\n\n");
    printf("A máscara digitada em forma binária: \n");

    for(i = 0; i <= 3; i++) {
        n = mask[i];
        for(u = 7; u >= 0; u--) {
            resto = n % 2;
            n /= 2;
            maskbin[i][u] = resto;
            printf("%d", maskbin[i][u]);
        }
        if (i<30) {
            printf(".");
        }
    }

    printf("\n\n");

    bitsrede = 0;
    for (i = 3; i>=0 ; i--) {
        u = mask[i];
        if (u > 1) {
            bits = 128;
                while ( u > bits ) {
                    bitsrede += 1;
                    u -= bits;
                    bits -= bits/2;
                }
            bitsrede += 1;
        }
    }

    bitshosts = 32 - bitsrede;
    qtdehosts = (pow(2,bitshosts)) - 2;

    n = bitshosts;
    while (n>8) {
        qtdeips += 256;
        n -= 8;
    }

    if (n<=8 && n > 0) {
        qtdeips += (pow(2,n)) - 1;
    }

    for (i=3 ; i>= 0 ; i--) {
        if (mask[i] < 255) {
            u = 255 - mask[i];
            bcast[i] = net[i] + u;
        }

        if (mask[i] == 255) {
            bcast[i] = net[i];
        }

        if (mask[i] == 0) {
            bcast[i] = 255;
        }
    }

    printf("\nA rede usa %d bits \n\n", bitsrede);
    printf("Quantidade de bits para hosts: %d \n\n", bitshosts);
    printf("Soma da quantidade de IPs disponiveis em cada octeto: %d \n\n", qtdeips);
    printf("Quantidade de hosts (2n - 2): %d \n\n", qtdehosts);
    printf("Endereco da rede: %d %c %d %c %d %c %d\n\n", net[0], p, net[1], p, net[2], p, net[3]);
    printf("Endereco de Wildcard: %d %c %d %c %d %c %d\n\n", wildcard[0], p, wildcard[1], p, wildcard[2], p, wildcard[3]);
    printf("O primeiro IP disponivel na faixa eh: %d%c%d%c%d%c%d\n\n", net[0], p, net[1],  p, net[2], p, (net[3] + 1) );
    printf("O último IP disponivel na faixa eh: %d%c%d%c%d%c%d\n\n", bcast[0], p, bcast[1],  p, bcast[2], p, bcast[3] -1 );
    printf("O endereço de broadcast da rede eh: %d%c%d%c%d%c%d \n\n", bcast[0], p, bcast[1],  p, bcast[2], p, bcast[3] );
    printf("O gateway da rede eh: %d%c%d%c%d%c%d \n\n", bcast[0], p, bcast[1],  p, bcast[2], p, bcast[3] - 1 );

    char loop;

    printf("Deseja digitar um novo endereço? (S / N) ");
    scanf("%s" , &loop);

    if (loop == 's' || loop == 'S' ) {
        goto inputdados;
    }

    return 0;
}

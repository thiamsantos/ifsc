#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

void handle_exit(int signo) {
    exit(0);
}

bool should_keep_going() {
    char answer;
    printf("Deseja digitar um novo endereço? (S/N) ");
    scanf("%s", &answer);

    if (answer == 's' && answer == 'S') {
        return true;
    }

    return false;
}

void calculate_net_address(int *net, int ip[], int mask[]) {
    for (int i = 0; i < 4; i++){
        net[i] = ip[i] & mask[i];
    }
}

void calculate_wildcard_mask(int *wildcard, int mask[]) {
    for (int i = 0; i < 4; i++){
        wildcard[i] = 255 - mask[i];
    }
}

void get_ip(int *ip) {
    printf("Digite o endereco IP: ");
    scanf("%d %*c %d %*c %d %*c %d", &ip[0], &ip[1], &ip[2], &ip[3]);
}

bool is_mask_valid(int mask[]) {
    for (int i = 0; i < 4; i++) {
        if (mask[i] != 0 && mask[i] != 128 && mask[i] != 192 && mask[i] != 224 && mask[i] != 240 && mask[i] != 248 && mask[i] != 252 && mask[i] != 254 && mask[i] != 255) {
            return false;
        }
    }

    return true;
}

void get_mask(int *mask) {
    while (true) {
        printf("Digite a máscara de sub-rede: ");
        scanf("%d %*c %d %*c %d %*c %d", &mask[0], &mask[1], &mask[2], &mask[3]);

        if (is_mask_valid(mask)) {
            break;
        } else {
            printf("Você digitou um valor invalido para máscara de rede.\nOs valores permitidos são: 0 | 128 | 192 | 224 | 240 | 248 | 252 | 254 | 255\n");
        }
    }
}

void calculate_broadcast(int *broadcast, int net[], int mask[]) {
    for (int i = 0; i < 4; i++) {
        if (mask[i] < 255) {
            broadcast[i] = net[i] - mask[i] + 255;
        } else if (mask[i] == 255) {
            broadcast[i] = net[i];
        } else if (mask[i] == 0) {
            broadcast[i] = 255;
        }
    }
}

void calculate_binary(int (*bin)[8], int octets[]) {
    for (int i = 0; i < 4; i++) {
        int current_octet = octets[i];

        for (int j = 7; j >= 0; j--) {
            bin[i][j] = current_octet % 2;
            current_octet /= 2;
        }
    }
}

void print_binary(int binary[4][8]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", binary[i][j]);
        }

        if (i < 3) {
            printf(".");
        }
    }
}

void format_address(char *address, int octets[]) {
    sprintf(address, "%d.%d.%d.%d", octets[0], octets[1], octets[2], octets[3]);
}

void calculate_last_ip(int *last_ip, int broadcast[4]) {
    memcpy(last_ip, broadcast, sizeof(last_ip) * 4);
    last_ip[3] -= 1;
}

void calculate_firt_ip(int *first_ip, int net[4]) {
    memcpy(first_ip, net, sizeof(first_ip) * 4);
    first_ip[3] += 1;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_exit);
    setlocale(LC_ALL, "Portuguese");

    int bitsrede, bitshosts, bits;
    int qtdeips, qtdehosts;

    while (true) {
        int ip[4];
        get_ip(ip);

        int mask[4];
        get_mask(mask);

        int net[4];
        calculate_net_address(net, ip, mask);

        int wildcard[4];
        calculate_wildcard_mask(wildcard, mask);

        int broadcast[4];
        calculate_broadcast(broadcast, net, mask);

        int last_ip[4];
        calculate_last_ip(last_ip, broadcast);

        int first_ip[4];
        calculate_firt_ip(first_ip, net);

        int binary_ip[4][8];
        calculate_binary(binary_ip, ip);

        int binary_mask[4][8];
        calculate_binary(binary_mask, mask);

        char net_address[15];
        format_address(net_address, net);

        char wildcard_address[15];
        format_address(wildcard_address, wildcard);

        char broadcast_address[15];
        format_address(broadcast_address, broadcast);

        char last_ip_address[15];
        format_address(last_ip_address, last_ip);

        char first_ip_address[15];
        format_address(first_ip_address, first_ip);

        bitsrede = 0;
        for (int i = 3; i>=0 ; i--) {
            int u = mask[i];
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
        qtdehosts = pow(2, bitshosts) - 2;

        int n = bitshosts;
        while (n>8) {
            qtdeips += 256;
            n -= 8;
        }

        if (n<=8 && n > 0) {
            qtdeips += (pow(2,n)) - 1;
        }

        printf("\n");

        printf("O ip digitado em forma binária: ");
        print_binary(binary_ip);
        printf("\n");

        printf("A máscara digitada em forma binária: ");
        print_binary(binary_mask);
        printf("\n");

        printf("A rede usa %d bits\n", bitsrede);
        printf("Quantidade de bits para hosts: %d\n", bitshosts);
        printf("Soma da quantidade de IPs disponiveis em cada octeto: %d\n", qtdeips);
        printf("Quantidade de hosts (2n - 2): %d\n", qtdehosts);
        printf("Endereco da rede: %s\n", net_address);
        printf("Endereco de Wildcard: %s\n", wildcard_address);
        printf("O primeiro IP disponivel na faixa eh: %s\n", first_ip_address);
        printf("O último IP disponivel na faixa eh: %s\n", last_ip_address);
        printf("O endereço de broadcast da rede eh: %s\n", broadcast_address);
        printf("O gateway da rede eh: %s\n", last_ip_address);

        if (!should_keep_going()) {
            break;
        }
    }

    return 0;
}

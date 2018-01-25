#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

typedef int Octets[4];
typedef char Address[15];
typedef int BinaryOctets[4][8];

void handle_exit(int signo) {
    exit(0);
}

bool should_keep_going() {
    char answer;
    printf("Deseja digitar um novo endereço? (S/N) ");
    scanf("%s", &answer);

    if (answer == 's' || answer == 'S') {
        return true;
    }

    return false;
}

void calculate_net_address(int *net, Octets ip, Octets mask) {
    for (int i = 0; i < 4; i++){
        net[i] = ip[i] & mask[i];
    }
}

void calculate_wildcard_mask(int *wildcard, Octets mask) {
    for (int i = 0; i < 4; i++){
        wildcard[i] = 255 - mask[i];
    }
}

void get_ip(int *ip) {
    printf("Digite o endereco IP: ");
    scanf("%d %*c %d %*c %d %*c %d", &ip[0], &ip[1], &ip[2], &ip[3]);
}

bool is_mask_valid(Octets mask) {
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

void calculate_broadcast(int *broadcast, Octets net, Octets mask){
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

void calculate_binary(int (*bin)[8], Octets octets) {
    for (int i = 0; i < 4; i++) {
        int current_octet = octets[i];

        for (int j = 7; j >= 0; j--) {
            bin[i][j] = current_octet % 2;
            current_octet /= 2;
        }
    }
}

void print_binary(BinaryOctets binary) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", binary[i][j]);
        }

        if (i < 3) {
            printf(".");
        }
    }
}

void format_address(char *address, Octets octets) {
    sprintf(address, "%d.%d.%d.%d", octets[0], octets[1], octets[2], octets[3]);
}

void calculate_last_ip(int *last_ip, Octets broadcast) {
    memcpy(last_ip, broadcast, sizeof(last_ip) * 4);
    last_ip[3] -= 1;
}

void calculate_firt_ip(int *first_ip, Octets net) {
    memcpy(first_ip, net, sizeof(first_ip) * 4);
    first_ip[3] += 1;
}

int calculate_net_bits(Octets mask) {
    int net_bits = 0;
    for (int i = 0; i < 4; i++) {
        int current_value = mask[i];
        if (current_value > 1) {
            int bits = 128;
            while (current_value > bits) {
                net_bits += 1;
                current_value -= bits;
                bits -= bits/2;
            }
            net_bits += 1;
        }
    }

    return net_bits;
}

int calculate_hosts_bits(int net_bits) {
    return 32 - net_bits;
}

int calculate_hosts_amount(int hosts_bits) {
    return pow(2, hosts_bits) - 2;
}

int calculate_ips_amount(int hosts_bits) {
    int n = hosts_bits;
    int ips_amount = 0;

    while (n > 8) {
        ips_amount += 256;
        n -= 8;
    }

    if (n <= 8 && n > 0) {
        ips_amount += pow(2, n) - 1;
    }

}

void ip_calculator() {
    Octets ip;
    Octets mask;
    Octets net;
    Octets wildcard;
    Octets broadcast;
    Octets last_ip;
    Octets first_ip;
    BinaryOctets binary_ip;
    BinaryOctets binary_mask;
    Address net_address;
    Address wildcard_address;
    Address broadcast_address;
    Address last_ip_address;
    Address first_ip_address;

    get_ip(ip);
    get_mask(mask);

    calculate_net_address(net, ip, mask);
    calculate_wildcard_mask(wildcard, mask);
    calculate_broadcast(broadcast, net, mask);
    calculate_last_ip(last_ip, broadcast);
    calculate_firt_ip(first_ip, net);

    calculate_binary(binary_ip, ip);
    calculate_binary(binary_mask, mask);

    format_address(net_address, net);
    format_address(wildcard_address, wildcard);
    format_address(broadcast_address, broadcast);
    format_address(last_ip_address, last_ip);
    format_address(first_ip_address, first_ip);

    int net_bits = calculate_net_bits(mask);
    int hosts_bits = calculate_hosts_bits(net_bits);
    int hosts_amount = calculate_hosts_amount(hosts_bits);
    int ips_amount = calculate_ips_amount(hosts_bits);

    printf("\n");

    printf("O ip digitado em forma binária: ");
    print_binary(binary_ip);
    printf("\n");

    printf("A máscara digitada em forma binária: ");
    print_binary(binary_mask);
    printf("\n");

    printf("A rede usa %d bits\n", net_bits);
    printf("Quantidade de bits para hosts: %d\n", hosts_bits);
    printf("Soma da quantidade de IPs disponiveis em cada octeto: %d\n", ips_amount);
    printf("Quantidade de hosts (2n - 2): %d\n", hosts_amount);
    printf("Endereco da rede: %s\n", net_address);
    printf("Endereco de Wildcard: %s\n", wildcard_address);
    printf("O primeiro IP disponivel na faixa eh: %s\n", first_ip_address);
    printf("O último IP disponivel na faixa eh: %s\n", last_ip_address);
    printf("O endereço de broadcast da rede eh: %s\n", broadcast_address);
    printf("O gateway da rede eh: %s\n", last_ip_address);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_exit);
    setlocale(LC_ALL, "Portuguese");

    while (true) {
        ip_calculator();

        if (!should_keep_going()) {
            break;
        }
    }

    return 0;
}

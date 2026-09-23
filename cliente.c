#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "protocolo.h"
#include "jogo.h"

int main(void)
{
    int sock;
    struct sockaddr_in servidor;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Erro ao criar socket");
        return EXIT_FAILURE;
    }
    printf("Socket do cliente criado!\n");

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORTA_PADRAO);
    if (inet_pton(AF_INET, "127.0.0.1", &servidor.sin_addr) <= 0) {
        perror("Endereco IP invalido");
        close(sock);
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        perror("Erro ao conectar");
        close(sock);
        return EXIT_FAILURE;
    }
    printf("Conectado ao servidor!\n");

    //CONECTADO

    char mensagem[BUFFER_SIZE];

    while (1) {
        int resultado = receber_mensagem(sock, mensagem, sizeof(mensagem));

        if (resultado <= 0) {
            printf("Conexao encerrada.\n");
            break;
        }

        char *tipo = strtok(mensagem, "|\n");

        if (tipo == NULL) continue;

        if (strcmp(tipo, PROTO_NOME) == 0) {
            char nome[NOME_SIZE];
            char resposta[BUFFER_SIZE];

            printf("Digite seu nome: ");
            scanf("%31s", nome);

            snprintf(resposta, sizeof(resposta), "%s%s%s\n", PROTO_NOME, PROTO_SEP, nome);

            enviar_mensagem(sock, resposta);
        }
    }

    close(sock);
    return 0;
}
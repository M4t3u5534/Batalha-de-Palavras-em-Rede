#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "protocolo.h"

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

    const char *mensagem = "Ola servidor!";

    send(sock, mensagem, strlen(mensagem), 0);

    char buffer[BUFFER_SIZE];

    ssize_t bytes_recebidos = recv(sock, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_recebidos < 0) {
        perror("Erro no recv");
    }
    else if (bytes_recebidos == 0) {
        printf("Servidor encerrou a conexao.\n");
    }
    else {
        buffer[bytes_recebidos] = '\0';
        printf("Servidor respondeu: %s\n", buffer);
    }

    close(sock);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "protocolo.h"

int main(void)
{
    int servidor_fd;
    struct sockaddr_in endereco;

    servidor_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (servidor_fd < 0) {
        perror("Erro ao criar socket");
        return EXIT_FAILURE;
    }
    printf("Socket criado com sucesso!\n");
    
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORTA_PADRAO);

    if (bind(servidor_fd, (struct sockaddr *)&endereco, sizeof(endereco)) < 0) {
        perror("Erro no bind");
        close(servidor_fd);
        return EXIT_FAILURE;
    }

    if (listen(servidor_fd, 5) < 0) {
        perror("Erro no listen");
        close(servidor_fd);
        return EXIT_FAILURE;
    }
    printf("Servidor aguardando conexoes na porta %d...\n", PORTA_PADRAO);

    int cliente_fd;
    cliente_fd = accept(servidor_fd, NULL, NULL);

    if (cliente_fd < 0) {
        perror("Erro no accept");
        close(servidor_fd);
        return EXIT_FAILURE;
    }
    printf("Cliente conectado!\n");

    char buffer[BUFFER_SIZE];
    ssize_t bytes_recebidos;

    bytes_recebidos = recv(cliente_fd, buffer, BUFFER_SIZE - 1, 0);
    
    if (bytes_recebidos < 0) {
        perror("Erro no recv");
    }
    else if (bytes_recebidos == 0) {
        printf("Cliente desconectou.\n");
    }
    else {
        buffer[bytes_recebidos] = '\0';
        printf("Recebido do cliente: %s\n", buffer);
    }

    const char *resposta = "Ola cliente!";

    send(cliente_fd, resposta, strlen(resposta), 0);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "jogo.h"

int enviar_mensagem(int socket_fd, const char *mensagem) {
    ssize_t enviados;

    enviados = send(socket_fd, mensagem, strlen(mensagem), 0);

    if (enviados < 0) {
        perror("Erro no send");
        return -1;
    }

    return 0;
}

int receber_mensagem(int socket_fd, char *buffer, int tamanho) {
    ssize_t bytes_recebidos;

    bytes_recebidos = recv(socket_fd, buffer, tamanho - 1, 0);

    if (bytes_recebidos < 0) {
        perror("Erro no recv");
        return -1;
    }

    if (bytes_recebidos == 0) {
        return 0;
    }

    buffer[bytes_recebidos] = '\0';

    return (int)bytes_recebidos;
}

int solicitar_nome(Jogador *jogador) {
    char buffer[BUFFER_SIZE];
    char mensagem[BUFFER_SIZE];

    snprintf(mensagem, sizeof(mensagem), "%s%s\n", PROTO_NOME, PROTO_SEP);

    if (enviar_mensagem(jogador->socket_fd, mensagem) < 0) return -1;

    int resultado = receber_mensagem(jogador->socket_fd, buffer, sizeof(buffer));

    if (resultado <= 0) return -1;

    char *tipo;
    char *nome;

    tipo = strtok(buffer, "|");
    nome = strtok(NULL, "|\n");

    if (tipo == NULL || nome == NULL) return -1;

    if (strcmp(tipo, PROTO_NOME) != 0) return -1;

    strncpy(jogador->nome, nome, NOME_SIZE - 1);
    jogador->nome[NOME_SIZE - 1] = '\0';

    return 0;
}
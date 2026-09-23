#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "protocolo.h"
#include "jogo.h"

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
    
    Jogador jogador1 = {0};
    Jogador jogador2 = {0};

    //CLIENTE 1:
    jogador1.socket_fd = accept(servidor_fd, NULL, NULL);

    if (jogador1.socket_fd < 0) {
        perror("Erro no accept");
        close(servidor_fd);
        return EXIT_FAILURE;
    }
    printf("Cliente 1 conectado!\n");

    if (solicitar_nome(&jogador1) < 0) {
        printf("Erro ao obter nome do jogador 1.\n");
        close(jogador1.socket_fd);
        close(servidor_fd);
        return EXIT_FAILURE;
    }
    printf("Jogador 1: %s\n", jogador1.nome);

    char mensagem[BUFFER_SIZE];

    snprintf(mensagem, sizeof(mensagem), "%s%sEsperando outro jogador...\n", PROTO_AGUARDE, PROTO_SEP);

    enviar_mensagem(jogador1.socket_fd, mensagem);
    

    //CLIENTE 2:
    jogador2.socket_fd = accept(servidor_fd, NULL, NULL);

    if (jogador2.socket_fd < 0) {
        perror("Erro no accept");
        close(jogador1.socket_fd);
        close(servidor_fd);
        return EXIT_FAILURE;
    }
    printf("Cliente 2 conectado!\n");

    if (solicitar_nome(&jogador2) < 0) {
        printf("Erro ao obter nome do jogador 2.\n");
        close(jogador2.socket_fd);
        close(jogador1.socket_fd);
        close(servidor_fd);
        return EXIT_FAILURE;
    }
    printf("Jogador 2: %s\n", jogador2.nome);


    //////////


    //JOGO EM FUNCIONAMENTO:
    mensagem = snprintf(mensagem, sizeof(mensagem), "%s%s%s vs %s\n", PROTO_MSG, PROTO_SEP, jogador1.nome, jogador2.nome);

    enviar_mensagem(jogador1.socket_fd, mensagem);
    enviar_mensagem(jogador2.socket_fd, mensagem);

    executar_partida(jogador1, jogador2);

    for (int i = 0; i < 5; i++) {
        char *mensagem4 = PROTO_RODADA + PROTO_SEP + (char)i + LETRA + "10";
        
        send(cliente_fd_1, mensagem4, strlen(mensagem4), 0);
        send(cliente_fd_2, mensagem4, strlen(mensagem4), 0);

        char *palavra1;
        char *palavra2;

        while (CONTA_TEMPO <= 10) {
            bytes_recebidos = recv(cliente_fd_1, buffer, BUFFER_SIZE - 1, 0);
            
            if (bytes_recebidos < 0) {
                perror("Erro no recv");
            }
            else if (bytes_recebidos == 0) {
                printf("Cliente 1 desconectou.\n");
            }
            else {
                palavra1 = buffer;
                buffer[bytes_recebidos] = '\0';
                printf("Recebido do cliente 1: %s\n", buffer);
            }

            bytes_recebidos = recv(cliente_fd_2, buffer, BUFFER_SIZE - 1, 0);
            
            if (bytes_recebidos_2 < 0) {
                perror("Erro no recv");
            }
            else if (bytes_recebidos_2 == 0) {
                printf("Cliente 2 desconectou.\n");
            }
            else {
                palavra2 = buffer;
                buffer[bytes_recebidos_2] = '\0';
                printf("Recebido do cliente 2: %s\n", buffer);
            }
        }

        VALIDO = VALIDA(palavra1);
        char *mensagem5 = PROTO_RESULTADO + PROTO_SEP + VALIDO;
        send(cliente_fd_1, mensagem5, strlen(mensagem5), 0);

        VALIDO = VALIDA(palavra2);
        mensagem5 = PROTO_RESULTADO + PROTO_SEP + VALIDO;
        send(cliente_fd_2, mensagem5, strlen(mensagem5), 0);

        ATUALIZA_PONTUACAO;

        char *mensagem6 = PROTO_PLACAR + PROTO_SEP + nome_cliente_1 + PROTO_SEP + pontuacao_1 + PROTO_SEP + nome_cliente_2 + PROTO_SEP + pontuacao_2;

        send(cliente_fd_1, mensagem6, strlen(mensagem6), 0);
        send(cliente_fd_2, mensagem6, strlen(mensagem6), 0);
    }
    char *mensagem7 = PROTO_FIM + PROTO_SEP;
    if (pontuacao_1 == pontuacao_2) {
        mensagem7 += "Empate!";
    } else if (pontuacao_1 > pontuacao_2) {
        mensagem7 += nome_cliente_1 + " venceu!";
    } else {
        mensagem7 += nome_cliente_2 + " venceu!";
    }

    return 0;
}

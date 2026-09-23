#ifndef JOGO_H
#define JOGO_H

#include "protocolo.h"

typedef struct {
    int socket_fd;
    char nome[NOME_SIZE];
    int pontuacao;
} Jogador;


/* Comunicação */
int enviar_mensagem(int socket_fd, const char *mensagem);
int receber_mensagem(int socket_fd, char *buffer, int tamanho);


/* Jogador */
int solicitar_nome(Jogador *jogador);


/* Lógica do jogo */
char gerar_letra(void);
int validar_palavra(const char *palavra, char letra);


/* Partida */
void executar_partida(Jogador *jogador1, Jogador *jogador2);

#endif
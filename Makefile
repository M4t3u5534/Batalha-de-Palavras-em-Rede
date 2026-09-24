CC = gcc

CFLAGS = -Wall -Wextra -pedantic -std=c11

SERVER = servidor
CLIENT = cliente

all: $(SERVER) $(CLIENT)

$(SERVER): servidor.c jogo.c jogo.h protocolo.h
	$(CC) $(CFLAGS) servidor.c jogo.c -o $(SERVER) -pthread

$(CLIENT): cliente.c jogo.c jogo.h protocolo.h
	$(CC) $(CFLAGS) cliente.c jogo.c -o $(CLIENT)

server: $(SERVER)

client: $(CLIENT)

run-server: $(SERVER)
	./$(SERVER)

run-client: $(CLIENT)
	./$(CLIENT)

test: clean all
	@echo "Compilacao concluida com sucesso!"

clean:
	rm -f $(SERVER) $(CLIENT)

.PHONY: all server client run-server run-client test clean
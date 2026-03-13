###############################################################################################
#	cordinador.py -> Da permiso de entrar y salir de la region critica
#
#
################################################################################################

include <zmq.h>
include <stdio.h>
include <unistd.h>
include <string.h>
include <stdlib.h>
include <time.h>

int main (void) {  // programa publisher.c
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    
    // Bind al puerto 5556
    int rc = zmq_bind(publisher, "tcp://*:5556");
    if (rc != 0) { perror("Error bind"); return 1; }

    srand(time(NULL));
    printf("Publicador enviando DEPORTES y CINE...\n");
    int max = 1;
    while (max <  10) {
        char message[100];
        int score = rand() % 10; // Tópico 1: DEPORTES
        sprintf(message, "DEPORTES Marcador final: %d", score);
        zmq_send(publisher, message, strlen(message), 0);
        
        int rating = (rand() % 5) + 1; // Tópico 2: CINE
        sprintf(message, "CINE Nueva película, rating: %d estrellas", rating);
        zmq_send(publisher, message, strlen(message), 0);

        usleep(500000); // Enviar cada 0.5 segundos
        max++;
    }
    //  We never get here, but clean up anyhow
    zmq_close(publisher);
    zmq_ctx_destroy(context);
    return 0;
}

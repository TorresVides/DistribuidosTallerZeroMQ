#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
	printf("Uso: ./worker.exe worker1\n");
	return 1;
    }
    
    char *nombre = argv[1];
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);

    int rc = zmq_connect(socket, "tcp://127.0.0.1:5555");
    if (rc != 0) {
        perror("Error en zmq_connect");
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return 1;
    }

    char mensaje[100];
    char respuesta[100];
    int pudo_entrar = 0;


    for(int i = 0; i < 10; i++){
	snprintf(mensaje, sizeof(mensaje), "ENTER %s", nombre);
	zmq_send(socket, mensaje, strlen(mensaje), 0);

	int n = zmq_recv(socket, respuesta, sizeof(respuesta) - 1, 0);
	if (n == -1) {
		perror("Error en recv");
		break;
	}
	
	respuesta[n] = '\0';
	printf("[%s] intento %d -> %s\n", nombre, i + 1, respuesta);
	fflush(stdout);

	if (strcmp(respuesta, "OK") == 0){
		pudo_entrar = 1;
		break;
	}
	sleep(1);
    }

    if(!pudo_entrar) {
	printf("[%s] no pudo entrar tras 10 intentos\n", nombre);
	fflush(stdout);
	zmq_close(socket);
	zmq_ctx_destroy(context);
	return 0;
    }

    printf("[%s] entrando a zona critica\n", nombre);
    fflush(stdout);
    

    sleep(15); // El worker que entró se demora 5s en la zona crítica

    snprintf(mensaje, sizeof(mensaje), "EXIT %s", nombre);
    zmq_send(socket, mensaje, strlen(mensaje), 0);

    int n = zmq_recv(socket, respuesta, sizeof(respuesta) - 1, 0);
    if(n != -1) {
	respuesta[n] = '\0';
	printf("[%s] salida registrada; %s\n", nombre, respuesta);
	fflush(stdout);
    }    

    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}

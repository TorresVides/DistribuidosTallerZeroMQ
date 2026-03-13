#include <stdio.h>
#include <string.h>
#include <zmq.h>

int main() {
    bool libre = true;
    char ocupante[50] = "";
    char respuesta[256];

    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);

    int rc = zmq_bind(socket, "tcp://*:5555");
    if (rc != 0) {
        perror("Error en zmq_bind");
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return 1;
    }

    printf("Servidor esperando mensajes en tcp://*:5555\n");
    fflush(stdout);

    
    while(1){
	char buffer[256];
        int n = zmq_recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (n == -1) {
        	perror("Error en zmq_recv");
        	zmq_close(socket);
        	zmq_ctx_destroy(context);
        	return 1;
    	}
	
    	buffer[n] = '\0';
    	char comando[20], worker[50];
	int leidos = sscanf(buffer, "%19s %49s", comando, worker);

	if (leidos != 2) {
		zmq_send(socket, "ERROR", 5, 0);
		continue;
	}
    	
		
	if ( strcmp(comando, "ENTER") == 0) {
		if (libre) {
			libre = 0;
			strcpy(ocupante, worker);
    			printf("Coordinador: %s entra a la zona critica\n", worker);
    			fflush(stdout);
			zmq_send(socket, "OK", 2, 0);
    
		 }else{
			printf("Coordinador: %s debe esperar, ocupado por %s\n", worker, ocupante);
			fflush(stdout);
			zmq_send(socket, "WAIT", 4, 0);
		}

        }
	else if (strcmp(comando, "EXIT") == 0){
		if(!libre && strcmp(ocupante, worker) == 0){
			libre = 1;
			ocupante[0] = '\0';
			printf("Coordinador: %s sale de la zona critica\n", worker);
			fflush(stdout);
			zmq_send(socket, "RELEASED", 8, 0);
		}else {
			zmq_send(socket, "ERROR", 5, 0);

		}
	}
    }
    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}

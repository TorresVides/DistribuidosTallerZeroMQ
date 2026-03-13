// ZMQ Client (Requester)
// Initialize context, create ZMQ_REQ socket, and connect
void *context = zmq_ctx_new();
void *requester = zmq_socket(context, ZMQ_REQ);
zmq_connect(requester, "tcp://localhost:5555");

for (int n = 0; n < 10; n++) {
    char buffer[10];
    // Send request then wait for reply
    zmq_send(requester, "Hello", 5, 0);
    zmq_recv(requester, buffer, 10, 0);
    printf("Received: %s\n", buffer);
}
// Clean up
zmq_close(requester);
zmq_ctx_destroy(context);

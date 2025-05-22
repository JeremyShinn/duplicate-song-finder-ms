# Duplicate Song Finder Microservice
## How to Request Data:

Uses ZeroMQ as a pipeline.

### Connect to pipeline socket:

'''
void *context = zmq_ctx_new ();
void *requester = zmq_socket (context, ZMQ_REQ);
zmq_connect (requester, "tcp://localhost:5555");
'''

### Sends request:

'''
char message[] = "filepath.txt";
zmq_send(requester, message, strlen(message), 0);
'''

## How to Receive Data:

### Connect to pipeline as above.

### Receive request:

'''
char message[3];
zmq_recv(requester, message, 3, 0);
'''

## UML Diagram


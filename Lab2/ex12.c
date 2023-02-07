#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <sys/un.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/wait.h>

// Prototypes for internal functions and utilities
void error(const char *fmt, ...);
int runClient(char *clientName, int numMessages, char **messages);
int runServer();
void serverReady(int signal);

bool serverIsReady = false;

// Prototypes for functions to be implemented by students
void server();
void client(char *clientName, int numMessages, char *messages[]);

void verror(const char *fmt, va_list argp)
{
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, argp);
    fprintf(stderr, "\n");
}

void error(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    verror(fmt, argp);
    va_end(argp);
    exit(1);
}

int runServer(int port) {
    int forkPID = fork();
    if (forkPID < 0)
        error("ERROR forking server");
    else if (forkPID == 0) {
        server();
        exit(0);
    } else {
        printf("Main: Server(%d) launched...\n", forkPID);
    }
    return forkPID;
}

int runClient(char *clientName, int numMessages, char **messages) {
    fflush(stdout);
    printf("Launching client %s...\n", clientName);
    int forkPID = fork();
    if (forkPID < 0)

        error("ERROR forking client %s", clientName);
    else if (forkPID == 0) {
        client(clientName, numMessages, messages);
        exit(0);
    }
    return forkPID;
}

void serverReady(int signal) {
    serverIsReady = true;
}

#define NUM_CLIENTS 2
#define MAX_MESSAGES 5
#define MAX_CLIENT_NAME_LENGTH 10

struct client {
    char name[MAX_CLIENT_NAME_LENGTH];
    int num_messages;
    char *messages[MAX_MESSAGES];
    int PID;
};

// Modify these to implement different scenarios
struct client clients[] = {
        {"Uno", 3, {"Mensaje 1-1", "Mensaje 1-2", "Mensaje 1-3"}},
        {"Dos", 3, {"Mensaje 2-1", "Mensaje 2-2", "Mensaje 2-3"}}
};

int main() {
    signal(SIGUSR1, serverReady);
    int serverPID = runServer(getpid());
    while(!serverIsReady) {
        sleep(1);
    }
    printf("Main: Server(%d) signaled ready to receive messages\n", serverPID);

    for (int client = 0 ; client < NUM_CLIENTS ; client++) {
        clients[client].PID = runClient(clients[client].name, clients[client].num_messages,
                                        clients[client].messages);
    }

    for (int client = 0 ; client < NUM_CLIENTS ; client++) {
        int clientStatus;
        printf("Main: Waiting for client %s(%d) to complete...\n", clients[client].name, clients[client].PID);
        waitpid(clients[client].PID, &clientStatus, 0);
        printf("Main: Client %s(%d) terminated with status: %d\n",
               clients[client].name, clients[client].PID, clientStatus);
    }

    printf("Main: Killing server (%d)\n", serverPID);
    kill(serverPID, SIGINT);
    int statusServer;
    pid_t wait_result = waitpid(serverPID, &statusServer, 0);
    printf("Main: Server(%d) terminated with status: %d\n", serverPID, statusServer);
    return 0;
}



//*********************************************************************************
//**************************** EDIT FROM HERE *************************************
//#you can create the global variables and functions that you consider necessary***
//*********************************************************************************

#define PORT_NUMBER 41434
bool serverShutdown = false;

void shutdownServer(int signal) {
 //Indicate that the server has to shutdown
 serverShutdown = true;
 //Wait for the children to finish
 for (int client = 0 ; client < NUM_CLIENTS ; client++) {
    int clientStatus;
    waitpid(clients[client].PID, &clientStatus, 0);
 }
 //Exit
 exit(0);
}

void client(char *clientName, int numMessages, char *messages[])
{
    char buffer[256];
    struct sockaddr_in client_addr, server_addr;  // Handles internet addresses
    int client_socket, server_socket;  // File descriptors
    struct hostent *server;

    for (int i = 0; i < numMessages; i++){
        // Open the socket
        /* socket(int domain, int type, int protocol)
        Creates an endpoint for comms and returns a fd that refers to that endpoint.
        AF_INET: Address Family for IPv4 that sockets communicate with.
        SOCK_STREAM: TCP Connection protocol which establishes connection between two parties until
                    someone or something disconnects them.
        */
        client_socket = socket(AF_INET, SOCK_STREAM, 0); // protocol=0: Protocol to support the socket
        if(client_socket < 0) error("Error opening the client socket.");

        /* struct hostent *gethostbyname(const char *hostname)
           Retrieves host information corresponding to a host name from a host database.
           struct hostent {
                char   *h_name       Official name of the host.
                char  **h_aliases    A pointer to an array of pointers to
                                     alternative host names, terminated by a
                                     null pointer.
                int     h_addrtype   Address type.
                int     h_length     The length, in bytes, of the address.
                char  **h_addr_list  A pointer to an array of pointers to network
                                     addresses (in network byte order) for the host,
                                     terminated by a null pointer.
                char   *h_addr       h_addr_list[0]
            }
        */
        server = gethostbyname("localhost");
        if (server == NULL) {
            fprintf(stderr,"No such server exists.\n");
            exit(0);
        }

        bzero((char *) &server_addr, sizeof(server_addr));
        server_addr.sin_port = htons(PORT_NUMBER);
        server_addr.sin_family = AF_INET;
        // Copies n bytes from src to dest.
        memcpy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);

        //Connect to the server
        /* int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
           Connects the socket referred to by sockfd to the address specified by addr. The addrlen argument specifies
           the size of addr.
        */
        if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
            error("Failed to connect to the server.");

        //For each message, write to the server
        strcpy(buffer, messages[i]);
        if (write(client_socket, buffer, 255) == -1) error("Error writing back to the server.");

        // And read the response
        bzero(buffer, 256);
        if (read(client_socket, buffer, 255) == -1) error("Error reading back to the server.");

        // Print out the response
        fprintf(stdout, "Client %s(%d): Return message: %s\n", clientName, getpid(), buffer);
        fflush(stdout);
        sleep(1);
    }

    //Close socket
    close(client_socket);
}

void server()
{
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;  // Handles internet addresses
    int server_socket, client_socket, client_len;  // File descriptors

    //Handle SIGINT so the server stops when the main process kills it
    signal(SIGINT, shutdownServer);

    //Open the socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // protocol=0: Protocol to support the socket
    if (server_socket < 0)
        error("Error opening the server socket.");

    /* sin_port: Identifies the port to which the application must bind. It must be specified in network byte order.

       sin_addr: IP address in the socket. s_addr is variable that holds the information about the address we agree
                 to accept. INADDR_ANY in .s_addr means the connection will accept all incoming addresses.
                 Server will use INADDR_ANY (if want to accept all incoming connections) and client should just specify
                 one concrete address.

       sin_family: Must always be set to AF_INET
    */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_NUMBER); // convert IPv4 address in host byte order to IPv4 address in network byte order

    //Bind the socket
    /* int bind(int socket, const struct sockaddr *address, socklen_t address_len);
    */
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        error("Error binding the server socket.\n");

    /* int listen(int sockfd, int backlog)
       Accepts client connection requests, and creates a connection request queue of length backlog to queue
       incoming connection requests.
    */
    listen(server_socket, NUM_CLIENTS);

    //Signal server is ready
    /* int kill(pid_t pid, int sig)
       Used to send any signal to any process.
    */
    kill(getppid(), SIGUSR1);

    //Accept connection
    client_len = sizeof(server_addr);
    while(!serverShutdown){
        /* int accept(int socket, struct sockaddr *address, int *address_len)
           Used by a server to accept a connection request from a client.
           The call accepts the first connection on its queue of pending connections for the given socket.
           Creates a new socket descriptor with the same properties as socket and returns it to the caller.
        */
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);

        // Create a child proccess to read and write
        int server_fork = fork();
        if (server_fork < 0) error("Error forking server process for client sockets.");

        else if(server_fork == 0){
           // Establishes the different child servers
           for(int i = 0; i < MAX_MESSAGES; i++){
                /* void bzero(void *s, size_t n)
                   Erases the data in the n bytes of the memory starting at the location pointed to by s, by writing
                   zeros (bytes containing '\0') to that area.
                */
                bzero(buffer, 256);

                /* int read(int s, char *buf, int len)
                   Reads data on a socket with descriptor s and stores it in a buffer.
                */
                if(read(client_socket, buffer, 255) == -1) // Messages must end in \0
                    error("Error reading from client socket.");

                fprintf(stdout, "Server child(%d): got message: %s\n", getpid(), buffer);
                fflush(stdout);

                /* int write(int fd, const void *buf, size_t count)
                   Writes up to count bytes from the buffer starting at buf to the file referred to by the
                   file descriptor fd.
                */
                if(write(client_socket, buffer, 255) == -1)  // Writes message back to the client socket
                    error("Error writing to a client socket.");

                // Sets up elapsed time (10s) before exiting
                signal(SIGALRM, exit); // Asynchronous signal raised when a time interval specified in a call to the alarm expires
                alarm(10); // Generates SIGALRM signal after specified amount of time

                // Prints out time waiting
                for(int j = 1; j <= 10; j++){
                    fprintf(stdout, "Child server(%d): sleeping %d...\n", getpid(), j);
                    sleep(1);
                }
           }
        }

        else close(client_socket);
    }

    //Close socket
    /* int close(int fd)
       Closes a file descriptor, so that it no longer refers to any file and may be reused.
    */
   close(server_socket);
}
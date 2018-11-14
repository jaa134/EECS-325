/*     
Jacob Alspaw
jaa134
proj3.c
10/17/18
A simple web server that supports commands such as GET and QUIT
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <dirent.h>

/*******************************
 ERROR HANDLING SPECIFIC METHODS
*******************************/
//printing helpful messages
void debug(char *format, char *arg);
//printing error messages
void errorExit(char *format, char *arg);

/************************
ARGUMENT SPECIFIC METHODS
*************************/
//seperate args passed into running proj3
void extractArgs(int argc, char **argv);
//error if there are duplicate flags
void errorIfDupFlag(bool wasFlagFound, char flag);
//handle values for the -p flag
void extractPort(char *flagArg);
//handle values for the -d flag
void extractDirectory(char *flagArg);
//handle values for the -a flag
void extractToken(char *flagArg);

/*******************************
SEVER OPERATION SPECIFIC METHODS
********************************/
//creates the server socket that litens for incoming connections
void createListeningSocket();
//handles incoming connections and makes decisions on how to proceed
void handleIncomingConnections();
//reads client request from socket into buffer
void readRequestFromSocket(char *request);
//creates and writes server response to socket 
void writeResponseToSocket(char *request);
//handles a well formed QUIT request from the client
void handleQuitMethod(char *argument);
//handles a well formed GET request from the client
void handleGetMethod(char *argument);
//writes a response header to the accepting socket with the given code and message
void writeResponseHeader(char *code, char* message);
//persistenly writes message_size bytes of message to socket
void writeResponse(char *message, size_t message_size);
//closes our sockets if they are open
void closeSockets();
//c server
void shutdownServer();

/*******************************
SERVER OPERATION SPECIFIC VALUES
*******************************/
#define PROTOCOL "tcp"
#define MIN_PORT 0
#define MAX_PORT 65535
#define QLEN 1
#define BUFLEN 1024

//is debug flag present
bool is_v_present;
//socket that listens for incoming connection
int listening_sd;
//socket that accepts incoming connection
int accepting_sd;
//port number on which server listens for incoming connections
int  port;
//the root directory from which files will be served
char *document_directory;
//token used for QUIT method validation
char *auth_token;


int main(int argc, char **argv) {
    extractArgs(argc, argv);
    createListeningSocket();
    handleIncomingConnections();
}

void debug(char *format, char *arg) {
    if (is_v_present)
        printf(format, arg);
}

void errorExit(char *format, char *arg) {
    closeSockets();
    fprintf(stderr, format, arg);
    exit(EXIT_FAILURE);
}

void extractArgs(int argc, char **argv) {
    bool is_p_present;
    bool is_d_present;
    bool is_a_present;
    int opt;

    //loop over each given arg and handle accordingly
    while ((opt = getopt(argc, argv, "p:d:a:v")) != -1) {
        switch (opt) {  
            case 'p':
                errorIfDupFlag(is_p_present, opt);
                is_p_present = true;
                extractPort(optarg);
                break;
            case 'd':
                errorIfDupFlag(is_d_present, opt);
                is_d_present = true;
                extractDirectory(optarg);
                break;
            case 'a':
                errorIfDupFlag(is_a_present, opt);
                is_a_present = true;
                extractToken(optarg);
                break;
            case 'v':
                errorIfDupFlag(is_v_present, opt);
                is_v_present = true;
                break;
            default: /* '?' */
                errorExit("Usage: ./proj3 -p port -d document_directory -a auth_token\n", NULL);
        }
    }

    //if a flag is missing then handle error
    if (!is_p_present || !is_d_present || !is_a_present) {
        if (!is_p_present)
            fprintf(stderr, "Missing required flag: -p\n");
        if (!is_d_present)
            fprintf(stderr, "Missing required flag: -d\n");
        if (!is_a_present)
            fprintf(stderr, "Missing required flag: -a\n");
        errorExit("Usage: ./proj3 -p port -d document_directory -a auth_token\n", NULL);
    }
}

void errorIfDupFlag(bool wasFlagFound, char flag) {
    if (wasFlagFound) {
        fprintf(stderr, "Duplicate arg: %c\n", flag);
        errorExit("Usage: ./proj3 -p port -d document_directory -a auth_token\n", NULL);
    }
}

void extractPort(char *flagArg) {
    char *end_ptr;
    bool is_num_arg;

    port = strtol(flagArg, &end_ptr, 10);
    is_num_arg = *end_ptr == '\0' && !(port == 0 && flagArg[0] != '0');
    //if the port is NaN or outside an allowable port number then error
    if (!is_num_arg || port < MIN_PORT || port > MAX_PORT)
        errorExit("Port invalid: %s\n", flagArg);
}

void extractDirectory(char *flagArg) {
    document_directory = malloc(strlen(flagArg) + 1);
    if (document_directory == NULL)
        errorExit("OS ERROR: Call to malloc() failed", NULL);
    strcpy(document_directory, flagArg);
    //open the directory to test if it exists
    DIR* dir = opendir(document_directory);
    if (dir)
        closedir(dir);
    else if (ENOENT == errno)
        errorExit("SERVER ERROR: Directory does not exist\n", NULL);
    else
        errorExit("SERVER ERROR: Cannot open directory\n", NULL);
}

void extractToken(char *flagArg) {
    auth_token = malloc(strlen(flagArg) + 1);
    if (auth_token == NULL)
        errorExit("OS ERROR: Call to malloc() failed", NULL);
    strcpy(auth_token, flagArg);
}

void createListeningSocket() {
    struct protoent *protoinfo;
    struct sockaddr_in sin;
    struct sockaddr addr;

    //setup protocol
    debug("Setting the protocol...\n", NULL);
    if ((protoinfo = getprotobyname(PROTOCOL)) == NULL)
        errorExit("SERVER ERROR: Cannot find protocol information: %s\n", PROTOCOL);

    //setup endpoint info
    debug("Setting the endpoint information...\n", NULL);
    memset((char *)&sin, 0x0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons((u_short)port);

    //allocate a socket
    debug("Allocating the listening socket...\n", NULL);
    listening_sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (listening_sd < 0)
        errorExit("SERVER ERROR: Cannot create listening socket\n", NULL);

    //bind the socket
    debug("Binding to listening socket...\n", NULL);
    if (bind(listening_sd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
        errorExit("SERVER ERROR: Cannot bind listening socket to port\n", NULL);

    //listen for incoming connections
    debug("Socket listening on port...\n", NULL);
    if (listen(listening_sd, QLEN) < 0)
        errorExit("SERVER ERROR: Socket cannot listen on port\n", NULL);
}

void handleIncomingConnections() {
    struct sockaddr addr;
    unsigned int addrlen;
    char *request;


    while (true) {
        //accepting a connection
        debug("Awaiting incoming connection...\n", NULL);
        accepting_sd = accept(listening_sd, &addr, &addrlen);
        if (accepting_sd < 0)
            errorExit("SERVER ERROR: Cannot accept connection\n", NULL);
        debug("Accepted an incoming connection...\n", NULL);

        //reading request data from socket
        debug("Reading request data from socket...\n", NULL);
        request = malloc(1);
        if (request == NULL)
            errorExit("OS ERROR: Call to malloc() failed", NULL);
        memset(request, '\0', 1);
        readRequestFromSocket(request);

        //writing the response data to the socket
        debug("Writing response data to socket...\n", NULL);
        writeResponseToSocket(request);

        //close incoming connection
        debug("Closing the incoming connection...\n", NULL);
        close(accepting_sd);
    }
}

void readRequestFromSocket(char *request) {
    char request_buffer[BUFLEN];
    ssize_t total_bytes_read, current_bytes_read;

    //loop until entire request header has been consumed by read(3)
    total_bytes_read = 0;
    current_bytes_read = 0;
    while(true) {
        memset(request_buffer, '\0', BUFLEN);
        current_bytes_read = recv(accepting_sd, request_buffer, BUFLEN - 1, 0);
        total_bytes_read += current_bytes_read;
        if (current_bytes_read < 0) 
            errorExit("SERVER ERROR: Cannot read data from socket\n", NULL);
        //break if no bytes read
        else if (current_bytes_read == 0)
            break;
        else {
            //append newly read data to end of request
            const size_t s1 = strlen(request);
            const size_t s2 = strlen(request_buffer);
            const size_t request_size = s1 + s2 + 1;

            request = realloc(request, request_size);
            if (request == NULL)
                errorExit("OS ERROR: Call to realloc() failed", NULL);
            memcpy(request + s1, request_buffer, s2 + 1);

            //break if we found the end of the request
            if (strstr(request, "\r\n\r\n") != NULL)
                break;
        }
    }
}

void writeResponseToSocket(char *request) {
    char *header_end;
    bool has_bl_ending;
    char *method, *argument, *protocol, *version;
    
    //make sure header ends in blank line
    header_end = strstr(request, "\r\n\r\n");
    has_bl_ending = header_end != NULL && strlen(header_end) == strlen("\r\n\r\n");
    //if the doesnt end in CRLFCRLF, send a 400
    if (!has_bl_ending) {
        debug("Response code 400 writen to socket (bl)...\n", NULL);
        writeResponseHeader("400", "Malformed Request");
        return;
    }

    method = strtok(request, " ");
    //if the method is missing, send a 400
    if (method == NULL) {
        debug("Response code 400 writen to socket (method)...\n", NULL);
        writeResponseHeader("400", "Malformed Request");
        return;
    }

    argument = strtok(NULL, " ");
    //if the argument is missing, send a 400
    if (argument == NULL) {
        debug("Response code 400 writen to socket (argument)...\n", NULL);
        writeResponseHeader("400", "Malformed Request");
        return;
    }

    protocol = strtok(NULL, "/");
    //if the protocol is missing, send a 400
    if (protocol == NULL) {
        debug("Response code 400 writen to socket (protocol)...\n", NULL);
        writeResponseHeader("400", "Malformed Request");
        return;
    }

    version = strtok(NULL, "\r");
    //if the version is missing, send a 400
    if (version == NULL) {
        debug("Response code 400 writen to socket (version)...\n", NULL);
        writeResponseHeader("400", "Malformed Request");
        return;
    }   

    //if the protocol is not recognized, send a 501
    if (strcmp(protocol, "HTTP") != 0) {
        debug("Response code 501 writen to socket...\n", NULL);
        writeResponseHeader("501", "Protocol Not Implemented");
        return;
    }

    //if the method is not recognized, send a 405
    if (strcmp(method, "GET") != 0 && strcmp(method, "QUIT") != 0) {
        debug("Response code 405 writen to socket...\n", NULL);
        writeResponseHeader("405", "Unsupported Method");
        return;
    }

    //handle a QUIT method
    if (strcmp(method, "QUIT") == 0) {
        debug("Handling QUIT method...\n", NULL);
        handleQuitMethod(argument);
        return;
    }

    //handle a GET request
    if (strcmp(method, "GET") == 0) {
        debug("Handling GET method...\n", NULL);
        handleGetMethod(argument);
        return;
    }

    //unknown request
    debug("SERVER ERROR: Unhandled Request...\n", NULL);
}

void handleQuitMethod(char *argument) {
    //shutdown server if auth token matches request argument
    if (strcmp(argument, auth_token) == 0) {
        debug("Response code 200 writen to socket...\n", NULL);
        writeResponseHeader("200", "Server Shutting Down");
        debug("Server shutdown initiated...\n", NULL);
        shutdownServer();
    }
    //send a 403 if auth token does not match argument
    else {
        debug("Response code 403 writen to socket...\n", NULL);
        writeResponseHeader("403", "Operation Forbidden");
    }
}

void handleGetMethod(char *argument) {
    //send a 406 if filename does not begin with '/'
    if (argument[0] != '/') {
        debug("Response code 406 writen to socket...\n", NULL);
        writeResponseHeader("406", "Invalid Filename");
    }
    else {
        char *filepath;
        FILE *file;

        //if argument is '/' add the default filename to path
        if (strcmp(argument, "/") == 0) {
            char *default_filename;

            default_filename = "/default.html";
            filepath = malloc(strlen(document_directory) + strlen(default_filename) + 1);
            if (filepath == NULL)
                errorExit("OS ERROR: Call to malloc() failed", NULL);
            sprintf(filepath, "%s%s", document_directory, default_filename);
        }
        //else use argument as rest of path to requested file
        else
        {
            filepath = malloc(strlen(document_directory) + strlen(argument) + 1);
            if (filepath == NULL)
                errorExit("OS ERROR: Call to malloc() failed", NULL);
            sprintf(filepath, "%s%s", document_directory, argument);
        }

        //open a file to send to the client
        file = fopen(filepath, "r+");
        //if the file was found then send the contents
        if (file != NULL) {
            char filecontents_buffer[BUFLEN];
            int bytes_read;

            //send that we found the file and its coming
            debug("Response code 200 writen to socket...\n", NULL);
            writeResponseHeader("200", "OK");

            //repeatedly send the file contents to socket in chunks
            debug("File contents writen to socket...\n", NULL);
            while(true) {
                memset(filecontents_buffer, '\0', BUFLEN);
                bytes_read = fread(filecontents_buffer, 1, BUFLEN, file);
                if (bytes_read < 0) 
                    errorExit("SERVER ERROR: Cannot read data from file\n", NULL);
                else if (bytes_read == 0)
                    break;
                else {
                    writeResponse(filecontents_buffer, bytes_read);
                }
            }
            fclose(file);
        }
        //if the file was not found or invalid then send a 404
        else {
            debug("Response code 404 writen to socket...\n", NULL);
            writeResponseHeader("404", "File Not Found");
        }

        free(filepath);
    }
}

void writeResponseHeader(char *code, char* message) {
    char header_buffer[BUFLEN];

    //form the response header
    sprintf(header_buffer, "HTTP/1.1 %s %s\r\n\r\n", code, message);
    writeResponse(header_buffer, strlen(header_buffer));
}

void writeResponse(char *message, size_t message_size) {
    ssize_t total_bytes_wrote, current_bytes_wrote;

    //persistently sends data to socket and loops until all bytes in chunk have been written to socket
    total_bytes_wrote = 0;
    current_bytes_wrote = 0;
    while ((size_t)total_bytes_wrote < message_size) {
        current_bytes_wrote = send(accepting_sd, message + current_bytes_wrote, message_size, 0);
        total_bytes_wrote += current_bytes_wrote;
        if (current_bytes_wrote <= 0)
            errorExit("SERVER ERROR: Cannot write data to socket\n", NULL);
    }
}

void shutdownServer() {
    closeSockets();
    exit(0);
}

void closeSockets() {
    //close the sockets if the socket descriptor has a valid value
    if (listening_sd >= 0)
        close(listening_sd);
    if (accepting_sd >= 0)
        close(accepting_sd);
}

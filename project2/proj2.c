/*     
Jacob Alspaw
jaa134
proj2.c
9/28/18

A simple command line based web client. Exchanges information with another 
computer over a network. Creates and sends HTTP GET request. Contents are
saved to the specified location.

Usage: ./proj2 -u URL [-d] [-r] [-R] -o filename
	-u: URL
	-d: show command details
	-r: show request header
	-R: show response header
	-o: location to save request contents to
*/

#include <stdbool.h> 
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MIN_PORT 0
#define MAX_PORT 65535
#define PROTOCOL "tcp"
#define BUFLEN 1024

//seperate args passed into running proj2
void extractArgs(int argc, char **argv);
//error if there are duplicate flags
void errorIfDupFlag(bool wasFlagFound, char flag);
//handle values for the -u flag: hostname, port, filename
void extractUrl(char *url);
//handle values for the -o flag: filename
void extractFilename(char *filename);

//create the format of the GET request
char* create_request();
//setup the enpoint for the client
void connect_client();
//send GET request
void write_client_request(char *request_header);
//read GET response
void read_server_response();


//print output required by flags
void printArg_d();
void printArg_r();
void printArg_R();

//printing helpful messages
void debug(char* format, char *arg);
//printing error messages
void error_exit(char* format, char *arg);

//is flag present
bool is_u_present;
bool is_d_present;
bool is_r_present;
bool is_R_present;
bool is_o_present;
bool is_v_present;

char *u_protocol = "http://";
char *u_hostname;
int   u_port;
char *u_path2file;

char *r_header_l1;
char *r_header_l2;
char *r_header_l3;

char *R_header;

char *o_filename;
FILE *o_file;

//socket descriptor
int sd;

int main(int argc, char **argv) {
	extractArgs(argc, argv);

	debug("Opening file...\n", NULL);
    o_file = fopen(o_filename, "w");
    if (o_file == NULL)
    	error_exit("Error opening file\n", NULL);

	if (is_d_present)
		printArg_d();

	char *request_header = create_request();
	if (is_r_present)
		printArg_r();

	//connect client via socket
	debug("Connecting client...\n", NULL);
	connect_client(request_header);

	//exchange data
	write_client_request(request_header);
	read_server_response();

    // close socket connection
    debug("Closing connection...\n", NULL);
    close (sd);

    //close file
    debug("Closing file...\n", NULL);
    fclose(o_file);

	exit(0);
}

void extractArgs(int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "u:drRo:v")) != -1) {
        switch (opt) {
            case 'u':
            	errorIfDupFlag(is_u_present, opt);
    			is_u_present = true;
    			extractUrl(optarg);
                break;
            case 'd':
            	errorIfDupFlag(is_d_present, opt);
    			is_d_present = true;
                break;
            case 'r':
            	errorIfDupFlag(is_r_present, opt);
    			is_r_present = true;
                break;
            case 'R':
            	errorIfDupFlag(is_R_present, opt);
    			is_R_present = true;
                break;
            case 'o':
            	errorIfDupFlag(is_o_present, opt);
    			is_o_present = true;
    			extractFilename(optarg);
                break;
            case 'v':
            	errorIfDupFlag(is_v_present, opt);
            	is_v_present = true;
               	break;
            default: /* '?' */
                error_exit("Usage: ./proj2 -u URL [-d] [-r] [-R] -o filename\n", NULL);
        }
    }


	if (!is_u_present || !is_o_present) {
		if (!is_u_present)
			fprintf(stderr, "Missing required flag: -u\n");
		if (!is_o_present)
			fprintf(stderr, "Missing required flag: -o\n");
		error_exit("Usage: ./proj2 -u URL [-d] [-r] [-R] -o filename\n", NULL);
	}
}

void errorIfDupFlag(bool wasFlagFound, char flag) {
	if (wasFlagFound) {
    	fprintf(stderr, "Duplicate arg: %c\n", flag);
    	error_exit("Usage: ./proj2 -u URL [-d] [-r] [-R] -o filename\n", NULL);
	}
}

void extractUrl(char* url) {
    if (url == NULL || url[0] == '\0')
    	error_exit("URL not provided after -u flag\n", NULL);
    else {
    	if(strncasecmp(url, u_protocol, strlen(u_protocol)) != 0)
    		error_exit("URL protocol either missing or formatted incorrectly\n", NULL);
    	else {
    		//cut the protocol out of the url
    		url += strlen(u_protocol);

    		//find the hostaname
    		int hostBufferSize = strcspn(url, "/:");
    		char hostBuffer[hostBufferSize];
    		strncpy(hostBuffer, url, hostBufferSize);
    		hostBuffer[hostBufferSize] = '\0';
    		u_hostname = malloc(strlen(hostBuffer) + 1);
    		strcpy(u_hostname, hostBuffer);
    		if (strlen(u_hostname) == 0)
    			error_exit("URL hostname missing\n", NULL);
    		//cut out the hostname from the url
    		url += strlen(u_hostname);

    		//find the port number
    		if (url[0] == ':') {
    			url++;
    			int portBufferSize = strcspn(url, "/") + 1;
    			char portBuffer[portBufferSize];
    			strncpy(portBuffer, url, portBufferSize);
    			portBuffer[portBufferSize] = '\0';
    			u_port = atoi(portBuffer);
    			if ((u_port == 0 && portBuffer[0] != '0') || u_port < MIN_PORT || u_port > MAX_PORT)
    				error_exit("URL port invalid: %s\n", portBuffer);
    			//cut out the port from the url
    			url += strlen(portBuffer);
    		}
    		else
    			u_port = 80;

    		//find the path2file 
    		if (url[0] == '/') {
    			u_path2file = url;
    		}
    		else
    			u_path2file = "/";
    	}
    }
}

void extractFilename(char *filename) {
    if (filename == NULL || filename[0] == '\0')
    	error_exit("Filename not provided after -o flag\n", NULL);
    else {
    	o_filename = malloc(strlen(filename) + 1);
    	o_filename[strlen(filename)] = '\0';
    	strcpy(o_filename, filename);
    }
}

void printArg_d() {
	printf("DET: hostname = %s\n", u_hostname);
	printf("DET: port = %i\n", u_port);
	printf("DET: web_filename = %s\n", u_path2file);
	printf("DET: output_filename = %s\n", o_filename);
}

char* create_request() {
	//make request header
	r_header_l1 = malloc(strlen(u_path2file) + 50);
	sprintf(r_header_l1, "GET %s HTTP/1.0", u_path2file);
	debug("Header Line 1: %s\n", r_header_l1);

	r_header_l2 = malloc(strlen(u_hostname) + 50);
	sprintf(r_header_l2, "Host: %s", u_hostname);
	debug("Header Line 2: %s\n", r_header_l2);

	r_header_l3 = "User-Agent: CWRU EECS 325 Client 1.0";
	debug("Header Line 3: %s\n", r_header_l3);

	char *header = malloc(strlen(r_header_l1) + strlen(r_header_l2) + strlen(r_header_l3) + (4 * strlen("\r\n")) + 1);
	sprintf(header, "%s\r\n%s\r\n%s\r\n\r\n", r_header_l1, r_header_l2, r_header_l3);

	return header;
}

void connect_client(char *request_header) {
    // lookup the hostname
    debug("Looking up the hostname...\n", NULL);
    struct hostent *hinfo;
    hinfo = gethostbyname (u_hostname);
    if (hinfo == NULL)
    	error_exit("Cannot find hostname: %s\n", u_hostname);

    // set endpoint information
    debug("Setting the endpoint information...\n", NULL);
    struct sockaddr_in sin;
    memset ((char *)&sin, 0x0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons (u_port);
    memcpy ((char *)&sin.sin_addr, hinfo->h_addr, hinfo->h_length);

    // set the protocol
    debug("Setting the protocol...\n", NULL);
    struct protoent *protoinfo;
    if ((protoinfo = getprotobyname(PROTOCOL)) == NULL)
    	error_exit("Cannot find protocol information: %s\n", PROTOCOL);

    // allocate a socket
    debug("Allocating a socket...\n", NULL);
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
    	error_exit("Cannot create socket\n", NULL);

    // connect the socket
    debug("Connecting to socket...\n", NULL);
    if (connect (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    	error_exit("Socket refusing to connect\n", NULL);
}

void write_client_request(char *request_header) {
    // write data to server
    debug("Writing data to server...\n", NULL);
    int bytes_written = write(sd, request_header, strlen(request_header));
    if (bytes_written <= 0)
    	error_exit("Error writing data\n", NULL);
}

void read_server_response() {
	// read data from server and store it
    debug("Reading data from server...\n", NULL);
    int bytes_read;
    char response_buffer [BUFLEN];
    bool wasHeaderFound = false;
	while (1) {
		memset(response_buffer, '\0', BUFLEN);
    	bytes_read = read(sd, response_buffer, BUFLEN - 1);
    	if (bytes_read < 0)
    		error_exit("Error reading data\n", NULL);
    	else if (bytes_read == 0)
    		break;
    	else if (!wasHeaderFound) {
    		// seperate response in header and body
    		char *response_split = strstr(response_buffer, "\r\n\r\n");
    		if (response_split != NULL) {
    			wasHeaderFound = true;
    			int response_header_size = response_split - response_buffer + 1;
    			R_header = malloc(response_header_size + 1);
    			snprintf(R_header, response_header_size, "%s", response_buffer);
    			R_header[response_header_size] = '\0';

    			//print response details
				if (is_R_present)
					printArg_R();

				//find the response code in the header
				int response_code = atoi(R_header + strcspn(R_header, " "));
				if (response_code != 200) {
					char response_code_error_buffer [100];
					sprintf(response_code_error_buffer, "web server sent an unsupported return code: %d\n", response_code);
    				error_exit(response_code_error_buffer, NULL);
				}

    			//write the rest of the response body
    			long response_body_size = bytes_read - response_header_size - strlen("\r\n\r\n" + 1);
    			char *response_body = (char *)response_split + strlen("\r\n\r\n");
				fwrite(response_body, sizeof(char), response_body_size, o_file);
			}
		}
    	else {
    		//write the response body
    		fwrite(response_buffer, sizeof(char), bytes_read, o_file);
    	}
	}
}

void printArg_r() {
	printf("REQ: %s\n", r_header_l1);
	printf("REQ: %s\n", r_header_l2);
	printf("REQ: %s\n", r_header_l3);
}

void printArg_R() {
	char *header_slice;
	header_slice = strtok(R_header, "\r\n");
	while(header_slice != NULL)
    {
       printf("RSP: %s\n",header_slice);
       header_slice = strtok(NULL,"\r\n");
    }
}

void debug(char* format, char *arg) {
	if (is_v_present)
		printf(format, arg);
}

void error_exit(char* format, char *arg) {
    fprintf(stderr, format, arg);
    exit(EXIT_FAILURE);
}

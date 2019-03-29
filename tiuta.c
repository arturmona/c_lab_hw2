#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>

/*we can only send up to 2^32 bytes, and to represent this number we need only max 10 chars: */
#define MESSAGE_SIZE 10
/*we cant read and write all at once - because if we will try to do so for 2^32 -1  bytes
our system might not have enough memory to allocate and malloc will fail... or, we will try
to reach a non existing part of a file, and again, fail.
Therfore, we will read and write in chunks of the READ_WRITE_NORMAL size: */
#define READ_WRITE_NORMAL 4096

/************************* function declarations ******************************/
int step3(int file, char* read_buffer, unsigned int amount_bytes_to_read, int sockfd);
int step4(int sockfd);
unsigned int minimum(unsigned int a, unsigned int b);
void prepare();
void ctrl_c_signal_handler();

/***************************** main function **********************************/
int main(int argc, char **argv){
    unsigned int amount_bytes_to_read;
    uint16_t server_port;
    struct sockaddr_in server_ip_addr;
    int file, step_three, step_four;
    int sockfd = -1;
    char read_buffer[READ_WRITE_NORMAL+1];
    read_buffer[READ_WRITE_NORMAL] = '\0'; /*set the last char to be a terminator*/
    /*first lets check the amount of suplied arguments: */
    if(argc != 4){
        fprintf(stderr, "Error: suplied wrong amount of arguments\n");
        exit(EXIT_FAILURE);
    }
    /*we convert the string represnting the server port to an uint16_t :*/
    server_port = (uint16_t) strtoul(argv[2], NULL, 10);
    /*we convert the string represnting the amount of bytes we want to read to an unsigned int :*/
    amount_bytes_to_read = (unsigned int) strtoul(argv[3], NULL, 10);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error: Could not create socket");
        exit(EXIT_FAILURE);
    }
    server_ip_addr.sin_family = AF_INET;
    server_ip_addr.sin_port = htons(server_port);
    server_ip_addr.sin_addr.s_addr = inet_addr(argv[1]); /*argv[1] = server ip*/
    if(connect(sockfd, (struct sockaddr*) &server_ip_addr, sizeof(server_ip_addr)) < 0){
        printf("Error: Connect Failed. %s \n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /*step 2, open /dev/urandom :*/
    file = open("/dev/urandom", O_RDONLY);
    if (file == -1){
        perror("Error: failed opening the file /dev/urandom");
		exit(EXIT_FAILURE);
    }
    /*step 3, transfer argv[3] bytes from /dev/urandom to port argv[2] in server argv[1]
    over the TCP connection: */
    if ((step_three = step3(file, read_buffer, amount_bytes_to_read, sockfd)) == 1){
        close(file);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    close(file); /*we no longer need the file*/
    /*step 4, obtain the count of printable chars from the server: */
    if ((step_four = step4(sockfd)) == -1){
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    close(sockfd); /*we no longer need the socket*/
    /*step 5, exit with code 0: */
    exit(EXIT_SUCCESS);
}

/***************************** utility functions *******************************/

/**
 * step3 - transfer argv[3] bytes from /dev/urandom to port argv[2] in server argv[1]
 *         over the TCP connection
 * @param file - fd of the file we read from
 * @param read_buffer - a char* to where we read from the file
 * @param amount_bytes_to_read - the amount of bytes we will read
 * @param sockfd - the socket file descriptor
 * return 0 on success, 1 on failure
 */
int step3(int file, char* read_buffer, unsigned int amount_bytes_to_read, int sockfd){
    int rc;
    int written_amount = 0;
    unsigned int left_to_read = amount_bytes_to_read;

    while(written_amount < amount_bytes_to_read){
        if((rc = read(file, read_buffer, minimum(READ_WRITE_NORMAL, left_to_read))) == -1){
            perror("Error: failed to read from file /dev/urandom");
            return 1;
        }
        read_buffer[rc] = '\0';
        left_to_read -= rc;
        written_amount += rc;
        if(rc == 0){
            break;
        }
        /*write the actual message: */
        if(write(sockfd, read_buffer, rc) == -1){
            perror("Error: failed to write to server");
            return 1;
        }
    }
    shutdown(sockfd, SHUT_WR); /*disable further send operations, and make us not get stuck in a dead lock*/
    return 0;
}

/**
 * step4 - obtain the count of printable chars from the server
 * @param sockfd - the socket file descriptor
 * @param read_buffer - a char* to where we read
 * return -1 on failure, else returns the number of printable chars that were sent from the server
 */
int step4(int sockfd){
    unsigned int number_of_printable_characters;
    char incoming_message[MESSAGE_SIZE+1];
    incoming_message[MESSAGE_SIZE] = '\0';
    int rc;
    if((rc = read(sockfd, incoming_message, MESSAGE_SIZE)) == -1){
        perror("Error: failed to read back from server");
        return -1;
    }
    incoming_message[rc] = '\0';
    number_of_printable_characters = (unsigned int)strtoul(incoming_message, 0L, 10);
    printf("# of printable characters: %u\n", number_of_printable_characters);
    return number_of_printable_characters;
}

/**
 * minimum - finds min between two numbers
 * @param a - the first number
 * @param b - the second number
 * return the minimum
 */
unsigned int minimum(unsigned int a, unsigned int b){
    if (a > b){
        return b;
    }
    return a;
}

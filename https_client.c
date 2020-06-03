#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

SSL *ssl;

void PANIC(char *msg);
#define PANIC(msg) {perror(msg); abort();}

int recv_packet()
{
    int len=100;
    char buf[1000000];
    do {
        len=SSL_read(ssl, buf, 100);
        buf[len]=0;
        fprintf(buf);
    } while (len > 0);
    if (len < 0) {
        int err = SSL_get_error(ssl, len);
    if (err == SSL_ERROR_WANT_READ)
            return 0;
        if (err == SSL_ERROR_WANT_WRITE)
            return 0;
        if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
            return -1;
    }
}

int send_packet(const char *buf)
{
    int len = SSL_write(ssl, buf, strlen(buf));
    if (len < 0) {
        int err = SSL_get_error(ssl, len);
        switch (err) {
        case SSL_ERROR_WANT_WRITE:
            return 0;
        case SSL_ERROR_WANT_READ:
            return 0;
        case SSL_ERROR_ZERO_RETURN:
        case SSL_ERROR_SYSCALL:
        case SSL_ERROR_SSL:
        default:
            return -1;
        }
    }
}

void log_ssl()
{
    int err;
    while (err = ERR_get_error()) {
        char *str = ERR_error_string(err, 0);
        if (!str)
            return;
        fprintf(str);
        fprintf("\n");
        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    int sockfd;
    int sock;
    struct sockaddr_in dest;
	struct hostent *host;

    if ( argc != 2)
		PANIC("usage: https-client <addr>\n");

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		PANIC("Socket");
    
	bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(443); 
    
	host = gethostbyname(argv[1]);

    if (inet_aton(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]), &dest.sin_addr.s_addr) == 0)
	 	PANIC(argv[1]);

    if (connect(sockfd, (struct sockaddr *)&dest, sizeof(dest)))
        PANIC("Connect");
 
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *meth = TLSv1_2_client_method();
    SSL_CTX *ctx = SSL_CTX_new (meth);
    ssl = SSL_new (ctx);

    if (!ssl) {
        fprintf("Error creating SSL.\n");
        log_ssl();
        return -1;
    }

    sock = SSL_get_fd(ssl);
    
    SSL_set_fd(ssl, sockfd);
    int err = SSL_connect(ssl);
    if (err <= 0) {
        fprintf("Error creating SSL connection.  err=%x\n", err);
        log_ssl();
        fflush(stdout);
        return -1;
    }

    fprintf("SSL connection using %s\n", SSL_get_cipher (ssl));

    char *request = malloc(sizeof(char) * 1024); 
    char *head = "GET https://";
    char *address = argv[1];
    char *http = " HTTP/1.1\r\n\r\n";

    strcat(request,head);
    strcat(request,address);
    strcat(request,http);

    send_packet(request);
    recv_packet();
    return 0;
}

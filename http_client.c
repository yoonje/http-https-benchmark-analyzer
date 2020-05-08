#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>


#define MAXBUF 1024
void PANIC(char *msg);
#define PANIC(msg) {perror(msg); abort();}

int main(int argc, char *argv[])
{
	int sockfd, bytes_read;
	struct sockaddr_in dest;
	char buffer[MAXBUF];
	int port = atoi(argv[2]);
	struct hostent *host;

	if ( argc != 3)
		PANIC("usage: http-client <addr> <port>\n");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		PANIC("Socket");

	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	
	host = gethostbyname(argv[1]);
	
	if (inet_aton(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]), &dest.sin_addr.s_addr) == 0)
	 	PANIC(argv[1]);

	if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0)
		PANIC("Connect");

	sprintf(buffer, "GET /webhp HTTP/1.1\r\nUser-Agent: Mozilla/4.0\r\ncontent-type:text/html\r\nConnection: close\r\n\r\n");
	send(sockfd, buffer, strlen(buffer), 0);	

	do
	{
		bzero(buffer, sizeof(buffer));
		bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
		if ( bytes_read > 0)
			printf("%s", buffer);
	}
	while (bytes_read > 0);
	
	close(sockfd);
	return 0;
}

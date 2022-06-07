#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


//FUNKCJA OBSLUGUJACA BLEDY
void error(const char *msg)
	{
	perror(msg);		//wyswietlenie komunikatu o bledzie poprzedzonego informacja msg
	exit(0);		//zakonczenie dzialania programu
	}


//MAIN
int main(int argc, char *argv[])
	{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	
	//nieprawidlowa liczba argumentow
	if(argc < 3)
		{
		error("usage: ./client_1 hostname port\n"); //zapisanie oczekiwanych argumentow do standardowego strumienia bledow
		}
	
	portno = atoi(argv[2]);	//przypisanie numeru portow:
	//tworzenie socketa:
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//warstwa sieci: IP, warstwa transportu: TCP
	
	//blad przy tworzeniu socketa:
	if (sockfd < 1)
		{
		error("ERROR opening socket");
		}
	
	server = gethostbyname(argv[1]);	//pobieranie adresu servera
	
	//niepoprawny adres servera:
	if (server == NULL)
		{
		error("ERROR, no such host\n");
		}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));	//zerowanie adresu
	serv_addr.sin_family = AF_INET;			//rodzina adresow IPv4
	//kopiowanie bajt po bajcie adresu servera
	//bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);			//konwersja lokalnego hosta na format sieciowy i przypisanie numeru portu
	
	//blad polaczenia:
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		{
		error("ERROR connecting");
		}
	
	//czytanie wiadomosci z klawiatury:
	printf("Please enter the message: ");
	bzero(buffer,256);		//czyszczenie bufora
	fgets(buffer,255,stdin);	//pobieranie wiadomosci do buffora znakow z klawiatury 
	
	
	//pisanie:
	n = write(sockfd, buffer, strlen(buffer));
	
	//blad:
	if (n < 0)
		{
		error("ERROR reading from socket");
		}
	
	bzero(buffer,256);		//czyszczenie bufora
		
	//czytanie potwierdzenia:
	n = read(sockfd,buffer,18);
	
	//blad:
	if (n < 0)
		{
		error("ERROR reading from socket");
		}
	
	
	printf("%s\n",buffer);		//potwierdzenie
	bzero(buffer,256);		//czyszczenie bufora
	close(sockfd);
	
	return 0;
	}
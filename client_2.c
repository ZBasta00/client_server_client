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
	int sockfd, newsockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	char buffer[256];
	
	//nieprawidlowa liczba argumentow
	if(argc < 3)
		{
		error("usage: ./client_2 hostname port\n"); //zapisanie oczekiwanych argumentow do standardowego strumienia bledow
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
	
	
	//oczekiwanie na wiadomosc:
	//blad:
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		{
		error("ERROR on binding");
		}
		
	listen(sockfd, 10);	//nasluchiwanie na stworzonym sokecie, 10 polaczen w kolejce
	
	
	//czytanie:
	clilen = sizeof(cli_addr);	//
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);	//nowy deskryptor gniazda dla przychdzacego polaczenia
	
	//blad:
	if (newsockfd < 0)
		{
		error("ERROR on accept");
		}
	
	
	bzero(buffer,256);			//zerowanie bufora
	n = read(newsockfd,buffer,255);	//czytanie z nowego deskryptora do bufora 
	
	//blad przy czytaniu:
	if (n < 0)
		{
		error("ERROR reading from socket");
		}
	
	printf("Here is the message: %s\n",buffer);		//pokazanie wiadomosci
	n = write(newsockfd,"From cl2: I got your message",30);	//informacja zwrotna
	
	//blad informacji zwrotnej:
	if (n < 0)
		{
		error("ERROR writing to socket");
		}
	
	//koniec polaczenia i nasluchiwanie
	close(newsockfd);
	close(sockfd);
		
	return 0;
	}



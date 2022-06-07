#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
	
	int sock_1, portno_1, n_1;
	int sock_2, portno_2, n_2;
	
	char buffer_1[256];
	char buffer_2[256];
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	struct sockaddr_in cli_addr_2;
	socklen_t clilen;
	int newsockfd;
	
	
	//nieprawidlowa liczba argumentow
	if(argc < 3)
		{
		fprintf(stderr,"usage: %s port_cl1 port_cl2\n", argv[0]); //zapisanie oczekiwanych argumentow do standardowego strumienia bledow
		exit(0);		//zakonczenie dzialania programu
		}
	
	
	//przypisanie numerow portow:
	portno_1 = atoi(argv[1]);
	portno_2 = atoi(argv[2]);
	
	
	//PIERWSZY KLIENT
	
	//tworzenie socketa:
	sock_1 = socket(AF_INET, SOCK_STREAM, 0);	//warstwa sieci: IP, warstwa transportu: TCP
	
	//blad przy tworzeniu socketa:
	if (sock_1 < 1)
		{
		error("ERROR opening socket 2");
		}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));	//zerowanie adresu
	serv_addr.sin_family = AF_INET;			//rodzina adresow IPv4
	serv_addr.sin_addr.s_addr = INADDR_ANY;		//ustawienie adresu IP maszyny
	serv_addr.sin_port = htons(portno_1);			//konwersja lokalnego hosta na format sieciowy i przypisanie numeru portu
	
	//nasluchiwanie:
	//blad:
	if (bind(sock_1, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		{
		error("ERROR on binding");
		}
		
	listen(sock_1, 10);	//nasluchiwanie na stworzonym sokecie, 10 polaczen w kolejce
	
	
	//czytanie:
	clilen = sizeof(cli_addr);	//
	newsockfd = accept(sock_1,(struct sockaddr *) &cli_addr,&clilen);	//nowy deskryptor gniazda dla przychdzacego polaczenia
	//blad:
	if (newsockfd < 0)
		{
		error("ERROR on accept");
		}
	
	
	bzero(buffer_1,256);			//zerowanie bufora
	n_1 = read(newsockfd,buffer_1,255);	//czytanie z nowego deskryptora do bufora 
	
	//blad przy czytaniu:
	if (n_1 < 0)
		{
		error("ERROR reading from socket");
		}
	
	printf("Here is the message: %s\n",buffer_1);		//pokazanie wiadomosci
	n_1 = write(newsockfd,"From server: I got your message",30);	//informacja zwrotna
	strcpy(buffer_2, buffer_1);				//kopiowanie do drugiego buffora zeby przeslac dalej
	
	//blad informacji zwrotnej:
	if (n_1 < 0)
		{
		error("ERROR writing to socket");
		}
	
	//koniec polaczenia i nasluchiwanie
	close(newsockfd);
	close(sock_1);
	
	
	//KLIENT DRUGI
	
	//tworzenie socketa:
	sock_2 = socket(AF_INET, SOCK_STREAM, 0);	//warstwa sieci: IP, warstwa transportu: TCP
	
	//blad przy tworzeniu socketa:
	if (sock_2 < 1)
		{
		error("ERROR opening socket 2");
		}
	
	bzero((char *) &cli_addr_2, sizeof(cli_addr_2));	//zerowanie adresu
	cli_addr_2.sin_family = AF_INET;			//rodzina adresow IPv4
	cli_addr_2.sin_addr.s_addr = INADDR_ANY;		//ustawienie adresu IP maszyny
	cli_addr_2.sin_port = htons(portno_2);		//konwersja lokalnego hosta na format sieciowy i przypisanie numeru portu
	
	//blad polaczenia:
	if (connect(sock_2, (struct sockaddr *) &cli_addr_2, sizeof(cli_addr_2)) < 0)
		{
		error("ERROR connecting 2");
		}
	
	//pisanie do klienta pierwszego:
	n_2 = write(sock_2, buffer_2, strlen(buffer_2));
	
	//blad:
	if (n_2 < 0)
		{
		error("ERROR writing to socket 2");
		}
	
	bzero(buffer_2,256);	//czyszczenie bufora
	
	//czytanie potwierdzenia:
	n_2 = read(sock_2,buffer_2,30);
	
	//blad:
	if (n_2 < 0)
		{
		error("ERROR reading from socket 2");
		}
	
	printf("%s\n",buffer_2);	//potwierdzenie
	bzero(buffer_2,256);		//czyszczenie bufora
	close(sock_2);
	
	
	
	
	return 0;
	}
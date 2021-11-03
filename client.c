#include <stdio.h>
#include <sys/types.h>
#include <winsock2.h>
#include <windows.h>
#include <strings.h>
#include <unistd.h> 

//#pragma comment(lib, "lws2_32.lib")
#define  SERVER_PORT 5432
#define  MAX_LINE 256

int main(int argc, char* argv[]){
	// FILE* fp;
	struct hostent* hp;
	struct sockaddr_in sin; // Adresse des Sockets
	char* host;
	char buf[MAX_LINE];
	int s;
	int len;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if(argc == 2){
		host = argv[1];		
	}else{
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	}
	
	/* Uebersetze Host-Name in IP-Adresse des Partners*/
	hp = gethostbyname(host);
	if(!hp){
		fprintf(stderr, "simplex-talk: unknown host: %s \n", host);
		exit(1);
	}
	
	/*Initialisiere die Datenstruktur fuer die Adresse */
	// bzero((char*)&sin, sizeof(sin)); --> DEPRECIATED
	memset((char*)&sin, 0, sizeof(sin)); // Beschreibt den Speicherbereich fuer die Socketadresse mit Nullen
	sin.sin_family = AF_INET;
	// bcopy(hp->h_addr, (char*)&sin.sin_addr, hp->h_length); --> DEPRECIsATED
	// Kopiere den Inhalt von h_addr nach sin_addr
	memcpy((char*)&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT); 
	
	/*Aktives Oeffnen  */
	if((s = socket(PF_INET, SOCK_STREAM,0))<0){
		perror("simplex-talk: socket");
		exit(1);
	}
	
	if(connect(s, (struct sockaddr*)&sin, sizeof(sin)) < 0){
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	
	printf("test \n");
	
	/*Hauptschleife: Lies und sende Textzeilen*/
	while(fgets(buf, sizeof(buf), stdin)){
		buf[MAX_LINE - 1] = '\0';
		len = strlen(buf) + 1;
		send(s, buf, len, 0);
		//printf("Gesendet: %s \n", buf);
		if(len = recv(s, buf, sizeof(buf), 0)){
			 fputs(buf, stdout);
			//printf("Empfangen: %s \n", buf);
		}
	}
	
	 
	return 0;
}
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 

#define	SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

int main(){
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
	int s, new_s;
	
	/* Initialisiere Datenstruktur fuer die Adresse */
	memset((char*)&sin, 0, sizeof(sin)); // Beschreibt den Speicherbereich fuer die Socketadresse mit Nullen
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT); 
	
	/*Richte passivses Oeffnen ein  */
	if((s = socket(PF_INET, SOCK_STREAM,0))<0){
		perror("simplex-talk: socket");
		exit(1);
	}
	
	if((bind(s, (struct sockaddr*)&sin, sizeof(sin))) < 0){
		perror("simplex-talk: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);
	
	/* Warte auf Verbindung*/
	while(1){
		if((new_s = accept(s, (struct sockaddr*)&sin, &len))>0){
			// Das Handle des neuen Sockets ausgeben
			printf("%i \n", new_s);
			break;
		}
		else{
			printf("waiting");
		}
	}
			
	/*Empfange und drucke Text*/ 	
	while(1){
	
	    while((len = recv(new_s, buf, sizeof(buf), 0))>0){
			fputs(buf, stdout);
			len = strlen("Antwort \n") + 1;
			send(new_s, "Antwort \n", len, 0);
			// TODO: Den Puffer bereinigen wenn keine Werte mehr empfangen werden.
		}	
		close(new_s);
	
	}
	
	 
	return 0;
	
}


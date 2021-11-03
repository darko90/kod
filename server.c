#include <stdio.h>
#include <sys/types.h>
#include <winsock2.h>
#include <windows.h>
#include <strings.h>
#include <unistd.h> 

#define	SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

int main(){
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
	int s, new_s;
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
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
		for(int i=0; i<10;i++){} // Offenbar ist diese Verzoegerung notwendig damit der Verbindungsaufbau funktioniert ... (???)
		if((new_s = accept(s, (struct sockaddr*)&sin, &len))>0){
			printf("%d \n", new_s);
			break;
		}
		else{
			printf("waiting");
		}
	}
			
	/*Empfange und drucke Text*/ 	
	int recvLen = 1; // initiiere Anzahl empfangener Byte mit 1
	int respLen = 0;
	while(recvLen > 0){
		recvLen = recv(new_s, buf, sizeof(buf), 0);
		fputs(buf, stdout);
		respLen = strlen("Antwort \n") + 1;
		send(new_s, "Antwort \n", respLen, 0);
		// TODO: Den Puffer bereinigen wenn keine Werte mehr empfangen werden.
	}	
	close(new_s);
	 
	return 0;
	
}


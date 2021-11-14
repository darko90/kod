package clients;

import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

public class clientPong {
	
	private static final int EIGENER_PORT = 10001;
	private static final int GEGENSPIELER_PORT = 10000;
	private static final String SERVER_HOSTNAME = "localhost";

	public static void main(String[] args) {

		
		try {
			
			System.out.println("Warte auf den Aufschlag!");
			byte[] buffer = new byte[128];
			
			// Serversocket erzeugen um Nachrichten empfangen zu können
			ServerSocket serverSocket = new ServerSocket(EIGENER_PORT);
			
			while (true) {
				Socket clientSocket = serverSocket.accept();
				InputStream inputStream =  clientSocket.getInputStream();
				// Warten solange nichts empfanfen wird
				while(inputStream.available() == 0);
				
				// Nachricht lesen und ausgeben
//				while(inputStream.read(buffer) != -1) {
				//	System.out.println(new String(buffer));
//				}
				
				clientSocket.close();
				
				// Socket erzeugen
				Socket socket = new Socket(SERVER_HOSTNAME, GEGENSPIELER_PORT);
				// 1 Sekunde schlafen
				Thread.sleep(1000);
				System.out.println("Pong");
				//Partner über Ping benachrichtigen
				socket.getOutputStream().write(("Pong gemacht.").getBytes());
				// Socket schließen
				socket.close();				
			}
		} 
			catch (UnknownHostException e) {
			System.out.println("Rechnername unbekannt: \n" + e.getMessage());

		} catch (IOException e) {
			System.out.println("Fehler währen der Kommunikations \n" + e.getMessage());
		} 
		catch (InterruptedException e1) {

			e1.printStackTrace();

		}

	}
}

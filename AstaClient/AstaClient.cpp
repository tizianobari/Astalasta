// Autore: Bari Tiziano, Pacicco Manuel
// Classe: 5B INF
// Data: 19/03/2015
// Specifiche: Astaclient

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <QtGui>
#include "FinestraLogin.h"
using namespace std;

int main(int na, char **va)
{
     // Dichiarazione gestore dell'applicazione
     QApplication applicazione(na, va);
     
     if ( na > 1 )
     {
	// Dichiarazione del socket
	int socketClient = socket(AF_INET, SOCK_STREAM, 0);
	
	// Verifica che il socket sia stato correttamente creato
	if ( socketClient != -1)
	{
		  // Dichiarazione di una struttura dati contenente l'indirizzo IP e la porta
	      // di ascolto del server
	      sockaddr_in indirizzoServer;
	      
	      // Indica il protocollo utilizzato (Internet)
	      indirizzoServer.sin_family = AF_INET;
	      
	      // Indica il numero di porta di ascolto del server (20000)
	      indirizzoServer.sin_port = htons(20000);
	      
	      // Indica l'indirizzo del server (in network order,
	      // convertendolo con la funzione inet_addr)
	      indirizzoServer.sin_addr.s_addr = inet_addr(va[1]);
	      
	      // Verifica che il client si connetta correttamente al server
	      if ( connect(socketClient, (sockaddr*)&indirizzoServer, sizeof(sockaddr_in)) == 0 )
	      {
		  cout << "Connesso correttamente al server\n";
		  // Istanza della finestra per effettuare il login
		  // Al costrutture passo come parametro il socket connesso al server
		  FinestraLogin* finestraLogin = new FinestraLogin(socketClient);
		    
		  // Mostro la finestra
		  finestraLogin->show();
		  
		  // Avvio l'applicazione
		  finestraLogin->exec();
	      }
	      // Altrimenti...
	      else
	      {
		  //...Messaggio di errore
		  cerr << "Errore durante la connessione al server \n";
	      }
	}
	// Altrimenti...
	else
	{
	      // .. Messaggio di errore
	      cerr << "Errore durante la creazione del socket\n";
	}
    }
    else
    {
	cerr << "Usage: ./Astaclient <ip dotted decimal server>\n";
    }
}

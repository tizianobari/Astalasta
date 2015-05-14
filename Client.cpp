// Autore: Bari Tiziano
// Client di test

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main(int na, char** va)
{
    int socketClient;
    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if ( socketClient != -1 )
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
        indirizzoServer.sin_addr.s_addr = inet_addr("127.0.0.1");
          
        // Connessione al server
        if ( connect(socketClient, (sockaddr*)&indirizzoServer, sizeof(sockaddr_in)) == 0 )
        {
            int scritto;
            while (1)
            {
                cout << "Connesso inserire numero intero: ";
                cin >> scritto;
                send(socketClient, &scritto, 4, 0);
            }
        }
        else
        {
            cerr << "Errore durante la connessione al server\n";
        }
    }
    else
    {
        cerr << "Errore durante la creazione del socket\n";
    }
}

// Autore: Bari Tiziano, Paccico Manuel
// Data inizio: 05/03/2015
// Classe: 5B INF
// Specifiche: Gestione di aste online (lato SERVER)
// 12/03/2015 - Gestione socket, gestione accettazione client
///g++ -pthread AstaServer.cpp -o AstaServer -I/usr/include/mysql -lmysqlclient_r
#include <iostream>
#include <mysql/mysql.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Effettuare cifrature in md5
//#include <openssl/md5.h>
// Segnali
#include <signal.h>
// Pipe
#include <unistd.h>
#include <errno.h>

using namespace std;

// Classe da appoggio per passare le variabili di connessione al
// thread sganciato che eseguirà le richieste del client
class Connessioni
{
public:
    // Attributi
    int socketClient;
    // Variabile mutex per gestire l'accesso concorrenziale al database
    pthread_mutex_t semaforoDatabase;
    // Variabile mutex per gestire l'uso concorrenziale di ciascun socket
    // tra le operazioni del server e l'aggiornamento delle puntate
    pthread_mutex_t semaforoSocket;
    // Variabile contenente la pipe
    // pipe[1] per scrivere
    // pipe[0] per leggere
    int* pipe;
    // Variabile per contenere l'id del pthread lanciato per la gestione
    // delle puntate
    pthread_t compitoPuntate;
    // Puntatori all'elemento successivo e precedente per effettuare
    // una lista doppiamente concatenata
    Connessioni* successivo;
    Connessioni* precedente;
};

// Classe da appoggio per passare le variabili necessarie per la gestione
// delle puntate
class ConnessioniPuntate
{
public:
    // Record generatore per la lista delle connessioni
    Connessioni* recordGeneratore;
    // Segnali da intercettare
    sigset_t segnaliSettati;
};

void* operazioniServer(void*);
void* invioAggiornamentiPuntate(void*);

void stampaLista(Connessioni* rg)
{
    Connessioni* attuale = rg->successivo;
    cerr << "\n\nLista\n";
    while ( attuale != NULL )
    {
        cerr << "Elemento: " << attuale << "\n";
        attuale = attuale->successivo;
    }
}

int main(int na, char **va)
{
     // Dichiarazione puntatore alla variabile di connessione MYSQL
     MYSQL* connessione;
     
     // Inizializzo le librerie mysql
     mysql_library_init(0, NULL, NULL);
     
     // Inizializzazione variabile MYSQL
     connessione = mysql_init(0);
     
     // Connessione al database tramite la funzione mysql_real_connect
     // Parametri: 1- La variabile di tipo MYSQL*
     //            2- L'indirizzo IP del server database a cui connettersi
     //            3- L'utente con cui connettersi al server database
     //            4- La password dell'utente con cui connettersi al server database
     //            5- Il nome del database a cui connettersi
     //            6- La porta da usare durante la connessione (0: default (3306))
     //            7- La named pipe da usare (NULL, default)
     //            8- Flag per ulteriori configurazioni (0: default)
     // Valori di ritorno: ?
     //     0: la connessione è fallita
     //     1: la connessione ha avuto successo
     if ( mysql_real_connect(connessione, "127.0.0.1", "quintabinf", "annonuovo", "Astalasta", 0, NULL, 0) != NULL )
     {
         // Dichiarazione del socket
         // Il socket usa Internet (AF_INET)
         //		il protocollo di comunicazione è TCP (instaurazione della connessione)
         // 	usa i parametri di default
         int socketServer;
         socketServer = socket(AF_INET, SOCK_STREAM, 0);
         
         // Verifica che il socket sia stato creato correttamente
         if ( socketServer != -1 )
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
              indirizzoServer.sin_addr.s_addr = inet_addr("0.0.0.0");
              
              // Lego l'indirizzo dell'host al socket, ovvero dove 
              // il socket sarà in ascolto (indirizzo e porta locale)
              // Parametri: 1- Il socket
              //		2- L'indirizzo della struttura contenente l'indirizzo e il numero di porta del server
              //		   (sockaddr*)
              //		3- La dimensione della struttura di dati contenente l'indirizzo
              if ( bind(socketServer, (sockaddr*)&indirizzoServer, sizeof(sockaddr_in)) == 0)
              {
                   // Mette in ascolto il server per le richieste dei client
                   // Parametri: 
                   if ( listen(socketServer, 50) == 0 )
                   {
                        // Dichiarazione di una struttura dati che verrà riempita con i dati
                        // del client connesso durante la funzione accept()
                        sockaddr_in indirizzoClient;
                        int socketClient;
                        unsigned int dimensioneSocket = sizeof(sockaddr_in);
                        
                        // Dichiarazione variabile contenente gli attributi di creazione del thread
                        pthread_attr_t attributi;
                        // Inizializzazione degli attributi del thread
                        pthread_attr_init(&attributi);
                        
                        // Puntatore ad un'area di memoria che conterrà i parametri passati
                        // al thread per gestire le operazioni del client
                        Connessioni* parametriConnessione;
                        
                        // Istanza dell'oggetto ConnessioniPuntate da passare al thread per la gestione
                        // dell'aggiornamento delle puntate
                        ConnessioniPuntate* parametriPuntate = new ConnessioniPuntate;
                        // Inizializzazione lista vuota
                        parametriPuntate->recordGeneratore = new Connessioni;
                        parametriPuntate->recordGeneratore->successivo = NULL;
                        // Creazione della mutex per gestire la concorrenza sulle query nel database
                        pthread_mutex_t semaforo;
                        // Inizializzazione statica della variabile mutex
                        semaforo = PTHREAD_MUTEX_INITIALIZER;
                        
                        // Dichiarazione variabile pthread_t contenente l'id del thread che gestisce
                        // l'aggiornamento delle puntate ai client
                        pthread_t compitoAggiornamentoPuntate;
                        
                        /// Chiamata della funzione signal per intercettare ii segnali
                        sigemptyset(&(parametriPuntate->segnaliSettati));
                        sigaddset(&(parametriPuntate->segnaliSettati), SIGUSR1);
                        pthread_sigmask(SIG_BLOCK, &(parametriPuntate->segnaliSettati), NULL);
                        
                        // Creazione della pipe per far comunicare diversi 
                        int canale[2];
                        pipe(canale);
                        
                        /*
                        // Iterazione per inizializzare i valori del vettore
                        int indice = 0;
                        while ( indice < 50 )
                        {
                            parametriPuntate->connessioni[indice] = 0;
                            indice = indice + 1;
                        }                                               
                        */
                        
                        // Creazione di un thread per gestire l'aggiornamento delle puntate ai
                        // client attualmente connessi
                        if ( pthread_create(&compitoAggiornamentoPuntate, &attributi, invioAggiornamentiPuntate, (void*)parametriPuntate) == 0 )
                        {
                            // Gestione delle connessioni da parte del client
                            while (1)
                            {
                                 // Accetto le connessioni con i vari client che ne fanno richiesta
                                 // Parametri: 1- Il socket
                                 // 	       2- L'indirizzo della struttura dove scrivere i dati del client
                                 // 	       3- La dimensione della struttura di dati contenente l'indirizzo
                                 // Valore di ritorno: Il descrittore del socket connesso con il client
                                 if ( (socketClient = accept(socketServer, (sockaddr*)&indirizzoClient, &dimensioneSocket)) != -1 )
                                 {
                                      cerr << "Accettata connessione\n";
                                      // Creazione di un thread per gestire le richieste del client
                                      // (un thread per ciascun client)
                                      // Dichiarazione variabile pthread_t che conterrà l'id del thread
                                      pthread_t compito;
                                      
                                      // Preparazione del parametro da passare al thread successivamente
                                      // che conterrà le variabili di connessione verso il client e il database
                                      parametriConnessione = new Connessioni();
                                      parametriConnessione->socketClient = socketClient;
                                      parametriConnessione->semaforoDatabase = semaforo;
                                      parametriConnessione->semaforoSocket = PTHREAD_MUTEX_INITIALIZER;
                                      parametriConnessione->pipe = canale;
                                      parametriConnessione->compitoPuntate = compitoAggiornamentoPuntate;
                                      
                                      // Creazione del thread usando la funzione pthread_create
                                      // Parametri: 1- L'indirizzo della variabile pthread_t che conterrà l'id del thread
                                      //            2- L'indirizzo della variabile pthread_attr_t che conterrà l'attributi del thread
                                      //            3- La routine (void*) che verrà eseguita sganciandola con il thread
                                      //            4- L'indirizzo del parametro da passare alla routine sganciata (void*)
                                      if ( pthread_create(&compito, &attributi, operazioniServer, (void*)parametriConnessione) == 0 )
                                      {
                                            cout << "Lanciato pthread!\n";
                                            // Registro le connessioni nella lista
                                            parametriConnessione->precedente = parametriPuntate->recordGeneratore;
                                            parametriConnessione->successivo = parametriPuntate->recordGeneratore->successivo;
                                            parametriPuntate->recordGeneratore->successivo = parametriConnessione;
                                            cerr << "\n\nConnessione creata: " << parametriConnessione << "\n";
                                            cerr << "Record generatore: " << parametriPuntate->recordGeneratore << "\n";
                                            cerr << "Record generatore->successivo: " << parametriPuntate->recordGeneratore->successivo << "\n\n\n";
                                            
                                            stampaLista(parametriPuntate->recordGeneratore);
                                      }
                                      // Altrimenti...
                                      else
                                      {
                                            cerr << socketClient;
                                            // ...Messaggio di errore
                                            cerr << "Errore durante la creazione del thread\n";
                                            // Libero la memoria
                                            delete parametriConnessione;
                                      }
                                 }
                                 // Altrimenti...
                                 else
                                 {
                                     // ...Messaggio di errore
                                     cerr << "Connessione non accettata\n";
                                     cerr << errno;
                                 }
                            }
                        }
                        // Altrimenti..
                        else
                        {
                            //...Messaggio di errore
                            cerr << "Errore durante la creazione del thread per la gestione delle puntate\n";
                        }
                   }
                   // Altrimenti...
                   else
                   {
                        // ...Messaggio di errore
                        cerr << "Errore durante la messa in ascolto del server\n";
                   }
              }
              // Altrimenti...
              else
              {
                   // ...Messaggio di errore
                   cerr << "Errore durante il bind\n";
                   cerr << errno;
              }
         }
         // Altrimenti...
         else
         {
          // Messaggio di errore
          cerr << "Errore durante la creazione del socket\n";
         }
     }
     // Altrimenti...
     else
     {
         // Messaggio di errore
         cerr << "Errore durante la connessione al database\n";
     }
     mysql_library_end();
}

// Implementazione routine del thread che soddisferà le richieste del client
void* operazioniServer(void* parametri)
{
    // Dichiarazione variabili
    Connessioni* connessioni = (Connessioni*)parametri;
    Connessioni* appoggio;
    // Variabile usata per leggere il servizio richiesto dal client
    int servizio;
    // Dichiarazione puntatori per i dati che verranno usati durante le richieste
    char username[33];
    char password[33];

    // Servizio 1
    int idUtente;
    int successo;
    bool connesso = true;
    // Servizio 3
    int offset;
    int numeroRighe;
    char* nomeOggetto;
    char* descrizioneOggetto;
    int secondiOggetto;
    // Servizio 2-3
    int idOggetto;
    double prezzoOggetto;
    
    // Dichiarazione variabili per formare query sql ed ottenere i dati
    char comandoSql[500];
    MYSQL connessione;
    MYSQL_RES* risultato = NULL;
    MYSQL_ROW riga;
    
    // Inizializzazione variabile MYSQL
    mysql_init(&connessione);
    
    // Connessione al server database
    if ( mysql_real_connect(&connessione, "127.0.0.1", "quintabinf", "annonuovo", "Astalasta", 0, NULL, 0) != NULL )
    {
        // Iterazione per la gestione delle richieste del client
        while ( connesso )
        {
            cout << "Socket client" << connessioni->socketClient << "\n";
            cout << "In attesa di ricezione del numero del servizio\n";
            if ( recv(connessioni->socketClient, &servizio, 4, 0) > 0 )
            {
                cout << "Ricevuto numero: " << servizio << "\n";					
                switch ( servizio )
                {
                    // Servizio = 1
                    // Ciò che segue è una richiesta di login composta da
                    // 64 caratteri (32 per username, 32 per la password)
                    case 1:
                        // Ricezione dei dati inviati dal client (username, password) e setto
                        // il carattere di terminazione
                        recv(connessioni->socketClient, username, 32, 0);
                        recv(connessioni->socketClient, password, 32, 0);
                        username[32] = 0;
                        password[32] = 0;
                                                                    
                        // Istanza del vettore di caratteri contenente il comando SQL da inviare
                        // al server database
                        //comandoSql = new char(150);
                        
                        // Formazione del comando SQL
                        sprintf(comandoSql, "SELECT id FROM Clienti WHERE username='%s' AND password='%s'", username, password);
                        
                        cout << comandoSql << "\n";
                        /// CONCORRENZA: non è possibile inviare query contemporaneamente nella stessa connessione
                        ///              bisogna gestire l'accesso al database
                        /// Lock della variabile mutex, se è bloccata aspetto lo sblocco
                        pthread_mutex_lock(&(connessioni->semaforoDatabase));
                        
                        // Invio della query 
                        mysql_query(&connessione, comandoSql);
                        
                        // Ottengo il risultato della query
                        risultato = mysql_store_result(&connessione);
                        
                        /// FINE CONCORRENZA: Sblocco della variabile mutex dopo aver ricevuto i dati
                        pthread_mutex_unlock(&(connessioni->semaforoDatabase));
                        
                        // Verifico i dati ottenuti
                        if ( risultato != NULL )
                        {
                            riga = mysql_fetch_row(risultato);
                            
                            /// CONCORRENZA: scrittura sul socket 
                            pthread_mutex_lock(&(connessioni->semaforoSocket));
                            // Lettura della riga ottenuta se ho ottenuto risultati dalla query
                            if ( riga != NULL )
                            {
                                 idUtente = atoi(riga[0]);
                                 // Il valore ritornato della query è l'id dell'utente (login corretto)
                                 // l'id è un valore intero, uso della funzione atoi() per convertirlo in int
                                 // Invio dell'id al client (id > 0)
                                 send(connessioni->socketClient, &idUtente, 4, 0);
                            }
                            // Altrimenti non ho ottenuto alcun risultato, quindi il login è fallito
                            else
                            {
                                 idUtente = 0;
                                 // Invio dell'id al client (id = 0)
                                 send(connessioni->socketClient, &idUtente, 4, 0);
                            }
                            
                            /// FINE CONCORRENZA:  fine scrittura sul socket
                            pthread_mutex_unlock(&(connessioni->semaforoSocket));
                            
                            // Libero la memoria
                            mysql_free_result(risultato);
                        }
                    break;
                    
                    // Servizio = 2
                    // Ciò che segue è una richiesta di effettuare una puntata
                    // composto da 44 byte formati da:
                    //   4 byte (intero) per l'id dell'utente
                    //   4 byte (intero) per l'id dell'oggetto
                    //   8 byte (double) per la puntata da effettuare (prezzo)
                    case 2:		 
                        
                         // Ricezione dei dati inviati al server (username, id oggetto, prezzo)
                         recv(connessioni->socketClient, &idUtente, 4, 0);
                         recv(connessioni->socketClient, &idOggetto, 4, 0);
                         recv(connessioni->socketClient, &prezzoOggetto, 8, 0);
                         
                         // Istanza al vettore di caratteri per formare la query SQL
                         //comandoSql = new char(200);
                         
                         // Formazione del comando SQL
                         sprintf(comandoSql, "INSERT INTO Puntate (rifCliente, rifArticolo, prezzo, data) VALUES (%i, %i, %f, now())",
                             idUtente, idOggetto, prezzoOggetto);
                         
                         cout << comandoSql << "\n";
                         
                         /// CONCORRENZA: non è possibile inviare query simultanee con la stessa connessione
                         ///	          bisogna gestire l'accesso al database
                         // Lock della variabile mutex per l'invio della query, se è bloccata aspetto lo sblocco
                         pthread_mutex_lock(&(connessioni->semaforoDatabase));
                         
                         // Invio della query, il valore di ritorno è se l'inserimento ha avuto successo oppure no
                         // Valore di ritorno: 0 successo, altrimenti errore
                         successo = mysql_query(&connessione, comandoSql);
                         
                         if ( successo == 0 )
                         {
                            sprintf(comandoSql, "UPDATE Articoli SET prezzoAttuale=%f WHERE id=%i", prezzoOggetto, idOggetto);
                            cout << comandoSql << "\n";
                            successo = mysql_query(&connessione, comandoSql);
                         }
                         
                         /// CONCORRENZA SOCKET
                         pthread_mutex_lock(&(connessioni->semaforoSocket));
                         // Servizio = 2
                         send(connessioni->socketClient, &servizio, 4, 0);
                         send(connessioni->socketClient, &successo, 4, 0);
                         pthread_mutex_unlock(&(connessioni->semaforoSocket));
                         /// FINE CONCORRENZA SOCKET
                         /// FINE CONCORRENZA: Sblocco della variabile mutex dopo aver riceduto i dati
                         pthread_mutex_unlock(&(connessioni->semaforoDatabase));	
                                          
                         // Scrittura sulla pipe dell'id dell'oggetto da inviare gli aggiornamenti
                         write(connessioni->pipe[1], &idOggetto, 4);                 
                         
                         // Invio di un segnale SIGUSR1 al thread che gestisce l'aggiornamento delle puntate
                         pthread_kill(connessioni->compitoPuntate, SIGUSR1);
                    break;
                    
                    // Servizio = 3
                    // Ciò che segue è l'offset delle righe ottenute e il numero di righe da ottenere
                    // 4 byte (intero) per l'offset
                    // 4 byte (intero) per il numero di righe
                    case 3:
                        // Ricezione dei dati inviati al server
                        recv(connessioni->socketClient, &offset, 4, 0);
                        recv(connessioni->socketClient, &numeroRighe, 4, 0);
                        
                        // Formazione del comando SQL
                        sprintf(comandoSql, 
                            "SELECT id, nome, descrizione, prezzoAttuale, TIMESTAMPDIFF(SECOND, NOW(), dataFine) AS TempoRimanente FROM Articoli WHERE rifCliente IS NULL HAVING TempoRimanente>0 LIMIT %i,%i",
                                offset, numeroRighe);
                                
                        cout << comandoSql << "\n";
                        /// CONCORRENZA: non è possibile inviare query contemporaneamente nella stessa connessione
                        ///              bisogna gestire l'accesso al database
                        /// Lock della variabile mutex, se è bloccata aspetto lo sblocco
                        pthread_mutex_lock(&(connessioni->semaforoDatabase));
                        
                        // Invio della query 
                        mysql_query(&connessione, comandoSql);
                        
                        // Ottengo il risultato della query
                        risultato = mysql_store_result(&connessione);
                        
                        /// FINE CONCORRENZA: Sblocco della variabile mutex dopo aver ricevuto i dati
                        pthread_mutex_unlock(&(connessioni->semaforoDatabase));
                        
                        // Verifico i dati ottenuti
                        if ( risultato != NULL )
                        {
                            // Lettura della prima riga
                            while ( riga = mysql_fetch_row(risultato) )
                            {
                                cout << "Ricevuto righe!" << riga[0] << "\n";
                                // Campi della riga
                                // riga[0] = idOggetto, intero
                                // riga[1] = nomeOggetto, stringa 30 caratteri
                                // riga[2] = descrizioneOggetto, stringa 50 caratteri
                                // riga[3] = prezzoOggetto, double
                                // riga[4] = secondiOggetto, intero
                                idOggetto = atoi(riga[0]);
                                nomeOggetto = riga[1];
                                descrizioneOggetto = riga[2];
                                prezzoOggetto = atof(riga[3]);
                                secondiOggetto = atoi(riga[4]);
                                
                                // Invio al client i dati dell'oggetto (oltre al servizio)
                                /// CONCORRENZA: scrittura sul socket
                                pthread_mutex_lock(&(connessioni->semaforoSocket));
                                send(connessioni->socketClient, &servizio, 4, 0);
                                send(connessioni->socketClient, &idOggetto, 4, 0);
                                send(connessioni->socketClient, nomeOggetto, 30, 0);
                                send(connessioni->socketClient, descrizioneOggetto, 50, 0);
                                send(connessioni->socketClient, &prezzoOggetto, 8, 0);
                                send(connessioni->socketClient, &secondiOggetto, 4, 0);
                                /// FINE CONCORRENZA: fine scrittura sul socket
                                pthread_mutex_unlock(&(connessioni->semaforoSocket));
                            }
                        }
                        // Libero la memoria
                        mysql_free_result(risultato);
                    break;
                    
                    // Servizio 8:
                    // Inserimento nuovo oggetto: nome, descrizione, prezzo
                    case 8:
                        // Ricezione dei dati inviati dal client
                        recv(connessioni->socketClient, nomeOggetto, 30, 0);
                        recv(connessioni->socketClient, descrizioneOggetto, 50, 0);
                        recv(connessioni->socketClient, &prezzoOggetto, 8, 0);
                        
                        // Formazione del comando SQL
                        sprintf(comandoSql, "INSERT INTO Articoli (nome, descrizione, prezzoIniziale, prezzoAttuale, dataInizio, dataFine) VALUES ('%s', '%s', %f, %f, NOW(), DATE_ADD(NOW(), INTERVAL 7 DAY))"
                        , nomeOggetto, descrizioneOggetto, prezzoOggetto, prezzoOggetto);
                        //strcat(comandoSql, nomeOggetto);
                        //sprintf(comandoSql, "', '");
                        //strcat(comandoSql, descrizioneOggetto);
                        //sprintf(comandoSql, "', %f, %f, NOW(), DATE_ADD(NOW(), INTERVAL 7 DAY))",prezzoOggetto, prezzoOggetto);
                        cerr << comandoSql << "\n";
                        
                        /// CONCORRENZA: non è possibile inviare query contemporaneamente nella stessa connessione
                        ///              bisogna gestire l'accesso al database
                        /// Lock della variabile mutex, se è bloccata aspetto lo sblocco
                        pthread_mutex_lock(&(connessioni->semaforoDatabase));
                        
                        // Invio della query 
                        mysql_query(&connessione, comandoSql);
                                                
                        /// FINE CONCORRENZA: Sblocco della variabile mutex dopo aver ricevuto i dati
                        pthread_mutex_unlock(&(connessioni->semaforoDatabase));
                        
                        
                    break;
                }
            }
            else
            {
                // Errore durante la ricezione
                connesso = false;
                cerr << "Errore durante la ricezione o il client ha chiuso la comunicazione\n";
                // Elimino dalla lista il nodo corrente visto che è disconnesso
                if ( connessioni->precedente != NULL )
                {
                    (connessioni->precedente)->successivo = connessioni->successivo;
                }
                if ( connessioni->successivo != NULL )
                {
                    (connessioni->successivo)->precedente = connessioni->precedente;
                }
                cerr << "Esplodo prima di eliminare\n";
                delete connessioni;
                cerr << "Eliminato!\n";
            }
        }
    }
    else
    {
        cerr << "Errore thread-mysql\n";
    }
    mysql_thread_end();
    pthread_exit(0);
}

void* invioAggiornamentiPuntate(void* parametri)
{
    // Dichiarazione variabili
    ConnessioniPuntate* dati = (ConnessioniPuntate*)parametri;
    Connessioni* connessioneAttuale = NULL;
    //connessioneAttuale = dati->recordGeneratore->successivo; non funziona
    //connessioneAttuale = dati->recordGeneratore;
    //connessioneAttuale = connessioneAttuale->successivo;
    int segnale;
    int idOggetto;
    double prezzo;
    int servizio = 5;
    char comandoSql[100];
    MYSQL* connessione;
    MYSQL_RES* risultato = NULL;
    MYSQL_ROW riga;
    
    // Inizializzazione variabile MYSQL per effetuare la connessione
    connessione = mysql_init(0);
    
    // Connessione al server database
    if ( mysql_real_connect(connessione, "127.0.0.1", "quintabinf", "annonuovo", "Astalasta", 0, NULL, 0) != NULL )
    {
        while (1)
        {
            // Chiamata della procedura sigwait() per attendere un segnale
            if ( sigwait(&(dati->segnaliSettati), &segnale) == 0 )
            {
                connessioneAttuale = dati->recordGeneratore->successivo;
                // Se ha ottenuto il segnale
                if ( segnale == SIGUSR1 )
                {
                    cout << "Segnale ricevuto\n";
                    
                    cerr << "(thread) Record generatore: " << dati->recordGeneratore << "\n";
                    cerr << "(thread) Record generatore->successivo: " << dati->recordGeneratore->successivo << "\n";
                    cerr << "(thread) Record generatore(connessioneAttuale) = " << connessioneAttuale << "\n";
                    cerr << "(thread) Record generatore->successivo (connessioneAttuale->successivo) = " << connessioneAttuale->successivo << "\n";
                    
                    // Lettura dalla pipe dell'ID dell'oggetto da aggiornare
                    if ( read(connessioneAttuale->pipe[0], &idOggetto, 4) > 0 )
                    {
                        // Iterazione per scorrere la lista di connessioni
                        while ( connessioneAttuale != NULL )
                        {
                            cerr << "Dentro al while della lista\n";

                            // Formo la query
                            sprintf(comandoSql, "SELECT prezzoAttuale FROM Articoli WHERE id=%i", idOggetto);
                            /// CONCORRENZA: Mutua esclusione per l'utilizzo della connessione al DBMS
                            pthread_mutex_lock(&(connessioneAttuale->semaforoDatabase));
                            // Query nel database per richiedere tutti i dati necessari per l'aggiornamento
                            // dell'oggetto coinvolto da inviare ai client
                            mysql_query(connessione, comandoSql);
                            // Ottengo i risultati dalla query 
                            risultato = mysql_store_result(connessione);
                            /// FINE CONCORRENZA: Dopo aver ottenuto i risultati, sblocco la mutex
                            pthread_mutex_unlock(&(connessioneAttuale->semaforoDatabase));
                            // La query ritorna un solo valore, il prezzo aggiornato dell'id selezionato
                            if ( riga = mysql_fetch_row(risultato) )
                            {
                                // Converto il prezzo scritto come caratteri in un double con la funzione atof()
                                prezzo = atof(riga[0]);
                            }
                            else
                            {
                                cerr << "Nessun prezzo trovato (aggiornamento puntate)\n";
                            }
                            // Invio dei risultati a tutti i client connessi    
                            /// CONCORRENZA: Verifica se stanno trasmettendo dati sul socket
                            pthread_mutex_lock(&(connessioneAttuale->semaforoSocket));
                            cerr << "Invio aggiornamento\n";
                            // Invio: servizio(int, 5), idOggetto (int), prezzo(double)
                            send(connessioneAttuale->socketClient, &servizio, 4, 0);
                            send(connessioneAttuale->socketClient, &idOggetto, 4, 0);
                            send(connessioneAttuale->socketClient, &prezzo, 8, 0);
                            
                            /// FINE CONCORRENZA: Fine trasmissione                          
                            pthread_mutex_unlock(&(connessioneAttuale->semaforoSocket));     
                            
                            // Passo al nodo successivo
                            cerr << "Nodo attuale è " << connessioneAttuale << "\n";
                            cerr << "Passo al nodo successivo\n";
                            connessioneAttuale = connessioneAttuale->successivo;
                            cerr << "Nodo attuale è " << connessioneAttuale << "\n";                       
                        }
                        cerr << "Uscito dal ciclo\n";
                    }
                    else
                    {
                        cerr << "Errore durante la lettura del servizio (gestionepuntate)\n";
                    }
                }
            }
            else
            {
                cerr << "Errore durante la cattura del segnale\n";
            }
        }
    }
    else
    {
        cerr << "Errore durante thread-mysql\n";
    }
}

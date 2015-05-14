// Autore: Bari Tiziano, Pacicco Manuel
// Classe: 5B INF
// Data: 24/03/2015
// Specifiche: Implementazione finestra principale di Astalasta

#include "FinestraPrincipale.h"
#include "Dati.h"
#include "FinestraNuovoOggetto.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QString>

// Implementazione costruttore
// Parametri passati: socketClient, idUtente
// Chiamata del costruttore della superclasse QMainWindow
FinestraPrincipale::FinestraPrincipale(int socketClient, int idUtente) : QMainWindow(0)
{       
    // Imposto gli attributi
    this->socketClient = socketClient;
    this->idUtente = idUtente;
    numeroOggetti = 6;
    indice = 0;
    offset = 0;
    
    // Setto il titolo della finestra
    setWindowTitle("Astalasta");
    
    // Dimensiono la finestra
    resize(800, 600);
    
    // Aggiungo il menù File alla barra dei menù impostando
    // la shortcut ALT + F per aprirla
    menuFile = menuBar() -> addMenu("&File");
    
    // Aggiunta della voce "Esci" al menu "menuFile" e lo associo
    // all'azione da eseguire 
    menuFile -> addAction("Esci", this, SLOT(close()));
    menuFile->addAction("Aggiungi oggetto", this, SLOT(aggiungiOggetto()));
    
    // Inizializzazione vettore degli 'idOggetti'
    while (indice < numeroOggetti )
    {
        idOggetti[indice] = 0;
        indice = indice + 1;
    }
    
    // Istanza oggetti grafici
    indice = 0;
    while ( indice < numeroOggetti )
    {
        // Istanza oggetti grafici
        caselleNomiOggetti[indice] = new QLineEdit(this);
        caselleDescrizioniOggetti[indice] = new QLineEdit(this);
        casellePrezziOggetti[indice] = new QLineEdit(this);
        timerOggetti[indice] = new TestoTimer(this);
        bottoniPuntata[indice] = new QPushButton("Punta!", this);
        casellePrezziOfferti[indice] = new QDoubleSpinBox(this);
        
        // Setto le caselle di testo come sola lettura
        caselleNomiOggetti[indice]->setReadOnly(true);
        caselleDescrizioniOggetti[indice]->setReadOnly(true);
        casellePrezziOggetti[indice]->setReadOnly(true);
        timerOggetti[indice]->setReadOnly(true);
        
        // Setto lo scalo minimo per il prezzo
        casellePrezziOfferti[indice]->setSingleStep(0.01);
        casellePrezziOfferti[indice]->setMaximum(9999.99);
        // Passo all'elemento successivo del vettore
        indice = indice + 1;
    }
    
    // Posizionamento degli oggetti grafici
    caselleNomiOggetti[0]->setGeometry(20,40,201,31);
    caselleDescrizioniOggetti[0]->setGeometry(20,90,101,101);
    timerOggetti[0]->setGeometry(130,90,71,31);
    casellePrezziOggetti[0]->setGeometry(140,150,51,31);
    casellePrezziOfferti[0]->setGeometry(40,200,62,27);
    bottoniPuntata[0]->setGeometry(130,200,71,25);
    
    caselleNomiOggetti[1]->setGeometry(300,40,201,31);
    caselleDescrizioniOggetti[1]->setGeometry(300,90,101,101);
    timerOggetti[1]->setGeometry(410,90,71,31);
    casellePrezziOggetti[1]->setGeometry(420,150,51,31);
    casellePrezziOfferti[1]->setGeometry(320,200,62,27);
    bottoniPuntata[1]->setGeometry(410,200,71,25);
    
    caselleNomiOggetti[2]->setGeometry(560,40,201,31);
    caselleDescrizioniOggetti[2]->setGeometry(560,90,101,101);
    timerOggetti[2]->setGeometry(670,90,71,31);
    casellePrezziOggetti[2]->setGeometry(680,150,51,31);
    casellePrezziOfferti[2]->setGeometry(580,200,62,27);
    bottoniPuntata[2]->setGeometry(670,200,71,25);
    
    caselleNomiOggetti[3]->setGeometry(20,320,201,31);
    caselleDescrizioniOggetti[3]->setGeometry(20,370,101,101);
    timerOggetti[3]->setGeometry(130,370,71,31);
    casellePrezziOggetti[3]->setGeometry(140,430,51,31);
    casellePrezziOfferti[3]->setGeometry(40,480,62,27);
    bottoniPuntata[3]->setGeometry(130,480,71,25);
    
    caselleNomiOggetti[4]->setGeometry(300,320,201,31);
    caselleDescrizioniOggetti[4]->setGeometry(300,370,101,101);
    timerOggetti[4]->setGeometry(410,370,71,31);
    casellePrezziOggetti[4]->setGeometry(420,430,51,31);
    casellePrezziOfferti[4]->setGeometry(320,480,62,27);
    bottoniPuntata[4]->setGeometry(410,480,71,25);
    
    caselleNomiOggetti[5]->setGeometry(560,320,201,31);
    caselleDescrizioniOggetti[5]->setGeometry(560,370,101,101);
    timerOggetti[5]->setGeometry(670,370,71,31);
    casellePrezziOggetti[5]->setGeometry(680,430,51,31);
    casellePrezziOfferti[5]->setGeometry(580,480,62,27);
    bottoniPuntata[5]->setGeometry(670,480,71,25);
    
    // Imposto l'intercettatore degli eventi
    // Parametri: oggetto a cui allegare l'evento
    //            evento da intercettare
    //            chi lo intercetta
    //            funzione da eseguire quando intercettato
    connect(bottoniPuntata[0], SIGNAL(clicked()), this, SLOT(effettuaPuntata()));
    connect(bottoniPuntata[1], SIGNAL(clicked()), this, SLOT(effettuaPuntata()));
    connect(bottoniPuntata[2], SIGNAL(clicked()), this, SLOT(effettuaPuntata()));
    connect(bottoniPuntata[3], SIGNAL(clicked()), this, SLOT(effettuaPuntata()));
    connect(bottoniPuntata[4], SIGNAL(clicked()), this, SLOT(effettuaPuntata()));
    connect(bottoniPuntata[5], SIGNAL(clicked()), this, SLOT(effettuaPuntata()));
    
    // Istanza per passare i dati al thread
    Dati* dati = new Dati();
    dati->idOggetti = this->idOggetti;
    dati->caselleNomiOggetti = this->caselleNomiOggetti;
    dati->caselleDescrizioniOggetti = this->caselleDescrizioniOggetti;
    dati->casellePrezziOggetti = this->casellePrezziOggetti;
    dati->casellePrezziOfferti = this->casellePrezziOfferti;
    dati->timerOggetti = this->timerOggetti;
    dati->bottoniPuntata = this->bottoniPuntata;
    dati->socketClient = this->socketClient;
    
    // Lancio il thread per la gestione della ricezione dei servizi da parte del server
    pthread_attr_t attributi;
    pthread_attr_init(&attributi);
    pthread_create(&compitoRicezione, &attributi, FinestraPrincipale::riceviServizi, (void*)dati);
    
    // Mostro la finestra
    this->show();
}


void FinestraPrincipale::richiediOggetti()
{
    // Richiesta degli oggetti attualmente all'asta
    servizio = 3;
    send(socketClient, &servizio, 4,0);
    send(socketClient, &offset, 4, 0);
    send(socketClient, &numeroOggetti, 4, 0);
    
    // Libero le informazioni registrate fino ad ora
    int pedice = 0;
    while ( pedice < numeroOggetti )
    {
        //cerr << "Dentro al ciclo\n";
        if ( idOggetti != 0 )
        {
            //cerr << "Dentro l'if\n";
            idOggetti[pedice] = 0;
            caselleNomiOggetti[pedice]->clear();
            caselleDescrizioniOggetti[pedice]->clear();
            casellePrezziOggetti[pedice]->clear();
            timerOggetti[pedice]->clear();
            casellePrezziOfferti[pedice]->setMinimum(0.0);
            casellePrezziOfferti[pedice]->setValue(0.0);
            bottoniPuntata[pedice]->setEnabled(false);
        }
        pedice = pedice + 1;
    }
}

// non usata
void FinestraPrincipale::ottieniOggetto()
{
    //  idOggetto, intero --> intero
    // nomeOggetto, stringa 30 caratteri --> dentro QLineEdit[indice] caselleNomiOggetti
    //  descrizioneOggetto, stringa 50 caratteri --> QLineEdit[indice] caselleDescrizioniOggetti
    // prezzoOggetto, double --> dentro QLineEdit[indice] casellePrezziOggetti
    // secondiOggetto, intero --> dentro QTimer[indice] timerOggetti
    int idOggetto;
    char nomeOggetto[31];
    char descrizioneOggetto[51];
    double prezzoOggetto;
    int secondiOggetto;
    
    // Setto i caratteri di terminazione
    nomeOggetto[30] = 0;
    descrizioneOggetto[50] = 0;
        
    // Lettura dei dati dal socket
    recv(socketClient, &idOggetto, 4, 0);
    recv(socketClient, nomeOggetto, 30, 0);
    recv(socketClient, descrizioneOggetto, 50, 0);
    recv(socketClient, &prezzoOggetto, 8, 0);
    recv(socketClient, &secondiOggetto, 4, 0);
    
    // Setto i dati ottenuti negli oggetti grafici
    idOggetti[indice] = idOggetto;
    caselleNomiOggetti[indice]->setText(QString::fromLatin1(nomeOggetto));
    caselleDescrizioniOggetti[indice]->setText(QString::fromLatin1(descrizioneOggetto));
    casellePrezziOggetti[indice]->setText(QString::number(prezzoOggetto));
    casellePrezziOfferti[indice]->setMinimum(prezzoOggetto+0.01);
    //timerOggetti[indice]->start(secondiOggetto*1000);
    cerr << "Secondi oggetto: " << secondiOggetto << "\n";
    timerOggetti[indice]->calcolaTempo(secondiOggetto);
    bottoniPuntata[indice]->setEnabled(true);
}

// Procedura per effettuare la puntata
void FinestraPrincipale::effettuaPuntata()
{
    /*
    cout << "Mandante:" << sender() << "\n";
    cout << "Bottone1:" << bottoniPuntata[0] << "\n";
    cout << "Bottone2:" << bottoniPuntata[1] << "\n";
    cout << "Bottone3:" << bottoniPuntata[2] << "\n";
    cout << "Bottone4:" << bottoniPuntata[3] << "\n";
    cout << "Bottone5:" << bottoniPuntata[4] << "\n";
    cout << "Bottone6:" << bottoniPuntata[5] << "\n";*/
    // Ottengo il bottone mandante per ottenere l'indice
    // dell'oggetto 
    int index;
    /*index = (sender() == bottoniPuntata[0] ? 0 : -1);
    index = (sender() == bottoniPuntata[1] ? 1 : -1);
    index = (sender() == bottoniPuntata[2] ? 2 : -1);
    index = (sender() == bottoniPuntata[3] ? 3 : -1);
    index = (sender() == bottoniPuntata[4] ? 4 : -1);
    index = (sender() == bottoniPuntata[5] ? 5 : -1);*/
    if ( sender() == bottoniPuntata[0] )
    {
        index = 0;
    }   
    else if ( sender() == bottoniPuntata[1] )
    {
        index = 1;
    }
    else if ( sender() == bottoniPuntata[2] )
    {
        index = 2;
    }        
    else if ( sender() == bottoniPuntata[3] )
    {
        index = 3;
    }
    else if ( sender() == bottoniPuntata[4] )
    {
        index = 4;
    }
    else if ( sender() == bottoniPuntata[5] )
    {
        index = 5;
    }
    cout << "Bottone:" << index << "\n";
    
    // Ottengo il prezzo inserito puntato
    double prezzo;
    // Servizio = 2 effettuare una puntata
    int servizio = 2;
    prezzo = casellePrezziOfferti[index]->value();
    
    // Invio la richiesta al server
    send(socketClient, &servizio, 4, 0);
    send(socketClient, &idUtente, 4, 0);
    send(socketClient, &idOggetti[index], 4, 0);
    send(socketClient, &prezzo, 8, 0);
    
    // La ricezione della risposta del server 
    // è effettuata in un' altra funzione
    
}

void FinestraPrincipale::aggiungiOggetto()
{
    // Istanza di un oggetto grafico per la finestra per aggiungere un oggetto
    FinestraNuovoOggetto* finestraNuovoOggetto = new FinestraNuovoOggetto(this, socketClient);
    finestraNuovoOggetto->show();
}

// Metodo statico, perché il pthread_create lo vuole così
void* FinestraPrincipale::riceviServizi(void* attributi)
{
    // Thread con accesso alle risorse grafiche e al socketClient per
    // poter leggere e modificare i dati
    Dati* dati = (Dati*)attributi;
    cerr << "sono qui\n";
    int servizio;
    // servizio = 3
    int indice = 0;
    // servizio = 5 e 3
    int pedice = 0;
    bool trovato; 
    int idOggetto;
    int numeroOggetti = 6;
    // Servizio = 3
    char nomeOggetto[31];
    char descrizioneOggetto[51];
    double prezzoOggetto;
    int secondiOggetto;
    
    // Iterazione per la ricezione di servizi da parte del client
    while ( recv(dati->socketClient, &servizio, 4, 0) > 0 )
    {
        cerr << "Letto servizio: " << servizio << "\n";
        switch (servizio)
        {
            // Servizio: 3 
            // Oggetti attualmente all'asta
            case 3:
                    // Setto i caratteri di terminazione
                    nomeOggetto[30] = 0;
                    descrizioneOggetto[50] = 0;
                        
                    // Lettura dei dati dal socket
                    recv(dati->socketClient, &idOggetto, 4, 0);
                    recv(dati->socketClient, nomeOggetto, 30, 0);
                    recv(dati->socketClient, descrizioneOggetto, 50, 0);
                    recv(dati->socketClient, &prezzoOggetto, 8, 0);
                    recv(dati->socketClient, &secondiOggetto, 4, 0);
                    
                    // Setto i dati ottenuti negli oggetti grafici
                    dati->idOggetti[indice] = idOggetto;
                    dati->caselleNomiOggetti[indice]->setText(QString::fromLatin1(nomeOggetto));
                    dati->caselleDescrizioniOggetti[indice]->setText(QString::fromLatin1(descrizioneOggetto));
                    dati->casellePrezziOggetti[indice]->setText(QString::number(prezzoOggetto));
                    dati->casellePrezziOfferti[indice]->setMinimum(prezzoOggetto+0.01);
		    cerr << "Secondi oggetto: " << secondiOggetto << "\n";
                    dati->timerOggetti[indice]->calcolaTempo(secondiOggetto);
                    dati->bottoniPuntata[indice]->setEnabled(true);
                    
                    indice++;
            break;
            
            // Servizio: 5
            // Aggiornamento delle puntate (idOggetto, prezzo)
            case 5:
                    cerr << "Ricevuto aggiornamento!\n";
                    // Lettura dei dati dal socket
                    recv(dati->socketClient, &idOggetto, 4, 0);
                    recv(dati->socketClient, &prezzoOggetto, 8, 0);
                    
                    // Inizializzazione contatore per navigare nei vettori
                    pedice = 0;
                    trovato = false;
                    while ( pedice < numeroOggetti && !trovato)
                    {
                        // Verifica che l'oggetto a cui è stato richiesto l'aggiornamento
                        // sia quello attualmente navigato
                        if ( dati->idOggetti[pedice] == idOggetto )
                        {
                            // Setto il flag
                            trovato = true;
                            // Aggiorno l'oggetto in questione
                            dati->casellePrezziOggetti[pedice]->setText(QString::number(prezzoOggetto));
                            dati->casellePrezziOfferti[pedice]->setMinimum(prezzoOggetto+0.01);
                        }
                        // Passo all'elemento successivo
                        pedice = pedice + 1;
                    }
            break;
        }
        
        // Verifica che l'indice non sia andato fuori vettore
        if ( indice > numeroOggetti )
        {
            // Lo riporto all'inizio del vettore
            indice = 0;
        }
    }    
    
    // Uscito dal ciclo è stata interrotta la connessione con il server
    // distruzione di tutto
    pthread_exit(0);
}

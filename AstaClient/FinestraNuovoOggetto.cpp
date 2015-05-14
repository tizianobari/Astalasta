// Autore: Bari Tiziano e Pacicco Manuel
// Data: 35/03/15
// Specifiche: Implementazione classe FinestraNuovoOggetto

#include "FinestraNuovoOggetto.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;

FinestraNuovoOggetto::FinestraNuovoOggetto(QWidget* padre, int socketClient) : QDialog(0)
{
    // Imposto il riferimento al socket
    this->socketClient = socketClient;
    
    // Imposto il padre della finestra
    //setParent(padre);
    
    resize(400,300);
    
    setModal(true);
    
    // Istanza oggetti grafici
    etichettaNome = new QLabel("Nome", this);
    etichettaDescrizione = new QLabel("Cognome", this);
    etichettaPrezzo = new QLabel("Prezzo", this);
    casellaNome = new QLineEdit(this);
    casellaDescrizione = new QLineEdit(this);
    casellaPrezzo = new QDoubleSpinBox(this);
    bottoneInvia = new QPushButton("&Invia!", this);
    
    // Posizionamento oggetti grafici
    etichettaNome->setGeometry(20,30,81,21);
    etichettaDescrizione->setGeometry(20,80,81,21);
    etichettaPrezzo->setGeometry(20,130,81,21);
    casellaNome->setGeometry(140,30,191,31);
    casellaDescrizione->setGeometry(140,80,191,31);
    casellaPrezzo->setGeometry(140,130,191,27);
    bottoneInvia->setGeometry(130,200,98,27);
    
    // Setto lo scalo minimo per il prezzo
    casellaPrezzo->setSingleStep(0.01);
    
    // Intercettazione degli eventi/segnali
    connect(bottoneInvia, SIGNAL(clicked()), this, SLOT(inviaNuovoOggetto()));
}

void FinestraNuovoOggetto::inviaNuovoOggetto()
{
    // Invia sul socket il servizio = 8 
    // e il nome, descrizione, prezzo del nuovo oggetto
    // prendendoli dai textedit
    int servizio = 8;
    string nome = casellaNome->text().toStdString();
    string descrizione = casellaDescrizione->text().toStdString();
    double prezzo = casellaPrezzo->value();
    
    // Invio sul socket
    send(socketClient, &servizio, 4, 0);
    send(socketClient, nome.c_str(), 30, 0);
    send(socketClient, descrizione.c_str(), 50, 0);
    send(socketClient, &prezzo, 8, 0);
}

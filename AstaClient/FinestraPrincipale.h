// Autore: Bari Tiziano e Pacicco Manuel
// Classe: 5B INF
// Data: 24/03/2015
// Specifiche: Finestra principale di Astalasta

#ifndef FINESTRAPRINCIPALE_H
#define FINESTRAPRINCIPALE_H

#include <unistd.h>
#include <iostream>
#include <QtGui>
#include "TestoTimer.h"
using namespace std;

// Dichiarazione classe FinestraLogin che eredita QDialog 
class FinestraPrincipale : public QMainWindow
{
     Q_OBJECT
     
public:
     // Funzione costruttore
     FinestraPrincipale(int, int);
     // Funzione distruttore
     //~FinestraPrincipale();
     // Dichiarazione attributi
     int socketClient;
     int idUtente;
     int offset;
     int numeroOggetti;
     int servizio;
     // Inizializzazione indice di posizione
     int indice;
     // Dichiarazione variabile che conterrà l'id del thread per la gestione
     // delle risposte del server
     pthread_t compitoRicezione;
     
     // Dichiarazione oggetti grafici
     QMenu *menuFile;
     
     // Ci sono 7 oggetti che corrispondono agli oggetti messi all'asta visualizzabili
     int idOggetti[6];
     QLineEdit* caselleNomiOggetti[6];
     QLineEdit* caselleDescrizioniOggetti[6];
     QLineEdit* casellePrezziOggetti[6];
     QDoubleSpinBox* casellePrezziOfferti[6];
     //QTimer* timerOggetti[6];
     TestoTimer* timerOggetti[6];
     QPushButton* bottoniPuntata[6];
     
public slots:
     // Dichiarazione funzione da effettuare quando avviene un evento in particolare
     void aggiungiOggetto();
     void effettuaPuntata();
     //void aggiornaOggetto();
     void richiediOggetti();
     void ottieniOggetto();
     static void* riceviServizi(void*);
};

#endif

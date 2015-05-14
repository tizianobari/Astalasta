// Autore: Bari Tiziano e Pacicco Manuel
// Classe: 5B INF
// Data: 25/03/2015
// Specifiche: Classe di appoggio per passare i dati al thread per 
//             la gestione degli aggiornamenti

#ifndef DATI_H
#define DATI_H

#include <iostream>
#include <QtGui>
#include "TestoTimer.h"
using namespace std;

class Dati
{
    public:
        int* idOggetti;
        QLineEdit** caselleNomiOggetti;
        QLineEdit** caselleDescrizioniOggetti;
        QLineEdit** casellePrezziOggetti;
        QDoubleSpinBox** casellePrezziOfferti;
        TestoTimer** timerOggetti;
        QPushButton** bottoniPuntata;
        int socketClient;
};

#endif

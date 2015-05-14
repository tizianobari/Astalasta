// Autore: Bari Tiziano e Pacicco Manuel
// Classe: 5B INF
// Data: 25/03/2015
// Specifiche: Timer per la scadenza dell'asta

#ifndef TESTOTIMER_H
#define TESTOTIMER_H

#include <iostream>
#include <QtGui>
using namespace std;


// Eredita da QLineEdit
class TestoTimer : public QLineEdit
{
    Q_OBJECT
    // Dichiarazione attributi
    public:
        int ore;
        int minuti;
        int secondi;
	int secondiTotali;
        QTimer* timer;
        QTime* tempo;
        // Dichiarazione costruttore
        TestoTimer(QWidget*);
        // Dichiarazione destruttore
        //~TestoTimer();
        
    public slots:
        void visualizzaTempo();
        void calcolaTempo(int);
};

#endif

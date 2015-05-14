// Autore: Bari Tiziano e Pacicco Manuel
// Data: 35/03/15
// Specifiche: Definizione classe FinestraNuovoOggetto

#ifndef FINESTRANUOVOOGGETTO_H
#define FINESTRANUOVOOGGETTO_H

#include <QtGui>
#include <iostream>
using namespace std;

class FinestraNuovoOggetto : public QDialog
{
    Q_OBJECT
    
    public:
        // Costruttore
        FinestraNuovoOggetto(QWidget*, int);
        int socketClient;
        QLabel* etichettaNome;
        QLabel* etichettaDescrizione;
        QLabel* etichettaPrezzo;
        QLineEdit* casellaNome;
        QLineEdit* casellaDescrizione;
        QDoubleSpinBox* casellaPrezzo;
        QPushButton* bottoneInvia;
    public slots:
        void inviaNuovoOggetto();
};

#endif

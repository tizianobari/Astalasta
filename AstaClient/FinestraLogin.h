// Autore: Bari Tiziano e Pacicco Manuel
// Classe: 5B INF
// Data: 19/03/2015
// Specifiche:

#ifndef FINESTRALOGIN_H
#define FINESTRALOGIN_H

#include <unistd.h>
#include <iostream>
#include <QtGui>
using namespace std;

// Dichiarazione classe FinestraLogin che eredita QDialog 
class FinestraLogin : public QDialog
{
     Q_OBJECT
     
public:
     // Funzione costruttore
     FinestraLogin(int);
     // Funzione distruttore
     ~FinestraLogin();
     // Dichiarazione attributi
     int socketClient;
     int idUtente;
     
     // Dichiarazione oggetti grafici
     QLabel* etichettaUsername;
     QLabel* etichettaPassword;
     QLineEdit* casellaUsername;
     QLineEdit* casellaPassword;
     QPushButton* bottoneLogin;
     
private slots:
     // Dichiarazione funzione da effettuare quando viene cliccato il bottone
     void effettuaLogin();
};

#endif

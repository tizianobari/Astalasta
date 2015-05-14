// Autore: Bari Tiziano e Pacicco Manuel
// Data: 25/03/2015
// Classe: 5B INF
// Specifiche: Implementazione oggetto TestoTimer

#include "TestoTimer.h"
#include <iostream>
//#include <QtGui>

TestoTimer::TestoTimer(QWidget* padre) : QLineEdit(0)
{
    //cerr << "Costruttore TestoTimer\n";
    ore = 0;
    minuti = 0;
    secondi =0;
    
    timer = new QTimer(0);
    tempo = new QTime(ore, minuti, secondi);
    
    setParent (padre);
    
    connect(timer, SIGNAL(timeout()), this, SLOT(visualizzaTempo()));
    timer->start(1000);
}

void TestoTimer::visualizzaTempo()
{
    //tempo->setHMS(tempo->addSecs(-1).hour(), tempo->addSecs(-1).minute(), tempo->addSecs(-1).second());
    // Passa un secondo
    this->secondiTotali--;
    // Formo la stringa con ore minuti e secondi
    char orario[20];
    sprintf(orario, "%i:%i:%i", secondiTotali/3600, (secondiTotali/60)%60, secondiTotali%60);
    cout << orario << "\n";
    this->setText(orario);
    //cout << (tempo->toString()).toStdString() <<"\n";
}

void TestoTimer::calcolaTempo(int sec)
{
    cout << "calcolaTempo: " << sec << "\n";
    this->secondiTotali = sec;
    this->ore = sec/3600;
    this->minuti = (sec/60) % 60;
    this->secondi = sec % 60;
    cout << "ore: " << this->ore << "\n";
    cout << "minuti: " << this->minuti<< "\n";
    cout << "secondi: " << this->secondi << "\n";
    //tempo->setHMS(ore, minuti, secondi);
}

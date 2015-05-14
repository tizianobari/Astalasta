// Autore: Bari Tiziano
// Data: 25/03/2015
// Classe: 5B INF
// Specifiche: Finestra per effettuare una puntata

#ifndef FINESTRAPUNTATA_H
#define FINESTRAPUNTATA_H

class FinestraPuntata : public QDialog
{
    Q_OBJECT
    
    public:
        // Funzione costruttore
        // Parametri: socketClient, idOggetto, prezzoAttuale
        FinestraPuntata(int, int, double);
        
        // Dichiarazione attributi
        int socketClient;
        int idOggetto;
        double prezzoOggetto;
        
        // Dichiarazione oggetti grafici
        QLineEdit* casellaPrezzo;
        QPushButton* bottonePuntata;
        
    private slots:
        //effettuaPuntata();
};

#endif

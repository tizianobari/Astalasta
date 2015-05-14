//Autore: Bari Tiziano e Pacicco Manuel
// Classe: 5B INF
// Data: 19/03/2015
// Specifiche: Implementazione classe FinestraLogin

#include <unistd.h>
//#include <openssl/md5.h>
#include <QByteArray>
#include <QCryptographicHash>
#include <sys/socket.h>
#include <sys/types.h>
#include "FinestraLogin.h"
#include "FinestraPrincipale.h"

// Implementazione costruttore
FinestraLogin::FinestraLogin(int socketClient)
{
     // Imposto il riferimento al socket 
     this->socketClient = socketClient;
     
     // Setto il nome della finestra
     setWindowTitle("Login Astalasta");
     
     // Setto la finestra come modale
     //setModal(true);
     
     // Dimensiono la finestra
     resize(400, 300);
     
     // Istanza degli oggetti grafici
     etichettaUsername = new QLabel("Username:", this);
     etichettaPassword = new QLabel("Password:", this);
     casellaUsername = new QLineEdit(this);
     casellaPassword = new QLineEdit(this);
     bottoneLogin = new QPushButton("&Login", this);
     
     // Setto la posizione e la dimensione degli oggetti grafici
     // Posizione e dimensione delle caselle
     // Parametri: coordinate x, y e larghezza, altezza
     etichettaUsername->setGeometry(30, 50, 50, 50);
     etichettaPassword->setGeometry(30, 125, 50, 50);
     casellaUsername->setGeometry(90, 50, 220, 50);
     casellaPassword->setGeometry(90, 125, 220, 50);
     bottoneLogin->setGeometry(55, 200, 60, 50);
     
     // Setto che la casella della password mostri gli asterischi invece che
     // i caratteri digitati
     casellaPassword->setEchoMode(QLineEdit::Password);
     
     // Imposto l'intercettatore degli eventi
     // Parametri: oggetto a cui allegare l'evento
     //            evento da intercettare
     //            chi lo intercetta
     //            funzione da eseguire quando intercettato
     connect(bottoneLogin, SIGNAL(clicked()), this, SLOT(effettuaLogin()));
}

// Implementazione distruttore
FinestraLogin::~FinestraLogin()
{
    // Chiudo la connessione al socket (con la funzione close() della libreria unistd.h)
    ::close(this->socketClient);
}

// Implementazione della funzione eseguita quando viene cliccato il bottone
void FinestraLogin::effettuaLogin()
{
     // Assegno alle variabili i dati scritti nella casella di testo
     // codificate con algoritmo MD5
     //string username = MD5((unsigned char*)((casellaUsername->text()).toStdString()), casellaUsername->text().length(), NULL);
     //unsigned char* username = (unsigned char*)((casellaUsername->text).toStdString());
     //string username = casellaUsername->text().toStdString();
     // Ottengo l'username e la password sotto forma di byte
     QByteArray username(casellaUsername->text().toStdString().c_str());
     QByteArray password(casellaPassword->text().toStdString().c_str());
     // Codifico i dati secondo l'MD5 usando il metodo statico hash() dell'oggetto QCryptographicHash
     QByteArray usernameCifrato = QCryptographicHash::hash(username,QCryptographicHash::Md5);
     QByteArray passwordCifrata = QCryptographicHash::hash(password,QCryptographicHash::Md5);
     /*
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText(usernameCifrato.toHex().data());
        msgBox->exec();
     */
     // Invio al server delle credenziali per effettuare l'accesso
     // Servizio = 1, richiesta di login 
     // username 32 cifre esadecimali e password 32 cifre esadecimali
     int servizio = 1;
     send(this->socketClient, &servizio , 4, 0);
     send(this->socketClient, usernameCifrato.toHex().data(), 32, 0);
     send(this->socketClient, passwordCifrata.toHex().data(), 32, 0);
     // Aspetto la risposta
     recv(this->socketClient, &(this->idUtente), 4, 0);
     if ( this->idUtente > 0 )
     {
        // Istanza della nuova finestra del programma
        FinestraPrincipale* finestraPrincipale = new FinestraPrincipale(socketClient, idUtente);
        //finestraPrincipale->show();
        // Chiamata del metodo per ottenere gli oggetti
        finestraPrincipale->richiediOggetti();
        // Chiamata del metodo per mettere in comunicazione il client con il server
     }
     else
     {
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText("Login non corretto");
        msgBox->exec();
     }
     cout << this->idUtente << "\n";
}

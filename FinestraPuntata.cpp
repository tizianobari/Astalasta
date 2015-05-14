// Autore: Bari Tiziano
// Classe: 5B INF
// Specifiche: Implementazione classe FinestraPuntata

// Implementazione costruttore, chiamata del costruttore della superclasse QDialog
FinestraPuntata::FinestraPuntata(int socketClient, int idOggetto, double prezzoOggetto) : QDialog(0)
{
    // Imposto gli attributi
    this->socketClient = socketClient;
    this->idOggetto = idOggetto;
    this->prezzoOggetto = prezzoOggetto;
    
    // Istanza degli oggetti grafici
    casellaPrezzo = new QLineEdit(this);
    bottonePuntata = new QPushButton("&Punta", this);
    
}

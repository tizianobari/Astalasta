CREATE TABLE Articoli (
    id INT NOT NULL AUTO_INCREMENT,
    nome CHAR(30) NOT NULL, 
    descrizione CHAR(50),
    prezzoIniziale DECIMAL(9,2) NOT NULL,
    prezzoAttuale DECIMAL(9,2) NOT NULL,
    dataInizio DATETIME,
    dataFine DATETIME,
    rifFoto INT,
    rifCliente INT,
    PRIMARY KEY (id)
) ENGINE=InnoDB; 
    
    
CREATE TABLE Clienti (
    id INT NOT NULL AUTO_INCREMENT,
    nome CHAR(30) NOT NULL, 
    cognome CHAR(30) NOT NULL,
    dataNascita DATE,
    username CHAR(32) NOT NULL COMMENT 'Cifrato in MD5',
    password CHAR(32) NOT NULL COMMENT 'Cifrato in MD5',
    UNIQUE (username),
    PRIMARY KEY (id)
) ENGINE=InnoDB;
    
    

CREATE TABLE Puntate (
    id INT NOT NULL AUTO_INCREMENT,
    prezzo DECIMAL(9,2) NOT NULL,
    data DATETIME NOT NULL,
    rifCliente INT NOT NULL,
    rifArticolo INT NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB;    
    

CREATE TABLE Foto (
    id INT NOT NULL AUTO_INCREMENT,
    fotografia MEDIUMBLOB,
    UNIQUE (fotografia),
    PRIMARY KEY (id)
) ENGINE=InnoDB;    
 
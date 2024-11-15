//*********************************************************************************************
//* Programme : ControleCamera.cpp                                              Date : 15/11/2024
//*--------------------------------------------------------------------------------------------
//* Dernière mise à jour : 15/11/2024
//*
//* Programmeurs : Lemaire Kévin                                               Classe : BTSCIEL2
//*                Tellier Néo
//*--------------------------------------------------------------------------------------------
//* But : Gérer les commandes de la caméra de surveillance via un port série, y compris 
//*       l'ouverture du port, l'envoi des commandes, et le traitement des confirmations.
//* Programmes associés : CameraDeSurveillance.cpp
//*********************************************************************************************

#include "ControleCamera.h"
#include <QThread>
#include <QDebug>

ControleCamera::ControleCamera() {}

ControleCamera::~ControleCamera()
{
    if (port)
    {
        port->close();
        delete port;
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'ouvrir le port série pour communiquer avec la caméra
//* Paramètres :
//*  - QSerialPort* newPort : le port série à ouvrir pour la communication avec la caméra
//*
//* Valeur de retour : bool, vrai si le port est ouvert avec succès, sinon faux.
//---------------------------------------------------------------------------------------------
bool ControleCamera::openPort(QSerialPort* newPort)
{
    if (newPort)
    {
        // Fermer et supprimer le port existant si nécessaire
        if (port && port->isOpen())
        {
            port->close();
        }

        // Affecter le nouveau port
        port = newPort;

        // Configuration du port série
        port->setBaudRate(QSerialPort::Baud9600);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        port->setFlowControl(QSerialPort::NoFlowControl);

        // Essayer d'ouvrir le port
        if (port->open(QIODevice::ReadWrite))
        {
            isportOpen = true;
            return true;
        }
        else
        {
            isportOpen = false;
            return false;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'initialiser la caméra en envoyant une commande spécifique
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::camInitialisation()
{
    if (checkPort())
    {
        writeToPort("81 01 06 04 FF");
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'allumer la caméra en envoyant une commande spécifique pour l'alimentation
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::powerON()
{
    if (checkPort())
    {
        writeToPort("81 01 04 00 02 FF");
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de déplacer la caméra vers le haut
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveUp()
{
    writeToPort("81 01 06 01 18 14 03 01 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de déplacer la caméra vers le bas
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveDown()
{
    writeToPort("81 01 06 01 18 14 03 02 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de déplacer la caméra vers la gauche
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveLeft()
{
    writeToPort("81 01 06 01 18 14 01 03 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de déplacer la caméra vers la droite
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveRight()
{
    writeToPort("81 01 06 01 18 14 02 03 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'activer un mode automatique qui balaye la salle trois fois de suite
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::autoMode()
{
    writeToPort("81 01 06 03 18 14 09 09 09 09 00 00 00 00 FF");
    writeToPort("81 01 06 03 18 14 05 05 05 05 00 00 00 00 FF");
}

//* Fonction permettant d'ajuster le zoom de la caméra en fonction de la valeur du curseur
//* Paramètres :
//*  - int zoomValue : la valeur du zoom de 0 (large) à 1023 (rapproché)
//* 
//* Valeur de retour : aucun
void ControleCamera::adjustZoom(int zoomValue)
{
    if (checkPort())
    {
        // La valeur du zoom varie entre 0 (large) et 1023 (rapproché)
        // Conversion de zoomValue en hexadécimal pour envoyer la commande correspondante.
        QString zoomCommand = QString("81 01 04 47 0%1 0%2 0%3 0%4 FF")
            .arg(zoomValue & 0xFF, 2, 16, QLatin1Char('0'))  // Premier octet
            .arg((zoomValue >> 8) & 0xFF, 2, 16, QLatin1Char('0'))  // Deuxième octet
            .arg("00")  // Valeur fixe
            .arg("00");  // Valeur fixe
        writeToPort(zoomCommand);  // Envoi de la commande à la caméra
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de vérifier si le port série est ouvert pour la communication
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : bool, vrai si le port est ouvert, sinon faux.
//---------------------------------------------------------------------------------------------
bool ControleCamera::checkPort()
{
    if (port && port->isOpen())
    {
        return true;
    }

    qDebug() << "Erreur: Port série non ouvert.";
    return false;
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'écrire une commande sur le port série pour la caméra
//* Paramètres :
//*  - QString command : la commande à envoyer à la caméra sous forme de chaîne hexadécimale
//*
//* Valeur de retour : bool, vrai si la commande a été envoyée avec succès, sinon faux.
//---------------------------------------------------------------------------------------------
bool ControleCamera::writeToPort(const QString& command)
{
    if (!checkPort())
    {
        return false;
    }

    QByteArray data = QByteArray::fromHex(command.toLatin1());
    qint64 bytesWritten = port->write(data);

    if (bytesWritten == -1)
    {
        qDebug() << "Erreur lors de l'envoi de la commande: " << port->errorString();
        return false;
    }

    // Attente de confirmation (port peut être en mode asynchrone)
    this->waitingForConfirmation = true;

    return true;
}

//---------------------------------------------------------------------------------------------
//* Fonction appelée lorsque des données sont reçues depuis le port série, permettant de traiter la réponse
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::onSerialPortReadyRead()
{
    QByteArray receivedData = port->readAll();

    // Traitement des données reçues
    qDebug() << "Données reçues: " << receivedData;

    // Si la confirmation est reçue (par exemple, un 'Q' ou 'R' dans la réponse)
    if (receivedData.contains('Q') || receivedData.contains('R'))
    {
        waitingForConfirmation = false;
        qDebug() << "Réponse reçue, confirmation!";
    }
}
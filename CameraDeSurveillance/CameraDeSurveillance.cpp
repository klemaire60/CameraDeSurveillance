//*********************************************************************************************
//* Programme : CameraDeSurveillance.cpp                                      Date : 15/11/2024
//*--------------------------------------------------------------------------------------------
//* Dernière mise à jour : 15/11/2024
//*
//* Programmeurs : Lemaire Kévin                                              Classe : BTSCIEL2
//*                Tellier Néo
//*--------------------------------------------------------------------------------------------
//* But : Interface de contrôle d'une caméra de surveillance avec gestion du port série, 
//*       initialisation, déplacements, mode automatique, et gestion d'alimentation.
//* Programmes associés : ControleCamera.cpp
//*********************************************************************************************

#include "CameraDeSurveillance.h"
#include "ControleCamera.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QDebug>
#include <QString>

//---------------------------------------------------------------------------------------------
//* Constructeur de la classe CameraDeSurveillance, initialise l'interface et la connexion des boutons
//* Paramètres :
//*  - QWidget* parent : le widget parent (généralement une fenêtre principale)
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
CameraDeSurveillance::CameraDeSurveillance(QWidget* parent)
    : QMainWindow(parent), controleCamera(nullptr), port(nullptr)
{
    ui.setupUi(this);
    controleCamera = new ControleCamera();  // Création de l'objet ControleCamera
    
   
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : availablePorts)
    {
        ui.portChoiceComboBox->addItem(info.portName(), QVariant(info.portName()));
    }

    setupConnections();  // Initialisation des connexions entre boutons et slots
}

//---------------------------------------------------------------------------------------------
//* Destructeur de la classe CameraDeSurveillance, libère les ressources allouées
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
CameraDeSurveillance::~CameraDeSurveillance()
{
    if (controleCamera)
    {
        delete controleCamera;
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction pour établir les connexions entre les boutons de l'interface et les slots correspondants
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::setupConnections()
{
    connect(ui.OpenPortButton, &QPushButton::clicked, this, &CameraDeSurveillance::openPort);
    connect(ui.initbutton, &QPushButton::clicked, this, &CameraDeSurveillance::camInitialisation);
    connect(ui.powerbutton, &QPushButton::clicked, this, &CameraDeSurveillance::powerOn);
    connect(ui.moveUpButton, &QPushButton::clicked, this, &CameraDeSurveillance::moveUp);
    connect(ui.moveDownButon, &QPushButton::clicked, this, &CameraDeSurveillance::moveDown);
    connect(ui.moveLeftButton, &QPushButton::clicked, this, &CameraDeSurveillance::moveLeft);
    connect(ui.moveRightButon, &QPushButton::clicked, this, &CameraDeSurveillance::moveRight);
    connect(ui.autobutton, &QPushButton::clicked, this, &CameraDeSurveillance::autoMode); 
    connect(ui.zoomVerticalSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustZoom(int)));
}

//---------------------------------------------------------------------------------------------
//* Fonction pour ouvrir un port série pour la communication avec la caméra
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::openPort()
{
    if (ui.portChoiceComboBox->currentIndex() < 0)
    {
        switch (ui.ChoseLanguage->currentIndex()) {
        case 0:
            ui.portStatusLabel->setText("Veuillez selectionner un port.");
            break;
        case 1:
            ui.portStatusLabel->setText("Please select a port.");
            break;
        case 2:
            ui.portStatusLabel->setText("Bitte waehlen Sie einen Port.");
            break;
        case 3:
            ui.portStatusLabel->setText("Yarja ikhtiyar manfath.");
            break;
        case 4:
            ui.portStatusLabel->setText("Goulit ul port.");
            break;
        case 5:
            ui.portStatusLabel->setText("Pora otkryt' port.");
            break;
        case 6:
            ui.portStatusLabel->setText("Qing xuanze yi ge duankou.");
            break;
        case 7:
            ui.portStatusLabel->setText("Por favor seleccione un puerto.");
            break;
        case 8:
            ui.portStatusLabel->setText("Molimo odaberite port.");
            break;
        case 9:
            ui.portStatusLabel->setText("Portum aperire selige.");
            break;
        case 10:
            ui.portStatusLabel->setText("Anoigma thyras, parakalw epilogh.");
            break;
        }

        return;
    
    }

    // Créez un nouveau port série et passez-le à ControleCamera
    port = new QSerialPort(ui.portChoiceComboBox->currentText());

    // Vérifiez si le port peut être ouvert via ControleCamera
    if (controleCamera->openPort(port))  // Appel de la méthode openPort() dans ControleCamera
    {
        switch (ui.ChoseLanguage->currentIndex()) {
        case 0:
            ui.portStatusLabel->setText("Statut port: Ouvert");
            break;
        case 1:
            ui.portStatusLabel->setText("Port status: Open");
            break;
        case 2:
            ui.portStatusLabel->setText("Port status: Offen");
            break;
        case 3:
            ui.portStatusLabel->setText("Statut al manfath: Maftuh");
            break;
        case 4:
            ui.portStatusLabel->setText("Status port: Ouvert");
            break;
        case 5:
            ui.portStatusLabel->setText("Status port: Otkryt");
            break;
        case 6:
            ui.portStatusLabel->setText("Zhuangtai duankou: Daka");
            break;
        case 7:
            ui.portStatusLabel->setText("Estado del puerto: Abierto");
            break;
        case 8:
            ui.portStatusLabel->setText("Status porta: Otvoren");
            break;
        case 9:
            ui.portStatusLabel->setText("Status portus: Apertus");
            break;
        case 10:
            ui.portStatusLabel->setText("Katalogi thyras: Anoigmeni");
            break;
        }

    }
    else
    {
        switch (ui.ChoseLanguage->currentIndex()) {
        case 0:
            ui.portStatusLabel->setText("Erreur: " + port->errorString());
            break;
        case 1:
            ui.portStatusLabel->setText("Error: " + port->errorString());
            break;
        case 2:
            ui.portStatusLabel->setText("Fehler: " + port->errorString());
            break;
        case 3:
            ui.portStatusLabel->setText("Khata: " + port->errorString());
            break;
        case 4:
            ui.portStatusLabel->setText("Erreur: " + port->errorString());
            break;
        case 5:
            ui.portStatusLabel->setText("Oshibka: " + port->errorString());
            break;
        case 6:
            ui.portStatusLabel->setText("Cuowu: " + port->errorString());
            break;
        case 7:
            ui.portStatusLabel->setText("Error: " + port->errorString());
            break;
        case 8:
            ui.portStatusLabel->setText("Greska: " + port->errorString());
            break;
        case 9:
            ui.portStatusLabel->setText("Error: " + port->errorString());
            break;
        case 10:
            ui.portStatusLabel->setText("Lathos: " + port->errorString());
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction pour initialiser la caméra en envoyant une commande spécifique
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::camInitialisation()
{
    controleCamera->camInitialisation();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour allumer la caméra en envoyant une commande spécifique pour l'alimentation
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::powerOn()
{
    controleCamera->powerON();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers le haut
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveUp()
{
    controleCamera->MoveUp();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers le bas
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveDown()
{
    controleCamera->MoveDown();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers la gauche
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveLeft()
{
    controleCamera->MoveLeft();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers la droite
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveRight()
{
    controleCamera->MoveRight();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour activer le mode automatique de la caméra
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::autoMode()
{
    controleCamera->autoMode();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour changer la langue de l'interface graphique
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------

void CameraDeSurveillance::ChangeLanguage()
{
    
    switch (ui.ChoseLanguage->currentIndex())
    {
    case 0: 
        ui.OpenPortButton->setText("Ouvrir le port");
        ui.autobutton->setText("Mode automatique");
        ui.initbutton->setText("Initialiser");
        ui.label->setText("Zoom");
        ui.label_4->setText("Changer la langue");
        ui.moveDownButon->setText("Descendre");
        ui.moveLeftButton->setText("Gauche");
        ui.moveRightButon->setText("Droite");
        ui.moveUpButton->setText("Monter");
        ui.powerbutton->setText("Allumer");
        ui.portStatusLabel->setText("");
        break;
          
    case 1:
        ui.OpenPortButton->setText("Open Port");
        ui.autobutton->setText("Automatic mode");
        ui.initbutton->setText("Initialise");
        ui.label->setText("Zoom");
        ui.label_4->setText("Changer language");
        ui.moveDownButon->setText("Down");
        ui.moveLeftButton->setText("Left");
        ui.moveRightButon->setText("Right");
        ui.moveUpButton->setText("Up");
        ui.powerbutton->setText("Turn On");
        ui.portStatusLabel->setText("");
        break;

    case 2:
        ui.OpenPortButton->setText("Offener Port");
        ui.autobutton->setText("Automatikmodus");
        ui.initbutton->setText("Initialisieren");
        ui.label->setText("Zoom");
        ui.label_4->setText("Sprache andern");
        ui.moveDownButon->setText("Runterkommen");
        ui.moveLeftButton->setText("Links");
        ui.moveRightButon->setText("Rechts");
        ui.moveUpButton->setText("Nach oben");
        ui.powerbutton->setText("Zum Leuchten");
        ui.portStatusLabel->setText("");
        break;

    case 3:
        ui.OpenPortButton->setText("Fath Al manfaz");
        ui.autobutton->setText("telqaa'i");
        ui.initbutton->setText("Tahyi'aa");
        ui.label->setText("Takbir");
        ui.label_4->setText("taghir al logha");
        ui.moveDownButon->setText("nuzul");
        ui.moveLeftButton->setText("yasar");
        ui.moveRightButon->setText("yamin");
        ui.moveUpButton->setText("sooud");
        ui.powerbutton->setText("tashghil");
        ui.portStatusLabel->setText("");
        break;
        
    case 4: 
        ui.OpenPortButton->setText("Digerin ar porzh");
        ui.autobutton->setText("Mod emgefre");
        ui.initbutton->setText("Kregin");
        ui.label->setText("Zoom");
        ui.label_4->setText("Chench ar yezh");
        ui.moveDownButon->setText("Diskenn");
        ui.moveLeftButton->setText("Tu kleiz");
        ui.moveRightButon->setText("Tu dehou");
        ui.moveUpButton->setText("Pignat");
        ui.powerbutton->setText("Prenan");
        ui.portStatusLabel->setText("");
        break;

    case 5:
        ui.OpenPortButton->setText("Otvori port");
        ui.autobutton->setText("Avtomaticheskij rezhim");
        ui.initbutton->setText("Initsializirovat'");
        ui.label->setText("Zoom");
        ui.label_4->setText("Smenit' yazyik");
        ui.moveDownButon->setText("Vniz");
        ui.moveLeftButton->setText("Vlevo");
        ui.moveRightButon->setText("Vpravo");
        ui.moveUpButton->setText("Vverkh");
        ui.powerbutton->setText("Vklyuchit'");
        ui.portStatusLabel->setText("");
        break;

    case 6:
        ui.OpenPortButton->setText("da kai duan kou");
        ui.autobutton->setText("zi dong mo shi");
        ui.initbutton->setText("chu xin hua");
        ui.label->setText("zuo fang");
        ui.label_4->setText("geng huan yu yan");
        ui.moveDownButon->setText("xiang xia");
        ui.moveLeftButton->setText("xiang zuo");
        ui.moveRightButon->setText("xiang you");
        ui.moveUpButton->setText("xiang shang");
        ui.powerbutton->setText("da kai dian yuan");
        ui.portStatusLabel->setText("");
        break;
    
    case 7:
        ui.OpenPortButton->setText("Abrir puerto");
        ui.autobutton->setText("Modo automatico");
        ui.initbutton->setText("Inicializar");
        ui.label->setText("Zoom");
        ui.label_4->setText("Cambiar idioma");
        ui.moveDownButon->setText("Bajar");
        ui.moveLeftButton->setText("Izquierda");
        ui.moveRightButon->setText("Derecha");
        ui.moveUpButton->setText("Subir");
        ui.powerbutton->setText("Encender");
        ui.portStatusLabel->setText("");
        break;
    
    case 8:
        ui.OpenPortButton->setText("Otvoriti port");
        ui.autobutton->setText("Automatski rezim");
        ui.initbutton->setText("Inicijalizuj");
        ui.label->setText("Zum");
        ui.label_4->setText("Promeni jezik");
        ui.moveDownButon->setText("Nadalje");
        ui.moveLeftButton->setText("Levo");
        ui.moveRightButon->setText("Desno");
        ui.moveUpButton->setText("Gore");
        ui.powerbutton->setText("Ukljuci");
        ui.portStatusLabel->setText("");
        break;
    
    case 9:
        ui.OpenPortButton->setText("Portum aperire");
        ui.autobutton->setText("Modus automaticus");
        ui.initbutton->setText("Iniciari");
        ui.label->setText("Zoom");
        ui.label_4->setText("Mutare linguam");
        ui.moveDownButon->setText("Descendere");
        ui.moveLeftButton->setText("Sinistrorsum");
        ui.moveRightButon->setText("Dextrorsum");
        ui.moveUpButton->setText("Ascendere");
        ui.powerbutton->setText("Accendere");
        ui.portStatusLabel->setText("");
        break;
    
    case 10:
        ui.OpenPortButton->setText("Anoigma thyras");
        ui.autobutton->setText("Aftomati leitourgia");
        ui.initbutton->setText("Arxikopoiisi");
        ui.label->setText("Zoom");
        ui.label_4->setText("Allagi glossas");
        ui.moveDownButon->setText("Kato");
        ui.moveLeftButton->setText("Aristera");
        ui.moveRightButon->setText("Dexia");
        ui.moveUpButton->setText("Epanw");
        ui.powerbutton->setText("Anoigma");
        ui.portStatusLabel->setText("");
        break;
    }
    
}

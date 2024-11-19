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
        ui.portStatusLabel->setText("Veuillez sélectionner un port.");
        return;
    }

    // Créez un nouveau port série et passez-le à ControleCamera
    port = new QSerialPort(ui.portChoiceComboBox->currentText());

    // Vérifiez si le port peut être ouvert via ControleCamera
    if (controleCamera->openPort(port))  // Appel de la méthode openPort() dans ControleCamera
    {
        ui.portStatusLabel->setText("Statut port: Ouvert");
    }
    else
    {
        ui.portStatusLabel->setText("Erreur: " + port->errorString());
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
        break;
    }
    
}

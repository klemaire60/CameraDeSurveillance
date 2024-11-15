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
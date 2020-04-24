# Bienvenue sur le dépôt `display-pasteur`

Ce dépôt fait partie de la plateforme Pasteur. Si vous souhaitez revenir à la documentation principale c'est par ici : [Documentation Plateforme Pasteur 2020](https://docs.google.com/document/d/1M4JxwqTX00nvsrxuXshFL5_MUrutsWxuCbkqYIgzKzs/edit)

Sur ce dépôt vous toute la documentation technique liée au microcontrôleur Atmel Atmega328pu utilisé pour l'affichage :
 
![](img/affichage.png?raw=true)

## Description

Ce package contient le pilote et un exemple de base des modules d'affichage HotelPasteur.

## Driver Files
Le pilote dépends des fichiers suivants
 * src/AvrGPIO.h
 * src/GpioPin.h
 * src/HpasteurDisplay.h

Il a été développé et testé sur le Atmega328pu.
Il n'y a pas d'exigences spécifiques au-delà du backend SDK platformio standard pour Atmega.

Le fichier "HpasteurDisplay_tests.cc" contient des fonctions de test et quelques exemples.

## Utilisation

L'interface est crée en utilisant la classe *HPLetter*:
```c++
    hpasteur::HPLetter gpioSegment;
```

Une led peut être activée à la fois. Pour l'activer,
```c++
    gpioSegment.ledOn()
```

Deux boutons sont disponibles (0 et 1) et accessibles via la fonction:
```c++
    gpioSegment.getButton( 0 /* button number */)
```

Le nombre total de led,
```c++
    gpioSegment.size();
```

## Compilation
Les scripts platformio ont été étendus pour prendre en charge et télécharger automatiquement le firmware sur le réseau à l'aide d'un ESP8266 (ESP8266AVRISP)

Nouvelles cibles pio :
 * "telnet":   command line on the atmega328p flash program
 * "wupload":  upload the firmware via an esp8266 at an IP address set in the "platformio.ini" file
 * "assembly": compile the main file and generate the assembly code
 * "sclean":   clean the "compile" target

Pour utiliser une cible :
```bash
    platformio run --target <target>
```


> Written with [StackEdit](https://stackedit.io/).

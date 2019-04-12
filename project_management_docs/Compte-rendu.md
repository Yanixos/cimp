[![P7 Logo](http://www.ciup.fr/wp-content/uploads/2013/10/Universit%C3%A9-Paris-Diderot.jpg "P7 Logo")](http://www.ciup.fr/wp-content/uploads/2013/10/Universit%C3%A9-Paris-Diderot.jpg "P7 Logo")

------------


# Compte-rendu: Projet Cimp
### Membres:  
**- Alim Yanis**
**- Bareche Yassine**
**- Bouzaouit Oussama**

------------


##### DATE: 12 FEVRIER / 14h30 à 16h30 / SALLE 2001
##### Rapporteur: Alim Yanis

------------


### Ordre du jour:
#### Suivi de la semaine 1: 
1. BrainStorming du projet
2. Plan general du projet

#### Points abordés: 
1. Rédaction des specifications

### Tâches à accomplir lors de la semaine 2:
1. Faire le découpage modulaire du projet




------------


##### DATE: 26 FEVRIER / 14h30 à 16h30 / SALLE 2001
##### Rapporteur: Bouzaouit Oussama

------------


### Ordre du jour :
#### Suivi de la semaine 2: 
1. Découpage modulaire du projet

#### Points abordés  : 
1. Découpage des tâches du projet sous forme de modules
2. Précision des dépendances entre les modules

### Tâches à accomplir lors de la semaine 3:
1. Détailler les fonctionnalités de chaque module




------------


##### DATE: 05 MARS / 14h30 à 16h30 / SALLE 2001
##### Rapporteur: Bouzaouit Oussama

------------


### Ordre du jour :
#### Suivi de la semaine 3:
1. Précision des fonctionnalités des modules
2. Codage des fonctionnalités élémentaires

#### Points abordés  : 
1. Finalisation du découpage modulaire
2. Familiarisation avec le bibliothèque SDL 2.0

### Tâches à accomplir lors de la semaine 4:
1. Codage des parties principales du Parser par Alim Yanis
2. Codage des parties principales de Windows & Images par Bareche Yassine
3. Codage des parties principales de Selection par Bouzaouit Oussama




------------


##### DATE: 12 MARS / 14h30 à 16h30 / SALLE 2001
##### Rapporteur: Bareche Yassine

------------


### Ordre du jour :
#### Suivi de la semaine 4:
1. Mise au point sur quelques détails de syntaxe du CLI.
2. Codage des fonctionnalités principales du module Windows & Images restreintes sur les images ".bmp".
3. Codage des parties principales du module Selection.

#### Points abordés  : 
1. Discussion sur la possibilité d'utiliser les bibliothèques SDL_image et SDL_gfx pour des extensions graphiques étendus.
2. Travail technique pour comprendre la structure de qulques composants SDL à savoir: Surface et Texture afin de décider avec quel composant on représentera les images chargées.
3. Mise au point sur la technique de selection par tracé libre.

### Tâches à accomplir lors de la semaine 5:
1. Concatiner les parties codés jusqu'à présent:Tous les membres.
2. Codage des parties principales du Regex :Alim Yanis
3. Compléter le module Windows & Images et commecer le codage du module Image Effect:Bareche Yassine
4. Compléter le module Selection et commecer le codage du module Selection 
Effect:Bouzaouit Oussama.




------------


##### DATE: 19 MARS / 14h30 à 16h30 / SALLE 2001
##### Rapporteur: Alim Yanis

------------


### Ordre du jour :
#### Suivi de la semaine 5:
1. Module Parser : fini. 
2. Module Windows & Images : la plus part des fonctionnalités a été faite.
3. Module Selection : selection d'une couleur avec ou sans marge de tolerance.

#### Points abordés  : 
1. Utilisation la structure Surface au lieu de Texture
2. Disscussion sur la méthode de test pour chaque module

### Tâches à accomplir lors de la semaine 6:
1. Commencer à faire les fonctionnalités du script editor : Alim Yanis
2. Commencer à faire les fonctionnalités du module Images_Effects : Bareche Yassine
3. Commencer à faire les fonctionnalités du module Selection_Effects : Bouzaouit Oussama




------------


##### DATE: 02 AVRIL / 14h30 à 16h30 / SALLE 2001
##### Rapporteur: Bouzaouit Oussama

------------


### Ordre du jour :
#### Suivi de la semaine 6:
1. Editeur de scripts : création, modification et sauvegarde de scripts.
2. Module Image Effects : codage des principaux transformations d'image.
3. Module Selection Effects : codage des principaux effets de sélections.

#### Points abordés  : 
1. Liaison des modules finalisés
2. Installation des bibliothèques libpng et libjpeg pour permettre la lecture/écriture des format PNG et JPEG

### Tâches à accomplir lors de la semaine 7:
1. Liaison du module Parser avec le Main et le reste des modules : Alim Yanis
2. Compléter le module Images_Effects : Bareche Yassine
3. Compléter le module Selection_Effects : Bouzaouit Oussama




------------


##### DATE: 09 AVRIL / 14h30 à 16h30 / SALLE 2001
##### Rapporteur: Bouzaouit Oussama

------------


### Ordre du jour :
#### Suivi de la semaine 7:
1. Création d'un nouveau module Linker pour faciliter la liaison des modules avec le Main.
2. Complétion du module Image Effects.
3. Complétion du module Selection Effects et ajout de Copy/Cut/Paste.

#### Points abordés  : 
1. Suppression de la dépendance entre SDL et les modules Main, Parser, Linker
2. Fusion des branches Image Effects et Selection Effects avec le master

### Tâches à accomplir lors de la semaine 8:
1. Liaison du module Linker avec les fonctions publiques des autres modules : Alim Yanis
2. Changer l'interface du module Images_Effects, utiliser les ID des fenêtres : Bareche Yassine
3. Changer l'interface du module Selection_Effects, utiliser les ID des fenêtres : Bouzaouit Oussama

------------
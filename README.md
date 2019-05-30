# architecture:
![alt text](../project_management_docs/Architecture.png)

# compilation:
lancer la commande "make" depuis un terminal

# exécution:
lancer "./cimp"

# tests check:
ouvrir un terminal et se placer au sous-repertoire ./tests
compiler le code des tests avec la commande "make"
lancer les tests avec la commande "CK_FORK=no ./tests_runner"


# manuelle des commandes cimp:
```
open filename.EXT [-o target_window]
```
ouvrir une image, target_window peut etre la fenetre dans laquelle l'image sera ouverte, sinon une nouvelle fenetre est créée

```
save window_name -o output.EXT
```
exporter l'image d'une fenetre window_name dans un fichier output.EXT

```
set_bg color
```
définir une couleur par défaut de l'arrière plan
color: une couleur constante (white,..) ou les 3 composantes d'une couleur (r,g,b)

```
select_rect window_name [-m ADD|SUB|OVERWRITE] [-p (X1,Y1) (X2,Y2)]
```
séléctionner une region rectanglaire dans une image
options:
	-m: définir le mode de sélection, en addition, en soustraction, ou en écrasement
	-p: définir les 2 pixels d'extrémités d'un rectangle

```
select_free window_name [-m ADD|SUB|OVERWRITE]
```
séléctionner une région dans une image avec tracé libre de la souris
options:
	-m: définir le mode de sélection, en addition, en soustraction, ou en écrasement

```
select_color window_name [-m ADD|SUB|OVERWRITE] -c color [-t 0-100]
```
séléctionner tous les pixels d'une couleur "color"
options:
	-m: définir le mode de sélection, en addition, en soustraction, ou en écrasement
	-t: ajouter un pourcentage de tolérance entre 0 et 100

```
unselect window_name
```
désélectionner tous les pixels d'une image

```
copy window_name
```
copier dans le presse-papier la région séléctionnée dans une image

```
cut window_name [-f color]
```
couper d'une image la région séléctionnée, la mettre dans le presse-papier
options:
	-f: donner la couleur de fond, soit une couleur constante (white,...) ou en donnant les composante (r,g,b)

```
paste window_name -p (X,Y)
```
coller dans une image le contenu du presse-papier, dans la position (X,Y)

```
symetric window_name -d V|H
```
effectuer une symétrie à une image, peut etre soit horizontale (-d H) ou verticale (-d V)

```
rotate window_name -a ANGLE
```
pivoter une image d'un angle ANGLE

```
fill window_name -c color
```
remplir la région séléctionnée d'une image avec la couleur color, qui peut être soit une couleur constante (white,..) soit des composantes (r,g,b)

```
replace window_name color1 color2 [-t 0-100]
```
remplacer dans la région séléctionnée d'une image, tous les pixels colorié d'une couleur "color1", par la couleur "color2"
options:
	-t: donner un pourcentage de tolérance, entre 0 et 100

```
negative window_name
```
appliquer l'effet négatif sur une image

```
gray window_name
```
appliquer l'effet niveaux de gris sur une image

```
black_white window_name -t 0-100
```
appliquer l'effet noir et blanc sur une image, -t V contient le seuil de séparation entre le noir et blanc, avec une valeur comprise entre 0 et 100

```
brightness window_name -r 0-200
```
changer la luminosité d'une image, -r V contient le pourcentage de luminosité, entre 0 et 200

```
contrast window_name -r 0-200
```
changer le contraste d'une image, -r V contient le pourcentage de contraste, entre 0 et 200

```
blur window_name -r V
```
appliquer l'effet de flou sur une image, V contient l'intensité de l'effet

```
pixel window_name -s PIXELSIZE
```
appliquer l'effet de pixelization sur une image, PIXELSIZE contient l'intensité de l'effet

```
undo window_name
```
annuler la dernière action effectuée sur une image

```
redo window_name
```
retablir une action annulée sur une image

```
close window_name
```
fermer une image

```
write_script filename.cimp
```
écrire un script cimp

```
load_script filename.cimp
```
charger un script cimp

```
edit_script filename.cimp
```
modifier un script cimp

```
rename_script filename.cimp -o newname.cimp
```
renommer une script cimp

```
execute_script filename.cimp
```
exéctuer un script cimp

```
cd [ref]
```
se positionner dans un répertoire

```
ls [ref]
```
afficher le contenu d'un répertoire

```
batch_mode 0|1|2
```
sélectionner le mode du batch

```
help [command]
```
afficher le manuel d'une commande

```
exit [value]
```
quitter cimp

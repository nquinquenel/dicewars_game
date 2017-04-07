Projet DiceWars
===================

Développement du jeu DiceWars, composé d'une interface graphique (librairie SDL) et d'une intelligence artificielle.

Comment lancer l'éxécutable ?
===================

./DiceWars 4 5 libStrategie1.so libStrategie2.so libStrategie3.so libStrategie4.so

4: Nombre de partie à faire

5: Nombre de joueurs (1 humains et 4 IA)

libStrategie1.so libStrategie2.so libStrategie3.so libStrategie4.so : Nom des librairies pour les stratégies (possible de mettre la même plusieurs fois)

Comment compiler ?
===================

Il suffit de se placer dans le répertoire /src/ puis d'utiliser les commandes suivantes :

    make: Compile le projet dans son ensemble et créer un fichier /src/DiceWars
    make clean: Supprime les fichiers objets (.o)
    make mrproper: Supprime les fichiers objets et l'exécutable

Organisation
===================

Partie Interface graphique

    -QUINQUENEL Nicolas
    -EHRESMANN Nicolas
    
Partie Stratégie de jeu

    -SOYER Mathieu
    -OUTHIER Arthur

    
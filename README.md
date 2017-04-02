Projet DiceWars
===================

Développement du jeu DiceWars, composé d'une interface graphique (librairie SDL) et d'une intelligence artificielle.

Comment jouer ?
===================

Sélectionner son terrain (une bordure interne blanche va apparaître si vous avez sélectionné votre territoire).

Sélectionner un terrain à attaquer.

Appuyer sur la touche entrée pour passer son tour.

Comment lancer l'éxécutable ?
===================

./DiceWars.exe 4 5 ia1 ia2 ia3

4: Nombre de partie à faire (non implémenté)
5: Nombre de joueurs
ia1 ia2 ia3: Nom des IA (et nombre d'IA qui jouent)

Note: Les noms des IA ne servent à rien, ils seront bientôt changés par les noms des librairies (.dll par exemple)

Comment compiler ?
===================

Il suffit de se placer dans le répertoire /src/ puis d'utiliser les commandes suivantes :

    make: Compile le projet dans son ensemble et créer un fichier /src/DiceWars.exe
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

    
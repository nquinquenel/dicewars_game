CFLAGS=-Wall `sdl2-config --cflags --libs`

# création de l'exécutable 'Programme'
all: main.o arbitre.o strategie.o fenetre.o generation.o jouer.o
				gcc main.o arbitre.o strategie.o fenetre.o generation.o jouer.o -o DiceWars $(CFLAGS)

main.o: main.c Librairies/interface.h Librairies/interfacePerso.h
				gcc -c main.c -o main.o $(CFLAGS)

arbitre.o: Strategie/arbitre.c Librairies/interface.h Interface/fenetre.h
				gcc -c Strategie/arbitre.c -o arbitre.o

strategie.o: Strategie/strategie.c
				gcc -c Strategie/strategie.c -o strategie.o $(CFLAGS)

fenetre.o: Interface/fenetre.c Interface/generation.h
				gcc -c Interface/fenetre.c -o fenetre.o $(CFLAGS)

generation.o: Interface/generation.c Librairies/interface.h
				gcc -c Interface/generation.c -o generation.o $(CFLAGS)

jouer.o: Interface/jouer.c Interface/jouer.h
				gcc -c Interface/jouer.c -o jouer.o $(CFLAGS)

# suppression des fichiers temporaires
clean:
				rm -rf ./*.o

# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
mrproper: clean
				rm -rf DiceWars

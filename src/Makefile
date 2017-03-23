CFLAGS=-Wall -lSDL2

# création de l'exécutable 'Programme'
all: main.o strategie.o fenetre.o generation.o
				gcc main.o strategie.o fenetre.o generation.o -o DiceWars $(CFLAGS)

main.o: main.c Librairies/interface.h Librairies/interfacePerso.h
				gcc -c main.c -o main.o $(CFLAGS)

strategie.o: Strategie/strategie.c Librairies/interface.h Librairies/interfacePerso.h Interface/fenetre.h
				gcc -c Strategie/strategie.c -o strategie.o $(CFLAGS)

fenetre.o: Interface/fenetre.c Interface/generation.h
				gcc -c Interface/fenetre.c -o fenetre.o $(CFLAGS)

generation.o: Interface/generation.c Librairies/interface.h
				gcc -c Interface/generation.c -o generation.o $(CFLAGS)

# suppression des fichiers temporaires
clean:
				rm -rf ./*.o

# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
mrproper: clean
				rm -rf DiceWars

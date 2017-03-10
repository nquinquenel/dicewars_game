CFLAGS=-Wall

# création de l'exécutable 'Programme'
all: main.o fenetre.o
				gcc main.o fenetre.o -o DiceWars $(CFLAGS)

main.o: main.c Interface/fenetre.h
				gcc -c main.c -o main.o $(CFLAGS)

fenetre.o: Interface/fenetre.c
				gcc -c Interface/fenetre.c -o fenetre.o $(CFLAGS)

# suppression des fichiers temporaires
clean:
				rm -rf ./*.o

# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
mrproper: clean
				rm -rf DiceWars

EXE=game wizard rogue barbarian

all:$(EXE)

game: game.c dungeon.o
	gcc game.c dungeon_source/dungeon.o -o game -lrt -pthread

wizard: wizard.c dungeon.o
	gcc wizard.c -o wizard

rogue: rogue.c dungeon.o
	gcc rogue.c -o rogue

barbarian: barbarian.c dungeon.o
	gcc barbarian.c -o barbarian

clean:
	rm $(EXE) 
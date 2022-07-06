EXE=game.dexe wizard.dexe rogue.dexe barbarian.dexe
ORI_DUNGEON=dungeon.o
SRC_DUNGEON=dungeon_source/dungeon.c
all:$(EXE)

game.dexe:
	gcc game.c shmmgr.c $(SRC_DUNGEON) -o game.dexe -lrt -pthread

wizard.dexe:
	gcc wizard.c shmmgr.c -o wizard.dexe

rogue.dexe:
	gcc rogue.c shmmgr.c -o rogue.dexe

barbarian.dexe:
	gcc barbarian.c shmmgr.c -o barbarian.dexe

clean:
	rm $(EXE) 
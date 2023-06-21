parametrosCompilacao=-Wall #-Wshadow
nomePrograma=trab

all: $(nomePrograma)

$(nomePrograma): main.o avl.o
	gcc -o $(nomePrograma) main.o avl.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

ordenacao.o: avl.h avl.c
	gcc -c avl.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)

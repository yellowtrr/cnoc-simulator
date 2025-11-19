cnoc-simulator: router.c packet.c stats.c simulation.c main.c
	clang -Wall -O3 router.c packet.c stats.c simulation.c main.c -o cnoc-simulator

clean: 
	rm -f cnoc-simulator

LIBS=-Itommyds/tommyds

all:
	gcc $(LIBS) st.c bench.c tommyds/tommyds/tommy.c main.c -ljemalloc -O3 -o test

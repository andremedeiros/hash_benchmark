all:
	gcc st.c bench.c main.c -ljemalloc -O3 -o test

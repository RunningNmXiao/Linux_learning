SRC:=$(wildcard *.c)
thread_pool_server:$(SRC)
	gcc $^ -o $@ -lpthread
clean:
	rm thread_pool_server

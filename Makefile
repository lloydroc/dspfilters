all:
	gcc filters.c -o filters
	./filters
clean:
	rm filters

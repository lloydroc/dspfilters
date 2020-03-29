all:
	gcc filters_fp.c -o filters_fp
	gcc filters_int.c -o filters_int
run: all
	./filters_fp
	./filters_int
clean:
	rm filters_int filters_fp

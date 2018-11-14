CC = gcc
SRC = proj2.c
OUT = proj2

all: $(SRC)
	clear
	$(CC) -o $(OUT) $(SRC)
	chmod 755 $(OUT)

clean: $(OUT)
	rm $(OUT)


test:
	@echo "\n"
	./proj2 -u http://www.icir.org -d -o icir.html
	sha1sum icir.html


	@echo "\n"
	./proj2 -r -u http://www.icir.org -o mallman.html
	sha1sum mallman.html


	@echo "\n"
	./proj2 -u http://case.edu/images/2014/give-home.png -R -o case.png
	sha1sum case.png


	@echo "\n"
	./proj2 -u http://www.icir.org/mallman/325-samples/10.tar.gz -o 10.tar.gz
	sha1sum 10.tar.gz


	@echo "\n"
	./proj2 -u http://www.icir.org/mallman/325-samples/50.tar.gz -o 50.tar.gz
	sha1sum 50.tar.gz


	@echo "\n"
	./proj2 -u http://thedaily.case.edu/5-questions-spartan-marching-band-director-benjamin-helton/ -r -R -o band.html
	sha1sum band.html
FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all: install 

install: 
	make -C ./lib/libmx
	make -C ./lib/cjson
	make -C ./server
	make -C ./client
	mv server/uchat_server .
	mv client/uchat .


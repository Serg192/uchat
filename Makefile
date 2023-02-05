FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all: install 

install: 
	make -C ./lib/libmx
	make -C ./server
	mv server/uchat_server .


# uchat
Chat “Message Mania” is developed by four students of Innovation Campus (NTU “Khpi”). The team members are:

- sstohnij
- ozelenskyj
- ayefimenko
- dstarosvit


We have developed our chat for MacOS Catalina X, but you can also run it on Linux Ubuntu.


To run it on MacOS, just clone our repo by the “git clone {link}” command or downloading zip archive, and then use the command “make” in the terminal. Run ./uchat_server [port] in the terminal window and ./uchat [ip] [port] in another one.


To compile on Ubuntu, after cloning you need to modify some files:
- lib/libmx/inc/libmx.h and client/inc/client.h: use #include <malloc.h>;
- lib/libmx/src/mx_realloc.c: use function malloc_usable_size instead of malloc_size.

In some cases you might need to change makefiles according to our commit “Linux final”.


You also have an opportunity to see all recently created chats and test our program with already registared user:
- Login - Tester
- Password - 123456


If you found any problems while running, please, don’t be strict. Just reload the server and clients and check again. We hope you will enjoy using Message Mania!
<br><br>Screenshots:

![Screenshot from 2023-06-13 21-05-32](https://github.com/Serg192/uchat/assets/66135825/b2299bc5-cc94-4b98-a687-34da63292240)

<br><br>
![Screenshot from 2023-06-13 21-07-53](https://github.com/Serg192/uchat/assets/66135825/e363bd9b-7e89-434d-a83d-351db50d8697)


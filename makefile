
executable = quic

all : quic

quic : copy.c misc.c delete.c quic.c
	gcc -o quic quic.c delete.c misc.c copy.c

clean:
	rm $(executable) $(file)

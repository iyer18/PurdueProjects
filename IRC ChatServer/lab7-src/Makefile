
goal:  TestIRCServer Client room 


Client: Client.cc
	gcc Client.cc -o Client `pkg-config --cflags --libs gtk+-2.0`

TestIRCServer: TestIRCServer.c
	g++ -g -o TestIRCServer TestIRCServer.c

room: room.cc
	gcc room.cc -o room `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm -f hello panned entry radio timer TestIRCServer


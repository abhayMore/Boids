all: sfml-app
	./sfml-app

sfml-app: $(F2) Vector2.o
	g++ -std=c++17 $(F2) Vector2.o -o sfml-app -ltgui -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

Vector2.o : Vector2.cpp Vector2.h;
	g++ -std=c++17 -c Vector2.cpp

$(F2) : Vector2.h $(F1)
		g++ -c $(F1)

clean:
	rm -rf *o sfml-app

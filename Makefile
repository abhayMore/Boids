all: sfml-app
	./sfml-app

sfml-app: $(F2) Vector2.o Boids.o
	g++ -std=c++17 $(F2) Vector2.o Boids.o -o sfml-app -ltgui -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

Vector2.o : Vector2.cpp Vector2.h;
	g++ -std=c++17 -c Vector2.cpp

Boids.o : Boids.cpp Boids.h
		g++ -std=c++17 -c Boids.cpp
		
$(F2) : Vector2.h $(F1) Boids.h
		g++ -std=c++17 -c $(F1)

clean:
	rm -rf *o sfml-app

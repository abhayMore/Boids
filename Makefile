EXEC = sfml-app
CC = g++
CFLAGS = -std=c++17 -c -Wall
LDFLAGS = -std=c++17 -o $(EXEC) -ltgui -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -rf *o $(EXEC)

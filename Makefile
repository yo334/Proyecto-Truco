# Makefile - Proyecto Truco
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = truco
SRCS = main.cpp carta.cpp mazo.cpp jugador.cpp ronda.cpp ia.cpp partida.cpp interfaz.cpp mensajes.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

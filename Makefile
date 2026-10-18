# Makefile - Proyecto Truco
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = truco
SRCS = main.cpp carta.cpp mazo.cpp jugador.cpp ronda.cpp ia.cpp partida.cpp interfaz.cpp mensajes.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# -MMD -MP: el compilador anota de qué .h depende cada .o (archivos .d),
# así si cambiás un .h se recompilan todos los .cpp que lo incluyen.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(TARGET) $(TARGET).exe

.PHONY: clean

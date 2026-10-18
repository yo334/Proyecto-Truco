#pragma once
#include <string>
#include <vector>
#include "carta.h"

class Interfaz {
public:
    static void mostrarCartasHorizontal(const std::vector<Carta>& cartas);
    static void mostrarTablero(const std::string& nombre_jugador, int pts_jugador, const std::string& nombre_ia, int pts_ia, int objetivo);
    static void mostrarEstadoMano(const std::vector<Carta>& mano_jugador, int cartas_ia, const std::vector<std::string>& resultados_rondas);
    static int pedirOpcion(const std::vector<std::string>& opciones);
    static void limpiarPantalla();
    static void pausar();
    static void mostrarBanner();
};

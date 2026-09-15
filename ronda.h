#pragma once
#include <string>
#include <optional>
#include "carta.h"

// Clase que gestiona el enfrentamiento de cartas en una baza (una ronda de las 3)
class Ronda {
public:
    Ronda();

    // Registra qué carta jugó cada uno
    void registrarCartaJugador(Carta c);
    void registrarCartaIA(Carta c);

    // Determina el ganador comparando jerarquías (menor jerarquia = más fuerte)
    // Retorna: "jugador", "ia", "parda"
    std::string determinarGanador();

    bool esParda();
    bool estaCompleta(); // true si ambos jugaron

    // Getters para las cartas jugadas
    Carta obtenerCartaJugador();
    Carta obtenerCartaIA();

private:
    std::optional<Carta> carta_jugador;
    std::optional<Carta> carta_ia;
    bool jugador_jugo;
    bool ia_jugo;
};

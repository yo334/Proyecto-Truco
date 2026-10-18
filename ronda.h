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

    bool estaCompleta(); // true si ambos jugaron

    // ¿Ya tiró cada uno en esta ronda?
    bool jugoJugador() const;
    bool jugoIA() const;

    // Getters para las cartas jugadas (solo si ya tiró)
    Carta obtenerCartaJugador();
    Carta obtenerCartaIA();

private:
    std::optional<Carta> carta_jugador;
    std::optional<Carta> carta_ia;
};

#include "ronda.h"

using namespace std;

Ronda::Ronda() {}

void Ronda::registrarCartaJugador(Carta c) {
  carta_jugador = c;
}

void Ronda::registrarCartaIA(Carta c) {
  carta_ia = c;
}

string Ronda::determinarGanador() {
  if (!estaCompleta())
    return ""; // Si no jugaron los dos, no hay ganador todavia

  int jerarquia_jugador = carta_jugador->obtenerJerarquia();
  int jerarquia_ia = carta_ia->obtenerJerarquia();

  if (jerarquia_jugador < jerarquia_ia) {
    return "jugador";
  } else if (jerarquia_ia < jerarquia_jugador) {
    return "ia";
  } else {
    return "parda";
  }
}

// optional ya sabe si tiene una carta adentro: no hace falta otro flag
bool Ronda::estaCompleta() {
  return carta_jugador.has_value() && carta_ia.has_value();
}

bool Ronda::jugoJugador() const { return carta_jugador.has_value(); }

bool Ronda::jugoIA() const { return carta_ia.has_value(); }

Carta Ronda::obtenerCartaJugador() { return *carta_jugador; }

Carta Ronda::obtenerCartaIA() { return *carta_ia; }

/*
 * ¿Qué hace esta clase?
 * La clase Ronda se encarga de representar una baza (una tirada de cartas por
 * cada jugador en la mano). Guarda qué carta jugó cada uno y determina quién es
 * el ganador basándose en la jerarquía del truco.
 *
 * ¿Qué concepto de C++ ilustra?
 * Ilustra el uso de `optional` de C++17, que es re útil para representar un
 * valor que "puede o no estar" (en este caso, una carta que todavía no se tiró
 * a la mesa). Evitamos usar punteros nulos o cartas "vacías".
 */

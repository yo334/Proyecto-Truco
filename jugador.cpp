#include "jugador.h"
#include <stdexcept>
#include <algorithm>

Jugador::Jugador(std::string nombre) : nombre(nombre), puntos(0), esMano(false) {}

void Jugador::recibirCarta(Carta c) {
    if (mano.size() >= 3) {
        throw std::logic_error("¡Epa! No podés tener más de 3 cartas en la mano.");
    }
    mano.push_back(c);
}

// Juega (y saca) la carta en la posición indicada (0-based)
Carta Jugador::jugarCarta(int indice) {
    if (indice < 0 || indice >= static_cast<int>(mano.size())) {
        throw std::out_of_range("Índice de carta inválido. Fijate bien qué elegís.");
    }
    Carta elegida = mano[indice];
    // Borramos la carta de la mano, porque ya la tiró a la mesa
    mano.erase(mano.begin() + indice);
    return elegida;
}

// Acá calculamos el envido respetando las sagradas reglas
int Jugador::calcularEnvido() const {
    if (mano.empty()) return 0;

    int maxEnvido = 0;
    bool hayPareja = false;

    // Chequeamos si hay dos (o más) del mismo palo
    for (size_t i = 0; i < mano.size(); ++i) {
        for (size_t j = i + 1; j < mano.size(); ++j) {
            if (mano[i].obtenerPalo() == mano[j].obtenerPalo()) {
                hayPareja = true;
                int envidoPareja = mano[i].obtenerValorEnvido() + mano[j].obtenerValorEnvido() + 20;
                if (envidoPareja > maxEnvido) {
                    maxEnvido = envidoPareja;
                }
            }
        }
    }

    if (hayPareja) {
        return maxEnvido;
    }

    // Si nadie hizo pareja (los 3 palos distintos), el envido es la carta más alta
    int mayorCarta = 0;
    for (const auto& c : mano) {
        if (c.obtenerValorEnvido() > mayorCarta) {
            mayorCarta = c.obtenerValorEnvido();
        }
    }
    return mayorCarta;
}

// True si las 3 cartas (y solo si tiene 3) son del mismo palo
bool Jugador::tieneFlor() const {
    if (mano.size() < 3) return false;
    return (mano[0].obtenerPalo() == mano[1].obtenerPalo() &&
            mano[1].obtenerPalo() == mano[2].obtenerPalo());
}

// Calcula los puntos de la flor (suma de las 3 cartas + 20)
int Jugador::calcularFlor() const {
    if (!tieneFlor()) return 0; // Si sos un fantasma y pedís flor sin tener
    return mano[0].obtenerValorEnvido() + 
           mano[1].obtenerValorEnvido() + 
           mano[2].obtenerValorEnvido() + 20;
}

void Jugador::limpiarMano() {
    mano.clear();
}

void Jugador::sumarPuntos(int pts) {
    puntos += pts;
}

int Jugador::obtenerPuntos() const {
    return puntos;
}

void Jugador::setEsMano(bool valor) {
    esMano = valor;
}

bool Jugador::obtenerEsMano() const {
    return esMano;
}

std::string Jugador::obtenerNombre() const {
    return nombre;
}

// Retornamos la referencia constante para no copiar vectores enteros al pedo
const std::vector<Carta>& Jugador::obtenerMano() const {
    return mano;
}

int Jugador::cantidadCartasEnMano() const {
    return static_cast<int>(mano.size());
}

/*
=============================================================================
[MODO TUTOR ACTIVO]
Acá vemos la clase Jugador armada de forma bien robusta.
- Concepto C++: El paso por referencia constante (`const std::vector<Carta>&`).
  El método `obtenerMano` no devuelve una copia de las cartas, sino una 
  referencia a la lista original, pero "protegida" con `const`. Esto es 
  fundamental para el rendimiento, porque copiamos solo un punterito invisible
  en vez de todo el array cada vez que queremos mirar la mano del chabón.
- Además, usamos excepciones de estándar (`std::logic_error`, `std::out_of_range`)
  para que, si la lógica de control la pifia desde afuera, el programa no
  reviente de manera silenciosa, sino que explote avisando por qué.
=============================================================================
*/

#include "mazo.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>


using namespace std;

// Al instanciar, arrancamos de una con un mazo nuevo y mezcladito
Mazo::Mazo() { reiniciar(); }

// Mezclador usando las herramientas modernas de C++
void Mazo::mezclar() {
  // Agarramos la hora actual del sistema como semilla (seed)
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  // Usamos el motor Mersenne Twister (mt19937) que es posta el mejor para esto
  mt19937 g(seed);

  // Mezclamos (shuffle) el vector completo
  shuffle(cartas.begin(), cartas.end(), g);
}

// Saca una carta de atrás (el tope del mazo)
Carta Mazo::repartir() {
  if (cartas.empty()) {
    throw out_of_range(
        "¡Pará un poco fiera! El mazo está vacío, no se puede repartir.");
  }
  // Agarramos la última y la sacamos
  Carta c = cartas.back();
  cartas.pop_back();
  return c;
}

// Resetea el mazo a su estado inicial (40 cartas) y mezcla
void Mazo::reiniciar() {
  cartas.clear(); // Limpiamos si quedó algo

  vector<string> palos = {"Espada", "Basto", "Oro", "Copa"};
  vector<int> numeros = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12};

  // Doble bucle, armamos las 40 cartas y las mandamos al vector
  for (const auto &p : palos) {
    for (int n : numeros) {
      cartas.push_back(Carta(n, p));
    }
  }

  mezclar();
}

int Mazo::cantidadRestante() const { return static_cast<int>(cartas.size()); }

/*

Acá vemos la clase Mazo en acción.
- Concepto C++: Uso dinámico de contenedores de la Standard Template Library
  (STL). Usamos `vector` que es un arreglo dinámico, re piola porque
  crece y se achica solo (con `push_back` y `pop_back`).
- Otro tema clave es la mezcla: En C++ moderno (desde C++11) usamos
  `<random>` con `mt19937` y `shuffle` en vez del viejo `rand()`.
  Esto garantiza una distribución probabilística muchísimo mejor (más
"aleatoria" de verdad), para que no te caguen en el truco repartiendo siempre lo
mismo.
*/

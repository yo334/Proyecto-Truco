#include "jugador.h"
#include <algorithm>
#include <stdexcept>


using namespace std;

Jugador::Jugador(string nombre) : nombre(nombre), puntos(0) {}

// Al recibir cartas se guardan en dos contenedores:
// - 'mano': representa las cartas activas que el jugador todavía puede jugar a la mesa.
// - 'mano_inicial': conserva de forma inmutable las 3 cartas repartidas originalmente.
// Esto es indispensable porque el envido y la flor se evalúan con la mano completa, incluso si
// el canto o la resolución se dan cuando ya se tiraron una o más cartas a la mesa.
void Jugador::recibirCarta(Carta c) {
  if (mano.size() >= 3) {
    throw logic_error("¡Epa! No podés tener más de 3 cartas en la mano.");
  }
  mano.push_back(c);
  mano_inicial.push_back(c);
}

// Juega (y saca) la carta en la posición indicada (0-based).
// Se elimina de 'mano' para que no pueda volver a jugarse, pero permanece en 'mano_inicial'.
Carta Jugador::jugarCarta(int indice) {
  if (indice < 0 || indice >= static_cast<int>(mano.size())) {
    throw out_of_range("Índice de carta inválido. Fijate bien qué elegís.");
  }
  Carta elegida = mano[indice];
  // Borramos la carta de la mano, porque ya la tiró a la mesa
  mano.erase(mano.begin() + indice);
  return elegida;
}

// Algoritmo de cálculo de envido (reglamento oficial):
// 1. Busca todas las combinaciones posibles de 2 cartas (pares C(N,2)) mediante un doble bucle.
// 2. Si dos cartas comparten el palo, suman sus valores faciales de envido (figuras 10, 11, 12 valen 0)
//    más el plus reglamentario de 20 puntos. Se actualiza maxEnvido para quedarse con el par más alto
//    (clave si el jugador tiene 3 cartas del mismo palo sin jugar con flor).
// 3. Fallback: si los 3 palos son distintos (hayPareja == false), el envido es el valor individual
//    de la carta más alta (0 a 7).
int Jugador::calcularEnvido() const {
  if (mano_inicial.empty())
    return 0;

  int maxEnvido = 0;
  bool hayPareja = false;

  // Chequeamos si hay dos (o más) del mismo palo
  for (size_t i = 0; i < mano_inicial.size(); ++i) {
    for (size_t j = i + 1; j < mano_inicial.size(); ++j) {
      if (mano_inicial[i].obtenerPalo() == mano_inicial[j].obtenerPalo()) {
        hayPareja = true;
        int envidoPareja =
            mano_inicial[i].obtenerValorEnvido() + mano_inicial[j].obtenerValorEnvido() + 20;
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
  for (const auto &c : mano_inicial) {
    if (c.obtenerValorEnvido() > mayorCarta) {
      mayorCarta = c.obtenerValorEnvido();
    }
  }
  return mayorCarta;
}

// True si las 3 cartas (y solo si tiene 3) son del mismo palo
bool Jugador::tieneFlor() const {
  if (mano_inicial.size() < 3)
    return false;
  return (mano_inicial[0].obtenerPalo() == mano_inicial[1].obtenerPalo() &&
          mano_inicial[1].obtenerPalo() == mano_inicial[2].obtenerPalo());
}

// Calcula los puntos de la flor (suma de las 3 cartas + 20)
int Jugador::calcularFlor() const {
  if (!tieneFlor())
    return 0; // Si sos un fantasma y pedís flor sin tener
  return mano_inicial[0].obtenerValorEnvido() + mano_inicial[1].obtenerValorEnvido() +
         mano_inicial[2].obtenerValorEnvido() + 20;
}

void Jugador::limpiarMano() {
  mano.clear();
  mano_inicial.clear();
}

void Jugador::sumarPuntos(int pts) { puntos += pts; }

int Jugador::obtenerPuntos() const { return puntos; }

string Jugador::obtenerNombre() const { return nombre; }

// Retornamos la referencia constante para no copiar vectores enteros al pedo
const vector<Carta> &Jugador::obtenerMano() const { return mano; }

int Jugador::cantidadCartasEnMano() const {
  return static_cast<int>(mano.size());
}

/*
Acá vemos la clase Jugador armada de forma bien robusta.
- Concepto C++: El paso por referencia constante (`const vector<Carta>&`).
  El método `obtenerMano` no devuelve una copia de las cartas, sino una
  referencia a la lista original, pero "protegida" con `const`. Esto es
  fundamental para el rendimiento, porque copiamos solo un punterito invisible
  en vez de todo el array cada vez que queremos mirar la mano del chabón.
- Además, usamos excepciones de estándar (`logic_error`, `out_of_range`)
  para que, si la lógica de control la pifia desde afuera, el programa no
  reviente de manera silenciosa, sino que explote avisando por qué.
=============================================================================
*/

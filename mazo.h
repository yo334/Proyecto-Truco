#pragma once
#include <vector>
#include "carta.h"

// Clase que maneja el mazo del juego (40 cartas españolas)
class Mazo {
private:
    std::vector<Carta> cartas;

public:
    // El constructor arma el mazo automáticamente
    Mazo();

    // Mezcla las cartas disponibles de forma aleatoria
    void mezclar();

    // Saca la carta de arriba del mazo y la retorna
    Carta repartir();

    // Vuelve a generar las 40 cartas y las mezcla para una nueva mano
    void reiniciar();

    // Nos dice cuántas cartas quedan en el mazo
    int cantidadRestante() const;
};

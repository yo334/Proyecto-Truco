#pragma once
#include <string>
#include <vector>
#include "carta.h"

// Clase que representa a un jugador en la mesa
class Jugador {
private:
    std::string nombre;
    std::vector<Carta> mano;         // las que le quedan por tirar
    std::vector<Carta> mano_inicial; // las 3 que recibió (para envido y flor)
    int puntos;

public:
    // Constructor
    Jugador(std::string nombre);

    // Agrega una carta a la mano del jugador
    void recibirCarta(Carta c);

    // Selecciona una carta (0 a size-1) para tirarla a la mesa
    Carta jugarCarta(int indice);

    // Envido y flor se calculan con las 3 cartas REPARTIDAS, aunque ya haya
    // tirado alguna (el envido se puede cantar después de que tiró el mano).
    // Calcula y retorna los puntos de envido
    int calcularEnvido() const;

    // Retorna true si las tres cartas son del mismo palo
    bool tieneFlor() const;

    // Retorna el valor de la flor (solo válido si tiene flor)
    int calcularFlor() const;

    // Vacía las cartas que le quedan (para el final de la ronda)
    void limpiarMano();

    // Suma puntos al total del jugador
    void sumarPuntos(int pts);

    // Retorna los puntos actuales
    int obtenerPuntos() const;

    // Devuelve el nombre del chabón
    std::string obtenerNombre() const;

    // Retorna una referencia constante a las cartas en mano
    const std::vector<Carta>& obtenerMano() const;

    // Dice cuántas cartas le quedan por jugar
    int cantidadCartasEnMano() const;
};

#pragma once
#include <string>
#include <vector>
#include "carta.h"

// Clase que representa a un jugador en la mesa
class Jugador {
private:
    std::string nombre;
    std::vector<Carta> mano;
    int puntos;
    bool esMano; // Indica si es el jugador "mano" (el que empieza)

public:
    // Constructor
    Jugador(std::string nombre);

    // Agrega una carta a la mano del jugador
    void recibirCarta(Carta c);

    // Selecciona una carta (0 a size-1) para tirarla a la mesa
    Carta jugarCarta(int indice);

    // Calcula y retorna los puntos de envido que tiene en la mano
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

    // Setea si es mano o no
    void setEsMano(bool valor);

    // Dice si es mano o no
    bool obtenerEsMano() const;

    // Devuelve el nombre del chabón
    std::string obtenerNombre() const;

    // Retorna una referencia constante a las cartas en mano
    const std::vector<Carta>& obtenerMano() const;

    // Dice cuántas cartas le quedan por jugar
    int cantidadCartasEnMano() const;
};

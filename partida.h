#pragma once
#include <string>
#include <vector>
#include <utility>
#include "jugador.h"
#include "ia.h"
#include "mazo.h"

class Partida {
public:
    Partida();

    // Inicia y corre el loop completo de la partida
    void iniciar();

private:
    Jugador jugador;
    IA ia;
    Mazo mazo;
    int puntos_objetivo; // 15 o 30
    bool con_flor;
    bool turno_es_mano_jugador; // true si el jugador humano es mano

    int nivel_truco; // 0=sin, 1=truco, 2=retruco, 3=vale4
    std::string quien_canto_truco; // "jugador", "ia", o ""
    bool truco_querido; // true si el truco cantado ya fue querido

    // Juega una mano completa (reparte, resuelve envido/flor/truco, cuenta puntos)
    void jugarMano();

    // Reparte 3 cartas a cada uno
    void repartirCartas();

    // Resuelve el envido de la mano (interacción con el jugador)
    // Retorna cuántos puntos ganó cada uno: pair<pts_jugador, pts_ia>
    std::pair<int,int> resolverEnvido();

    // Resuelve el truco de la mano
    // Retorna cuántos puntos ganó cada uno
    std::pair<int,int> resolverTruco(const std::string& origen);

    // Resuelve la flor si corresponde
    std::pair<int,int> resolverFlor();

    // Juega las rondas de cartas (hasta 3). Retorna quién ganó la mano: "jugador", "ia"
    std::string jugarRondas();

    // Pregunta al jugador qué acción quiere tomar en su turno
    // opciones: lista de strings posibles
    // Retorna el índice de la opción elegida
    int pedirAccion(const std::vector<std::string>& opciones);

    // Retorna true si alguien llegó al puntaje objetivo
    bool terminoPartida();
};

#pragma once
#include <string>
#include <random>
#include "jugador.h"

// Clase IA hereda de Jugador e implementa el sistema de toma de decisiones
class IA : public Jugador {
public:
    IA();

    // DECISIONES DE ENVIDO
    std::string decidirCantoEnvido();
    bool decidirAceptarEnvido(const std::string& canto);

    // DECISIONES DE TRUCO
    bool decidirCantarTruco(int ronda_actual, int rondas_ganadas_ia,
                            int rondas_ganadas_jugador, bool hubo_parda,
                            int pts_ia, int pts_jugador);
    bool decidirAceptarTruco(int nivel);

    // ESTRATEGIA DE JUEGO DE CARTAS
    int elegirCarta(int rondas_ganadas_ia, int rondas_ganadas_jugador);

    // FRASES DE PIQUE
    std::string obtenerFraseDePique(const std::string& situacion);

private:
    std::mt19937 generador;

    // Evalúa la fuerza media de las cartas que le quedan
    double evaluarFuerzaMedia();

    // Tira un dado de probabilidad: retorna true con la probabilidad dada (0.0 a 1.0)
    bool tirarDado(double probabilidad);
};

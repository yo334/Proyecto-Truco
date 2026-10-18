#pragma once
#include <optional>
#include <random>
#include <string>
#include <vector>
#include "jugador.h"

// Lo que la IA "ve" de la mano en curso para tomar decisiones.
// Es un struct de datos (sin comportamiento): reemplaza las firmas de
// 6 parámetros que tenían antes las funciones de la IA.
struct ContextoIA {
    int ronda = 0;                      // 0, 1 o 2
    int ganadas_ia = 0;                 // rondas ganadas (sin contar pardas)
    int ganadas_jugador = 0;
    bool hubo_parda = false;
    std::string ganador_primera = "";   // "ia", "jugador", "parda" o "" si no terminó
    std::optional<Carta> carta_rival;   // la que tiró el jugador en esta ronda, si ya tiró
    std::optional<Carta> carta_propia;  // la que tiró la IA en esta ronda, si ya tiró
    int pts_jugador = 0;                // puntaje del rival en la partida
    // true si ganar ESTA ronda le da la mano a la IA (ej: 1ra parda, o 3ra ronda)
    bool ganar_esta_ronda_gana_la_mano = false;
    int puntos_objetivo = 30;           // 15 o 30
    int valor_falta = 30;               // cuánto vale HOY la falta envido
    // Al jugador le falta 1 punto para ganar: cualquier "no quiero" de la IA
    // le da la partida, así que la IA no rechaza nada y siempre sube
    bool rival_a_un_punto = false;
};

// Clase IA hereda de Jugador e implementa el sistema de toma de decisiones
class IA : public Jugador {
public:
    IA();

    // ENVIDO
    // Decide si abre el envido: "Envido", "Real Envido", "Falta Envido" o "nada"
    std::string decidirCantoEnvido(const ContextoIA& ctx);
    // Responde a una cadena de envido. Retorna "Quiero", "No quiero", "Flor"
    // o uno de los cantos de "subidas_posibles" para subir la apuesta.
    // "pts_si_no_quiero": lo que cuesta rechazar esta cadena.
    std::string responderEnvido(const std::vector<std::string>& cadena,
                                const std::vector<std::string>& subidas_posibles,
                                bool puede_cantar_flor, int pts_si_no_quiero,
                                const ContextoIA& ctx);

    // FLOR: si le cantan flor y también tiene, ¿canta contraflor?
    bool decidirContraflor(const ContextoIA& ctx);

    // TRUCO
    bool decidirCantarTruco(const ContextoIA& ctx);
    // Responde a un truco/retruco/vale4 (nivel 1/2/3).
    // Retorna "quiero", "no_quiero" o "subir" (quiero y retruco / vale cuatro).
    std::string responderTruco(int nivel, const ContextoIA& ctx);

    // CARTAS: índice de la carta a tirar
    int elegirCarta(const ContextoIA& ctx);

private:
    std::mt19937 generador;

    // Promedio de jerarquía de las cartas en mano (+ la que tiró, si se pasa)
    double evaluarFuerzaMedia(const std::optional<Carta>& carta_en_mesa = std::nullopt);
    // Tabla base: cuanto más fuerte la mano, más probable cantar/aceptar truco
    double probabilidadPorFuerza(double fuerza);
    double probabilidadAceptarEnvido(int tanto, const std::string& canto);
    // ¿La falta envido vale más que un Real Envido? Si no, no le sale a cuenta cantarla
    bool faltaLeSaleACuenta(const ContextoIA& ctx);
    // Probabilidad base de truco (la comparten cantar y responder)
    double probabilidadTruco(const ContextoIA& ctx);
    bool leGanaA(const Carta& carta_rival);
    // Tu carta está en la mesa, la IA tiene con qué ganarle y eso le da la mano
    bool tieneLaManoAsegurada(const ContextoIA& ctx);
    int indiceMasFuerte();
    int indiceMasDebil();
    int indiceDelMedio();

    // Tira un dado de probabilidad: retorna true con la probabilidad dada (0.0 a 1.0)
    bool tirarDado(double probabilidad);
};

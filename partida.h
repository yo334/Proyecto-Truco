#pragma once
#include <string>
#include <vector>
#include <utility>
#include "jugador.h"
#include "ia.h"
#include "mazo.h"
#include "ronda.h"

// Todo lo que dura UNA mano y tiene que arrancar de cero en la siguiente.
// Está junto en un struct para que reiniciarlo sea una sola línea:
// antes cada variable se inicializaba en el constructor y nunca se reseteaba
// (por eso la mano siguiente arrancaba ofreciendo "Retruco").
struct EstadoMano {
    int nivel_truco = 0;                // 0=sin, 1=truco, 2=retruco, 3=vale4
    std::string quien_canto_truco = ""; // "jugador", "ia" o "" (el otro es el que puede subir)
    bool envido_cerrado = false;        // true cuando ya no se puede cantar envido
    bool flor_cerrada = false;          // true cuando ya no se puede cantar flor
    std::vector<std::string> cadena_envido;      // ej: {"Envido", "Envido", "Real Envido"}
    std::vector<std::string> resultados_rondas;  // "jugador", "ia" o "parda" por ronda
    Ronda ronda_actual;                          // las cartas en la mesa ahora
};

class Partida {
public:
    Partida();

    // Inicia y corre el loop completo de la partida
    void iniciar();

    // ---- Reglas puras (static: no dependen de una partida en curso) ----
    // Son públicas para poder probarlas en pruebas.cpp sin jugar.

    // Puntos de una cadena de envido. "falta" = lo que vale la falta envido.
    static int puntosEnvido(const std::vector<std::string>& cadena, bool querido,
                            int falta);
    // Qué cantos de envido se pueden agregar a la cadena (máx 2 envidos, 1 real, 1 falta)
    static std::vector<std::string> cantosEnvidoPosibles(
        const std::vector<std::string>& cadena);
    // Cuánto vale la falta envido (SDD v1.1, 3.1)
    static int calcularFalta(int pts_jugador, int pts_ia, int objetivo,
                             bool a_buenas);
    // Quién gana la mano según los resultados de las rondas jugadas.
    // Retorna "jugador", "ia" o "" si todavía no se definió.
    static std::string ganadorDeLaMano(const std::vector<std::string>& resultados,
                                       bool jugador_es_mano);

private:
    Jugador jugador;
    IA ia;
    Mazo mazo;
    int puntos_objetivo;        // 15 o 30
    bool con_flor;
    bool falta_a_buenas;        // true: la falta envido es "a las buenas" (solo a 30)
    bool turno_es_mano_jugador; // true si el jugador humano es mano

    EstadoMano estado; // se reinicia al empezar cada mano

    // Deja el estado de la mano como nuevo
    void reiniciarEstadoMano();

    // Juega una mano completa (reparte, juega las rondas, cuenta puntos)
    void jugarMano();

    // Reparte 3 cartas a cada uno
    void repartirCartas();

    // Juega las rondas de cartas (hasta 3). Retorna "jugador", "ia", "mazo_jugador" o ""
    std::string jugarRondas();

    // Turnos: cada uno canta lo que puede y después tira su carta.
    // Retornan "" si se tiró carta, o cómo terminó la mano.
    std::string turnoJugador(int ronda);
    std::string turnoIA(int ronda);

    // Resuelve una cadena de envido que empieza "quien_canta" con "canto".
    // Retorna pair<pts_jugador, pts_ia>
    std::pair<int,int> resolverEnvido(const std::string& quien_canta,
                                      const std::string& canto);
    std::pair<int,int> compararEnvido(int pts);

    // Resuelve la flor cantada por "quien_canta" (con contraflor si el otro tiene)
    std::pair<int,int> resolverFlor(const std::string& quien_canta);
    bool puedeCantarFlor(const std::string& quien);

    // Resuelve un canto de truco (con subidas). Retorna "quiero" si quedó
    // querido, o cómo terminó la mano: "jugador", "ia" o "mazo_jugador".
    std::string resolverTruco(const std::string& origen);

    // Lo que la IA ve de la mano en este momento
    ContextoIA armarContextoIA();

    // "Ganada por Vos" / "Ganada por IA" / "Parda" para mostrar en pantalla
    std::vector<std::string> historialRondas();

    // ¿Todavía se puede cantar "el envido está primero" contestando un truco?
    bool envidoEstaPrimero(const std::string& responde);

    // Muestra el canto de la IA en grande + su frase de pique
    void anunciarCantoIA(const std::string& canto);

    // Suma puntos y retorna true si terminó la partida
    bool sumarYVerSiTermino(std::pair<int,int> pts);

    // Puntos de la mano según el nivel de truco querido
    int puntosTrucoActual();

    // Retorna true si alguien llegó al puntaje objetivo
    bool terminoPartida();
};

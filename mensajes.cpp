#include "mensajes.h"
#include <vector>
#include <random>
#include <map>

using namespace std;

static random_device rd;
static mt19937 generador(rd());

string Mensajes::obtenerPique(const string& situacion) {
    static map<string, vector<string>> frases = {
        {"canto_envido", {"Envido, papá!", "Envido... si te animás", "Dale, envido", "Envido, a ver qué tenés", "Te canto envido, fiera"}},
        {"canto_truco", {"Truco! A ver qué tenés", "Truco, macho!", "Trucazo, dale", "Te juego un truco", "Vamos con un truco, che"}},
        {"acepto", {"Quiero! Vení tranquilo", "Dale, quiero", "Quiero... vas a ver", "Venga, quiero", "Acepto, sin miedo"}},
        {"rechazo", {"No quiero, paso...", "Me voy al mazo", "Esta vez no, jefe", "Paso, no me da", "No, al mazo"}},
        {"gano_ronda", {"Tomá! Esa la gané", "Jajá, esa me la llevo", "Plop, mi ronda", "Adentro, gano yo", "Esa es mía"}},
        {"perdi_ronda", {"Bien jugada, loco", "Uf, esa dolió", "Bueno, quedan cartas", "Me ganaste esta", "Qué suerte, che"}},
        {"gano_partida", {"Soy el campeón!", "Se acabó, capo. Otra más?", "Te gané, fiera", "A casa, pa", "Victoria total, papá!"}}
    };

    if (frases.count(situacion)) {
        const auto& opciones = frases[situacion];
        uniform_int_distribution<> dis(0, opciones.size() - 1);
        return opciones[dis(generador)];
    }
    return "Mmm...";
}

string Mensajes::bienvenida() {
    return "¡Bienvenido al Truco! Preparate para perder, papá.";
}

string Mensajes::ganadorPartida(const string& nombre) {
    return "¡El ganador indiscutible es: " + nombre + "!";
}

/*
 * Modo Tutor:
 * Esta clase `Mensajes` encapsula toda la lógica de los diálogos y "piques" de la IA y el juego. 
 * Se diseñó utilizando métodos estáticos para que puedan ser llamados sin instanciar la clase, funcionando como utilidades.
 * Concepto de C++: Uso de `std::map` para asociar claves (situaciones) con valores (vectores de frases), 
 * y `std::mt19937` para una generación de números aleatorios eficiente y de calidad de la librería `<random>`.
 */

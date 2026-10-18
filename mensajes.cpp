#include "mensajes.h"
#include <map>
#include <random>
#include <string>
#include <vector>

using namespace std;

static random_device rd;
static mt19937 generador(rd());

string Mensajes::obtenerPique(const string &situacion) {
  // Regla: toda frase de un canto NOMBRA el canto, así nunca hay duda de
  // qué cantó la IA (antes había frases vacías y truco/retruco se confundían).
  static map<string, vector<string>> frases = {
      {"canto_envido",
       {"¡Envido, papá!", "Envido... a ver si te animás", "Dale, envido",
        "Envido, a ver qué tenés", "Te canto envido, ¿qué hacés?"}},
      {"canto_real_envido",
       {"¡Real Envido, papá!", "Real Envido... si te animás",
        "Dale, Real Envido", "Te subo a Real Envido"}},
      {"canto_falta_envido",
       {"¡Falta Envido, carajo!", "Falta Envido... a ver qué tenés",
        "Todo adentro, Falta Envido"}},
      {"canto_truco",
       {"¡Truco! A ver qué tenés", "¡Truco, macho!",
        "Truco, Truquito y Trucazo, dale", "Te juego un truco",
        "Vamos con un truco, che"}},
      {"canto_retruco",
       {"¡Retruco, wachín!", "Retruco, no te me arrugues papá",
        "Re, re, reeeeetruco", "Te lo subo: ¡Retruco!"}},
      {"canto_valecuatro",
       {"¡Vale Cuatro, papá!", "Vale Cuatro, jugate", "Dale, Vale Cuatro, a ver",
        "Vale Cuatro, si te animás"}},
      {"canto_flor",
       {"¡Flor! Tres del mismo palo, papá", "Flor, y achicate",
        "Te canto Flor, ¿qué hacés?"}},
      {"canto_contraflor",
       {"¡Contraflor! A ver qué flor tenés", "Contraflor, vamos a ver",
        "Yo también tengo: ¡Contraflor!"}},
      {"me_achico",
       {"Con flor me achico", "Me achico, llevátela", "Me achico esta vez"}},
      {"acepto",
       {"¡Quiero! Vení tranquilo", "Dale, quiero", "Quiero... vas a ver",
        "Venga, quiero", "Acepto, sin miedo"}},
      {"rechazo",
       {"No quiero, paso...", "Esta vez no, rey", "Paso, no me da",
        "No quiero, llevátelo"}},
      {"gano_ronda",
       {"¡Tomá! Esa la gané", "Jajá, esa me la llevo", "Plop, mi ronda",
        "Adentro, gano yo", "Esa es mía"}},
      {"perdi_ronda",
       {"Bien jugada, loco", "Uf, esa dolió", "Bueno, quedan cartas",
        "Me ganaste esta", "Qué suerte, che"}},
      {"gano_partida",
       {"¡Soy el campeón!", "Se acabó, capo. ¿Otra más?", "Te gané, fiera",
        "A casa, pa", "¡Victoria total, papá!"}}};

  if (frases.count(situacion)) {
    const auto &opciones = frases[situacion];
    uniform_int_distribution<> dis(0, opciones.size() - 1);
    return opciones[dis(generador)];
  }
  return "Mmm...";
}

/*
 * Esta clase `Mensajes` encapsula toda la lógica de los diálogos y "piques" de
 * la IA y el juego. Se diseñó utilizando métodos estáticos para que puedan ser
 * llamados sin instanciar la clase, funcionando como utilidades. Concepto de
 * C++: Uso de `map` para asociar claves (situaciones) con valores (vectores de
 * frases), y `mt19937` para una generación de números aleatorios eficiente y de
 * calidad de la librería `<random>`.
 */

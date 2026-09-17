#include "ia.h"
#include <algorithm>
#include <chrono>
#include <iostream>

using namespace std;

IA::IA() : Jugador("Rival") {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  generador = mt19937(seed);
}

bool IA::tirarDado(double probabilidad) {
  uniform_real_distribution<double> distribucion(0.0, 1.0);
  return distribucion(generador) < probabilidad;
}

double IA::evaluarFuerzaMedia() {
  int suma = 0;
  int cantidad = cantidadCartasEnMano();
  if (cantidad == 0)
    return 14.0;
  for (const auto &c : obtenerMano()) {
    suma += c.obtenerJerarquia();
  }
  return static_cast<double>(suma) / cantidad;
}

string IA::decidirCantoEnvido() {
  int tanto = calcularEnvido();
  double p_envido = 0, p_real = 0, p_falta = 0;

  if (tanto <= 20) {
    p_envido = 0.15;
    p_real = 0.10;
    p_falta = 0.05;
  } else if (tanto <= 25) {
    p_envido = 0.40;
    p_real = 0.10;
    p_falta = 0.05;
  } else if (tanto <= 28) {
    p_envido = 0.75;
    p_real = 0.40;
    p_falta = 0.15;
  } else if (tanto <= 31) {
    p_envido = 0.90;
    p_real = 0.60;
    p_falta = 0.25;
  } else { // 32-33
    p_envido = 0.95;
    p_real = 0.80;
    p_falta = 0.50;
  }

  if (tirarDado(p_falta))
    return "Falta Envido";
  if (tirarDado(p_real))
    return "Real Envido";
  if (tirarDado(p_envido))
    return "Envido";
  return "nada";
}

bool IA::decidirAceptarEnvido(const string &canto) {
  int tanto = calcularEnvido();
  double p_aceptar = 0;

  if (tanto <= 23) {
    if (canto == "Envido")
      p_aceptar = 0.40;
    else if (canto == "Real Envido")
      p_aceptar = 0.20;
    else if (canto == "Falta Envido")
      p_aceptar = 0.10;
  } else if (tanto <= 27) {
    if (canto == "Envido")
      p_aceptar = 0.65;
    else if (canto == "Real Envido")
      p_aceptar = 0.50;
    else if (canto == "Falta Envido")
      p_aceptar = 0.25;
  } else if (tanto <= 30) {
    if (canto == "Envido")
      p_aceptar = 1.0;
    else if (canto == "Real Envido")
      p_aceptar = 0.80;
    else if (canto == "Falta Envido")
      p_aceptar = 0.50;
  } else { // 31-33
    if (canto == "Envido")
      p_aceptar = 1.0;
    else if (canto == "Real Envido")
      p_aceptar = 0.95;
    else if (canto == "Falta Envido")
      p_aceptar = 0.75;
  }

  return tirarDado(p_aceptar);
}

bool IA::decidirCantarTruco(int ronda_actual, int rondas_ganadas_ia,
                            int rondas_ganadas_jugador, bool hubo_parda,
                            int pts_ia, int pts_jugador) {
  // Para silenciar warnings de variables sin uso en logica básica
  (void)ronda_actual;

  // Casos especiales
  if (rondas_ganadas_ia == 2) {
    return tirarDado(0.15); // Si ya ganó, tira farol o no canta
  }

  int cantidad_cartas = cantidadCartasEnMano();
  double fuerza = evaluarFuerzaMedia();

  // Farol con una carta mala
  if (cantidad_cartas == 1 && fuerza >= 11.1) {
    if (tirarDado(0.10))
      return true;
  }

  double factor_a = 0.0;
  if (fuerza <= 3.0)
    factor_a = 0.95;
  else if (fuerza <= 5.5)
    factor_a = 0.75;
  else if (fuerza <= 8.0)
    factor_a = 0.45;
  else if (fuerza <= 11.0)
    factor_a = 0.25;
  else
    factor_a = 0.10;

  double factor_b = 0.0; // Modificadores
  if (rondas_ganadas_ia > rondas_ganadas_jugador)
    factor_b += 0.20;
  else if (rondas_ganadas_jugador > rondas_ganadas_ia)
    factor_b -= 0.15;

  if (hubo_parda)
    factor_b += 0.30;

  if (pts_ia > pts_jugador)
    factor_b += 0.05;
  else if (pts_jugador > pts_ia)
    factor_b += 0.10; // más desesperado

  double prob_final = factor_a + factor_b;
  prob_final = max(0.0, min(1.0, prob_final));

  return tirarDado(prob_final);
}

bool IA::decidirAceptarTruco(int nivel) {
  double fuerza = evaluarFuerzaMedia();
  double base = 0;
  if (fuerza <= 3.0)
    base = 0.95;
  else if (fuerza <= 5.5)
    base = 0.75;
  else if (fuerza <= 8.0)
    base = 0.45;
  else if (fuerza <= 11.0)
    base = 0.25;
  else
    base = 0.10;

  // Reducir la probabilidad según qué tan picante se pone
  if (nivel == 2)
    base -= 0.15; // retruco
  if (nivel == 3)
    base -= 0.30; // vale cuatro

  base = max(0.0, base);
  return tirarDado(base);
}

int IA::elegirCarta(int rondas_ganadas_ia, int rondas_ganadas_jugador) {
  (void)rondas_ganadas_ia;
  (void)rondas_ganadas_jugador;

  // Estrategia básica: elegir la mejor carta si vamos perdiendo,
  // o una baja si ya vamos ganando.
  // Como simplificación rápida, tiramos la más fuerte siempre (índice con menor
  // valor de jerarquía)
  int mejor_indice = 0;
  int mejor_jerarquia = 15;
  const auto &mano = obtenerMano();

  for (size_t i = 0; i < mano.size(); ++i) {
    if (mano[i].obtenerJerarquia() < mejor_jerarquia) {
      mejor_jerarquia = mano[i].obtenerJerarquia();
      mejor_indice = static_cast<int>(i);
    }
  }
  return mejor_indice;
}

string IA::obtenerFraseDePique(const string &situacion) {
  vector<string> frases;
  if (situacion == "canto_envido") {
    frases = {"¡Envido, papá!", "Envido... si te animás", "Dale, envido"};
  } else if (situacion == "canto_real_envido") {
    frases = {"¡Real Envido, papá!", "Real Envido... si te animás",
              "Dale, Real Envido"};
  } else if (situacion == "canto_falta_envido") {
    frases = {"¡Falta Envido, carajo!", "Falta Envido... a ver qué tenés",
              "Todo adentro, Falta Envido"};
  } else if (situacion == "canto_truco") {
    frases = {"¡Truco! A ver qué tenés", "¡Truco, macho!", "Trucazo, dale"};
  } else if (situacion == "acepto") {
    frases = {"¡Quiero! Vení tranquilo", "Dale, quiero", "Quiero... vas a ver"};
  } else if (situacion == "rechazo") {
    frases = {"No quiero, paso...", "Me voy al mazo", "Esta vez no, jefe"};
  } else if (situacion == "gano_ronda") {
    frases = {"¡Tomá! Esa la gané", "Jajá, esa me la llevo", "Plop, mi ronda"};
  } else if (situacion == "perdi_ronda") {
    frases = {"Bien jugada, loco", "Uf, esa dolió", "Bueno, quedan cartas"};
  } else if (situacion == "gano_partida") {
    frases = {"¡Soy el campeón!", "Se acabó, capo. ¿Otra más?"};
  } else {
    frases = {"¿Qué decís?"};
  }

  uniform_int_distribution<int> dist(0, frases.size() - 1);
  return frases[dist(generador)];
}

/*
 * ¿Qué hace esta clase?
 * La clase IA representa al bot o jugador automático. Hereda de Jugador porque,
 * al fin y al cabo, tiene cartas, puntos y mano, igual que un humano, pero
 * además incorpora lógica para tomar decisiones (cantar, aceptar, jugar).
 * Implementa las reglas del SDD de probabilidad usando generadores aleatorios.
 *
 * ¿Qué concepto de C++ ilustra?
 * Ilustra la Herencia (OOP) al extender las funcionalidades de la clase base
 * `Jugador`. Además usa `<random>` de la STL moderna (`mt19937`) que es el
 * estándar actual y mucho mejor que el clásico `rand()` de C para generar
 * números pseudoaleatorios con una semilla de tiempo precisa.
 */

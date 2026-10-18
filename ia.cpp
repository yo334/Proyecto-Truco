#include "ia.h"
#include <algorithm>
#include <chrono>

using namespace std;

// =============================================================================
// PERILLAS DE CALIBRACIÓN — calibrar jugando (prueba y error)
// Cambiar estos números ajusta qué tan arriesgada es la IA sin tocar la lógica.
// =============================================================================
// Aceptar truco (SDD v1.1, sección 6.2)
// Las cartas ya tiradas NO cuentan: solo las que siguen en juego + cómo va la
// mano en rondas (ej: en la 3ra con 1 ganada y 1 perdida, solo cuenta su
// carta).
constexpr double AJUSTE_VA_GANANDO = 0.25; // lleva más rondas ganadas que vos
constexpr double AJUSTE_VA_PERDIENDO =
    0.25; // lleva menos rondas ganadas que vos
constexpr double PENALIDAD_POR_NIVEL = 0.10; // retruco -0.10, vale cuatro -0.20
constexpr double BONUS_LE_GANA_A_LA_MESA =
    0.15; // ve tu carta y tiene con qué ganarle
constexpr double UMBRAL_SUBIR_TRUCO =
    0.80; // desde acá puede responder "quiero y retruco"
constexpr double PROB_SUBIR_TRUCO = 0.60; // ...y lo hace con esta probabilidad
// Envido (SDD v1.1, sección 6.3)
constexpr int TANTO_PARA_SUBIR_ENVIDO = 28;
constexpr int TANTO_PARA_REAL_ENVIDO = 30;
constexpr int TANTO_PARA_FALTA_ENVIDO = 31;
// Falta envido: si vale esto o menos, no conviene cantarla (un Real vale 3)
constexpr int FALTA_MINIMA_PARA_CANTAR = 4;
// Flor
constexpr int FLOR_PARA_CONTRAFLOR = 30;
// Cartas: jerarquía 1..4 = ancho de espada, ancho de basto, 7 de espada, 7 de
// oro
constexpr int JERARQUIA_TOP = 4;

IA::IA() : Jugador("Rival") {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  generador = mt19937(seed);
}

bool IA::tirarDado(double probabilidad) {
  uniform_real_distribution<double> distribucion(0.0, 1.0);
  return distribucion(generador) < probabilidad;
}

// -----------------------------------------------------------------------------
// Evaluación de la mano
// -----------------------------------------------------------------------------

double IA::evaluarFuerzaMedia(const optional<Carta> &carta_en_mesa) {
  int suma = 0;
  int cantidad = 0;
  for (const auto &c : obtenerMano()) {
    suma += c.obtenerJerarquia();
    cantidad++;
  }
  if (carta_en_mesa) {
    suma += carta_en_mesa->obtenerJerarquia();
    cantidad++;
  }
  if (cantidad == 0)
    return 14.0;
  return static_cast<double>(suma) / cantidad;
}

double IA::probabilidadPorFuerza(double fuerza) {
  if (fuerza <= 3.0)
    return 0.95;
  if (fuerza <= 5.5)
    return 0.75;
  if (fuerza <= 8.0)
    return 0.45;
  if (fuerza <= 11.0)
    return 0.25;
  return 0.10;
}

// ¿Tiene alguna carta que le gane a la del rival? (menor jerarquía = más
// fuerte)
bool IA::leGanaA(const Carta &carta_rival) {
  for (const auto &c : obtenerMano())
    if (c.obtenerJerarquia() < carta_rival.obtenerJerarquia())
      return true;
  return false;
}

// Ejemplo: la 1ra fue parda, tiraste un 12 y la IA tiene un 3 -> gana seguro.
bool IA::tieneLaManoAsegurada(const ContextoIA &ctx) {
  return ctx.ganar_esta_ronda_gana_la_mano && ctx.carta_rival &&
         leGanaA(*ctx.carta_rival);
}

int IA::indiceMasFuerte() {
  const auto &mano = obtenerMano();
  int mejor = 0;
  for (size_t i = 1; i < mano.size(); ++i)
    if (mano[i].obtenerJerarquia() < mano[mejor].obtenerJerarquia())
      mejor = static_cast<int>(i);
  return mejor;
}

int IA::indiceMasDebil() {
  const auto &mano = obtenerMano();
  int peor = 0;
  for (size_t i = 1; i < mano.size(); ++i)
    if (mano[i].obtenerJerarquia() > mano[peor].obtenerJerarquia())
      peor = static_cast<int>(i);
  return peor;
}

// La carta "del medio": ni la más fuerte ni la más débil (con 3 cartas)
int IA::indiceDelMedio() {
  int fuerte = indiceMasFuerte();
  int debil = indiceMasDebil();
  for (int i = 0; i < cantidadCartasEnMano(); ++i)
    if (i != fuerte && i != debil)
      return i;
  return debil;
}

// -----------------------------------------------------------------------------
// ENVIDO
// -----------------------------------------------------------------------------

bool IA::faltaLeSaleACuenta(const ContextoIA &ctx) {
  return ctx.valor_falta >= FALTA_MINIMA_PARA_CANTAR;
}

string IA::decidirCantoEnvido(const ContextoIA &ctx) {
  int tanto = calcularEnvido();
  double p_envido = 0, p_real = 0, p_falta = 0;

  if (tanto <= 20) {
    p_envido = 0.15;
    p_real = 0.10;
    p_falta = 0.05;
  } else if (tanto <= 25) {
    p_envido = 0.40;
    p_real = 0.25;
    p_falta = 0.15;
  } else if (tanto <= 28) {
    p_envido = 0.75;
    p_real = 0.55;
    p_falta = 0.30;
  } else if (tanto <= 31) {
    p_envido = 0.90;
    p_real = 0.60;
    p_falta = 0.25;
  } else { // 32-33
    p_envido = 0.95;
    p_real = 0.80;
    p_falta = 0.50;
  }

  // Si la falta vale poco (ej: al que va ganando le falta 1 para las buenas),
  // cantarla no tiene sentido: arriesga lo mismo que un envido y gana menos
  if (faltaLeSaleACuenta(ctx) && tirarDado(p_falta))
    return "Falta Envido";
  if (tirarDado(p_real))
    return "Real Envido";
  if (tirarDado(p_envido))
    return "Envido";
  return "nada";
}

// Probabilidad de querer según su tanto y el último canto de la cadena
double IA::probabilidadAceptarEnvido(int tanto, const string &canto) {
  if (tanto <= 23) {
    if (canto == "Envido")
      return 0.40;
    if (canto == "Real Envido")
      return 0.20;
    return 0.10; // Falta Envido
  }
  if (tanto <= 27) {
    if (canto == "Envido")
      return 0.65;
    if (canto == "Real Envido")
      return 0.50;
    return 0.25;
  }
  if (tanto <= 30) {
    if (canto == "Envido")
      return 1.0;
    if (canto == "Real Envido")
      return 0.80;
    return 0.50;
  }
  // 31-33
  if (canto == "Envido")
    return 1.0;
  if (canto == "Real Envido")
    return 0.95;
  return 0.75;
}

string IA::responderEnvido(const vector<string> &cadena,
                           const vector<string> &subidas_posibles,
                           bool puede_cantar_flor, int pts_si_no_quiero,
                           const ContextoIA &ctx) {
  // Con flor, la flor le gana al envido (lo anula)
  if (puede_cantar_flor)
    return "Flor";

  auto sePuede = [&](const string &canto) {
    return find(subidas_posibles.begin(), subidas_posibles.end(), canto) !=
           subidas_posibles.end();
  };

  // Al rival le falta 1 punto: un "no quiero" le regala la partida.
  // Nunca rechaza y siempre sube (lo que más puntos le dé si gana).
  if (ctx.rival_a_un_punto) {
    if (sePuede("Real Envido"))
      return "Real Envido";
    if (sePuede("Envido"))
      return "Envido";
    if (sePuede("Falta Envido") && faltaLeSaleACuenta(ctx))
      return "Falta Envido";
    return "Quiero";
  }

  int tanto = calcularEnvido();

  // Con mucho tanto, a veces sube en vez de solo querer
  if (tanto >= TANTO_PARA_FALTA_ENVIDO && sePuede("Falta Envido") &&
      faltaLeSaleACuenta(ctx) && tirarDado(0.5))
    return "Falta Envido";
  if (tanto >= TANTO_PARA_REAL_ENVIDO && sePuede("Real Envido") &&
      tirarDado(0.6))
    return "Real Envido";
  if (tanto >= TANTO_PARA_SUBIR_ENVIDO && sePuede("Envido") && tirarDado(0.4))
    return "Envido";

  double p_aceptar = probabilidadAceptarEnvido(tanto, cadena.back());

  if (cadena.back() == "Falta Envido") {
    // Teoría de juegos ante Falta Envido:
    // 1. Dominancia estricta: si la falta cuesta menos o igual que los puntos
    // acumulados
    //    por rechazar (pts_si_no_quiero), aceptar es la opción dominante
    //    (diciendo "No quiero" se pierden esos puntos de forma garantizada,
    //    mientras que aceptando existe chance de ganar).
    // 2. Modulación por interpolación lineal (Lerp): la falta envido varía
    // según el puntaje.
    //    Calculamos el ratio de riesgo normalizado: riesgo = valor_falta /
    //    puntos_objetivo (rango ~0.0 a 1.0). Interpolamos: p_aceptar =
    //    p_como_envido + (p_aceptar_falta - p_como_envido) * riesgo.
    //    - Si la falta vale poco (partida avanzada, riesgo -> 0), la IA la
    //    trata casi como un Envido común.
    //    - Si la falta vale toda la partida (inicio de juego, riesgo -> 1), se
    //    rige por la tabla conservadora.
    if (ctx.valor_falta <= pts_si_no_quiero)
      return "Quiero";
    double riesgo = static_cast<double>(ctx.valor_falta) / ctx.puntos_objetivo;
    double p_como_envido = probabilidadAceptarEnvido(tanto, "Envido");
    p_aceptar = p_como_envido + (p_aceptar - p_como_envido) * riesgo;
  }

  if (tirarDado(p_aceptar))
    return "Quiero";
  return "No quiero";
}

// -----------------------------------------------------------------------------
// FLOR
// -----------------------------------------------------------------------------

bool IA::decidirContraflor(const ContextoIA &ctx) {
  // Achicarse le daría 3 puntos al rival, que está a 1: siempre contraflor
  if (ctx.rival_a_un_punto)
    return true;
  if (calcularFlor() >= FLOR_PARA_CONTRAFLOR)
    return tirarDado(0.8);
  return tirarDado(0.3);
}

// -----------------------------------------------------------------------------
// TRUCO
// -----------------------------------------------------------------------------

// Probabilidad base de jugarse al truco según el estado ACTUAL de la mano:
// - fuerza de las cartas que siguen en juego (las que tiene + la que tiró
//   en esta ronda, si ya tiró). Las de rondas anteriores ya no importan.
// - cómo va en rondas: si van iguales (0-0 o 1-1) no suma ni resta.
// - si ve tu carta y tiene con qué ganarle.
// La usan TANTO cantar como responder, así nunca se contradicen.
double IA::probabilidadTruco(const ContextoIA &ctx) {
  double prob = probabilidadPorFuerza(evaluarFuerzaMedia(ctx.carta_propia));

  if (ctx.ganadas_ia > ctx.ganadas_jugador)
    prob += AJUSTE_VA_GANANDO;
  else if (ctx.ganadas_jugador > ctx.ganadas_ia)
    prob -= AJUSTE_VA_PERDIENDO;

  if (ctx.carta_rival && leGanaA(*ctx.carta_rival))
    prob += BONUS_LE_GANA_A_LA_MESA;

  return max(0.0, min(1.0, prob));
}

bool IA::decidirCantarTruco(const ContextoIA &ctx) {
  // Si ya la tiene ganada, canta siempre: es sumar puntos gratis.
  // Con el rival a 1 punto también: si no quiere, los puntos son de la IA,
  // y si quiere, la mano ya valía la partida igual.
  if (tieneLaManoAsegurada(ctx) || ctx.rival_a_un_punto)
    return true;

  // Farol con una carta mala
  if (cantidadCartasEnMano() == 1 && evaluarFuerzaMedia() >= 11.1 &&
      tirarDado(0.10))
    return true;

  double prob = probabilidadTruco(ctx);

  // Según el puntaje de la partida
  if (obtenerPuntos() > ctx.pts_jugador)
    prob += 0.05;
  else if (ctx.pts_jugador > obtenerPuntos())
    prob += 0.10; // más desesperado

  prob = max(0.0, min(1.0, prob));
  return tirarDado(prob);
}

string IA::responderTruco(int nivel, const ContextoIA &ctx) {
  // Mano asegurada o rival a 1 punto: NUNCA rechaza, y si puede, sube
  if (tieneLaManoAsegurada(ctx) || ctx.rival_a_un_punto)
    return (nivel < 3) ? "subir" : "quiero";

  double prob = probabilidadTruco(ctx) - PENALIDAD_POR_NIVEL * (nivel - 1);
  prob = max(0.0, min(1.0, prob));

  if (nivel < 3 && prob >= UMBRAL_SUBIR_TRUCO && tirarDado(PROB_SUBIR_TRUCO))
    return "subir";
  if (tirarDado(prob))
    return "quiero";
  return "no_quiero";
}

// Heurística de selección de cartas según posición y ronda (SDD v1.1,
// sección 6.1):
// 1. Tira segunda (responde a una carta rival ya visible en la mesa):
//    - "Matar con lo justo": en la convención de este proyecto, MENOR jerarquía
//    = carta más fuerte
//      (1=As de espadas, 14=Cuatro). Para ganar la baza sin desperdiciar poder
//      de fuego, busca todas las cartas que superen a la del rival (jer <
//      jer_rival) y entre ellas elige la de MAYOR valor numérico (la carta más
//      débil dentro de las ganadoras).
//    - "Empardar en primera": si ninguna carta propia supera a la del rival,
//    pero es la 1ra ronda,
//      busca empatar. Empardar la 1ra traslada la definición completa a la 2da
//      ronda.
//    - "Descarte defensivo": si no puede ganar ni empatar, la baza está
//    irremediablemente perdida;
//      por ende, se descarta de su carta más débil (indiceMasDebil) para
//      conservar sus mejores cartas de cara a las rondas siguientes.
// 2. Tira primera (abre la mano o la ronda):
//    - Ronda 1: si tiene una carta de jerarquía top (ancho de espada/basto o 7
//    de espada/oro, <= 4),
//      la juega para asegurarse la iniciativa; de lo contrario, guarda su mejor
//      carta y juega la del "medio" para tantear la mesa sin regalar cartas
//      altas ni regalar la mano con basura.
//    - Rondas 2 y 3: si ya ganó la 1ra ronda, tiene la ventaja de las pardas a
//    su favor; puede
//      darse el lujo de tirar la peor (indiceMasDebil) para obligar al rival a
//      quemar cartas buenas. Si no ganó la 1ra, juega su carta más fuerte a
//      matar o morir.
int IA::elegirCarta(const ContextoIA &ctx) {
  const auto &mano = obtenerMano();

  // Tira segunda: busca la carta JUSTA para ganar
  if (ctx.carta_rival) {
    int jer_rival = ctx.carta_rival->obtenerJerarquia();

    // De las que le ganan, la más débil (la de mayor jerarquía que siga <
    // rival)
    int elegida = -1;
    for (size_t i = 0; i < mano.size(); ++i) {
      int jer = mano[i].obtenerJerarquia();
      if (jer < jer_rival &&
          (elegida == -1 || jer > mano[elegida].obtenerJerarquia()))
        elegida = static_cast<int>(i);
    }
    if (elegida != -1)
      return elegida;

    // No le gana con ninguna: en la 1ra ronda, empardar es mejor que perder
    if (ctx.ronda == 0)
      for (size_t i = 0; i < mano.size(); ++i)
        if (mano[i].obtenerJerarquia() == jer_rival)
          return static_cast<int>(i);

    // Ronda perdida: no regala una carta buena
    return indiceMasDebil();
  }

  // Tira primera
  if (ctx.ronda == 0) {
    int fuerte = indiceMasFuerte();
    // Con una carta top la juega; si no, guarda la mejor y tira una media
    if (mano[fuerte].obtenerJerarquia() <= JERARQUIA_TOP || mano.size() < 3)
      return fuerte;
    return indiceDelMedio();
  }
  // Rondas 2 y 3: si ya ganó la 1ra, se puede dar el lujo de tirar la peor
  if (ctx.ganador_primera == "ia")
    return indiceMasDebil();
  return indiceMasFuerte();
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
 * `Jugador`. Además usa `<random>` de la STL moderna (`mt19937`) y
 * `constexpr` para las constantes de calibración: son valores que el
 * compilador conoce de antemano, con nombre, en un solo lugar.
 */

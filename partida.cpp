#include "partida.h"
#include "interfaz.h"
#include "mensajes.h"
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

// El rival de "quien" ("jugador" <-> "ia")
static string otro(const string &quien) {
  return quien == "jugador" ? "ia" : "jugador";
}

Partida::Partida()
    : jugador("Jugador"), ia(), mazo(), falta_a_buenas(false),
      turno_es_mano_jugador(true) {
  string nombre;
  while (true) {
    cout << "¡Bienvenido al Truco!\n¿Cuál es tu nombre? ";
    if (cin >> nombre) {
      bool tieneLetras = false;
      for (char c : nombre) {
        if (isalpha(c))
          tieneLetras = true;
      }
      if (tieneLetras)
        break;
      cout << "Che, poné un nombre válido (al menos una letra).\n";
    } else {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
  jugador = Jugador(nombre);

  int pts;
  while (true) {
    cout << "¿A cuántos puntos jugamos? (15/30): ";
    if (cin >> pts && (pts == 15 || pts == 30)) {
      break;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Por favor, ingresá 15 o 30.\n";
  }
  puntos_objetivo = pts;

  string flor;
  while (true) {
    cout << "¿Jugamos con flor? (s/n): ";
    if (cin >> flor &&
        (flor == "s" || flor == "S" || flor == "n" || flor == "N")) {
      break;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Por favor, ingresá 's' o 'n'.\n";
  }
  con_flor = (flor == "s" || flor == "S");

  // A 15 no hay malas y buenas, así que solo se pregunta jugando a 30
  if (puntos_objetivo == 30) {
    string modo;
    while (true) {
      cout << "¿La falta envido es a ganar (g) o a las buenas (b)? ";
      if (cin >> modo &&
          (modo == "g" || modo == "G" || modo == "b" || modo == "B")) {
        break;
      }
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Por favor, ingresá 'g' o 'b'.\n";
    }
    falta_a_buenas = (modo == "b" || modo == "B");
  }
}

void Partida::iniciar() {
  cout << "\n=== COMIENZA LA PARTIDA (" << puntos_objetivo << " puntos) ===\n";
  while (!terminoPartida()) {
    jugarMano();
  }

  if (jugador.obtenerPuntos() >= puntos_objetivo) {
    cout << "\n¡Ganaste la partida! Felicitaciones, " << jugador.obtenerNombre()
         << ".\n";
  } else {
    cout << "\n" << Mensajes::obtenerPique("gano_partida") << "\n";
  }
}

// Cada mano arranca sin cantos: vale 1 hasta que alguien cante truco.
void Partida::reiniciarEstadoMano() { estado = EstadoMano{}; }

void Partida::jugarMano() {
  reiniciarEstadoMano();
  cout << "\n================================= NUEVA MANO "
          "=================================\n";
  Interfaz::mostrarTablero(jugador.obtenerNombre(), jugador.obtenerPuntos(),
                           "IA", ia.obtenerPuntos(), puntos_objetivo);

  mazo.reiniciar();
  repartirCartas();

  string resultado = jugarRondas();

  // Este es el ÚNICO lugar donde se suman los puntos de la mano (truco).
  // Si alguien dijo "no quiero", el nivel no subió, así que
  // puntosTrucoActual() ya es lo que cobra el que cantó.
  if (resultado == "jugador") {
    int pts = puntosTrucoActual();
    jugador.sumarPuntos(pts);
    cout << "\n>> Ganaste la mano (+" << pts << ") <<\n";
  } else if (resultado == "ia") {
    int pts = puntosTrucoActual();
    ia.sumarPuntos(pts);
    cout << "\n>> La IA ganó la mano (+" << pts << ") <<\n";
  } else if (resultado == "mazo_jugador") {
    int pts = puntosTrucoActual();
    // Irse al mazo antes de que se juegue el envido le regala 1 punto más
    if (!estado.envido_cerrado)
      pts += 1;
    ia.sumarPuntos(pts);
    cout << "\n>> Te fuiste al mazo. La IA suma " << pts << " <<\n";
  }
  if (resultado != "")
    Interfaz::pausar();

  jugador.limpiarMano();
  ia.limpiarMano();
  turno_es_mano_jugador = !turno_es_mano_jugador;
}

void Partida::repartirCartas() {
  for (int i = 0; i < 3; ++i) {
    if (turno_es_mano_jugador) {
      jugador.recibirCarta(mazo.repartir());
      ia.recibirCarta(mazo.repartir());
    } else {
      ia.recibirCarta(mazo.repartir());
      jugador.recibirCarta(mazo.repartir());
    }
  }
}

// =============================================================================
// REGLAS PURAS (static)
// =============================================================================

// SDD v1.1, 3.3.
// Cálculo de puntos de envido con resolución recursiva para cantos "no queridos":
// - Si el envido es querido, se suman los valores acumulados en la cadena (Envido=2, Real=3).
//   Si aparece una "Falta Envido", esta anula y reemplaza todos los puntos anteriores.
// - Si es NO querido:
//   1. Con un solo canto (ej: "Envido" -> "No quiero"), el que cantó cobra 1 punto básico.
//   2. Con subidas (ej: "Envido" -> "Real Envido" -> "No quiero"), el que rechaza no regala 1 punto,
//      sino que el rival cobra lo acumulado hasta el canto anterior (lo que ya estaba "pactado").
//      Para resolver esto sin duplicar lógica, se recorta el último canto (sin_ultimo) y se hace
//      una llamada recursiva evaluándolo como querido=true.
int Partida::puntosEnvido(const vector<string> &cadena, bool querido,
                          int falta) {
  if (!querido) {
    if (cadena.size() <= 1)
      return 1;
    vector<string> sin_ultimo(cadena.begin(), cadena.end() - 1);
    return puntosEnvido(sin_ultimo, true, falta);
  }

  int total = 0;
  for (const auto &canto : cadena) {
    if (canto == "Falta Envido")
      return falta; // la falta reemplaza todo lo anterior
    total += (canto == "Real Envido") ? 3 : 2;
  }
  return total;
}

// Máximo 2 Envido (y nunca después de un Real), 1 Real, 1 Falta.
// La Falta siempre cierra la cadena.
vector<string> Partida::cantosEnvidoPosibles(const vector<string> &cadena) {
  int envidos = static_cast<int>(count(cadena.begin(), cadena.end(), "Envido"));
  bool hay_real =
      find(cadena.begin(), cadena.end(), "Real Envido") != cadena.end();
  bool hay_falta =
      find(cadena.begin(), cadena.end(), "Falta Envido") != cadena.end();

  vector<string> posibles;
  if (hay_falta)
    return posibles;
  if (envidos < 2 && !hay_real)
    posibles.push_back("Envido");
  if (!hay_real)
    posibles.push_back("Real Envido");
  posibles.push_back("Falta Envido");
  return posibles;
}

// SDD v1.1, 3.1 (decisión D1: se cuenta lo que le falta al puntero / que va ganando).
// En el truco argentino tradicional, la Falta Envido no tiene un puntaje fijo:
// otorga los puntos necesarios para que el jugador que va liderando alcance la meta.
// Si se juega a 30 con modalidad "a las buenas" y el puntero todavía está en "las malas"
// (menos de 15 puntos), la falta solo otorga los puntos para entrar a las buenas (15 - lider),
// en vez de liquidar la partida completa de un solo golpe.
int Partida::calcularFalta(int pts_jugador, int pts_ia, int objetivo,
                           bool a_buenas) {
  int lider = max(pts_jugador, pts_ia);
  if (a_buenas && objetivo == 30 && lider < 15)
    return 15 - lider; // el que va ganando está en las malas: hasta las buenas
  return objetivo - lider;
}

// Reglas de pardas (SDD v1.0, 3.6):
// La resolución de la mano con empates (pardas) es asimétrica y depende del orden de las rondas:
// - Condición base: el primero en ganar 2 rondas se lleva la mano de inmediato.
// - 1ra ronda parda: la 2da ronda pasa a ser decisiva directa. Si alguien la gana, se lleva la
//   mano sin necesidad de jugar la 3ra.
// - 1ra ganada por X y 2da parda: gana X automáticamente, porque ganar la primera ronda otorga
//   la ventaja de desempate en caso de parda posterior.
// - 1ra parda, 2da parda: define la 3ra ronda.
// - 1ra ganada por X, 2da ganada por Y, y 3ra parda: gana X por haber ganado la 1ra ronda.
// - Tres pardas consecutivas ("triple parda"): la mano se la adjudica quien inició siendo "mano".
// Si aún no se dan estas condiciones de cierre, retorna "" para indicar que la mano debe continuar.
string Partida::ganadorDeLaMano(const vector<string> &resultados,
                                bool jugador_es_mano) {
  int ganadas_j =
      static_cast<int>(count(resultados.begin(), resultados.end(), "jugador"));
  int ganadas_i =
      static_cast<int>(count(resultados.begin(), resultados.end(), "ia"));
  if (ganadas_j == 2)
    return "jugador";
  if (ganadas_i == 2)
    return "ia";

  size_t n = resultados.size();
  if (n >= 2) {
    if (resultados[0] == "parda" && resultados[1] != "parda")
      return resultados[1]; // la 1ra fue parda: define la 2da
    if (resultados[0] != "parda" && resultados[1] == "parda")
      return resultados[0]; // parda después de ganar la 1ra: gana el de la 1ra
  }
  if (n == 3) {
    if (resultados[2] != "parda")
      return resultados[2]; // 1ra y 2da pardas: define la 3ra
    if (resultados[0] != "parda")
      return resultados[0]; // 3ra parda: gana el que ganó la 1ra
    return jugador_es_mano ? "jugador" : "ia"; // triple parda
  }
  return ""; // todavía no se sabe
}

// =============================================================================
// ENVIDO
// =============================================================================

// Muestra los dos tantos y le da los puntos al que gana.
// Si empatan, gana el que es mano.
pair<int, int> Partida::compararEnvido(int pts) {
  int tanto_jugador = jugador.calcularEnvido();
  int tanto_ia = ia.calcularEnvido();
  cout << "Tu tanto: -" << tanto_jugador << "-\n";
  cout << "Tanto IA: -" << tanto_ia << "-\n";

  bool gana_jugador = tanto_jugador > tanto_ia ||
                      (tanto_jugador == tanto_ia && turno_es_mano_jugador);
  if (gana_jugador) {
    cout << ">> Ganaste el envido (+" << pts << ") <<\n";
    return {pts, 0};
  }
  cout << ">> La IA ganó el envido (+" << pts << ") <<\n";
  return {0, pts};
}

// Resuelve el diálogo interactivo del envido mediante un bucle de alternancia de turnos:
// - quien_canta abre la apuesta inicial y queda registrado en estado.cadena_envido.
// - Cada iteración evalúa las subidas válidas (ej: máximo 2 envidos, nunca un envido después de un real).
// - Si el rival canta "Flor", esta tiene prioridad reglamentaria: anula la cadena de envido previa
//   (estado.cadena_envido.clear()) y pasa a resolverse la flor directamente.
// - Si el rival dice "No quiero", se calculan los puntos de penalización usando la cadena actual
//   y la función recursiva puntosEnvido(..., false, falta).
// - Si el rival sube la apuesta, la subida se apila en la cadena y se invierten los roles (canta = responde).
pair<int, int> Partida::resolverEnvido(const string &quien_canta,
                                       const string &canto) {
  estado.envido_cerrado = true;
  estado.cadena_envido = {canto};
  string canta = quien_canta;

  while (true) {
    const string ultimo = estado.cadena_envido.back();
    if (canta == "jugador")
      cout << ">> Cantaste: " << ultimo << " <<\n";
    else
      anunciarCantoIA(ultimo);

    string responde = otro(canta);
    vector<string> subidas = cantosEnvidoPosibles(estado.cadena_envido);
    string respuesta;

    if (responde == "ia") {
      int falta_hoy = calcularFalta(jugador.obtenerPuntos(), ia.obtenerPuntos(),
                                    puntos_objetivo, falta_a_buenas);
      respuesta = ia.responderEnvido(
          estado.cadena_envido, subidas, puedeCantarFlor("ia"),
          puntosEnvido(estado.cadena_envido, false, falta_hoy),
          armarContextoIA());
    } else {
      vector<string> opciones = {"Quiero", "No quiero"};
      vector<string> valores = {"Quiero", "No quiero"};
      for (const auto &s : subidas) {
        opciones.push_back("Cantar " + s);
        valores.push_back(s);
      }
      if (puedeCantarFlor("jugador")) {
        opciones.push_back("Cantar Flor");
        valores.push_back("Flor");
      }
      respuesta = valores[Interfaz::pedirOpcion(opciones)];
    }

    int falta = calcularFalta(jugador.obtenerPuntos(), ia.obtenerPuntos(),
                              puntos_objetivo, falta_a_buenas);

    if (respuesta == "Quiero") {
      if (responde == "ia")
        cout << "La IA dice: " << Mensajes::obtenerPique("acepto") << "\n";
      return compararEnvido(puntosEnvido(estado.cadena_envido, true, falta));
    }
    if (respuesta == "No quiero") {
      if (responde == "ia")
        cout << "La IA dice: " << Mensajes::obtenerPique("rechazo") << "\n";
      int pts = puntosEnvido(estado.cadena_envido, false, falta);
      if (canta == "jugador") {
        cout << ">> Ganaste el envido (+" << pts << ") <<\n";
        return {pts, 0};
      }
      cout << ">> La IA ganó el envido (+" << pts << ") <<\n";
      return {0, pts};
    }
    if (respuesta == "Flor") {
      // La flor anula todo lo cantado de envido
      estado.cadena_envido.clear();
      return resolverFlor(responde);
    }

    // Subió: se agrega a la cadena y ahora responde el otro
    estado.cadena_envido.push_back(respuesta);
    canta = responde;
  }
}

// =============================================================================
// FLOR
// =============================================================================

bool Partida::puedeCantarFlor(const string &quien) {
  if (!con_flor || estado.flor_cerrada)
    return false;
  if (quien == "jugador")
    return jugador.tieneFlor();
  return ia.tieneFlor();
}

// "quien_canta" canta flor. Si el otro no tiene: 3 puntos. Si tiene, puede
// cantar Contraflor (se comparan las flores, el ganador cobra 6) o achicarse
// (el que cantó cobra 3). La flor anula el envido.
pair<int, int> Partida::resolverFlor(const string &quien_canta) {
  estado.flor_cerrada = true;
  estado.envido_cerrado = true;

  if (quien_canta == "jugador")
    cout << ">> Cantaste: Flor <<\n";
  else
    anunciarCantoIA("Flor");

  string responde = otro(quien_canta);
  bool responde_tiene =
      (responde == "jugador") ? jugador.tieneFlor() : ia.tieneFlor();
  pair<int, int> tres_al_que_canto =
      (quien_canta == "jugador") ? make_pair(3, 0) : make_pair(0, 3);

  if (!responde_tiene) {
    cout << (quien_canta == "jugador" ? "La IA no tiene flor. Sumás 3.\n"
                                      : "No tenés flor. La IA suma 3.\n");
    return tres_al_que_canto;
  }

  bool contraflor;
  if (responde == "jugador") {
    contraflor = Interfaz::pedirOpcion({"Cantar Contraflor", "Me achico"}) == 0;
    if (contraflor)
      cout << ">> Cantaste: Contraflor <<\n";
  } else {
    contraflor = ia.decidirContraflor(armarContextoIA());
    if (contraflor)
      anunciarCantoIA("Contraflor");
    else
      cout << "La IA dice: " << Mensajes::obtenerPique("me_achico") << "\n";
  }

  if (!contraflor) {
    cout << (quien_canta == "jugador" ? "Sumás 3 por la flor.\n"
                                      : "La IA suma 3 por la flor.\n");
    return tres_al_que_canto;
  }

  // Contraflor: 20 + las 3 cartas (figuras valen 0). Empate: gana el mano.
  int flor_jugador = jugador.calcularFlor();
  int flor_ia = ia.calcularFlor();
  cout << "Tu flor: -" << flor_jugador << "-\n";
  cout << "Flor IA: -" << flor_ia << "-\n";
  bool gana_jugador = flor_jugador > flor_ia ||
                      (flor_jugador == flor_ia && turno_es_mano_jugador);
  if (gana_jugador) {
    cout << ">> Ganaste la contraflor (+6) <<\n";
    return {6, 0};
  }
  cout << ">> La IA ganó la contraflor (+6) <<\n";
  return {0, 6};
}

// =============================================================================
// TRUCO
// =============================================================================

// Resuelve la negociación escalonada de truco (Truco -> Retruco -> Vale Cuatro):
// - Maneja el diálogo interactivo donde los roles 'canta' y 'responde' se van alternando.
// - Interrupción reglamentaria "El envido está primero":
//   Si en la primera ronda el rival canta truco antes del envido, quien debe responder puede
//   priorizar cantar envido. En ese caso, se suspende la negociación del truco, se resuelve
//   el envido por completo, se suman los puntos y se verifica si alguien ya ganó el partido.
//   Si la partida sigue, el diálogo del truco se reanuda en el mismo nivel donde quedó (con continue
//   y el flag anunciar=false para no volver a imprimir el canto anterior).
// - Si quien responde rechaza ("No quiero"), el nivel no sube y la mano la gana quien cantó último.
// - Si acepta ("Quiero"), el nivel se consolida en estado.nivel_truco y cierra definitivamente el envido.
string Partida::resolverTruco(const string &origen) {
  string cantos[] = {"", "Truco", "Retruco", "Vale Cuatro"};
  string canta = origen;

  bool anunciar = true;

  while (true) {
    int nuevo_nivel = estado.nivel_truco + 1;
    if (anunciar) {
      if (canta == "jugador")
        cout << ">> Cantaste: " << cantos[nuevo_nivel] << " <<\n";
      else
        anunciarCantoIA(cantos[nuevo_nivel]);
    }
    anunciar = true;

    string responde = otro(canta);
    string respuesta; // "quiero", "no_quiero", "subir", "mazo" o un envido

    if (responde == "ia") {
      // El envido está primero: con flor o buen tanto, la IA canta antes
      if (envidoEstaPrimero("ia")) {
        if (puedeCantarFlor("ia")) {
          cout << "La IA dice: ¡El envido está primero!\n";
          if (sumarYVerSiTermino(resolverFlor("ia")))
            return "fin_partida";
          cout << "\nSeguimos con el " << cantos[nuevo_nivel] << ":\n";
        } else {
          string canto = ia.decidirCantoEnvido(armarContextoIA());
          if (canto != "nada") {
            cout << "La IA dice: ¡El envido está primero!\n";
            if (sumarYVerSiTermino(resolverEnvido("ia", canto)))
              return "fin_partida";
            cout << "\nSeguimos con el " << cantos[nuevo_nivel] << ":\n";
          }
        }
      }
      respuesta = ia.responderTruco(nuevo_nivel, armarContextoIA());
      if (respuesta == "no_quiero")
        cout << "La IA dice: " << Mensajes::obtenerPique("rechazo") << "\n";
      else
        cout << "La IA dice: " << Mensajes::obtenerPique("acepto") << "\n";
    } else {
      vector<string> opciones = {"Quiero", "No quiero"};
      if (nuevo_nivel < 3)
        opciones.push_back("Quiero y " + cantos[nuevo_nivel + 1]);
      // El envido está primero: podés contestar el truco con un envido o flor
      if (envidoEstaPrimero("jugador")) {
        if (puedeCantarFlor("jugador"))
          opciones.push_back("Cantar Flor (el envido está primero)");
        for (const auto &c : cantosEnvidoPosibles({}))
          opciones.push_back("Cantar " + c + " (el envido está primero)");
      }
      opciones.push_back("Irme al mazo");
      string elegida = opciones[Interfaz::pedirOpcion(opciones)];

      if (elegida.find("el envido está primero") != string::npos) {
        if (elegida.find("Flor") != string::npos) {
          if (sumarYVerSiTermino(resolverFlor("jugador")))
            return "fin_partida";
        } else {
          // "Cantar Real Envido (el envido...)" -> "Real Envido"
          string canto = elegida.substr(string("Cantar ").size());
          canto = canto.substr(0, canto.find(" ("));
          if (sumarYVerSiTermino(resolverEnvido("jugador", canto)))
            return "fin_partida";
        }
        cout << "\nSeguimos con el " << cantos[nuevo_nivel] << ":\n";
        anunciar = false; // no repetir el canto, solo volver a preguntar
        continue;
      }

      if (elegida == "Quiero")
        respuesta = "quiero";
      else if (elegida == "No quiero")
        respuesta = "no_quiero";
      else if (elegida == "Irme al mazo")
        respuesta = "mazo";
      else
        respuesta = "subir";
    }

    // No quiso: el nivel no sube y la mano la gana el que cantó
    if (respuesta == "no_quiero")
      return canta;
    if (respuesta == "mazo")
      return "mazo_jugador";

    // Quiso (y tal vez sube)
    estado.nivel_truco = nuevo_nivel;
    estado.quien_canto_truco = canta;
    // Al querer el truco sin haber cantado envido, el envido se pierde
    estado.envido_cerrado = true;

    if (respuesta == "quiero")
      return "quiero";
    canta = responde; // "Quiero y retruco": ahora canta el que respondió
  }
}

// Regla "El envido está primero": al contestar un truco, cantar envido solo es válido si:
// 1. Es la 1ra ronda (estado.resultados_rondas está vacío).
// 2. El envido no fue cerrado previamente (nadie lo jugó ni se fue al mazo).
// 3. Es el primer nivel de truco ofrecido (nadie lo quiso todavía, estado.nivel_truco == 0).
bool Partida::envidoEstaPrimero(const string &responde) {
  (void)responde;
  return estado.resultados_rondas.empty() && !estado.envido_cerrado &&
         estado.nivel_truco == 0;
}

// Puntos que vale la mano según el truco ya querido (1 si no se cantó).
// También es lo que cobra el que canta si el otro dice "no quiero".
int Partida::puntosTrucoActual() {
  if (estado.nivel_truco == 0)
    return 1;
  return estado.nivel_truco + 1;
}

// =============================================================================
// TURNOS Y RONDAS
// =============================================================================

// Construye una foto del estado actual del juego para alimentar el árbol de decisiones de la IA.
// Además de recopilar rondas jugadas, puntos y cartas visibles en mesa, ejecuta una simulación
// virtual ('lookahead'): agrega hipotéticamente una victoria de la IA a los resultados
// (si_la_gana.push_back("ia")) y consulta a ganadorDeLaMano(). Si eso definiría la mano a su favor,
// marca ctx.ganar_esta_ronda_gana_la_mano = true, lo que le permite a la IA saber con certeza
// si le conviene arriesgarse a definir o asegurar la jugada.
ContextoIA Partida::armarContextoIA() {
  ContextoIA ctx;
  ctx.ronda = static_cast<int>(estado.resultados_rondas.size());
  for (const auto &r : estado.resultados_rondas) {
    if (r == "ia")
      ctx.ganadas_ia++;
    else if (r == "jugador")
      ctx.ganadas_jugador++;
    else
      ctx.hubo_parda = true;
  }
  if (!estado.resultados_rondas.empty())
    ctx.ganador_primera = estado.resultados_rondas[0];
  if (estado.ronda_actual.jugoJugador())
    ctx.carta_rival = estado.ronda_actual.obtenerCartaJugador();
  if (estado.ronda_actual.jugoIA())
    ctx.carta_propia = estado.ronda_actual.obtenerCartaIA();
  ctx.pts_jugador = jugador.obtenerPuntos();
  ctx.puntos_objetivo = puntos_objetivo;
  ctx.valor_falta = calcularFalta(jugador.obtenerPuntos(), ia.obtenerPuntos(),
                                  puntos_objetivo, falta_a_buenas);
  ctx.rival_a_un_punto = (jugador.obtenerPuntos() == puntos_objetivo - 1);

  // ¿Ganar esta ronda le da la mano a la IA? Se usa la misma regla de pardas
  // que define las manos, así no hay dos versiones de la regla.
  vector<string> si_la_gana = estado.resultados_rondas;
  si_la_gana.push_back("ia");
  ctx.ganar_esta_ronda_gana_la_mano =
      ganadorDeLaMano(si_la_gana, turno_es_mano_jugador) == "ia";
  return ctx;
}

vector<string> Partida::historialRondas() {
  vector<string> historial;
  for (const auto &r : estado.resultados_rondas)
    historial.push_back(r == "jugador" ? "Ganada por Vos"
                        : r == "ia"    ? "Ganada por IA"
                                       : "Parda");
  return historial;
}

// Muestra qué cantó la IA: primero el canto en mayúsculas (para que no haya
// dudas) y después la frase de pique, que siempre nombra el canto.
void Partida::anunciarCantoIA(const string &canto) {
  string clave = "canto_envido";
  if (canto == "Real Envido")
    clave = "canto_real_envido";
  else if (canto == "Falta Envido")
    clave = "canto_falta_envido";
  else if (canto == "Truco")
    clave = "canto_truco";
  else if (canto == "Retruco")
    clave = "canto_retruco";
  else if (canto == "Vale Cuatro")
    clave = "canto_valecuatro";
  else if (canto == "Flor")
    clave = "canto_flor";
  else if (canto == "Contraflor")
    clave = "canto_contraflor";

  string mayusculas = canto;
  for (char &c : mayusculas)
    c = static_cast<char>(toupper(static_cast<unsigned char>(c)));

  cout << "\n>>> La IA canta: " << mayusculas << " <<<\n";
  cout << "La IA dice: " << Mensajes::obtenerPique(clave) << "\n";
}

// Suma puntos (de envido o flor) y avisa si con eso terminó la partida
bool Partida::sumarYVerSiTermino(pair<int, int> pts) {
  jugador.sumarPuntos(pts.first);
  ia.sumarPuntos(pts.second);
  return terminoPartida();
}

// Turno del jugador: puede cantar (flor/envido/truco), irse al mazo o tirar.
// Repite el menú hasta que tire una carta o termine la mano.
// Retorna "" si tiró carta normalmente; si no, cómo terminó la mano:
// "ia", "jugador", "mazo_jugador" o "fin_partida".
string Partida::turnoJugador(int ronda) {
  while (true) {
    vector<string> historial = historialRondas();
    if (estado.ronda_actual.jugoIA())
      cout << "\n>> En la mesa: la IA tiró "
           << estado.ronda_actual.obtenerCartaIA().mostrar() << " <<\n";
    Interfaz::mostrarEstadoMano(jugador.obtenerMano(),
                                ia.cantidadCartasEnMano(), historial);

    vector<string> opciones = {"Jugar carta"};
    // Flor o envido: solo en la 1ra ronda, antes de tirar.
    // Con flor no se ofrece envido (la flor lo anula).
    if (ronda == 0 && puedeCantarFlor("jugador")) {
      opciones.push_back("Cantar Flor");
    } else if (ronda == 0 && !estado.envido_cerrado) {
      for (const auto &c : cantosEnvidoPosibles({}))
        opciones.push_back("Cantar " + c);
    }
    // Truco: solo si no fuiste vos el último en cantarlo
    if (estado.nivel_truco < 3 && estado.quien_canto_truco != "jugador") {
      string cantos[] = {"Cantar Truco", "Cantar Retruco", "Cantar Vale Cuatro"};
      opciones.push_back(cantos[estado.nivel_truco]);
    }
    opciones.push_back("Irme al mazo");

    string elegida = opciones[Interfaz::pedirOpcion(opciones)];

    if (elegida == "Irme al mazo")
      return "mazo_jugador";

    if (elegida == "Cantar Flor") {
      if (sumarYVerSiTermino(resolverFlor("jugador")))
        return "fin_partida";
      continue;
    }

    if (elegida.find("Envido") != string::npos) {
      string canto = elegida.substr(string("Cantar ").size());
      if (sumarYVerSiTermino(resolverEnvido("jugador", canto)))
        return "fin_partida";
      continue; // vuelve al menú: todavía tenés que tirar
    }

    if (elegida.find("Cantar") != string::npos) { // truco/retruco/vale4
      string resultado = resolverTruco("jugador");
      if (resultado != "quiero")
        return resultado;
      continue;
    }

    // Jugar carta
    vector<string> opc_cartas;
    for (const auto &c : jugador.obtenerMano())
      opc_cartas.push_back(c.mostrar());
    opc_cartas.push_back("Volver atrás");

    int eleccion = Interfaz::pedirOpcion(opc_cartas);
    if (eleccion == static_cast<int>(opc_cartas.size()) - 1) {
      // Regresa al menú de acciones sin tirar carta
      continue;
    }

    Carta c_j = jugador.jugarCarta(eleccion);
    estado.ronda_actual.registrarCartaJugador(c_j);
    cout << ">> Jugaste: " << c_j.mostrar() << " <<\n";
    return "";
  }
}

// Turno de la IA: flor (si tiene), envido (solo 1ra ronda), truco, y tira.
// Retorna lo mismo que turnoJugador.
string Partida::turnoIA(int ronda) {
  if (ronda == 0 && puedeCantarFlor("ia")) {
    if (sumarYVerSiTermino(resolverFlor("ia")))
      return "fin_partida";
  }

  if (ronda == 0 && !estado.envido_cerrado) {
    string canto = ia.decidirCantoEnvido(armarContextoIA());
    if (canto != "nada") {
      if (sumarYVerSiTermino(resolverEnvido("ia", canto)))
        return "fin_partida";
    }
  }

  if (estado.nivel_truco < 3 && estado.quien_canto_truco != "ia" &&
      ia.decidirCantarTruco(armarContextoIA())) {
    string resultado = resolverTruco("ia");
    if (resultado != "quiero")
      return resultado;
  }

  Carta c_i = ia.jugarCarta(ia.elegirCarta(armarContextoIA()));
  estado.ronda_actual.registrarCartaIA(c_i);
  cout << ">> IA jugó: " << c_i.mostrar() << " <<\n";
  return "";
}

// Juega las rondas de la mano. Retorna "jugador" o "ia" (quién gana la mano),
// "mazo_jugador" si te fuiste al mazo, o "" si la partida terminó en el medio.
string Partida::jugarRondas() {
  bool turno_actual_jugador = turno_es_mano_jugador;

  for (int r = 0; r < 3; ++r) {
    estado.ronda_actual = Ronda();
    cout << "\n=== RONDA " << (r + 1) << " ===\n";
    // Si arranca la IA, primero mostrás tus cartas: así ves con qué contás
    // antes de que te cante envido o truco.
    if (!turno_actual_jugador)
      Interfaz::mostrarEstadoMano(jugador.obtenerMano(),
                                  ia.cantidadCartasEnMano(), historialRondas());

    // Cada uno juega su turno; si un turno termina la mano, se corta acá
    for (int turno = 0; turno < 2; ++turno) {
      string fin = turno_actual_jugador ? turnoJugador(r) : turnoIA(r);
      if (fin == "fin_partida")
        return "";
      if (fin != "")
        return fin;
      turno_actual_jugador = !turno_actual_jugador;
    }

    // Terminada la 1ra ronda, ya nadie puede cantar envido ni flor
    estado.envido_cerrado = true;
    estado.flor_cerrada = true;

    // Resumen de la ronda
    cout << "\n--- Fin de la ronda " << (r + 1) << " ---\n";
    cout << "Vos: " << estado.ronda_actual.obtenerCartaJugador().mostrar()
         << "  vs  IA: " << estado.ronda_actual.obtenerCartaIA().mostrar()
         << "\n";

    string gan_ronda = estado.ronda_actual.determinarGanador();
    estado.resultados_rondas.push_back(gan_ronda);
    if (gan_ronda == "jugador") {
      cout << ">> Ganaste la ronda <<\n";
      cout << "La IA dice: " << Mensajes::obtenerPique("perdi_ronda") << "\n";
      turno_actual_jugador = true;
    } else if (gan_ronda == "ia") {
      cout << ">> La IA ganó la ronda <<\n";
      cout << "La IA dice: " << Mensajes::obtenerPique("gano_ronda") << "\n";
      turno_actual_jugador = false;
    } else {
      cout << ">> ¡Parda! <<\n";
      turno_actual_jugador = turno_es_mano_jugador;
    }

    string ganador =
        ganadorDeLaMano(estado.resultados_rondas, turno_es_mano_jugador);
    if (ganador != "")
      return ganador;

    // La mano sigue: pausa para leer el resultado y pantalla limpia
    Interfaz::pausar();
    Interfaz::limpiarPantalla();
    Interfaz::mostrarTablero(jugador.obtenerNombre(), jugador.obtenerPuntos(),
                             "IA", ia.obtenerPuntos(), puntos_objetivo);
  }

  // Después de 3 rondas ganadorDeLaMano siempre define (ver sus reglas)
  return ganadorDeLaMano(estado.resultados_rondas, turno_es_mano_jugador);
}

bool Partida::terminoPartida() {
  return jugador.obtenerPuntos() >= puntos_objetivo ||
         ia.obtenerPuntos() >= puntos_objetivo;
}

/*
 * ¿Qué hace esta clase?
 * La clase Partida es el motor del juego. Controla el flujo completo: pregunta
 * las reglas (15/30 puntos, flor, falta envido), reparte cartas, e intercala
 * los turnos manejando flor, envido, truco y el juego de las rondas.
 *
 * ¿Qué concepto de C++ ilustra?
 * Ilustra la Composición de objetos y la delegación de responsabilidades.
 * `Partida` no sabe cómo evaluar si un envido es bueno o no, eso lo delega a la
 * `IA` y a `Jugador`. Las reglas "puras" (puntos del envido, pardas) son
 * funciones `static`: no necesitan una partida en curso, y por eso se pueden
 * probar solas en pruebas.cpp.
 */

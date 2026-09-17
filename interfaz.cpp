#include "interfaz.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

void Interfaz::mostrarCarta(const Carta &carta) {
  auto lineas = carta.mostrarASCII();
  for (const auto &linea : lineas) {
    cout << linea << "\n";
  }
}

void Interfaz::mostrarCartasHorizontal(const vector<Carta> &cartas) {
  if (cartas.empty())
    return;

  // Suponemos que cada carta tiene 7 líneas de alto.
  const int ALTO_CARTA = 7;
  vector<vector<string>> asciiCartas;
  for (const auto &c : cartas) {
    asciiCartas.push_back(c.mostrarASCII());
  }

  for (int i = 0; i < ALTO_CARTA; ++i) {
    for (const auto &lineasCarta : asciiCartas) {
      if (i < (int)lineasCarta.size()) {
        cout << lineasCarta[i] << "   ";
      }
    }
    cout << "\n";
  }
}

void Interfaz::mostrarTablero(const string &nombre_jugador, int pts_jugador,
                              const string &nombre_ia, int pts_ia,
                              int objetivo) {
  auto generarBarra = [objetivo](int puntos) {
    int longitud = 12; // Longitud total de la barra
    int llenos = (puntos * longitud) / objetivo;
    if (llenos > longitud)
      llenos = longitud;
    string barra = "[";
    for (int i = 0; i < longitud; ++i) {
      barra += (i < llenos) ? "=" : ".";
    }
    barra += "]";
    return barra;
  };

  cout << "+----------------------------------+\n";
  cout << "|  PUNTAJE                         |\n";
  cout << "|  " << left << setw(10) << nombre_jugador << ": "
       << generarBarra(pts_jugador) << " " << setw(2) << pts_jugador << "/"
       << objetivo << "  |\n";
  cout << "|  " << left << setw(10) << nombre_ia << ": " << generarBarra(pts_ia)
       << " " << setw(2) << pts_ia << "/" << objetivo << "  |\n";
  cout << "+----------------------------------+\n";
}

void Interfaz::mostrarEstadoMano(const vector<Carta> &mano_jugador,
                                 int cartas_ia,
                                 const vector<string> &resultados_rondas) {
  cout << "\n--- Estado de la Mano ---\n";
  cout << "Cartas IA:\n";
  for (int i = 0; i < cartas_ia; ++i) {
    cout << "[X] ";
  }
  cout << "\n\nTus cartas:\n";
  mostrarCartasHorizontal(mano_jugador);

  if (!resultados_rondas.empty()) {
    cout << "\nResultados de las rondas previas:\n";
    for (size_t i = 0; i < resultados_rondas.size(); ++i) {
      cout << "Ronda " << (i + 1) << ": " << resultados_rondas[i] << "\n";
    }
  }
  cout << "-------------------------\n";
}

void Interfaz::mostrarMensaje(const string &mensaje) {
  cout << "\n************************************\n";
  cout << "  " << mensaje << "\n";
  cout << "************************************\n\n";
}

int Interfaz::pedirOpcion(const vector<string> &opciones) {
  if (opciones.empty())
    return -1;

  for (size_t i = 0; i < opciones.size(); ++i) {
    cout << (i + 1) << ". " << opciones[i] << "\n";
  }

  int eleccion;
  while (true) {
    cout << "Elegí una opción: ";
    if (cin >> eleccion && eleccion >= 1 && eleccion <= (int)opciones.size()) {
      return eleccion - 1;
    } else {
      cout << "Opción inválida, che. Probá de nuevo.\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
}

void Interfaz::limpiarPantalla() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void Interfaz::pausar() {
  cout << "Presioná Enter para continuar...";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cin.get();
}

void Interfaz::mostrarBanner() {
  cout << "  _______  _____   _    _   _____   ____  \n"
       << " |__   __||  __ \\ | |  | | / ____| / __ \\ \n"
       << "    | |   | |__) || |  | || |     | |  | |\n"
       << "    | |   |  _  / | |  | || |     | |  | |\n"
       << "    | |   | | \\ \\ | |__| || |____ | |__| |\n"
       << "    |_|   |_|  \\_\\ \\____/  \\_____| \\____/ \n\n";
}

void Interfaz::mostrarCartaBocaAbajo() {
  cout << "+-------------+\n";
  cout << "| ########### |\n";
  cout << "| ########### |\n";
  cout << "| ########### |\n";
  cout << "| ########### |\n";
  cout << "| ########### |\n";
  cout << "+-------------+\n";
}

/*
 * La clase `Interfaz` maneja todo lo visual de la aplicación en la terminal.
 * Se diseñó agrupando funciones de entrada/salida como métodos estáticos para
 * separar la lógica de presentación de la del juego, lo que se alinea con el
 * principio de responsabilidad única (Single Responsibility Principle).
 * Conceptos de C++:
 * - Uso de directivas del preprocesador `#ifdef _WIN32` para compilación
 * condicional según el sistema operativo, permitiendo limpiar la consola en
 * Windows o Linux apropiadamente.
 * - `setw` y `left` de la librería `<iomanip>` para formatear el texto del
 * tablero y dejarlo prolijo.
 */

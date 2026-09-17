#ifdef _WIN32
#include <windows.h>
#endif
#include "partida.h"
#include "interfaz.h"

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    Interfaz::limpiarPantalla();
    Interfaz::mostrarBanner();
    Partida partida;
    partida.iniciar();
    return 0;
}

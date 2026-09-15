#include "carta.h"
#include <stdexcept>

// Constructor, inicializa los atributos de la carta
Carta::Carta(int numero, std::string palo) : numero(numero), palo(palo) {}

int Carta::obtenerNumero() const {
    return numero;
}

std::string Carta::obtenerPalo() const {
    return palo;
}

// Devuelve la jerarquía de la carta (1 al 14).
// Acá es donde aplicamos la posta de las reglas del Truco.
int Carta::obtenerJerarquia() const {
    if (numero == 1 && palo == "Espada") return 1;
    if (numero == 1 && palo == "Basto") return 2;
    if (numero == 7 && palo == "Espada") return 3;
    if (numero == 7 && palo == "Oro") return 4;
    if (numero == 3) return 5;
    if (numero == 2) return 6;
    if (numero == 1 && (palo == "Oro" || palo == "Copa")) return 7;
    if (numero == 12) return 8;
    if (numero == 11) return 9;
    if (numero == 10) return 10;
    if (numero == 7 && (palo == "Basto" || palo == "Copa")) return 11;
    if (numero == 6) return 12;
    if (numero == 5) return 13;
    if (numero == 4) return 14;
    
    // Fallback por si llega algo raro (aunque no debería pasar)
    return 15;
}

// Calcula los puntos crudos de la carta para el envido.
// Figuras valen cero, el resto su valor facial.
int Carta::obtenerValorEnvido() const {
    if (numero == 10 || numero == 11 || numero == 12) {
        return 0;
    }
    return numero;
}

std::string Carta::obtenerNombrePalo() const {
    return palo;
}

std::string Carta::mostrar() const {
    return std::to_string(numero) + " de " + palo;
}

// Devuelve un vector de strings que dibuja la carta.
// ¡Queda flama en la consola!
std::vector<std::string> Carta::mostrarASCII() const {
    std::vector<std::string> lineas(7);
    
    // Tratamiento de figuras (Sota, Caballo, Rey)
    std::string numStr = std::to_string(numero);
    if (numero == 10) numStr = "S";
    else if (numero == 11) numStr = "C";
    else if (numero == 12) numStr = "R";

    // Acomodamos el espaciado para que el numerito quede alineado
    std::string padIzq = (numStr.length() == 1) ? " " : "";

    // Centramos el nombre del palo a ojímetro (ancho interior 11)
    std::string paloCentrado;
    if (palo == "Espada") paloCentrado = "  Espada   ";
    else if (palo == "Basto") paloCentrado = "   Basto   ";
    else if (palo == "Oro") paloCentrado = "    Oro    ";
    else if (palo == "Copa") paloCentrado = "   Copa    ";
    else paloCentrado = "           "; // Por las dudas

    lineas[0] = "┌─────────────┐";
    lineas[1] = "│ " + numStr + "         " + padIzq + "│";
    lineas[2] = "│             │";
    lineas[3] = "│ " + paloCentrado + " │";
    lineas[4] = "│             │";
    lineas[5] = "│         " + padIzq + numStr + " │";
    lineas[6] = "└─────────────┘";

    return lineas;
}

/*
=============================================================================
[MODO TUTOR ACTIVO]
Che, mirá lo que hicimos acá. Esta es la implementación de la clase Carta.
- Diseño: Usamos encapsulamiento. Los atributos (numero y palo) están 
  privados (`private`) para que no los puedan tocar desde afuera a lo bruto. 
  La única forma de leerlos es a través de los getters (`obtenerNumero()`).
- Const Correctness: Todos los getters tienen el calificador `const`. Esto
  es una buena práctica en C++ para asegurarle al compilador (y a otros 
  programadores) que llamar a estos métodos NO va a modificar el estado interno 
  del objeto. ¡Ayuda a prevenir bugs!
=============================================================================
*/

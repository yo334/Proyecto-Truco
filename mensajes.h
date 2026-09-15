#pragma once
#include <string>

// Clase Mensajes con frases de pique de la IA.
class Mensajes {
public:
    static std::string obtenerPique(const std::string& situacion);
    static std::string bienvenida();
    static std::string ganadorPartida(const std::string& nombre);
};

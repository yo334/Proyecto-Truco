#pragma once
#include <string>
#include <vector>

// Clase que representa una carta del mazo español
class Carta {
private:
    int numero;
    std::string palo;

public:
    // Constructor de la carta
    Carta(int numero, std::string palo);

    // Getters para los atributos principales
    int obtenerNumero() const;
    std::string obtenerPalo() const;

    // Retorna la posición en la jerarquía del truco (1 es la mejor, 14 la peor)
    int obtenerJerarquia() const;

    // Retorna el valor numérico para el cálculo del envido (1-7 o 0 para figuras)
    int obtenerValorEnvido() const;

    // Retorna el nombre del palo (alias para obtenerPalo, según requerimiento)
    std::string obtenerNombrePalo() const;

    // Retorna un string formateado, ej: "7 de Espada"
    std::string mostrar() const;

    // Retorna el arte ASCII de la carta en 7 líneas
    std::vector<std::string> mostrarASCII() const;
};

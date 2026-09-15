#include "partida.h"
#include "ronda.h"
#include "interfaz.h"
#include <iostream>
#include <limits>

Partida::Partida() : jugador("Jugador"), ia(), mazo(), turno_es_mano_jugador(true) {
    std::string nombre;
    while (true) {
        std::cout << "¡Bienvenido al Truco!\n¿Cuál es tu nombre? ";
        if (std::cin >> nombre) {
            bool tieneLetras = false;
            for (char c : nombre) {
                if (std::isalpha(c)) tieneLetras = true;
            }
            if (tieneLetras) break;
            std::cout << "Che, poné un nombre válido (al menos una letra).\n";
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    jugador = Jugador(nombre);

    int pts;
    while (true) {
        std::cout << "¿A cuántos puntos jugamos? (15/30): ";
        if (std::cin >> pts && (pts == 15 || pts == 30)) {
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Por favor, ingresá 15 o 30.\n";
    }
    puntos_objetivo = pts;

    std::string flor;
    while (true) {
        std::cout << "¿Jugamos con flor? (s/n): ";
        if (std::cin >> flor && (flor == "s" || flor == "S" || flor == "n" || flor == "N")) {
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Por favor, ingresá 's' o 'n'.\n";
    }
    con_flor = (flor == "s" || flor == "S");

    nivel_truco = 0;
    truco_querido = false;
    quien_canto_truco = "";
}

void Partida::iniciar() {
    std::cout << "\n=== COMIENZA LA PARTIDA (" << puntos_objetivo << " puntos) ===\n";
    while (!terminoPartida()) {
        jugarMano();
    }
    
    if (jugador.obtenerPuntos() >= puntos_objetivo) {
        std::cout << "\n¡Ganaste la partida! Felicitaciones, " << jugador.obtenerNombre() << ".\n";
    } else {
        std::cout << "\n" << ia.obtenerFraseDePique("gano_partida") << "\n";
    }
}

void Partida::jugarMano() {
    std::cout << "\n--- NUEVA MANO ---\n";
    std::cout << "Puntaje -> " << jugador.obtenerNombre() << ": " << jugador.obtenerPuntos()
              << " | IA: " << ia.obtenerPuntos() << "\n";
              
    mazo.reiniciar();
    repartirCartas();

    jugador.setEsMano(turno_es_mano_jugador);
    ia.setEsMano(!turno_es_mano_jugador);

    if (con_flor) {
        auto pts_flor = resolverFlor();
        jugador.sumarPuntos(pts_flor.first);
        ia.sumarPuntos(pts_flor.second);
        if (terminoPartida()) return;
    }

    // Aquí, para mantener el flujo, jugamos las rondas.
    std::string ganador_mano = jugarRondas();

    // Puntos básicos o de truco si terminó la mano normalmente
    if (ganador_mano == "jugador") {
        int pts = (nivel_truco > 0) ? nivel_truco + 1 : 1;
        jugador.sumarPuntos(pts);
    } else if (ganador_mano == "ia") {
        int pts = (nivel_truco > 0) ? nivel_truco + 1 : 1;
        ia.sumarPuntos(pts);
    }

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

std::pair<int,int> Partida::resolverFlor() {
    int pts_jugador = 0;
    int pts_ia = 0;

    bool j_flor = jugador.tieneFlor();
    bool i_flor = ia.tieneFlor();

    if (j_flor && i_flor) {
        std::cout << "¡Ambos tienen flor!\n";
        if (jugador.calcularFlor() > ia.calcularFlor()) {
            pts_jugador = 3;
        } else if (ia.calcularFlor() > jugador.calcularFlor()) {
            pts_ia = 3;
        } else {
            if (turno_es_mano_jugador) pts_jugador = 3;
            else pts_ia = 3;
        }
    } else if (j_flor) {
        std::cout << "¡Tenés Flor!\n";
        pts_jugador = 3;
    } else if (i_flor) {
        std::cout << "La IA cantó Flor.\n";
        pts_ia = 3;
    }

    return {pts_jugador, pts_ia};
}

std::pair<int,int> Partida::resolverEnvido() {
    int pts_jugador = 0, pts_ia = 0;
    std::cout << "\n(Sección de Envido)\n";
    
    bool jugador_canta = false;
    std::string canto_ia = "nada";

    if (turno_es_mano_jugador) {
        int accion = pedirAccion({"Cantar Envido", "Cantar Real Envido", "Cantar Falta Envido", "No cantar nada"});
        if (accion < 3) {
            jugador_canta = true;
            std::string cantos[] = {"Envido", "Real Envido", "Falta Envido"};
            std::string canto = cantos[accion];
            std::cout << "Cantaste: " << canto << "\n";
            
            if (ia.decidirAceptarEnvido(canto)) {
                std::cout << "La IA dice: " << ia.obtenerFraseDePique("acepto") << "\n";
                std::cout << "Tu tanto: " << jugador.calcularEnvido() << "\n";
                std::cout << "Tanto IA: " << ia.calcularEnvido() << "\n";
                
                int pts_querido = 2;
                if (canto == "Real Envido") pts_querido = 3;
                else if (canto == "Falta Envido") pts_querido = puntos_objetivo - std::max(jugador.obtenerPuntos(), ia.obtenerPuntos());
                
                if (jugador.calcularEnvido() > ia.calcularEnvido() || (jugador.calcularEnvido() == ia.calcularEnvido() && turno_es_mano_jugador)) {
                    pts_jugador = pts_querido;
                } else {
                    pts_ia = pts_querido;
                }
            } else {
                std::cout << "La IA dice: " << ia.obtenerFraseDePique("rechazo") << "\n";
                pts_jugador = 1;
            }
        }
    }

    if (!jugador_canta) {
        canto_ia = ia.decidirCantoEnvido();
        if (canto_ia != "nada") {
            std::string situacion = "canto_envido";
            int pts_querido = 2; 
            if (canto_ia == "Real Envido") {
                situacion = "canto_real_envido";
                pts_querido = 3;
            } else if (canto_ia == "Falta Envido") {
                situacion = "canto_falta_envido";
                pts_querido = puntos_objetivo - std::max(jugador.obtenerPuntos(), ia.obtenerPuntos());
            }

            std::cout << "La IA dice: " << ia.obtenerFraseDePique(situacion) << "\n";
            
            int accion = pedirAccion({"Quiero", "No quiero"});
            if (accion == 0) { 
                std::cout << "Tu tanto: " << jugador.calcularEnvido() << "\n";
                std::cout << "Tanto IA: " << ia.calcularEnvido() << "\n";
                if (jugador.calcularEnvido() > ia.calcularEnvido() || (jugador.calcularEnvido() == ia.calcularEnvido() && turno_es_mano_jugador)) {
                    pts_jugador = pts_querido;
                } else {
                    pts_ia = pts_querido;
                }
            } else {
                pts_ia = 1;
            }
        } else {
            std::cout << "Nadie cantó envido.\n";
        }
    }
    
    if (!turno_es_mano_jugador && canto_ia == "nada") {
        int accion = pedirAccion({"Cantar Envido", "Cantar Real Envido", "Cantar Falta Envido", "No cantar nada"});
        if (accion < 3) {
            std::string cantos[] = {"Envido", "Real Envido", "Falta Envido"};
            std::string canto = cantos[accion];
            std::cout << "Cantaste: " << canto << "\n";
            
            if (ia.decidirAceptarEnvido(canto)) {
                std::cout << "La IA dice: " << ia.obtenerFraseDePique("acepto") << "\n";
                std::cout << "Tu tanto: " << jugador.calcularEnvido() << "\n";
                std::cout << "Tanto IA: " << ia.calcularEnvido() << "\n";
                
                int pts_querido = 2;
                if (canto == "Real Envido") pts_querido = 3;
                else if (canto == "Falta Envido") pts_querido = puntos_objetivo - std::max(jugador.obtenerPuntos(), ia.obtenerPuntos());
                
                if (jugador.calcularEnvido() > ia.calcularEnvido() || (jugador.calcularEnvido() == ia.calcularEnvido() && turno_es_mano_jugador)) {
                    pts_jugador = pts_querido;
                } else {
                    pts_ia = pts_querido;
                }
            } else {
                std::cout << "La IA dice: " << ia.obtenerFraseDePique("rechazo") << "\n";
                pts_jugador = 1;
            }
        }
    }

    return {pts_jugador, pts_ia};
}

std::pair<int,int> Partida::resolverTruco(const std::string& origen) {
    std::string cantos[] = {"", "Truco", "Retruco", "Vale Cuatro"};
    int nuevo_nivel = nivel_truco + 1;
    if (nuevo_nivel > 3) return {0,0}; 

    std::cout << "\n(Sección de Truco)\n";

    if (origen == "jugador") {
        std::cout << "Cantaste: " << cantos[nuevo_nivel] << "\n";
        if (ia.decidirAceptarTruco(nuevo_nivel)) {
            std::cout << "La IA dice: " << ia.obtenerFraseDePique("acepto") << "\n";
            nivel_truco = nuevo_nivel;
            quien_canto_truco = "jugador";
            truco_querido = true;
            return {0, 0}; 
        } else {
            std::cout << "La IA dice: " << ia.obtenerFraseDePique("rechazo") << "\n";
            int pts = (nivel_truco == 0) ? 1 : nivel_truco + 1; 
            return {pts, 0};
        }
    } else {
        std::cout << "La IA dice: " << ia.obtenerFraseDePique("canto_truco") << "\n";
        int accion = pedirAccion({"Quiero", "No quiero"});
        if (accion == 0) {
            nivel_truco = nuevo_nivel;
            quien_canto_truco = "ia";
            truco_querido = true;
            return {0, 0}; 
        } else {
            int pts = (nivel_truco == 0) ? 1 : nivel_truco + 1; 
            return {0, pts};
        }
    }
}

std::string Partida::jugarRondas() {
    int ganadas_j = 0, ganadas_i = 0;
    bool turno_actual_jugador = turno_es_mano_jugador;
    
    std::vector<std::string> historial_rondas;
    Interfaz::mostrarEstadoMano(jugador.obtenerMano(), ia.cantidadCartasEnMano(), historial_rondas);

    auto pts_env = resolverEnvido();
    jugador.sumarPuntos(pts_env.first);
    ia.sumarPuntos(pts_env.second);

    bool parda_historica = false;
    std::string ganador_parda = ""; 
    
    for (int r = 0; r < 3; ++r) {
        if (ganadas_j == 2) return "jugador";
        if (ganadas_i == 2) return "ia";
        if (terminoPartida()) return "";

        Ronda ronda_actual;
        std::cout << "\n=== RONDA " << (r+1) << " ===\n";
        
        for (int turno = 0; turno < 2; ++turno) {
            if (turno_actual_jugador) {
                bool jugo_carta = false;
                while (!jugo_carta) {
                    Interfaz::mostrarEstadoMano(jugador.obtenerMano(), ia.cantidadCartasEnMano(), historial_rondas);
                    std::vector<std::string> opciones = {"Jugar carta"};
                    if (nivel_truco < 3 && quien_canto_truco != "jugador") {
                        if (nivel_truco == 0) opciones.push_back("Cantar Truco");
                        else if (nivel_truco == 1) opciones.push_back("Cantar Retruco");
                        else if (nivel_truco == 2) opciones.push_back("Cantar Vale Cuatro");
                    }
                    
                    int accion = pedirAccion(opciones);
                    
                    if (opciones[accion].find("Cantar") != std::string::npos) {
                        auto pts_truco = resolverTruco("jugador");
                        if (pts_truco.first > 0 || pts_truco.second > 0) {
                            jugador.sumarPuntos(pts_truco.first);
                            ia.sumarPuntos(pts_truco.second);
                            return "terminada_por_truco"; 
                        }
                    } else {
                        std::vector<std::string> opc_cartas;
                        auto mano = jugador.obtenerMano();
                        for (size_t i = 0; i < mano.size(); ++i) {
                            opc_cartas.push_back(mano[i].mostrar());
                        }
                        int idx = pedirAccion(opc_cartas);
                        Carta c_j = jugador.jugarCarta(idx);
                        ronda_actual.registrarCartaJugador(c_j);
                        std::cout << "Jugaste: " << c_j.mostrar() << "\n";
                        jugo_carta = true;
                    }
                }
                turno_actual_jugador = false; 
            } else {
                if (nivel_truco < 3 && quien_canto_truco != "ia") {
                    if (ia.decidirCantarTruco(r+1, ganadas_i, ganadas_j, parda_historica, ia.obtenerPuntos(), jugador.obtenerPuntos())) {
                        auto pts_truco = resolverTruco("ia");
                        if (pts_truco.first > 0 || pts_truco.second > 0) {
                            jugador.sumarPuntos(pts_truco.first);
                            ia.sumarPuntos(pts_truco.second);
                            return "terminada_por_truco";
                        }
                    }
                }
                int idx_ia = ia.elegirCarta(ganadas_i, ganadas_j);
                Carta c_i = ia.jugarCarta(idx_ia);
                ronda_actual.registrarCartaIA(c_i);
                std::cout << "IA jugó: " << c_i.mostrar() << "\n";
                turno_actual_jugador = true; 
            }
        }

        std::string gan_ronda = ronda_actual.determinarGanador();
        std::string res_str = "";
        if (gan_ronda == "jugador") {
            ganadas_j++;
            std::cout << "Ganaste la ronda.\n";
            res_str = "Ganada por Vos";
            turno_actual_jugador = true; 
            if (!parda_historica) ganador_parda = "jugador";
        } else if (gan_ronda == "ia") {
            ganadas_i++;
            std::cout << "La IA ganó la ronda.\n";
            res_str = "Ganada por IA";
            turno_actual_jugador = false; 
            if (!parda_historica) ganador_parda = "ia";
        } else {
            std::cout << "¡Parda!\n";
            ganadas_j++; ganadas_i++; 
            res_str = "Parda";
            parda_historica = true;
            turno_actual_jugador = turno_es_mano_jugador; 
        }
        historial_rondas.push_back(res_str);
        
        if (parda_historica && r > 0) {
           if (ganadas_j > ganadas_i) return "jugador";
           if (ganadas_i > ganadas_j) return "ia";
           if (ganadas_j == ganadas_i && ganadas_j > 0) {
               return ganador_parda != "" ? ganador_parda : (turno_es_mano_jugador ? "jugador" : "ia");
           }
        }
    }

    if (ganadas_j > ganadas_i) return "jugador";
    else if (ganadas_i > ganadas_j) return "ia";
    return turno_es_mano_jugador ? "jugador" : "ia";
}

int Partida::pedirAccion(const std::vector<std::string>& opciones) {
    int eleccion = -1;
    while (true) {
        std::cout << "Elegí una acción:\n";
        for (size_t i = 0; i < opciones.size(); ++i) {
            std::cout << i << ") " << opciones[i] << "\n";
        }
        std::cout << "Opción: ";
        if (std::cin >> eleccion && eleccion >= 0 && eleccion < (int)opciones.size()) {
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Che, poné una opción válida.\n";
    }
    return eleccion;
}

bool Partida::terminoPartida() {
    return jugador.obtenerPuntos() >= puntos_objetivo || ia.obtenerPuntos() >= puntos_objetivo;
}

/*
 * MODO TUTOR:
 * ¿Qué hace esta clase?
 * La clase Partida es el motor del juego. Controla el flujo completo: pregunta las reglas (15/30 puntos, flor),
 * reparte cartas, e intercala los turnos manejando envido, truco, y el juego de las rondas.
 * 
 * ¿Qué concepto de C++ ilustra?
 * Ilustra la Composición de objetos y la delegación de responsabilidades. `Partida` no sabe cómo
 * evaluar si un envido es bueno o no, eso lo delega a la `IA` y a `Jugador`. También usa `std::pair`
 * para retornar dos valores juntos (puntos para jugador e IA).
 */

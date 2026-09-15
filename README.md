# 🃏 Proyecto Truco

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=cplusplus)
![Terminal](https://img.shields.io/badge/Plataforma-Terminal-green?style=for-the-badge)
![License](https://img.shields.io/badge/Licencia-MIT-yellow?style=for-the-badge)

Un juego de **Truco Argentino** jugable desde la terminal, donde enfrentás a una inteligencia artificial con personalidad. / A playable **Argentine Truco** card game for the terminal, where you face an AI opponent with personality.

---

## 🇪🇸 Español

### 📖 ¿Qué es?

El **Proyecto Truco** es un juego de cartas de Truco Argentino completo, programado en C++17 para la terminal. Jugás contra una IA que evalúa la fuerza de su mano y toma decisiones basadas en probabilidades — no juega al azar, piensa (y te carga con frases de pique).

### 🎮 Características

- ✅ **Truco completo:** Truco → Retruco → Vale Cuatro (con la regla "solo sube el desafiado")
- ✅ **Envido completo:** Envido → Envido → Real Envido → Falta Envido
- ✅ **Flor opcional:** Configurable al inicio de la partida
- ✅ **Partidas a 15 o 30 puntos:** Elegís antes de empezar
- ✅ **IA con personalidad:** Toma decisiones por probabilidades y te tira frases de pique
- ✅ **Arte ASCII completo:** Cartas, tablero de puntaje y marcos decorativos en la terminal
- ✅ **Reglas de empate (pardas):** Implementadas según el reglamento oficial

### 🛠️ Compilación

**Requisitos:**
- `g++` con soporte para C++17 (MinGW, MSYS2, o cualquier distribución Linux)
- `make` (incluido en MinGW/MSYS2, o instalable con `sudo apt install make` en Linux)

**Compilar:**
```bash
make
```

**Ejecutar:**
```bash
./truco
```

**Limpiar archivos compilados:**
```bash
make clean
```

### 📁 Estructura del Proyecto

```
Proyecto_Truco/
├── main.cpp              # Punto de entrada
├── carta.h / carta.cpp   # Clase Carta (número, palo, jerarquía, valor de envido)
├── mazo.h / mazo.cpp     # Clase Mazo (mezcla, reparto)
├── jugador.h / jugador.cpp # Clase Jugador (mano, puntaje, envido)
├── ia.h / ia.cpp         # Clase IA (decisiones por probabilidades, hereda de Jugador)
├── ronda.h / ronda.cpp   # Clase Ronda (resolución de bazas)
├── partida.h / partida.cpp # Motor principal del juego
├── interfaz.h / interfaz.cpp # Arte ASCII y presentación visual
├── mensajes.h / mensajes.cpp # Frases de pique de la IA
├── Makefile              # Compilación automática
├── README.md             # Este archivo
└── datos/
    └── reglas_truco.json # Referencia de reglas y jerarquía de cartas
```

### 🃏 ¿Cómo se juega?

1. Al iniciar, el programa te pregunta:
   - ¿A cuántos puntos querés jugar? (15 o 30)
   - ¿Querés jugar con flor? (Sí o No)
2. Se reparten 3 cartas a cada jugador.
3. En la primera ronda podés cantar **envido** antes de tirar tu carta.
4. En cualquier ronda podés cantar **truco** para subir la apuesta.
5. Gana la mano quien gane 2 de 3 rondas.
6. Gana la partida quien llegue primero al puntaje objetivo.

### 🤖 La IA

La IA no es omnisciente — no sabe qué cartas tenés. Evalúa la fuerza de su propia mano y decide con probabilidades:
- Si tiene buen envido, lo canta (a mayor envido, más probable que cante).
- Si tiene buenas cartas de pelea, acepta el truco.
- A veces se la juega con un farol (baja probabilidad de cantar con mano débil).
- Y siempre te tira un pique. 😏

### 📐 Contexto

Este proyecto fue desarrollado como **trabajo práctico de C++ para 6to año** y como **proyecto de portfolio** en GitHub. Utiliza el sistema de agentes [fefe-models](https://github.com/yo334/fefe-models) para la planificación y desarrollo.

---

## 🇺🇸 English

### 📖 What is it?

**Proyecto Truco** is a complete Argentine Truco card game, built in C++17 for the terminal. You play against an AI that evaluates its hand strength and makes probability-based decisions — it doesn't play randomly, it thinks (and trash-talks you in Argentine Spanish).

### 🎮 Features

- ✅ **Full Truco:** Truco → Retruco → Vale Cuatro (only the challenged player can raise)
- ✅ **Full Envido:** Envido → Envido → Real Envido → Falta Envido
- ✅ **Optional Flor:** Configurable at game start
- ✅ **15 or 30 point games:** Choose before playing
- ✅ **AI with personality:** Probability-based decisions + trash talk in Argentine Spanish
- ✅ **Full ASCII art:** Cards, scoreboard, and decorative frames in the terminal
- ✅ **Tie-breaking rules (pardas):** Implemented per official rules

### 🛠️ Building

**Requirements:**
- `g++` with C++17 support (MinGW, MSYS2, or any Linux distro)
- `make` (included in MinGW/MSYS2, or install with `sudo apt install make` on Linux)

**Build:**
```bash
make
```

**Run:**
```bash
./truco
```

**Clean build files:**
```bash
make clean
```

### 🃏 How to play?

1. On launch, the game asks:
   - How many points? (15 or 30)
   - Play with Flor? (Yes or No)
2. 3 cards are dealt to each player.
3. In the first round, you can call **envido** before playing your card.
4. In any round, you can call **truco** to raise the stakes.
5. Win a hand by winning 2 out of 3 rounds.
6. Win the game by reaching the target score first.

### 🤖 The AI

The AI doesn't know your cards. It evaluates its own hand and decides with probabilities:
- Strong envido → more likely to call it.
- Strong fighting cards → accepts truco.
- Sometimes it bluffs (low probability of calling with weak hands).
- And it always trash-talks. 😏

---

### 📜 Licencia / License
MIT

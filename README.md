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

- ✅ **Truco completo:** Truco → Retruco → Vale Cuatro (con la regla "solo sube el desafiado") y respuesta directa "Quiero y Retruco"
- ✅ **Envido completo:** Envido → Envido → Real Envido → Falta Envido, respondiendo con subidas
- ✅ **Falta envido configurable:** a ganar o a las buenas (jugando a 30)
- ✅ **Flor opcional:** se canta, se puede responder a un envido y existe la contraflor
- ✅ **Irse al mazo** en cualquier turno
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

> En Windows con MinGW, usá `mingw32-make` en vez de `make` y ejecutá `.\truco.exe`.

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
└── README.md             # Este archivo
```

### 🃏 ¿Cómo se juega?

1. Al iniciar, el programa te pregunta:
   - ¿A cuántos puntos querés jugar? (15 o 30)
   - ¿Querés jugar con flor? (Sí o No)
   - Si jugás a 30: ¿la falta envido es a ganar o a las buenas?
2. Se reparten 3 cartas a cada jugador.
3. En tu turno de la primera ronda, antes de tirar, podés cantar **envido** (o **flor** si tenés). El mano decide primero; si tira sin cantar, después puede cantar el pie.
4. En cualquier turno podés cantar **truco**, y cuando te lo cantan podés responder **"Quiero y Retruco"**.
5. Podés **irte al mazo** en cualquier turno.
6. Al final de cada ronda se muestra quién la ganó y el juego espera un Enter.
7. Gana la mano quien gane 2 de 3 rondas (con las reglas de pardas del reglamento).
8. Gana la partida quien llegue primero al puntaje objetivo.

### 🤖 La IA

La IA no es omnisciente — no sabe qué cartas tenés. Evalúa la fuerza de su propia mano y decide con probabilidades:
- Si tiene buen envido, lo canta (a mayor envido, más probable que cante).
- Si tiene buenas cartas de pelea, acepta el truco, y tiene en cuenta si ganó o perdió la primera ronda.
- Cuando tira segunda, tira la carta justa para ganarte (la más baja que te gane).
- A veces se la juega con un farol (baja probabilidad de cantar con mano débil).
- Y siempre te tira un pique. 😏

---

## 🇺🇸 English

### 📖 What is it?

**Proyecto Truco** is a complete Argentine Truco card game, built in C++17 for the terminal. You play against an AI that evaluates its hand strength and makes probability-based decisions — it doesn't play randomly, it thinks (and trash-talks you in Argentine Spanish).

### 🎮 Features

- ✅ **Full Truco:** Truco → Retruco → Vale Cuatro (only the challenged player can raise), including "Quiero y Retruco" replies
- ✅ **Full Envido:** Envido → Envido → Real Envido → Falta Envido, with raises
- ✅ **Configurable Falta Envido:** to win or to "buenas" (30-point games)
- ✅ **Optional Flor:** can be called, used to answer an envido, and countered with Contraflor
- ✅ **Fold ("irse al mazo")** on any turn
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

> On Windows with MinGW, use `mingw32-make` instead of `make` and run `.\truco.exe`.

### 🃏 How to play?

1. On launch, the game asks:
   - How many points? (15 or 30)
   - Play with Flor? (Yes or No)
   - In 30-point games: is Falta Envido "to win" or "to buenas"?
2. 3 cards are dealt to each player.
3. On your first-round turn, before playing, you can call **envido** (or **flor** if you have it). The "mano" decides first; if they play without calling, the other player may call afterwards.
4. On any turn you can call **truco**, and when called you can answer **"Quiero y Retruco"**.
5. You can **fold** on any turn.
6. After each round the game shows who won it and waits for Enter.
7. Win a hand by winning 2 out of 3 rounds (with the official tie rules).
8. Win the game by reaching the target score first.

### 🤖 The AI

The AI doesn't know your cards. It evaluates its own hand and decides with probabilities:
- Strong envido → more likely to call it.
- Strong fighting cards → accepts truco, weighing whether it won or lost the first round.
- When playing second, it plays the lowest card that beats yours.
- Sometimes it bluffs (low probability of calling with weak hands).
- And it always trash-talks. 😏

---

### 📜 Licencia / License
MIT

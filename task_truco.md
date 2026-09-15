# 🃏 Task List — Proyecto Truco

## Fase 0: Documentación de Diseño
- [x] Investigar reglas del truco argentino
- [x] Entrevista de clarificación con el usuario
- [x] Generar SDD (sdd-proyecto-truco.md)
- [x] Obtener aprobación del usuario

## Fase 1: Datos de Referencia
- [x] Generar `datos/reglas_truco.json` con jerarquía, valores y reglas

## Fase 2: Primer Entregable
- [x] Generar `README.md` bilingüe dentro del proyecto
- [x] Eliminar `nose.cpp` (archivo vacío obsoleto)

## Fase 3: Implementación — Clases Base [/]
- [/] `carta.h` / `carta.cpp`
  - [ ] Sistema de valores de cartas (jerarquía de pelea, valor de envido)
- [ ] `mazo.h` / `mazo.cpp`
- [ ] `jugador.h` / `jugador.cpp`
  - [ ] Sistema de puntaje del jugador

## Fase 4: Implementación — Motor de Juego
- [ ] `ronda.h` / `ronda.cpp`
  - [ ] Sistema de turnos (mano/pie, rotación)
  - [ ] Canto del envido + sistema de envido (cadena, aceptar/rechazar)
  - [ ] Canto del truco (cadena truco→retruco→vale cuatro, "solo sube el desafiado")
- [ ] `ia.h` / `ia.cpp`
  - [ ] Bot: probabilidad de cantar envido (tabla 6.3)
  - [ ] Bot: probabilidad de cantar truco (sistema ponderado 6.5)
  - [ ] Bot: jugar sin cantar (estrategia de cartas, modo agresivo/conservador)
- [ ] `partida.h` / `partida.cpp`
  - [ ] Preguntar 15/30 y flor o no al inicio

## Fase 5: Implementación — Presentación
- [ ] `interfaz.h` / `interfaz.cpp`
  - [ ] "Interfaz gráfica" de terminal (arte ASCII completo: cartas, tablero de puntaje, marcos)
- [ ] `mensajes.h` / `mensajes.cpp`
- [ ] `main.cpp`
- [ ] `Makefile`

## Fase 6: Verificación QA
- [ ] Compilación sin warnings
- [ ] Ejecución sin segfaults
- [ ] Verificación de reglas (envido, truco, pardas, flor)
- [ ] Verificación de IA coherente
- [ ] Verificación de arte ASCII

## Fase 7: Aprendizaje (/learn)
- [x] Proponer learning_proposal.md con comportamientos reutilizables
- [x] Crear regla de workspace `.agents/rules/AGENTS.md`
- [ ] *(Opcional)* Actualizar perfiles.md en fefe-models con preguntas SDD para `cpp-terminal`

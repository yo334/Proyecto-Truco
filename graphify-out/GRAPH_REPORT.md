# Graph Report - Proyecto_Truco  (2026-10-18)

## Corpus Check
- Corpus is ~11,983 words - fits in a single context window. You may not need a graph.

## Summary
- 197 nodes · 508 edges · 9 communities (7 shown, 2 thin omitted)
- Extraction: 79% EXTRACTED · 21% INFERRED · 0% AMBIGUOUS · INFERRED: 105 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- Inteligencia Artificial y Decisiones
- Flujo de Partida y Cantos
- Modelo de Cartas y Jerarqu?as
- Gesti?n de Jugadores y Envido
- Coordinaci?n Principal e Inclusiones
- Interfaz de Consola
- Suite de Pruebas Unitarias
- Administraci?n del Mazo
- Estado de Mano y Reglas

## God Nodes (most connected - your core abstractions)
1. `Partida` - 36 edges
2. `Carta` - 32 edges
3. `IA` - 27 edges
4. `ContextoIA` - 26 edges
5. `Jugador` - 24 edges
6. `Ronda` - 17 edges
7. `EstadoMano` - 12 edges
8. `main()` - 12 edges
9. `Interfaz` - 8 edges
10. `Mazo` - 8 edges

## Surprising Connections (you probably didn't know these)
- `probarCartaJusta()` --calls--> `Carta`  [INFERRED]
  pruebas.cpp → carta.h
- `probarCartasASCII()` --calls--> `Carta`  [INFERRED]
  pruebas.cpp → carta.h
- `probarEnvidoConCartasTiradas()` --calls--> `Carta`  [INFERRED]
  pruebas.cpp → carta.h
- `probarFaltaSegunPuntos()` --calls--> `Carta`  [INFERRED]
  pruebas.cpp → carta.h
- `probarManoAsegurada()` --calls--> `Carta`  [INFERRED]
  pruebas.cpp → carta.h

## Import Cycles
- None detected.

## Communities (9 total, 2 thin omitted)

### Community 0 - "Inteligencia Artificial y Decisiones"
Cohesion: 0.11
Nodes (18): ContextoIA, carta_propia, carta_rival, ganadas_ia, ganadas_jugador, ganador_primera, ganar_esta_ronda_gana_la_mano, hubo_parda (+10 more)

### Community 1 - "Flujo de Partida y Cantos"
Cohesion: 0.17
Nodes (10): otro(), Partida, con_flor, estado, falta_a_buenas, ia, jugador, mazo (+2 more)

### Community 2 - "Modelo de Cartas y Jerarqu?as"
Cohesion: 0.13
Nodes (7): Carta, Carta::Carta(), numero, palo, Ronda, carta_ia, carta_jugador

### Community 3 - "Gesti?n de Jugadores y Envido"
Cohesion: 0.16
Nodes (8): Jugador, Jugador::Jugador(), mano, mano_inicial, nombre, puntos, Partida::Partida(), probarEnvidoConCartasTiradas()

### Community 6 - "Suite de Pruebas Unitarias"
Cohesion: 0.29
Nodes (9): largoVisible(), main(), probarCantosPosibles(), probarCartaJusta(), probarCartasASCII(), probarFalta(), probarPardas(), probarPuntosEnvido() (+1 more)

### Community 7 - "Administraci?n del Mazo"
Cohesion: 0.25
Nodes (3): Mazo, cartas, Mazo::Mazo()

### Community 8 - "Estado de Mano y Reglas"
Cohesion: 0.20
Nodes (8): EstadoMano, cadena_envido, envido_cerrado, flor_cerrada, nivel_truco, quien_canto_truco, resultados_rondas, ronda_actual

## Knowledge Gaps
- **37 isolated node(s):** `numero`, `palo`, `ronda`, `ganadas_ia`, `ganadas_jugador` (+32 more)
  These have ≤1 connection - possible missing edges. (Counts symbols only; 58 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Carta` connect `Modelo de Cartas y Jerarqu?as` to `Inteligencia Artificial y Decisiones`, `Gesti?n de Jugadores y Envido`, `Coordinaci?n Principal e Inclusiones`, `Interfaz de Consola`, `Suite de Pruebas Unitarias`, `Administraci?n del Mazo`?**
  _High betweenness centrality (0.270) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `Carta` (e.g. with `probarCartaJusta()` and `probarCartasASCII()`) actually correct?**
  _`Carta` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `numero`, `palo`, `ronda` to the rest of the system?**
  _37 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Inteligencia Artificial y Decisiones` be split into smaller, more focused modules?**
  _Cohesion score 0.11382113821138211 - nodes in this community are weakly interconnected._
- **Why does `Partida` connect `Flujo de Partida y Cantos` to `Inteligencia Artificial y Decisiones`, `Gesti?n de Jugadores y Envido`, `Coordinaci?n Principal e Inclusiones`, `Administraci?n del Mazo`, `Estado de Mano y Reglas`?**
  _High betweenness centrality (0.258) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `IA` (e.g. with `probarCartaJusta()` and `probarFaltaSegunPuntos()`) actually correct?**
  _`IA` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `Modelo de Cartas y Jerarqu?as` be split into smaller, more focused modules?**
  _Cohesion score 0.12698412698412698 - nodes in this community are weakly interconnected._
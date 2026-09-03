# Project Synthesis — 3D Action Tower Defense

A hybrid action-defense gameplay prototype developed in Unreal Engine 5 demonstrating real-time C++ spatial queries, decoupled combat architecture, and AI navigation.

## Gameplay & Systems Demo

<!-- If using YouTube, replace YOUR_VIDEO_ID below with your actual unlisted YouTube video ID -->
[![Project Synthesis Demo](https://img.youtube.com/vi/vM-Qaj_8eXw/maxresdefault.jpg)](https://www.youtube.com/watch?v=vM-Qaj_8eXw)

*Click above to watch the gameplay systems demonstration (1080p60).*

---

## Technical Architecture & Combat Systems

### 1. C++ Spatial Queries & Hit Registration
- Core raycasting, collision channel filtering, and damage application are encapsulated in a reusable C++ actor component: [`UCombatTraceComponent`](Source/ProjectSynth/CombatTraceComponent.h).
- Calculates directional vectors from camera orientation, applies point damage via `UGameplayStatics::ApplyPointDamage`, and eliminates per-frame tick overhead by executing strictly on input.
- Includes editor-configurable debug visualizers (`bDrawDebugTraces`) for runtime hit confirmation.
- **Source Code:** [CombatTraceComponent.h](Source/ProjectSynth/CombatTraceComponent.h) | [CombatTraceComponent.cpp](Source/ProjectSynth/CombatTraceComponent.cpp)

### 2. Decoupled Combat Architecture
- Uses dynamic multicast delegates (`OnTargetHit`, `OnTraceMiss`) to separate low-level line trace math from visual presentation layers.
- Blueprints and visual components subscribe to trace events to drive Niagara beam FX, audio cues, and target impact reactions without tight coupling to the weapon logic.

### 3. AI Navigation & Objective Defense
- Leverages Unreal NavMesh pathfinding and `AIMoveTo` behavior loops to drive multi-wave enemy navigation toward objective structures.
- Implements dynamic raycast surface detection for player-deployed base defenses.

---

## Repository Structure

text
├── Config/                  # Default engine, input, and project settings
├── Source/
│   └── ProjectSynth/
│       ├── CombatTraceComponent.h
│       └── CombatTraceComponent.cpp
├── ProjectSynth.uproject
└── README.md

*Note: Large art assets (`/Content`) and binary build caches are excluded from this repository to highlight core systems architecture and C++ implementation.*

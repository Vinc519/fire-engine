# Fire Engine — Architecture

Ce document décrit les décisions d'architecture du projet, leur justification,
et les règles à respecter pour maintenir la cohérence du code au fil du temps.

---

## Philosophie générale

Fire Engine est construit sur trois principes fondamentaux :

**1. Modularité** — Chaque système est indépendant et peut être remplacé ou
désactivé sans impacter les autres. Un module ne doit jamais supposer l'existence
d'un autre module sauf s'il en dépend explicitement.

**2. Lisibilité avant optimisation** — Le code doit être compréhensible par
quelqu'un qui le découvre pour la première fois. L'optimisation vient ensuite,
justifiée par des mesures concrètes (profiling), jamais par intuition.

**3. Dépendances explicites** — Les dépendances entre modules sont déclarées
dans CMake et visibles dans les includes. Un module ne doit jamais inclure
les headers d'un module dont il n'est pas censé dépendre.

---

## Structure des couches

L'architecture suit un modèle en couches strictement ordonné.
Une couche ne peut dépendre que des couches situées **en dessous** d'elle.

```
┌─────────────────────────────────────────────────┐
│              Sandbox / Application              │  ← Utilise tout
├─────────────────────────────────────────────────┤
│   Renderer │  Input  │  Audio  │   Physics      │  ← Systèmes haut niveau
├─────────────────────────────────────────────────┤
│                   Core Systems                  │  ← Fondations
├─────────────────────────────────────────────────┤
│           Platform Independence Layer           │  ← Abstraction OS
├─────────────────────────────────────────────────┤
│               3rd Party SDKs / OS               │  ← Externe
└─────────────────────────────────────────────────┘
```

### Couche 1 — Platform Independence Layer (`engine/platform/`)

Abstrait tout ce qui dépend de l'OS derrière une interface commune.
Chaque fonctionnalité expose une API générique avec des implémentations
séparées par plateforme (`_Windows.cpp`, `_Linux.cpp`).

| Module         | Rôle                                          |
|----------------|-----------------------------------------------|
| `detection/`   | Identification de la plateforme au runtime    |
| `threading/`   | Threads, mutex, atomics                       |
| `filesystem/`  | Accès aux fichiers et répertoires             |
| `timer/`       | Timer haute résolution                        |
| `networking/`  | Sockets de base                               |

> **Règle** : Toute interaction avec l'OS passe par `platform`.
> Jamais d'appel à `WinAPI` ou `unistd.h` directement depuis `core`
> ou les couches supérieures.

### Couche 2 — Core Systems (`engine/core/`)

La base de tout. Aucune dépendance vers d'autres modules engine.
Contient les outils que **tous** les autres systèmes peuvent utiliser.

| Module       | Rôle                                                  |
|--------------|-------------------------------------------------------|
| `memory/`    | Allocateurs personnalisés, tracking mémoire           |
| `math/`      | Vecteurs, matrices, quaternions, fonctions utilitaires|
| `strings/`   | StringId (hashed strings), manipulation de chaînes    |
| `debug/`     | Logger, assertions, macros de débogage                |
| `config/`    | Chargement de la configuration moteur                 |
| `profiling/` | Timers de performance, compteurs                      |
| `io/`        | Lecture/écriture de fichiers (interface générique)    |
| `rtti/`      | Informations de type à l'exécution (sans RTTI C++)    |

> **Règle** : `core` ne doit jamais inclure de headers de `platform`,
> `renderer`, `input`, `audio` ou `physics`.


### Couche 3 — Systèmes haut niveau

Chaque système est indépendant des autres systèmes de sa couche.
Le renderer ne connaît pas l'audio. L'input ne connaît pas la physique.

| Module       | Dépend de               |
|--------------|-------------------------|
| `renderer/`  | `core`, `platform`      |
| `input/`     | `core`, `platform`      |
| `audio/`     | `core`, `platform`      |
| `physics/`   | `core`                  |

---

## Le Renderer — Architecture interne

Le renderer est le système le plus complexe. Il est conçu autour d'une
**abstraction backend** qui permet de cibler Vulkan, DirectX 12 ou OpenGL
sans changer le code appelant.

```
Application
    │
    ▼
RendererAPI          ← Interface abstraite (pure virtuelle)
    │
    ├── VulkanDevice
    ├── DX12Device
    └── OGLDevice
```

Le backend actif est sélectionné à la compilation via les options CMake :
- `-DFE_ENABLE_VULKAN=ON`
- `-DFE_ENABLE_DX12=ON`
- `-DFE_ENABLE_OPENGL=ON`

> **Décision** : On préfère la sélection à la compilation plutôt qu'au runtime
> pour éviter la complexité d'un système de plugins et garder le code simple
> pendant la phase d'apprentissage. Cette décision pourra être revisitée.

---

## Règles de dépendances (résumé)

```
core        → (rien d'autre dans engine/)
platform    → core
renderer    → core, platform
input       → core, platform
audio       → core, platform
physics     → core
sandbox     → FireEngine (tout)
tests       → FireEngine (tout)
```

Toute violation de ces règles est une erreur d'architecture.

---

## Ajouter un nouveau module

1. Créer le dossier sous `engine/mon_module/`
2. Créer son `CMakeLists.txt` en déclarant ses dépendances explicitement
3. L'ajouter dans `engine/CMakeLists.txt` via `add_subdirectory`
4. L'ajouter à la cible `FireEngine INTERFACE` dans `engine/CMakeLists.txt`
5. Documenter sa place dans les couches ici

---

## Journal des décisions (ADR)

| Date       | Décision                                              | Raison                                              |
|------------|-------------------------------------------------------|-----------------------------------------------------|
| 2026-02-21 | C++17 comme standard                                  | Bon équilibre entre modernité et support toolchain  |
| 2026-02-21 | Libs statiques pour chaque module                     | Simplicité, pas de gestion de .dll/.so au démarrage |
| 2026-02-21 | Vulkan en premier backend                             | API bas niveau, meilleur apprentissage              |
| 2026-02-21 | Pas de RTTI C++ natif (`-fno-rtti`)                   | Contrôle explicite, performances, habitude moteur   |
| 2026-02-21 | Sélection du backend renderer à la compilation        | Simplicité pendant la phase d'apprentissage         |

# 🔥 Fire Engine

![Build Status](https://img.shields.io/badge/build-in%20progress-yellow)
![Language](https://img.shields.io/badge/language-C%2FC%2B%2B-blue)
![API](https://img.shields.io/badge/graphics%20API-Vulkan-red)
![License](https://img.shields.io/badge/license-MIT-green)
![CMake](https://img.shields.io/badge/build%20system-CMake-blue)

> A custom 2D/3D rendering engine built from scratch to explore graphics APIs, game engine architecture, and the mathematics behind real-time rendering.

---

## 📖 About

**Fire Engine** is a personal learning project written in **C/C++** with **CMake**. The goal is to build a fully functional game engine from the ground up — covering rendering, physics, audio, and input — while gaining deep understanding of:

- Real-time 2D and 3D rendering techniques
- Low-level graphics APIs (Vulkan, DirectX, OpenGL)
- The mathematics behind transformations, lighting, and physics
- The architecture of a modern game engine

---

## ✨ Features (planned)

- 🎨 **3D Renderer** — Vulkan-based rendering pipeline
- 🖼️ **2D Renderer** — Sprite rendering, tilemaps, and more
- ⚙️ **3D Physics Engine** — Rigid body dynamics, collision detection
- 🧱 **2D Physics Engine** — AABB, collision response
- 🔊 **Audio System** — Sound loading and playback
- 🕹️ **Input System** — Customizable input mapping

---

## 🛠️ Tech Stack

| Component       | Technology         |
|-----------------|--------------------|
| Language        | C / C++            |
| Build System    | CMake              |
| 3D Graphics API | Vulkan → DirectX → OpenGL |
| Platform        | Windows / Linux    |

---

## 🚀 Getting Started

### Prerequisites

- CMake 3.25+
- Vulkan SDK
- A C++17 compatible compiler (GCC, Clang, MSVC)

### Build

```bash
git clone https://github.com/your-username/fire-engine.git
cd fire-engine
mkdir build && cd build
cmake ..
cmake --build .
```

---

## 🗺️ Roadmap

### Phase 1 — 3D Rendering with Vulkan
- [ ] Window creation and Vulkan initialization
- [ ] Swap chain, render pass, framebuffers
- [ ] Vertex/index buffers
- [ ] Shader pipeline (GLSL → SPIR-V)
- [ ] Texture mapping
- [ ] Camera and transformations (MVP matrices)
- [ ] Basic lighting (Phong model)

### Phase 2 — 3D Physics Engine
- [ ] Rigid body dynamics
- [ ] Collision detection (AABB, OBB, Sphere)
- [ ] Collision response

### Phase 3 — Input System
- [ ] Keyboard and mouse input
- [ ] Customizable input mapping / action system
- [ ] Gamepad support

### Phase 4 — Audio System
- [ ] Audio file loading (WAV, OGG)
- [ ] Basic playback and volume control
- [ ] 3D positional audio

### Phase 5 — 2D Renderer
- [ ] Sprite rendering
- [ ] Tilemap support
- [ ] 2D camera

### Phase 6 — 2D Physics Engine
- [ ] AABB collision
- [ ] Trigger zones
- [ ] Raycasting 2D

### Phase 7 — API Migration
- [ ] Port 3D renderer to DirectX 12
- [ ] Port 3D renderer to OpenGL
- [ ] Abstraction layer to switch APIs at runtime

---

## 📚 Learning Resources

- [Vulkan Tutorial](https://vulkan-tutorial.com)
- [3D Math Primer for Graphics and Game Development](https://gamemath.com)
- [Game Engine Architecture – Jason Gregory](https://www.gameenginebook.com)

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
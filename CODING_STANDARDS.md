# Fire Engine — Coding Standards

Ce document définit les conventions de code du projet.
Elles doivent être respectées dans **tous** les fichiers sans exception.
L'objectif est qu'un fichier soit immédiatement lisible sans avoir à deviner
les intentions de son auteur.

---

## 1. Nommage

### Types (classes, structs, enums)
```cpp
// PascalCase — toujours
class RenderDevice { };
struct VertexBuffer { };
enum class ShaderStage { };
```

### Fonctions et méthodes
```cpp
// PascalCase — toujours
void Initialize();
bool IsValid() const;
RenderDevice* GetDevice();
```

### Variables locales et paramètres
```cpp
// camelCase
int frameCount = 0;
float deltaTime = 0.0f;
void Submit(CommandBuffer* commandBuffer);
```

### Membres de classe
```cpp
// camelCase avec préfixe "m_"
class AudioSource {
    float    m_volume   = 1.0f;
    bool     m_isPlaying = false;
    AudioClip* m_clip   = nullptr;
};
```

### Membres statiques de classe
```cpp
// camelCase avec préfixe "s_"
class Logger {
    static Logger* s_instance;
};
```

### Constantes et valeurs d'enum
```cpp
// SCREAMING_SNAKE_CASE pour les constantes globales
constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
constexpr float    PI = 3.14159265358979f;

// PascalCase pour les valeurs d'enum class
enum class ShaderStage {
    Vertex,
    Fragment,
    Compute,
};
```

### Macros
```cpp
// SCREAMING_SNAKE_CASE avec préfixe FE_
#define FE_ASSERT(condition, message)
#define FE_LOG_ERROR(message)
#define FE_UNUSED(x) (void)(x)
```

### Fichiers
```cpp
// PascalCase, correspondant exactement au nom de la classe principale
RenderDevice.h
RenderDevice.cpp
VulkanSwapChain.h
VulkanSwapChain.cpp
```

---

## 2. Organisation des fichiers

### Header (.h)
Un header ne doit contenir que :
- La déclaration de la classe / struct / enum.
- Les includes **strictement nécessaires** (préférer les forward declarations).
- Pas d'implémentation (sauf templates et inline justifiés).

```cpp
// RenderDevice.h
#pragma once

#include <cstdint>

// Forward declaration — ne pas inclure CommandBuffer.h si on utilise juste un pointeur
class CommandBuffer;
class Texture;

namespace fe {

class RenderDevice
{
public:
    virtual ~RenderDevice() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown()   = 0;
    virtual void Submit(CommandBuffer* commandBuffer) = 0;

private:
    uint32_t m_frameIndex = 0;
};

} // namespace fe
```

### Source (.cpp)
```cpp
// RenderDevice.cpp

// 1. Header correspondant EN PREMIER
#include "renderer/common/RenderDevice.h"

// 2. Headers du projet (par ordre alphabétique dans chaque groupe)
#include "core/debug/Logger.h"
#include "core/memory/Allocator.h"

// 3. Headers tiers
// (rien ici dans cet exemple)

// 4. Headers système
#include <cstring>

namespace fe {

// ... implémentation

} // namespace fe
```

### Ordre des includes (toujours respecter)
1. Header propre au fichier
2. Headers `engine/` du projet
3. Headers `third_party/`
4. Headers système (`<cstdint>`, `<vector>`, etc.)

---

## 3. Namespaces

Tout le code du moteur vit dans le namespace `fe` (Fire Engine).
Les backends spécifiques ont leur propre sous-namespace.

```cpp
namespace fe {          // Code commun du moteur
namespace fe::vk {      // Backend Vulkan
namespace fe::dx12 {    // Backend DirectX 12
namespace fe::gl {      // Backend OpenGL
```

Ne jamais écrire `using namespace fe;` dans un header.
Dans un `.cpp`, c'est acceptable mais non obligatoire.

---

## 4. Classes

### Ordre des membres (toujours respecter)
```cpp
class MySystem
{
public:
    // 1. Constructeurs / destructeur
    MySystem();
    ~MySystem();

    // 2. Méthodes publiques
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    // 3. Getters / setters publics
    float GetVolume() const       { return m_volume; }
    void  SetVolume(float volume) { m_volume = volume; }

protected:
    // 4. Méthodes protégées

private:
    // 5. Méthodes privées
    void InternalReset();

    // 6. Membres privés (toujours en dernier)
    float m_volume   = 1.0f;
    bool  m_isActive = false;
};
```

### Règles générales
- **Une classe = un fichier** (sauf petites structs utilitaires très liées).
- Toujours initialiser les membres dans la déclaration (`= 0`, `= nullptr`, `= false`).
- Préférer `= default` et `= delete` explicites plutôt que de laisser le compilateur décider.
- Les méthodes `const` sont obligatoires quand elles ne modifient pas l'état.

---

## 5. Gestion de la mémoire

Fire Engine n'utilise **jamais** `new` / `delete` directement dans le code métier.
Toute allocation passe par les allocateurs de `core/memory/`.

```cpp
// ❌ Interdit
RenderDevice* device = new VulkanDevice();

// ✅ Correct (une fois l'allocateur implémenté)
RenderDevice* device = FE_NEW(VulkanDevice);
FE_DELETE(device);
```

Les `std::unique_ptr` et `std::shared_ptr` sont tolérés en phase d'initialisation
mais doivent être évités dans les chemins critiques (boucle de rendu, physique).

---

## 6. Gestion des erreurs

Fire Engine n'utilise **pas** les exceptions C++ (`-fno-exceptions`).
Les erreurs sont signalées par valeur de retour ou assertions.

```cpp
// Fonctions qui peuvent échouer → retourner bool ou un code d'erreur
bool RenderDevice::Initialize()
{
    if (!CreateSwapChain()) {
        FE_LOG_ERROR("Failed to create swap chain");
        return false;
    }
    return true;
}

// Préconditions → assertions (désactivées en release)
void Submit(CommandBuffer* commandBuffer)
{
    FE_ASSERT(commandBuffer != nullptr, "CommandBuffer must not be null");
    // ...
}
```

---

## 7. Commentaires

### Quoi commenter
```cpp
// ✅ Expliquer le POURQUOI, pas le QUOI
// On utilise un double buffer pour éviter que le CPU attende le GPU
uint32_t m_currentFrame = 0;

// ❌ Inutile — le code se lit tout seul
// Incrémente le compteur de frames
m_frameCount++;
```

### En-tête de fichier
Chaque fichier `.h` commence par un bloc de description :

```cpp
// =============================================================================
// RenderDevice.h
// Interface abstraite pour un périphérique de rendu.
// Toute implémentation concrète (Vulkan, DX12, OpenGL) doit hériter de cette
// classe et implémenter toutes ses méthodes virtuelles.
// =============================================================================
#pragma once
```

### TODO et FIXME
```cpp
// TODO(nom): description de ce qui reste à faire
// FIXME(nom): description d'un bug connu
// HACK(nom): solution temporaire, à nettoyer
// NOTE: information importante pour le lecteur
```

---

## 8. Règles diverses

- **Pas de nombres magiques** — toujours nommer les constantes.
- **Pas de `using namespace std;`** dans les headers.
- **`#pragma once`** en tête de chaque header (pas de include guards manuels).
- **Accolades obligatoires** sur tous les blocs `if`, `for`, `while`, même sur une ligne.
- **Pas de code commenté** — le code mort va dans git, pas dans les fichiers.
- **Longueur de ligne** — 100 caractères maximum.

```cpp
// ❌ Interdit
if (isReady) DoSomething();

// ✅ Correct
if (isReady) {
    DoSomething();
}
```

---

## 9. Checklist avant tout commit

- [ ] Les noms respectent les conventions de ce document
- [ ] Les includes sont dans le bon ordre
- [ ] Pas de `new` / `delete` direct dans le code métier
- [ ] Les méthodes `const` sont marquées `const`
- [ ] Les préconditions sont protégées par `FE_ASSERT`
- [ ] Pas de nombres magiques
- [ ] Les fichiers commencent par leur bloc de description
- [ ] Le code compile sans warnings (`-Wall -Wextra`)

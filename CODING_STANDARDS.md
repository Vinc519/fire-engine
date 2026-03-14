# Fire Engine — Coding Standards

This document defines the coding conventions for the project.
They must be followed in **all** files without exception.
The goal is that any file should be immediately readable without having to guess
the author's intent.

---

## 1. Naming

### Types (classes, structs, enums)
```cpp
// PascalCase — always
class RenderDevice { };
struct VertexBuffer { };
enum class ShaderStage { };
```

### Functions and methods
```cpp
// PascalCase — always
void Initialize();
bool IsValid() const;
RenderDevice* GetDevice();
```

### Local variables and parameters
```cpp
// camelCase
int frameCount = 0;
float deltaTime = 0.0f;
void Submit(CommandBuffer* commandBuffer);
```

### Class members
```cpp
// camelCase with "m_" prefix
class AudioSource {
    float      m_volume    = 1.0f;
    bool       m_isPlaying = false;
    AudioClip* m_clip      = nullptr;
};
```

### Static class members
```cpp
// camelCase with "s_" prefix
class Logger {
    static Logger* s_instance;
};
```

### Constants and enum values
```cpp
// SCREAMING_SNAKE_CASE for global constants
constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
constexpr float    PI = 3.14159265358979f;

// PascalCase for enum class values
enum class ShaderStage {
    Vertex,
    Fragment,
    Compute,
};
```

### Macros
```cpp
// SCREAMING_SNAKE_CASE with FE_ prefix
#define FE_ASSERT(condition, message)
#define FE_LOG_ERROR(message)
#define FE_UNUSED(x) (void)(x)
```

### Files
```cpp
// PascalCase, matching exactly the name of the main class
RenderDevice.h
RenderDevice.cpp
VulkanSwapChain.h
VulkanSwapChain.cpp
```

---

## 2. File organization

### Header (.h)
A header must only contain:
- The declaration of the class / struct / enum.
- **Strictly necessary** includes (prefer forward declarations).
- No implementation (except justified templates and inlines).

```cpp
// RenderDevice.h
#pragma once

#include <cstdint>

// Forward declaration — do not include CommandBuffer.h if only a pointer is used
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

// 1. Corresponding header FIRST
#include "renderer/common/RenderDevice.h"

// 2. Project headers (alphabetical order within each group)
#include "core/debug/Logger.h"
#include "core/memory/Allocator.h"

// 3. Third-party headers
// (none in this example)

// 4. System headers
#include <cstring>

namespace fe {

// ... implementation

} // namespace fe
```

### Include order (always respect)
1. Own header
2. Project `engine/` headers
3. `third_party/` headers
4. System headers (`<cstdint>`, `<vector>`, etc.)

---

## 3. Namespaces

All engine code lives in the `fe` namespace (Fire Engine).
Backend-specific code has its own sub-namespace.

```cpp
namespace fe {          // Common engine code
namespace fe::vk {      // Vulkan backend
namespace fe::dx12 {    // DirectX 12 backend
namespace fe::gl {      // OpenGL backend
```

Never write `using namespace fe;` in a header.
In a `.cpp` file it is acceptable but not required.

---

## 4. Classes

### Member order (always respect)
```cpp
class MySystem
{
public:
    // 1. Constructors / destructor
    MySystem();
    ~MySystem();

    // 2. Public methods
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    // 3. Public getters / setters
    float GetVolume() const       { return m_volume; }
    void  SetVolume(float volume) { m_volume = volume; }

protected:
    // 4. Protected methods

private:
    // 5. Private methods
    void InternalReset();

    // 6. Private members (always last)
    float m_volume   = 1.0f;
    bool  m_isActive = false;
};
```

### General rules
- **One class = one file** (except small closely related utility structs).
- Always initialize members at the declaration site (`= 0`, `= nullptr`, `= false`).
- Prefer explicit `= default` and `= delete` rather than letting the compiler decide.
- `const` methods are mandatory when they do not modify state.

---

## 5. Memory management

Fire Engine **never** uses `new` / `delete` directly in business code.
All allocations go through the allocators in `core/memory/`.

```cpp
// ❌ Forbidden
RenderDevice* device = new VulkanDevice();

// ✅ Correct (once the allocator is implemented)
RenderDevice* device = FE_NEW(VulkanDevice);
FE_DELETE(device);
```

`std::unique_ptr` and `std::shared_ptr` are tolerated during initialization
but must be avoided in hot paths (render loop, physics).

---

## 6. Error handling

Fire Engine does **not** use C++ exceptions (`-fno-exceptions`).
Errors are reported via return values or assertions.

```cpp
// Functions that can fail → return bool or an error code
bool RenderDevice::Initialize()
{
    if (!CreateSwapChain())
    {
        FE_LOG_ERROR("Failed to create swap chain");
        return false;
    }
    return true;
}

// Preconditions → assertions (disabled in release)
void Submit(CommandBuffer* commandBuffer)
{
    FE_ASSERT(commandBuffer != nullptr, "CommandBuffer must not be null");
    // ...
}
```

---

## 7. Comments

### What to comment
```cpp
// ✅ Explain the WHY, not the WHAT
// Double buffering prevents the CPU from stalling while the GPU is still processing
uint32_t m_currentFrame = 0;

// ❌ Useless — the code is self-explanatory
// Increment the frame counter
m_frameCount++;
```

### File header
Every `.h` and `.cpp` file starts with a description block:

```cpp
// =============================================================================
// RenderDevice.h
// Abstract interface for a rendering device.
// Every concrete implementation (Vulkan, DX12, OpenGL) must inherit from this
// class and implement all its pure virtual methods.
// =============================================================================
#pragma once
```

### TODO and FIXME
```cpp
// TODO(name): description of what still needs to be done
// FIXME(name): description of a known bug
// HACK(name): temporary workaround, needs to be cleaned up
// NOTE: important information for the reader
```

---

## 8. Casting

Never use C-style casts. Always use the explicit C++ cast operators so that
the intent is clear and the compiler can catch mistakes.

```cpp
// ❌ Forbidden — hides the nature of the conversion
float f = (float)myInt;

// ✅ Correct
float f = static_cast<float>(myInt);
```

| Cast | When to use |
|---|---|
| `static_cast` | Safe, well-defined conversions (numeric, up/downcast with known type) |
| `reinterpret_cast` | Low-level reinterpretation of bits (e.g. pointer to integer). Use sparingly. |
| `const_cast` | Removing `const` only when interfacing with legacy C APIs. Justify with a comment. |
| `dynamic_cast` | Avoided — RTTI is disabled (`-fno-rtti`). Use virtual dispatch or a custom type system instead. |

---

## 9. Templates

Templates are useful but can harm readability and compilation times if overused.

### Rules
- Prefer concrete types over templates when the number of instantiations is known and small.
- Always document template parameters.
- Use `static_assert` to enforce constraints on template parameters.

```cpp
// Document the expected interface of T
template <typename T>
class ResourcePool
{
    static_assert(std::is_trivially_destructible_v<T>,
        "ResourcePool only supports trivially destructible types.");
    // ...
};
```

### `static_assert`
Use `static_assert` to catch logic errors at compile time rather than at runtime.

```cpp
static_assert(sizeof(Vertex) == 32, "Vertex layout has changed — update the pipeline.");
static_assert(MAX_FRAMES_IN_FLIGHT <= 3, "More than 3 frames in flight is not supported.");
```

---

## 10. Miscellaneous rules

- **No magic numbers** — always name constants.
- **No `using namespace std;`** in headers.
- **`#pragma once`** at the top of every header (no manual include guards).
- **Braces are mandatory** on all `if`, `for`, `while` blocks, even single-line ones.
- **No commented-out code** — dead code belongs in git history, not in source files.
- **Line length** — 120 characters maximum.

```cpp
// ✅ Allowed — single-line body without else
if (isReady)
    DoSomething();

// ❌ Forbidden — body on the same line as the condition
if (isReady) DoSomething();

// ✅ Mandatory braces as soon as there is an else or a nested block
if (isReady)
{
    DoSomething();
}
else
{
    DoSomethingElse();
}
```

---

## 11. Pre-commit checklist

- [ ] Names follow the conventions in this document
- [ ] Includes are in the correct order
- [ ] No direct `new` / `delete` in business code
- [ ] `const` methods are marked `const`
- [ ] Preconditions are guarded by `FE_ASSERT`
- [ ] No magic numbers
- [ ] Files start with their description block
- [ ] No C-style casts
- [ ] Code compiles without warnings (`-Wall -Wextra`)
- [ ] No commented-out code left behind
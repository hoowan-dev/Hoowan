# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

Hoowan is a personal 2D game engine (C++17, OpenGL), built while following The Cherno's game engine YouTube series. It runs on Windows x64 only: `Core.h` stops with `#error` on any other platform.

## Build

Premake generates the Visual Studio files from [premake5.lua](premake5.lua) (and [vendor/premake/Box2D.lua](vendor/premake/Box2D.lua) for the Box2D submodule). `.sln`/`.vcxproj` files are generated output: edit the premake scripts, never the project files, then regenerate.

```
vendor/bin/premake/premake5.exe vs2019          # regenerate Hoowan.sln (same as GenerateProjects.bat)
powershell -File scripts/Build.ps1 -Configuration Debug -Project Hoowan-Editor
powershell -File scripts/Build.ps1 -Configuration Release          # whole solution
powershell -File scripts/Build.ps1 -Project Sandbox -Rebuild
```

- `Build.ps1` finds MSBuild through vswhere (it prefers VS2019 because the projects use the v142 toolset) and runs premake first if `Hoowan.sln` is missing.
- `-Project` maps to a target in MSBuild's solution metaproject. With MSBuild 17+, a bare project name builds it (there is no `Name:Build` target), and a project inside a solution folder needs the folder prefix (e.g. `Dependencies\GLFW`). Keep `group ""` after the vendor includes in premake5.lua, or the engine/app projects end up in the `Dependencies` folder.
- Configurations: `Debug` (`HW_DEBUG`, enables `HW_ASSERT`), `Release` (`HW_RELEASE`), `Dist` (`HW_DIST`). Output goes to `bin/<Config>-windows-x86_64/<Project>/`.
- In VS Code, F5 runs "Debug Hoowan-Editor" ([.vscode/launch.json](.vscode/launch.json)), which builds first. The working directory must be the app's project folder (`Hoowan-Editor/` or `Sandbox/`) because assets are loaded through relative paths like `assets/...`.

There is no test suite or linter.

## Architecture

- **Hoowan/** is the engine, a static lib. **Hoowan-Editor/** and **Sandbox/** are console apps that link it. Hoowan-Editor is where the current game/demo work happens (`src/game/`: the player script and a level parser that builds scenes from string-literal maps plus sprite sheets). Sandbox is an older 2D renderer test bed.
- **Entry point**: an app includes `<Hoowan.h>` (the public API) and `<Hoowan/Core/EntryPoint.h>` (which defines `main`), then implements `Hoowan::CreateApplication()`. The `Application` subclass calls `PushLayer`/`PushOverlay`.
- **Application loop**: `Application::Run` updates each `Layer` in the `LayerStack` with a `Timestep`, then runs the ImGui layer. Events from `WindowsWindow` (GLFW callbacks) go to `Application::OnEvent` and then through the layers in reverse order until one marks the event handled. Bind handlers with `HW_BIND_EVENT_FN`.
- **Rendering**: front-end classes in `Hoowan/Renderer/` (`Shader`, `Texture`, `VertexArray`, `FrameBuffer`, ...) pick a backend in `Platform/OpenGL/` through static `Create` factories keyed on `RendererAPI`. `Renderer2D` is a batch renderer: draw quads between `BeginScene`/`EndScene`. `SubTexture2D` handles sprite-sheet regions. The editor renders into a `FrameBuffer` that is shown in a docked ImGui viewport.
- **ECS** (`Hoowan/ECS/`): wraps EnTT. `Scene` owns the `entt::registry` and a `Physics2DWorld`. Every entity gets a `TagComponent` and a `TransformComponent`. `Scene::OnUpdate` runs native scripts, then physics/collisions, then rendering of the colored/textured/sub-textured sprite components through the primary `CameraComponent`.
- **Native scripting**: subclass `ScriptableEntity` (override `OnCreate`/`OnUpdate`/`OnDestroy`, use `GetComponent<T>()`) and attach it with `NativeScriptComponent`.
- **Physics** (`Hoowan/Physics/`): wraps Box2D. `CollisionBody::Init` gets the scene's `b2World` (gravity -9.8). `CollisionComponent` creates a static or dynamic body, and transforms are synced from the bodies each frame.
- **Precompiled header**: every engine `.cpp` must `#include "hwpch.h"` first.
- **Logging/asserts**: `HW_CORE_*` macros are for the engine and `HW_*` for clients (spdlog). `HW_ASSERT`/`HW_CORE_ASSERT` only exist in Debug builds.
- **Profiling**: set `#define HW_PROFILE 1` in [Hoowan/src/Hoowan/Debug/Instrumentor.h](Hoowan/src/Hoowan/Debug/Instrumentor.h) to have `HW_PROFILE_FUNCTION`/`HW_PROFILE_SCOPE` write `HoowanProfile-*.json` (Startup/Runtime/Shutdown) into the working directory. Open these in `chrome://tracing`.

## Dependencies

Vendor libraries live in `Hoowan/vendor/`. spdlog, GLFW, imgui (docking branch), glm and Box2D are git submodules (`git submodule update --init --recursive`). Glad, stb_image and EnTT are checked in directly.

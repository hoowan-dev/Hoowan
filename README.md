# Hoowan

A personal 2D game engine written in C++17 and OpenGL. I'm building it to learn engine architecture, real-time rendering and C++. It started from [The Cherno's Game Engine series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) and also borrows ideas from Jason Gregory's *Game Engine Architecture* (3rd ed.).

![Hoowan Editor](RMImages/7.PNG)

## Features

- **Batch 2D renderer** – draws thousands of quads in a single draw call, with textures, sprite sheets (sub-textures) and an orthographic camera
- **Entity-component system** built on [EnTT](https://github.com/skypjack/entt): tag, transform, sprite, camera, collision and native-script components
- **2D physics** through [Box2D](https://github.com/erincatto/box2d), with static and dynamic collision bodies
- **Native C++ scripting** – attach scripts with `OnCreate` / `OnUpdate` / `OnDestroy` to any entity
- **Editor** – a dockable ImGui viewport rendered from a framebuffer
- **Profiling** – instrumentation output you can view in `chrome://tracing`

## Getting started

Hoowan currently supports **Windows x64 only**. You need Visual Studio 2019 or newer with the *Desktop development with C++* workload and the v142 (VS2019) toolset.

```sh
git clone --recursive https://github.com/hoowan-dev/Hoowan.git
cd Hoowan
GenerateProjects.bat
```

`GenerateProjects.bat` runs premake to generate `Hoowan.sln`. From there, either:

- **Visual Studio:** open `Hoowan.sln` and press F5 (Hoowan-Editor is the startup project), or
- **VS Code:** open `Hoowan.code-workspace` and press **F5** to build and debug the editor, or
- **Command line:** `powershell -File scripts/Build.ps1 -Configuration Debug -Project Hoowan-Editor`

If you cloned without `--recursive`, run `git submodule update --init --recursive` first.

In the demo, **A** / **D** move the player and **Space** jumps.

## Project layout

| Folder | What it is |
| --- | --- |
| `Hoowan/` | The engine (static library) and its third-party libraries in `vendor/` |
| `Hoowan-Editor/` | The editor app and the demo game (`src/game/`) |
| `Sandbox/` | A minimal app for trying out the renderer |

Apps link against the engine, include `<Hoowan.h>` and `<Hoowan/Core/EntryPoint.h>`, and implement `Hoowan::CreateApplication()`.

## Profiling

Set `HW_PROFILE` to `1` in [`Hoowan/src/Hoowan/Debug/Instrumentor.h`](Hoowan/src/Hoowan/Debug/Instrumentor.h). The app then writes `HoowanProfile-Startup.json`, `-Runtime.json` and `-Shutdown.json` to its working directory. Load them in `chrome://tracing` to see function timings.

## Dev log

| Date | Update |
| --- | --- |
| 09/06/2020 | **Box2D, kinematics and native scripting.** Replaced the homemade collision correction with Box2D and added C++ script components. |
| 08/03/2020 | **ECS, 2D collision and the player.** Added the EnTT entity-component system, rect-vs-rect collision and a player that can move and jump. |
| 07/12/2020 | **Sprite sheets, scene viewport and the editor.** Added sub-textures, a docked ImGui viewport and a string-based level parser. |
| 07/08/2020 | **Instrumentation, stats and batch rendering.** Added Chrome tracing output, a stats panel and a batched quad renderer. |
| 07/03/2020 | **Camera, context and 2D rendering.** Moved the camera controller, textures and shaders behind a simple `Renderer2D::DrawQuad` API. |
| 06/24/2020 | **Transforms, delta time and materials.** Added a shader file parser, uniform handling and per-frame timesteps. |
| 06/21/2020 | **Hello, abstraction!** Set up the project and wrapped spdlog, GLFW, Glad, glm and ImGui. |

<details>
<summary>Screenshots</summary>

| | |
| --- | --- |
| ![Box2D](RMImages/7.PNG) | ![ECS](RMImages/6.PNG) |
| ![Editor](RMImages/5.PNG) | ![Batching](RMImages/4.PNG) |
| ![Renderer2D](RMImages/3.PNG) | ![Transforms](RMImages/2.PNG) |
| ![Hello](RMImages/1.PNG) | |

</details>

## License

See [LICENSE](LICENSE).

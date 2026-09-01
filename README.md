# ILC — I Love C

> Toolkit for C development. Scaffolds, builds, and manages projects with a header-only library ecosystem.

**POSIX only** · Linux · macOS · BSD

---

## Quick start

```sh
ilc new my_project    # scaffold
cd my_project
ilc build             # compile
ilc run               # execute
```

---

## CLI

| Command | Description | Status |
|---------|-------------|--------|
| `ilc new <name>` | Create a new project | Done |
| `ilc build` | Build the project | Done |
| `ilc run <args>` | Run the project (with optional arguments) | Done |
| `ilc add <lib>` | Add a dependency | Planned |
| `ilc remove <lib>` | Remove a dependency | Planned |

---

## Project structure

```
my_project/
├── src/
│   └── main.c
├── include/
├── build/
└── ilc.toml
```

---

## ilc.toml

```toml
[project]
name = "my_project"
version = "0.1.0"
flags = "-O2 -Wall"

[dependencies]
array = "1.0.0"
file = "1.0.0"
```

---

## Libraries

The core ecosystem lives in [ilcLibs](https://codeberg.org/ilx159/ilcLibs).

### Core (this repo)

| Library | Description | Status |
|---------|-------------|--------|
| `ilcTypes.h` | Type aliases (`u8`, `i32`, `f64`...) and vector structs | Stable |
| `ilcArray.h` | Dynamic arrays via X-Macro | Stable |
| `ilcDir.h` | Directory management | Stable |
| `ilcFile.h` | File I/O | Stable |
| `ilcString.h` | Dynamic strings (`str_t`) | Stable |
| `ilcToml.h` | TOML parser | Partial |
| `ilcError.h` | Error framework | Empty stub |

### Extended (ilcLibs)

| Library | Description |
|---------|-------------|
| `ilcArena.h` | Arena memory allocator |
| `ilcMath.h` | Math utilities (sin/cos LUT, clamp, interpolate) |
| `ilcTerminalInput.h` | Raw terminal input |
| `ilcTest.h` | Test macros |

---

## Known bugs

There are open bugs in both repos. Contributions welcome.

- **ilc** — [docs/TODO.md](./docs/TODO.md)
- **ilcLibs** — [ilcLibs/TODO.md](https://codeberg.org/ilx159/ilcLibs/src/branch/main/TODO.md)

---

## Roadmap

```
v0.1  (current)
  - CLI: new, build, run
  - Core libs: Types, Array, Dir, File, String

v0.2
  - ilc add / remove
  - ilcError.h error framework
  - ilcProcess.h process execution
  - Tests (run_tests.sh)

v0.3
  - Refactor src/ into modules
  - Complete Toml integration (partial)
  - Recursive src/ build
```

---

## External dependencies

ILC manages only its own ecosystem libs. For external deps (e.g. SDL2), it tells you to install via your system package manager.

---

## Docs

- [Design](./docs/design/) — architecture diagrams
- [TODO](./docs/TODO.md) — known issues and planned features

---

## License

[MIT](./LICENSE)

## Author

Isaac Estevan Geuster
[github](https://github.com/Ilx159) · [codeberg](https://codeberg.org/ilx159)
ilx159@proton.me

---

*AI tools are used only as auxiliary aids. Every contribution must reflect genuine human intent.*

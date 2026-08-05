# ILC — I Love C

> A package manager and project scaffolder for C, with its own standard library ecosystem.

**Platform:** POSIX only (Linux, macOS, BSD)

---

## What is ILC?

ILC is a tool that brings the convenience of modern package managers (like Rust's Cargo) to C development. It manages dependencies, scaffolds projects, and builds them — all from the command line.

It also comes with a growing ecosystem of header-only libraries designed to work seamlessly together.

> **Note:** The general-purpose system libraries have moved to their own repository:
> [https://codeberg.org/ilx159/ilcLibs](https://codeberg.org/ilx159/ilcLibs)
>
> This repository now contains only the libraries used by the ILC tool itself.

---

## CLI Usage

```sh
ilc new <project_name>   # Create a new project
ilc build                # Build the project
ilc run                  # Run the project
ilc add <lib_name>       # Add a library (planned)
ilc remove <lib_name>    # Remove a library (planned)
```

---

## Project Structure

When you run `ilc new my_project`, ILC creates:

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

Every ILC project has a `ilc.toml` configuration file:

```toml
[project]
name="my_project"
version="0.1.0"
flags="-O2 -Wall"

[dependencies]
array="1.0.0"
file="1.0.0"
```

---

## Remaining Libraries

| Library | Description | Status |
|---|---|---|
| `ilcTypes.h` | Type aliases (`u8`, `i32`, `f64`...) and vector structs | ✅ |
| `ilcArray.h` | Dynamic arrays for primitive types via X-Macro | ⚠️ bugs |
| `ilcDir.h` | Directory management (POSIX) | ⚠️ bugs |
| `ilcFile.h` | File I/O — read, write, copy, move | ⚠️ bugs |
| `ilcString.h` | Dynamic strings (`str_t`) | ⚠️ bugs |
| `ilcToml.h` | TOML configuration parser | ⚠️ bugs |

### Known Bugs

See [docs/TODO.md](./docs/TODO.md#bugs-conhecidos-nas-libs) for the full list of known issues.

---

## Dependencies

ILC manages only libraries within its own ecosystem. For external dependencies (e.g. SDL2), ILC will notify you to install them via your system package manager.

---

## Documentation

- [Design docs](./docs/design/) — architecture diagrams and notes
- [TODO](./docs/TODO.md) — known bugs and planned features

---

## License

[MIT](./LICENSE)

## Author

Isaac Estevan Geuster
[github](https://github.com/Ilx159) · [codeberg](https://codeberg.org/ilx159)
Email: ilx159@proton.me

 ## AI Contribution Policy

This project does **not** accept content written 100% by AI. AI tools are used only as an auxiliary aid to assist with thinking and organization, but they do not replace human ideas and judgment. Every contribution must reflect genuine human intent and understanding.


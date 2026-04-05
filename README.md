# ILC — I Love C

> A package manager and project scaffolder for C, with its own standard library ecosystem.

---

## What is ILC?

ILC is a tool that brings the convenience of modern package managers (like Rust's Cargo) to C development. It manages dependencies, scaffolds projects, and builds them — all from the command line.

It also comes with a growing ecosystem of header-only libraries designed to work seamlessly together, giving C developers a consistent and ergonomic standard library.

---

## CLI Usage

```sh
ilc new <project_name>   # Create a new project
ilc add <lib_name>       # Add a library from the ecosystem
ilc remove <lib_name>    # Remove a library
ilc build                # Build the project
ilc run                  # Run the project
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
name = "my_project"
version = "0.1.0"
compiler_flags = "-O2 -Wall"

[dependencies]
array = "1.0.0"
file = "1.0.0"
```

---

## Ecosystem Libraries

All libraries are **header-only** and follow the `#define LIB_IMPLEMENTATION` pattern.

| Library | Description |
|---|---|
| `types.h` | Type aliases (`u8`, `i32`, `f64`...) and math vector structs |
| `array.h` | Dynamic arrays for all primitive types, generated via X-Macro |
| `file.h` | File I/O — read, write, copy, move, delete, rename |
| `dir.h` | Directory management — create, delete, list, check existence |
| *(planned)* `error.h` | Error typing system based on `errno`/`perror` |

### Usage Example

```c
#define ARRAY_IMPLEMENTATION
#define FILE_IMPLEMENTATION
#include "array.h"
#include "file.h"

int main(void) {
    fileInfo_t f = fileOpen("data.bin", "rb");
    u8 *data = u8FileRead(&f);
    fileClose(&f);
    free(data);
    return 0;
}
```

---

## Dependencies

ILC manages only libraries within its own ecosystem. For external dependencies (e.g. SDL2), ILC will notify you to install them via your system package manager.

---

## Platform Support

| Platform | Status |
|---|---|
| Linux | ✅ Supported |
| macOS | ✅ Supported |
| Windows | 🔜 Planned |

The project uses the posix system

---

## Roadmap

- [x] `types.h`
- [x] `array.h`
- [x] `file.h`
- [x] `dir.h`
- [ ] `error.h`
- [ ] ILC CLI — `new`, `add`, `remove`, `build`, `run`
- [ ] Central repository of `.toml` package manifests
- [ ] Community repository support

---

## License

[MIT](./LICENSE)

---

## Author

Isaac Estevan Geuster

[github](https://github.com/Ilx159)

[codeberg](https://codeberg.org/ilx159)

Email: ilx159@proton.me

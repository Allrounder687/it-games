# 🧭 IT Games Steering & Architecture Guide

## 🎯 Project Mission & Principles
**IT Games** aims to build a responsive, modular, native PS5 C/C++ application. All architecture decisions must follow these core principles:

1. **Host-Independent Builds**: Build all payloads and native binaries via the containerized Docker environment (`ps5-sdk`). Do not introduce dependencies that require native Windows compiling tools.
2. **Minimal Kernel Footprint**: Payloads should be lightweight, perform cleanly, handle memory safety strictly, and never cause unhandled kernel panics on the console.
3. **Modular Subsystems**: Separate code into cleanly defined modules as the codebase expands:
   - `src/core/`: Application lifecycle, entrypoint, memory allocation.
   - `src/ui/`: Video output, font rendering, framebuffers.
   - `src/input/`: DualSense controller handling (`libScePad` / `libSceUserService`).
   - `src/net/`: Socket networking, asynchronous HTTP/HTTPS client.
   - `src/storage/`: File system I/O, package installation helpers.

---

## 🛠 Toolchain & Developer Conventions

### Compiler Environment
- **Base Container**: Ubuntu 22.04 with `clang-15`, `lld-15`, `python3-pyelftools`.
- **Target Triple**: `x86_64-sie-ps5` (FreeBSD/Prospero ABI).
- **SDK Path**: `/opt/ps5-payload-sdk`.
- **Wrapper Binary**: `prospero-clang` and `prospero-lld`.

### Line Endings & Formatting
- **LF Only**: All source files (`.c`, `.h`, `Makefile`, `.sh`) must maintain Linux `\n` (LF) line endings. Windows CRLF will break GNU Make inside Linux containers.
- **C Standard**: C11 / C++17 with `-Wall -Werror` enabled.

---

## 🔄 Development Workflows

### Compiling Payloads
```bash
# Build Docker image (first time or when Dockerfile changes)
docker build -t ps5-sdk .

# Compile ELF binary
docker run --rm -v "${PWD}:/app" ps5-sdk
```

### Adding New Native SCE APIs
When linking new Sony PlayStation libraries (e.g. `libScePad.so`, `libSceVideoOut.so`):
1. Check available stub definitions in `/opt/ps5-payload-sdk/sysroot/usr/lib/`.
2. Add necessary function declarations or headers under `include/` or include direct SDK headers.
3. Pass `-lSce<LibraryName>` to `CFLAGS` / `LDFLAGS` in the `Makefile`.

---

## 🧭 Milestone Roadmap & Next Steps
1. **Graphics Rendering Subsystem**: Initialize `libSceVideoOut` framebuffer for 1080p/4K display output.
2. **Input Processing**: Poll DualSense controller events via userland services.
3. **Network Stack**: Implement non-blocking socket downloader.

# 🎮 IT Games - Native PS5 Homebrew Application

[![PS5 Toolchain](https://img.shields.io/badge/Target-PS5%20(x86__64--sie--ps5)-blue)](https://github.com/john-tornblom/ps5-payload-sdk)
[![Build Tool](https://img.shields.io/badge/Build-Docker%20%2B%20Clang--15-orange)]()
[![License](https://img.shields.io/badge/License-GPLv3-green)](./LICENSE)

**IT Games** is a native PlayStation 5 homebrew application and payload framework built in C using the open-source [PS5 Payload SDK](https://github.com/john-tornblom/ps5-payload-sdk). It is designed to run natively on the PS5's customized FreeBSD kernel (Prospero OS) upon payload injection.

---

## 🌟 Features & Milestones

- **Native Execution**: Directly interacts with the PS5 userland and kernel space.
- **Dockerized Build System**: Clean, isolated containerized build environment with Clang-15, LLD-15, and the PS5 Payload SDK — no toolchain pollution on host OS (Windows/macOS/Linux).
- **System Notification Engine**: Native OS-level pop-up notifications utilizing `sceKernelSendNotificationRequest`.
- **Lightweight ELF Binary**: Produces an optimized, standalone `it_games.elf` payload.

---

## 🏗 Architecture Overview

```
+-------------------------------------------------------------+
|                      Host Machine                           |
|  [Source Code: main.c, Makefile]                            |
|                     | (Volume Mount)                        |
|                     v                                       |
|  [Docker Container: ps5-sdk (Ubuntu 22.04 + Clang-15)]     |
|                     |                                       |
|                     v (prospero-clang compiler)             |
|              [it_games.elf]                                 |
+-------------------------------------------------------------+
                              |
                              | (TCP / Wi-Fi Network Injection)
                              v
+-------------------------------------------------------------+
|                   PlayStation 5 (Target)                    |
|  Port 9020/9021 -> Payload Loader                           |
|       -> Memory Injection -> Kernel/Userland Execution      |
|       -> sceKernelSendNotificationRequest()                 |
|       -> TV Display: "Welcome to IT Games!"                 |
+-------------------------------------------------------------+
```

---

## 📋 Prerequisites

1. **Docker Desktop** (or Docker Engine on Linux)
2. **Git**
3. Target: Jailbroken PlayStation 5 (listening on port `9020` or `9021`)

---

## 🚀 Getting Started

### 1. Build the Docker Toolchain

Build the containerized PS5 SDK image once:

```bash
docker build -t ps5-sdk .
```

### 2. Compile the Payload

Run the container to compile `main.c` into `it_games.elf`:

**PowerShell (Windows):**
```powershell
docker run --rm -v "${PWD}:/app" ps5-sdk
```

**Bash (Linux / macOS):**
```bash
docker run --rm -v "$(pwd):/app" ps5-sdk
```

The output payload binary will be created at `./it_games.elf` (~73 KB).

---

## 📡 Deploying & Testing

To send the compiled payload to your PS5:

1. Ensure your PS5 is on the same local network and running the payload loader.
2. Set environment variables or pass host/port directly:
   ```bash
   # Using netcat
   nc -q0 <PS5_IP_ADDRESS> 9021 < it_games.elf
   ```
   *or using make:*
   ```bash
   make test PS5_HOST=<PS5_IP_ADDRESS> PS5_PORT=9021
   ```

---

## 🗺 Roadmap

- [x] **Milestone 1**: Dockerized Clang-15 PS5 toolchain setup & Hello World Notification ELF.
- [x] **Milestone 2**: Modular architecture, Hardware/SoC Telemetry engine, and Native HTTP/2 Network Subsystem (`libSceNet`/`libSceHttp2`).
- [ ] **Milestone 3**: 2D/3D Hardware-accelerated graphics rendering pipeline via `libSceVideoOut`.
- [ ] **Milestone 4**: DualSense controller input polling (`libScePad` / `libSceUserService`).
- [ ] **Milestone 5**: Full-screen interactive game browser & manager UI.

---

## 📄 License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0). See [LICENSE](./LICENSE) for details.

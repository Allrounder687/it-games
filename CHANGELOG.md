# Changelog

All notable changes to the **IT Games** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.4.0] - 2026-08-19

### Added
- **Full PS5 Standalone App Packaging Pipeline**:
  - Migrated from raw injected payloads to full native PlayStation 5 Homebrew Application format (`ITGA00001-app0`).
  - Automated `param.json` metadata generation targeting Title ID `ITGA00001` and Content ID `IV0001-ITGA00001_00-ITGAMES000000000`.
  - Automated asset generation for `icon0.png` (512x512 dashboard icon) and `pic0.png` / `pic1.png` (1920x1080 splash).
  - Executable conversion mapping `it_games.elf` -> `eboot.bin`.
  - Direct FTP deployment utility (`scripts/deploy_app.py`) deploying complete native app packages directly into `/data/homebrew/` for Itemzflow and ShadowMount launcher discovery.
  - Added `make app` and `make deploy-app` automation targets.

---

## [0.3.0] - 2026-08-19

### Added
- **PlayStation User Service Integration** (`src/user.c`, `include/user.h`):
  - Initialization of `sceUserService`.
  - Active user ID resolution via `sceUserServiceGetInitialUser`.
  - Account username / gamertag query via `sceUserServiceGetUserName`.
- **Display & Video Output Subsystem** (`src/display.c`, `include/display.h`):
  - Handle acquisition via `sceVideoOutOpen`.
  - Display resolution detection (1080p/4K) via `sceVideoOutGetResolutionStatus`.
- **Personalized Welcome Notification**:
  - Combined gamertag, PS5 hardware model, active screen resolution, and real-time SoC/CPU thermal stats.
- Compiled `it_games.elf` binary (92 KB) linking `libSceVideoOut`, `libSceUserService`, `libSceSystemService`, `libkernel_sys`, `libkernel`, `libSceNet`, `libSceSsl`, and `libSceHttp2`.

---

## [0.2.0] - 2026-08-19

### Added
- Modular architecture with clean subsystem separation under `src/` and `include/`.
- **System Telemetry Module** (`src/sysinfo.c`, `include/sysinfo.h`):
  - Hardware model detection via `sceKernelGetHwModelName`.
  - Serial number query via `sceKernelGetHwSerialNumber`.
  - CPU temperature and SoC temperature sensors via `sceKernelGetCpuTemperature` / `sceKernelGetSocSensorTemperature`.
  - CPU operating frequency reporting via `sceKernelGetCpuFrequency`.
- **PS5 Native Notification Engine** (`src/notify.c`, `include/notify.h`):
  - Formatted multi-argument pop-up notifications (`notify_send`).
- **Network Subsystem** (`src/net.c`, `include/net.h`):
  - Context pool initialization using `sceNet`, `sceSsl`, and `sceHttp2`.
  - HTTP/HTTPS GET request capabilities for game metadata and asset retrieval.
- Updated `Makefile` to compile modular sources against `libkernel_sys`, `libkernel`, `libSceSystemService`, `libSceUserService`, `libSceNet`, `libSceSsl`, and `libSceHttp2`.

---

## [0.1.0] - 2026-08-19

### Added
- Initial project scaffolding for **IT Games** native PS5 homebrew application.
- Dockerized build environment (`Dockerfile`) with Ubuntu 22.04, `clang-15`, `lld-15`, and the complete `ps5-payload-sdk`.
- Symlink configuration bridging `clang`/`clang++`/`lld` for `prospero-clang` wrapper compatibility.
- Basic payload implementation (`main.c`) utilizing `sceKernelSendNotificationRequest` to display "Welcome to IT Games!" on the PS5 system UI.
- Makefile targeting `it_games.elf` using the Prospero toolchain.
- Comprehensive documentation in `README.md` and steering guidelines in `STEERING.md` / `GEMINI.md`.
- `.gitignore` configured to ignore binary outputs (`*.elf`, `*.o`, temporary files).

---

## [0.0.1] - 2026-08-19

### Initial
- Project conception and toolchain discovery for PlayStation 5 homebrew development.

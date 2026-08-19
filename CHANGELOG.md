# Changelog

All notable changes to the **IT Games** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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

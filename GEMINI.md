# IT Games Project Context

## Overview
Native C/C++ PlayStation 5 Homebrew Application and Payload Framework.

## Build System
- Build is containerized via Docker (`Dockerfile` -> `ps5-sdk` image).
- Uses `prospero-clang` from `john-tornblom/ps5-payload-sdk` targeting `x86_64-sie-ps5`.
- Do not run Windows-native compilers directly for PS5 targets; always compile via `docker run --rm -v "${PWD}:/app" ps5-sdk`.
- Keep all files using LF line endings.

## Key Files
- `main.c`: Application entrypoint and payload logic.
- `Makefile`: Toolchain makefile targeting `it_games.elf`.
- `Dockerfile`: Toolchain container setup.
- `STEERING.md`: Architecture rules and development guidelines.
- `CHANGELOG.md`: Project version history.

# Contributing

Contributions are welcome. Please follow the guidelines below to keep the codebase consistent.

## Reporting Issues

Open an issue on GitHub with the following details:
- Hardware setup (sensor wiring, STM32 board variant)
- Arduino IDE version and STM32duino core version
- Serial output or error message
- Steps to reproduce

## Submitting Changes

1. Fork the repository and create a branch from `main`:
   ```bash
   git checkout -b fix/your-description
   ```
2. Make your changes. Keep commits focused and atomic.
3. Test on hardware before submitting.
4. Open a pull request against `main` with a clear description of the change and why it is needed.

## Code Style

- Use descriptive `#define` names for all thresholds and pin assignments.
- Comment each register write with its purpose.
- Keep `setup()` and `loop()` readable; move logic into named functions.
- Do not use blocking `while(1)` loops in production code; reserve them for init-time hardware checks only.

## Scope

This project intentionally targets the STM32F407VGTx with the listed sensor set. Pull requests that introduce dependencies on external libraries (beyond the STM32duino Wire library) should include justification.

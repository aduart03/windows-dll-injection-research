# Methodology

## Goal
Explore process accessibility across Windows user-level and protected processes as a precursor to controlled DLL injection experiments.

## Environment
- Windows host or VM
- C++ with MSVC
- ToolHelp API for process enumeration
- OpenProcess access-mask testing

## Procedure
1. Enumerate active processes by executable name.
2. Collect matching PIDs.
3. Attempt selected OpenProcess access masks.
4. Record success/failure and associated error codes.
5. Compare behavior across user-level and protected processes.

## Initial Observations
- User-level processes are generally accessible.
- Some system-critical processes return `ERROR_ACCESS_DENIED (5)`.
- Accessibility varies across some service-hosted processes.
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

## Phase 1: Local DLL Load Validation

### Objective
Verify that a custom DLL can be successfully loaded and executed within the current process.

### Procedure
1. Compile a test DLL (`payload.dll`) with a simple `DllMain`.
2. Use `LoadLibraryA` in a user-mode executable (`main.exe`) to load the DLL.
3. Confirm execution via a visible indicator (MessageBox).

### Result
- DLL loaded successfully using `LoadLibraryA`.
- `DllMain` executed as expected.
- MessageBox ("Payload DLL loaded") appeared.

### Notes
- Relative path used: `"..\\payload\\payload.dll"`
- Both executable and DLL compiled as x64
- Built using MSVC (`cl`)

## Phase 2: Baseline Target Selection

### Objective
Identify a suitable user-level process for controlled DLL injection experiments based on verified access rights.

### Target
- notepad.exe (user-level process)

### Procedure
1. Launch a benign user-level process (`notepad.exe`).
2. Enumerate the process and obtain its PID.
3. Use the access-check system to test required access rights.
4. Record success/failure for each access mask.

### Access Results
- PROCESS_VM_OPERATION: Success
- PROCESS_VM_WRITE: Success
- PROCESS_CREATE_THREAD: Success

### Interpretation
`notepad.exe` was selected as a baseline target because all required memory and thread-related access rights were successfully obtained. This makes it a valid candidate for controlled injection experiments.

### Notes
- Tested under current user context (admin)
- Used as baseline before evaluating restricted/system processes
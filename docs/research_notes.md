## Initial Findings

- User-level processes such as notepad.exe, explorer.exe, and cmd.exe allowed the tested OpenProcess access masks in the current session.
- System-critical processes such as lsass.exe, winlogon.exe, and services.exe consistently returned ERROR_ACCESS_DENIED (5).
- Different svchost.exe instances showed different accessibility results, suggesting that process name alone does not determine access behavior.
- Elevation and token privilege state may affect these results and should be tested comparatively in future experiments.

## Early Test Results

Process tested: notepad.exe  
Result: injection successful

Process tested: explorer.exe  
Result: injection successful

Process tested: lsass.exe  
Result: access denied
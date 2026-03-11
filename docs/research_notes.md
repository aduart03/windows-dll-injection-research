Process tested: notepad.exe
Result: injection successful

Process tested: explorer.exe
Result: injection successful

Process tested: lsass.exe
Result: access denied

## Initial Findings

- User-launched processes such as Notepad, Explorer, and CMD allowed all tested OpenProcess access masks in the current session.
- System-critical processes such as LSASS, Winlogon, and Services consistently returned ERROR_ACCESS_DENIED (5).
- svchost.exe showed mixed results across instances, indicating that executable name alone does not determine accessibility.
- Elevation state may influence results and should be tested comparatively.
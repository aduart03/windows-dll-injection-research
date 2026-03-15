# Test Matrix

| Process       | Category       | Result              | Notes |
|---------------|----------------|---------------------|-------|
| notepad.exe   | user-level     | success             | all tested masks succeeded |
| explorer.exe  | user-session   | success             | accessible in current session |
| lsass.exe     | protected      | access denied       | expected boundary |
| winlogon.exe  | protected      | access denied       | expected boundary |
| svchost.exe   | service-hosted | mixed               | depends on instance |
# Windows Process Access Research

This project studies Windows process visibility, access rights, and user/kernel boundary behavior through controlled research components.

## Current status
- Milestone 1: process enumeration
- Milestone 2: access-right analysis with OpenProcess
- Payload source included for controlled local testing
- Logging of success/failure and Win32 error codes

## Repository structure
- `src/` – user-mode source code
- `payload/` – test DLL source
- `logs/` – experiment outputs
- `docs/` – methodology and notes

## Build
Explain how to build the main tool and the payload DLL.

## Notes
Compiled binaries are not tracked as source artifacts; build locally from the included code.

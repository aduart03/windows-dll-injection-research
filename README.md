Milestone 2 – Process Access Analysis

Implemented a Windows C++ tool that:

• Enumerates running processes using ToolHelp32
• Matches target processes by executable name
• Supports multiple instances of the same executable
• Tests several OpenProcess access rights
• Reports success/failure and Win32 error codes
• Detects whether the program is running with admin privileges

Initial experiments show that user-level processes allow requested
access rights while system-critical processes return ERROR_ACCESS_DENIED.
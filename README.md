# BVInjector

BVInjector is a modern, user-friendly, and stealthy DLL injection library for Windows, designed to be easily embedded into any C++ console application or empty project. It features a plug-and-play system, automatic DLL folder management, and a robust manual mapping injector that avoids the limitations and detection risks of traditional injection methods.

# Features
Plug-and-Play Integration
Easily add a full-featured DLL injector menu to your app by including the library and calling a single macro in your main().
Modern Console Menu
Interactive console UI for selecting DLLs and target processes, with clear prompts and error handling.
Automatic DLL Folder Management
Automatically creates a dlls folder next to your executable and lists all .dll files for easy selection.
Process Selection by Name
Enter the name of any running process (e.g., notepad.exe) to inject your DLL.
Manual Mapping Injection
Uses advanced manual mapping to inject DLLs, bypassing many anti-cheat and anti-malware protections.
Clear and Consistent Logging
All logs are prefixed with bvinjector: for easy identification and grouped for readability.
Cross-Platform Console Clearing
Clears the console after each injection for a clean user experience.
No Code Deletion or Feature Loss
All original injector features and methods are preserved and accessible.

# How It Works
Include and Start
Add #include "bvinjector.h" to your project.
Call bvinjectorstart(); in your main().
Run Your App
The menu appears automatically.
Option 1: Inject DLL
Lists all DLLs in the dlls folder.
Prompts for the target process name.
Performs manual mapping injection.
Option 0: Exit

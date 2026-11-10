# Auto Decorator X

A Geode mod project for Geometry Dash, targeting Android 64-bit and Windows 64-bit.

## Planned features
- Smart decoration suggestions
- One-click decoration workflow
- Adjustable decoration settings
- Color presets
- Glow/effect presets
- Undo support

## Current status
This commit establishes the Geode project structure, mod metadata, a main-menu entry point, and CI builds. The actual editor-side decoration engine is **not implemented yet**; the menu button currently opens a status panel and does not modify levels.

## Build
The GitHub Actions workflow builds installable Geode packages for Android64 and Windows x64. Download the packages from the successful workflow run Artifacts section.

## Install
Copy the matching Geode package into your Geode mods folder, then restart Geometry Dash.

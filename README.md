# Auto Decorator X

A Geode mod project for Geometry Dash, targeting Android 64-bit and Windows 64-bit.

## Planned features
- Smart decoration suggestions
- One-click decoration workflow
- Adjustable decoration settings
- Color presets
- Glow/effect presets
- Undo support

## Current status\n\nThe mod now adds an Auto Decorator X shortcut button directly inside the Geometry Dash level editor. Tapping it opens the first menu preview with sections for style presets, color themes, glow effects, and decorating selected objects. These controls are UI placeholders for now: automatic decoration and level-object editing are not implemented yet.\n
This commit establishes the Geode project structure, mod metadata, a main-menu entry point, and CI builds. The actual editor-side decoration engine is **not implemented yet**; the menu button currently opens a status panel and does not modify levels.

## Build
The GitHub Actions workflow builds installable Geode packages for Android64 and Windows x64. Download the packages from the successful workflow run Artifacts section.

## Install
Copy the matching Geode package into your Geode mods folder, then restart Geometry Dash.


## Effect selector update

The editor now includes an FX shortcut that opens a panel with interactive Glow, Pulse, Particles, and Outline toggles. These currently store selection state for the editor session and log changes; they do not yet apply visual effects to level objects. The decoration engine remains in development.


## Gemini AI integration

The selected experience is **Both**: manual suggestions and assisted automatic decoration with a preview and explicit Apply confirmation.

- Integration architecture: [docs/GEMINI_AI_INTEGRATION.md](docs/GEMINI_AI_INTEGRATION.md)
- Secure Gemini proxy starter: [backend/gemini-proxy/README.md](backend/gemini-proxy/README.md)

The proxy starter is not deployed and the Geode client is not connected to it yet. The Gemini API key must be stored as an encrypted server secret, never in this repository or inside the mod. The decoration engine and Undo implementation still need to be completed and tested.

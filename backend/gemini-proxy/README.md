# Auto Decorator X — Gemini proxy starter

This is a starter backend for the optional Gemini-powered decoration planner.

## Required server settings

- `GEMINI_API_KEY`: encrypted secret in the hosting provider's secret manager.
- `GEMINI_MODEL`: a currently supported Gemini model ID.

Never put either value in the public Git repository. In particular, do not put a Gemini key in the Geode mod, `mod.json`, GitHub Actions logs, or a mobile/desktop client. Google recommends a backend proxy for client applications because keys embedded in clients can be extracted.

## Deploy

1. Create a Cloudflare Worker for this folder.
2. Deploy the Worker.
3. Add `GEMINI_API_KEY` using the provider's encrypted secret setting.
4. Set `GEMINI_MODEL` to a model currently enabled for your Google AI project.
5. Test `POST /api/decorate` with a small JSON body such as:
   `{"style":"glow","description":"blue cyber city","selectedObjects":[{"type":"block","color":"blue"}]}`

The Worker returns a structured suggestion plan. It does not edit a Geometry Dash level.

## Safety boundary

The response is suggestions only. The Geode mod must validate every value, show a preview, and require the player to press Apply. Only a separate, locally implemented allowlist-based decoration engine may make supported editor changes. Keep Undo available and never execute model-generated code or arbitrary object commands.

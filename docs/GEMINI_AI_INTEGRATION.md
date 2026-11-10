# Gemini AI integration plan

## Chosen experience: Both modes

Auto Decorator X will support:
- **Manual mode:** Gemini suggests a style, palette, and effect combination; the player chooses what to apply.
- **Assisted automatic mode:** Gemini proposes a complete decoration plan; the player previews it and confirms Apply. It is not silent or irreversible automation.

## Architecture

1. Geometry Dash editor UI collects only a compact, non-identifying summary of selected object types/colors and user preferences.
2. The Geode client sends that summary over HTTPS to the configured Auto Decorator X proxy.
3. The proxy stores the Gemini key as an encrypted server secret and calls the Gemini API.
4. Gemini returns JSON constrained by a response schema.
5. The proxy validates the palette and effect allowlist before returning the plan.
6. The mod validates again, displays a preview, then applies only supported local decoration operations after explicit confirmation.
7. Undo records every local change so the player can revert the operation.

## Initial AI response contract

- style: one of modern, glow, minimal, tech, nature, retro
- palette: primary, secondary, accent, background as #RRGGBB
- effects: only glow, pulse, particles, outline
- density: integer from 0 to 100
- notes: short explanation

The AI response is data, never executable code. Do not let model output directly call Geode APIs or mutate arbitrary level objects.

## Secrets and privacy

- Never commit Gemini API keys.
- Never compile a Gemini API key into the Android or Windows mod.
- Store the key only as an encrypted backend secret.
- Send only the minimum level summary needed; do not upload complete level files by default.
- Add rate limits, request-size limits, error handling, and usage monitoring before public deployment.

## Delivery phases

1. **Backend starter:** Cloudflare Worker proxy, schema-constrained Gemini response, validation.
2. **Editor connection:** HTTPS request client, loading/error states, manual/assisted mode selector.
3. **Preview UI:** show palette, style, and effect toggles before applying.
4. **Decoration engine:** implement supported local object edits, snapshots, and Undo.
5. **Verification:** build Android64 and Win64 packages; test network failure, malformed responses, and Undo on both platforms.

## Current status

The backend starter is a foundation only. The Geode mod is not yet connected to the proxy, and the decoration engine is not implemented. A deployment URL and encrypted server secret are still required before live Gemini requests can work.

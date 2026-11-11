const JSON_HEADERS = { "content-type": "application/json; charset=utf-8" };

const responseSchema = {
  type: "OBJECT",
  properties: {
    style: { type: "STRING", enum: ["modern", "glow", "minimal", "tech", "nature", "retro"] },
    palette: {
      type: "OBJECT",
      properties: {
        primary: { type: "STRING" },
        secondary: { type: "STRING" },
        accent: { type: "STRING" },
        background: { type: "STRING" }
      },
      required: ["primary", "secondary", "accent", "background"]
    },
    effects: {
      type: "ARRAY",
      items: { type: "STRING", enum: ["glow", "pulse", "particles", "outline"] }
    },
    density: { type: "INTEGER" },
    notes: { type: "STRING" }
  },
  required: ["style", "palette", "effects", "density", "notes"]
};

function json(data, status = 200) {
  return new Response(JSON.stringify(data), {
    status,
    headers: {
      ...JSON_HEADERS,
      "access-control-allow-origin": "*"
    }
  });
}

export default {
  async fetch(request, env) {
    if (request.method === "OPTIONS") {
      return new Response(null, {
        status: 204,
        headers: {
          "access-control-allow-origin": "*",
          "access-control-allow-methods": "POST, OPTIONS",
          "access-control-allow-headers": "content-type"
        }
      });
    }

    const url = new URL(request.url);
    if (url.pathname !== "/api/decorate") return json({ error: "Not found" }, 404);
    if (request.method !== "POST") return json({ error: "Use POST" }, 405);

    // Try the preferred model, then lighter stable fallbacks if Google reports capacity pressure.
    const models = ["gemini-3.8-flash", "gemini-3.6-flash", "gemini-3.5-flash-lite"];
    if (!env.GEMINI_API_KEY) {
      return json({ error: "Missing GEMINI_API_KEY secret. Add it under Worker Settings > Variables and Secrets > Secrets." }, 503);
    }

    const declaredLength = Number(request.headers.get("content-length") || 0);
    if (declaredLength > 16_384) return json({ error: "Request too large" }, 413);

    let input;
    try {
      input = await request.json();
    } catch {
      return json({ error: "Invalid JSON" }, 400);
    }

    const allowedStyles = ["modern", "glow", "minimal", "tech", "nature", "retro"];
    const style = typeof input.style === "string" && allowedStyles.includes(input.style)
      ? input.style : "modern";
    const description = typeof input.description === "string"
      ? input.description.slice(0, 1000) : "";
    const objectSummary = Array.isArray(input.selectedObjects)
      ? input.selectedObjects.slice(0, 100).map((item) => ({
          type: String(item?.type || "unknown").slice(0, 40),
          color: String(item?.color || "unknown").slice(0, 24)
        }))
      : [];

    const prompt = [
      "You are Auto Decorator X, an assistant for Geometry Dash level decoration.",
      "Return a safe, practical decoration suggestion using only the allowed schema.",
      "Do not return code, scripts, object IDs, executable commands, or instructions to modify files.",
      "Choose effects only from glow, pulse, particles, outline.",
      "Choose colors as hex strings. Keep density between 0 and 100.",
      "User's preferred style: " + style,
      "Level description: " + description,
      "Selected object summary: " + JSON.stringify(objectSummary)
    ].join("\n");

    let upstream = null;
    let lastError = null;
    let usedModel = models[0];

    for (const model of models) {
      usedModel = model;
      const endpoint = "https://generativelanguage.googleapis.com/v1beta/models/"
        + encodeURIComponent(model) + ":generateContent";
      const maxAttempts = 1;

      for (let attempt = 0; attempt < maxAttempts; attempt++) {
        try {
          upstream = await fetch(endpoint, {
            signal: AbortSignal.timeout(12_000),
            method: "POST",
            headers: {
              "content-type": "application/json",
              "x-goog-api-key": env.GEMINI_API_KEY
            },
            body: JSON.stringify({
              contents: [{ role: "user", parts: [{ text: prompt }] }],
              generationConfig: {
                responseMimeType: "application/json",
                responseSchema
              }
            })
          });
        } catch (error) {
          lastError = { status: 504, message: error?.name === "TimeoutError" ? "Gemini took too long to respond (12-second limit)" : "Could not reach Gemini" };
          if (attempt + 1 < maxAttempts) {
            await new Promise((resolve) => setTimeout(resolve, 700 * (attempt + 1)));
            continue;
          }
          break;
        }

        if (upstream.ok) break;

        let errorMessage = "Gemini request failed";
        try {
          const errorBody = await upstream.clone().json();
          errorMessage = String(errorBody?.error?.message || errorMessage).slice(0, 300);
        } catch {
          // Keep the generic message if Google did not return JSON.
        }
        lastError = { status: upstream.status, message: errorMessage };

        const retryable = [408, 429, 500, 502, 503, 504].includes(upstream.status);
        if (!retryable) {
          return json({
            error: "Gemini request failed",
            upstreamStatus: upstream.status,
            details: errorMessage,
            model
          }, 502);
        }

        if (attempt + 1 < maxAttempts) {
          // Exponential backoff with a small deterministic jitter.
          const delay = Math.min(4000, 800 * (2 ** attempt)) + (attempt * 173);
          await new Promise((resolve) => setTimeout(resolve, delay));
        }
      }

      if (upstream?.ok) break;
      upstream = null;
    }

    if (!upstream?.ok) {
      return json({
        error: "Gemini is temporarily unavailable on all fallback models",
        upstreamStatus: lastError?.status || 503,
        details: lastError?.message || "No response from Gemini",
        triedModels: models
      }, 503);
    }

    try {
      const result = await upstream.json();
      const output = result?.candidates?.[0]?.content?.parts?.[0]?.text;
      if (!output) throw new Error("Missing model output");
      const plan = JSON.parse(output);
      const hex = /^#[0-9a-fA-F]{6}$/;
      for (const key of ["primary", "secondary", "accent", "background"]) {
        if (typeof plan.palette?.[key] !== "string" || !hex.test(plan.palette[key])) {
          throw new Error("Invalid palette");
        }
      }
      plan.density = Math.max(0, Math.min(100, Number(plan.density) || 0));
      plan.effects = Array.isArray(plan.effects)
        ? plan.effects.filter((effect) => ["glow", "pulse", "particles", "outline"].includes(effect))
        : [];
      return json({ plan, model: usedModel });
    } catch {
      return json({ error: "Gemini returned an invalid decoration plan" }, 502);
    }
  }
};

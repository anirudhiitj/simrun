const { app, BrowserWindow, ipcMain } = require('electron');
const { spawn, execFile } = require('child_process');
const path = require('path');
const fs = require('fs');
const os = require('os');
const http = require('http');

let compilerProcess = null;

function startCompiler() {
  const compilerPath = path.resolve(__dirname, '../../compiler/compiler.exe');
  const compilerDir = path.dirname(compilerPath);

  console.log("Launching compiler at:", compilerPath);
  console.log("Working directory:", compilerDir);

  compilerProcess = spawn(compilerPath, [], {
    stdio: 'inherit',
    windowsHide: true,
    cwd: compilerDir  // Set working directory to compiler folder
  });

  compilerProcess.on('error', (err) => {
    console.error('Failed to start compiler:', err);
  });

  compilerProcess.on('exit', (code) => {
    console.log('Compiler exited with code', code);
  });
}


function createWindow() {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  });

  win.loadURL('http://localhost:5173');
}

/* ---------------- IPC HANDLERS ---------------- */

ipcMain.handle('ping-electron', async () => {
  return 'Pong from Electron main process';
});

function postToCompiler(data) {
  return new Promise((resolve, reject) => {
    const jsonData = JSON.stringify(data);

    console.log("\n[Electron Main] ➡ Sending POST to http://127.0.0.1:8081/compile");
    console.log("[Electron Main] Request body:", JSON.stringify(data, null, 2));
    console.log("[Electron Main] Payload size:", jsonData.length, "bytes");

    const options = {
      hostname: '127.0.0.1',
      port: 8081,
      path: '/compile',
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Content-Length': Buffer.byteLength(jsonData)
      }
    };

    const req = http.request(options, (res) => {
      console.log("[Electron Main] ⬅ Response status:", res.statusCode);

      let body = '';
      res.on('data', chunk => body += chunk);
      res.on('end', () => {
        console.log("[Electron Main] ⬅ Response received");
        console.log("[Electron Main] Raw response body:", body);
        try {
          const parsed = JSON.parse(body);
          console.log("[Electron Main] Response parsed successfully");
          resolve(parsed);
        } catch {
          console.error("[Electron Main] Failed to parse response as JSON");
          resolve({ status: "error", raw: body });
        }
      });
    });

    req.on('error', (err) => {
      console.error("[Electron Main] HTTP request failed:", err.message);
      reject(err);
    });

    req.write(jsonData);
    req.end();
  });
}


/* ---------------- SIMULATE: spawn simrun.exe with IR file ---------------- */

function runSimulation(enrichedIR) {
  return new Promise((resolve, reject) => {
    const simrunPath = path.resolve(__dirname, '../../sim/simrun.exe');

    // Write enriched IR to a temp file
    const tmpDir = os.tmpdir();
    const tmpFile = path.join(tmpDir, `simrun_ir_${Date.now()}.json`);
    fs.writeFileSync(tmpFile, JSON.stringify(enrichedIR), 'utf-8');

    console.log("[Electron Main] Wrote enriched IR to:", tmpFile);
    console.log("[Electron Main] Spawning simrun.exe at:", simrunPath);

    let stdout = '';
    let stderr = '';

    const proc = spawn(simrunPath, [tmpFile], {
      windowsHide: true,
      cwd: path.dirname(simrunPath)
    });

    proc.stdout.on('data', (chunk) => {
      stdout += chunk.toString();
    });

    proc.stderr.on('data', (chunk) => {
      stderr += chunk.toString();
    });

    proc.on('error', (err) => {
      console.error("[Electron Main] simrun.exe failed to start:", err.message);
      // Clean up temp file
      try { fs.unlinkSync(tmpFile); } catch (_) {}
      reject(err);
    });

    proc.on('close', (code) => {
      console.log("[Electron Main] simrun.exe exited with code:", code);
      console.log("[Electron Main] simrun stdout:", stdout);
      if (stderr) console.error("[Electron Main] simrun stderr:", stderr);

      // Clean up temp file
      try { fs.unlinkSync(tmpFile); } catch (_) {}

      try {
        const parsed = JSON.parse(stdout.trim());
        resolve(parsed);
      } catch (parseErr) {
        resolve({
          status: code === 0 ? "completed" : "error",
          phase: "simulation",
          raw_stdout: stdout,
          raw_stderr: stderr,
          exit_code: code
        });
      }
    });
  });
}


ipcMain.handle('simulate-project', async (_, projectJson) => {
  console.log("\n[Electron Main] ====== SIMULATE-PROJECT ======");
  try {
    /* --- Step 1: Compile IR (enriches with profile defaults) --- */
    console.log("[Electron Main] Step 1: Compiling IR via compiler.exe...");
    const enrichedIR = await postToCompiler(projectJson);

    // Check if compiler returned an error
    if (enrichedIR.status === "error" || enrichedIR.valid === false) {
      console.log("[Electron Main] Compiler returned error:", enrichedIR);
      return {
        status: "error",
        phase: "compilation",
        compiler_result: enrichedIR
      };
    }

    console.log("[Electron Main] Step 1 done. Enriched IR received.");

    /* --- Step 2: Run simulation engine --- */
    console.log("[Electron Main] Step 2: Running simrun.exe...");
    const simResult = await runSimulation(enrichedIR);
    console.log("[Electron Main] Step 2 done. Sim result:", simResult);

    /* --- Return combined result --- */
    return {
      status: simResult.status || "completed",
      phase: "simulation",
      compilation: { status: "ok" },
      simulation: simResult
    };

  } catch (err) {
    console.error("[Electron Main] Pipeline failed:", err.message);
    return {
      status: "error",
      phase: "system",
      valid: false,
      errors: [{ message: err.message }]
    };
  }
});


/* ---------------- APP LIFECYCLE ---------------- */

app.whenReady().then(() => {
  startCompiler();
  createWindow();
});

app.on('before-quit', () => {
  if (compilerProcess) {
    compilerProcess.kill();
  }
});

app.on('window-all-closed', () => {
  app.quit();
});

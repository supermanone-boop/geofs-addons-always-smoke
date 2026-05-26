'use strict';

(() => {

let activeSmoke = null;
let colorLoop = null;

// =====================================================
// UI作成（右上）
// =====================================================

const ui = document.createElement("div");

Object.assign(ui.style, {
    position: "fixed",
    top: "10px",
    right: "10px",
    zIndex: 99999,
    background: "rgba(0,0,0,0.6)",
    padding: "8px",
    borderRadius: "8px",
    color: "#fff",
    fontSize: "12px",
    fontFamily: "sans-serif",
    width: "140px"
});

ui.innerHTML = `
<div style="margin-bottom:6px;">BROWN SMOKE</div>
<button id="toggle" style="
    width:100%;
    padding:6px;
    background:#333;
    color:#fff;
    border:none;
    border-radius:6px;
    cursor:pointer;
">OFF</button>
`;

document.body.appendChild(ui);

// =====================================================
// スモークON
// =====================================================

function startSmoke() {

    const ac = geofs?.aircraft?.instance;

    if (!ac || !ac.engines) {
        console.warn("Aircraft not ready");
        return;
    }

    const brown = new Cesium.Color(0.45, 0.25, 0.10, 1.0);

    activeSmoke = [];

    ac.engines.forEach(engine => {

        const emitter = new geofs.fx.ParticleEmitter({

            anchor: engine.points?.contrailAnchor || {
                worldPosition: engine.object3d.worldPosition
            },

            duration: 1e10,
            rate: 0.05,
            life: 2500,

            startScale: 0.01,
            endScale: 0.12,

            randomizeStartScale: 0.01,
            randomizeEndScale: 0.2,

            startOpacity: 1,
            endOpacity: 0.2,

            startRotation: "random",
            texture: "smoke",

            velocity: new Cesium.Cartesian3(0, 0, 1000)

        });

        activeSmoke.push(emitter);
    });

    // 色ループ（1回だけ）
    colorLoop = setInterval(() => {
        geofs.fx.setParticlesColor(brown);
    }, 50);

    console.log("[Smoke] BROWN ON");
}

// =====================================================
// スモークOFF
// =====================================================

function stopSmoke() {

    if (activeSmoke) {
        activeSmoke.forEach(e => e.destroy?.());
        activeSmoke = null;
    }

    if (colorLoop) {
        clearInterval(colorLoop);
        colorLoop = null;
    }

    console.log("[Smoke] OFF");
}

// =====================================================
// toggle
// =====================================================

function toggle() {

    const btn = document.getElementById("toggle");

    if (!activeSmoke) {
        startSmoke();
        btn.innerText = "ON";
    } else {
        stopSmoke();
        btn.innerText = "OFF";
    }
}

document.getElementById("toggle")
    .addEventListener("click", toggle);

console.log("[UI Smoke] READY");

})();
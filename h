'use strict';

(() => {

let activeSmoke = null;
let interval = null;

const btn = document.createElement("button");

btn.innerText = "DUST OFF";

Object.assign(btn.style, {
    position: "fixed",
    top: "10px",
    right: "10px",
    zIndex: 99999,
    padding: "6px 10px",
    background: "#222",
    color: "#fff",
    border: "1px solid #555",
    borderRadius: "6px",
    cursor: "pointer"
});

document.body.appendChild(btn);

function toggleDust() {

    const ac = geofs?.aircraft?.instance;

    if (!ac || !ac.engines) {
        console.warn("Aircraft not ready yet.");
        return;
    }

    // OFF
    if (activeSmoke) {

        activeSmoke.forEach(e => e.destroy?.());
        activeSmoke = null;

        if (interval) {
            clearInterval(interval);
            interval = null;
        }

        btn.innerText = "DUST OFF";
        console.log("[Dust] stopped");
        return;
    }

    // =====================================================
    // 土煙カラー（ここがポイント）
    // =====================================================

    const dustColor = new Cesium.Color(0.55, 0.45, 0.30, 1.0);

    activeSmoke = [];

    ac.engines.forEach(engine => {

        const emitter = new geofs.fx.ParticleEmitter({

            anchor:
                engine.points?.contrailAnchor ||
                { worldPosition: engine.object3d.worldPosition },

            duration: 1e10,
            rate: 0.08,

            life: 3000,

            startScale: 0.05,
            endScale: 1.5,

            randomizeStartScale: 0.02,
            randomizeEndScale: 0.5,

            startOpacity: 0.9,
            endOpacity: 0.0,

            startRotation: "random",

            texture: "whitesmoke", // ←これ固定（GeoFS仕様）

            velocity: new Cesium.Cartesian3(0, 0, 30) // ゆっくり舞う＝土煙っぽさ

        });

        activeSmoke.push(emitter);
    });

    // 色を“土色固定”
    interval = setInterval(() => {
        geofs.fx.setParticlesColor(dustColor);
    }, 50);

    btn.innerText = "DUST ON";

    console.log("[Dust] ON");
}

btn.addEventListener("click", toggleDust);

console.log("[GeoFS DUST READY]");

})();
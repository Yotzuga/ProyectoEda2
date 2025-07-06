const BASE_URL = "http://localhost:18080";

// ----------- Usuarios (Hash Table) -----------
export async function getUsuarios() {
    const res = await fetch(`${BASE_URL}/usuarios`);
    return res.json();
}

export async function registrarUsuario(dni, perfil) {
    const res = await fetch(`${BASE_URL}/usuario`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ dni, perfil }),
    });
    return res;
}

export async function buscarUsuario(dni) {
    const res = await fetch(`${BASE_URL}/usuario/${dni}`);
    return res.json();
}

export async function actualizarPerfil(dni, perfil) {
    const res = await fetch(`${BASE_URL}/usuario/${dni}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ perfil }),
    });
    return res;
}

// ----------- Cola (Max Heap) -----------
export async function encolarUsuario(dni, ts) {
    // ts debe ser un número en segundos
    const res = await fetch(`${BASE_URL}/cola`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ dni, ts }),
    });
    return res;
}

export async function getTop5Cola() {
    const res = await fetch(`${BASE_URL}/cola/top5`);
    return res.json();
}

export async function extraerSiguienteCola() {
    const res = await fetch(`${BASE_URL}/cola/extract`, { method: "POST" });
    if (res.status === 204) return null;
    return res.json();
}

export async function actualizarPrioridadCola(dni, nuevo_perfil) {
    const res = await fetch(`${BASE_URL}/cola/update`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ dni, nuevo_perfil }),
    });
    return res;
}

// ----------- Accesos (AVL) -----------
export async function registrarAcceso(zona, ts) {
    const res = await fetch(`${BASE_URL}/acceso`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ zona, ts }),
    });
    return res;
}

export async function getAccesosRango(inicio, fin) {
    const res = await fetch(`${BASE_URL}/accesos/rango?inicio=${inicio}&fin=${fin}`);
    return res.json();
}

export async function getZonaTop() {
    const res = await fetch(`${BASE_URL}/accesos/zona_top`);
    return res.text();
}

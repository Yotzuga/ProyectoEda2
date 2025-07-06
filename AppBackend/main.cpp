#include "httplib.h"
#include "json.hpp"
#include "hash_table.h"
#include "max_heap.h"
#include "avl_tree.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using json = nlohmann::json;
using namespace httplib;

// Estructuras principales
TablaHash usuarios;
MaxHeap heap;
ArbolAVL arbol;

// Cargar datos y medir tiempos y espacio para análisis técnico
void cargarDatosInicialesPruebaTecnica(const std::string &path)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        std::printf("Error: no se pudo abrir %s\n", path.c_str());
        return;
    }
    json jdata;
    ifs >> jdata;
    ifs.close();
    //////////////////////////////////////////////////////////////////////////
    // --- Medición Tabla Hash ----
    //////////////////////////////////////////////////////////////////////////
    // --- Prueba técnica: insertar usuarios en la tabla hash
    TablaHash tablaTest;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (const auto &u : jdata["usuarios"])
    {
        tablaTest.insertar(u["dni"], u["perfil"]);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[Hash] Insertar " << jdata["usuarios"].size() << " usuarios: "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";

    // --- Prueba técnica: buscar
    // Nota: validar() internamente llama a buscar(), por lo que esta prueba cubre ambas funciones.
    t1 = std::chrono::high_resolution_clock::now();
    int encontrados = 0;
    long dni_actual = 20000000; // Un rango fuera del existente
    for (size_t i = 0; i < jdata["usuarios"].size(); ++i)
    {
        if (tablaTest.buscar(dni_actual))
            ++encontrados;
        --dni_actual;
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[Hash] Buscar/Validar " << jdata["usuarios"].size() << " usuarios (no existen): "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms (encontrados: " << encontrados << ")\n";

    //////////////////////////////////////////////////////////////////////////
    // --- Medición MaxHeap ---
    //////////////////////////////////////////////////////////////////////////
    // --- Prueba técnica: insertar usuarios en el heap
    MaxHeap heapTest;
    t1 = std::chrono::high_resolution_clock::now();
    int insertados = 0;
    for (const auto &u : jdata["usuarios"])
    {
        long dni = u["dni"];
        std::string perfil = u["perfil"];
        long ts = 1720406400 + (std::rand() % (60 * 60 * 24)); // Un día aleatorio

        NodoHash *nodo = tablaTest.buscar(dni);
        if (!nodo)
            continue;
        if (nodo->enCola || nodo->atendido)
            continue;

        heapTest.insertar(dni, perfil, ts);
        nodo->enCola = true;
        ++insertados;
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[Heap] Insertar " << insertados << " usuarios (siguiendo logica /cola): "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";

    // --- Prueba técnica: cambiar prioridad de todos los usuarios en el heap (aleatorio) ---
    t1 = std::chrono::high_resolution_clock::now();
    int cambios = 0;
    for (int i = 0; i < jdata["usuarios"].size(); ++i)
    {
        long dni = jdata["usuarios"][i]["dni"];
        int nueva_prioridad = 1 + (std::rand() % 5); // Prioridad aleatoria entre 1 y 5
        int idx = heapTest.buscarIndice(dni);
        if (idx != -1)
        {
            heapTest.actualizarPrioridad(idx, nueva_prioridad);
            ++cambios;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[Heap] Cambiar prioridad aleatoria a todos los usuarios (" << cambios << "): "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";

    // --- Prueba técnica: ver los 5 usuarios con mayor prioridad (antes de vaciar el heap) ---
    t1 = std::chrono::high_resolution_clock::now();
    int topCount = 0;
    Elemento *top5 = heapTest.verTop5(topCount);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[Heap] verTop5 ejecutado, usuarios devueltos: " << topCount
              << ", tiempo: " << std::chrono::duration<double, std::micro>(t2 - t1).count() << " us\n";
    for (int i = 0; i < topCount; ++i)
    {
        std::cout << "  DNI: " << top5[i].dni << ", prioridad: " << top5[i].prioridad << ", ts: " << top5[i].ts << "\n";
    }
    delete[] top5;

    // --- Prueba técnica: extraer usuarios del heap (todos) ---
    // Esto simula que todos los usuarios en cola han sido atendidos y el heap queda vacío.
    t1 = std::chrono::high_resolution_clock::now();
    int extraidos = 0;
    while (!heapTest.estaVacio())
    {
        heapTest.extraerMax();
        ++extraidos;
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[Heap] ExtraerMax " << extraidos << " usuarios: "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";

    //////////////////////////////////////////////////////////////////////////
    // --- Medición AVL ---
    //////////////////////////////////////////////////////////////////////////
    // --- Prueba técnica: insertar accesos en el árbol AVL (lógica realista por perfil) ---
    ArbolAVL avlTest;
    long base_ts = 1720406400; // inicio del día
    int i_vip = 0, i_med = 0, i_seg = 0, i_disc = 0, i_pub = 0;
    const long intervalo = 7 * 60; // 7 minutos

    long min_ts = std::numeric_limits<long>::max();
    long max_ts = std::numeric_limits<long>::min();

    t1 = std::chrono::high_resolution_clock::now();
    for (const auto &u : jdata["usuarios"])
    {
        std::string perfil = u["perfil"];
        long ts = 0;
        int min_offset = std::rand() % 7; // offset aleatorio de 0 a 6 minutos
        if (perfil == "vip")
        {
            ts = base_ts + i_vip * intervalo + min_offset * 60;
            avlTest.insertar("puerta-vip", ts);
            ++i_vip;
        }
        else if (perfil == "personal-medico")
        {
            ts = base_ts + i_med * intervalo + min_offset * 60;
            avlTest.insertar("puerta-medico", ts);
            ++i_med;
        }
        else if (perfil == "seguridad")
        {
            ts = base_ts + i_seg * intervalo + min_offset * 60;
            avlTest.insertar("puerta-staff", ts);
            ++i_seg;
        }
        else if (perfil == "discapacitados")
        {
            ts = base_ts + i_disc * intervalo + min_offset * 60;
            avlTest.insertar("puerta-ada", ts);
            ++i_disc;
        }
        else if (perfil == "publico-general")
        {
            ts = base_ts + i_pub * intervalo + min_offset * 60;
            avlTest.insertar("puerta-general", ts);
            ++i_pub;
        }
        // Actualiza min/max timestamp
        if (ts < min_ts)
            min_ts = ts;
        if (ts > max_ts)
            max_ts = ts;
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[AVL] Insertar " << jdata["usuarios"].size() << " accesos: "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";

    // --- Prueba técnica: consultar zona con más entradas ---
    long ini = min_ts;
    long fin = max_ts;

    // Conversión de timestamp a fecha legible
    auto print_fecha = [](long ts)
    {
        std::time_t t = static_cast<std::time_t>(ts);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::gmtime(&t));
        return std::string(buf);
    };

    t1 = std::chrono::high_resolution_clock::now();
    auto rango = avlTest.rangoTiempos(ini, fin);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[AVL] Consulta rango (" << ini << " - " << fin << ") ["
              << print_fecha(ini) << " - " << print_fecha(fin) << "]: "
              << std::chrono::duration<double, std::micro>(t2 - t1).count() << " us, resultados: " << rango.size() << "\n";
}

// Cargar datos desde data.json
void cargarDatosIniciales(const std::string &path)
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Semilla para random

    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        std::printf("Error: no se pudo abrir %s\n", path.c_str());
        return;
    }

    json jdata;
    ifs >> jdata;
    ifs.close();

    // 1. Insertar usuarios en la hash table
    for (const auto &u : jdata["usuarios"])
    {
        usuarios.insertar(u["dni"], u["perfil"]);
    }

    // 2. Insertar todos los usuarios en el heap con ts aleatorio
    for (const auto &u : jdata["usuarios"])
    {
        long dni = u["dni"];
        std::string perfil = u["perfil"];
        long ts = 1720406400 + (std::rand() % (60 * 60 * 24)); // Un día aleatorio
        NodoHash *nodo = usuarios.buscar(dni);
        if (nodo && !nodo->enCola && !nodo->atendido)
        {
            heap.insertar(dni, perfil, ts);
            usuarios.marcarEnCola(dni, true);
        }
    }

    // 3. Vaciar el heap (simula operaciones de extracción)
    while (!heap.estaVacio())
    {
        Elemento e = heap.extraerMax();
        usuarios.marcarEnCola(e.dni, false);
        usuarios.marcarAtendido(e.dni, true);
    }

    // 4. Insertar en AVL un acceso por cada usuario, zona según perfil, ts lineal por perfil (7 min) + minutos aleatorios
    long base_ts = 1720406400; // inicio del día
    int i_vip = 0, i_med = 0, i_seg = 0, i_disc = 0, i_pub = 0;
    const long intervalo = 7 * 60; // 7 minutos

    for (const auto &u : jdata["usuarios"])
    {
        std::string perfil = u["perfil"];
        long ts = 0;
        int min_offset = std::rand() % 7; // offset aleatorio de 0 a 6 minutos
        if (perfil == "vip")
        {
            ts = base_ts + i_vip * intervalo + min_offset * 60;
            arbol.insertar("puerta-vip", ts);
            ++i_vip;
        }
        else if (perfil == "personal-medico")
        {
            ts = base_ts + i_med * intervalo + min_offset * 60;
            arbol.insertar("puerta-medico", ts);
            ++i_med;
        }
        else if (perfil == "seguridad")
        {
            ts = base_ts + i_seg * intervalo + min_offset * 60;
            arbol.insertar("puerta-staff", ts);
            ++i_seg;
        }
        else if (perfil == "discapacitados")
        {
            ts = base_ts + i_disc * intervalo + min_offset * 60;
            arbol.insertar("puerta-ada", ts);
            ++i_disc;
        }
        else if (perfil == "publico-general")
        {
            ts = base_ts + i_pub * intervalo + min_offset * 60;
            arbol.insertar("puerta-general", ts);
            ++i_pub;
        }
    }
}

int main()
{
    Server svr;
    // cargarDatosIniciales("data.json");
    cargarDatosInicialesPruebaTecnica("data.json");

    // Middleware CORS
    svr.set_pre_routing_handler([](const Request &req, Response &res)
                                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        if (req.method == "OPTIONS") {
            res.status = 200;
            return Server::HandlerResponse::Handled;
        }
        return Server::HandlerResponse::Unhandled; });

    // ---------------- HASH TABLE ----------------

    // GET /usuarios → usuarios no atendidos ni en cola
    svr.Get("/usuarios", [](const Request &, Response &res)
            {
        json arr = json::array();
        for (int i = 0; i < usuarios.getTam(); ++i) {
            NodoHash* n = usuarios.getBucket(i);
            while (n) {
                if (!n->enCola && !n->atendido) {
                    arr.push_back({ {"dni", n->dni}, {"perfil", n->perfil} });
                }
                n = n->siguiente;
            }
        }
        res.set_content(arr.dump(), "application/json"); });

    // POST /usuario → registrar nuevo usuario
    svr.Post("/usuario", [](const Request &req, Response &res)
             {
        if (req.body.empty()) {
            res.status = 400;
            res.set_content("Body vacío", "text/plain");
            return;
        }

        auto j = json::parse(req.body);
        long dni = j["dni"];
        std::string perfil = j["perfil"];

        if (usuarios.validar(dni)) {
            res.status = 409;
            res.set_content("Usuario ya existe", "text/plain");
            return;
        }

        usuarios.insertar(dni, perfil);
        res.status = 201;
        res.set_content("Usuario creado", "text/plain"); });

    // PUT /usuario/{dni} → actualizar perfil (y prioridad si aplica)
    svr.Put(R"(/usuario/(\d+))", [](const Request &req, Response &res)
            {
        if (req.body.empty()) {
            res.status = 400;
            res.set_content("Body vacío", "text/plain");
            return;
        }

        long dni = std::stol(req.matches[1]);
        auto j = json::parse(req.body);
        std::string nuevoPerfil = j["perfil"];

        NodoHash* nodo = usuarios.buscar(dni);
        if (!nodo) {
            res.status = 404;
            res.set_content("Usuario no encontrado", "text/plain");
            return;
        }

        nodo->perfil = nuevoPerfil;

        // Si está en el heap, actualizar prioridad
        int idx = heap.buscarIndice(dni);
        if (idx >= 0) {
            heap.actualizarPrioridad(idx, heap.perfilAPrioridadPublic(nuevoPerfil));
        }

        res.set_content("Perfil actualizado", "text/plain"); });

    // GET /usuario/{dni} → validación de existencia
    svr.Get(R"(/usuario/(\d+))", [](const Request &req, Response &res)
            {
        long dni = std::stol(req.matches[1]);
        NodoHash* nodo = usuarios.buscar(dni);
        if (nodo) {
            res.set_content(json({{"valid", true}, {"perfil", nodo->perfil}}).dump(), "application/json");
        } else {
            res.set_content(json({{"valid", false}}).dump(), "application/json");
        } });

    // ---------------- MAX HEAP ----------------

    // POST /cola → insertar en cola
    svr.Post("/cola", [](const Request &req, Response &res)
             {
        auto j = json::parse(req.body);
        long dni = j["dni"];
        long ts = j["ts"];
        // Si ts es inválido (<=0 o menor a 1_000_000_000), usar timestamp actual
        if (ts <= 0 || ts < 1000000000) {
            ts = std::time(nullptr);
        }
        NodoHash* nodo = usuarios.buscar(dni);
        if (!nodo) {
            res.status = 404;
            res.set_content("Usuario no registrado", "text/plain");
            return;
        }
        if (nodo->enCola || nodo->atendido) {
            res.status = 409;
            res.set_content("Usuario ya fue procesado", "text/plain");
            return;
        }
        heap.insertar(dni, nodo->perfil, ts);
        usuarios.marcarEnCola(dni, true);
        res.set_content("Insertado en cola", "text/plain"); });

    // GET /cola/top5 → ver los siguientes 5 por prioridad, con perfil
    svr.Get("/cola/top5", [&](const Request &, Response &res)
            {
        int count = 0;
        Elemento* top = heap.verTop5(count);
        json arr = json::array();

        for (int i = 0; i < count; ++i) {
            long dni = top[i].dni;
            NodoHash* nodo = usuarios.buscar(dni);
            std::string perfil = nodo ? nodo->perfil : "-";

            arr.push_back({
                {"dni", dni},
                {"perfil", perfil},
                {"prioridad", top[i].prioridad},
                {"ts", top[i].ts}
            });
        }

        delete[] top;
        res.set_content(arr.dump(), "application/json"); });

    // POST /cola/extract → extraer al siguiente y marcar como atendido
    svr.Post("/cola/extract", [](const Request &, Response &res)
             {
        if (heap.estaVacio()) {
            res.status = 204;
            return;
        }

        Elemento e = heap.extraerMax();
        usuarios.marcarEnCola(e.dni, false);
        usuarios.marcarAtendido(e.dni, true);
        res.set_content(json({{"dni", e.dni}, {"ts", e.ts}, {"prioridad", e.prioridad}}).dump(), "application/json"); });

    // PUT /cola/update → cambiar prioridad
    svr.Put("/cola/update", [](const Request &req, Response &res)
            {
        auto j = json::parse(req.body);
        long dni = j["dni"];
        std::string nuevoPerfil = j["nuevo_perfil"];

        int idx = heap.buscarIndice(dni);
        if (idx < 0) {
            res.status = 404;
            res.set_content("No está en cola", "text/plain");
            return;
        }

        heap.actualizarPrioridad(idx, heap.perfilAPrioridadPublic(nuevoPerfil));
        res.set_content("Prioridad actualizada", "text/plain"); });

    // ---------------- AVL TREE ----------------

    // POST /acceso → registrar acceso a zona (no afecta heap)
    svr.Post("/acceso", [](const Request &req, Response &res)
             {
        auto j = json::parse(req.body);
        arbol.insertar(j["zona"], j["ts"]);
        res.set_content("Acceso registrado", "text/plain"); });

    // GET /accesos/rango?inicio=...&fin=...
    svr.Get("/accesos/rango", [](const Request &req, Response &res)
            {
        if (!req.has_param("inicio") || !req.has_param("fin")) {
            res.status = 400;
            res.set_content("Parámetros inicio y fin requeridos", "text/plain");
            return;
        }

        long ini = std::stol(req.get_param_value("inicio"));
        long fin = std::stol(req.get_param_value("fin"));

        auto vec = arbol.rangoTiempos(ini, fin);
        json arr = json::array();
        for (auto* n : vec) {
            arr.push_back({{"zona", n->zona}, {"ts", n->timestamp}});
        }
        res.set_content(arr.dump(), "application/json"); });

    // GET /accesos/zona_top
    svr.Get("/accesos/zona_top", [](const Request &, Response &res)
            { res.set_content(arbol.zonaMasEntradas(), "text/plain"); });

    std::cout << "Servidor escuchando en http://localhost:18080\n";
    svr.listen("0.0.0.0", 18080);
    return 0;
}

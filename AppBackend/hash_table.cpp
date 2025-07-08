#include "hash_table.h"
#include <iostream>

TablaHash::TablaHash(int tamano_inicial, float carga_maxima)
    : tam(tamano_inicial), usados(0), cargaMaxima(carga_maxima)
{
    tabla = new NodoHash *[tam];
    for (int i = 0; i < tam; ++i)
        tabla[i] = nullptr;
}

TablaHash::~TablaHash()
{
    for (int i = 0; i < tam; ++i)
    {
        NodoHash *actual = tabla[i];
        while (actual)
        {
            NodoHash *aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
    }
    delete[] tabla;
}
// DNI % 17 = 54345985349853
int TablaHash::hashFunc(long clave) const
{
    long h = clave < 0 ? -clave : clave;
    return static_cast<int>(h % tam);
}

void TablaHash::insertar(long dni, const std::string &perfil)
{
    if (static_cast<float>(usados + 1) / tam > cargaMaxima)
    {
        rehash();
    }
    int idx = hashFunc(dni);
    NodoHash *nuevo = new NodoHash(dni, perfil);
    nuevo->enCola = false;
    nuevo->atendido = false;
    nuevo->siguiente = tabla[idx];
    tabla[idx] = nuevo;
    ++usados;
}

// buscar un usuario
NodoHash *TablaHash::buscar(long dni) const
{
    int idx = hashFunc(dni);
    NodoHash *actual = tabla[idx];
    while (actual)
    {
        if (actual->dni == dni)
            return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

bool TablaHash::validar(long dni) const
{
    return buscar(dni) != nullptr;
}

// Rehashing: duplica el tamaño de la tabla y reubica todos los elementos
void TablaHash::rehash()
{
    int nuevoTam = tam * 2 + 1;
    NodoHash **vieja = tabla;

    tabla = new NodoHash *[nuevoTam];
    for (int i = 0; i < nuevoTam; ++i)
        tabla[i] = nullptr;

    int viejoTam = tam;
    tam = nuevoTam;
    usados = 0;

    // Recorre todos los buckets de la tabla vieja y reinserta cada nodo en la nueva tabla
    for (int i = 0; i < viejoTam; ++i)
    {
        NodoHash *actual = vieja[i];
        while (actual)
        {
            insertar(actual->dni, actual->perfil);
            NodoHash *siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
    delete[] vieja;
}

// Busca nodo por perfil (clave de conteo)
NodoHash *TablaHash::buscarPorPerfil(const std::string &perfil) const
{
    for (int i = 0; i < tam; ++i)
    {
        NodoHash *n = tabla[i];
        while (n)
        {
            if (n->perfil == perfil)
                return n;
            n = n->siguiente;
        }
    }
    return nullptr;
}

// Incrementa el contador asociado a un perfil
void TablaHash::incrementar(const std::string &clave)
{
    NodoHash *n = buscarPorPerfil(clave);
    if (!n)
    {
        insertar(0, clave);
        n = buscarPorPerfil(clave);
    }
    n->contador++;
}

// Obtiene el contador asociado a un perfil
int TablaHash::obtenerConteo(const std::string &clave) const
{
    NodoHash *n = buscarPorPerfil(clave);
    return n ? n->contador : 0;
}

// Marca si un usuario esta actualmente en la cola
void TablaHash::marcarEnCola(long dni, bool estado)
{
    NodoHash *nodo = buscar(dni);
    if (nodo)
    {
        nodo->enCola = estado;
    }
    else
    {
        std::cerr << "marcarEnCola: DNI no encontrado: " << dni << "\n";
    }
}

// Marca si un usuario ya fue atendido (extraido)
void TablaHash::marcarAtendido(long dni, bool estado)
{
    NodoHash *nodo = buscar(dni);
    if (nodo)
    {
        nodo->atendido = estado;
    }
    else
    {
        std::cerr << "marcarAtendido: DNI no encontrado: " << dni << "\n";
    }
}

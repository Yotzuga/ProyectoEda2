#include "avl_tree.h"
#include <iostream>

// Constructor
ArbolAVL::ArbolAVL() : raiz(nullptr) {}

// Insercion
void ArbolAVL::insertar(const std::string &zona, long ts)
{
    raiz = insertarRecursivo(raiz, zona, ts);
}

// Insercion recursiva con rebalanceo
NodoAVL *ArbolAVL::insertarRecursivo(NodoAVL *nodo, const std::string &zona, long ts)
{
    if (!nodo)
        return new NodoAVL(zona, ts);
    if (ts < nodo->timestamp)
    {
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, zona, ts);
        nodo->izquierdo->padre = nodo;
    }
    else
    {
        nodo->derecho = insertarRecursivo(nodo->derecho, zona, ts);
        nodo->derecho->padre = nodo;
    }
    actualizarFactor(nodo);
    return reBalancear(nodo);
}

// actualizar altura
void ArbolAVL::actualizarFactor(NodoAVL *nodo)
{
    int h_izq = nodo->izquierdo ? nodo->izquierdo->altura : 0;
    int h_der = nodo->derecho ? nodo->derecho->altura : 0;
    nodo->altura = 1 + (h_izq > h_der ? h_izq : h_der);
    nodo->factor_balance = h_izq - h_der;
}

// Rebalanceo segun factor
NodoAVL *ArbolAVL::reBalancear(NodoAVL *nodo)
{
    actualizarFactor(nodo);
    if (nodo->factor_balance > 1)
    {
        if (nodo->izquierdo->factor_balance < 0)
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }
    if (nodo->factor_balance < -1)
    {
        if (nodo->derecho->factor_balance > 0)
            nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

NodoAVL *ArbolAVL::rotarIzquierda(NodoAVL *nodo)
{
    NodoAVL *r = nodo->derecho;
    nodo->derecho = r->izquierdo;
    if (r->izquierdo)
        r->izquierdo->padre = nodo;
    r->izquierdo = nodo;
    r->padre = nodo->padre;
    nodo->padre = r;
    actualizarFactor(nodo);
    actualizarFactor(r);
    return r;
}

NodoAVL *ArbolAVL::rotarDerecha(NodoAVL *nodo)
{
    NodoAVL *l = nodo->izquierdo;
    nodo->izquierdo = l->derecho;
    if (l->derecho)
        l->derecho->padre = nodo;
    l->derecho = nodo;
    l->padre = nodo->padre;
    nodo->padre = l;
    actualizarFactor(nodo);
    actualizarFactor(l);
    return l;
}

// Mostrar arbol (in-order visual)
void ArbolAVL::mostrar()
{
    if (!raiz)
    {
        std::cout << "Arbol vacio\n";
        return;
    }
    mostrarRecursivo(raiz, 0);
}

void ArbolAVL::mostrarRecursivo(NodoAVL *nodo, int nivel)
{
    if (!nodo)
        return;
    mostrarRecursivo(nodo->derecho, nivel + 1);
    for (int i = 0; i < nivel; ++i)
        std::cout << "    ";
    std::cout << nodo->zona << " @" << nodo->timestamp
              << " (" << nodo->factor_balance << ")\n";
    mostrarRecursivo(nodo->izquierdo, nivel + 1);
}

// Obtener rango de tiempos
std::vector<NodoAVL *> ArbolAVL::rangoTiempos(long inicio, long fin)
{
    std::vector<NodoAVL *> out;
    rangoRec(raiz, inicio, fin, out);
    return out;
}

// Recursion para obtener nodos en rango
void ArbolAVL::rangoRec(NodoAVL *nodo, long inicio, long fin, std::vector<NodoAVL *> &out)
{
    if (!nodo)
        return;
    if (nodo->timestamp > inicio)
        rangoRec(nodo->izquierdo, inicio, fin, out);
    if (nodo->timestamp >= inicio && nodo->timestamp <= fin)
        out.push_back(nodo);
    if (nodo->timestamp < fin)
        rangoRec(nodo->derecho, inicio, fin, out);
}

// Zona con mas entradas (usa TablaHash para conteo)
std::string ArbolAVL::zonaMasEntradas()
{
    TablaHash cnt(101, 0.7f);
    contarZonas(raiz, cnt);
    std::string best;
    int maxc = 0;
    for (int i = 0; i < cnt.getTam(); ++i)
    {
        NodoHash *n = cnt.getBucket(i);
        while (n)
        {
            if (n->contador > maxc)
            {
                maxc = n->contador;
                best = n->perfil;
            }
            n = n->siguiente;
        }
    }
    return best;
}

void ArbolAVL::contarZonas(NodoAVL *nodo, TablaHash &cnt)
{
    if (!nodo)
        return;
    cnt.incrementar(nodo->zona);
    contarZonas(nodo->izquierdo, cnt);
    contarZonas(nodo->derecho, cnt);
}

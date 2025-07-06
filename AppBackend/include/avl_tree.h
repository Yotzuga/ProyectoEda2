// avl_tree.h
#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <string>
#include <vector>
#include "hash_table.h"

struct NodoAVL
{
    std::string zona;
    long timestamp;
    int altura; // NUEVO: altura del subárbol
    int factor_balance;
    NodoAVL *izquierdo;
    NodoAVL *derecho;
    NodoAVL *padre;

    NodoAVL(const std::string &z, long ts)
        : zona(z), timestamp(ts), altura(1), factor_balance(0),
          izquierdo(nullptr), derecho(nullptr), padre(nullptr) {}
};

class ArbolAVL
{
private:
    NodoAVL *raiz;

    NodoAVL *insertarRecursivo(NodoAVL *nodo, const std::string &zona, long ts);
    // int altura(NodoAVL* nodo); // ELIMINAR: ya no se usa
    void actualizarFactor(NodoAVL *nodo);
    NodoAVL *reBalancear(NodoAVL *nodo);
    NodoAVL *rotarIzquierda(NodoAVL *nodo);
    NodoAVL *rotarDerecha(NodoAVL *nodo);
    void mostrarRecursivo(NodoAVL *nodo, int nivel);

    void rangoRec(NodoAVL *nodo, long inicio, long fin,
                  std::vector<NodoAVL *> &out);
    void contarZonas(NodoAVL *nodo, TablaHash &cnt);

public:
    ArbolAVL();

    void insertar(const std::string &zona, long timestamp);
    std::vector<NodoAVL *> rangoTiempos(long inicio, long fin);
    std::string zonaMasEntradas();
    void mostrar();
};

#endif // AVL_TREE_H

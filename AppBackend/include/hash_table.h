#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>

// Nodo para encadenamiento de colisiones, conteo de zonas y estado de servicio
struct NodoHash
{
    long dni;
    std::string perfil;
    int contador;  // contador gen�rico: para conteo de zonas
    bool enCola;   // true si est� actualmente en la cola de prioridad
    bool atendido; // true si ya fue extra�do de la cola alguna vez
    NodoHash *siguiente;

    NodoHash(long _dni, const std::string &_perfil)
        : dni(_dni),
          perfil(_perfil),
          contador(0),
          enCola(false),
          atendido(false),
          siguiente(nullptr)
    {
    }
};

class TablaHash
{
private:
    NodoHash **tabla;  // arreglo de punteros a NodoHash
    int tam;           // tama�o actual de la tabla
    int usados;        // n�mero de elementos almacenados
    float cargaMaxima; // umbral para rehashing

    void rehash();
    int hashFunc(long clave) const;

    // Busca un nodo por su perfil (clave), sin usar dni
    NodoHash *buscarPorPerfil(const std::string &perfil) const;

public:
    TablaHash(int tamano_inicial = 17, float carga_maxima = 0.7f);
    ~TablaHash();

    // Operaciones cl�sicas
    void insertar(long dni, const std::string &perfil);
    NodoHash *buscar(long dni) const;
    bool validar(long dni) const;

    // Estado de usuario en cola / atendido
    void marcarEnCola(long dni, bool estado);
    void marcarAtendido(long dni, bool estado);

    // M�todos de conteo
    void incrementar(const std::string &clave);
    int obtenerConteo(const std::string &clave) const;

    // Acceso a buckets para iteracion externa
    int getTam() const { return tam; }
    NodoHash *getBucket(int idx) const { return tabla[idx]; }
};

#endif // HASH_TABLE_H

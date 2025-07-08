
#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <string>

//  Elemento : Usuario Registrados
struct Elemento
{
    long dni;
    int prioridad;
    long ts; // Timestamp de cuándo fue encolado
};

class MaxHeap
{
private:
    Elemento *heap; // Array dinámico que soporta la estructura de heap
    int capacidad;  // Capacidad actual del array
    int tamanio;    // Número de elementos en el heap

    // logica de PROPIEDADES
    int padre(int i) const { return (i - 1) / 2; }
    int izq(int i) const { return 2 * i + 1; }
    int der(int i) const { return 2 * i + 2; }

    // Reorganiza hacia abajo para mantener la propiedad de max-heap al sacar la cabeza
    void heapifyDown(int i);
    // Reorganiza hacia arriba tras insertar un nuevo elemento
    void heapifyUp(int i);
    // Duplica la capacidad del array cuando está lleno
    void expandir();
    // Convierte un string de perfil a un valor numérico de prioridad
    int perfilAPrioridad(const std::string &perfil) const;

public:
    // Constructor: inicializa con capacidad por defecto (100)
    MaxHeap(int cap_inicial = 100);
    // Destructor: libera la memoria dinámica
    ~MaxHeap();

    // Inserta un nuevo usuario en la cola, usando perfil→prioridad
    void insertar(long dni, const std::string &perfil, long ts);
    // Extrae y devuelve el usuario con mayor prioridad (raíz del heap)
    Elemento extraerMax();
    // Actualiza la prioridad de un elemento existente y reordena
    void actualizarPrioridad(int idx, int nuevaPrio);

    // Devuelve un array con los 5 elementos de mayor prioridad
    // Sin afectar el heap original
    Elemento *verTop5(int &outCount) const;

    // Comprueba si el heap está vacío
    bool estaVacio() const { return tamanio == 0; }

    // Busca la posición de un usuario por DNI (O(n))
    int buscarIndice(long dni) const;

    // Permite usar la conversión perfil→prioridad desde fuera
    int perfilAPrioridadPublic(const std::string &perfil) const
    {
        return perfilAPrioridad(perfil);
    }
};

#endif

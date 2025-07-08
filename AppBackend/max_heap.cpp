#include "max_heap.h"
#include <string>

// Constructor / Destructor
MaxHeap::MaxHeap(int cap_inicial)
    : capacidad(cap_inicial), tamanio(0)
{
    heap = new Elemento[capacidad];
}

// Destructor: libera la memoria dinámica
// Se asegura de liberar el array dinámico que contiene los elementos del heap
MaxHeap::~MaxHeap()
{
    delete[] heap;
}

// Convierte perfil a valor de prioridad
int MaxHeap::perfilAPrioridad(const std::string &perfil) const
{
    if (perfil == "vip")
        return 5;
    if (perfil == "personal-medico")
        return 4;
    if (perfil == "seguridad")
        return 3;
    if (perfil == "discapacitados")
        return 2;
    if (perfil == "publico-general")
        return 1;
    return 0; // desconocido o perfil inválido
}

// Duplicar la capacidad del array cuando está lleno
// Crea un nuevo array con el doble de capacidad, copia los elementos existentes y libera el antiguo
// Esta función se llama automáticamente cuando el heap está lleno
void MaxHeap::expandir()
{
    int nuevaCap = capacidad * 2;
    Elemento *nuevo = new Elemento[nuevaCap];
    for (int i = 0; i < tamanio; ++i)
    {
        nuevo[i] = heap[i];
    }
    delete[] heap; // Aquí se libera la memoria del array antiguo para evitar fugas de memoria
    heap = nuevo;
    capacidad = nuevaCap;
}

void MaxHeap::insertar(long dni, const std::string &perfil, long ts)
{
    if (tamanio == capacidad)
    {
        expandir();
    }
    Elemento e;
    e.dni = dni;
    e.prioridad = perfilAPrioridad(perfil);
    e.ts = ts;
    heap[tamanio] = e;
    heapifyUp(tamanio);
    ++tamanio;
}

// Compara SOLO por prioridad
bool mayorPrioridad(const Elemento &a, const Elemento &b)
{
    return a.prioridad > b.prioridad;
}

void MaxHeap::heapifyUp(int i)
{
    while (i > 0)
    {
        int p = padre(i);
        if (mayorPrioridad(heap[i], heap[p]))
        {
            Elemento aux = heap[i];
            heap[i] = heap[p];
            heap[p] = aux;
            i = p;
        }
        else
        {
            break;
        }
    }
}

void MaxHeap::heapifyDown(int i)
{
    while (true)
    {
        int mayor = i;
        int l = izq(i);
        int r = der(i);
        if (l < tamanio && mayorPrioridad(heap[l], heap[mayor]))
        {
            mayor = l;
        }
        if (r < tamanio && mayorPrioridad(heap[r], heap[mayor]))
        {
            mayor = r;
        }
        if (mayor != i)
        {
            Elemento aux = heap[i];
            heap[i] = heap[mayor];
            heap[mayor] = aux;
            i = mayor;
        }
        else
        {
            break;
        }
    }
}

Elemento MaxHeap::extraerMax()
{
    if (tamanio == 0)
    {
        return {0, 0, 0}; // dni = 0 prioridad = 0 ts = 0
    }
    Elemento root = heap[0];     // Extrae la raíz: el nodo con mayor prioridad
    heap[0] = heap[tamanio - 1]; // Reemplaza la raíz con el último elemento
    --tamanio;
    heapifyDown(0);
    return root; // Retorna el elemento extraído
}

void MaxHeap::actualizarPrioridad(int idx, int nuevaPrio)
{
    if (idx < 0 || idx >= tamanio)
        return;
    int antigua = heap[idx].prioridad;
    heap[idx].prioridad = nuevaPrio;
    if (nuevaPrio > antigua)
    {
        heapifyUp(idx);
    }
    else
    {
        heapifyDown(idx);
    }
}

Elemento *MaxHeap::verTop5(int &outCount) const
{
    int n = (tamanio < 5 ? tamanio : 5);
    Elemento *copia = new Elemento[tamanio];
    for (int i = 0; i < tamanio; ++i)
    {
        copia[i] = heap[i];
    }

    // Selection sort SOLO por prioridad para los primeros n elementos
    for (int i = 0; i < n; ++i)
    {
        int maxIdx = i;
        for (int j = i + 1; j < tamanio; ++j)
        {
            if (copia[j].prioridad > copia[maxIdx].prioridad)
            {
                maxIdx = j;
            }
        }
        if (maxIdx != i)
        {
            Elemento temp = copia[i];
            copia[i] = copia[maxIdx];
            copia[maxIdx] = temp;
        }
    }

    Elemento *top5 = new Elemento[n];
    for (int i = 0; i < n; ++i)
    {
        top5[i] = copia[i];
    }
    delete[] copia;
    outCount = n;
    return top5;
}

int MaxHeap::buscarIndice(long dni) const
{
    for (int i = 0; i < tamanio; ++i)
    {
        if (heap[i].dni == dni)
        {
            return i;
        }
    }
    return -1;
}


# Pure Functional Template Language

Esta librería intenta acercarse a lenguajes funcionales puros mediante el uso de templates.

## Empezando

La librería consta de ficheros header (.h), por tanto lo único necesario es importarlas a tu proyecto.

### Requisitos

Se requiere como mínimo C++14.


## Funciones

En los lenguajes funcionales puros el elemento más importante es la función, por lo que vamos a empezar por aquí. Quien haya indagado en C++ tarde o temprano se encuentra con las meta-funciones:

```cpp
template<class X, class Y>
struct myFunction{
    using type = //...
}
```

Sin embargo son un poco engorrosas de manejar, sobre todo si queremos recibir otras funciones como parámetros e incluso retornarlas. Por esto propongo lo que he denominado (supongo que ya tendrán un nombre, pero no he encontrado nada por internet) 'meta-funciones perezosas':

```cpp
struct myFunction{
    template<class X, class Y>
    using type = //...
}
```

Ahora los parámetros se pasan tarde. ¿Qué consecuencias tiene esto? Nuestra función va a ser la clase myFunction, es decir, las funciones serán clases. Además podemos pasar como parámetros otras clases, es decir, otras funciones. Y como type es un alias de otra clase significa que estamos retornando otra función (o un tipo de dato como int).

### Ejemplo

```cpp

struct Sucesor{             // <-- Función que recibe un entero devuelve su sucesor.
    template<int k>     
    static const int value = k+1;
}

struct Cuadrado{            // <-- Función que recibe un entero y devuelve su cuadrado.
    template<int k>
    static const int value = k*k;   
}


struct Comp{                // <-- Función que recibe dos funciones y un entero resultado de F(G(k)).
    template<class F, class G, int k>
    static const int value = F::template value<G::template value<k>>;
}

int main(){

    std::cout << Comp::value<Cuandrado,Sucesor,5> << std::endl;

}

```
> Output: 36

Primero se ejecuta Sucesor::value<5> = 6, y despues se ejecuta Cuadrado::value<6> = 36.

## Funciones curryficadas

Se dice que una función está curryficada cuando puede tomar sus parámetros de uno en uno. Esto significa que al tomar un parámetro, nuestra función devuelve otra función que recibe un parámetro menos. 

### Ejemplo

```cpp

struct Max{         // <-- Funcion curryficada que devuelve el máximo de dos enteros.
    template<int m>
    struct let{
        template<int n>
        using let = m > n ? m : n;
    }
}

using Max5 = Max::let<5>;   // <-- Pasando un parámetro a Max, recibimos una nueva función.

int main(){

    static const n = 7;
    static const k = 3;

    std::cout << "El máximo entre 5 y " << n << " es: " << Max5::let<n> << td::endl;
    std::cout << "El máximo entre 5 y " << k<< " es: " << Max5::let<k> <<std::endl;

}

```
> Output: El máximo entre 5 y 7 es: 7

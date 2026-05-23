# Rosenbrock Optimizer

Сравнение трёх методов нулевого порядка для безусловной минимизации 
функции Розенброка.

## Методы
- **Direct Search** — координатный спуск с уменьшением шага
- **Hooke-Jeeves** — исследовательский поиск + шаг по образцу  
- **Nelder-Mead** — метод деформируемого симплекса

## Функция
f(x,y) = (1-x)² + 100(y-x²)²  
Минимум: (1, 1), f = 0

## Сборка
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .

## Стек
- C++20
- SFML 3.0

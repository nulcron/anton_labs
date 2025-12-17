#include <iostream>
#include <limits>
#include <cxxabi.h> // для GCC/Clang
#include <typeinfo>

// Функция для вывода полного имени типа. Найдено на просторах stackoverflow.
// Чёрный ящик! НЕ СМОТРЕТЬ
/**
 * Функция для преобразования "mangled" (искаженного) имени типа в читаемое
 * Mangled имена используются компилятором для внутреннего представления типов
 * Например: "i" -> "int", "PFivE" -> "int (*)()"
 * 
 * @param mangled - строка с искаженным именем типа (возвращается typeid().name())
 * @return std::string - читаемое имя типа или исходное mangled имя при ошибке
 */
std::string demangle(const char* mangled) {
    // Переменная status будет содержать код результата операции деманглинга
    // 0 - успех, другие значения - различные коды ошибок
    int status = 0;
    
    /**
     * abi::__cxa_demangle - функция из библиотеки C++ ABI (Application Binary Interface)
     * Выполняет преобразование mangled имени в читаемую строку
     * 
     * Параметры:
     * 1. mangled - исходное искаженное имя
     * 2. output_buffer - указатель на буфер для результата (nullptr означает, что функция
     *    сама выделит память через malloc())
     * 3. length - размер буфера (игнорируется если output_buffer = nullptr)
     * 4. status - указатель на переменную для кода результата
     * 
     * Возвращает: указатель на деманглированную строку (память выделена через malloc)
     *             или nullptr при ошибке
     */
    char* demangled = abi::__cxa_demangle(mangled,   // 1. Исходное искаженное имя
                                          nullptr,    // 2. nullptr - выделить память автоматически
                                          nullptr,    // 3. nullptr - не передавать размер буфера
                                          &status);   // 4. Указатель на переменную статуса
    
    /**
     * Проверяем результат деманглинга:
     * 1. status == 0 - операция успешна
     * 2. demangled != nullptr - указатель валиден
     * 
     * Возможные значения status:
     * 0 - успех
     * -1 - ошибка выделения памяти
     * -2 - невалидное mangled имя
     * -3 - невалидные аргументы
     */
    if (status == 0 && demangled) {
        // Создаем std::string из деманглированной строки
        std::string result(demangled);
        
        /**
         * ОЧЕНЬ ВАЖНО: освобождаем память, выделенную __cxa_demangle()
         * Поскольку функция выделила память через malloc(), освобождаем через free()
         * (не через delete или delete[], так как это не C++ оператор new)
         */
        free(demangled);
        
        // Возвращаем результат как std::string
        return result;
    }
    
    /**
     * Если деманглинг не удался, возвращаем исходное mangled имя
     * в обертке std::string для консистентности интерфейса
     */
    return mangled;
}

template<class T, int steps>
// c++ 20 only
// requires std::integral<T>
void closureMin()
{
    if (steps < 0) {
        
        std::cout << "Введите положительное целочисленное!" << std::endl;
        return;
    }
    
    std::cout << "Min: " << (T)std::numeric_limits<T>::min() << std::endl;
    std::cout << "Количество шагов: " << (int)steps << std::endl;
    
    std::cout << "Замыкание в " << demangle(typeid(T).name()) << std::endl;
    // Если посмотрели, то используй это:
    // std::cout << "Замыкание в " << typeid(T).name() << std::endl;
    std::cout << std::endl << "На минимальном значении:" << std::endl;
    
    T minValue = std::numeric_limits<T>::min();
    
    for(int iterator = steps; iterator >= -steps; iterator--)
    {
        std::cout << minValue << " + " << ((iterator < 0) ? "" : " ") << iterator << " = " << minValue + iterator << std::endl;
    }
}

template<class T, int steps>
// c++ 20 only
// requires std::integral<T>
void closureMax()
{
    if (steps < 0) {
        
        std::cout << "Введите положительное целочисленное!" << std::endl;
        return;
    }
    
    std::cout << "Max: " << (T)std::numeric_limits<T>::max() << std::endl;
    std::cout << "Количество шагов: "  << (int)steps << std::endl;
    
    std::cout << "Замыкание в " << demangle(typeid(T).name()) << std::endl;
    // Если посмотрели, то используй это:
    // std::cout << "Замыкание в " << typeid(T).name() << std::endl;
    std::cout << std::endl << "На максимальном значении:";
    
    T maxValue = std::numeric_limits<T>::max();
    
    for(int iterator = -steps; iterator <= steps; iterator++)
    {
        std::cout << maxValue << " + " << ((iterator < 0) ? "" : " ") << iterator << " = " << maxValue + iterator << std::endl;
    }
}

int main()
{    
    
    closureMin<unsigned short, 3>();

    std::cout << std::endl << std::endl;

    closureMax<short, 3>();
}

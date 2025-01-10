/* 1003. Parity - http://acm.timus.ru/problem.aspx?num=1003
 *
 * Strategy:
 * For each test, maintain a mapping between the digits of the sequence and the shortest question
 * interval that has its rightmost endpoint at the digit. Whenever an interval is added to the
 * mapping whose endpoint conflicts with an earlier added interval, split the interval into two in
 * the mapping and maintain the above invariant by recursing. Whenever there is a parity conflict
 * between two same-sized and positioned intervals, stop.
 *
 * Performance:
 * Linear in the number of intervals (questions) given since the left endpoint and right endpoints
 * each cause at most one split each. The solution runs the test cases in 0.031s using 588KB memory.
 */



#include <iostream>
#include <vector>
#include <unordered_map>
#include <cctype>

// Структура для представления интервала
struct Interval {
    int start;
    int end;
    bool isOdd;
};

// Функция для решения задачи
int findMaxNonOverlappingIntervals(const int length, const std::vector<Interval>& intervals) {
    // Создаем хэш-карту для хранения интервалов
    std::unordered_map<int, Interval> map;
    
    int result = 0;
    
    // Проходимся по каждому интервалу
    for (const auto& current : intervals) {
        // Проверяем границы интервала
        if (current.start < 1 || current.end > length) {
            return result;
        }
        
        while (true) {
            // Находим конец текущего интервала в карте
            auto it = map.find(current.end);
            
            // Если элемент не найден, добавляем его в карту
            if (it == map.end()) {
                map[current.end] = current;
                break;
            }
            
            // Получаем существующий интервал
            const Interval& existing = it->second;
            
            // Разделяем текущий интервал и продолжаем обработку
            if (existing.start < current.start) {
                map[current.end].start = current.start;
                map[current.end].isOdd = current.isOdd;
                
                current.start = existing.start;
                current.end = current.start - 1;
                current.isOdd ^= existing.isOdd;
            } else if (existing.start > current.start) {
                current.start = existing.start;
                current.end = existing.start - 1;
                current.isOdd ^= existing.isOdd;
            } else if (existing.isOdd != current.isOdd) {
                return result;
            } else {
                break;
            }
        }
        
        ++result;
    }
    
    return result;
}

// Функция для чтения целого числа
int readInt() {
    int value = 0;
    char c;
    
    // Пропускаем все символы, кроме цифр
    do {
        c = std::getchar();
    } while (!isdigit(c));
    
    value = c - '0';
    
    // Читаем остальные цифры
    while (isdigit(c = std::getchar())) {
        value = value * 10 + (c - '0');
    }
    
    return value;
}

int main() {
    while (true) {
        int N = 0;
        std::cin >> N;
        
        // Завершаем программу, если введен -1
        if (N == -1) {
            return 0;
        }
        
        int M = readInt();
        
        // Создаем вектор для хранения интервалов
        std::vector<Interval> intervals(M);
        
        for (int i = 0; i < M; ++i) {
            char parityStr[10];
            int start = readInt();
            int end = readInt();
            std::cin >> parityStr;
            
            intervals[i] = { start, end, parityStr[0] == 'o' };
        }
        
        // Вычисляем результат и выводим его
        std::cout << findMaxNonOverlappingIntervals(N, intervals) << "\n";
    }
    
    return 0;
}

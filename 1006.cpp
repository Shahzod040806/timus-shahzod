/* 1006. Square Frames - http://acm.timus.ru/problem.aspx?num=1006
 *
 * Strategy:
 * Brute-force - for each coordinate and width, try to match a frame to that part of the picture.
 * If a frame matches, we add it to the answer, remove it from the picture and allow any part of
 * a future frame to match the empty space, but only add frames that match at least one non-empty
 * space.
 *
 * Performance:
 * O(NH^2W), runs the tests in 0.001s using 448KB memory.
 */

#include <iostream>
#include <string>
#include <vector>

// Символы ASCII для рамки
const int UPPER_LEFT_CORNER = 218;
const int UPPER_RIGHT_CORNER = 191;
const int LOWER_LEFT_CORNER = 192;
const int LOWER_RIGHT_CORNER = 217;
const int HORIZONTAL_LINE = 196;
const int VERTICAL_LINE = 179;
const int EMPTY_SPACE = 46;

// Структура для хранения информации о рамке
struct FrameInfo {
    int x;
    int y;
    int width;
};

std::vector<FrameInfo> framesFound;

// Структура для представления ячейки
struct Cell {
    int value;
    int type;
    bool isAdded;
} grid[50][20];

// Функция для проверки совпадения части рамки с ячейкой
bool checkCellMatch(int x, int y, int type, std::vector<int>& matchedCells) {
    auto& currentCell = grid[x][y];
    
    // Если ячейка уже добавлена к другой рамке, то она не подходит
    if (currentCell.isAdded) {
        return true;
    }
    
    // Если текущая ячейка пустая, то она также не подходит
    if (currentCell.type == EMPTY_SPACE) {
        return false;
    }
    
    // Проверяем, совпадает ли тип ячейки с ожидаемым типом рамки
    if (currentCell.type == type && !currentCell.isAdded) {
        matchedCells.push_back(x + y * 50); // Добавляем координаты совпавшей ячейки
        return true;
    }
    
    return false;
}

// Функция для поиска рамки заданного размера
void findFrame(int& remainingCells, int startX, int startY, int frameWidth) {
    int endX = startX + frameWidth - 1;
    int endY = startY + frameWidth - 1;
    
    std::vector<int> matchedCells; // Массив для хранения индексов совпавших ячеек

    // Проверяем верхний левый угол
    if (!checkCellMatch(startX, startY, UPPER_LEFT_CORNER, matchedCells)) {
        return;
    }
    
    // Проверяем верхнюю сторону
    for (int x = startX + 1; x < endX; ++x) {
        if (!checkCellMatch(x, startY, HORIZONTAL_LINE, matchedCells)) {
            return;
        }
    }
    
    // Проверяем верхний правый угол
    if (!checkCellMatch(endX, startY, UPPER_RIGHT_CORNER, matchedCells)) {
        return;
    }
    
    // Проверяем правую сторону
    for (int y = startY + 1; y < endY; ++y) {
        if (!checkCellMatch(endX, y, VERTICAL_LINE, matchedCells)) {
            return;
        }
    }
    
    // Проверяем нижний правый угол
    if (!checkCellMatch(endX, endY, LOWER_RIGHT_CORNER, matchedCells)) {
        return;
    }
    
    // Проверяем нижнюю сторону
    for (int x = endX - 1; x > startX; --x) {
        if (!checkCellMatch(x, endY, HORIZONTAL_LINE, matchedCells)) {
            return;
        }
    }
    
    // Проверяем нижний левый угол
    if (!checkCellMatch(startX, endY, LOWER_LEFT_CORNER, matchedCells)) {
        return;
    }
    
    // Проверяем левую сторону
    for (int y = endY - 1; y > startY; --y) {
        if (!checkCellMatch(startX, y, VERTICAL_LINE, matchedCells)) {
            return;
        }
    }
    
    // Если все клетки совпадают, добавляем найденную рамку
    if (!matchedCells.empty()) {
        remainingCells -= matchedCells.size();
        framesFound.push_back({startX, startY, frameWidth});
        
        // Отмечаем все совпавшие клетки как использованные
        for (auto index : matchedCells) {
            grid[index % 50][index / 50].isAdded = true;
        }
    }
}

int main() {
    int remainingCellsCount = 0; // Количество непустых клеток

    // Чтение входных данных
    for (int y = 0; y < 20; ++y) {
        std::wstring inputLine;
        std::wcin >> inputLine;
        
        for (int x = 0; x < inputLine.size(); ++x) {
            grid[x][y].value = inputLine[x]; // Значение символа
            grid[x][y].type = (int)inputLine[x]; // Тип символа
            grid[x][y].isAdded = false; // Изначально ни одна клетка не добавлена
            
            if (grid[x][y].type != EMPTY_SPACE) {
                ++remainingCellsCount; // Увеличиваем счетчик непустых клеток
            }
        }
    }

    // Поиск всех возможных рамок
    while (remainingCellsCount > 0) {
        for (int y = 0; y < 20; ++y) {
            for (int x = 0; x < 50; ++x) {
                for (int frameSize = 2; x + frameSize <= 50 && y + frameSize <= 20; ++frameSize) {
                    findFrame(remainingCellsCount, x, y, frameSize);
                }
            }
        }
    }

    // Вывод результатов
    std::cout << framesFound.size() << std::endl;
    for (auto it = framesFound.rbegin(); it != framesFound.rend(); ++it) {
        const auto& frame = *it;
        std::cout << frame.x << " " << frame.y << " " << frame.width << std::endl;
    }

    return 0;
}

#include "utils.h"

std::string generateRandomString() {
    const int length = 14;
    const char digits[] = "0123456789";

    std::string randomString;

    for (int i = 0; i < length; ++i) {
        // Генерация случайного индекса для выбора цифры из массива digits
        int randomIndex = std::rand() % (sizeof(digits) - 1);
        randomString += digits[randomIndex];
    }

    return randomString;
}

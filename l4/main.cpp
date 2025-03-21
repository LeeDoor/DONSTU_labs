#include <clocale>
#include <stdio.h>
#include <wchar.h>

int is_latin(wchar_t c) {
    return L'a' <= c && c <= L'z' || L'A' <= c && c <= L'Z';
}
int is_cyrillic(wchar_t c) {
    return L'а' <= c && c <= L'я' || L'А' <= c && c <= L'Я';
}
bool is_white(wchar_t c) {
    return c == L' ' || c == L'\t';
}
#define SWAP(a, b) \
    wchar_t sw = a; \
    a = b; \
    b = sw

int main() {
    setlocale(LC_ALL, "");
#define BUF_SIZE 200
    wchar_t buffer[BUF_SIZE];
    wprintf(L"Enter line: ");
    fgetws(buffer, BUF_SIZE, stdin);
    wprintf(L"Initial line: %ls", buffer); // NEW-LINE terminated 
    size_t cyr = 0, lat = 0, strs = 0;
    for (size_t i = 0; buffer[i] != L'\n'; ++i, ++strs) {
        cyr += is_cyrillic(buffer[i]);
        lat += is_latin(buffer[i]);
    }
    wprintf(L"cyrillic letters: %zd\n"
            L"latin letters: %zd\n",
            cyr, lat);
    for(int i = strs - 1; i >= 1; --i) {
        if(is_white(buffer[i]) || is_white(buffer[i - 1])) {
            continue;
        }
        wchar_t swap = buffer[i];
        buffer[i] = buffer[i - 1];
        buffer[i - 1] = swap;
    }
    wprintf(L"Edited line: %ls\n", buffer);
    return 0;
}

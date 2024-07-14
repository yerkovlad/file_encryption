#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdint>

const char* all_english_letters_and_numbers = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
const int length = strlen(all_english_letters_and_numbers);

bool isNumber(const char* str) {
    for (size_t i = 0; i < std::strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }
    return true;
}

char* readFile(const char* file_name) {
    std::ifstream ifs(file_name, std::ios::in);
    if (!ifs) {
        std::cerr << "Error: Incorrect file name: " << file_name << std::endl;
        return nullptr;
    }
    
    std::string file_content;
    std::string line;
    while (std::getline(ifs, line)) {
        file_content += line + "\n";
    }
    
    char* result = new char[file_content.size() + 1];
    std::strcpy(result, file_content.c_str());
    
    return result;
}

char* processText(const char* text, int64_t key, bool encrypt) {
    int text_length = strlen(text);
    char* processed_text = new char[text_length + 1];
    int actual_key = key % length;

    for (int i = 0; i < text_length; ++i) {
        bool found = false;
        for (int j = 0; j < length; ++j) {
            if (text[i] == all_english_letters_and_numbers[j]) {
                if (encrypt) {
                    processed_text[i] = all_english_letters_and_numbers[(j + actual_key) % length];
                } else {
                    processed_text[i] = all_english_letters_and_numbers[(j - actual_key + length) % length];
                }
                found = true;
                break;
            }
        }
        if (!found) {
            processed_text[i] = text[i];
        }
    }

    processed_text[text_length] = '\0';
    return processed_text;
}

bool writeToFile(const char* file_name, const char* text) {
    std::ofstream ofs(file_name, std::ios::out | std::ios::trunc);
    if (!ofs) {
        std::cerr << "Error: Could not open the file " << file_name << " for writing." << std::endl;
        return false;
    }

    ofs << text;

    if (!ofs) {
        std::cerr << "Error: Writing to the file " << file_name << " failed." << std::endl;
        return false;
    }

    ofs.close();
    return true;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Error: Incorrect number of arguments. Usage: <program> <file_name.txt> <key> <--encrypt|--decrypt>" << std::endl;
        return 1;
    }

    const char* file_name = argv[1];
    int64_t key = 0;
    const char* type = argv[3];

    if (strstr(file_name, ".txt") == nullptr) {
        std::cerr << "Error: The file name must end with .txt" << std::endl;
        return 1;
    }

    if (!isNumber(argv[2])) {
        std::cerr << "Error: The key is not a number" << std::endl;
        return 1;
    } else {
        key = std::stoll(argv[2]);
    }

    if (strcmp(type, "--encrypt") != 0 && strcmp(type, "--decrypt") != 0) {
        std::cerr << "Error: You have not written the correct type(--encrypt or --decrypt)" << std::endl;
        return 1;
    }

    char* file_content = readFile(file_name);
    if (!file_content) {
        return 1;
    }

    char* result = nullptr;
    if (strcmp(type, "--encrypt") == 0) {
        result = processText(file_content, key, true);
    } else {
        result = processText(file_content, key, false);
    }

    if (result) {
        if (!writeToFile(file_name, result)) {
            std::cerr << "Error: Writing to file failed." << std::endl;
        }
        delete[] result;
    }

    delete[] file_content;

    return 0;
}

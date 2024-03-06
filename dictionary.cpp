#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <locale.h>

std::vector<std::string> split(const std::string& input, const std::string& separators) {
    std::vector<std::string> tokens;
    size_t prev_pos = 0, curr_pos;
    while ((curr_pos = input.find_first_of(separators, prev_pos)) != std::string::npos) {
        if (curr_pos > prev_pos) {
            tokens.push_back(input.substr(prev_pos, curr_pos - prev_pos));
        }
        prev_pos = curr_pos + 1;
    }
    if (prev_pos < input.length()) {
        tokens.push_back(input.substr(prev_pos, std::string::npos));
    }
    return tokens;
}

bool is_one_char_differ(const std::string& word1, const std::string& word2) {

    int diff_len = word1.length() - word2.length();
    if (std::abs(diff_len) > 1) {
        return false;
    }

    int diff_count = 0; 

    for (size_t i = 0, j = 0; i < word1.length() && j < word2.length(); ++i, ++j) {
        if (word1[i] != word2[j]) {
            ++diff_count;

            if (diff_count > 1) {
                return false;
            }

            if (word1.length() > word2.length()) {
                --j;
            }
            else if (word1.length() < word2.length()) {
                --i;
            }
        }
    }

    return true;
}


int main() {
    setlocale(LC_ALL, "Rus");
    std::ifstream input_file("input.txt");
    std::ofstream output_file("output.txt");
    std::ofstream new_dictionary_file("new_dict.txt");
    std::set<std::string> dictionary;

    std::ifstream dictionary_file("dict.txt");
    std::string word;
    while (dictionary_file >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        dictionary.insert(word);
    }

    std::string line;
    while (std::getline(input_file, line)) {
        std::vector<std::string> words = split(line, " .,:;!?-()\"");

        for (const std::string& word : words) {
            std::string lower_word = word;
            std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);

            if (dictionary.find(lower_word) != dictionary.end()) {
                output_file << word << " ";
            }
            else {
                std::cout << "Слово \"" << word << "\" не найдено в словаре. Выберите действие:\n";
                std::cout << "1) Добавить в словарь и записать в output\n";
                std::cout << "2) Не добавлять в словарь, но записать в output\n";
                std::cout << "3) Заменить на одно из слов из словаря\n";

                int choice;
                std::cin >> choice;

                switch (choice) {
                case 1:
                    dictionary.insert(lower_word);
                    output_file << word << " ";
                    break;
                case 2:
                    output_file << word << " ";
                    break;
                case 3:
                {
                    std::vector<std::string> candidates;
                    for (const std::string& dictWord : dictionary) {
                        if (is_one_char_differ(lower_word, dictWord)) {
                            candidates.push_back(dictWord);
                        }
                    }
                    if (!candidates.empty()) {
                        std::cout << "Выберите слово для замены:\n";
                        for (size_t i = 0; i < candidates.size(); ++i) {
                            std::cout << i + 1 << ") " << candidates[i] << "\n";
                        }

                        int replace_choice;
                        bool valid_choice = false;
                        do {
                            std::cin >> replace_choice;
                            if (replace_choice >= 1 && replace_choice <= candidates.size()) {
                                valid_choice = true;
                            }
                            else {
                                std::cerr << "Неверный выбор. Пожалуйста, выберите число от 1 до " << candidates.size() << ": ";
                            }
                        } while (!valid_choice);

                        output_file << candidates[replace_choice - 1] << " ";
                    }
                    else {
                        std::cerr << "Нет подходящих слов для замены. Слово \"" << word << "\" будет пропущено.\n";
                    }
                }
                break;
                default:
                    std::cerr << "Неверный выбор. Слово \"" << word << "\" будет пропущено.\n";
                    break;
                }
            }
        }
        output_file << "\n";
    }

    input_file.close();
    output_file.close();

    for (const std::string& word : dictionary) {
        new_dictionary_file << word << "\n";
    }
    new_dictionary_file.close();

    return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <locale>

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


std::set<std::string> find_alternatives(const std::set<std::string>& dict, const std::string& str) {
    std::set<std::string> res;

    auto is_in_dict = [&dict](const std::string& word) {
        return dict.find(word) != dict.end();
    };

    for (size_t i = 0; i < str.length(); ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (str[i] != c) {
                std::string replaced = str;
                replaced[i] = c;
                if (is_in_dict(replaced))
                    res.insert(replaced);
            }

            std::string inserted = str;
            inserted.insert(i, 1, c);
            if (is_in_dict(inserted))
                res.insert(inserted);

            if (i < str.length() - 1) {
                std::string deleted = str;
                deleted.erase(i, 1);
                if (is_in_dict(deleted))
                    res.insert(deleted);
            }
        }
    }

    for (char c = 'a'; c <= 'z'; ++c) {
        std::string temp = str + c;
        if (is_in_dict(temp))
            res.insert(temp);
    }

    return res;
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
                case 3: {
                    std::set<std::string> replacements = find_alternatives(dictionary, lower_word);
                    if (replacements.empty()) {
                        std::cout << "Нет подходящих замен в словаре.\n";
                    }
                    else {
                        std::cout << "Выберите замену:\n";
                        int ind = 1;
                        for (const auto& repl : replacements) {
                            std::cout << ind << ". " << repl << std::endl;
                            ++ind;
                        }
                        int replace_choice;
                        std::cin >> replace_choice;
                        auto it = replacements.begin();
                        std::advance(it, replace_choice - 1);
                        output_file << *it << " ";
                    }
                    break;
                }
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

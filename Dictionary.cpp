#include <iostream>
#include <string>

#define ASCII_BEGIN 97
#define ASCII_END 122
#define DEFAULT_LETTER ' '

class Node {
private:
    std::string translation;
    Node** node_array;
    char letter;
public:
    explicit Node(char new_letter) {
        node_array = new Node*[ASCII_END - ASCII_BEGIN+1];
        for (int i = 0; i < ASCII_END - ASCII_BEGIN + 1; i++) {
            node_array[i] = nullptr;
        }
        translation = "";
        letter = new_letter;
    }
    bool AddToDictionary(std::string& word, std::string new_translation) {
        if (word.size()) {
            int current_letter = int(word[0])- ASCII_BEGIN;
            if (node_array[current_letter] == nullptr) {
                node_array[current_letter] = new Node(char(current_letter));
                if (node_array[current_letter] == nullptr) {
                    std::cout << "Error allocating memory, please try again later\n";
                    return false;
                }
            }
            if (word.size() == 1) {
                node_array[current_letter]->translation = new_translation;
                return true;
            }
            word.erase(0, 1);
            node_array[current_letter]->AddToDictionary(word, new_translation);
        }
        return false;
    }
    bool CheckCorrespondingWord(std::string word) {
        int i = int(word[0]) - ASCII_BEGIN;
        if (word.size() && node_array[i] != nullptr) {
            word.erase(0, 1);
            return node_array[i]->CheckCorrespondingWord(word);
        }
        else if (word.size() == 0) {
            bool output = false;
            if (translation != "") {
                output = true;
                std::cout << translation << "\n";
            }
            for (int j = 0; j < ASCII_END - ASCII_BEGIN + 1; j++) {
                if (node_array[j] != nullptr) {
                    output = true;
                    node_array[j]->CheckCorrespondingWord(word);
                }
            }
            return output;
        }
        else
            {
            return false;
        }
    }

    std::string GetDefinition(std::string word) {
        int i = int(word[0]) - ASCII_BEGIN;
        if (word.size() && node_array[i] != nullptr) {
            word.erase(0, 1);
            return node_array[i]->GetDefinition(word);
        }
        else if (word.size() == 0) {
            if (translation == "") {
                return "-";
            }
            else {
                return translation;
            }
        }
        return "-";
        
    }

    ~Node() {
        for (int i = 0; i < ASCII_END - ASCII_BEGIN + 1; i++) {
            delete node_array[i];
        }
        delete[] node_array;
    }
};

class Dictionary {
private:
    Node* root;
public:
    Dictionary() {
        root = nullptr;
    }
    bool AddToDictionary(std::string word, std::string translation) {
        if (root == nullptr) {
            root = new Node(DEFAULT_LETTER);
            if (root == nullptr) {
                return false;
            }
        }
        return root->AddToDictionary(word, translation);
    }
    bool CheckCorrespondingWord(std::string word) {
        if (root == nullptr) {
            root = new Node(DEFAULT_LETTER);
            if (root == nullptr) {
                return false;
            }
        }
        return root->CheckCorrespondingWord(word);
    }
    std::string GetDefinition(std::string word) {
        if (root == nullptr) {
            return "-";
        }
        return root->GetDefinition(word);
    }
    ~Dictionary() {
        if (root != nullptr) {
            delete root;
        }
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Dictionary* dict = new Dictionary;
    std::string polecenie;
    std::string argument1;
    std::string argument2;
    while (std::cin >> polecenie) {
        if (polecenie[0] == 'q') {
            break;
        }
        std::cin >> argument1;
        /*if (polecenie[0] == '*') {
            if (!dict->CheckCorrespondingWord(argument1)) {
                std::cout << "-\n";
            }
            continue;
        }*/
        if (polecenie[0] == '?') {
            std::cout << dict->GetDefinition(argument1) << '\n';
            continue;
        }
        std::cin >> argument2;
        if (polecenie[0] == '+') {
            dict->AddToDictionary(argument1, argument2);
        }
    }
    delete dict;
    return 0;
}
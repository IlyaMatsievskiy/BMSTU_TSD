#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <vector>
#include <string>

using namespace std;

struct Node {
    string id;
    int freq;
    Node* left, *right;
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return (l->freq > r->freq);
    }
};

Node* getNode(string id, int freq, Node* left, Node* right) {
    Node* node = new Node();
    node->id = id;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

void encode(Node* root, string str, map<string, string>& huffmanCode) {
    if (root == nullptr) return;
    if (!root->left && !root->right) {
        huffmanCode[root->id] = str;
    }
    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void writeHuffmanCodesToFile(const map<string, string>& huffmanCode, const string& encodedText, const map<string, int>& freq) {
    ofstream outputFile("/Users/ilya/Downloads/Типы и структуры данных 2 курс, 1 семестр/laba_7/laba_7/Huffman_codes.bin", ios::binary);

    if (outputFile.is_open()) {
        for (const auto& pair : huffmanCode) {
            // Записываем уникальный строковый идентификатор ноты
            outputFile.write(pair.first.c_str(), pair.first.size());
            outputFile.put('\0');  // Разделитель между идентификатором и частотой

            int frequency = freq.at(pair.first);
            outputFile.write(reinterpret_cast<const char*>(&frequency), sizeof(frequency));

            char codeLength = static_cast<char>(pair.second.length());
            outputFile.put(codeLength);
        }

        outputFile.put('\0'); // Разделитель между метаданными и закодированным текстом

        for (char ch : encodedText) {
            outputFile.put(ch);
        }

        outputFile.close();
        cout << "Данные успешно записаны в бинарный файл 'huffman_codes.bin'" << endl;
    }
    else {
        cout << "Невозможно открыть файл для записи" << endl;
    }
}

void gain(string text, const map<string, string>& huffmanCode, const map<string, int>& freq) {
    int bit_1 = text.size();
    int bit_2 = 0;

    for (const auto& pair : huffmanCode) {
        bit_2 += pair.second.size() * freq.at(pair.first) + pair.first.size() + pair.second.size();
    }
    int compressedSize = bit_2;  // размер закодированного текста в байтах
    double win = (bit_1 + 0.0) / (compressedSize);
    cout << "Сжатие в " << bit_1 << " байт / " << bit_2 << "байт = "  << win << " раз" << endl;
}



void buildHuffmanTree(string text) {
    map<string, int> freq;
    size_t pos = 0;
    size_t nextPos = text.find('\n', pos);

    while (nextPos != string::npos) {
        string id = text.substr(pos, nextPos - pos);
        freq[id]++;
        pos = nextPos + 1;
        nextPos = text.find('\n', pos);
    }

    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (const auto& pair : freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sumFreq = left->freq + right->freq;
        pq.push(getNode("", sumFreq, left, right));
    }

    Node* root = pq.top();

    map<string, string> huffmanCode;
    encode(root, "", huffmanCode);

    string encodedText;
    pos = 0;
    nextPos = text.find('\n', pos);

    while (nextPos != string::npos) {
        string id = text.substr(pos, nextPos - pos);
        encodedText += huffmanCode[id];
        pos = nextPos + 1;
        nextPos = text.find('\n', pos);
    }

    cout << "Частоты идентификаторов нот: " << endl;
    for (const auto& pair : freq) {
        cout << pair.first << " : " << pair.second << endl;
    }
    cout << endl;

    cout << "Код идентификаторов нот: " << endl;
    for (const auto& pair : huffmanCode) {
        cout << pair.first << " : " << pair.second << endl;
    }
    cout << endl;

    cout << "Закодированный текст по Хаффману: " << encodedText << endl;

    writeHuffmanCodesToFile(huffmanCode, encodedText, freq);
    cout << endl;
    gain(text, huffmanCode, freq);
}

int main() {
    setlocale(0, "");
    ifstream file("/Users/ilya/Downloads/Типы и структуры данных 2 курс, 1 семестр/laba_7/input.txt");  // Замените путь на путь к вашему файлу
    string text = "";

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            text += line + '\n';
        }
        file.close();
    }
    else {
        cout << "Невозможно открыть файл";
        return 0;
    }

    cout << "Прочитанный текст: " << endl << text << endl;
    buildHuffmanTree(text);

    return 0;
}

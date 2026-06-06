#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <windows.h> // библиотека для настройки кодировки консоли windows

using namespace std;

class Constructor {
private:
    string manufacturer;    // производитель
    int ageLimit;           // возрастные ограничения
    double price;           // цена
    int partsCount;         // кол-во деталей

public:
    // 1. Конструктор без параметров
    Constructor() : manufacturer("Неизвестно"), ageLimit(0), price(0.0), partsCount(0) {}

    // 2. Конструктор с параметрами
    Constructor(string m, int a, double p, int c) 
        : manufacturer(m), ageLimit(a), price(p), partsCount(c) {}

    // 3. Конструктор копирования
    Constructor(const Constructor& other) {
        manufacturer = other.manufacturer;
        ageLimit = other.ageLimit;
        price = other.price;
        partsCount = other.partsCount;
    }

    // 4. Деструктор
    ~Constructor() {}

    // Геттеры и сеттеры
    void setManufacturer(string m) { manufacturer = m; }
    string getManufacturer() const { return manufacturer; }

    void setAgeLimit(int a) { ageLimit = a; }
    int getAgeLimit() const { return ageLimit; }

    void setPrice(double p) { price = p; }
    double getPrice() const { return price; }

    void setPartsCount(int c) { partsCount = c; }
    int getPartsCount() const { return partsCount; }

    // Методы из твоей лабы
    void play() const {
        cout << "Играем с конструктором от " << manufacturer << "!" << endl;
    }

    void unpack() const {
        cout << "Распаковка конструктора на " << partsCount << " деталей." << endl;
    }

    void cleanup() const {
        cout << "Ушли убирать детали конструктора." << endl;
    }

    void addParts(int count) {
        partsCount += count;
        cout << "Добавлено " << count << " деталей. Теперь всего: " << partsCount << endl;
    }

    // Перегрузки операторов
    Constructor& operator++() {
        price += 100.0;
        return *this;
    }

    Constructor operator++(int) {
        Constructor temp(*this);
        price += 100.0;
        return temp;
    }

    Constructor& operator--() {
        if (price >= 100.0) price -= 100.0;
        return *this;
    }

    Constructor operator--(int) {
        Constructor temp(*this);
        if (price >= 100.0) price -= 100.0;
        return temp;
    }

    Constructor operator+(const Constructor& other) {
        return Constructor(this->manufacturer + " & " + other.manufacturer,
                           max(this->ageLimit, other.ageLimit),
                           this->price + other.price,
                           this->partsCount + other.partsCount);
    }

    bool operator==(const Constructor& other) const {
        return (manufacturer == other.manufacturer &&
                ageLimit == other.ageLimit &&
                price == other.price &&
                partsCount == other.partsCount);
    }

    // Дружественные операторы ввода-вывода
    friend ostream& operator<<(ostream& os, const Constructor& obj) {
        os << "Производитель: " << obj.manufacturer
           << " | Возраст до: " << obj.ageLimit << " лет"
           << " | Цена: " << obj.price << " руб."
           << " | Деталей: " << obj.partsCount << " шт." << endl;
        return os;
    }

    friend istream& operator>>(istream& is, Constructor& obj) {
        cout << "Введите производителя: ";
        is >> obj.manufacturer;
        cout << "Введите возрастное ограничение: ";
        is >> obj.ageLimit;
        cout << "Введите цену: ";
        is >> obj.price;
        cout << "Введите количество деталей: ";
        is >> obj.partsCount;
        return is;
    }
};

// Структура узла списка
struct Node {
    Constructor data;
    Node* next;
};

// Управляющий класс списка
class ConstructorList {
private:
    Node* head;
    int currentSize;
    string filename;

public:
    ConstructorList(string file) : head(nullptr), currentSize(0), filename(file) {}

    ~ConstructorList() {
        clear();
    }

    int size() const { return currentSize; }
    bool empty() const { return head == nullptr; }

    // ИЗМЕНЕНО ДЛЯ СРЕДНЕГО УРОВНЯ: упорядоченное добавление по ЦЕНЕ
    void push_back(const Constructor& item) {
        Node* newNode = new Node;
        newNode->data = item;
        newNode->next = nullptr;

        // Если список пуст или новая цена меньше цены первого элемента
        if (head == nullptr || item.getPrice() < head->data.getPrice()) {
            newNode->next = head;
            head = newNode;
        } 
        // Ищем место внутри списка
        else {
            Node* current = head;
            while (current->next != nullptr && current->next->data.getPrice() <= item.getPrice()) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        currentSize++;
    }

    // Удаление элемента по индексу
    bool removeAt(int idx) {
        if (idx < 0 || idx >= currentSize || head == nullptr) return false;
        Node* temp = head;
        if (idx == 0) {
            head = head->next;
            delete temp;
        } else {
            Node* prev = nullptr;
            for (int i = 0; i < idx; i++) {
                prev = temp;
                temp = temp->next;
            }
            prev->next = temp->next;
            delete temp;
        }
        currentSize--;
        return true;
    }

    // Получение элемента по индексу
    Constructor* getAt(int idx) {
        if (idx < 0 || idx >= currentSize) return nullptr;
        Node* temp = head;
        for (int i = 0; i < idx; i++) {
            temp = temp->next;
        }
        return &(temp->data);
    }

    // Вывод всей таблицы
    void displayAll() const {
        if (head == nullptr) {
            cout << "\n[!] Справочник пуст. База данных не содержит элементов.\n";
            return;
        }

        printHeader();
        Node* temp = head;
        int idx = 0;
        while (temp != nullptr) {
            printRow(idx++, temp->data);
            temp = temp->next;
        }
        printFooter();
    }

    // Индивидуальный запрос: поиск самого дорогого конструктора
    void findMostExpensive() const {
        if (head == nullptr) {
            cout << "\n[!] База данных пуста.\n";
            return;
        }
        Node* temp = head;
        Node* maxNode = head;
        while (temp != nullptr) {
            if (temp->data.getPrice() > maxNode->data.getPrice()) {
                maxNode = temp;
            }
            temp = temp->next;
        }
        cout << "\n--- САМЫЙ ДОРОГОЙ КОНСТРУКТОР В БАЗЕ ---\n" << maxNode->data;
    }

    // Индивидуальный запрос: фильтрация по возрасту и цене
    void filterByAgeAndPrice(int age, double maxPrice) const {
        if (head == nullptr) {
            cout << "\n[!] База данных пуста.\n";
            return;
        }

        Node* temp = head;
        int idx = 0;
        bool found = false;

        while (temp != nullptr) {
            if (temp->data.getAgeLimit() <= age && temp->data.getPrice() <= maxPrice) {
                if (!found) {
                    cout << "\n--- РЕЗУЛЬТАТЫ ПОИСКА (Возраст до " << age << ", Макс. Цена: " << maxPrice << ") ---\n";
                    printHeader();
                    found = true;
                }
                printRow(idx, temp->data);
            }
            idx++;
            temp = temp->next;
        }

        if (found) printFooter();
        else cout << "\n[-] По указанным критериям ничего не найдено.\n";
    }

    // Сохранение в бинарный файл
    void saveToFile() const {
        ofstream outFile(filename, ios::binary | ios::trunc);
        if (!outFile) {
            cout << "[!] Ошибка при открытии файла для сохранения!\n";
            return;
        }

        Node* temp = head;
        while (temp != nullptr) {
            size_t len = temp->data.getManufacturer().size();
            outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
            outFile.write(temp->data.getManufacturer().c_str(), len);

            int age = temp->data.getAgeLimit();
            double pr = temp->data.getPrice();
            int parts = temp->data.getPartsCount();

            outFile.write(reinterpret_cast<const char*>(&age), sizeof(age));
            outFile.write(reinterpret_cast<const char*>(&pr), sizeof(pr));
            outFile.write(reinterpret_cast<const char*>(&parts), sizeof(parts));

            temp = temp->next;
        }
        outFile.close();
        cout << "[+] Данные успешно сохранены в бинарный файл: " << filename << endl;
    }

    // Загрузка из бинарного файла
    void loadFromFile() {
        clear();
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "[i] Бинарный файл базы данных не найден. Будет создан новый при выходе.\n";
            return;
        }

        while (inFile.peek() != EOF) {
            size_t len;
            if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;

            string m(len, ' ');
            inFile.read(&m[0], len);

            int age;
            double pr;
            int parts;

            inFile.read(reinterpret_cast<char*>(&age), sizeof(age));
            inFile.read(reinterpret_cast<char*>(&pr), sizeof(pr));
            inFile.read(reinterpret_cast<char*>(&parts), sizeof(parts));

            push_back(Constructor(m, age, pr, parts));
        }
        inFile.close();
        cout << "[+] База данных успешно загружена из файла!\n";
    }

    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        currentSize = 0;
    }

private:
    void printHeader() const {
        cout << "+" << string(4, '-') << "+" << string(22, '-') << "+" << string(14, '-') << "+" << string(14, '-') << "+" << string(14, '-') << "+\n";
        cout << "| " << setw(2) << left << "ID" 
             << " | " << setw(20) << left << "Производитель" 
             << " | " << setw(12) << left << "Возраст до" 
             << " | " << setw(12) << left << "Цена (руб)" 
             << " | " << setw(12) << left << "Детали (шт)" << " |\n";
        cout << "+" << string(4, '-') << "+" << string(22, '-') << "+" << string(14, '-') << "+" << string(14, '-') << "+" << string(14, '-') << "+\n";
    }

    void printRow(int id, const Constructor& c) const {
        cout << "| " << setw(2) << left << id 
             << " | " << setw(20) << left << c.getManufacturer() 
             << " | " << setw(12) << left << c.getAgeLimit() 
             << " | " << setw(12) << left << fixed << setprecision(2) << c.getPrice() 
             << " | " << setw(12) << left << c.getPartsCount() << " |\n";
    }

    void printFooter() const {
        cout << "+" << string(4, '-') << "+" << string(22, '-') << "+" << string(14, '-') << "+" << string(14, '-') << "+" << string(14, '-') << "+\n";
    }
};

// Функции валидации ввода из твоего КУРСАЧ.txt
int getValidInt() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        while (cin.get() != '\n');
        cout << "[!] Ошибка ввода! Введите целое число: ";
    }
    return val;
}

double getValidDouble() {
    double val;
    while (!(cin >> val)) {
        cin.clear();
        while (cin.get() != '\n');
        cout << "[!] Ошибка ввода! Введите число: ";
    }
    return val;
}

void showMenu() {
    cout << "\n===== ИНФОРМАЦИОННО-СПРАВОЧНАЯ СИСТЕМА =====\n"
         << "1. Просмотр базы данных (Вывод таблицей)\n"
         << "2. Добавить новый конструктор в список\n"
         << "3. Удалить конструктор по ID\n"
         << "4. Вызвать стандартные методы лабы (play, unpack...)\n"
         << "5. Протестировать перегруженные операторы (++, --)\n"
         << "6. ЗАДАНИЕ: Найти самый дорогой конструктор\n"
         << "7. ЗАДАНИЕ: Фильтр по возрасту и максимальной цене\n"
         << "0. Выход из программы с сохранением в файл\n"
         << "=============================================\n"
         << "Ваш выбор: ";
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    ConstructorList db("constructors_db.bin");
    db.loadFromFile();

    if (db.empty()) {
        db.push_back(Constructor("Lego", 6, 3500.0, 450));
        db.push_back(Constructor("Bauer", 3, 1200.0, 120));
    }

    int choice;
    do {
        showMenu();
        choice = getValidInt();

        switch (choice) {
            case 1:
                db.displayAll();
                break;

            case 2: {
                string m; int a, c; double p;
                cout << "\nВведите имя производителя: "; cin >> m;
                cout << "Введите возрастное ограничение: "; a = getValidInt();
                cout << "Введите цену: "; p = getValidDouble();
                cout << "Введите количество деталей: "; c = getValidInt();
                db.push_back(Constructor(m, a, p, c));
                cout << "[+] Конструктор добавлен!\n";
                break;
            }

            case 3: {
                if (db.empty()) { cout << "\nБаза данных пуста!\n"; break; }
                cout << "\nВведите ID для удаления (0 - " << db.size() - 1 << "): ";
                int id = getValidInt();
                if (db.removeAt(id)) cout << "[+] Удалено!\n";
                else cout << "[-] Неверный ID!\n";
                break;
            }

            case 4: {
                if (db.empty()) { cout << "\nБаза данных пуста!\n"; break; }
                cout << "\nВведите ID (0 - " << db.size() - 1 << "): ";
                int id = getValidInt();
                Constructor* obj = db.getAt(id);
                if (obj) {
                    obj->play();
                    obj->unpack();
                    obj->cleanup();
                } else cout << "[-] Неверный ID!\n";
                break;
            }

            case 5: {
                if (db.empty()) { cout << "\nБаза данных пуста!\n"; break; }
                cout << "\nВведите ID конструктора для теста перегрузок (0 - " << db.size() - 1 << "): ";
                int id = getValidInt();
                Constructor* obj = db.getAt(id);
                if (obj) {
                    cout << "\n[!] До применения: " << *obj;
                    (*obj)++;
                    cout << "После оператора ++ (Цена +100): " << *obj;
                    (*obj)--;
                    cout << "После оператора -- (Цена -100): " << *obj;
                } else cout << "[-] Неверный ID!\n";
                break;
            }

            case 6:
                db.findMostExpensive();
                break;

            case 7: {
                cout << "\nВведите желаемый возраст ребенка: "; int age = getValidInt();
                cout << "Введите ваш максимальный бюджет (цена): "; double budget = getValidDouble();
                db.filterByAgeAndPrice(age, budget);
                break;
            }

            case 0:
                cout << "\nСохранение базы данных и завершение работы...\n";
                db.saveToFile();
                break;

            default:
                cout << "\n[!] Неверный выбор!\n";
        }
    } while (choice != 0);

    return 0;
}
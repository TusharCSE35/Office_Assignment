#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>

template <typename T>
class _Node {
public:
    T* data;
    _Node<T>* next;

    _Node(T* data) : data(data), next(nullptr) {}

    T* _getData() {
        return data;
    }

    _Node<T>* _getnext() {
        return next;
    }

    void _setnext(_Node<T>* newNext) {
        next = newNext;
    }
};

template <typename T>
class _Linkedlist {
public:
    _Node<T>* _head;

    _Linkedlist() : _head(nullptr) {}

    void add(T* data) {
        _Node<T>* newNode = new _Node<T>(data);
        if (_head == nullptr) {
            _head = newNode;
        } else {
            _Node<T>* curr = _head;
            while (curr->_getnext()) {
                curr = curr->_getnext();
            }
            curr->_setnext(newNode);
        }
    }

    void display() {
        _Node<T>* curr = _head;
        while (curr != nullptr) {
            curr->_getData()->display();
            curr = curr->_getnext();
        }
    }

    T* search(int id) {
        _Node<T>* curr = _head;
        while (curr != nullptr) {
            if (curr->_getData()->id == id) {
                return curr->_getData();
            }
            curr = curr->_getnext();
        }
        return nullptr;
    }

    bool update(int id, const std::string& newName) {
        _Node<T>* curr = _head;
        while (curr != nullptr) {
            if (curr->_getData()->id == id) {
                curr->_getData()->name = newName;
                return true;
            }
            curr = curr->_getnext();
        }
        return false;
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            return false;  // If the file doesn't exist, return false
        }

        int id;
        std::string name;
        while (file >> id >> std::ws && std::getline(file, name)) {
            T* newEntity = new T(id, name, "");  // Empty sublist for simplicity
            add(newEntity);
        }

        file.close();
        return true;
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        _Node<T>* curr = _head;
        while (curr != nullptr) {
            file << curr->_getData()->id << " " << curr->_getData()->name << std::endl;
            curr = curr->_getnext();
        }

        file.close();
    }
};

template <typename T>
class _Entity {
public:
    int id;
    std::string name;
    T sublist;

    _Entity(int id, std::string name, T sublist)
        : id(id), name(name), sublist(sublist) {}

    void display() {
        std::cout << "ID: " << id << ", Name: " << name << std::endl;
    }
};

class _CRM {
public:
    _Linkedlist<_Entity<std::string>> leads;
    _Linkedlist<_Entity<std::string>> contacts;

    _CRM() {
        leads.loadFromFile("leads4.txt");
        contacts.loadFromFile("contacts4.txt");
    }

    void addLead() {
        int id;
        std::string name;
        std::cout << "Enter Lead ID: ";
        std::cin >> id;
        std::cout << "Enter Lead Name: ";
        std::cin >> name;

        _Entity<std::string>* newLead = new _Entity<std::string>(id, name, "Lead sublist");
        leads.add(newLead);
        leads.saveToFile("leads4.txt");  // Save to file after adding
    }

    void addContact() {
        int id;
        std::string name;
        std::cout << "Enter Contact ID: ";
        std::cin >> id;
        std::cout << "Enter Contact Name: ";
        std::cin >> name;

        _Entity<std::string>* newContact = new _Entity<std::string>(id, name, "Contact sublist");
        contacts.add(newContact);
        contacts.saveToFile("contacts4.txt");  // Save to file after adding
    }

    void searchLead() {
        int id;
        std::cout << "Enter Lead ID to search: ";
        std::cin >> id;
        _Entity<std::string>* foundLead = leads.search(id);
        if (foundLead) {
            foundLead->display();
        } else {
            std::cout << "Lead with ID " << id << " not found." << std::endl;
        }
    }

    void searchContact() {
        int id;
        std::cout << "Enter Contact ID to search: ";
        std::cin >> id;
        _Entity<std::string>* foundContact = contacts.search(id);
        if (foundContact) {
            foundContact->display();
        } else {
            std::cout << "Contact with ID " << id << " not found." << std::endl;
        }
    }

    void updateLead() {
        int id;
        std::string newName;
        std::cout << "Enter Lead ID to update: ";
        std::cin >> id;
        std::cout << "Enter new Lead Name: ";
        std::cin >> newName;
        if (leads.update(id, newName)) {
            std::cout << "Lead updated successfully." << std::endl;
            leads.saveToFile("leads4.txt");  // Save after update
        } else {
            std::cout << "Lead with ID " << id << " not found." << std::endl;
        }
    }

    void updateContact() {
        int id;
        std::string newName;
        std::cout << "Enter Contact ID to update: ";
        std::cin >> id;
        std::cout << "Enter new Contact Name: ";
        std::cin >> newName;
        if (contacts.update(id, newName)) {
            std::cout << "Contact updated successfully." << std::endl;
            contacts.saveToFile("contacts4.txt");  // Save after update
        } else {
            std::cout << "Contact with ID " << id << " not found." << std::endl;
        }
    }

    void displayLeads() {
        std::cout << "Leads:\n";
        leads.display();
    }

    void displayContacts() {
        std::cout << "Contacts:\n";
        contacts.display();
    }

    void displayLeadsWithContacts() {
        std::cout << "Leads with Contacts:\n";
        _Node<_Entity<std::string>>* leadCurr = leads._head;
        while (leadCurr != nullptr) {
            leadCurr->_getData()->display();
            std::cout << "Associated Contacts:\n";
            _Node<_Entity<std::string>>* contactCurr = contacts._head;
            while (contactCurr != nullptr) {
                contactCurr->_getData()->display();
                contactCurr = contactCurr->_getnext();
            }
            leadCurr = leadCurr->_getnext();
        }
    }

    ~_CRM() {
        leads.saveToFile("leads4.txt");
        contacts.saveToFile("contacts4.txt");
    }
};

int main() {
    _CRM manager;

    while (true) {
        std::cout << "\nEnter 1 to add leads:" << std::endl;
        std::cout << "Enter 2 to add contacts:" << std::endl;
        std::cout << "Enter 3 to search leads:" << std::endl;
        std::cout << "Enter 4 to search contacts:" << std::endl;
        std::cout << "Enter 5 to update leads:" << std::endl;
        std::cout << "Enter 6 to update contacts:" << std::endl;
        std::cout << "Enter 7 to display leads:" << std::endl;
        std::cout << "Enter 8 to display contacts:" << std::endl;
        std::cout << "Enter 9 to display Leads with Contacts:" << std::endl;
        std::cout << "Enter 10 to Exit:" << std::endl;

        int i;
        std::cin >> i;

        if (i == 10) {
            break;
        }

        switch (i) {
        case 1: manager.addLead(); break;
        case 2: manager.addContact(); break;
        case 3: manager.searchLead(); break;
        case 4: manager.searchContact(); break;
        case 5: manager.updateLead(); break;
        case 6: manager.updateContact(); break;
        case 7: manager.displayLeads(); break;
        case 8: manager.displayContacts(); break;
        case 9: manager.displayLeadsWithContacts(); break;
        default: std::cout << "Invalid Code" << std::endl; break;
        }
    }

    return 0;
}

#include<bits/stdc++.h>
using namespace std;

class Lead;
class Contact;
template <typename T> class Node;
template <typename T> class List;

class Contact {
private:
    int id;
    string name;
    string email;
    string address;

public:
    Contact(int id, const string& name, const string& email, const string& address)
        : id(id), name(name), email(email), address(address) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getAddress() const { return address; }

    void setName(const string& name) { this->name = name; }
    void setEmail(const string& email) { this->email = email; }
    void setAddress(const string& address) { this->address = address; }

    void display() const {
        cout << "Contact ID: " << id << ", Name: " << name << ", Email: " << email << ", Address: " << address << endl;
    }
};

class Lead {
private:
    int id;
    string name;
    string address;
    string website;
    List<Contact> contacts;

public:
    Lead(int id, const string& name, const string& address, const string& website)
        : id(id), name(name), address(address), website(website) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getWebsite() const { return website; }

    void setName(const string& name) { this->name = name; }
    void setAddress(const string& address) { this->address = address; }
    void setWebsite(const string& website) { this->website = website; }

    void addContact(const Contact& contact) {
        contacts.add(contact);
    }

    void displayContacts() const {
        contacts.displayAll([](const Contact& contact) {
            contact.display();
        });
    }

    void display() const {
        cout << "Lead ID: " << id << ", Name: " << name << ", Address: " << address << ", Website: " << website << endl;
    }

    List<Contact>& getContacts() { return contacts; }
    Node<Contact>* getContactsHead() { return contacts.getHead(); }
};

template <typename T>
class Node {
public:
    T data;
    Node* next;

    Node(const T& data) : data(data), next(nullptr) {}
};

template <typename T>
class List {
private:
    Node<T>* head;
    Node<T>* tail;

public:
    List() : head(nullptr), tail(nullptr) {}

    void add(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void displayAll(void (*displayFunc)(const T&)) const {
        Node<T>* current = head;
        while (current) {
            displayFunc(current->data);
            current = current->next;
        }
    }

    Node<T>* search(bool (*condition)(const T&)) const {
        Node<T>* current = head;
        while (current) {
            if (condition(current->data)) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void saveDataToFile(const string& filename, void (*saveFunc)(ofstream&, const T&)) const {
        ofstream file(filename);
        Node<T>* current = head;
        while (current) {
            saveFunc(file, current->data);
            current = current->next;
        }
        file.close();
    }

    void loadDataFromFile(const string& filename, T (*loadFunc)(ifstream&)) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            T data = loadFunc(ss);
            add(data);
        }
        file.close();
    }

    Node<T>* getHead() const { return head; }
};

// CRMModel Class (Business Logic)
class CRMModel {
private:
    List<Lead> leadList;
    List<Contact> contactList;
    int leadCounter = 0;
    int contactCounter = 0;

public:
    void addLead(const string& name, const string& address, const string& website) {
        Lead lead(++leadCounter, name, address, website);
        leadList.add(lead);
    }

    void addContactToLead(int leadId, const string& contactName, const string& email, const string& address) {
        Node<Lead>* leadNode = leadList.search([&](const Lead& lead) { return lead.getId() == leadId; });
        if (leadNode) {
            Contact contact(contactCounter++, contactName, email, address);
            leadNode->data.addContact(contact);
            contactList.add(contact);
        }
    }

    void modifyLead(int leadId, const string& newName, const string& newAddress, const string& newWebsite) {
        Node<Lead>* leadNode = leadList.search([&](const Lead& lead) { return lead.getId() == leadId; });
        if (leadNode) {
            leadNode->data.setName(newName);
            leadNode->data.setAddress(newAddress);
            leadNode->data.setWebsite(newWebsite);
        }
    }

    void modifyContact(int contactId, const string& newName, const string& newEmail, const string& newAddress) {
        Node<Contact>* contactNode = contactList.search([&](const Contact& contact) { return contact.getId() == contactId; });
        if (contactNode) {
            contactNode->data.setName(newName);
            contactNode->data.setEmail(newEmail);
            contactNode->data.setAddress(newAddress);
        }
    }

    Node<Lead>* searchLeadByName(const string& leadName) {
        return leadList.search([&](const Lead& lead) { return lead.getName() == leadName; });
    }

    Node<Contact>* searchContactByName(const string& contactName) {
        return contactList.search([&](const Contact& contact) { return contact.getName() == contactName; });
    }

    void displayLeadsAndContacts() {
        leadList.displayAll([](const Lead& lead) {
            lead.display();
            lead.displayContacts();
        });
    }

    void displayContacts() {
        contactList.displayAll([](const Contact& contact) { contact.display(); });
    }

    void saveDataToFile() {
        leadList.saveDataToFile("leads4.txt", [](ofstream& file, const Lead& lead) {
            file << lead.getId() << "|" << lead.getName() << "|" << lead.getAddress() << "|" << lead.getWebsite() << endl;
            Node<Contact>* contactNode = lead.getContactsHead();
            while (contactNode) {
                const Contact& contact = contactNode->data;
                file << contact.getId() << "|" << contact.getName() << "|" << contact.getEmail() << "|" << contact.getAddress() << endl;
                contactNode = contactNode->next;
            }
        });

        contactList.saveDataToFile("contacts4.txt", [](ofstream& file, const Contact& contact) {
            file << contact.getId() << "|" << contact.getName() << "|" << contact.getEmail() << "|" << contact.getAddress() << endl;
        });
    }

    void loadDataFromFile() {
        leadList.loadDataFromFile("leads4.txt", [this](ifstream& ss) {
            string id, name, address, website;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, address, '|');
            getline(ss, website, '|');
            Lead lead(stoi(id), name, address, website);
            leadCounter = max(leadCounter, stoi(id));
            return lead;
        });

        contactList.loadDataFromFile("contacts4.txt", [](ifstream& ss) {
            string id, name, email, address;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, address, '|');
            Contact contact(stoi(id), name, email, address);
            return contact;
        });
    }
};

// CRMView Class (Presentation Logic)
class CRMView {
public:
    void displayMenu() {
        cout << "--- CRM Menu ---" << endl;
        cout << "1. Add Lead" << endl;
        cout << "2. Add Contact to Lead" << endl;
        cout << "3. Modify Lead" << endl;
        cout << "4. Modify Contact" << endl;
        cout << "5. Search Lead by Name" << endl;
        cout << "6. Search Contact by Name" << endl;
        cout << "7. Display All Leads and Contacts" << endl;
        cout << "8. Display All Contacts" << endl;
        cout << "9. Exit" << endl;
    }

    int getChoice() {
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        return choice;
    }

    void promptLeadDetails(string& name, string& address, string& website) {
        cout << "Enter Lead Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Lead Address: ";
        getline(cin, address);
        cout << "Enter Lead Website: ";
        getline(cin, website);
    }

    void promptContactDetails(string& contactName, string& email, string& address) {
        cout << "Enter Contact Name: ";
        cin.ignore();
        getline(cin, contactName);
        cout << "Enter Contact Email: ";
        getline(cin, email);
        cout << "Enter Contact Address: ";
        getline(cin, address);
    }

    void displayLead(const Lead& lead) {
        lead.display();
    }

    void displayContact(const Contact& contact) {
        contact.display();
    }

    void displayMessage(const string& message) {
        cout << message << endl;
    }

    void displayErrorMessage(const string& message) {
        cout << "Error: " << message << endl;
    }
};

// CRMController Class (Connecting Model and View)
class CRMController {
private:
    CRMModel model;
    CRMView view;

public:
    void run() {
        model.loadDataFromFile();

        while (true) {
            view.displayMenu();
            int choice = view.getChoice();

            switch (choice) {
                case 1: {
                    string name, address, website;
                    view.promptLeadDetails(name, address, website);
                    model.addLead(name, address, website);
                    view.displayMessage("Lead added successfully.");
                    break;
                }
                case 2: {
                    string leadName;
                    cout << "Enter Lead Name to search: ";
                    cin.ignore();
                    getline(cin, leadName);
                    Node<Lead>* leadNode = model.searchLeadByName(leadName);
                    if (leadNode) {
                        int leadId;
                        cout << "Enter Lead ID to add a contact: ";
                        cin >> leadId;
                        string contactName, email, address;
                        view.promptContactDetails(contactName, email, address);
                        model.addContactToLead(leadId, contactName, email, address);
                        view.displayMessage("Contact added successfully.");
                    } else {
                        view.displayErrorMessage("Lead not found.");
                    }
                    break;
                }
                case 3: {
                    int leadId;
                    cout << "Enter Lead ID to modify: ";
                    cin >> leadId;
                    string newName, newAddress, newWebsite;
                    view.promptLeadDetails(newName, newAddress, newWebsite);
                    model.modifyLead(leadId, newName, newAddress, newWebsite);
                    view.displayMessage("Lead modified successfully.");
                    break;
                }
                case 4: {
                    int contactId;
                    cout << "Enter Contact ID to modify: ";
                    cin >> contactId;
                    string newName, newEmail, newAddress;
                    view.promptContactDetails(newName, newEmail, newAddress);
                    model.modifyContact(contactId, newName, newEmail, newAddress);
                    view.displayMessage("Contact modified successfully.");
                    break;
                }
                case 5: {
                    string leadName;
                    cout << "Enter Lead Name to search: ";
                    cin.ignore();
                    getline(cin, leadName);
                    Node<Lead>* leadNode = model.searchLeadByName(leadName);
                    if (leadNode) {
                        view.displayLead(leadNode->data);
                    } else {
                        view.displayErrorMessage("Lead not found.");
                    }
                    break;
                }
                case 6: {
                    string contactName;
                    cout << "Enter Contact Name to search: ";
                    cin.ignore();
                    getline(cin, contactName);
                    Node<Contact>* contactNode = model.searchContactByName(contactName);
                    if (contactNode) {
                        view.displayContact(contactNode->data);
                    } else {
                        view.displayErrorMessage("Contact not found.");
                    }
                    break;
                }
                case 7: {
                    model.displayLeadsAndContacts();
                    break;
                }
                case 8: {
                    model.displayContacts();
                    break;
                }
                case 9:
                    model.saveDataToFile();
                    cout << "Exiting program. Goodbye!" << endl;
                    return;
                default:
                    view.displayErrorMessage("Invalid choice! Try again.");
            }
        }
    }
};

int main() {
    cout << endl << "--- Customer Relation Management System ---" << endl;
    CRMController controller;
    controller.run();
    return 0;
}




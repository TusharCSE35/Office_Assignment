#include <bits/stdc++.h>
using namespace std;

template <typename T>
class Node {
public:
    T data;
    Node* next;

    Node(T value) : data(value), next(nullptr) {}
};

template <typename T>
class List {
private:
    Node<T>* head;

public:
    List() : head(nullptr) {}

    void add(const T& item) {
        Node<T>* newNode = new Node<T>(item);
        if (!head) {
            head = newNode;
            return;
        }

        Node<T>* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    Node<T>* search(const function<bool(const T&)>& predicate) {
        Node<T>* temp = head;
        while (temp) {
            if (predicate(temp->data)) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void displayAll(const function<void(const T&)>& displayFn) const {
        if (!head) {
            cout << "    No contacts available." << endl;
            return;
        }

        Node<T>* temp = head;
        while (temp) {
            displayFn(temp->data);
            temp = temp->next;
        }
    }

    Node<T>* getHead() const { return head; }
};

class Contact {
private:
    int _id;
    string _name;
    string _email;
    string _address;

public:
    Contact(int id, const string& name, const string& email, const string& address)
        : _id(id), _name(name), _email(email), _address(address) {}

    int getId() const { return _id; }
    string getName() const { return _name; }
    string getEmail() const { return _email; }
    string getAddress() const { return _address; }

    void setName(const string& name) { _name = name; }
    void setEmail(const string& email) { _email = email; }
    void setAddress(const string& address) { _address = address; }

    void display() const {
        cout << "    Contact(ID: " << _id << ", Name: " << _name
             << ", Email: " << _email << ", Address: " << _address << ")" << endl;
    }
};

class Lead {
private:
    int _id;
    string _name;
    string _address;
    string _website;
    List<Contact> contactList;

public:
    Lead(int id, const string& name, const string& address, const string& website)
        : _id(id), _name(name), _address(address), _website(website) {}

    int getId() const { return _id; }
    string getName() const { return _name; }
    string getAddress() const { return _address; }
    string getWebsite() const { return _website; }

    void setName(const string& name) { _name = name; }
    void setAddress(const string& address) { _address = address; }
    void setWebsite(const string& website) { _website = website; }

    void addContact(const Contact& contact) {
        contactList.add(contact);
    }

    Node<Contact>* searchContactByName(const string& name) {
        return contactList.search([&](const Contact & contact) {
            return contact.getName() == name;
        });
    }

    void display() const {
        cout << "Lead(ID: " << _id << ", Name: " << _name
             << ", Address: " << _address << ", Website: " << _website << ")" << endl;
    }

    void displayContacts() const {
        contactList.displayAll([](const Contact & contact) { contact.display(); });
    }

    Node<Contact>* getContactsHead() const { return contactList.getHead(); }
};

class DataStorage {
public:
    virtual void saveLeads(const List<Lead>& leadList) = 0;
    virtual void saveContacts(const List<Contact>& contactList, const List<Lead>& leadList) = 0;
    virtual void loadLeads(List<Lead>& leadList) = 0;
    virtual void loadContacts(List<Contact>& contactList, List<Lead>& leadList) = 0;
    virtual ~DataStorage() = default;
};

class FileBasedStorage : public DataStorage {
public:
    void saveLeads(const List<Lead>& leadList) override {
        ofstream leadFile("leads2.txt");
        if (!leadFile.is_open()) {
            throw runtime_error("Error opening leads2.txt for saving.");
        }

        Node<Lead>* leadNode = leadList.getHead();
        while (leadNode) {
            const Lead& lead = leadNode->data;
            leadFile << lead.getId() << "|" << lead.getName() << "|" << lead.getAddress() << "|" << lead.getWebsite() << endl;
            leadNode = leadNode->next;
        }

        leadFile.close();
    }

    void saveContacts(const List<Contact>& contactList, const List<Lead>& leadList) override {
        ofstream contactFile("contacts2.txt");
        if (!contactFile.is_open()) {
            throw runtime_error("Error opening contacts2.txt for saving.");
        }

        Node<Contact>* contactNode = contactList.getHead();
        while (contactNode) {
            const Contact& contact = contactNode->data;
            int leadId = -1;
            Node<Lead>* leadNode = leadList.getHead();
            while (leadNode) {

                if (contact.getId() / 100 == leadNode->data.getId()) {
                    leadId = leadNode->data.getId();
                    break;
                }
                leadNode = leadNode->next;
            }

            contactFile << contact.getId() << "|" << contact.getName() << "|" << contact.getEmail() << "|" << contact.getAddress() << "|" << leadId << endl;
            contactNode = contactNode->next;
        }

        contactFile.close();
    }

    void loadLeads(List<Lead>& leadList) override {
        ifstream leadFile("leads2.txt");
        if (!leadFile.is_open()) return;

        string line;
        while (getline(leadFile, line)) {
            stringstream ss(line);
            string id, name, address, website;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, address, '|');
            getline(ss, website, '|');

            Lead lead(stoi(id), name, address, website);
            leadList.add(lead);
        }

        leadFile.close();
    }

    void loadContacts(List<Contact>& contactList, List<Lead>& leadList) override {
        ifstream contactFile("contacts2.txt");
        if (!contactFile.is_open()) return;

        string line;
        while (getline(contactFile, line)) {
            stringstream ss(line);
            string id, name, email, address, leadId;

            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, address, '|');
            getline(ss, leadId, '|');

            Contact contact(stoi(id), name, email, address);
            Node<Lead>* leadNode = leadList.search([&](const Lead & lead) {
                return lead.getId() == stoi(leadId);
            });

            if (leadNode) {
                leadNode->data.addContact(contact);
                contactList.add(contact);
            }
        }

        contactFile.close();
    }
};

class MemoryBasedStorage : public DataStorage {
public:
    void saveLeads(const List<Lead>&) override {
        // cout << "Memory-based mode: No data saved to files." << endl;
    }
    void saveContacts(const List<Contact>&, const List<Lead>&) override {
        // cout << "Memory-based mode: No data saved to files." << endl;
    }
    void loadLeads(List<Lead>&) override {
        // cout << "Memory-based mode: No data loaded from files." << endl;
    }
    void loadContacts(List<Contact>&, List<Lead>&) override {
        // cout << "Memory-based mode: No data loaded from files." << endl;
    }
};

class CRM {
private:
    List<Lead> leadList;
    List<Contact> contactList;
    unique_ptr<DataStorage> storage;
    int leadCounter = 0;
    int contactCounter = 0;

public:
    CRM(bool isFileBased) {
        if (isFileBased) {
            storage = unique_ptr<DataStorage>(new FileBasedStorage());
        } else {
            storage = unique_ptr<DataStorage>(new MemoryBasedStorage());
        }
        storage->loadLeads(leadList);
        storage->loadContacts(contactList, leadList);
    }

    void saveData() {
        storage->saveLeads(leadList);
        storage->saveContacts(contactList, leadList);
    }

    void addLead() {
        string name, address, website;
        cout << "Enter Lead Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Lead Address: ";
        getline(cin, address);
        cout << "Enter Lead Website: ";
        getline(cin, website);

        Lead lead(++leadCounter, name, address, website);
        leadList.add(lead);
        cout << "Lead added successfully." << endl;
    }

    void addContactToLead() {
        string leadName;
        cout << "Enter Lead Name to search: ";
        cin.ignore();
        getline(cin, leadName);

        bool flag = false;
        leadList.displayAll([&](const Lead & lead) {
            if (lead.getName() == leadName) {
                lead.display();
                flag = true;
            }
        });

        if (!flag) {
            cout << "No leads found with the name: " << leadName << endl;
            return;
        }

        int leadId;
        cout << "Enter Lead ID to add a contact: ";
        cin >> leadId;
        Node<Lead>* leadNode = leadList.search([&](const Lead & lead) {
            return lead.getId() == leadId;
        });

        if (!leadNode) {
            cout << "Invalid Lead ID." << endl;
            return;
        }

        leadNode->data.display();

        string contactName, email, address;
        cout << "Enter Contact Name: ";
        cin.ignore();
        getline(cin, contactName);
        cout << "Enter Contact Email: ";
        getline(cin, email);
        cout << "Enter Contact Address: ";
        getline(cin, address);

        int contactId = leadId * 100 + (++contactCounter);
        Contact contact(contactId, contactName, email, address);
        leadNode->data.addContact(contact);
        contactList.add(contact);

        cout << "Contact added successfully to the lead." << endl;
    }

    void modifyLead() {
        string leadName;
        cout << "Enter Lead Name to search: ";
        cin.ignore();
        getline(cin, leadName);

        bool flag = false;
        leadList.displayAll([&](const Lead & lead) {
            if (lead.getName() == leadName) {
                lead.display();
                flag = true;
            }
        });

        if (!flag) {
            cout << "No leads found with the name: " << leadName << endl;
            return;
        }

        int leadId;
        cout << "Enter Lead ID to modify: ";
        cin >> leadId;
        Node<Lead>* leadNode = leadList.search([&](const Lead & lead) {
            return lead.getId() == leadId;
        });

        if (!leadNode) {
            cout << "Lead not found." << endl;
            return;
        }

        string name, address, website;
        cout << "Enter new Lead Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter new Lead Address: ";
        getline(cin, address);
        cout << "Enter new Lead Website: ";
        getline(cin, website);

        leadNode->data.setName(name);
        leadNode->data.setAddress(address);
        leadNode->data.setWebsite(website);

        cout << "Lead updated successfully." << endl;
    }

    void modifyContact() {
        string contactName;
        cout << "Enter Contact Name to search: ";
        cin.ignore();
        getline(cin, contactName);

        bool flag = false;
        Node<Lead>* leadNode = leadList.getHead();
        while (leadNode) {
            Node<Contact>* contactNode = leadNode->data.getContactsHead();
            while (contactNode) {
                if (contactNode->data.getName() == contactName) {
                    contactNode->data.display();
                    leadNode->data.display();
                    flag = true;
                }
                contactNode = contactNode->next;
            }
            leadNode = leadNode->next;
        }
        if (!flag) {
            cout << "No contacts found with the name: " << contactName << endl;
        }

        int contactId;
        cout << "Enter Contact ID to modify: ";
        cin >> contactId;

        leadNode = leadList.getHead();
        flag = false;
        while (leadNode) {
            Node<Contact>* contactNode = leadNode->data.getContactsHead();
            while (contactNode) {
                if (contactNode->data.getId() == contactId) {
                    contactNode->data.display();

                    string name, email, address;
                    cout << "Enter new Contact Name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter new Contact Email: ";
                    getline(cin, email);
                    cout << "Enter new Contact Address: ";
                    getline(cin, address);

                    contactNode->data.setName(name);
                    contactNode->data.setEmail(email);
                    contactNode->data.setAddress(address);

                    Node<Contact>* tempContact = contactList.getHead();
                    while (tempContact) {
                        if (tempContact->data.getId() == contactId) {
                            tempContact->data.setName(name);
                            tempContact->data.setEmail(email);
                            tempContact->data.setAddress(address);
                            break;
                        }
                        tempContact = tempContact->next;
                    }

                    cout << "Contact updated successfully." << endl;
                    flag = true;
                    break;
                }
                contactNode = contactNode->next;
            }
            if (flag)break;
            leadNode = leadNode->next;
        }
    }

    void searchLeadsByName() {
        string leadName;
        cout << "Enter Lead Name to search: ";
        cin.ignore();
        getline(cin, leadName);

        bool flag = false;
        leadList.displayAll([&](const Lead & lead) {
            if (lead.getName() == leadName) {
                lead.display();
                flag = true;
            }
        });

        if (!flag) {
            cout << "No leads found with the name: " << leadName << endl;
            return;
        }
    }

    void searchContactsByName() {
        string contactName;
        cout << "Enter Contact Name to search: ";
        cin.ignore();
        getline(cin, contactName);

        bool flag = false;
        Node<Lead>* leadNode = leadList.getHead();
        while (leadNode) {
            Node<Contact>* contactNode = leadNode->data.getContactsHead();
            while (contactNode) {
                if (contactNode->data.getName() == contactName) {
                    contactNode->data.display();
                    leadNode->data.display();
                    flag = true;
                }
                contactNode = contactNode->next;
            }
            leadNode = leadNode->next;
        }

        if (!flag) {
            cout << "No contacts found with the name: " << contactName << endl;
        }
    }

    void displayAllLeadsAndContacts() {
        if (!leadList.getHead()) {
            cout << "No leads available." << endl;
            return;
        }

        cout << "--- All Leads and Contacts ---" << endl;
        Node<Lead>* leadNode = leadList.getHead();

        while (leadNode) {
            leadNode->data.display();
            cout << "  Contacts for this lead:" << endl;
            leadNode->data.displayContacts();

            leadNode = leadNode->next;
        }
    }

    void displayAllLeads() {
        if (!leadList.getHead()) {
            cout << "No leads available." << endl;
            return;
        }

        cout << "--- All Leads ---" << endl;
        Node<Lead>* leadNode = leadList.getHead();
        while (leadNode) {
            leadNode->data.display();
            leadNode = leadNode->next;
        }
    }

    void displayAllContacts() {
        contactList.displayAll([](const Contact & contact) {
            contact.display();
        });
    }
};

int main() {
    cout << endl << "=== Customer Relation Management ===" << endl << endl;
    cout << "Select Mode" << endl;
    cout << "1. File Based" << endl;
    cout << "2. Memory Based" << endl;
    cout << endl << "Enter Based: ";
    int mode;
    cin >> mode;
    CRM crm(mode == 1);

    while (true) {
        cout << "--- CRM Menu ---" << endl;
        cout << "1. Add Lead" << endl;
        cout << "2. Add Contact to Lead" << endl;
        cout << "3. Modify Lead" << endl;
        cout << "4. Modify Contact" << endl;
        cout << "5. Search Lead by Name" << endl;
        cout << "6. Search Contact by Name" << endl;
        cout << "7. Display All Leads" << endl;
        cout << "8. Display All Contacts" << endl;
        cout << "9. Display all Leads and Contacts" << endl;
        cout << "10. Exit" << endl;

        cout << "Enter your choice: ";
        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter an integer value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        };

        switch (choice) {
        case 1:
            crm.addLead();
            break;
        case 2:
            crm.addContactToLead();
            break;
        case 3:
            crm.modifyLead();
            break;
        case 4:
            crm.modifyContact();
            break;
        case 5:
            crm.searchLeadsByName();
            break;
        case 6:
            crm.searchContactsByName();
            break;
        case 7:
            crm.displayAllLeads();
            break;
        case 8:
            crm.displayAllContacts();
            break;
        case 9:
            crm.displayAllLeadsAndContacts();
            break;
        case 10:
            crm.saveData();
            cout << "Exiting program!" << endl;
            return 0;
        default:
            cout << "Invalid choice! Plz try again." << endl;
        }
    }

    return 0;
}

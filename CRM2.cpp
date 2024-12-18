#include <bits/stdc++.h>
using namespace std;


class Contact {
private:
    int _id;
    string _name;
    string _email;
    string _address;

public:
    Contact(int id, string name, string email, string address)
        : _id(id), _name(name), _email(email), _address(address) {}

    int getId() const { return _id; }
    string getName() const { return _name; }
    string getEmail() const { return _email; }
    string getAddress() const { return _address; }

    void setName(const string& name) { _name = name; }
    void setEmail(const string& email) { _email = email; }
    void setAddress(const string& address) { _address = address; }

    void display() const {
        cout << "Contact(ID: " << _id << ", Name: " << _name
             << ", Email: " << _email << ", Address: " << _address << ")" << endl;
    }
};



class cNode {
public:
    Contact data;
    cNode* next;

    cNode(Contact contact) : data(contact), next(nullptr) {}
};



class ContactList {
private:
    cNode* head;

public:
    ContactList() : head(nullptr) {}

    void addContact(const Contact& contact) {
        cNode* newNode = new cNode(contact);
        if (!head) {
            head = newNode;
            return;
        }

        cNode* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    cNode* searchContactByName(const string& name) {
        cNode* temp = head;
        while (temp) {
            if (temp->data.getName() == name) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr; 
    }

    void displayAllContacts() const {
        if (!head) {
            cout << "No contacts available." << endl;
            return;
        }
        cNode* temp = head;
        while (temp) {
            temp->data.display();
            temp = temp->next;
        }
    }

    cNode* getHead() const { return head; }
};



class Lead {
private:
    int _id;
    string _name;
    string _address;
    string _website;
    ContactList _contactList;

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
        _contactList.addContact(contact);
    }

    cNode* searchContactByName(const string& name) {
        return _contactList.searchContactByName(name);
    }

    void display() const {
        cout << "Lead(ID: " << _id << ", Name: " << _name
             << ", Address: " << _address << ", Website: " << _website << ")" << endl;
    }

    void displayContacts() const {
        _contactList.displayAllContacts();
    }

    cNode* getContactsHead() const { return _contactList.getHead(); }
};



class LNode {
public:
    Lead data;
    LNode* next;

    LNode(Lead lead) : data(lead), next(nullptr) {}
};


class LeadList {
private:
    LNode* head;

public:
    LeadList() : head(nullptr) {}

    void addLead(const Lead& lead) {
        LNode* newNode = new LNode(lead);
        if (!head) {
            head = newNode;
            return;
        }

        LNode* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    LNode* searchLeadByName(const string& name) {
        LNode* temp = head;
        while (temp) {
            if (temp->data.getName() == name) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    bool searchAllLeadsByName(const string& name) {
        LNode* temp = head;
        bool found = false;
        while (temp) {
            if (temp->data.getName() == name) {
                found = true;
                temp->data.display(); 
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "No leads found with that name." << endl;
        }
        return found; 
    }

    bool searchAllContactsByName(const string& name) {
        LNode* temp = head;
        bool found = false;
        while (temp) {
            cNode* contactTemp = temp->data.getContactsHead();
            while (contactTemp) {
                if (contactTemp->data.getName() == name) {
                    found = true;
                    contactTemp->data.display();
                    temp->data.display();  
                }
                contactTemp = contactTemp->next;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "No contacts found with that name." << endl;
        }
        return found;
    }

    void displayAllLeadsAndContacts() const {
        LNode* temp = head;
        if(temp==nullptr){
            cout << "No lead and contact!" << endl;
            return;
        }
        while (temp) {
            temp->data.display();
            temp->data.displayContacts();
            temp = temp->next;
        }
    }

    void displayAllContacts() const {
        LNode* temp = head;
        if(temp==nullptr){
            cout << "No contact available!" << endl;
            return;
        }
        while (temp) {
            temp->data.displayContacts();
            temp = temp->next;
        }
    }

    LNode* getHead() const { return head; }
};


class CRM {
private:
    LeadList leadList;
    ContactList mContactList;
    int leadCounter = 0;
    int contactCounter = 0;

public:
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
        leadList.addLead(lead);
        cout << "Lead added successfully." << endl;
    }

    void addContactToLead() {
        string name;
        cout << "Enter Lead Name to add contact: ";
        cin.ignore();
        getline(cin, name);

        if (leadList.searchAllLeadsByName(name)) { 
            int leadId;
            cout << "Enter Lead ID to add contact under that lead: ";
            cin >> leadId;
            LNode* leadNode = leadList.getHead();
            while (leadNode) {
                if (leadNode->data.getId() == leadId) {
                    leadNode->data.display(); 

                    string contactName, email, address;
                    cout << "Enter Contact Name: ";
                    cin.ignore();
                    getline(cin, contactName);
                    cout << "Enter Contact Email: ";
                    getline(cin, email);
                    cout << "Enter Contact Address: ";
                    getline(cin, address);

                    int contactId = leadNode->data.getId() * 100 + (++contactCounter);
                
                    Contact contact(contactId, contactName, email, address);
                    leadNode->data.addContact(contact);
                    mContactList.addContact(contact);

                    cout << "Contact added successfully." << endl;
                    return;
                }
                leadNode = leadNode->next;
            }

            cout << "Lead not found." << endl;
        }
    }

    void modifyContact() {
        cout << "Enter Contact Name to modify: ";
        string name;
        cin.ignore();
        getline(cin, name);

        if(leadList.searchAllContactsByName(name)){

            int contactId;
            cout << "Enter Contact ID to modify: ";
            cin >> contactId;

            bool found = false;
            LNode* leadTemp = leadList.getHead();

            while (leadTemp) {
                cNode* contactTemp = leadTemp->data.getContactsHead();
                while (contactTemp) {
                    if (contactTemp->data.getId() == contactId) {
                        found = true;
                        contactTemp->data.display();

                      
                        string newName, newEmail, newAddress;
                        cout << "Enter new Contact Name: ";
                        cin.ignore();
                        getline(cin, newName);
                        cout << "Enter new Contact Email: ";
                        getline(cin, newEmail);
                        cout << "Enter new Contact Address: ";
                        getline(cin, newAddress);

                        contactTemp->data.setName(newName);
                        contactTemp->data.setEmail(newEmail);
                        contactTemp->data.setAddress(newAddress);

                        cNode* globalContactTemp = mContactList.getHead();
                        while (globalContactTemp) {
                            if (globalContactTemp->data.getId() == contactId) {
                                globalContactTemp->data.setName(newName);
                                globalContactTemp->data.setEmail(newEmail);
                                globalContactTemp->data.setAddress(newAddress);
                                break;
                            }
                            globalContactTemp = globalContactTemp->next;
                        }

                        cout << "Contact updated successfully." << endl;
                            break;
                    }
                    contactTemp = contactTemp->next;
                }
                if (found) break;
                leadTemp = leadTemp->next;
            }

            if (!found) {
                cout << "No contacts found with that ID." << endl;
            }
        }
    }

    void modifyLead() {
        cout << "Enter Lead Name to modify: ";
        string name;
        cin.ignore();
        getline(cin, name);

        if(leadList.searchAllLeadsByName(name)){
            int leadId;
            cout << "Enter Lead ID to modify: ";
            cin >> leadId;

            LNode* leadTemp = leadList.getHead();
            while (leadTemp) {
                if (leadTemp->data.getId() == leadId) {
                    leadTemp->data.display();

                    string newName, newAddress, newWebsite;
                    cout << "Enter new Lead Name: ";
                    cin.ignore();
                    getline(cin, newName);
                    cout << "Enter new Lead Address: ";
                    getline(cin, newAddress);
                    cout << "Enter new Lead Website: ";
                    getline(cin, newWebsite);

                    leadTemp->data.setName(newName);
                    leadTemp->data.setAddress(newAddress);
                    leadTemp->data.setWebsite(newWebsite);

                    cout << "Lead updated successfully." << endl;
                    return;
                }
                leadTemp = leadTemp->next;
            }

            cout << "Lead not found." << endl;
        }
    }
    
    void searchLeadByName(){
        cout << "Enter Lead Name to Search: ";
        string name;
        cin.ignore();
        getline(cin, name);

        leadList.searchAllLeadsByName(name);
    }
    
    void searchContactByName(){
        cout << "Enter Contact Name to Search: ";
        string name;
        cin.ignore();
        getline(cin, name);

        leadList.searchAllContactsByName(name);
    }
    
    void displayAllLeadsAndContacts() {
        leadList.displayAllLeadsAndContacts();
    }

    void displayAllContacts() {
        mContactList.displayAllContacts();
    }
};


int main() {
    cout << endl << "Contact Relation Management" << endl;
    CRM crm;

    int choice;
    while (true) {
        cout << endl << "--- CRM Menu ---" << endl;
        cout << "1. Add Lead" << endl;
        cout << "2. Add Contact to Lead" << endl;
        cout << "3. Modify Contact" << endl;
        cout << "4. Modify Lead" << endl;
        cout << "5. Search Lead by Name" << endl;
        cout << "6. Search Contact by Name" << endl;
        cout << "7. Display All Leads and Contacts" << endl;
        cout << "8. Display All Contacts" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) { 
            cout << "Invalid input. Please enter an integer value." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                crm.addLead();
                break;
            case 2:
                crm.addContactToLead();
                break;
            case 3:
                crm.modifyContact();
                break;
            case 4:
                crm.modifyLead();
                break;
            case 5:
                crm.searchLeadByName();
                break;    
            case 6:
                crm.searchContactByName();
                break;
            case 7:
                crm.displayAllLeadsAndContacts();
                break;
            case 8:
                crm.displayAllContacts();
                break;
            case 9:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}

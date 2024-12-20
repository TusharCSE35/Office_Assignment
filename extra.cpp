#include <bits/stdc++.h>
#include <fstream>
using namespace std;

// Add these methods to the CRM class
class CRM {
    // Existing methods...

public:
    // Save all leads and contacts to file
    void saveDataToFile() {
        ofstream leadFile("leads.txt");
        ofstream contactFile("contacts.txt");

        if (!leadFile.is_open() || !contactFile.is_open()) {
            cout << "Error opening file for saving data!" << endl;
            return;
        }

        Node<Lead>* leadNode = leadList.getHead();
        while (leadNode) {
            const Lead& lead = leadNode->data;
            leadFile << lead.getId() << "|" << lead.getName() << "|" << lead.getAddress() << "|" << lead.getWebsite() << endl;

            Node<Contact>* contactNode = lead.getContactsHead();
            while (contactNode) {
                const Contact& contact = contactNode->data;
                contactFile << contact.getId() << "|" << contact.getName() << "|" << contact.getEmail() << "|" << contact.getAddress() << "|" << lead.getId() << endl;
                contactNode = contactNode->next;
            }

            leadNode = leadNode->next;
        }

        leadFile.close();
        contactFile.close();
        cout << "Data saved to files successfully." << endl;
    }

    // Load all leads and contacts from file
    void loadDataFromFile() {
        ifstream leadFile("leads.txt");
        ifstream contactFile("contacts.txt");

        if (!leadFile.is_open() || !contactFile.is_open()) {
            cout << "No data files found. Starting with an empty CRM." << endl;
            return;
        }

        // Load leads
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

            leadCounter = max(leadCounter, stoi(id)); // Update lead counter
        }

        // Load contacts
        while (getline(contactFile, line)) {
            stringstream ss(line);
            string id, name, email, address, leadId;

            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, address, '|');
            getline(ss, leadId, '|');

            Contact contact(stoi(id), name, email, address);

            Node<Lead>* leadNode = leadList.search([&](const Lead& lead) {
                return lead.getId() == stoi(leadId);
            });

            if (leadNode) {
                leadNode->data.addContact(contact);
                contactList.add(contact);
            }

            contactCounter = max(contactCounter, stoi(id) % 100); // Update contact counter
        }

        leadFile.close();
        contactFile.close();
        cout << "Data loaded from files successfully." << endl;
    }
};

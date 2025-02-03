const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

class Node {
    constructor(value) {
        this.data = value;
        this.next = null;
    }
}

class List {
    constructor() {
        this.head = null;
    }

    add(item) {
        const newNode = new Node(item);
        if (!this.head) {
            this.head = newNode;
            return;
        }
        let temp = this.head;
        while (temp.next) {
            temp = temp.next;
        }
        temp.next = newNode;
    }

    search(predicate) {
        let temp = this.head;
        while (temp) {
            if (predicate(temp.data)) {
                return temp;
            }
            temp = temp.next;
        }
        return null;
    }

    displayAll(displayFn) {
        if (!this.head) {
            console.log("    No contacts available.");
            return;
        }
        let temp = this.head;
        while (temp) {
            displayFn(temp.data);
            temp = temp.next;
        }
    }

    getHead() {
        return this.head;
    }
}

class Contact {
    constructor(id, name, email, address) {
        this._id = id;
        this._name = name;
        this._email = email;
        this._address = address;
    }

    getId() {
        return this._id;
    }

    getName() {
        return this._name;
    }

    getEmail() {
        return this._email;
    }

    getAddress() {
        return this._address;
    }

    setName(name) {
        this._name = name;
    }

    setEmail(email) {
        this._email = email;
    }

    setAddress(address) {
        this._address = address;
    }

    display() {
        console.log(`    Contact(ID: ${this._id}, Name: ${this._name}, Email: ${this._email}, Address: ${this._address})`);
    }
}

class Lead {
    constructor(id, name, address, website) {
        this._id = id;
        this._name = name;
        this._address = address;
        this._website = website;
        this.contactList = new List();
    }

    getId() {
        return this._id;
    }

    getName() {
        return this._name;
    }

    getAddress() {
        return this._address;
    }

    getWebsite() {
        return this._website;
    }

    setName(name) {
        this._name = name;
    }

    setAddress(address) {
        this._address = address;
    }

    setWebsite(website) {
        this._website = website;
    }

    addContact(contact) {
        this.contactList.add(contact);
    }

    searchContactByName(name) {
        return this.contactList.search(contact => contact.getName() === name);
    }

    display() {
        console.log(`Lead(ID: ${this._id}, Name: ${this._name}, Address: ${this._address}, Website: ${this._website})`);
    }

    displayContacts() {
        this.contactList.displayAll(contact => contact.display());
    }

    getContactsHead() {
        return this.contactList.getHead();
    }
}

class CRM {
    constructor() {
        this.leadList = new List();
        this.contactList = new List();
        this.leadCounter = 0;
        this.contactCounter = 0;
    }

    askQuestion(question) {
        return new Promise(resolve => {
            rl.question(question, resolve);
        });
    }

    async addLead() {
        const name = await this.askQuestion("Enter Lead Name: ");
        const address = await this.askQuestion("Enter Lead Address: ");
        const website = await this.askQuestion("Enter Lead Website: ");

        const lead = new Lead(++this.leadCounter, name, address, website);
        this.leadList.add(lead);
        console.log("Lead added successfully.");
    }

    async addContactToLead() {
        const leadName = await this.askQuestion("Enter Lead Name to search: ");
        let flag = false;

        this.leadList.displayAll(lead => {
            if (lead.getName() === leadName) {
                lead.display();
                flag = true;
            }
        });

        if (!flag) {
            console.log(`No leads found with the name: ${leadName}`);
            return;
        }

        const leadId = parseInt(await this.askQuestion("Enter Lead ID to add a contact: "), 10);
        const leadNode = this.leadList.search(lead => lead.getId() === leadId);

        if (!leadNode) {
            console.log("Invalid Lead ID.");
            return;
        }

        leadNode.data.display();

        const contactName = await this.askQuestion("Enter Contact Name: ");
        const email = await this.askQuestion("Enter Contact Email: ");
        const address = await this.askQuestion("Enter Contact Address: ");

        const contactId = leadId * 100 + (++this.contactCounter);
        const contact = new Contact(contactId, contactName, email, address);
        leadNode.data.addContact(contact);
        this.contactList.add(contact);

        console.log("Contact added successfully to the lead.");
    }

    async modifyLead() {
        const leadName = await this.askQuestion("Enter Lead Name to search: ");
        let flag = false;

        this.leadList.displayAll(lead => {
            if (lead.getName() === leadName) {
                lead.display();
                flag = true;
            }
        });

        if (!flag) {
            console.log(`No leads found with the name: ${leadName}`);
            return;
        }

        const leadId = parseInt(await this.askQuestion("Enter Lead ID to modify: "), 10);
        const leadNode = this.leadList.search(lead => lead.getId() === leadId);

        if (!leadNode) {
            console.log("Lead not found.");
            return;
        }

        const name = await this.askQuestion("Enter new Lead Name: ");
        const address = await this.askQuestion("Enter new Lead Address: ");
        const website = await this.askQuestion("Enter new Lead Website: ");

        leadNode.data.setName(name);
        leadNode.data.setAddress(address);
        leadNode.data.setWebsite(website);

        console.log("Lead updated successfully.");
    }

    async modifyContact() {
        const contactName = await this.askQuestion("Enter Contact Name to search: ");
        let flag = false;

        let leadNode = this.leadList.getHead();
        while (leadNode) {
            let contactNode = leadNode.data.getContactsHead();
            while (contactNode) {
                if (contactNode.data.getName() === contactName) {
                    contactNode.data.display();
                    leadNode.data.display();
                    flag = true;
                }
                contactNode = contactNode.next;
            }
            leadNode = leadNode.next;
        }

        if (!flag) {
            console.log(`No contacts found with the name: ${contactName}`);
        }

        const contactId = parseInt(await this.askQuestion("Enter Contact ID to modify: "), 10);
        leadNode = this.leadList.getHead();
        flag = false;

        while (leadNode) {
            let contactNode = leadNode.data.getContactsHead();
            while (contactNode) {
                if (contactNode.data.getId() === contactId) {
                    contactNode.data.display();

                    const name = await this.askQuestion("Enter new Contact Name: ");
                    const email = await this.askQuestion("Enter new Contact Email: ");
                    const address = await this.askQuestion("Enter new Contact Address: ");

                    contactNode.data.setName(name);
                    contactNode.data.setEmail(email);
                    contactNode.data.setAddress(address);

                    let tempContact = this.contactList.getHead();
                    while (tempContact) {
                        if (tempContact.data.getId() === contactId) {
                            tempContact.data.setName(name);
                            tempContact.data.setEmail(email);
                            tempContact.data.setAddress(address);
                            break;
                        }
                        tempContact = tempContact.next;
                    }

                    console.log("Contact updated successfully.");
                    flag = true;
                    break;
                }
                contactNode = contactNode.next;
            }
            if (flag) break;
            leadNode = leadNode.next;
        }
    }

    async searchLeadsByName() {
        const leadName = await this.askQuestion("Enter Lead Name to search: ");
        let flag = false;

        this.leadList.displayAll(lead => {
            if (lead.getName() === leadName) {
                lead.display();
                flag = true;
            }
        });

        if (!flag) {
            console.log(`No leads found with the name: ${leadName}`);
        }
    }

    async searchContactsByName() {
        const contactName = await this.askQuestion("Enter Contact Name to search: ");
        let flag = false;

        let leadNode = this.leadList.getHead();
        while (leadNode) {
            let contactNode = leadNode.data.getContactsHead();
            while (contactNode) {
                if (contactNode.data.getName() === contactName) {
                    contactNode.data.display();
                    leadNode.data.display();
                    flag = true;
                }
                contactNode = contactNode.next;
            }
            leadNode = leadNode.next;
        }

        if (!flag) {
            console.log(`No contacts found with the name: ${contactName}`);
        }
    }

    async displayAllLeadsAndContacts() {
        if (!this.leadList.getHead()) {
            console.log("No leads available.");
            return;
        }

        console.log("--- All Leads and Contacts ---");
        let leadNode = this.leadList.getHead();

        while (leadNode) {
            leadNode.data.display();
            console.log("  Contacts for this lead:");
            leadNode.data.displayContacts();

            leadNode = leadNode.next;
        }
    }

    async displayAllLeads() {
        if (!this.leadList.getHead()) {
            console.log("No leads available.");
            return;
        }

        console.log("--- All Leads ---");
        let leadNode = this.leadList.getHead();
        while (leadNode) {
            leadNode.data.display();
            leadNode = leadNode.next;
        }
    }

    async displayAllContacts() {
        this.contactList.displayAll(contact => contact.display());
    }
}

async function main() {
    const crm = new CRM();

    let running = true;
    while (running) {
        console.log("CRM Menu:");
        console.log("1. Add Lead");
        console.log("2. Add Contact to Lead");
        console.log("3. Modify Lead");
        console.log("4. Modify Contact");
        console.log("5. Search Leads by Name");
        console.log("6. Search Contacts by Name");
        console.log("7. Display All Leads");
        console.log("8. Display All Contacts");
        console.log("9. Display All Leads and Contacts");
        console.log("10. Exit");

        const choice = parseInt(await crm.askQuestion("Enter your choice: "), 10);

        switch (choice) {
            case 1:
                await crm.addLead();
                break;
            case 2:
                await crm.addContactToLead();
                break;
            case 3:
                await crm.modifyLead();
                break;
            case 4:
                await crm.modifyContact();
                break;
            case 5:
                await crm.searchLeadsByName();
                break;
            case 6:
                await crm.searchContactsByName();
                break;
            case 7:
                await crm.displayAllLeads();
                break;
            case 8:
                await crm.displayAllContacts();
                break;
            case 9:
                await crm.displayAllLeadsAndContacts();
                break;
            case 10:
                console.log("Exiting CRM.");
                running = false;
                rl.close();
                break;
            default:
                console.log("Invalid choice. Please try again.");
        }
    }
}

main();

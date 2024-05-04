#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Subscriber {
    string surname;
    string address;

    void serialize(ofstream& out) const {
        out << surname.size() << ' ' << surname << ' ';
        out << address.size() << ' ' << address << ' ';
    }

    void deserialize(ifstream& in) {
        size_t size;
        in >> size;
        in.ignore(); // Ignore the space
        surname.resize(size);
        in.read(&surname[0], size);
        in >> size;
        in.ignore(); // Ignore the space
        address.resize(size);
        in.read(&address[0], size);
    }
};

struct Phone {
    string number;
    string paymentType;
    double paymentAmount;

    void serialize(ofstream& out) const {
        out << number.size() << ' ' << number << ' ';
        out << paymentType.size() << ' ' << paymentType << ' ';
        out.write(reinterpret_cast<const char*>(&paymentAmount), sizeof(double));
    }

    void deserialize(ifstream& in) {
        size_t size;
        in >> size;
        in.ignore(); // Ignore the space
        number.resize(size);
        in.read(&number[0], size);
        in >> size;
        in.ignore(); // Ignore the space
        paymentType.resize(size);
        in.read(&paymentType[0], size);
        in.read(reinterpret_cast<char*>(&paymentAmount), sizeof(double));
    }
};

void addRecordToFile(const string& filename, const Subscriber& subscriber, const Phone& phone) {
    ofstream file(filename, ios::binary | ios::app);
    if (file.is_open()) {
        subscriber.serialize(file);
        phone.serialize(file);
        cout << "Record added successfully.\n";
    }
    else {
        cout << "Error: Unable to open file.\n";
    }
}

void readRecordsFromFile(const string& filename, vector<Subscriber>& subscribers, vector<Phone>& phones) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        while (file.peek() != EOF) {
            Subscriber subscriber;
            subscriber.deserialize(file);
            Phone phone;
            phone.deserialize(file);
            subscribers.push_back(subscriber);
            phones.push_back(phone);
        }
        file.close();
    }
    else {
        cout << "Error: Unable to open file.\n";
    }
}

void addByPhoneNumberPrefix(const string& filename, const string& prefix) {
    Subscriber subscriber;
    Phone phone;
    cout << "Enter subscriber's surname: ";
    cin >> subscriber.surname;
    cout << "Enter subscriber's address: ";
    cin >> subscriber.address;
    cout << "Enter phone number: ";
    cin >> phone.number;
    cout << "Enter payment type: ";
    cin >> phone.paymentType;
    cout << "Enter payment amount: ";
    cin >> phone.paymentAmount;

    if (phone.number.find(prefix) == 0) {
        addRecordToFile(filename, subscriber, phone);
        cout << "Record added successfully.\n";
    }
    else {
        cout << "Phone number doesn't match the specified prefix.\n";
    }
}

void displayBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        cout << "Contents of binary file:\n";
        char ch;
        while (file.get(ch)) {
            cout << ch;
        }
        file.close();
        cout << endl;
    }
    else {
        cout << "Error: Unable to open file.\n";
    }
}

void appendDataToFile(const string& filename, const Subscriber& subscriber, const Phone& phone) {
    addRecordToFile(filename, subscriber, phone);
}

void replaceDataInFile(const string& filename, int index, const Subscriber& subscriber, const Phone& phone) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (file.is_open()) {
        file.seekp(index * (sizeof(Subscriber) + sizeof(Phone)), ios::beg);
        file.write(reinterpret_cast<const char*>(&subscriber), sizeof(Subscriber));
        file.write(reinterpret_cast<const char*>(&phone), sizeof(Phone));
        file.close();
        cout << "Data has been successfully replaced.\n";
    }
    else {
        cout << "Error: Unable to open file.\n";
    }
}

void deleteDataFromFile(const string& filename, int index) {
    vector<Subscriber> subscribers;
    vector<Phone> phones;
    readRecordsFromFile(filename, subscribers, phones);

    if (index < 0 || index >= subscribers.size()) {
        cout << "Invalid index.\n";
        return;
    }

    subscribers.erase(subscribers.begin() + index);
    phones.erase(phones.begin() + index);

    ofstream file(filename, ios::binary | ios::trunc);
    if (file.is_open()) {
        for (size_t i = 0; i < subscribers.size(); ++i) {
            file.write(reinterpret_cast<const char*>(&subscribers[i]), sizeof(Subscriber));
            file.write(reinterpret_cast<const char*>(&phones[i]), sizeof(Phone));
        }
        file.close();
        cout << "Data deleted successfully.\n";
    }
    else {
        cout << "Error: Unable to open file for writing.\n";
    }
}

void listSubscribersByPayment(const vector<Subscriber>& subscribers, const vector<Phone>& phones, double maxAmount) {
    cout << "Subscribers with a charge per minute below $" << maxAmount << ":\n";
    for (size_t i = 0; i < phones.size(); ++i) {
        if (phones[i].paymentType == "hv" && phones[i].paymentAmount < maxAmount) {
            cout << "Last name: " << subscribers[i].surname << ", Address: " << subscribers[i].address << ", Phone: " << phones[i].number << endl;
        }
    }
}

string findPhoneNumberByAddress(const vector<Subscriber>& subscribers, const vector<Phone>& phones, const string& address) {
    for (size_t i = 0; i < subscribers.size(); ++i) {
        if (subscribers[i].address == address) {
            return phones[i].number;
        }
    }
    return "Phone number not found for the given address.";
}

void displayMenu() {
    cout << "Menu:\n"
        << "1. Add record to file\n"
        << "2. Read records from file\n"
        << "3. Add subscriber records based on phone number prefix\n"
        << "4. List subscribers with minute-based payment below a certain amount\n"
        << "5. Find phone number by address\n"
        << "6. Append data to file\n"
        << "7. Replace data in file\n"
        << "8. Delete data from file\n"
        << "9. Exit\n";
}

int main() {
    vector<Subscriber> subscribers;
    vector<Phone> phones;
    string filename;

    cout << "Enter filename: ";
    cin >> filename;

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Subscriber subscriber;
            Phone phone;

            cout << "Enter subscriber's surname: ";
            cin >> subscriber.surname;
            cout << "Enter subscriber's address: ";
            cin >> subscriber.address;
            cout << "Enter phone number: ";
            cin >> phone.number;
            cout << "Enter payment type: ";
            cin >> phone.paymentType;
            cout << "Enter payment amount: ";
            cin >> phone.paymentAmount;

            addRecordToFile(filename, subscriber, phone);
            break;
        }
        case 2: {
            cout << "Enter the binary file name: ";
            cin >> filename;


            displayBinaryFile(filename);

            return 0;
        }



        case 3: {
            string prefix;
            cout << "Enter phone number prefix: ";
            cin >> prefix;
            addByPhoneNumberPrefix(filename, prefix);
            break;
        }
        case 4: {

            double maxAmount;
            cout << "Enter maximum payment amount: $";
            cin >> maxAmount;
            listSubscribersByPayment(subscribers, phones, maxAmount);
            break;
        }
        case 5: {
            string address;
            cout << "Enter address to search: ";
            cin >> address;
            cout << "Phone number: " << findPhoneNumberByAddress(subscribers, phones, address) << endl;
            break;
        }
        case 6: {
            Subscriber subscriber;
            Phone phone;

            cout << "Enter subscriber's surname: ";
            cin >> subscriber.surname;
            cout << "Enter subscriber's address: ";
            cin >> subscriber.address;
            cout << "Enter phone number: ";
            cin >> phone.number;
            cout << "Enter payment type: ";
            cin >> phone.paymentType;
            cout << "Enter payment amount: ";
            cin >> phone.paymentAmount;

            appendDataToFile(filename, subscriber, phone);
            break;
        }
        case 7: {
            int index;
            cout << "Enter index to replace: ";
            cin >> index;

            Subscriber subscriber;
            Phone phone;

            cout << "Enter subscriber's surname: ";
            cin >> subscriber.surname;
            cout << "Enter subscriber's address: ";
            cin >> subscriber.address;
            cout << "Enter phone number: ";
            cin >> phone.number;
            cout << "Enter payment type: ";
            cin >> phone.paymentType;
            cout << "Enter payment amount: ";
            cin >> phone.paymentAmount;

            replaceDataInFile(filename, index, subscriber, phone);
            break;
        }
        case 8: {
            int index;
            cout << "Enter index to delete: ";
            cin >> index;

            deleteDataFromFile(filename, index);
            break;
        }
        case 9:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 9);

    return 0;
}
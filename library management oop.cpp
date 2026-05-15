#include <iostream>
#include <string>
using namespace std;

class LibraryMember;

//  BASE CLASS 
class LibraryResource {
protected:
    int resourceID;
    string title;
    string author;
    bool isAvailable;

public:
    LibraryResource(int id = 0, string t = "", string a = "") {
        this->resourceID = id;     // 'this' pointer refers to current object
        this->title = t;
        this->author = a;
        this->isAvailable = true;  // current object initialized
    }

    int getID() {
        return resourceID;
    }

    bool getAvailability() {
        return isAvailable;
    }

    void setAvailability(bool status) {
        this->isAvailable = status; // 'this' pointer  used
    }

    void displayDetails() {
        cout << "ID: " << resourceID
            << " | Title: " << title
            << " | Author: " << author
            << " | Status: "
            << (isAvailable ? "Available" : "Borrowed");
    }

    // function hiding 
    double calculateLateFee(int daysLate) {
        return 0;
    }

    // Friend function declaration 
    friend void adminView(LibraryResource[], int, LibraryMember[], int);
};

//  DERIVED CLASSES 
class Book : public LibraryResource {
public:
    Book(int id = 0, string t = "", string a = "")
        : LibraryResource(id, t, a) {
    }

    double calculateLateFee(int daysLate) {
        return daysLate * 5;
    }
};

class Magazine : public LibraryResource {
public:
    Magazine(int id = 0, string t = "", string a = "")
        : LibraryResource(id, t, a) {
    }

    double calculateLateFee(int daysLate) {
        return daysLate * 3;
    }
};

class DVD : public LibraryResource {
public:
    DVD(int id = 0, string t = "", string a = "")
        : LibraryResource(id, t, a) {
    }

    double calculateLateFee(int daysLate) {
        return daysLate * 10;
    }
};

//  MEMBER CLASS (COMPOSITION) 
class LibraryMember {
private:
    int memberID;
    string name;

    LibraryResource borrowedItems[5];
    int borrowedCount;

public:
    LibraryMember(int id = 0, string n = "") {
        this->memberID = id;
        this->name = n;
        this->borrowedCount = 0;   // 'this' pointer used
    }

    int getMemberID() {
        return memberID;
    }

    // BORROW RESOURCE
    void borrowResource(LibraryResource resources[], int rIndex) {
        if (borrowedCount < 5 && resources[rIndex].getAvailability()) {

            borrowedItems[borrowedCount++] = resources[rIndex]; // composition copy
            resources[rIndex].setAvailability(false);

            cout << "Borrowed successfully\n";
        }
        else {
            cout << "Cannot borrow\n";
        }
    }

    // RETURN RESOURCE
    void returnResource(LibraryResource resources[], int rID) {

        for (int i = 0; i < borrowedCount; i++) {

            if (borrowedItems[i].getID() == rID) {

                // make resource available again
                for (int j = 0; j < 10; j++) {
                    if (resources[j].getID() == rID) {
                        resources[j].setAvailability(true);
                        break;
                    }
                }

                // shift array
                for (int j = i; j < borrowedCount - 1; j++) {
                    borrowedItems[j] = borrowedItems[j + 1];
                }

                borrowedCount--;

                cout << "Returned successfully\n";
                return;
            }
        }

        cout << "Not found\n";
    }

    // DISPLAY BORROWED ITEMS
    void displayBorrowed() {
        cout << "\nMember: " << name << endl;

        if (borrowedCount == 0) {
            cout << "No items borrowed\n";
            return;
        }

        for (int i = 0; i < borrowedCount; i++) {
            borrowedItems[i].displayDetails();
            cout << endl;
        }
    }

    // CALCULATE FEE (function hiding from derived classes)
    double calculateTotalLateFee(int daysLate) {
        double total = 0;

        for (int i = 0; i < borrowedCount; i++) {
            total += borrowedItems[i].calculateLateFee(daysLate);
        }

        return total;
    }

    // Friend function
    friend void adminView(LibraryResource[], int, LibraryMember[], int);
};

//  FRIEND FUNCTION 
void adminView(LibraryResource resources[], int rCount,
    LibraryMember members[], int mCount) {

    cout << "\n===== ALL RESOURCES =====\n";
    for (int i = 0; i < rCount; i++) {
        resources[i].displayDetails();
        cout << endl;
    }

    cout << "\n===== ALL MEMBERS =====\n";
    for (int i = 0; i < mCount; i++) {
        members[i].displayBorrowed();
    }
}

int main() {

    LibraryResource resources[10];
    LibraryMember members[10];

    int rCount = 0, mCount = 0, choice;

    while (true) {

        cout << "\n1.Add Resource\n2.Add Member\n3.Borrow\n4.Return\n5.Fee\n6.Admin\n7.Exit\nChoice: ";
        cin >> choice;

        if (choice == 7) break;

        // ADD RESOURCE
        if (choice == 1) {

            int id, type;
            string t, a;

            cout << "ID: "; cin >> id;
            cin.ignore();
            cout << "Title: "; getline(cin, t);
            cout << "Author: "; getline(cin, a);

            cout << "1.Book 2.Magazine 3.DVD: ";
            cin >> type;

            if (type == 1)
                resources[rCount] = Book(id, t, a);
            else if (type == 2)
                resources[rCount] = Magazine(id, t, a);
            else
                resources[rCount] = DVD(id, t, a);

            rCount++;
        }

        // ADD MEMBER
        else if (choice == 2) {

            int id;
            string name;

            cout << "ID: "; cin >> id;
            cin.ignore();
            cout << "Name: "; getline(cin, name);

            members[mCount++] = LibraryMember(id, name);
        }

        // BORROW
        else if (choice == 3) {

            int mid, rid;
            cout << "Member ID: "; cin >> mid;
            cout << "Resource ID: "; cin >> rid;

            int m = -1, r = -1;

            for (int i = 0; i < mCount; i++)
                if (members[i].getMemberID() == mid) m = i;

            for (int i = 0; i < rCount; i++)
                if (resources[i].getID() == rid) r = i;

            if (m != -1 && r != -1)
                members[m].borrowResource(resources, r);
            else
                cout << "Not found\n";
        }

        // RETURN
        else if (choice == 4) {

            int mid, rid;
            cout << "Member ID: "; cin >> mid;
            cout << "Resource ID: "; cin >> rid;

            for (int i = 0; i < mCount; i++)
                if (members[i].getMemberID() == mid)
                    members[i].returnResource(resources, rid);
        }

        // FEE
        else if (choice == 5) {

            int mid, days;
            cout << "Member ID: "; cin >> mid;
            cout << "Days Late: "; cin >> days;

            for (int i = 0; i < mCount; i++)
                if (members[i].getMemberID() == mid)
                    cout << "Total Fee: "
                    << members[i].calculateTotalLateFee(days)
                    << endl;
        }

        // ADMIN VIEW
        else if (choice == 6) {
            adminView(resources, rCount, members, mCount);
        }
    }

    return 0;
}
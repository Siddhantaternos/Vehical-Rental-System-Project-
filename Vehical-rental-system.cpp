#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

/* =======================
   BASE CLASS: Vehicle
   ======================= */
class Vehicle {
protected:
    int vehicleId;
    string brand;
    string model;
    float rentPerDay;
    bool isAvailable;

public:
    Vehicle(int id, const string& b, const string& m, float rent, bool avail)
        : vehicleId(id), brand(b), model(m), rentPerDay(rent), isAvailable(avail) {}

    virtual ~Vehicle() {}

    // --- Core behavior ---
    virtual float calculateRent(int days) const {
        return rentPerDay * days;
    }

    // --- Polymorphic helpers ---
    virtual string getType() const = 0;
    virtual void printExtra() const = 0;
    virtual void saveToFile(ofstream& out) const = 0;

    // --- Common display ---
    virtual void display() const {
        cout << left
             << setw(5)  << vehicleId
             << setw(15) << brand
             << setw(15) << model
             << setw(10) << rentPerDay
             << setw(12) << (isAvailable ? "Available" : "Rented")
             << setw(8)  << getType();
        printExtra();
        cout << endl;
    }

    // --- Getters ---
    int getId() const { return vehicleId; }
    bool available() const { return isAvailable; }
    void setAvailability(bool status) { isAvailable = status; }
};

/* =======================
   DERIVED CLASS: Car
   ======================= */
class Car : public Vehicle {
    int seats;

public:
    Car(int id, const string& b, const string& m, float rent, bool avail, int s)
        : Vehicle(id, b, m, rent, avail), seats(s) {}

    string getType() const override {
        return "CAR";
    }

    void printExtra() const override {
        cout << "Seats: " << seats;
    }

    void saveToFile(ofstream& out) const override {
        out << vehicleId << " CAR "
            << brand << " "
            << model << " "
            << rentPerDay << " "
            << isAvailable << " "
            << seats << endl;
    }
};

/* =======================
   DERIVED CLASS: Bike
   ======================= */
class Bike : public Vehicle {
    int engineCC;

public:
    Bike(int id, const string& b, const string& m, float rent, bool avail, int cc)
        : Vehicle(id, b, m, rent, avail), engineCC(cc) {}

    string getType() const override {
        return "BIKE";
    }

    void printExtra() const override {
        cout << "CC: " << engineCC;
    }

    void saveToFile(ofstream& out) const override {
        out << vehicleId << " BIKE "
            << brand << " "
            << model << " "
            << rentPerDay << " "
            << isAvailable << " "
            << engineCC << endl;
    }
};

/* =======================
   CONTROLLER: RentalSystem
   ======================= */
class RentalSystem {
    vector<Vehicle*> vehicles;
    int nextId;

public:
    RentalSystem() : nextId(1) {
        loadFromFile();
    }

    ~RentalSystem() {
        for (auto v : vehicles)
            delete v;
    }

    /* ---------- ADD VEHICLE ---------- */
    void addVehicle() {
        int choice;
        cout << "1. Car\n2. Bike\nChoose: ";
        cin >> choice;

        if (cin.fail() || (choice != 1 && choice != 2)) {
            resetInput();
            cout << "Invalid choice.\n";
            return;
        }

        string brand, model;
        float rent;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Brand: ";
        getline(cin, brand);

        cout << "Model: ";
        getline(cin, model);

        cout << "Rent per day: ";
        cin >> rent;

        if (cin.fail() || rent <= 0) {
            resetInput();
            cout << "Invalid rent.\n";
            return;
        }

        if (choice == 1) {
            int seats;
            cout << "Seats: ";
            cin >> seats;

            if (cin.fail() || seats <= 0) {
                resetInput();
                cout << "Invalid seats.\n";
                return;
            }

            vehicles.push_back(new Car(nextId++, brand, model, rent, true, seats));
        } else {
            int cc;
            cout << "Engine CC: ";
            cin >> cc;

            if (cin.fail() || cc <= 0) {
                resetInput();
                cout << "Invalid engine CC.\n";
                return;
            }

            vehicles.push_back(new Bike(nextId++, brand, model, rent, true, cc));
        }

        saveToFile();
        cout << "Vehicle added successfully.\n";
    }

    /* ---------- DISPLAY VEHICLES ---------- */
    void showVehicles() const {
        cout << left
             << setw(5)  << "ID"
             << setw(15) << "Brand"
             << setw(15) << "Model"
             << setw(10) << "Rent"
             << setw(12) << "Status"
             << setw(8)  << "Type"
             << "Extra\n";

        cout << string(80, '-') << endl;

        for (auto v : vehicles)
            v->display();
    }

    /* ---------- RENT VEHICLE ---------- */
    void rentVehicle() {
        int id, days;
        cout << "Vehicle ID: ";
        cin >> id;

        for (auto v : vehicles) {
            if (v->getId() == id && v->available()) {
                cout << "Days: ";
                cin >> days;

                if (cin.fail() || days <= 0) {
                    resetInput();
                    cout << "Invalid days.\n";
                    return;
                }

                cout << "Total Rent: " << v->calculateRent(days) << endl;
                v->setAvailability(false);
                saveToFile();
                return;
            }
        }
        cout << "Vehicle not available.\n";
    }

    /* ---------- RETURN VEHICLE ---------- */
    void returnVehicle() {
        int id;
        cout << "Vehicle ID: ";
        cin >> id;

        for (auto v : vehicles) {
            if (v->getId() == id && !v->available()) {
                v->setAvailability(true);
                saveToFile();
                cout << "Vehicle returned.\n";
                return;
            }
        }
        cout << "Invalid return.\n";
    }

private:
    /* ---------- FILE HANDLING ---------- */
    void saveToFile() const {
        ofstream out("vehicles.txt");
        for (auto v : vehicles)
            v->saveToFile(out);
    }

    void loadFromFile() {
        cout << "Loading vehicles from file...\n";
        ifstream in("vehicles.txt");
        if (!in) return;

        int id, extra;
        string type, brand, model;
        float rent;
        bool avail;

        while (in >> id >> type >> brand >> model >> rent >> avail >> extra) {
            cout << "Loaded ID: " << id << endl;
            if (type == "CAR")
                vehicles.push_back(new Car(id, brand, model, rent, avail, extra));
            else
                vehicles.push_back(new Bike(id, brand, model, rent, avail, extra));

            nextId = max(nextId, id + 1);
        }
    }

    void resetInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

/* =======================
   MAIN
   ======================= */
int main() {
    RentalSystem system;
    int choice;

    while (true) {
        cout << "\n1.Add Vehicle\n2.Show Vehicles\n3.Rent Vehicle\n4.Return Vehicle\n5.Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.addVehicle(); break;
            case 2: system.showVehicles(); break;
            case 3: system.rentVehicle(); break;
            case 4: system.returnVehicle(); break;
            case 5: return 0;
            default: cout << "Invalid option.\n";
        }
    }
}

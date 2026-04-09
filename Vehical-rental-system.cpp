/*
 * ============================================================
 *   VEHICLE RENTAL SYSTEM — DBMS Concepts Demo in C++
 * ============================================================
 *
 * DBMS Concepts Demonstrated:
 *  1. DDL Commands      — CREATE TABLE, ALTER TABLE, DROP TABLE (simulated)
 *  2. Constraints       — NOT NULL, UNIQUE, CHECK, DEFAULT, FOREIGN KEY
 *  3. Stored Procedure  — Procedure: RentVehicle(), ReturnVehicle()
 *  4. Stored Function   — Function: CalculateFine()
 *  5. JOIN Operations   — INNER JOIN, LEFT JOIN (simulated with C++ vectors)
 *  6. TCL Commands      — BEGIN TRANSACTION, COMMIT, ROLLBACK (simulated)
 *
 * Author : [Your Name]
 * Subject: DBMS Practical Evaluation
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include <stdexcept>

using namespace std;

// ============================================================
// SECTION 1: CONSTRAINT HELPERS (Simulating SQL Constraints)
// ============================================================

/*
 * DDL Equivalent:
 * CREATE TABLE vehicles (
 *   vehicle_id   INT          PRIMARY KEY NOT NULL,
 *   brand        VARCHAR(50)  NOT NULL,
 *   model        VARCHAR(50)  NOT NULL,
 *   type         VARCHAR(10)  CHECK(type IN ('CAR','BIKE')) NOT NULL,
 *   rent_per_day FLOAT        CHECK(rent_per_day > 0) NOT NULL,
 *   extra_info   INT          NOT NULL,
 *   is_available BOOLEAN      DEFAULT TRUE NOT NULL
 * );
 *
 * CREATE TABLE customers (
 *   customer_id  INT          PRIMARY KEY NOT NULL,
 *   name         VARCHAR(100) NOT NULL,
 *   phone        VARCHAR(15)  UNIQUE NOT NULL
 * );
 *
 * CREATE TABLE rentals (
 *   rental_id    INT          PRIMARY KEY NOT NULL,
 *   customer_id  INT          NOT NULL,   -- FOREIGN KEY → customers(customer_id)
 *   vehicle_id   INT          NOT NULL,   -- FOREIGN KEY → vehicles(vehicle_id)
 *   days         INT          CHECK(days > 0) NOT NULL,
 *   total_rent   FLOAT        NOT NULL,
 *   is_returned  BOOLEAN      DEFAULT FALSE NOT NULL
 * );
 */

// Constraint violation exception
class ConstraintViolation : public runtime_error {
public:
    ConstraintViolation(const string& msg) : runtime_error(msg) {}
};

// ============================================================
// SECTION 2: TABLE ROW STRUCTURES
// ============================================================

struct VehicleRow {
    int    vehicle_id;           // PRIMARY KEY, NOT NULL
    string brand;                // NOT NULL
    string model;                // NOT NULL
    string type;                 // CHECK(type IN ('CAR','BIKE')), NOT NULL
    float  rent_per_day;         // CHECK(rent_per_day > 0), NOT NULL
    int    extra_info;           // NOT NULL (seats for CAR, cc for BIKE)
    bool   is_available = true;  // DEFAULT TRUE
};

struct CustomerRow {
    int    customer_id;   // PRIMARY KEY, NOT NULL
    string name;          // NOT NULL
    string phone;         // UNIQUE, NOT NULL
};

struct RentalRow {
    int   rental_id;              // PRIMARY KEY, NOT NULL
    int   customer_id;            // FOREIGN KEY → customers, NOT NULL
    int   vehicle_id;             // FOREIGN KEY → vehicles, NOT NULL
    int   days;                   // CHECK(days > 0), NOT NULL
    float total_rent;             // NOT NULL
    bool  is_returned = false;    // DEFAULT FALSE
};

// ============================================================
// SECTION 3: TABLE CLASSES (DDL Simulated)
// ============================================================

class VehicleTable {
public:
    vector<VehicleRow> rows;
    int next_id = 1;

    // DDL: CREATE TABLE vehicles (...)
    VehicleTable() { cout << "[DDL] CREATE TABLE vehicles — OK\n"; }

    // DDL: DROP TABLE simulation
    void dropTable() {
        rows.clear();
        cout << "[DDL] DROP TABLE vehicles — OK (all data cleared)\n";
    }

    // Constraint checks before INSERT
    void validateRow(const VehicleRow& r) {
        // NOT NULL checks
        if (r.brand.empty())  throw ConstraintViolation("NOT NULL violated: brand");
        if (r.model.empty())  throw ConstraintViolation("NOT NULL violated: model");
        if (r.type.empty())   throw ConstraintViolation("NOT NULL violated: type");

        // CHECK constraint: type must be CAR or BIKE
        if (r.type != "CAR" && r.type != "BIKE")
            throw ConstraintViolation("CHECK violated: type must be 'CAR' or 'BIKE'");

        // CHECK constraint: rent_per_day > 0
        if (r.rent_per_day <= 0)
            throw ConstraintViolation("CHECK violated: rent_per_day must be > 0");

        // UNIQUE on (brand + model + type) — no two identical vehicles
        for (auto& row : rows) {
            if (row.brand == r.brand && row.model == r.model && row.type == r.type)
                throw ConstraintViolation("UNIQUE violated: vehicle already exists");
        }
    }

    VehicleRow* findById(int id) {
        for (auto& r : rows)
            if (r.vehicle_id == id) return &r;
        return nullptr;
    }

    void insert(VehicleRow r) {
        r.vehicle_id = next_id++;
        validateRow(r);
        rows.push_back(r);
    }
};

class CustomerTable {
public:
    vector<CustomerRow> rows;
    int next_id = 1;

    CustomerTable() { cout << "[DDL] CREATE TABLE customers — OK\n"; }

    void validateRow(const CustomerRow& r) {
        if (r.name.empty())  throw ConstraintViolation("NOT NULL violated: name");
        if (r.phone.empty()) throw ConstraintViolation("NOT NULL violated: phone");

        // UNIQUE constraint on phone
        for (auto& row : rows)
            if (row.phone == r.phone)
                throw ConstraintViolation("UNIQUE violated: phone already registered");
    }

    CustomerRow* findById(int id) {
        for (auto& r : rows)
            if (r.customer_id == id) return &r;
        return nullptr;
    }

    void insert(CustomerRow r) {
        r.customer_id = next_id++;
        validateRow(r);
        rows.push_back(r);
    }
};

class RentalTable {
public:
    vector<RentalRow> rows;
    int next_id = 1;

    RentalTable() { cout << "[DDL] CREATE TABLE rentals — OK\n"; }

    void validateRow(const RentalRow& r, CustomerTable& ct, VehicleTable& vt) {
        // FOREIGN KEY: customer_id must exist in customers
        if (!ct.findById(r.customer_id))
            throw ConstraintViolation("FOREIGN KEY violated: customer_id not found");

        // FOREIGN KEY: vehicle_id must exist in vehicles
        if (!vt.findById(r.vehicle_id))
            throw ConstraintViolation("FOREIGN KEY violated: vehicle_id not found");

        // CHECK: days > 0
        if (r.days <= 0)
            throw ConstraintViolation("CHECK violated: days must be > 0");
    }

    RentalRow* findById(int id) {
        for (auto& r : rows)
            if (r.rental_id == id) return &r;
        return nullptr;
    }

    void insert(RentalRow r, CustomerTable& ct, VehicleTable& vt) {
        r.rental_id = next_id++;
        validateRow(r, ct, vt);
        rows.push_back(r);
    }
};

// ============================================================
// SECTION 4: TCL — TRANSACTION MANAGER
// ============================================================

class Transaction {
    bool active = false;

    // Snapshots for ROLLBACK
    vector<VehicleRow>  vehicle_snapshot;
    vector<CustomerRow> customer_snapshot;
    vector<RentalRow>   rental_snapshot;

public:
    void begin(VehicleTable& vt, CustomerTable& ct, RentalTable& rt) {
        vehicle_snapshot  = vt.rows;
        customer_snapshot = ct.rows;
        rental_snapshot   = rt.rows;
        active = true;
        cout << "[TCL] BEGIN TRANSACTION\n";
    }

    void commit() {
        if (!active) { cout << "[TCL] No active transaction.\n"; return; }
        active = false;
        cout << "[TCL] COMMIT — changes saved permanently.\n";
    }

    void rollback(VehicleTable& vt, CustomerTable& ct, RentalTable& rt) {
        if (!active) { cout << "[TCL] No active transaction.\n"; return; }
        vt.rows = vehicle_snapshot;
        ct.rows = customer_snapshot;
        rt.rows = rental_snapshot;
        active = false;
        cout << "[TCL] ROLLBACK — all changes since BEGIN have been undone.\n";
    }

    bool isActive() const { return active; }
};

// ============================================================
// SECTION 5: STORED FUNCTION — CalculateFine()
// ============================================================

/*
 * SQL Equivalent:
 * CREATE FUNCTION CalculateFine(days_late INT, rent_per_day FLOAT)
 * RETURNS FLOAT
 * BEGIN
 *   RETURN days_late * rent_per_day * 1.5;
 * END;
 */
float CalculateFine(int days_late, float rent_per_day) {
    if (days_late <= 0) return 0.0f;
    return days_late * rent_per_day * 1.5f;  // 1.5x penalty rate
}

// ============================================================
// SECTION 6: STORED PROCEDURES
// ============================================================

/*
 * SQL Equivalent:
 * CREATE PROCEDURE RentVehicle(IN cust_id INT, IN veh_id INT, IN days INT)
 * BEGIN
 *   DECLARE total FLOAT;
 *   IF (SELECT is_available FROM vehicles WHERE vehicle_id = veh_id) THEN
 *     SET total = days * (SELECT rent_per_day FROM vehicles WHERE vehicle_id = veh_id);
 *     INSERT INTO rentals VALUES (...);
 *     UPDATE vehicles SET is_available = FALSE WHERE vehicle_id = veh_id;
 *     COMMIT;
 *   END IF;
 * END;
 */
void ProcRentVehicle(int cust_id, int veh_id, int days,
                     VehicleTable& vt, CustomerTable& ct, RentalTable& rt,
                     Transaction& txn) {
    cout << "\n[PROCEDURE] CALL RentVehicle(" << cust_id << ", " << veh_id << ", " << days << ")\n";

    txn.begin(vt, ct, rt);

    try {
        VehicleRow* v = vt.findById(veh_id);
        if (!v)               throw runtime_error("Vehicle not found.");
        if (!v->is_available) throw runtime_error("Vehicle is already rented.");

        float total = v->rent_per_day * days;

        RentalRow r;
        r.customer_id = cust_id;
        r.vehicle_id  = veh_id;
        r.days        = days;
        r.total_rent  = total;

        rt.insert(r, ct, vt);
        v->is_available = false;

        cout << "  Rental created. Total Rent = Rs." << fixed << setprecision(2) << total << "\n";
        txn.commit();
    } catch (exception& e) {
        cout << "  ERROR: " << e.what() << "\n";
        txn.rollback(vt, ct, rt);
    }
}

/*
 * SQL Equivalent:
 * CREATE PROCEDURE ReturnVehicle(IN rental_id INT, IN extra_days INT)
 * BEGIN
 *   DECLARE fine FLOAT DEFAULT 0;
 *   UPDATE rentals SET is_returned = TRUE WHERE rental_id = rental_id;
 *   UPDATE vehicles SET is_available = TRUE WHERE vehicle_id = ...;
 *   IF extra_days > 0 THEN
 *     SET fine = CalculateFine(extra_days, rent_per_day);
 *   END IF;
 *   COMMIT;
 * END;
 */
void ProcReturnVehicle(int rental_id, int extra_days,
                       VehicleTable& vt, CustomerTable& ct, RentalTable& rt,
                       Transaction& txn) {
    cout << "\n[PROCEDURE] CALL ReturnVehicle(" << rental_id << ", " << extra_days << ")\n";

    txn.begin(vt, ct, rt);

    try {
        RentalRow* r = rt.findById(rental_id);
        if (!r)           throw runtime_error("Rental record not found.");
        if (r->is_returned) throw runtime_error("Vehicle already returned.");

        VehicleRow* v = vt.findById(r->vehicle_id);
        if (!v)           throw runtime_error("Vehicle data corrupted.");

        r->is_returned   = true;
        v->is_available  = true;

        float fine = CalculateFine(extra_days, v->rent_per_day);

        cout << "  Vehicle returned successfully.\n";
        if (fine > 0)
            cout << "  Late return fine (via CalculateFine()): Rs." << fixed << setprecision(2) << fine << "\n";

        txn.commit();
    } catch (exception& e) {
        cout << "  ERROR: " << e.what() << "\n";
        txn.rollback(vt, ct, rt);
    }
}

// ============================================================
// SECTION 7: JOIN OPERATIONS
// ============================================================

/*
 * INNER JOIN — rentals INNER JOIN customers INNER JOIN vehicles
 * SQL:
 * SELECT r.rental_id, c.name, v.brand, v.model, r.days, r.total_rent, r.is_returned
 * FROM rentals r
 * INNER JOIN customers c ON r.customer_id = c.customer_id
 * INNER JOIN vehicles  v ON r.vehicle_id  = v.vehicle_id;
 */
void InnerJoinRentals(RentalTable& rt, CustomerTable& ct, VehicleTable& vt) {
    cout << "\n[JOIN] INNER JOIN: rentals ⟕ customers ⟕ vehicles\n";
    cout << string(95, '-') << "\n";
    cout << left
         << setw(10) << "RentalID"
         << setw(20) << "Customer"
         << setw(15) << "Brand"
         << setw(15) << "Model"
         << setw(8)  << "Days"
         << setw(12) << "Total(Rs.)"
         << setw(12) << "Status"
         << "\n";
    cout << string(95, '-') << "\n";

    bool found = false;
    for (auto& r : rt.rows) {
        CustomerRow* c = ct.findById(r.customer_id);
        VehicleRow*  v = vt.findById(r.vehicle_id);
        if (c && v) {  // INNER JOIN: only matching rows
            cout << left
                 << setw(10) << r.rental_id
                 << setw(20) << c->name
                 << setw(15) << v->brand
                 << setw(15) << v->model
                 << setw(8)  << r.days
                 << setw(12) << fixed << setprecision(2) << r.total_rent
                 << setw(12) << (r.is_returned ? "Returned" : "Active")
                 << "\n";
            found = true;
        }
    }
    if (!found) cout << "  (no rental records)\n";
}

/*
 * LEFT JOIN — vehicles LEFT JOIN rentals
 * SQL:
 * SELECT v.vehicle_id, v.brand, v.model, r.rental_id, r.days
 * FROM vehicles v
 * LEFT JOIN rentals r ON v.vehicle_id = r.vehicle_id;
 * (Shows ALL vehicles, even those never rented)
 */
void LeftJoinVehiclesRentals(VehicleTable& vt, RentalTable& rt) {
    cout << "\n[JOIN] LEFT JOIN: vehicles ⟕ rentals (all vehicles, rentals if any)\n";
    cout << string(80, '-') << "\n";
    cout << left
         << setw(10) << "VehicleID"
         << setw(15) << "Brand"
         << setw(15) << "Model"
         << setw(12) << "RentalID"
         << setw(10) << "Days"
         << setw(15) << "Status"
         << "\n";
    cout << string(80, '-') << "\n";

    for (auto& v : vt.rows) {
        bool rented = false;
        for (auto& r : rt.rows) {
            if (r.vehicle_id == v.vehicle_id) {
                cout << left
                     << setw(10) << v.vehicle_id
                     << setw(15) << v.brand
                     << setw(15) << v.model
                     << setw(12) << r.rental_id
                     << setw(10) << r.days
                     << setw(15) << (r.is_returned ? "Returned" : "Active")
                     << "\n";
                rented = true;
            }
        }
        if (!rented) {  // LEFT JOIN includes vehicles with no rentals (NULL equivalent)
            cout << left
                 << setw(10) << v.vehicle_id
                 << setw(15) << v.brand
                 << setw(15) << v.model
                 << setw(12) << "NULL"
                 << setw(10) << "NULL"
                 << setw(15) << "Never Rented"
                 << "\n";
        }
    }
}

// ============================================================
// SECTION 8: DISPLAY HELPERS
// ============================================================

void showVehicles(VehicleTable& vt) {
    cout << "\n--- VEHICLES TABLE ---\n";
    cout << left
         << setw(5)  << "ID"
         << setw(15) << "Brand"
         << setw(15) << "Model"
         << setw(8)  << "Type"
         << setw(12) << "Rent/Day"
         << setw(12) << "Status"
         << "Extra\n";
    cout << string(80, '-') << "\n";
    for (auto& v : vt.rows) {
        cout << left
             << setw(5)  << v.vehicle_id
             << setw(15) << v.brand
             << setw(15) << v.model
             << setw(8)  << v.type
             << setw(12) << fixed << setprecision(2) << v.rent_per_day
             << setw(12) << (v.is_available ? "Available" : "Rented");
        if (v.type == "CAR") cout << "Seats: " << v.extra_info;
        else                  cout << "CC: "    << v.extra_info;
        cout << "\n";
    }
}

void showCustomers(CustomerTable& ct) {
    cout << "\n--- CUSTOMERS TABLE ---\n";
    cout << left << setw(5) << "ID" << setw(25) << "Name" << "Phone\n";
    cout << string(45, '-') << "\n";
    for (auto& c : ct.rows)
        cout << left << setw(5) << c.customer_id << setw(25) << c.name << c.phone << "\n";
}

// ============================================================
// SECTION 9: FILE PERSISTENCE
// ============================================================

void saveToFile(VehicleTable& vt, CustomerTable& ct, RentalTable& rt) {
    ofstream out("vehicles.txt");
    for (auto& v : vt.rows)
        out << v.vehicle_id << " " << v.type << " " << v.brand << " "
            << v.model << " " << v.rent_per_day << " " << v.is_available << " " << v.extra_info << "\n";

    ofstream cout2("customers.txt");
    for (auto& c : ct.rows)
        cout2 << c.customer_id << " " << c.name << " " << c.phone << "\n";

    ofstream cout3("rentals.txt");
    for (auto& r : rt.rows)
        cout3 << r.rental_id << " " << r.customer_id << " " << r.vehicle_id << " "
              << r.days << " " << r.total_rent << " " << r.is_returned << "\n";

    cout << "[FILE] Data saved to vehicles.txt, customers.txt, rentals.txt\n";
}

void loadFromFile(VehicleTable& vt, CustomerTable& ct, RentalTable& rt) {
    ifstream vin("vehicles.txt");
    if (vin) {
        int id, extra; string type, brand, model; float rent; bool avail;
        while (vin >> id >> type >> brand >> model >> rent >> avail >> extra) {
            VehicleRow v;
            v.vehicle_id = id; v.type = type; v.brand = brand; v.model = model;
            v.rent_per_day = rent; v.is_available = avail; v.extra_info = extra;
            vt.rows.push_back(v);
            vt.next_id = max(vt.next_id, id + 1);
        }
        cout << "[FILE] Loaded " << vt.rows.size() << " vehicle(s) from file.\n";
    }
    ifstream cin2("customers.txt");
    if (cin2) {
        int id; string name, phone;
        while (cin2 >> id >> name >> phone) {
            CustomerRow c; c.customer_id = id; c.name = name; c.phone = phone;
            ct.rows.push_back(c);
            ct.next_id = max(ct.next_id, id + 1);
        }
        cout << "[FILE] Loaded " << ct.rows.size() << " customer(s) from file.\n";
    }
    ifstream rin("rentals.txt");
    if (rin) {
        int id, cid, vid, days; float total; bool ret;
        while (rin >> id >> cid >> vid >> days >> total >> ret) {
            RentalRow r; r.rental_id = id; r.customer_id = cid; r.vehicle_id = vid;
            r.days = days; r.total_rent = total; r.is_returned = ret;
            rt.rows.push_back(r);
            rt.next_id = max(rt.next_id, id + 1);
        }
        cout << "[FILE] Loaded " << rt.rows.size() << " rental(s) from file.\n";
    }
}

// ============================================================
// SECTION 10: MAIN MENU
// ============================================================

void resetInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    cout << "==============================================\n";
    cout << "  VEHICLE RENTAL SYSTEM — DBMS Demo (C++)\n";
    cout << "==============================================\n\n";

    // DDL: CREATE TABLE (constructor output)
    VehicleTable  vt;
    CustomerTable ct;
    RentalTable   rt;
    Transaction   txn;

    // Load persisted data
    loadFromFile(vt, ct, rt);

    // Seed default vehicles if file was empty (DEFAULT values demonstrated)
    if (vt.rows.empty()) {
        cout << "\n[DDL] Seeding default data...\n";
        VehicleRow v1; v1.brand="Toyota"; v1.model="Fortuner"; v1.type="CAR"; v1.rent_per_day=3000; v1.extra_info=7;
        VehicleRow v2; v2.brand="Yamaha"; v2.model="R15"; v2.type="BIKE"; v2.rent_per_day=800; v2.extra_info=155;
        VehicleRow v3; v3.brand="Honda"; v3.model="City"; v3.type="CAR"; v3.rent_per_day=2200; v3.extra_info=5;
        VehicleRow v4; v4.brand="KTM"; v4.model="Duke390"; v4.type="BIKE"; v4.rent_per_day=1200; v4.extra_info=373;
        try { vt.insert(v1); vt.insert(v2); vt.insert(v3); vt.insert(v4); }
        catch(exception& e) { cerr << e.what() << "\n"; }
    }

    int choice;
    while (true) {
        cout << "\n========== MAIN MENU ==========\n";
        cout << " 1. Add Vehicle        (DDL INSERT + Constraints)\n";
        cout << " 2. Show Vehicles      (SELECT)\n";
        cout << " 3. Add Customer       (DDL INSERT + UNIQUE)\n";
        cout << " 4. Show Customers     (SELECT)\n";
        cout << " 5. Rent Vehicle       (PROCEDURE + TCL)\n";
        cout << " 6. Return Vehicle     (PROCEDURE + FUNCTION + TCL)\n";
        cout << " 7. Show Rentals       (INNER JOIN)\n";
        cout << " 8. Vehicle-Rental Map (LEFT JOIN)\n";
        cout << " 9. Manual Rollback    (TCL ROLLBACK demo)\n";
        cout << "10. Save & Exit\n";
        cout << "================================\nChoice: ";
        cin >> choice;

        if (cin.fail()) { resetInput(); continue; }

        if (choice == 1) {
            // ADD VEHICLE (DDL INSERT with constraint check)
            string brand, model, type;
            float rent; int extra;
            cin.ignore();
            cout << "Brand: "; getline(cin, brand);
            cout << "Model: "; getline(cin, model);
            cout << "Type (CAR/BIKE): "; cin >> type;
            cout << "Rent/day: "; cin >> rent;
            cout << (type == "CAR" ? "Seats: " : "Engine CC: "); cin >> extra;

            VehicleRow v;
            v.brand = brand; v.model = model; v.type = type;
            v.rent_per_day = rent; v.extra_info = extra;

            txn.begin(vt, ct, rt);
            try {
                vt.insert(v);
                cout << "[INSERT] Vehicle added (ID=" << vt.next_id-1 << ")\n";
                txn.commit();
                saveToFile(vt, ct, rt);
            } catch (ConstraintViolation& e) {
                cout << "[CONSTRAINT ERROR] " << e.what() << "\n";
                txn.rollback(vt, ct, rt);
            }

        } else if (choice == 2) {
            showVehicles(vt);

        } else if (choice == 3) {
            // ADD CUSTOMER (UNIQUE phone constraint)
            string name, phone;
            cin.ignore();
            cout << "Name: "; getline(cin, name);
            cout << "Phone: "; cin >> phone;

            CustomerRow c; c.name = name; c.phone = phone;
            txn.begin(vt, ct, rt);
            try {
                ct.insert(c);
                cout << "[INSERT] Customer added (ID=" << ct.next_id-1 << ")\n";
                txn.commit();
            } catch (ConstraintViolation& e) {
                cout << "[CONSTRAINT ERROR] " << e.what() << "\n";
                txn.rollback(vt, ct, rt);
            }

        } else if (choice == 4) {
            showCustomers(ct);

        } else if (choice == 5) {
            // RENT VEHICLE — calls stored procedure
            showCustomers(ct);
            showVehicles(vt);
            int cid, vid, days;
            cout << "Customer ID: "; cin >> cid;
            cout << "Vehicle ID: ";  cin >> vid;
            cout << "Number of days: "; cin >> days;
            ProcRentVehicle(cid, vid, days, vt, ct, rt, txn);
            saveToFile(vt, ct, rt);

        } else if (choice == 6) {
            // RETURN VEHICLE — calls stored procedure + function
            InnerJoinRentals(rt, ct, vt);
            int rid, extra_days;
            cout << "Rental ID to return: "; cin >> rid;
            cout << "Extra days (if late, else 0): "; cin >> extra_days;
            ProcReturnVehicle(rid, extra_days, vt, ct, rt, txn);
            saveToFile(vt, ct, rt);

        } else if (choice == 7) {
            InnerJoinRentals(rt, ct, vt);

        } else if (choice == 8) {
            LeftJoinVehiclesRentals(vt, rt);

        } else if (choice == 9) {
            // Manual rollback demo
            cout << "\n[TCL DEMO] Starting a transaction, adding dummy vehicle, then rolling back...\n";
            txn.begin(vt, ct, rt);
            VehicleRow dummy; dummy.brand = "DUMMY"; dummy.model = "TestX";
            dummy.type = "CAR"; dummy.rent_per_day = 1; dummy.extra_info = 1;
            vt.insert(dummy);
            cout << "  Dummy vehicle added. Vehicles count = " << vt.rows.size() << "\n";
            txn.rollback(vt, ct, rt);
            cout << "  After ROLLBACK, vehicles count = " << vt.rows.size() << "\n";

        } else if (choice == 10) {
            saveToFile(vt, ct, rt);
            cout << "\nGoodbye!\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
    

---
      
# 🚗 Vehicle Rental System (C++) 

A **C++ console-based Vehicle Rental System** project designed to demonstrate **Object-Oriented Programming (OOP)** concepts such as **classes, inheritance, polymorphism, and file handling**. This project allows users to manage a rental service with **Cars** and **Bikes**, track their availability, and rent them while persisting data between program runs.

---

## 📖 Table of Contents

1. [Project Overview](#project-overview)
2. [DBMS Simulation Core](#DBMS-Simulation-Core)
3. [Features](#features)
4. [How It Works](#how-it-works)
5. [Class Structure & OOP Design](#class-structure--oop-design)
6. [Data Storage](#data-storage)
7. [File Structure](#file-structure)
8. [Sample Console Output](#sample-console-output)
9. [Diagrams](#diagrams)
10. [How to Run](#how-to-run)
11. [Future Improvements](#future-improvements)

---

## 📝 Project Overview

This project simulates a **vehicle rental service** in a console environment. It allows a rental manager to:

* Add new vehicles (Cars/Bikes)
* Display all available vehicles with details
* Rent vehicles to customers
* Track availability dynamically
* Save all vehicle data persistently in a file

It is a **hands-on demonstration of OOP** principles in C++, including:

* **Encapsulation**: Data members are private/protected; accessed via getters/setters
* **Inheritance**: `Car` and `Bike` inherit from `Vehicle`
* **Polymorphism**: Virtual functions allow dynamic behavior for different vehicle types
* **File Handling**: Persistent storage using `.txt` file

---

## ⚡ Features

| Feature            | Description                                           |
| ------------------ | ----------------------------------------------------- |
| Vehicle Management | Add Cars or Bikes with unique IDs                     |
| Rent / Return      | Update availability status automatically              |
| Data Persistence   | Vehicle info saved and loaded from `vehicles.txt`     |
| Bulk Loading       | Load multiple vehicles automatically for testing/demo |
| Unique IDs         | Each vehicle has a unique ID for tracking             |
| Console Interface  | Simple, menu-driven system for easy navigation        |

---

🚗 Vehicle Rental System (C++ & DBMS Simulation)

A comprehensive console-based application that merges Object-Oriented Programming (OOP) with Database Management System (DBMS) simulation. This project allows users to manage a rental service while demonstrating core relational database engine concepts like integrity constraints, joins, and transactions.

## 🏗️ DBMS Simulation Core

The project includes a dedicated engine that mimics professional SQL behavior through the following features:

1. DDL & Constraints

* Table Structure: Data is logically organized into three virtual tables: vehicles, customers, and rentals.
* NOT NULL: Every field is strictly validated to ensure it contains data before any insertion.
* UNIQUE: The system prevents duplicate phone numbers in the customer table to maintain data integrity.
* CHECK: Values like rent_per_day and days must be positive, and vehicle type is restricted to 'CAR' or 'BIKE'.

* FOREIGN KEY: Referential integrity is enforced; rental records are blocked unless both the customer_id and vehicle_id exist in the parent tables.

2. JOIN Operations

* The system implements relational logic to combine data across multiple tables:
* INNER JOIN: Generates a comprehensive report by matching existing rentals with their specific customer and vehicle details.
* LEFT JOIN: Lists all vehicles in the database. If a vehicle has no rental history, the system displays 'NULL' for the rental columns, mirroring standard SQL behavior.

3. TCL (Transaction Control Language)

* To maintain data integrity (ACID properties) during complex operations, the system utilizes a Transaction class:
* BEGIN: Saves a snapshot of all table data before an operation begins.
* COMMIT: Finalizes changes and makes them permanent after a successful operation.
* ROLLBACK: If a constraint is violated or an error occurs, the system restores the tables to their previous state using the saved snapshot.

4. Stored Procedures & Functions

* ProcRentVehicle(): An atomic procedure that handles the multi-step process of checking availability, inserting a rental record, and updating vehicle status in one transaction.
* CalculateFine(): A stored function that computes a 1.5x penalty for late returns based on the daily rent rate.

⚡ OOP Design & Features

* The application is built on a foundation of strong Object-Oriented principles:
* Inheritance: Car and Bike classes inherit from a base Vehicle class to share common attributes while maintaining unique properties (like seats or engine CC).
* Encapsulation: Data members are protected and accessed only through specialized validation methods to ensure safe data modification.
* Default Values: Attributes like is_available and is_returned utilize default initializers (e.g., DEFAULT TRUE), simulating SQL table defaults.

💾 File Handling & Persistence

* Data is persisted through local text files to ensure records are preserved between program executions.

* Format: ID | TYPE | BRAND | MODEL | RENT | STATUS | EXTRA

# Mechanism: The system automatically loads the state of all virtual tables from vehicles.txt on startup and updates the file whenever a transaction is successfully committed.

---

## 🔍 How It Works

### 1️⃣ Vehicle Classes

The system has **3 main classes**:

* **Vehicle (Base Class)**:

  ```cpp
  class Vehicle {
      int id;
      string brand;
      string model;
      float rentPerDay;
      bool isAvailable;
  };
  ```

  * **Purpose**: Stores common properties for all vehicles
  * **How it works**: Constructor initializes ID, brand, model, rent, and availability.
  * **Methods**:

    * `getId()`, `getBrand()`, `getModel()`, `getRentPerDay()`, `available()`

* **Car (Derived Class)**:

  ```cpp
  class Car : public Vehicle {
      int seats;
  };
  ```

  * Inherits from `Vehicle`
  * Adds `seats` property
  * Overrides virtual functions as needed

* **Bike (Derived Class)**:

  ```cpp
  class Bike : public Vehicle {
      int engineCC;
  };
  ```

  * Inherits from `Vehicle`
  * Adds `engineCC` property
  * Overrides virtual functions as needed

---

### 2️⃣ RentalSystem Class

This is the **main manager class**:

* Stores all vehicles in a `vector<Vehicle*> vehicles`
* Tracks `nextId` for automatic unique ID assignment
* Handles:

  * Adding vehicles (`addVehicle()`)
  * Displaying vehicles (`showVehicles()`)
  * Renting vehicles (`rentVehicle()`)
  * Loading/saving vehicles to a file (`loadFromFile()`, `saveToFile()`)

**Key principle**: All vehicle operations go through this class → central point of control.

---

### 3️⃣ File Handling

* File: `vehicles.txt`
* Each line format:

```
ID TYPE BRAND MODEL RENT AVAILABLE EXTRA
```

* Example:

```
1 CAR Toyota Fortuner 3000 1 7
2 BIKE Yamaha R15 800 1 155
```

* **How it works**:

  * On startup, file is read line by line
  * Each line is parsed and added to `vehicles` vector
  * `nextId` is updated to avoid ID conflicts
  * On adding/renting vehicles, file is overwritten with current data

---

### 4️⃣ Menu Flow (Console UI)

```
=========================
 VEHICLE RENTAL SYSTEM
=========================
1. Add Vehicle
2. Show Vehicles
3. Rent Vehicle
4. Exit
```

* **Add Vehicle**: User chooses Car/Bike → inputs brand, model, rent, extra info
* **Show Vehicles**: Prints table with all details (ID, Brand, Model, Rent, Type, Extra)
* **Rent Vehicle**: User enters ID → availability toggled → file updated
* **Exit**: Program ends

---

## 📊 Class Diagram (Textual)

```
          +------------------+
          |    Vehicle       |
          +------------------+
          | - id             |
          | - brand          |
          | - model          |
          | - rentPerDay     |
          | - isAvailable    |
          +------------------+
          | +getId()         |
          | +getBrand()      |
          | +getModel()      |
          | +getRentPerDay() |
          | +available()     |
          +------------------+
                  ^
                  |
        -------------------
        |                 |
   +---------+       +---------+
   |   Car   |       |  Bike   |
   +---------+       +---------+
   | - seats |       | - cc    |
   +---------+       +---------+
```

---

## 🖥️ Sample Output

```
ID   Brand       Model        Rent  Status     Type    Extra
---------------------------------------------------------------
1    Toyota      Fortuner     3000  Available  CAR     Seats: 7
2    Yamaha      R15          800   Available  BIKE    CC: 155
```

* Renting a vehicle updates `Status` → `Rented`
* File automatically updates for persistence

---

## ⚙️ How to Run

1. Clone the repo:

```bash
git clone <repo-url>
cd Vehical-Rental-System
```

2. Compile:

```bash
g++ -std=c++11 Vehical-rental-system.cpp -o rental.exe
```

3. Run:

```bash
.\rental.exe
```

4. Follow the menu instructions

---

## 💡 Future Improvements

* Customer class & booking history
* GUI interface using Qt/console graphics
* Search/filter vehicles by brand or type
* Multiple rental rates (hour/day/week)
* Support for CSV import/export

---

## 🔗 Summary

This project is a **complete demonstration of OOP in C++**, including:

* Inheritance & Polymorphism
* Encapsulation
* Dynamic memory management (`vector<Vehicle*>`)
* File persistence
* Menu-driven console interface

It is ideal as a **college-level project** to show **full OOP understanding and practical file-based persistence**.

---


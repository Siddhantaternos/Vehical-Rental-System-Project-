    

---
      
# 🚗 Vehicle Rental System (C++) 

A **C++ console-based Vehicle Rental System** project designed to demonstrate **Object-Oriented Programming (OOP)** concepts such as **classes, inheritance, polymorphism, and file handling**. This project allows users to manage a rental service with **Cars** and **Bikes**, track their availability, and rent them while persisting data between program runs.

---

## 📖 Table of Contents

1. [Project Overview](#project-overview)
2. [Features](#features)
3. [How It Works](#how-it-works)
4. [Class Structure & OOP Design](#class-structure--oop-design)
5. [Data Storage](#data-storage)
6. [File Structure](#file-structure)
7. [Sample Console Output](#sample-console-output)
8. [Diagrams](#diagrams)
9. [How to Run](#how-to-run)
10. [Future Improvements](#future-improvements)

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


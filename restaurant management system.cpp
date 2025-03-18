#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <algorithm>

using namespace std;

class MenuItem {
public:
    string name;
    double price;

    MenuItem(string n, double p) : name(move(n)), price(p) {}
};

class Order {
public:
    int orderID;
    unordered_map<string, int> items;
    double totalAmount;
    string status;

    Order(int id, unordered_map<string, int>& items, double amount) 
        : orderID(id), items(move(items)), totalAmount(amount), status("Preparing") {}
};

class Restaurant {
private:
    unordered_map<string, double> menu;
    unordered_map<string, int> orderCount;
    vector<Order> orderHistory;
    unordered_map<string, int> loyaltyProgram;
    unordered_map<int, string> orderStatus;
    vector<string> reservedTables;
    double totalSales = 0;
    int orderCounter = 1;

public:
    Restaurant() {
        addMenuItem("Biryani", 12.99);
        addMenuItem("Burger", 8.99);
        addMenuItem("Soup", 5.49);
        addMenuItem("Pepsi", 1.99);
        addMenuItem("Coffee", 2.49);
    }

    void addMenuItem(const string& name, double price) {
        menu[name] = price;
    }

    void displayMenu() const {
        if (menu.empty()) {
            cout << "No menu items available.\n";
            return;
        }
        cout << "\nMenu:\n";
        for (const auto& item : menu)
            cout << item.first << " - $" << fixed << setprecision(2) << item.second << endl;
    }

    void reserveTable(const string& customerName) {
        reservedTables.push_back(customerName);
        cout << "Table reserved for " << customerName << endl;
    }

    void displayReservations() const {
        if (reservedTables.empty()) {
            cout << "No reservations yet.\n";
            return;
        }
        cout << "Reserved Tables:\n";
        for (const auto& name : reservedTables) 
            cout << "- " << name << endl;
    }

    void takeOrder() {
        unordered_map<string, int> orderItems;
        string customerName, itemName;
        int quantity;
        double total = 0.0;

        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, customerName);

        while (true) {
            cout << "Enter menu item to order (or 'done' to finish): ";
            getline(cin, itemName);
            if (itemName == "done") break;

            if (menu.find(itemName) != menu.end()) {
                cout << "Enter quantity: ";
                cin >> quantity;
                cin.ignore();
                orderItems[itemName] += quantity;
                orderCount[itemName] += quantity;
                totalSales += menu[itemName] * quantity;
                total += menu[itemName] * quantity;
            } else {
                cout << "Item not found!\n";
            }
        }

        // Loyalty Discount
        if (loyaltyProgram[customerName] >= 5) {
            cout << "Loyalty Discount Applied: 10% Off\n";
            total *= 0.90;
        }
        loyaltyProgram[customerName]++;

        orderHistory.emplace_back(orderCounter++, orderItems, total);
        orderStatus[orderCounter - 1] = "Preparing";

        // Bill Splitting
        cout << "Split bill? (yes/no): ";
        string split;
        cin >> split;
        if (split == "yes") {
            int people;
            cout << "Enter number of people: ";
            cin >> people;
            cout << "Each person pays: $" << fixed << setprecision(2) << total / people << endl;
        } else {
            cout << "Total bill: $" << fixed << setprecision(2) << total << endl;
        }
    }

    void displayOrderHistory() const {
        if (orderHistory.empty()) {
            cout << "No orders yet.\n";
            return;
        }
        cout << "\nOrder History:\n";
        for (const auto& order : orderHistory) {
            cout << "Order #" << order.orderID << " - $" << fixed << setprecision(2) << order.totalAmount 
                 << " - Status: " << orderStatus.at(order.orderID) << endl;
            for (const auto& item : order.items) 
                cout << "  " << item.first << " x" << item.second << endl;
        }
    }

    void updateOrderStatus(int orderID, const string& newStatus) {
        if (orderStatus.find(orderID) != orderStatus.end()) {
            orderStatus[orderID] = newStatus;
            cout << "Order #" << orderID << " status updated to: " << newStatus << endl;
        } else {
            cout << "Order not found.\n";
        }
    }
};

int main() {
    Restaurant restaurant;
    int choice;

    do {
        cout << "\nRestaurant Management System\n";
        cout << "1. Display Menu\n";
        cout << "2. Reserve Table\n";
        cout << "3. View Reservations\n";
        cout << "4. Take Order\n";
        cout << "5. View Order History\n";
        cout << "6. Update Order Status\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                restaurant.displayMenu();
                break;
            case 2: {
                string name;
                cout << "Enter customer name: ";
                getline(cin, name);
                restaurant.reserveTable(name);
                break;
            }
            case 3:
                restaurant.displayReservations();
                break;
            case 4:
                restaurant.takeOrder();
                break;
            case 5:
                restaurant.displayOrderHistory();
                break;
            case 6: {
                int id;
                string status;
                cout << "Enter Order ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter new status (Preparing/Ready/Served): ";
                getline(cin, status);
                restaurant.updateOrderStatus(id, status);
                break;
            }
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }
    } while (choice != 7);

    return 0;
}

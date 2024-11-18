#include "productOrder.h"
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

// Functions for displaying menus
void displayMenu() {
    std::cout << "Hello! Be welcome to your favorite fast food!\n";
    std::cout << "\nSelect below what you would like to add to your order: \n";
    std::cout << "\n==== Main Menu ====\n";
    std::cout << "1. Fries\n";
    std::cout << "2. Grill\n";
    std::cout << "3. Salad\n";
    std::cout << "4. Drink\n";
    std::cout << "5. Dessert\n";
    std::cout << "6. Finish order\n";
    std::cout << "7. Exit\n";
    std::cout << "Choose an option:\n";
}

void displayFriesMenu() {
    std::cout << "\n==== Menu Fries ====\n";
    std::cout << "1. French Fries - R$ 8,00\n";
    std::cout << "2. Potato Wedge - R$ 10,00\n";
    std::cout << "3. Go Back\n";
    std::cout << "Choose an option: ";
}

void displayGrillMenu() {
    std::cout << "\n==== Grill Menu ====\n";
    std::cout << "1. Hamburger - R$ 12,50\n";
    std::cout << "2. Cheeseburguer - R$ 20,00\n";
    std::cout << "3. Go Back\n";
    std::cout << "Choose an option: ";
}

void displaySaladMenu() {
    std::cout << "\n==== Salad Menu ====\n";
    std::cout << "1. Green Salad - R$ 6,00\n";
    std::cout << "2. Caesar Salad - R$ 10,00\n";
    std::cout << "3. Go Back\n";
    std::cout << "Choose an option: ";
}

void displayDrinkMenu() {
    std::cout << "\n==== Drink Menu ====\n";
    std::cout << "1. Orange Juice - R$ 5,00\n";
    std::cout << "2. Coca-Cola - R$ 4,50\n";
    std::cout << "3. Voltar\n";
    std::cout << "Choose an option: ";
}

void displayDessertMenu() {
    std::cout << "\n==== Dessert Menu ====\n";
    std::cout << "1. Pudding - R$ 7,50\n";
    std::cout << "2. Ice Cream - R$ 6,00\n";
    std::cout << "3. Go Back\n";
    std::cout << "Choose an option: ";
}

int getIntInput(const std::string& prompt) {
    int choice;
    std::cout << prompt;
    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid Input.... " << prompt;
    }
    return choice;
}

// Function to automatically add products to the order
void addProductToOrder(Order& order, ProductType type, const std::string& name, double price) {
    int quantity;
    std::cout << "\nEnter the amount of " << name << ": ";
    quantity = getIntInput("Enter quantity: ");

    Product newProduct = { name, quantity, price, type };
    order.products.push_back(newProduct);
    order.totalValueSum();
    std::cout << "Product added: " << name << " (Quantity: " << quantity << ", Price: " << price << ")\n";
}

// Function to print the order summary
void printOrderSummary(const Order& order) {
    std::cout << "\n==== Order Resume ====\n";
    for (const auto& product : order.products) {
        std::cout << "Product: " << product.productName
            << " | Quantity: " << product.productQuantity
            << " | Unitary Price: R$ " << product.productValue
            << " | Subtotal: R$ " << product.totalOrderValue() << "\n";
    }
    std::cout << "Order total value: R$ " << order.totalValue << "\n";
}

int main() {
    orderLibrary orders;
    orders.startProcessing();

    Order currentOrder = { 0, {} };
    bool running = true;

    while (running) {
        displayMenu();
        int choice = getIntInput("Choose an option: ");

        switch (choice) {
        case 1: { // Fries
            int friesChoice;
            displayFriesMenu();
            friesChoice = getIntInput("Choose an option: ");
            if (friesChoice == 1) {
                addProductToOrder(currentOrder, ProductType::Fries, "French Fries", 8.00);
            }
            else if (friesChoice == 2) {
                addProductToOrder(currentOrder, ProductType::Fries, "Potato Wedge", 10.00);
            }
            else if (friesChoice == 3) {
                break;
            }
            break;
        }
        case 2: { // Grill
            int grillChoice;
            displayGrillMenu();
            grillChoice = getIntInput("Choose an option: ");
            if (grillChoice == 1) {
                addProductToOrder(currentOrder, ProductType::Grill, "Hamburger", 12.50);
            }
            else if (grillChoice == 2) {
                addProductToOrder(currentOrder, ProductType::Grill, "Cheeseburger", 20.00);
            }
            else if (grillChoice == 3) {
                break;
            }
            break;
        }
        case 3: { // Salad
            int saladChoice;
            displaySaladMenu();
            saladChoice = getIntInput("Choose an option: ");
            if (saladChoice == 1) {
                addProductToOrder(currentOrder, ProductType::Salad, "Green Salad", 6.00);
            }
            else if (saladChoice == 2) {
                addProductToOrder(currentOrder, ProductType::Salad, "Caesar Salad", 10.00);
            }
            else if (saladChoice == 3) {
                break;
            }
            break;
        }
        case 4: { // Drink
            int drinkChoice;
            displayDrinkMenu();
            drinkChoice = getIntInput("Choose an option: ");
            if (drinkChoice == 1) {
                addProductToOrder(currentOrder, ProductType::Drink, "Orange Juice", 5.00);
            }
            else if (drinkChoice == 2) {
                addProductToOrder(currentOrder, ProductType::Drink, "Coca-Cola", 4.50);
            }
            else if (drinkChoice == 3) {
                break;
            }
            break;
        }
        case 5: { // Dessert
            int dessertChoice;
            displayDessertMenu();
            dessertChoice = getIntInput("Choose an option: ");
            if (dessertChoice == 1) {
                addProductToOrder(currentOrder, ProductType::Dessert, "Pudding", 7.50);
            }
            else if (dessertChoice == 2) {
                addProductToOrder(currentOrder, ProductType::Dessert, "Ice Cream", 6.00);
            }
            else if (dessertChoice == 3) {
                break;
            }
            break;
        }
        case 6: { // Finish order
            std::cout << "\nOrder Finished!\n";
            printOrderSummary(currentOrder);  // Print the order summary
            orders.addOrder(currentOrder);

            // Clean the order after finished
            currentOrder.products.clear();  // Clean the products for the next order
            currentOrder.totalValue = 0;    // Reset the total value of order
            break;
        }
        case 7: { // Exit
            std::cout << "\nThank you for shopping with us! See you latter!\n";
            running = false;
            break;
        }
        default:
            std::cout << "\n\nInvalid option! Try again.\n\n";
        }
    }

    // Thread to remove finished ready products from queue and display them
    // The index at the end of the print is equivalent to the kitchen areas 
    // (0 = Fries, 1 = Grill, 2 = Salad, 3 = Drink, 4 = Dessert)
    std::thread kitchen([&orders]() {
        while (true) {
            Product readyProduct = orders.removeReadyProduct();
            std::cout << "\nProduct ready to serve: " << readyProduct.productName << " ("
                << static_cast<int>(readyProduct.productType) << ")" << std::endl;
        }
        });

    kitchen.join();
    return 0;
}

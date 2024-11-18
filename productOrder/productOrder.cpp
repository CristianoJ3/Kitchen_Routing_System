#include "productOrder.h"
#include <iostream>
#include <thread>
#include <chrono>

// A function to add a new order to the system
void orderLibrary::addOrder(Order order)
{
    std::lock_guard<std::mutex> lock(mtxOrder);
    order.id = nextID++;

    queueOrder.push(order);
    condOrderVar.notify_one();  // Notifies the thread that a new order has been added
}
// A function to start to processing an order
void orderLibrary::startProcessing()
{
    std::thread(&orderLibrary::processingOrder, this).detach();
}
// A function to remove a product when it is ready to pick up
Product orderLibrary::removeReadyProduct()
{
    std::unique_lock<std::mutex> lock(mtxOrder); // Mutex to protect access to the request queue
    condOrderVar.wait(lock, [this] {
        // Wait while all queues are empty
        return !queueFries.empty() || !queueGrill.empty() || !queueSalad.empty() || !queueDrink.empty() || !queueDessert.empty();
        });

    // Check which queue has ready products and return the first item in that queue
    Product readyProduct;
    if (!queueFries.empty()) {
        readyProduct = queueFries.front();
        queueFries.pop();
    }
    else if (!queueGrill.empty()) {
        readyProduct = queueGrill.front();
        queueGrill.pop();
    }
    else if (!queueSalad.empty()) {
        readyProduct = queueSalad.front();
        queueSalad.pop();
    }
    else if (!queueDrink.empty()) {
        readyProduct = queueDrink.front();
        queueDrink.pop();
    }
    else if (!queueDessert.empty()) {
        readyProduct = queueDessert.front();
        queueDessert.pop();
    }

    return readyProduct;
}

// A function to notify when the order starts to be prepared
void orderLibrary::startingOrder(const Order& order)
{
    std::cout << "\nStarting the order from ID: " << order.id << "\n" << std::endl;
    std::cout << "\nOrder total value: R$ " << order.totalValue << "\n" << std::endl;

    for (const auto& product : order.products)
    {
        std::cout << "\nPreparing product: " << product.productName << "\n"
            << "Quantity: " << product.productQuantity << "\n"
            << "Unit price: R$ " << product.productValue << "\n"
            << "Subtotal: R$ " << product.totalOrderValue() << "\n" << std::endl;

        // Routes the product to the correct queue based on the product type
        {
            std::lock_guard<std::mutex> lock([this, &product]() -> std::mutex& {
                switch (product.productType) {
                case ProductType::Fries: return mtxFries;
                case ProductType::Grill: return mtxGrill;
                case ProductType::Salad: return mtxSalad;
                case ProductType::Drink: return mtxDrink;
                case ProductType::Dessert: return mtxDessert;
                default: throw std::invalid_argument("Unknown product type");
                }
                }());

            switch (product.productType) {
            case ProductType::Fries: queueFries.push(product); break;
            case ProductType::Grill: queueGrill.push(product); break;
            case ProductType::Salad: queueSalad.push(product); break;
            case ProductType::Drink: queueDrink.push(product); break;
            case ProductType::Dessert: queueDessert.push(product); break;
            default: throw std::invalid_argument("Unknown product type");
            }
        }

        // Notifies that a product has been added to the queue
        switch (product.productType) {
        case ProductType::Fries: condFries.notify_one(); break;
        case ProductType::Grill: condGrill.notify_one(); break;
        case ProductType::Salad: condSalad.notify_one(); break;
        case ProductType::Drink: condDrink.notify_one(); break;
        case ProductType::Dessert: condDessert.notify_one(); break;
        default: throw std::invalid_argument("Unknown product type");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500 * product.productQuantity));
    }
    std::cout << "\nOrder ID: " << order.id << " finished!\n" << std::endl;
}
// A funtion to processing the order and send this to starting to prepair
void orderLibrary::processingOrder()
{
    while (true)
    {
        Order currentOrder;
        {
            std::unique_lock<std::mutex> lock(mtxOrder);
            condOrderVar.wait(lock, [this] { return !queueOrder.empty(); });

            currentOrder = queueOrder.front();
            queueOrder.pop();
        }

        currentOrder.totalValueSum();
        startingOrder(currentOrder);
    }
}

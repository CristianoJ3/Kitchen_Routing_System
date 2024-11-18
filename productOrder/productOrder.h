#ifndef PRODUCT_ORDER_H
#define PRODUCT_ORDER_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>

// Enum for product types
enum class ProductType {
    Fries,
    Grill,
    Salad,
    Drink,
    Dessert
};

// This struct represents a product and your characteristics
struct Product
{
    std::string productName;
    int productQuantity;
    double productValue;
    ProductType productType; 

    // This function returns the total order value based on the product quantity
    double totalOrderValue() const
    {
        return productQuantity * productValue;
    }
};

// This structure represents an order
struct Order
{
    int id;
    std::vector<Product> products;
    double totalValue;

    // This structure adds the value of each order using the function totalOrderValue()
    double totalValueSum()
    {
        totalValue = 0;
        for (const auto& product : products) {
            totalValue += product.totalOrderValue();
        }
        return totalValue;
    }
};

// Class responsible for processing all orders
class orderLibrary
{
public:
    void addOrder(Order order); // Adding an order to the queue
    void startProcessing(); // Starting order processing
    Product removeReadyProduct(); // Remove and return a finished product

private:
    std::queue<Order> queueOrder;  // Queue to store orders

    // Queue to store products by type
    std::queue<Product> queueFries;
    std::queue<Product> queueGrill;
    std::queue<Product> queueSalad;
    std::queue<Product> queueDrink;
    std::queue<Product> queueDessert;

    // Mutexes to control access to queues
    std::mutex mtxOrder;   // Mutex for orders
    std::mutex mtxFries;   // Mutex for the Fries queue
    std::mutex mtxGrill;   // Mutex for the Grill queue
    std::mutex mtxSalad;   // Mutex for the Salad queue
    std::mutex mtxDrink;   // Mutex for the Drink queue
    std::mutex mtxDessert; // Mutex for the Dessert queue

    // Condition variables to control access to ready product queues
    std::condition_variable condOrderVar; // Condition for order queue
    std::condition_variable condFries;    // Condition for Fries queue
    std::condition_variable condGrill;    // Condition for Grill queue
    std::condition_variable condSalad;    // Condition for Salad queue
    std::condition_variable condDrink;    // Condition for Drink queue
    std::condition_variable condDessert;  // Condition for Dessert queue

    int nextID = 1; // Variable to generate a new ID

    void startingOrder(const Order& order); // Function to prepare the order
    void processingOrder(); // Continuous function to process the order
};

#endif //PRODUCT_ORDER_H

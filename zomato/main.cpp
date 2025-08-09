#include<bits/stdc++.h>
using namespace std;

enum class OrderStatus {
    Pending,
    Confirmed,
    Preparing,
    OutForDelivery,
    Delivered,
    Cancelled
};

enum class PaymentMode {
    CreditCard,
    DebitCard,
    UPI,
    Cash,
    Wallet
};

enum class DeliveryMode {
    HomeDelivery,
    Pickup
};

enum class RestaurantStatus {
    Open,
    Closed,
    Busy
};

// Forward declarations
class User;
class Restaurant;
class MenuItem;
class Order;

class Location {
private:
    double latitude;
    double longitude;
    string address;
    string city;
    string pincode;

public:
    Location() {}
    Location(double lat, double lng, string addr, string city, string pin) 
        : latitude(lat), longitude(lng), address(addr), city(city), pincode(pin) {}
    
    // Getters
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    string getAddress() const { return address; }
    string getCity() const { return city; }
    string getPincode() const { return pincode; }
    
    // Calculate distance between two locations (simplified)
    double calculateDistance(const Location& other) const {
        double latDiff = latitude - other.latitude;
        double lngDiff = longitude - other.longitude;
        return sqrt(latDiff * latDiff + lngDiff * lngDiff) * 111; // Rough km calculation
    }
    
    void displayLocation() const {
        cout << address << ", " << city << " - " << pincode;
    }
};

class MenuItem {
private:
    int itemId;
    string name;
    string description;
    double price;
    string category;
    bool isVegetarian;
    bool isAvailable;
    int preparationTime; // in minutes
    double rating;
    vector<string> ingredients;

public:
    MenuItem() {}
    MenuItem(int id, string name, string desc, double price, string category, 
             bool isVeg = false, int prepTime = 15) 
        : itemId(id), name(name), description(desc), price(price), 
          category(category), isVegetarian(isVeg), preparationTime(prepTime), 
          isAvailable(true), rating(0.0) {}
    
    // Getters
    int getItemId() const { return itemId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }
    bool getIsVegetarian() const { return isVegetarian; }
    bool getIsAvailable() const { return isAvailable; }
    int getPreparationTime() const { return preparationTime; }
    double getRating() const { return rating; }
    
    // Setters
    void setAvailability(bool available) { isAvailable = available; }
    void setPrice(double newPrice) { price = newPrice; }
    void setRating(double newRating) { rating = newRating; }
    
    void displayMenuItem() const {
        cout << "ID: " << itemId << " | " << name << " (" << (isVegetarian ? "Veg" : "Non-Veg") << ")" << endl;
        cout << "Description: " << description << endl;
        cout << "Price: $" << price << " | Category: " << category << endl;
        cout << "Prep Time: " << preparationTime << " mins | Rating: " << rating << "/5" << endl;
        cout << "Available: " << (isAvailable ? "Yes" : "No") << endl;
        cout << "-------------------" << endl;
    }
};

class Menu {
private:
    vector<MenuItem*> menuItems;
    map<string, vector<MenuItem*>> categoryWiseItems;

public:
    void addMenuItem(MenuItem* item) {
        menuItems.push_back(item);
        categoryWiseItems[item->getCategory()].push_back(item);
    }
    
    void removeMenuItem(int itemId) {
        menuItems.erase(
            remove_if(menuItems.begin(), menuItems.end(),
                [itemId](MenuItem* item) { return item->getItemId() == itemId; }),
            menuItems.end());
        
        // Also remove from category wise items
        for (auto& category : categoryWiseItems) {
            category.second.erase(
                remove_if(category.second.begin(), category.second.end(),
                    [itemId](MenuItem* item) { return item->getItemId() == itemId; }),
                category.second.end());
        }
    }
    
    MenuItem* getMenuItemById(int itemId) {
        for (MenuItem* item : menuItems) {
            if (item->getItemId() == itemId) {
                return item;
            }
        }
        return nullptr;
    }
    
    vector<MenuItem*> getItemsByCategory(const string& category) {
        return categoryWiseItems[category];
    }
    
    vector<MenuItem*> getAllMenuItems() {
        return menuItems;
    }
    
    vector<MenuItem*> searchItems(const string& query) {
        vector<MenuItem*> results;
        string lowerQuery = query;
        transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
        
        for (MenuItem* item : menuItems) {
            string itemName = item->getName();
            transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
            
            if (itemName.find(lowerQuery) != string::npos || 
                item->getCategory().find(query) != string::npos) {
                results.push_back(item);
            }
        }
        return results;
    }
    
    void displayMenu() const {
        cout << "\n=== MENU ===" << endl;
        for (const auto& categoryPair : categoryWiseItems) {
            cout << "\n--- " << categoryPair.first << " ---" << endl;
            for (MenuItem* item : categoryPair.second) {
                if (item->getIsAvailable()) {
                    item->displayMenuItem();
                }
            }
        }
    }
};

class User {
private:
    int userId;
    string name;
    string email;
    string phone;
    Location address;
    vector<Order*> orderHistory;
    double walletBalance;

public:
    User() {}
    User(int id, string name, string email, string phone, Location addr) 
        : userId(id), name(name), email(email), phone(phone), address(addr), walletBalance(0.0) {}
    
    // Getters
    int getUserId() const { return userId; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    Location getAddress() const { return address; }
    double getWalletBalance() const { return walletBalance; }
    vector<Order*> getOrderHistory() const { return orderHistory; }
    
    // Setters
    void setAddress(const Location& newAddress) { address = newAddress; }
    void addToWallet(double amount) { walletBalance += amount; }
    bool deductFromWallet(double amount) {
        if (walletBalance >= amount) {
            walletBalance -= amount;
            return true;
        }
        return false;
    }
    
    void addOrderToHistory(Order* order) {
        orderHistory.push_back(order);
    }
    
    void displayUserInfo() const {
        cout << "\n=== User Profile ===" << endl;
        cout << "ID: " << userId << " | Name: " << name << endl;
        cout << "Email: " << email << " | Phone: " << phone << endl;
        cout << "Address: ";
        address.displayLocation();
        cout << endl;
        cout << "Wallet Balance: $" << walletBalance << endl;
        cout << "Total Orders: " << orderHistory.size() << endl;
    }
};

class Restaurant {
private:
    int restaurantId;
    string name;
    Location location;
    string cuisine;
    double rating;
    RestaurantStatus status;
    Menu* menu;
    vector<string> operatingHours; // ["9:00 AM", "11:00 PM"]
    double deliveryFee;
    int averageDeliveryTime; // in minutes
    double minimumOrderAmount;

public:
    Restaurant() {}
    Restaurant(int id, string name, Location loc, string cuisine, double deliveryFee = 2.0) 
        : restaurantId(id), name(name), location(loc), cuisine(cuisine), 
          deliveryFee(deliveryFee), rating(0.0), status(RestaurantStatus::Open),
          averageDeliveryTime(30), minimumOrderAmount(10.0) {
        menu = new Menu();
        operatingHours = {"9:00 AM", "11:00 PM"};
    }
    
    // Getters
    int getRestaurantId() const { return restaurantId; }
    string getName() const { return name; }
    Location getLocation() const { return location; }
    string getCuisine() const { return cuisine; }
    double getRating() const { return rating; }
    RestaurantStatus getStatus() const { return status; }
    Menu* getMenu() { return menu; }
    double getDeliveryFee() const { return deliveryFee; }
    int getAverageDeliveryTime() const { return averageDeliveryTime; }
    double getMinimumOrderAmount() const { return minimumOrderAmount; }
    
    // Setters
    void setStatus(RestaurantStatus newStatus) { status = newStatus; }
    void setRating(double newRating) { rating = newRating; }
    void setDeliveryFee(double fee) { deliveryFee = fee; }
    
    bool isOpen() const {
        return status == RestaurantStatus::Open;
    }
    
    double calculateDeliveryDistance(const Location& userLocation) const {
        return location.calculateDistance(userLocation);
    }
    
    void displayRestaurantInfo() const {
        cout << "\n=== Restaurant Info ===" << endl;
        cout << "ID: " << restaurantId << " | Name: " << name << endl;
        cout << "Cuisine: " << cuisine << " | Rating: " << rating << "/5" << endl;
        cout << "Status: " << (status == RestaurantStatus::Open ? "Open" : 
                              status == RestaurantStatus::Closed ? "Closed" : "Busy") << endl;
        cout << "Delivery Fee: $" << deliveryFee << " | Avg Delivery: " << averageDeliveryTime << " mins" << endl;
        cout << "Min Order: $" << minimumOrderAmount << endl;
        cout << "Location: ";
        location.displayLocation();
        cout << endl;
        cout << "Operating Hours: " << operatingHours[0] << " - " << operatingHours[1] << endl;
    }
};

class Order {
private:
    int orderId;
    User* user;
    Restaurant* restaurant;
    vector<pair<MenuItem*, int>> orderItems; // item, quantity
    double totalAmount;
    double deliveryFee;
    OrderStatus status;
    PaymentMode paymentMode;
    DeliveryMode deliveryMode;
    string orderTime;
    string deliveryTime;
    Location deliveryAddress;

public:
    Order() {}
    Order(int id, User* user, Restaurant* restaurant) 
        : orderId(id), user(user), restaurant(restaurant), totalAmount(0.0),
          deliveryFee(restaurant->getDeliveryFee()), status(OrderStatus::Pending),
          deliveryMode(DeliveryMode::HomeDelivery), deliveryAddress(user->getAddress()) {
        
        // Set order time
        time_t now = time(0);
        orderTime = ctime(&now);
        orderTime.pop_back(); // Remove newline
    }
    
    // Getters
    int getOrderId() const { return orderId; }
    User* getUser() { return user; }
    Restaurant* getRestaurant() { return restaurant; }
    double getTotalAmount() const { return totalAmount; }
    OrderStatus getStatus() const { return status; }
    string getOrderTime() const { return orderTime; }
    vector<pair<MenuItem*, int>> getOrderItems() const { return orderItems; }
    
    // Add items to order
    void addItem(MenuItem* item, int quantity) {
        if (item && item->getIsAvailable()) {
            orderItems.push_back({item, quantity});
            calculateTotalAmount();
        }
    }
    
    void removeItem(int itemId) {
        orderItems.erase(
            remove_if(orderItems.begin(), orderItems.end(),
                [itemId](const pair<MenuItem*, int>& orderItem) {
                    return orderItem.first->getItemId() == itemId;
                }),
            orderItems.end());
        calculateTotalAmount();
    }
    
    void calculateTotalAmount() {
        totalAmount = 0.0;
        for (const auto& orderItem : orderItems) {
            totalAmount += orderItem.first->getPrice() * orderItem.second;
        }
        
        if (deliveryMode == DeliveryMode::HomeDelivery) {
            totalAmount += deliveryFee;
        }
    }
    
    bool validateOrder() {
        if (orderItems.empty()) {
            cout << "Order is empty!" << endl;
            return false;
        }
        
        if (totalAmount - deliveryFee < restaurant->getMinimumOrderAmount()) {
            cout << "Order amount is below minimum order amount of $" << restaurant->getMinimumOrderAmount() << endl;
            return false;
        }
        
        if (!restaurant->isOpen()) {
            cout << "Restaurant is closed!" << endl;
            return false;
        }
        
        return true;
    }
    
    bool processPayment(PaymentMode mode) {
        paymentMode = mode;
        
        if (mode == PaymentMode::Wallet) {
            if (user->deductFromWallet(totalAmount)) {
                status = OrderStatus::Confirmed;
                cout << "Payment successful via Wallet!" << endl;
                return true;
            } else {
                cout << "Insufficient wallet balance!" << endl;
                return false;
            }
        } else {
            // Simulate other payment methods
            status = OrderStatus::Confirmed;
            cout << "Payment successful via " << 
                    (mode == PaymentMode::CreditCard ? "Credit Card" :
                     mode == PaymentMode::DebitCard ? "Debit Card" :
                     mode == PaymentMode::UPI ? "UPI" : "Cash") << "!" << endl;
            return true;
        }
    }
    
    void updateStatus(OrderStatus newStatus) {
        status = newStatus;
        if (status == OrderStatus::Delivered) {
            time_t now = time(0);
            deliveryTime = ctime(&now);
            deliveryTime.pop_back(); // Remove newline
        }
    }
    
    void displayOrderDetails() const {
        cout << "\n=== Order Details ===" << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Restaurant: " << restaurant->getName() << endl;
        cout << "Customer: " << user->getName() << endl;
        cout << "Order Time: " << orderTime << endl;
        cout << "Status: ";
        switch(status) {
            case OrderStatus::Pending: cout << "Pending"; break;
            case OrderStatus::Confirmed: cout << "Confirmed"; break;
            case OrderStatus::Preparing: cout << "Preparing"; break;
            case OrderStatus::OutForDelivery: cout << "Out for Delivery"; break;
            case OrderStatus::Delivered: cout << "Delivered"; break;
            case OrderStatus::Cancelled: cout << "Cancelled"; break;
        }
        cout << endl;
        
        cout << "\n--- Order Items ---" << endl;
        for (const auto& orderItem : orderItems) {
            cout << orderItem.first->getName() << " x " << orderItem.second 
                 << " = $" << (orderItem.first->getPrice() * orderItem.second) << endl;
        }
        
        cout << "\nSubtotal: $" << (totalAmount - deliveryFee) << endl;
        if (deliveryMode == DeliveryMode::HomeDelivery) {
            cout << "Delivery Fee: $" << deliveryFee << endl;
        }
        cout << "Total Amount: $" << totalAmount << endl;
        
        if (!deliveryTime.empty()) {
            cout << "Delivered At: " << deliveryTime << endl;
        }
    }
};

class Search {
private:
    vector<Restaurant*> restaurants;

public:
    void addRestaurant(Restaurant* restaurant) {
        restaurants.push_back(restaurant);
    }
    
    vector<Restaurant*> searchByName(const string& name) {
        vector<Restaurant*> results;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (Restaurant* restaurant : restaurants) {
            string restaurantName = restaurant->getName();
            transform(restaurantName.begin(), restaurantName.end(), restaurantName.begin(), ::tolower);
            
            if (restaurantName.find(lowerName) != string::npos) {
                results.push_back(restaurant);
            }
        }
        return results;
    }
    
    vector<Restaurant*> searchByCuisine(const string& cuisine) {
        vector<Restaurant*> results;
        for (Restaurant* restaurant : restaurants) {
            if (restaurant->getCuisine().find(cuisine) != string::npos) {
                results.push_back(restaurant);
            }
        }
        return results;
    }
    
    vector<Restaurant*> searchByLocation(const Location& userLocation, double maxDistance) {
        vector<Restaurant*> results;
        for (Restaurant* restaurant : restaurants) {
            double distance = restaurant->calculateDeliveryDistance(userLocation);
            if (distance <= maxDistance) {
                results.push_back(restaurant);
            }
        }
        
        // Sort by distance
        sort(results.begin(), results.end(), 
             [&userLocation](Restaurant* a, Restaurant* b) {
                 return a->calculateDeliveryDistance(userLocation) < 
                        b->calculateDeliveryDistance(userLocation);
             });
        
        return results;
    }
    
    vector<Restaurant*> getTopRatedRestaurants(int limit = 10) {
        vector<Restaurant*> sortedRestaurants = restaurants;
        sort(sortedRestaurants.begin(), sortedRestaurants.end(),
             [](Restaurant* a, Restaurant* b) {
                 return a->getRating() > b->getRating();
             });
        
        if (sortedRestaurants.size() > limit) {
            sortedRestaurants.resize(limit);
        }
        
        return sortedRestaurants;
    }
};

class DeliveryService {
private:
    vector<Order*> activeOrders;
    vector<Order*> deliveredOrders;

public:
    void assignOrder(Order* order) {
        if (order->getStatus() == OrderStatus::Confirmed) {
            activeOrders.push_back(order);
            order->updateStatus(OrderStatus::Preparing);
            cout << "Order " << order->getOrderId() << " assigned for preparation." << endl;
        }
    }
    
    void updateOrderStatus(int orderId, OrderStatus newStatus) {
        for (Order* order : activeOrders) {
            if (order->getOrderId() == orderId) {
                order->updateStatus(newStatus);
                
                if (newStatus == OrderStatus::Delivered) {
                    // Move to delivered orders
                    deliveredOrders.push_back(order);
                    activeOrders.erase(
                        remove(activeOrders.begin(), activeOrders.end(), order),
                        activeOrders.end());
                }
                
                cout << "Order " << orderId << " status updated." << endl;
                return;
            }
        }
        cout << "Order not found!" << endl;
    }
    
    vector<Order*> getActiveOrders() {
        return activeOrders;
    }
    
    void displayActiveOrders() {
        cout << "\n=== Active Orders ===" << endl;
        if (activeOrders.empty()) {
            cout << "No active orders." << endl;
            return;
        }
        
        for (Order* order : activeOrders) {
            cout << "Order ID: " << order->getOrderId() 
                 << " | Customer: " << order->getUser()->getName()
                 << " | Restaurant: " << order->getRestaurant()->getName()
                 << " | Amount: $" << order->getTotalAmount() << endl;
        }
    }
};

class RestaurantManager {
private:
    vector<Restaurant*> restaurants;
    vector<User*> users;
    vector<Order*> orders;
    Search* searchService;
    DeliveryService* deliveryService;
    int nextOrderId;

public:
    RestaurantManager() : nextOrderId(1001) {
        searchService = new Search();
        deliveryService = new DeliveryService();
    }
    
    ~RestaurantManager() {
        delete searchService;
        delete deliveryService;
    }
    
    // Restaurant management
    void addRestaurant(Restaurant* restaurant) {
        restaurants.push_back(restaurant);
        searchService->addRestaurant(restaurant);
    }
    
    Restaurant* getRestaurantById(int restaurantId) {
        for (Restaurant* restaurant : restaurants) {
            if (restaurant->getRestaurantId() == restaurantId) {
                return restaurant;
            }
        }
        return nullptr;
    }
    
    // User management
    void addUser(User* user) {
        users.push_back(user);
    }
    
    User* getUserById(int userId) {
        for (User* user : users) {
            if (user->getUserId() == userId) {
                return user;
            }
        }
        return nullptr;
    }
    
    // Order management
    Order* createOrder(int userId, int restaurantId) {
        User* user = getUserById(userId);
        Restaurant* restaurant = getRestaurantById(restaurantId);
        
        if (!user || !restaurant) {
            cout << "Invalid user or restaurant!" << endl;
            return nullptr;
        }
        
        Order* order = new Order(nextOrderId++, user, restaurant);
        orders.push_back(order);
        return order;
    }
    
    bool placeOrder(Order* order, PaymentMode paymentMode) {
        if (!order->validateOrder()) {
            return false;
        }
        
        if (order->processPayment(paymentMode)) {
            order->getUser()->addOrderToHistory(order);
            deliveryService->assignOrder(order);
            return true;
        }
        
        return false;
    }
    
    // Search functionality
    vector<Restaurant*> searchRestaurants(const string& query) {
        return searchService->searchByName(query);
    }
    
    vector<Restaurant*> searchByCuisine(const string& cuisine) {
        return searchService->searchByCuisine(cuisine);
    }
    
    vector<Restaurant*> searchNearby(const Location& userLocation, double maxDistance) {
        return searchService->searchByLocation(userLocation, maxDistance);
    }
    
    // Display methods
    void displayAllRestaurants() {
        cout << "\n=== All Restaurants ===" << endl;
        for (Restaurant* restaurant : restaurants) {
            cout << "ID: " << restaurant->getRestaurantId() 
                 << " | " << restaurant->getName() 
                 << " | " << restaurant->getCuisine()
                 << " | Rating: " << restaurant->getRating() << "/5" << endl;
        }
    }
    
    void displayOrderHistory(int userId) {
        User* user = getUserById(userId);
        if (!user) {
            cout << "User not found!" << endl;
            return;
        }
        
        cout << "\n=== Order History for " << user->getName() << " ===" << endl;
        vector<Order*> userOrders = user->getOrderHistory();
        
        if (userOrders.empty()) {
            cout << "No orders found." << endl;
            return;
        }
        
        for (Order* order : userOrders) {
            cout << "Order ID: " << order->getOrderId()
                 << " | Restaurant: " << order->getRestaurant()->getName()
                 << " | Amount: $" << order->getTotalAmount()
                 << " | Time: " << order->getOrderTime() << endl;
        }
    }
    
    DeliveryService* getDeliveryService() {
        return deliveryService;
    }
};

// Demo functions
void initializeData(RestaurantManager& manager) {
    // Create locations
    Location loc1(28.7041, 77.1025, "Connaught Place", "Delhi", "110001");
    Location loc2(28.5355, 77.3910, "Sector 18", "Noida", "201301");
    Location loc3(19.0760, 72.8777, "Andheri West", "Mumbai", "400058");
    
    // Create restaurants
    Restaurant* restaurant1 = new Restaurant(1, "Pizza Palace", loc1, "Italian", 3.0);
    Restaurant* restaurant2 = new Restaurant(2, "Spice Garden", loc2, "Indian", 2.5);
    Restaurant* restaurant3 = new Restaurant(3, "Dragon Express", loc3, "Chinese", 4.0);
    
    restaurant1->setRating(4.2);
    restaurant2->setRating(4.5);
    restaurant3->setRating(3.8);
    
    // Add menu items to Pizza Palace
    restaurant1->getMenu()->addMenuItem(new MenuItem(101, "Margherita Pizza", "Classic tomato and mozzarella", 12.99, "Pizza", true));
    restaurant1->getMenu()->addMenuItem(new MenuItem(102, "Pepperoni Pizza", "Pepperoni with cheese", 15.99, "Pizza", false));
    restaurant1->getMenu()->addMenuItem(new MenuItem(103, "Garlic Bread", "Crispy garlic bread", 6.99, "Appetizer", true));
    
    // Add menu items to Spice Garden
    restaurant2->getMenu()->addMenuItem(new MenuItem(201, "Butter Chicken", "Creamy tomato curry", 14.99, "Main Course", false));
    restaurant2->getMenu()->addMenuItem(new MenuItem(202, "Paneer Tikka", "Grilled cottage cheese", 12.99, "Main Course", true));
    restaurant2->getMenu()->addMenuItem(new MenuItem(203, "Naan", "Indian bread", 3.99, "Bread", true));
    
    // Add menu items to Dragon Express
    restaurant3->getMenu()->addMenuItem(new MenuItem(301, "Kung Pao Chicken", "Spicy chicken with peanuts", 13.99, "Main Course", false));
    restaurant3->getMenu()->addMenuItem(new MenuItem(302, "Fried Rice", "Wok-tossed rice", 8.99, "Rice", true));
    restaurant3->getMenu()->addMenuItem(new MenuItem(303, "Spring Rolls", "Crispy vegetable rolls", 7.99, "Appetizer", true));
    
    manager.addRestaurant(restaurant1);
    manager.addRestaurant(restaurant2);
    manager.addRestaurant(restaurant3);
    
    // Create users
    Location userLoc1(28.7041, 77.1025, "CP Metro Station", "Delhi", "110001");
    Location userLoc2(28.5355, 77.3910, "Sector 15", "Noida", "201301");
    
    User* user1 = new User(1, "John Doe", "john@email.com", "9876543210", userLoc1);
    User* user2 = new User(2, "Jane Smith", "jane@email.com", "8765432109", userLoc2);
    
    user1->addToWallet(100.0);
    user2->addToWallet(150.0);
    
    manager.addUser(user1);
    manager.addUser(user2);
}

int main() {
    cout << "=== Welcome to Zomato Food Delivery System ===" << endl;
    
    RestaurantManager manager;
    initializeData(manager);
    
    cout << "\n=== System Initialized Successfully ===" << endl;
    
    // Demo 1: Display all restaurants
    manager.displayAllRestaurants();
    
    // Demo 2: Search restaurants
    cout << "\n--- Searching for 'Pizza' ---" << endl;
    vector<Restaurant*> pizzaRestaurants = manager.searchRestaurants("Pizza");
    for (Restaurant* restaurant : pizzaRestaurants) {
        restaurant->displayRestaurantInfo();
    }
    
    // Demo 3: Display menu of a restaurant
    cout << "\n--- Pizza Palace Menu ---" << endl;
    Restaurant* pizzaPalace = manager.getRestaurantById(1);
    if (pizzaPalace) {
        pizzaPalace->getMenu()->displayMenu();
    }
    
    // Demo 4: Create and place an order
    cout << "\n--- Creating Order ---" << endl;
    Order* order1 = manager.createOrder(1, 1); // User 1 ordering from Restaurant 1
    
    if (order1) {
        // Add items to order
        MenuItem* margherita = pizzaPalace->getMenu()->getMenuItemById(101);
        MenuItem* garlicBread = pizzaPalace->getMenu()->getMenuItemById(103);
        
        order1->addItem(margherita, 2);
        order1->addItem(garlicBread, 1);
        
        // Display order details
        order1->displayOrderDetails();
        
        // Place order
        cout << "\n--- Placing Order ---" << endl;
        if (manager.placeOrder(order1, PaymentMode::Wallet)) {
            cout << "Order placed successfully!" << endl;
        } else {
            cout << "Failed to place order!" << endl;
        }
    }
    
    // Demo 5: Display user profile and order history
    cout << "\n--- User Profile ---" << endl;
    User* user1 = manager.getUserById(1);
    if (user1) {
        user1->displayUserInfo();
    }
    
    manager.displayOrderHistory(1);
    
    // Demo 6: Simulate order status updates
    cout << "\n--- Order Status Updates ---" << endl;
    DeliveryService* deliveryService = manager.getDeliveryService();
    
    if (order1) {
        deliveryService->updateOrderStatus(order1->getOrderId(), OrderStatus::OutForDelivery);
        deliveryService->updateOrderStatus(order1->getOrderId(), OrderStatus::Delivered);
        
        // Display final order details
        order1->displayOrderDetails();
    }
    
    // Demo 7: Search by cuisine
    cout << "\n--- Indian Restaurants ---" << endl;
    vector<Restaurant*> indianRestaurants = manager.searchByCuisine("Indian");
    for (Restaurant* restaurant : indianRestaurants) {
        cout << restaurant->getName() << " - " << restaurant->getCuisine() << endl;
    }
    
    // Demo 8: Search nearby restaurants
    cout << "\n--- Nearby Restaurants (within 5 km) ---" << endl;
    Location userLocation(28.7041, 77.1025, "User Location", "Delhi", "110001");
    vector<Restaurant*> nearbyRestaurants = manager.searchNearby(userLocation, 5.0);
    for (Restaurant* restaurant : nearbyRestaurants) {
        double distance = restaurant->calculateDeliveryDistance(userLocation);
        cout << restaurant->getName() << " - Distance: " << distance << " km" << endl;
    }
    
    cout << "\n=== Zomato Demo Completed ===" << endl;
    
    return 0;
}





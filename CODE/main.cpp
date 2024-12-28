#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <algorithm>
#include <sstream>

// Builder: Room
class Room {
public:
    std::string name;
    int capacity;

    Room(const std::string& name, int capacity) : name(name), capacity(capacity) {}
};

// Forward Declaration of Insect Class
class Insect {
public:
    virtual void show_attributes() = 0;
    virtual ~Insect() {}
};

// Builder: Colony
class Colony {
    std::string colony_type;
    int pos_x, pos_y;
    int food_stock;
    bool queen_alive;
    std::vector<std::shared_ptr<Room>> rooms;
    std::vector<std::shared_ptr<Insect>> insect_list;

public:
    Colony(const std::string& type, int x, int y) : colony_type(type), pos_x(x), pos_y(y), food_stock(100), queen_alive(true) {}

    void add_room(const std::string& name, int capacity) {
        rooms.push_back(std::make_shared<Room>(name, capacity));
    }

    void add_insect(std::shared_ptr<Insect> insect) {
        insect_list.push_back(insect);
    }

    void receive_resources(const std::string& resource_type, int quantity) {
        if (resource_type == "food") {
            food_stock += quantity;
            std::cout << "Food stock increased by " << quantity << ". Total: " << food_stock << std::endl;
        } else {
            std::cout << "Unknown resource type: " << resource_type << std::endl;
        }
    }

    void process_tick() {
        if (queen_alive) {
            food_stock -= 5; // Basic food consumption
            if (food_stock <= 0) {
                queen_alive = false;
                std::cout << "The queen of this colony has perished due to starvation." << std::endl;
            }
        }
    }

    void display_details() {
        std::cout << "Type: " << colony_type << "\n";
        std::cout << "Location: (" << pos_x << ", " << pos_y << ")\n";
        std::cout << "Food Stock: " << food_stock << "\n";
        std::cout << "Queen Alive: " << (queen_alive ? "Yes" : "No") << "\n";
        std::cout << "Rooms: " << rooms.size() << "\n";
        std::cout << "Insects: " << insect_list.size() << "\n";
    }
};

// Singleton: Meadow Class
class Meadow {
private:
    static Meadow* meadow_instance;
    std::vector<std::shared_ptr<Colony>> colony_list;
    Meadow() {}

public:
    static Meadow* get_instance() {
        if (!meadow_instance) {
            meadow_instance = new Meadow();
        }
        return meadow_instance;
    }

    void spawn_colony(const std::string& type, int pos_x, int pos_y) {
        auto colony = std::make_shared<Colony>(type, pos_x, pos_y);
        colony_list.push_back(colony);
        std::cout << "Colony of type " << type << " created at (" << pos_x << ", " << pos_y << ").\n";
    }

    void allocate_resources(int colony_id, const std::string& resource_type, int quantity) {
        if (colony_id >= 0 && colony_id < colony_list.size()) {
            colony_list[colony_id]->receive_resources(resource_type, quantity);
        } else {
            std::cout << "Invalid colony ID." << std::endl;
        }
    }

    void execute_ticks(int tick_count) {
        for (int i = 0; i < tick_count; ++i) {
            std::cout << "Processing Tick: " << i + 1 << std::endl;
            for (auto& colony : colony_list) {
                colony->process_tick();
            }
        }
    }

    void display_summary(int colony_id) {
        if (colony_id >= 0 && colony_id < colony_list.size()) {
            colony_list[colony_id]->display_details();
        } else {
            std::cout << "Invalid colony ID." << std::endl;
        }
    }
};

Meadow* Meadow::meadow_instance = nullptr;

// Decorator: Insect Attributes
class BasicInsect : public Insect {
public:
    void show_attributes() override {
        std::cout << "Basic Insect Attributes" << std::endl;
    }
};

class PowerfulInsect : public Insect {
    std::shared_ptr<Insect> base_insect;

public:
    PowerfulInsect(std::shared_ptr<Insect> insect) : base_insect(insect) {}

    void show_attributes() override {
        base_insect->show_attributes();
        std::cout << " + Powerful" << std::endl;
    }
};

class ResourcefulInsect : public Insect {
    std::shared_ptr<Insect> base_insect;

public:
    ResourcefulInsect(std::shared_ptr<Insect> insect) : base_insect(insect) {}

    void show_attributes() override {
        base_insect->show_attributes();
        std::cout << " + Resourceful" << std::endl;
    }
};

// Factory: Create Insects
class InsectFactory {
public:
    enum InsectCategory { Worker, Fighter };

    static std::shared_ptr<Insect> generate_insect(InsectCategory category) {
        if (category == Worker) {
            return std::make_shared<BasicInsect>();
        } else if (category == Fighter) {
            return std::make_shared<PowerfulInsect>(std::make_shared<BasicInsect>());
        }
        return nullptr;
    }
};

// Main Function
int main() {
    Meadow* meadow = Meadow::get_instance();

    // CLI Interface
    std::string user_input;
    while (true) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, user_input);

        std::istringstream input_stream(user_input);
        std::string command;
        input_stream >> command;

        if (command == "quit") {
            break;
        }

        int x, y, id, amount;
        std::string resource, type;

        if (command == "spawn") {
            input_stream >> x >> y >> type;
            meadow->spawn_colony(type, x, y);
        } else if (command == "give") {
            input_stream >> id >> resource >> amount;
            meadow->allocate_resources(id, resource, amount);
        } else if (command == "tick") {
            input_stream >> amount;
            amount = amount > 0 ? amount : 1;
            meadow->execute_ticks(amount);
        } else if (command == "summary") {
            input_stream >> id;
            meadow->display_summary(id);
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}

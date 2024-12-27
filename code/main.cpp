// Colony Simulation in C++
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <algorithm>
#include <sstream>


// Singleton: Environment Class
class Environment
{
private:

    static Environment* env_instance;
    std::vector<std::shared_ptr<class Colony>> colony_list;
    Environment() {}

public:
    static Environment* get_instance() {
        if (!env_instance) {
            env_instance = new Environment();
        }
        return env_instance;
    }

    void create_colony(const std::string& type, int pos_x, int pos_y)
    {
        auto colony = std::make_shared<Colony>(type, pos_x, pos_y);
        colony_list.push_back(colony);
        std::cout << "Colony of type " << type << " created at (" << pos_x << ", " << pos_y << ").\n";
    }

    void allocate_resources(int colony_id, const std::string& resource_type, int quantity)
    {
        if (colony_id >= 0 && colony_id < colony_list.size()) {
            std::cout << "Allocated " << quantity << " " << resource_type << " to colony " << colony_id << ".\n";
            // Logic for resource allocation to the specified colony.
        } else {
            std::cout << "Invalid colony ID." << std::endl;
        }
    }

    void execute_ticks(int tick_count);
    void display_summary(int colony_id);
};

Environment* Environment::env_instance = nullptr;

// Decorator: Insect Attributes
class Insect 

{
public:
    virtual void show_attributes() = 0;
    virtual ~Insect() {}
};

class BasicInsect : public Insect 

{
public:
    void show_attributes() override {
        std::cout << "Basic Insect Attributes" << std::endl;
    }
};

class PowerfulInsect : public Insect

{
    std::shared_ptr<Insect> base_insect;

public:
    PowerfulInsect(std::shared_ptr<Insect> insect) : base_insect(insect) {}

    void show_attributes() override {
        base_insect->show_attributes();
        std::cout << " + Powerful" << std::endl;
    }
};

class ResourcefulInsect : public Insect 

{
    std::shared_ptr<Insect> base_insect;

public:
    ResourcefulInsect(std::shared_ptr<Insect> insect) : base_insect(insect) {}

    void show_attributes() override {
        base_insect->show_attributes();
        std::cout << " + Resourceful" << std::endl;
    }
};

// Factory: Create Insects
class InsectFactory

{
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

// Builder: Colony
class Colony

{
    std::string colony_type;
    int pos_x, pos_y;
    std::vector<std::string> sections;
    std::vector<std::shared_ptr<Insect>> insect_list;

public:
    Colony(const std::string& type, int x, int y) : colony_type(type), pos_x(x), pos_y(y) {}

    void add_section(const std::string& section_name) {
        sections.push_back(section_name);
    }

    void add_insect(std::shared_ptr<Insect> insect) {
        insect_list.push_back(insect);
    }

    void list_sections() {
        for (const auto& section : sections) {
            std::cout << section << std::endl;
        }
    }

    void conflict(std::shared_ptr<Colony> other_colony);
    void rejuvenate();

    void display_details() {
        std::cout << "Type: " << colony_type << "\n";
        std::cout << "Location: (" << pos_x << ", " << pos_y << ")\n";
        std::cout << "Sections: " << sections.size() << "\n";
        std::cout << "Insects: " << insect_list.size() << "\n";
    }
};

// Mediator: Tick Handler
class TickHandler 

{
    std::vector<std::shared_ptr<Colony>> colony_network;

public:
    void register_colony(std::shared_ptr<Colony> colony) {
        colony_network.push_back(colony);
    }

    void process_ticks(int tick_count) {
        for (int t = 0; t < tick_count; ++t) {
            std::cout << "Processing Tick: " << t + 1 << std::endl;
            for (auto& colony : colony_network) {
                colony->rejuvenate();
            }
        }
    }
};

// Main Function
int main() 

{
    Environment* environment = Environment::get_instance();

    // Create Colonies
    environment->create_colony("Aggressive", 0, 0);
    environment->create_colony("Harvester", 10, 20);

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

        switch (command[0]) {
        case 'c': // create
            input_stream >> x >> y >> type;
            environment->create_colony(type, x, y);
            break;

        case 'r': // resource
            input_stream >> id >> resource >> amount;
            environment->allocate_resources(id, resource, amount);
            break;

        case 't': // tick
            input_stream >> amount;
            amount = amount > 0 ? amount : 1;
            std::cout << "Simulating " << amount << " ticks.\n";
            break;

        case 's': // summary
            input_stream >> id;
            std::cout << "Displaying summary for colony " << id << ".\n";
            break;

        default:
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}
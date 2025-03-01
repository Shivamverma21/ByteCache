#include <iostream>
#include "Storage.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Storage storage;
    storage.create_key("name", "Alice");
    storage.read_key("name");
    storage.update_key("name", "Bob");
    storage.read_key("name");
    storage.delete_key("name");
    storage.read_key("name");
    return 0;
}

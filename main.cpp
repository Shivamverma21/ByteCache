#include <iostream>
#include "storage.h"
#include "server.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    storage storage;
    storage.create_key("name", "Alice");
    storage.read_key("name");
    storage.update_key("name", "Bob");
    storage.read_key("name");
    storage.delete_key("name");
    storage.read_key("name");
    int port = 12345;
    StartServer(port);
    return 0;
}

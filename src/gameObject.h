#include <component.h>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

class gameObject {
public:
    string name;
    vector<Component> components;
    unordered_set<string> tags;

    gameObject() : id(generateUniqueId()) {}
private:
    const int id;

    static int lastId;

    static int generateUniqueId() {
        return ++lastId;
    }
};

int gameObject::lastId = 0;
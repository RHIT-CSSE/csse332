#include <memory>

using namespace std;

class ExampleObj {
public:
    int data[100];
    shared_ptr<ExampleObj> next;
    
    ~ExampleObj() {
        printf("ExampleObj memory freed\n");
    }
};


int main(int argc, char** argv) {

    shared_ptr<ExampleObj> var1 = make_shared<ExampleObj>();
    shared_ptr<ExampleObj> var2 = var1;

    printf("Setting var1 to null\n");
    var1 = nullptr;
    //ExampleObj should not be deleted yet because var2 still exists
    var2->data[17] = 5;
    printf("Setting var2 to null\n");
    var2 = nullptr;

    printf("Activity 1\n");
    
    shared_ptr<ExampleObj> holder = make_shared<ExampleObj>();
    holder->next = make_shared<ExampleObj>();
    holder = nullptr;

    printf("Activity 2\n");
    
    shared_ptr<ExampleObj> a = make_shared<ExampleObj>();
    shared_ptr<ExampleObj> b = make_shared<ExampleObj>();
    shared_ptr<ExampleObj> c = make_shared<ExampleObj>();
    a->next = b;
    b->next = c;
    c->next = a;
    b = nullptr;
    c = nullptr;
    // in a better world, this would free all three
    a = nullptr;


}

#include <memory>

/*
 Compile this code like this:

 g++ -ggdb shared_pointer.cpp -o shared_pointer

 */

using namespace std;

class ExampleObj {
public:
    int data[100];
    ExampleObj *next;
    
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
    
    ExampleObj* holder = new ExampleObj();
    holder->next = new ExampleObj();
    holder = nullptr;

    printf("Activity 2\n");
    
    // write your solution here

}

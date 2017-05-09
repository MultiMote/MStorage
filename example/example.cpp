#include "MStorage.h"
#include <cstdio>
#include <stdint.h>

void print_fieds(MStorage *mst) {
    printf("someString: %s\n", mst->getString("someString").c_str());
    printf("someInt: %d\n", mst->getInt("someInt"));
    printf("someByte: 0x%x\n", mst->getByte("someByte"));
    
    printf("someGroup: \n");
    MStorage* group = mst->getGroup("someGroup");
    printf("    someDouble: %.9f\n", group->getDouble("someDouble"));
    //or mst->getGroup("someGroup")->getDouble("someDouble")

    int32_t* arr;
    uint16_t arr_len;
    arr = group->getIntArray("someIntArray", &arr_len);
    if (arr) {
        printf("    someIntArray: ");
        for (int i = 0; i < arr_len; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}


int main(int argc, char const *argv[]) {
    MStorage *storage;

    printf("\n## write test ##\n");
    storage = new MStorage();

    storage->setString("someString", "Hello world!");
    storage->setInt("someInt", 123456);
    storage->setByte("someByte", 0xA4);

    MStorage *group = new MStorage(); // you can create groups in groups too
        group->setDouble("someDouble", 3.141592653);
        int32_t _ints[] = {42, 34, 32323, 5573453};
        group->setIntArray("someIntArray", _ints, 4); // array is being copied

    MStorage *groupToMerge = new MStorage();
        groupToMerge->setDouble("someDouble2", 1.23);

    group->merge(groupToMerge, false);

    storage->setGroup("someGroup", group);

    print_fieds(storage);
    // std::cout << *storage << std::endl;

    storage->writeToFile("data.mst");
    delete storage; // subfields are deleted automatically


    printf("\n## read test ##\n");
    storage = new MStorage();
    storage->readFromFile("data.mst");
    print_fieds(storage);
    delete storage;
    
    return 0;
}

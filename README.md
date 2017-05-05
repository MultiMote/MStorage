MStorage - simple multi-type map storage for some fixed data types
==================================================================

Absolutely no warranty. Use at your own risk.
Written for one small game.

### Supported types:
- 32-bit Signed Integer
- 32-bit Signed Integer Array
- 8-bit Unsigned Integer (Byte)
- 8-bit Unsigned Integer (Byte) Array
- Double
- String (std::string)

### Features:
- Values and groups are associated with keys.
- Grouping. All of types listed above can be grouped.
- Base types are casted automatically to each other.
- Writing to file / reading from file.
- Writing and reading from stream.



## Example usage:

### Assigning and writing to file:

```c++
    MStorage mst;
    mst.setInt("someInt", 123456);
    mst.writeToFile("data.mst");
```

### Reading:

```c++
    MStorage mst;
    mst.readFromFile("data.mst");
    int32_t val = mst.getInt("someInt");   
```

### Groups:

```c++
    MStorage mst;
    MStorage *group = new MStorage();
    group->setString("str", "Hello");
    mst.setGroup("someGroup", group);  
 
    std::string str = mst.getGroup("someGroup")->getString("str");
```
### Iterating:

```c++
    MValueMapIterator it = mst.begin();
    while(it != mst.end()) {
        MStorageBase *val = it->second;
        // do something
        // type can be discovered via val->getType()
        it++;
    }  
```
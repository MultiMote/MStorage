#ifndef MSTORAGE_H
#define MSTORAGE_H

#include <stdint.h>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <iomanip>
#include <map>

#define MST_HEADER {'M', 'S', 'T', 0xFA, 0x33}
#define MST_HEADER_LEN 5

static const unsigned char _mst_header[MST_HEADER_LEN] = MST_HEADER;

enum MStorageDataType {
    MDT_INT32 = 0x01,
    MDT_DOUBLE = 0x02,
    MDT_BYTE = 0x03,
    MDT_STRING = 0x04,
    MDT_INT32_ARRAY = 0x05,
    MDT_BYTE_ARRAY = 0x06,
    MDT_GROUP = 0xAF
};

class IMStorageBase {
public:
    virtual ~IMStorageBase() {}
    virtual MStorageDataType getType() = 0;
    virtual void writeData(std::ostream &of) = 0;
    virtual void readData(std::istream &ifs) = 0;
    virtual uint16_t getLength() = 0;
};

class IMStorageBasicType : public IMStorageBase {
public:
    virtual uint8_t getByte() = 0;
    virtual double getDouble() = 0;
    virtual int32_t getInt() = 0;
};

class MStorageInt32 : public IMStorageBasicType {
public:
    MStorageInt32(int32_t val);
    MStorageDataType getType();
    uint8_t getByte();
    double getDouble();
    int32_t getInt();
    void writeData(std::ostream &of);
    void readData(std::istream &ifs);
    uint16_t getLength();
private:
    int32_t val;
};

class MStorageDouble : public IMStorageBasicType {
public:
    MStorageDouble(double val);
    MStorageDataType getType();
    uint8_t getByte();
    double getDouble();
    int32_t getInt();
    void writeData(std::ostream &of);
    void readData(std::istream &ifs);
    uint16_t getLength();
private:
    double val;
};

class MStorageByte : public IMStorageBasicType {
public:
    MStorageByte(int8_t val);
    MStorageDataType getType();
    uint8_t getByte();
    double getDouble();
    int32_t getInt();
    void writeData(std::ostream &of);
    void readData(std::istream &ifs);
    uint16_t getLength();
private:
    int8_t val;
};

class MStorageString : public IMStorageBase {
public:
    MStorageString(std::string val);
    MStorageDataType getType();
    std::string getString();
    void writeData(std::ostream &of);
    void readData(std::istream &ifs);
    uint16_t getLength();
private:
    std::string val;
};

class MStorageInt32Array : public IMStorageBase {
public:
    MStorageInt32Array(int32_t *data, uint16_t len);
    virtual ~MStorageInt32Array();

    MStorageDataType getType();
    int32_t * getIntArray();
    uint16_t getLength();
    uint16_t getArrayLength();
    void writeData(std::ostream &of);
    void readData(std::istream &ifs);
private:
    int32_t *val;
    uint16_t val_len;
};

class MStorageByteArray : public IMStorageBase {
public:
    MStorageByteArray(uint8_t *data, uint16_t len);
    virtual ~MStorageByteArray();
    MStorageDataType getType();
    uint8_t * getByteArray();
    uint16_t getLength();
    uint16_t getArrayLength();
    void writeData(std::ostream &of);
    void readData(std::istream &ifs);
private:
    uint8_t *val;
    uint16_t val_len;
};


typedef std::map<const std::string, IMStorageBase*> MValueMap;
typedef MValueMap::iterator MValueMapIterator;

class MStorage : public IMStorageBase {
public:
    MStorage();
    virtual ~MStorage();

    int8_t writeToFile(std::string fname);
    int8_t readFromFile(std::string fname);

    void insertRaw(const std::string &key, IMStorageBase *val);
    IMStorageBase* getRaw(const std::string &key);

    void setInt(const std::string &key, uint32_t val);
    void setIntArray(const std::string &key, int32_t *val, uint16_t len);
    void setByteArray(const std::string &key, uint8_t *val, uint16_t len);
    void setDouble(const std::string &key, double val);
    void setByte(const std::string &key, uint8_t val);
    void setString(const std::string &key, std::string val);
    void setGroup(const std::string &key, MStorage *group);
    void merge(MStorage *group, bool replaceValues);

    int32_t getInt(const std::string &key);
    int32_t* getIntArray(const std::string &key, uint16_t *len);
    uint8_t* getByteArray(const std::string &key, uint16_t *len);
    double getDouble(const std::string &key);
    uint8_t getByte(const std::string &key);
    std::string getString(const std::string &key);
    MStorage* getGroup(const std::string &key);

    bool containsKey(const std::string &key);

    MValueMapIterator begin();
    MValueMapIterator end();
    void clear();
    void erase(const std::string &key);
    void dbgPrintKeys();

private:
    static IMStorageBase* createByID(uint8_t id);
    IMStorageBasicType* getBasicType(const std::string &key);
    MValueMap values;

    // IMStorageBase
public:
    MStorageDataType getType();
    void writeData(std::ostream &of);
    void readData(std::istream &ifs);
    uint16_t getLength();
};

std::ostream &operator<<(std::ostream &os, IMStorageBase &st);

#endif // MSTORAGE_H

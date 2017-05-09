#include "MStorage.h"

///////////////////////////////////////////////////////////////////////

MStorageInt32::MStorageInt32(int32_t val) {
    this->val = val;
}

MStorageDataType MStorageInt32::getType() {
    return MDT_INT32;
}

uint8_t MStorageInt32::getByte() {
    return val & 0xFF;
}

double MStorageInt32::getDouble() {
    return (double)val;
}

int32_t MStorageInt32::getInt() {
    return val;
}

void MStorageInt32::writeData(std::ostream &of) {
    of.put((char)getType());
    of.write((char*)&val , sizeof(int32_t));
}

void MStorageInt32::readData(std::istream &ifs) {
    ifs.read((char*)&val, sizeof(int32_t));
}

uint16_t MStorageInt32::getLength() {
    return sizeof(int32_t);
}

///////////////////////////////////////////////////////////////////////

MStorageDouble::MStorageDouble(double val) {
    this->val = val;
}

MStorageDataType MStorageDouble::getType() {
    return MDT_DOUBLE;
}

uint8_t MStorageDouble::getByte() {
    return ((int32_t)val) & 0xFF;
}

double MStorageDouble::getDouble() {
    return val;
}

int32_t MStorageDouble::getInt() {
    return (int32_t)val;
}

void MStorageDouble::writeData(std::ostream &of) {
    of.put((char)getType());
    of.write((char*)&val , sizeof(double));
}

void MStorageDouble::readData(std::istream &ifs) {
    ifs.read((char*)&val, sizeof(double));
}

uint16_t MStorageDouble::getLength() {
    return sizeof(double);
}

///////////////////////////////////////////////////////////////////////

MStorageByte::MStorageByte(int8_t val) {
    this->val = val;
}

MStorageDataType MStorageByte::getType() {
    return MDT_BYTE;
}

uint8_t MStorageByte::getByte() {
    return val;
}

double MStorageByte::getDouble() {
    return (double) val;
}

int32_t MStorageByte::getInt() {
    return val & 0xFF;
}

void MStorageByte::writeData(std::ostream &of) {
    of.put((char)getType());
    of.put((char)val);
}

void MStorageByte::readData(std::istream &ifs) {
    ifs.read((char*)&val, 1);
}

uint16_t MStorageByte::getLength() {
    return 1;
}

///////////////////////////////////////////////////////////////////////

MStorageString::MStorageString(std::string val) {
    this->val = val;
}

MStorageDataType MStorageString::getType() {
    return MDT_STRING;
}

std::string MStorageString::getString() {
    return val;
}

void MStorageString::writeData(std::ostream &of) {
    of.put((char)getType());
    uint16_t slen = val.size();
    of.write((char*)&slen, sizeof(uint16_t));
    for(size_t pos = 0; pos < slen; ++pos) {
        of << val.at(pos);
    }
}

void MStorageString::readData(std::istream &ifs) {
    uint16_t slen;
    ifs.read((char*)&slen, sizeof(uint16_t));
    char* str = new char[slen];
    ifs.read(str, slen);
    val = std::string(str, slen);
    delete[] str;
}

uint16_t MStorageString::getLength() {
    return val.size() + sizeof(uint16_t);
}

///////////////////////////////////////////////////////////////////////

MStorageInt32Array::MStorageInt32Array(int32_t *data, uint16_t len) {
    if(!data || len == 0) {
        val_len = 0;
        val = NULL;
        return;
    }
    val_len = len;
    val = new int32_t[val_len];
    memcpy(val, data, sizeof(uint32_t) * val_len);
}

MStorageInt32Array::~MStorageInt32Array() {
    delete[] val;
}

MStorageDataType MStorageInt32Array::getType() {
    return MDT_INT32_ARRAY;
}

int32_t *MStorageInt32Array::getIntArray() {
    return val;
}

uint16_t MStorageInt32Array::getLength() {
    return sizeof(int32_t) * val_len + sizeof(uint16_t);
}

uint16_t MStorageInt32Array::getArrayLength() {
    return val_len;
}

void MStorageInt32Array::writeData(std::ostream &of) {
    of.put((char)getType());
    of.write((char*)&val_len, sizeof(uint16_t));
    for(size_t pos = 0; pos < val_len; ++pos) {
        of.write((char*)&val[pos] , sizeof(int32_t));
    }
}

void MStorageInt32Array::readData(std::istream &ifs) {
    if(val)
        delete[] val;

    ifs.read((char*)&val_len, sizeof(uint16_t));
    val = new int32_t[val_len];
    ifs.read((char*)val, sizeof(int32_t) * val_len);
}

///////////////////////////////////////////////////////////////////////

MStorageByteArray::MStorageByteArray(uint8_t *data, uint16_t len) {
    if(!data || len == 0) {
        val_len = 0;
        val = NULL;
        return;
    }
    val_len = len;
    val = new uint8_t[val_len];
    memcpy(val, data, val_len);
}

MStorageByteArray::~MStorageByteArray() {
    delete [] val;
}

MStorageDataType MStorageByteArray::getType() {
    return MDT_BYTE_ARRAY;
}

uint8_t *MStorageByteArray::getByteArray() {
    return val;
}

uint16_t MStorageByteArray::getLength() {
    return val_len + sizeof(uint16_t);
}

uint16_t MStorageByteArray::getArrayLength() {
    return val_len;
}

void MStorageByteArray::writeData(std::ostream &of) {
    of.put((char)getType());
    of.write((char*)&val_len, sizeof(uint16_t));
    of.write((char*)val, val_len);
}

void MStorageByteArray::readData(std::istream &ifs) {
    if(val)
        delete[] val;
    ifs.read((char*)&val_len, sizeof(uint16_t));
    val = new uint8_t[val_len];
    ifs.read((char*)val, val_len);
}

///////////////////////////////////////////////////////////////////////

MStorage::MStorage() {

}

MStorage::~MStorage() {
    clear();
}

int8_t MStorage::writeToFile(std::string fname) {
    std::ofstream of;
    of.open(fname.c_str(), std::ios::binary | std::ios::out);
    if(!of.is_open()) {
        fprintf(stderr, "MStorage::%s(): Unable to open %s for writing!\n", __FUNCTION__, fname.c_str());
        return 1;
    }
    of.write((char*)_mst_header, MST_HEADER_LEN);
    writeData(of);
    of.close();
    return 0;
}

int8_t MStorage::readFromFile(std::string fname) {
    std::ifstream ifs;
    unsigned char mst_header[MST_HEADER_LEN];

    ifs.open(fname.c_str(), std::ios::binary | std::ios::in);
    if(!ifs.is_open()) {
        fprintf(stderr, "MStorage::%s(): Unable to open %s for reading!\n", __FUNCTION__, fname.c_str());
        return 1;
    }
    ifs.read((char*)mst_header, MST_HEADER_LEN);

    if (memcmp(mst_header, _mst_header, MST_HEADER_LEN) != 0) {
        fprintf(stderr, "MStorage::%s(): File header is wrong!\n", __FUNCTION__);
        ifs.close();
        return 2;
    } else {
        values.clear();
        uint8_t selfDtype = 0;
        ifs.read((char*)&selfDtype, 1);
        if(selfDtype != MDT_GROUP) {
            fprintf(stderr, "MStorage::%s(): Wrong group data type"
                    "(0x%02x)!\n", __FUNCTION__, selfDtype);
            ifs.close();
            return 3;
        }
        readData(ifs);
        ifs.close();
        return 0;
    }
    ifs.close();
    return 4;
}

void MStorage::insertRaw(const std::string &key, IMStorageBase *val) {
    size_t klen = key.size();
    if(klen > 0xFF) {
        fprintf(stderr, "MStorage::%s(): Key name is too long (%u), skipping\n", __FUNCTION__, klen);
        return;
    }
    MValueMapIterator it = values.find(key);
    if(it == end()) {
        values[key] = val;
    } else { // replacing
        if(it->second)
            delete it->second;
        it->second = val;
//        fprintf(stderr, "MStorage::%s(): Value with key \"%s\" is already exists!\n",
//                        __FUNCTION__, key.c_str());
    }

}

IMStorageBase *MStorage::getRaw(const std::string &key) {
    if(!containsKey(key)) {
        fprintf(stderr, "MStorage::%s(): Value with key \"%s\" not found!\n",
                __FUNCTION__, key.c_str());
        return NULL;
    } else {
        return values[key];
    }
}

void MStorage::setInt(const std::string &key, uint32_t val) {
    insertRaw(key, new MStorageInt32(val));
}

void MStorage::setIntArray(const std::string &key, int32_t *val, uint16_t len) {
    insertRaw(key, new MStorageInt32Array(val, len));
}
void MStorage::setByteArray(const std::string &key, uint8_t *val, uint16_t len) {
    insertRaw(key, new MStorageByteArray(val, len));
}

void MStorage::setDouble(const std::string &key, double val) {
    insertRaw(key, new MStorageDouble(val));
}

void MStorage::setByte(const std::string &key, uint8_t val) {
    insertRaw(key, new MStorageByte(val));
}

void MStorage::setString(const std::string &key, std::string val) {
    insertRaw(key, new MStorageString(val));
}

void MStorage::setGroup(const std::string &key, MStorage *group) {
    insertRaw(key, group);
}

void MStorage::merge(MStorage *group, bool replaceValues) {
    MValueMapIterator it = group->begin();
    while(it != group->end()) {
        if(replaceValues || !containsKey(it->first)) {
            insertRaw(it->first, it->second);
        }
        it++;
    }
}

int32_t MStorage::getInt(const std::string &key) {
    IMStorageBasicType *bval = getBasicType(key);
    return bval ? bval->getInt() : 0;
}

int32_t *MStorage::getIntArray(const std::string &key, uint16_t *len) {
    IMStorageBase *bval = getRaw(key);
    if(!bval) {
        return NULL;
    }
    if(bval->getType() != MDT_INT32_ARRAY) {
        fprintf(stderr, "MStorage::%s(): Value is not INT32 ARRAY!\n", __FUNCTION__);
        return NULL;
    }

    MStorageInt32Array *sval = static_cast<MStorageInt32Array*>(bval);
    if(len)
        *len = sval->getArrayLength();
    return sval->getIntArray();
}

uint8_t *MStorage::getByteArray(const std::string &key, uint16_t *len) {
    IMStorageBase *bval = getRaw(key);
    if(!bval) {
        return NULL;
    }
    if(bval->getType() != MDT_BYTE_ARRAY) {
        fprintf(stderr, "MStorage::%s(): Value is not BYTE ARRAY!\n", __FUNCTION__);
        return NULL;
    }

    MStorageByteArray *sval = static_cast<MStorageByteArray*>(bval);
    if(len)
        *len = sval->getArrayLength();
    return sval->getByteArray();
}

double MStorage::getDouble(const std::string &key) {
    IMStorageBasicType *bval = getBasicType(key);
    return bval ? bval->getDouble() : 0;
}

uint8_t MStorage::getByte(const std::string &key) {
    IMStorageBasicType *bval = getBasicType(key);
    return bval ? bval->getByte() : 0;
}

std::string MStorage::getString(const std::string &key) {
    IMStorageBase *bval = getRaw(key);
    if(!bval) {
        return "";
    }
    if(bval->getType() != MDT_STRING) {
        fprintf(stderr, "MStorage::%s(): Value is not STRING!\n", __FUNCTION__);
        return "";
    }
    MStorageString *sval = static_cast<MStorageString*>(bval);
    return sval->getString();
}

MStorage *MStorage::getGroup(const std::string &key) {
    IMStorageBase *bval = getRaw(key);
    if(!bval) {
        return NULL;
    }
    if(bval->getType() != MDT_GROUP) {
        fprintf(stderr, "MStorage::%s(): Value is not GROUP!\n", __FUNCTION__);
        return NULL;
    }

    MStorage *sval = static_cast<MStorage*>(bval);
    return sval;
}

bool MStorage::containsKey(const std::string &key) {
    return values.find(key) != end();
}

MValueMapIterator MStorage::begin() {
    return values.begin();
}

MValueMapIterator MStorage::end() {
    return values.end();
}

void MStorage::clear() {
    MValueMapIterator it = begin();
    while(it != end()) {
        delete it->second;
        it++;
    }
    values.clear();
}

void MStorage::erase(const std::string &key) {
    IMStorageBase *bval = getRaw(key);
    if(bval) {
        values.erase(key);
        delete bval;
    }
}

void MStorage::dbgPrintKeys() {
    MValueMapIterator it = begin();
    printf("MStorage: [ ");
    while(it != end()) {
        printf("%s ", it->first.c_str());
        it++;
    }
    printf("]\n");
}

IMStorageBase *MStorage::createByID(uint8_t id) {
    switch(id) {
    case MDT_INT32:
        return new MStorageInt32(0);
    case MDT_DOUBLE:
        return new MStorageDouble(0);
    case MDT_BYTE:
        return new MStorageByte(0);
    case MDT_STRING:
        return new MStorageString("");
    case MDT_INT32_ARRAY:
        return new MStorageInt32Array(NULL, 0);
    case MDT_BYTE_ARRAY:
        return new MStorageByteArray(NULL, 0);
    case MDT_GROUP:
        return new MStorage();
    default:
        fprintf(stderr, "MStorage::%s(): "
                "Unknown data type (0x%02x)!\n", __FUNCTION__, id);
        return NULL;

    }
}

IMStorageBasicType *MStorage::getBasicType(const std::string &key) {
    IMStorageBasicType *bval = dynamic_cast<IMStorageBasicType*>(getRaw(key));
    if(!bval) {
        fprintf(stderr, "MStorage::%s(): Unable to get Basic type!\n", __FUNCTION__);
        return NULL;
    }
    return bval;
}

MStorageDataType MStorage::getType() {
    return MDT_GROUP;
}

void MStorage::writeData(std::ostream &of) {
    MValueMapIterator it = begin();
    uint16_t datalen = getLength();
    of.put((char)getType());
    of.write((char*)&datalen, sizeof(uint16_t));

    while(it != end()) {
        size_t klen = it->first.size();
        of.put((char)(klen & 0xFF));
        for(size_t pos = 0; pos < klen; ++pos) {
            of << it->first.at(pos);
        }
        it->second->writeData(of);

        it++;
    }
}

void MStorage::readData(std::istream &ifs) {
    uint16_t blockSize = 0;
    uint16_t bytesRead = 0;

    ifs.read((char*)&blockSize, sizeof(uint16_t));


    while (!ifs.eof() && (bytesRead < blockSize)) {
        uint8_t klen;
        ifs.read((char*)&klen, 1); // key length
        bytesRead += ifs.gcount();

        char* key = new char[klen];
        ifs.read(key, klen); // key
        bytesRead += ifs.gcount();

        std::string skey(key, klen);
        uint8_t dtype;
        ifs.read((char*)&dtype, 1); // data type
        bytesRead += ifs.gcount();

        IMStorageBase * data = createByID(dtype);
        data->readData(ifs); // data
        bytesRead += data->getLength();

        insertRaw(skey, data);

        delete[] key;
    }

    if(bytesRead != blockSize) {
        fprintf(stderr, "MStorage::%s(): Group block size missmatch "
                "(0x%04x instead of 0x%04x)!\n", __FUNCTION__, bytesRead, blockSize);
    }

}

uint16_t MStorage::getLength() {
    uint16_t sz = 0;
    MValueMapIterator it = begin();
    //sz+=1; // self data type
    while(it != end()) {
        sz += 1; // key len byte
        sz += it->first.size(); // key characters
        sz += 1; // data type byte
        sz += it->second->getLength(); // data len
        it++;
    }
    return sz;
}

///////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &os, IMStorageBase &st) {
    switch (st.getType()) {
    case MDT_INT32:
        return os << "I32:" << static_cast<MStorageInt32&>(st).getInt();
    case MDT_DOUBLE:
        return os << "D:" << static_cast<MStorageDouble&>(st).getDouble();
    case MDT_BYTE:
        os << "B:";
        os << std::hex << std::setiosflags(std::ios::showbase);
        os << static_cast<MStorageByte&>(st).getInt();
        os << std::dec << std::resetiosflags (std::ios::showbase);
        return os;
    case MDT_STRING:
        return os << "STR:\"" << static_cast<MStorageString&>(st).getString() << "\"";
    case MDT_INT32_ARRAY: {
        MStorageInt32Array& msia = static_cast<MStorageInt32Array&>(st);
        int32_t* msia_arr = msia.getIntArray();
        uint16_t msia_len = msia.getArrayLength();
        os << "I32:[";
        for (uint16_t i = 0; i < msia_len; ++i) {
            os << msia_arr[i];
            if(i + 1 < msia_len)
                os << ", ";
        }
        os << "]";
        return os;
    }
    case MDT_BYTE_ARRAY: {
        MStorageByteArray& msba = static_cast<MStorageByteArray&>(st);
        uint8_t* msba_arr = msba.getByteArray();
        uint16_t msba_len = msba.getArrayLength();
        os << "B:[";
        os << std::hex << std::setiosflags(std::ios::showbase);
        for (uint16_t i = 0; i < msba_len; ++i) {
            os << static_cast<int>(msba_arr[i]);
            if(i + 1 < msba_len)
                os << ", ";
        }
        os << "]";
        os << std::dec << std::resetiosflags (std::ios::showbase);
        return os;
    }
    case MDT_GROUP: {
        MStorage& ms = static_cast<MStorage&>(st);
        MValueMapIterator it = ms.begin();
        os << "GR[";
        while(it != ms.end()) {
            os << "\"" << it->first << "\"=>" << *(it->second);
            it++;
            if(it != ms.end())
                os << ", ";
        }
        os << "]";
        return os;
    }
    default:
        break;
    }
    return os;
}

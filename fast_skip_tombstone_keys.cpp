#include <iostream>
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/iterator.h>
#include <cstdio>

int main()
{
    // Initialize RocksDB
    rocksdb::DB *db;
    rocksdb::Options options;
    options.create_if_missing = true;

    // Open the database
    rocksdb::Status status = rocksdb::DB::Open(options, "./out/test.db", &db);
    if (!status.ok())
    {
        std::cerr << "Unable to open/create RocksDB: " << status.ToString() << std::endl;
        return 1;
    }

    db->CompactRange(nullptr, nullptr);

    // Define a buffer for the formatted string
    char keyBuffer[10];

    // Write keys from 1 to 20
    for (int i = 1; i <= 20; ++i)
    {
        sprintf(keyBuffer, "%02d", i); // Format the integer with leading zeros
        std::string key = keyBuffer;
        db->Put(rocksdb::WriteOptions(), key, "value" + key);
    }

    // Delete keys from 5 to 15
    for (int i = 5; i <= 15; ++i)
    {
        sprintf(keyBuffer, "%02d", i); // Format the integer with leading zeros
        std::string key = keyBuffer;
        db->Delete(rocksdb::WriteOptions(), key);
    }

    // Read options with max_skippable_internal_keys set to a small value
    rocksdb::ReadOptions readOptions;
    readOptions.max_skippable_internal_keys = 5; // Example value

    // Iterate forward using Next
    std::unique_ptr<rocksdb::Iterator> it(db->NewIterator(readOptions));
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        std::cout << "Key: " << it->key().ToString() << ", Value: " << it->value().ToString() << std::endl;
    }

    it->Seek("5");
    if (it->Valid())
    {
        std::cout << "Seek Key: " << it->key().ToString() << ", Value: " << it->value().ToString() << std::endl;
    }

    std::cout << "using prev" << std::endl;

    // Iterate backward using Prev
    for (it->SeekToLast(); it->Valid(); it->Prev())
    {
        std::cout << "Key: " << it->key().ToString() << ", Value: " << it->value().ToString() << std::endl;
    }

    it.release();

    // Close the database
    delete db;

    return 0;
}

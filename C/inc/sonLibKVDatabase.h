/*
 * Copyright (C) 2006-2011 by Benedict Paten (benedictpaten@gmail.com)
 *
 * Released under the MIT license, see LICENSE.txt
 */

#ifndef SONLIB_KV_DATABASE_H_
#define SONLIB_KV_DATABASE_H_

#include "sonLibTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// General database exception id 
extern const char *ST_KV_DATABASE_EXCEPTION_ID;

// Exception where transaction should be retried
extern const char *ST_KV_DATABASE_RETRY_TRANSACTION_EXCEPTION_ID;

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//Database functions
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

/*
 * Constructs a non-relational database object, using the given configuration
 * information to connect to the database.  Create the database if create is
 * true, otherwise it must exist.
 */
stKVDatabase *stKVDatabase_construct(stKVDatabaseConf *conf, bool create);

/*
 * Destructs a database. If the destruction occurs during a transaction the transaction
 * is aborted and any changes are not committed to the database.
 * Also destroys any databases associated the the database.
 */
void stKVDatabase_destruct(stKVDatabase *database);

/*
 * Add a client cache to the database.
 * The size in the number of bytes of the cache. The cache is simple, it stores
 * all records retrieved, added and updated until it is full, then it starts to
 * remove stuff that has been accessed least recently to make room for the newly added
 * stuff.
 */
void stKVDatabase_makeMemCache(stKVDatabase *database, int64_t size, int64_t boundarySize);

/*
 * Empties the cache (if present).
 */
void stKVDatabase_clearCache(stKVDatabase *database);

/*
 * Removes the database from the disk. Any further operations on the database will
 * create an exception, so you should generally destruct the object after calling this function.
 */
void stKVDatabase_deleteFromDisk(stKVDatabase *database);

/*
 * Returns non-zero if the database contains a record with the given key.
 */
bool stKVDatabase_containsRecord(stKVDatabase *database, int64_t key);

/*
 * Add a new a key/value record into the table. Throws an exception if unsuccessful.
 */
void stKVDatabase_insertRecord(stKVDatabase *database, int64_t key, const void *value, int64_t sizeOfRecord);

/*
 * Update an existing key/value record in the table. Throws an exception if unsuccessful.
 */
void stKVDatabase_updateRecord(stKVDatabase *database, int64_t key, const void *value, int64_t sizeOfRecord);

/*
 * Gets a record from the database, given the key. The record is in newly allocated memory, and must be freed.
 * Returns NULL if the database does not contain the given record.
 */
void *stKVDatabase_getRecord(stKVDatabase *database, int64_t key);

/*
 * Gets a record from the database, given the key. The record is in newly allocated memory, and must be freed.
 * Returns NULL if the database does not contain the given record. Puts the size of the record in the in the record size field.
 */
void *stKVDatabase_getRecord2(stKVDatabase *database, int64_t key, int64_t *recordSize);

/*
 * Removes a record from the database. Throws an exception if unsuccessful.
 */
void stKVDatabase_removeRecord(stKVDatabase *database, int64_t key);

/*
 * Gets a record from the database, given the key. This function allows the partial retrieval of a record, using the
 * given offset and the size of the requested retrieval. The function should thrpw an exception if the record does
 * not exist (in contrast to stKVDatabase_getRecord) and similarly should throw an exception if the requested region lies
 * outside of the bounds of the record (the total size of the record must be passed the function).
 */
void *stKVDatabase_getPartialRecord(stKVDatabase *database, int64_t key, int64_t zeroBasedByteOffset, int64_t sizeInBytes, int64_t recordSize);

/*
 * Returns number of records in database.
 */
int64_t stKVDatabase_getNumberOfRecords(stKVDatabase *database);

/*
 * Starts a transaction with the database. Throws an exception if unsuccessful.
 */
void stKVDatabase_startTransaction(stKVDatabase *database);

/*
 * Commits the transaction to the database. Throws an exception if unsuccessful.
 */
void stKVDatabase_commitTransaction(stKVDatabase *database);

/*
 * Abort the current transaction. Throws an exception if unsuccessful.
 */
void stKVDatabase_abortTransaction(stKVDatabase *database);

/*
 * get the configuration object for the database.
 */
stKVDatabaseConf *stKVDatabase_getConf(stKVDatabase *database);

#ifdef __cplusplus
}
#endif
#endif

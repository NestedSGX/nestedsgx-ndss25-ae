//
//  sqlite_db.cc
//  YCSB-C
//

#include "sqlite_db.h"

#include <assert.h>
#include <cstring>

using namespace std;

extern "C" int printf(const char* fmt, ...);
extern "C" int puts(const char* str);

const char DB_NAME[] = ":memory:";
const int FIELD_NUM = 10;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for (int i = 0; i < argc; i++) {
        string azColName_str = azColName[i];
        string argv_str = (argv[i] ? argv[i] : "NULL");
        printf("%s", (azColName_str + " = " + argv_str + "\n").c_str());
    }
    puts("");
    return 0;
}

static int execute_sql_key_steps(sqlite3_stmt *pStmt, vector<vector<ycsbc::DB::KVPair>> *result) {
    int rc = 0;
    if (result) result->clear();
    int nCol = sqlite3_column_count(pStmt);
    while (sqlite3_step(pStmt) == SQLITE_ROW) {
        if (result) {
            vector<ycsbc::DB::KVPair> row;
            for (int i = 0; i < nCol; i++) {
                auto kv = make_pair((string)sqlite3_column_name(pStmt, i), (string)(char*)sqlite3_column_text(pStmt, i));
                row.push_back(kv);
            }
            result->push_back(row);
        }
    }
    rc = sqlite3_finalize(pStmt);
    return rc;
}

static int execute_sql_key(sqlite3 *db, const char *sql, const string& key,
        vector<vector<ycsbc::DB::KVPair>> *result) {
    int rc = 0;
    sqlite3_stmt *pStmt = 0;
    rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, NULL);
    assert(rc == SQLITE_OK || pStmt == 0);
    if (rc) return rc;

    rc = sqlite3_bind_text(pStmt, 1, key.c_str(), key.length(), SQLITE_STATIC);
    if (rc) return rc;

    return execute_sql_key_steps(pStmt, result);
}

static int execute_sql_args(sqlite3 *db, const char *sql, const vector<const string*>& args,
        vector<vector<ycsbc::DB::KVPair>> *result) {
    int rc = 0;
    sqlite3_stmt *pStmt = 0;
    rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, NULL);
    assert(rc == SQLITE_OK || pStmt == 0);
    if (rc) return rc;

    for (size_t i = 0; i < args.size(); i++) {
        rc = sqlite3_bind_text(pStmt, i + 1, args[i]->c_str(), args[i]->length(), SQLITE_STATIC);
        if (rc) return rc;
    }

    return execute_sql_key_steps(pStmt, result);
}

static int execute_sql(sqlite3 *db, const char *sql) {
    int rc;
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc) {
        printf("SQLite error: ");
        puts(sqlite3_errmsg(db));
        return rc;
    }
    return 0;
}

namespace ycsbc {

const int MAX_LEN = 4096 - 1;
static char stmt[MAX_LEN + 1];

SqliteDB::SqliteDB() {
    int rc; // For return status of SQLite
    rc = sqlite3_open(DB_NAME, &this->db); // Opening database
    if (rc) {
        puts("SQLite error - can't open database connection: ");
        puts(sqlite3_errmsg(this->db));
        return;
    }
    printf("Created database connection to %s\n", DB_NAME);

    const char table_name[] = "usertable";
    snprintf(stmt, MAX_LEN, "DROP TABLE IF EXISTS %s; CREATE TABLE IF NOT EXISTS %s (YCSB_KEY VARCHAR(64) PRIMARY KEY", table_name, table_name);
    for (size_t i = 0; i < FIELD_NUM; i++) {
        char field[256];
        snprintf(field, 255, ", FIELD%ld TEXT", i);
        strncat(stmt, field, MAX_LEN);
    }
    strncat(stmt, ");", MAX_LEN);
    // puts(stmt);
    assert(execute_sql(this->db, stmt) == 0);
}

SqliteDB::~SqliteDB() {
    sqlite3_close(this->db);
    puts("Closed database connection");
}

int SqliteDB::Read(const string &table, const string &key,
        const vector<string> *fields,
        vector<KVPair> &result) {
    vector<const string*> args;
    if (!fields)
        snprintf(stmt, MAX_LEN, "SELECT * FROM %s WHERE YCSB_KEY = ?", table.c_str());
    else {
        strncpy(stmt, "SELECT ", MAX_LEN);
        bool first = true;
        for (auto &f: *fields) {
            if (first) first = false;
            else strncat(stmt, ",", MAX_LEN);
            strncat(stmt, f.c_str(), MAX_LEN);
        }
        snprintf(stmt + strlen(stmt), MAX_LEN, " FROM %s WHERE YCSB_KEY = ?", table.c_str());
    }
    // puts(stmt);

    vector<vector<KVPair>> results;
    int rc = execute_sql_key(this->db, stmt, key, &results);
    if (results.size() > 0)
        result = results[0];
    else
        rc = 1;
    return rc;
}

int SqliteDB::Scan(const string &table, const string &key,
        int len, const vector<string> *fields,
        vector<vector<KVPair>> &results) {
    vector<const string*> args;
    if (!fields)
        snprintf(stmt, MAX_LEN, "SELECT * FROM %s WHERE YCSB_KEY >= ? LIMIT %d", table.c_str(), len);
    else {
        strncpy(stmt, "SELECT ", MAX_LEN);
        bool first = true;
        for (auto &f: *fields) {
            if (first) first = false;
            else strncat(stmt, ",", MAX_LEN);
            strncat(stmt, f.c_str(), MAX_LEN);
        }
        snprintf(stmt + strlen(stmt), MAX_LEN, " FROM %s WHERE YCSB_KEY >= ? LIMIT %d", table.c_str(), len);
    }
    // puts(stmt);

    return execute_sql_key(this->db, stmt, key, &results);
}

int SqliteDB::Update(const string &table, const string &key,
            vector<KVPair> &values) {
    snprintf(stmt, MAX_LEN, "UPDATE %s SET ", table.c_str());
    bool first = true;
    vector<const string*> args(values.size() + 1);
    for (size_t i = 0; i < values.size(); i++) {
        if (first) first = false;
        else strncat(stmt, ", ", MAX_LEN);
        strncat(stmt, values[i].first.c_str(), MAX_LEN);
        strncat(stmt, "=?", MAX_LEN);
        args[i] = &values[i].second;
    }
    strncat(stmt, " WHERE YCSB_KEY = ?;", MAX_LEN);
    args[values.size()] = &key;
    // puts(stmt);

    return execute_sql_args(this->db, stmt, args, NULL);
}

int SqliteDB::Insert(const string &table, const string &key,
            vector<KVPair> &values) {
    snprintf(stmt, MAX_LEN, "INSERT INTO %s (YCSB_KEY", table.c_str());
    for (auto kv : values) {
        strncat(stmt, ", ", MAX_LEN);
        strncat(stmt, kv.first.c_str(), MAX_LEN);
    }
    strncat(stmt, ") VALUES (?", MAX_LEN);

    vector<const string*> args(values.size() + 1);
    args[0] = &key;
    for (size_t i = 0; i < values.size(); i++) {
        strncat(stmt, ", ?", MAX_LEN);
        args[i + 1] = &values[i].second;
    }
    strncat(stmt, ");", MAX_LEN);
    // puts(stmt);

    return execute_sql_args(this->db, stmt, args, NULL);
}

int SqliteDB::Delete(const string &table, const string &key) {
    snprintf(stmt, 256, "DELETE FROM %s WHERE YCSB_KEY = ?;", table.c_str());
    puts(stmt);
    return execute_sql_key(this->db, stmt, key, NULL);
}


} // namespace ycsbc

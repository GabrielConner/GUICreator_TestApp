#include "database.h"

#include "sqlite3.h"

#include <vector>
#include <iostream>


namespace {

sqlite3* db = nullptr;
int err = 0;


} // namespace


namespace taco_truck {
namespace database {

void Start() {
  err = sqlite3_open("taco_truck_db.db", &db);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to open database : " << sqlite3_errmsg(db) << '\n';
    return;
  }

  char const* createTableStatements =
  "CREATE TABLE IF NOT EXISTS receipt ("
  "receipt_id INTEGER PRIMARY KEY,"
  "card BIT,"
  "extra DOUBLE(5,2)"
  ");"

  "CREATE TABLE IF NOT EXISTS menu_item ("
  "menu_item_id INTEGER PRIMARY KEY,"
  "name VARCHAR(25),"
  "price DOUBLE(5,2)"
  ");"

  "CREATE TABLE IF NOT EXISTS receipt_item ("
  "receipt_id INTEGER NOT NULL,"
  "menu_item_id INTEGER NOT NULL,"
  "count INT,"
  "CONSTRAINT pk_receipt_item PRIMARY KEY (receipt_id, menu_item_id),"
  "CONSTRAINT fk_receipt_id_ref_receipt_receipt_id FOREIGN KEY (receipt_id) REFERENCES receipt(receipt_id),"
  "CONSTRAINT fk_menu_item_id_ref_menu_item_menu_item_id FOREIGN KEY (menu_item_id) REFERENCES menu_item(menu_item_id)"
  ");"

  "INSERT INTO menu_item (name, price) VALUES "
  "('Cheese Taco', 1.09),"
  "('Bean Taco', 0.99),"
  "('Taco', 1.99);"
  ;


  sqlite3_stmt* stmt = nullptr;

  char const* statement = createTableStatements;
  while (statement != nullptr && *statement != '\0') {
    err = sqlite3_prepare(db, statement, -1, &stmt, &statement);
    if (err != SQLITE_OK) {
      std::cerr << "Failed to prepare statement : " << sqlite3_errmsg(db) << '\n';
      return;
    }

    sqlite3_step(stmt);
    err = sqlite3_finalize(stmt);

    if (err != SQLITE_OK) {
      std::cerr << "Error while running statement : " << sqlite3_errmsg(db) << '\n';
      return;
    }
  }
}


int AddReceipt(Receipt rec) {
  sqlite3_stmt* stmt = nullptr;

  err = sqlite3_prepare(db, "INSERT INTO receipt (card, extra) VALUES (?, ?);", -1, &stmt, nullptr);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to prepare receipt statement : " << sqlite3_errmsg(db) << '\n';
    return -1;
  }

  err = sqlite3_bind_int(stmt, 1, rec.card);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to bind 'card' : " << sqlite3_errmsg(db) << '\n';
    sqlite3_finalize(stmt);
    return -1;
  }

  err = sqlite3_bind_double(stmt, 2, rec.extra);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to bind 'extra' : " << sqlite3_errmsg(db) << '\n';
    sqlite3_finalize(stmt);
    return -1;
  }


  sqlite3_step(stmt);
  err = sqlite3_finalize(stmt);

  if (err != SQLITE_OK) {
    std::cerr << "Error while running receipt statement : " << sqlite3_errmsg(db) << '\n';
    return -1;
  }



  int64_t rowid = sqlite3_last_insert_rowid(db);

  err = sqlite3_prepare(db, "INSERT INTO receipt_item(receipt_id, menu_item_id, count) VALUES (?, ?, ?);", -1, &stmt, nullptr);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to prepare item statement : " << sqlite3_errmsg(db) << '\n';
    return -1;
  }

  err = sqlite3_bind_int(stmt, 1, rowid);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to bind 'receipt_id' : " << sqlite3_errmsg(db) << '\n';
    sqlite3_finalize(stmt);
    return -1;
  }


  for (Item& item : rec.items) {

    err = sqlite3_bind_int(stmt, 2, item.id);
    if (err != SQLITE_OK) {
      std::cerr << "Failed to bind 'menu_item_id' : " << sqlite3_errmsg(db) << '\n';
      sqlite3_finalize(stmt);
      return -1;
    }

    err = sqlite3_bind_int(stmt, 3, item.count);
    if (err != SQLITE_OK) {
      std::cerr << "Failed to bind 'count' : " << sqlite3_errmsg(db) << '\n';
      sqlite3_finalize(stmt);
      return -1;
    }


    sqlite3_step(stmt);
    err = sqlite3_reset(stmt);

    if (err != SQLITE_OK) {
      std::cerr << "Failed to run item statement : " << sqlite3_errmsg(db) << '\n';
      continue;
    }
  }

  sqlite3_finalize(stmt);

  return rowid;
}


Receipt GetReceiptByID(int id) {
  sqlite3_stmt* stmt = nullptr;
  Receipt receipt;

  char const* selectStatement =
    "SELECT * FROM receipt AS r\n"
    "INNER JOIN receipt_item AS i ON i.receipt_id = r.receipt_id\n"
    "INNER JOIN menu_item AS m ON m.menu_item_id = i.menu_item_id\n"
    "WHERE r.receipt_id = ?;"
    ;



  err = sqlite3_prepare(db, selectStatement, -1, &stmt, nullptr);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to prepare statement : " << sqlite3_errmsg(db) << '\n';
    return receipt;
  }


  err = sqlite3_bind_int(stmt, 1, id);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to bind 'receipt_id' : " << sqlite3_errmsg(db) << '\n';
    return receipt;
  }



  err = sqlite3_step(stmt);
  while (err == SQLITE_ROW) {
    int receiptID = sqlite3_column_int(stmt, 0);
    bool card = sqlite3_column_int(stmt, 1);
    double extra = sqlite3_column_double(stmt, 2);
    int count = sqlite3_column_int(stmt, 5);
    int typeID = sqlite3_column_int(stmt, 6);
    std::string name = std::string((char const*)sqlite3_column_text(stmt, 7));
    double price = sqlite3_column_double(stmt, 8);

    if (receipt.items.size() == 0) {
      receipt = Receipt(extra, card);
    }

    receipt.items.push_back(Item(name, price, count, typeID));

    err = sqlite3_step(stmt);
  }

  err = sqlite3_finalize(stmt);
  if (err != SQLITE_OK) {
    std::cerr << "Error while running statement : " << sqlite3_errmsg(db) << '\n';
  }

  return receipt;
}


std::vector<Receipt> GetAllReceipts() {
  sqlite3_stmt* stmt = nullptr;
  std::vector<Receipt> receiptList;

  char const* selectStatement = 
  "SELECT * FROM receipt r\n"
  "INNER JOIN receipt_item AS i ON i.receipt_id = r.receipt_id\n"
  "INNER JOIN menu_item AS m ON m.menu_item_id = i.menu_item_id;"
  ;


  err = sqlite3_prepare(db, selectStatement, -1, &stmt, nullptr);
  if (err != SQLITE_OK) {
    std::cerr << "Failed to prepare statement : " << sqlite3_errmsg(db) << '\n';
    return receiptList;
  }

  err = sqlite3_step(stmt);
  while (err == SQLITE_ROW) {
    int receiptID = sqlite3_column_int(stmt, 0);
    bool card = sqlite3_column_int(stmt, 1);
    double extra = sqlite3_column_double(stmt, 2);
    int count = sqlite3_column_int(stmt, 5);
    int typeID = sqlite3_column_int(stmt, 6);
    std::string name = std::string((char const*)sqlite3_column_text(stmt, 7));
    double price = sqlite3_column_double(stmt, 8);

    if (receiptID > receiptList.size()) {
      receiptList.push_back(Receipt(extra, card));
    }

    receiptList[receiptID - 1].items.push_back(Item(name, price, count, typeID));

    err = sqlite3_step(stmt);
  }

  err = sqlite3_finalize(stmt);
  if (err != SQLITE_OK) {
    std::cerr << "Error while running statement : " << sqlite3_errmsg(db) << '\n';
  }


  return receiptList;
}



void End() {
  sqlite3_close(db);
}



} // namespace database
} // namespace taco_truck
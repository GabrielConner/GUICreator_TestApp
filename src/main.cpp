#include "pPack/GUICreator.h"
#include "pPack/windowManager.h"
#include <format>

#include "registerController.h"
#include "database.h"

#include "sqlite3.h"


using namespace ::taco_truck;
using namespace ::pPack;
using namespace ::pPack::gui_creator;



int main() {
  StartAppInfo info = StartAppInfo();
  info.title = "Cash Register";

  if (!Start(info)) {
    std::cout << "Failed to start GUI\n";
    return 1;
  }
  database::Start();
  
  taco_truck::register_controller::Open();

  Update();

  End();
  //database::End();

  return 0;
}









//static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
//  int i;
//  for (i = 0; i<argc; i++) {
//    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//
//  }
//  printf("\n");
//  return 0;
//
//}
//
//
//
//int main() {
//  sqlite3 *db;
//  char *zErrMsg = 0;
//  int rc;
//
//  rc = sqlite3_open("taco_truck_db.db", &db);
//  if (rc) {
//    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//    sqlite3_close(db);
//    return(1);
//
//  }
//  rc = sqlite3_exec(db, "SELECT * FROM receipt r\nINNER JOIN receipt_item AS i ON i.receipt_id = r.receipt_id\nINNER JOIN menu_item AS m ON m.menu_item_id = i.menu_item_id;", callback, 0, &zErrMsg);
//  if (rc!=SQLITE_OK) {
//    fprintf(stderr, "SQL error: %s\n", zErrMsg);
//    sqlite3_free(zErrMsg);
//
//  }
//  sqlite3_close(db);
//  return 0;
//}
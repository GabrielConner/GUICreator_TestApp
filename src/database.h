#ifndef TACO_TRUCK_CASH_REGISTER_SRC_DATABASE_H
#define TACO_TRUCK_CASH_REGISTER_SRC_DATABASE_H

#include <string>
#include <vector>


namespace taco_truck {
namespace database {

struct Item {
  std::string name;
  double price;
  int count;
  int id;

  Item() : name(), price(), count(), id() {}
  Item(std::string Name, double Price, int Count, int ID) : name(Name), price(Price), count(Count), id(ID) {}
};

struct Receipt {
  std::vector<Item> items;

  double extra;

  bool card;

  Receipt() : items(), extra(), card() {}
  Receipt(double Extra, bool Card) : items(), extra(Extra), card(Card) {}
  Receipt(double Extra, bool Card, std::vector<Item> Items) : items(Items), extra(Extra), card(Card) {}
};



void Start();

int AddReceipt(Receipt rec);

Receipt GetReceiptByID(int id);
std::vector<Receipt> GetAllReceipts();

void End();

} // namespace database
} // namespace taco_truck


#endif
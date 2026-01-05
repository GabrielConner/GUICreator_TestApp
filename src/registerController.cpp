#include "registerController.h"
#include "pPack/GUICreator.h"
#include "pPack/vector.h"
#include "pPack/windowManager.h"
#include "database.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace ::taco_truck;
using namespace ::pPack;
using namespace ::pPack::gui_creator;
using namespace ::pPack::vector;
using namespace ::pPack::window_manager;



namespace {

void DisplayReceipt(int i);
int AddReceipt(double extra, bool card);
double SumCart();
void ClearCart();
void GetReceipts();


void DarkenObj(GUIElement* obj);
void ResetObj(GUIElement* obj);
void AddItem(GUIElement* obj);
void ClearCartButton(GUIElement* obj);
void Confirm(GUIElement* obj);
void Cancel(GUIElement* obj);
void Cash(GUIElement* obj);
void Card(GUIElement* obj);
void Yes(GUIElement* obj);
void No(GUIElement* obj);
void DisplayReceipts(GUIElement* obj);
void StartGrab(GUIElement* obj);
void Grab(GUIElement* obj);
void CloseReceipt(GUIElement* obj);
void LeftReceipt(GUIElement* obj);
void RightReceipt(GUIElement* obj);
void Release(GUIElement* obj);

struct ItemSave {
  int index;
  std::string name;
  double price;
  int number;

  ItemSave(int Index, std::string Name, double Price, int Number) : index(Index), name(Name), price(Price), number(Number) {}
};


Window* window = nullptr;
GUIElement* cartElement = nullptr;
GUIElement* confirmElement = nullptr;
GUIElement* cashCardElement = nullptr;
GUIElement* cardAsk = nullptr;
GUIElement* cardWarn = nullptr;
GUIElement* receipt = nullptr;
GUIElement* receiptText = nullptr;
GUIElement* receiptNumber = nullptr;

ItemSave items[] = {{1, "Cheese Taco", 1.09, -1}, {2, "Bean Taco", 0.99, -1}, {3, "Taco", 1.99, -1}};
std::vector<ItemSave> cart = std::vector<ItemSave>();

bool okayGrab = false;
Vector2 grabDiff = Vector2();

bool disableAll = false;

double cardExtraCharge = 0;

int currentReceiptDisplayed = -1;

bool readDatabase = false;

bool maximized = false;

std::vector<database::Receipt> receiptList = std::vector<database::Receipt>();

//double count = 0;

} // namespace


namespace taco_truck {
namespace register_controller {


void Open() {
  SetFunction("register", "darken", DarkenObj);
  SetFunction("register", "reset", ResetObj);
  SetFunction("register", "add", AddItem);
  SetFunction("register", "clear", ClearCartButton);
  SetFunction("register", "confirm", Confirm);
  SetFunction("register", "cancel", Cancel);
  SetFunction("register", "cash", Cash);
  SetFunction("register", "card", Card);
  SetFunction("register", "yes", Yes);
  SetFunction("register", "no", No);
  SetFunction("register", "displayReceipts", DisplayReceipts);
  SetFunction("register", "startGrab", StartGrab);
  SetFunction("register", "grab", Grab);
  SetFunction("register", "closeReceipt", CloseReceipt);
  SetFunction("register", "leftReceipt", LeftReceipt);
  SetFunction("register", "rightReceipt", RightReceipt);
  SetFunction("register", "release", Release);


  SetUpdateFunction(Update);


  SetPage("register", "register.xml");
  OpenPage("register");


  window = Window::GetCurrentContext();

  cartElement = static_cast<GUIElement*>(GetObjectByID("cart"));
  confirmElement = static_cast<GUIElement*>(GetObjectByID("confirm"));
  cashCardElement = static_cast<GUIElement*>(GetObjectByID("cashCard"));
  cardAsk = static_cast<GUIElement*>(GetObjectByID("cardAsk"));
  cardWarn = static_cast<GUIElement*>(GetObjectByID("cardWarn"));
  receipt = static_cast<GUIElement*>(GetObjectByID("receipt"));
  receiptText = static_cast<GUIElement*>(GetObjectByID("receiptText"));
  receiptNumber = static_cast<GUIElement*>(GetObjectByID("receiptNumber"));
}


void Update(double deltaTime) {
  if (window->GetInput(GLFW_KEY_W).pressed) {
    window->SetSize(720, 720);
  }

  if (window->GetInput(GLFW_KEY_F11).pressed) {
    if (maximized) {
      window->Restore();
      maximized = false;
    } else {
      window->Maximize();
      maximized = true;
    }
  }

  //count += deltaTime;
  //if (count > 0.5f) {
  //  std::cout << 1.0 / deltaTime << '\n';
  //  count = 0;
  //}
}




} // namespace register_controller
} // namespace taco_truck










namespace {


double SumCart() {
  double i = 0;
  for (auto& obj : cart) {
    i += obj.price * obj.number;
  }
  return i;
}


void DisplayReceipt(int i) {
  if (i >= receiptList.size() || i < 0) {
    return;
  }

  receipt->enabled = true;
  disableAll = true;


  database::Receipt rec = receiptList[i];

  std::stringstream str = std::stringstream();
  double cartTotal = 0;

  for (auto& item : rec.items) {
    int digitCount = std::floor(std::log(item.count) / std::log(10));
    int nameSize = item.name.size();
    int zeros = std::max((int)(12 - digitCount - nameSize), 0);

    str << std::fixed << std::setprecision(2) << item.count << "x " << item.name << std::string(zeros, ' ') << ": $" << item.price * item.count << '\n';
    cartTotal += item.price * item.count;
  }

  str << '\n'
    << "Card           : " << (rec.card ? "Yes" : "No") << '\n'
    << "Extra          : $" << rec.extra << '\n'
    << "Total          : $" << cartTotal + rec.extra << '\n';

  currentReceiptDisplayed = i;

  receiptText->data = str.str();
  receiptNumber->data = std::format("{}  /  {}", currentReceiptDisplayed + 1, receiptList.size());
}


int AddReceipt(double extra, bool card) {
  GetReceipts();

  std::vector<database::Item> itemList = std::vector<database::Item>();
  for (auto& item : cart) {
    if (item.number == -1) {
      continue;
    }

    itemList.push_back(database::Item(item.name, item.price, item.number, item.index));
  }

  database::Receipt receipt = database::Receipt(extra, card, itemList);
  receiptList.push_back(receipt);
  database::AddReceipt(receipt);
  return receiptList.size();
}


void ClearCart() {
  cart.clear();
  cartElement->data.clear();
  confirmElement->data = "Confirm   (empty)";
  confirmElement->primaryColor = Vector4(0, 0.1, 0, 1.0);

  for (auto& item : items) {
    item.number = -1;
  }

  confirmElement->SetCurrent();
}


void GetReceipts() {
  if (readDatabase) {
    return;
  }

  receiptList = database::GetAllReceipts();

  readDatabase = true;
}





// Callbacks
// -----------------


void CloseReceipt(GUIElement* obj) {
  receipt->enabled = false;
  disableAll=false;

  receipt->Reset();
  obj->Reset();
}

void LeftReceipt(GUIElement* obj) {
  if (currentReceiptDisplayed > 0) {
    DisplayReceipt(currentReceiptDisplayed - 1);
  }

  obj->Reset();
}

void RightReceipt(GUIElement* obj) {
  if (currentReceiptDisplayed < receiptList.size()) {
    DisplayReceipt(currentReceiptDisplayed + 1);
  }

  obj->Reset();
}


void DisplayReceipts(GUIElement* obj) {
  if (disableAll) {
    obj->Reset();
    return;
  }

  GetReceipts();

  DisplayReceipt(0);

  obj->Reset();
}




void Release(GUIElement* obj) {
  okayGrab = false;
}


void StartGrab(GUIElement* obj) {
  Vector2 mousePos = window->GetMousePosition().ConvertTo<float>();
  grabDiff = obj->drawTransform.position - mousePos;
  Vector2 pos = (obj->LazyMoveIntoLocal(mousePos) - obj->transform.position) / obj->transform.scale;
  if (pos.x <= 0.8 && pos.y >= 0.857142857) {
    okayGrab = true;
  } else {
    okayGrab = false;
  }
}

void Grab(GUIElement* obj) {
  Vector2 mousePos = window->GetMousePosition().ConvertTo<float>();

  if (okayGrab) {
    Vector2 moveToPos = mousePos + grabDiff;
    moveToPos.x = std::clamp(moveToPos.x, -1 + obj->drawTransform.scale.x, 1 - obj->drawTransform.scale.x);
    moveToPos.y = std::clamp(moveToPos.y, -1 + obj->drawTransform.scale.y, 1 - obj->drawTransform.scale.y);

    obj->transform.position = moveToPos;
  }
}




void DarkenObj(GUIElement* obj) {
  obj->primaryColor *= Vector4(0.8f, 0.8f, 0.8f, 1.0f);
}


void ResetObj(GUIElement* obj) {
  obj->Reset();
}


void AddItem(GUIElement* obj) {
  if (disableAll) {
    obj->Reset();
    return;
  }

  int objId = atoi(obj->id.c_str());
  if (objId == 0) {
    return;
  }
  objId--;

  ItemSave& item = items[objId];

  if (item.number == -1) {
    cart.push_back(ItemSave(items[objId].index, item.name, item.price, 1));
    item.number = cart.size() - 1;
  } else {
    cart[item.number].number++;
  }

  cartElement->data.clear();

  for (auto& cartItem : cart) {
    cartElement->data += std::format("{}x {: <12} :   ${:.2f}\n", cartItem.number, cartItem.name, cartItem.price * cartItem.number);
  }

  confirmElement->data = std::format("Confirm   ${:.2f}", SumCart());
  confirmElement->primaryColor = Vector4(0, 0.4, 0.1, 1.0);

  confirmElement->SetCurrent();


  obj->Reset();
}


void ClearCartButton(GUIElement* obj) {
  if (disableAll) {
    obj->Reset();
    return;
  }

  ClearCart();


  obj->Reset();
}


void Confirm(GUIElement* obj) {
  if (disableAll || cart.size() <= 0) {
    obj->Reset();
    return;
  }

  cashCardElement->enabled = true;

  obj->Reset();
}


void Cancel(GUIElement* obj) {
  if (cardAsk->enabled) {
    obj->Reset();
    return;
  }

  cashCardElement->enabled = false;


  obj->Reset();
}


void Cash(GUIElement* obj) {
  if (cardAsk->enabled) {
    obj->Reset();
    return;
  }

  DisplayReceipt(AddReceipt(0, false) - 1);

  ClearCart();

  confirmElement->Reset();

  cashCardElement->enabled = false;


  obj->Reset();
}


void Card(GUIElement* obj) {
  if (cardAsk->enabled) {
    obj->Reset();
    return;
  }

  cardAsk->enabled = true;

  double cartValue = SumCart();

  cardExtraCharge = cartValue * 0.03;


  cardWarn->data = std::format("WARN CUSTOMER\n\nCard comes with 3% surcharge of ${:.2f} which brings your total to ${:.2f}", cardExtraCharge, cartValue + cardExtraCharge);

  obj->Reset();
}


void Yes(GUIElement* obj) {
  DisplayReceipt(AddReceipt(cardExtraCharge, true) - 1);

  ClearCart();

  confirmElement->Reset();

  cashCardElement->enabled = false;
  cardAsk->enabled = false;


  obj->Reset();
}


void No(GUIElement* obj) {
  cashCardElement->enabled = false;
  cardAsk->enabled = false;


  obj->Reset();
}

} // namespace
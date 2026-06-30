#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "ExpenseManager.h"

int main()
{

  ExpenseManager manager;

  manager.initDb(); 
  manager.loadCsv();

  while (true)
  {
    std::cout << "\n===== 家計簿 =====\n";
    std::cout << "1:追加\n";
    std::cout << "2:一覧\n";
    std::cout << "3:削除\n";
    std::cout << "4:終了\n";

    int menu;
    std::cin >> menu;

    switch (menu)
    {
    case 1:
      manager.addExpense();
      break;

    case 2:
      manager.showExpenses();
      break;

    case 3:
      manager.deleteExpense();
      break;

    case 4:
      manager.exportCsv();
      return 0;

    default:
      std::cout << "無効な入力です\n";
      break;
    }
  }
}
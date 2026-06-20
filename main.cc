#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include"ExpenseManager.h"


int main()
{
  ExpenseManager manager;

  bool continue_input=true;
  char answer;//Y/Nを入力する

  while(continue_input)
  {
    
    manager.addExpense();
    std::cout<<"入力続けますか？(y/n)"<<std::endl;
    std::cin>>answer;

    if(answer=='n')
    {
      continue_input=false;
    }

  }
       manager.showExpenses();

std::cout
    << "合計金額: "
    << manager.getTotalAmount()
    << "円\n";


  manager.exportCsv();
    return 0;

}
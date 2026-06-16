#include<iostream>
#include<string>
#include<vector>

//家計簿の構造体定義
struct Expense {
    int amount; //金額
    std::string item; //品名
    std::string category; //カテゴリ
    std::string memo; //メモ
};

int main()
{
  std::vector<Expense> expenses; //vector型の定義(Expense構造体を格納する可変長配列、expensesの宣言)

  for(int i = 0; i < 2; i++)
  {
    Expense expense; //オブジェクト生成

    std::cout << "\n" << i + 1 << "件目\n";
    std::cout<<"金額を入力してください"<<std::endl;
    std::cin>>expense.amount;

    std::cout<<"品目を入力してください"<<std::endl;
    std::cin>>expense.item;

    std::cout<<"カテゴリを入力してください"<<std::endl;
    std::cin>>  expense.category;

    std::cout<<"メモを入力してください"<<std::endl;
    std::cin>>expense.memo;

    std::cout << "\n----- 登録内容 -----\n";
    std::cout << "金額: " << expense.amount << "円\n";
    std::cout << "カテゴリ: " << expense.category << "\n";
    std::cout << "品目: " << expense.item << "\n";
    std::cout << "メモ: " << expense.memo << "\n";

    expenses.push_back(expense); //expenses配列に入力内容を追加する

  }

  std::cout << "\n===== 一覧 =====\n";
  
  //入力一覧の表示
  for (const auto& expense : expenses)
  {
    std::cout
        << expense.amount << "円 "
        << expense.item << " "
        << expense.category << " "
        << expense.memo << "\n";
  }
  //合計金額の算出
  int total = 0;
  for (const auto& expense : expenses) 
  {
    total += expense.amount;
  }
    std::cout << "合計金額: " << total << "円\n";

  return 0;

}
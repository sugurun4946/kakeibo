#include<iostream>
#include<string>

//家計簿の構造体定義
struct Expense {
    int amount; //金額
    std::string item; //品名
    std::string category; //カテゴリ
    std::string memo; //メモ
};

int main()
{
    //オブジェクト生成
    Expense expense;

   //変数定義
    int amount;
    std::string item;
    std::string category; 
    std::string memo;

    //int amount; //金額
    //std::string category; //出費のカテゴリ

    std::cout<<"金額を入力してください"<<std::endl;
    std::cin>>amount;

    std::cout<<"品目を入力してください"<<std::endl;
    std::cin>>item;

    std::cout<<"カテゴリを入力してください"<<std::endl;
    std::cin>>category;

    std::cout<<"メモを入力してください"<<std::endl;
    std::cin>>memo;

    expense.amount = amount;
    expense.item = item;
    expense.category = category;
    expense.memo = memo;

    std::cout << "\n----- 登録内容 -----\n";
    std::cout << "金額: " << expense.amount << "円\n";
    std::cout << "カテゴリ: " << expense.category << "\n";
    std::cout << "品目: " << expense.item << "\n";
    std::cout << "メモ: " << expense.memo << "\n";


  return 0;

}
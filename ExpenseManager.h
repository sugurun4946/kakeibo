#pragma once

#include <vector>
#include <iostream>
#include <fstream>

#include "Expense.h"

class ExpenseManager
{
private:
    std::vector<Expense> expenses;
public:
//品目・価格を入力して、構造体に追加する関数
void addExpense()
{
    Expense expense; //オブジェクト生成

    std::cout<<"日付を入力してください"<<std::endl;
    std::cin>>expense.date;

    std::cout<<"金額を入力してください"<<std::endl;
    std::cin>>expense.amount;

    std::cout<<"品目を入力してください"<<std::endl;
    std::cin>>expense.item;

    std::cout << "カテゴリを選択してください\n";
    std::cout << "1: 食費\n";
    std::cout << "2: 娯楽・交際費\n";
    std::cout << "3: 日用品費\n";
    std::cout << "4: 交通費\n";
    std::cout << "5: 医療費\n";
    std::cout << "6: 通信費\n";
    std::cout << "7: 水道光熱費\n";
    std::cout << "8: その他\n";

    int categoryInput;
    std::cin >> categoryInput;

    switch (categoryInput)
    {
        case 1:
          expense.category = Category::Food;
        break;

        case 2:
          expense.category = Category::Entertainment;
        break;

        case 3:
          expense.category = Category::DailyNecessities;
        break;

        case 4:
          expense.category = Category::Transportation;
        break;

        case 5:
          expense.category = Category::Medical;
        break;

        case 6:
          expense.category = Category::Telecommunications;
        break;
        case 7:
          expense.category = Category::Utilities;
        break;

        case 8:
          expense.category = Category::Other;
        break;

        default:
          expense.category = Category::Other;
        break;
}

    std::cout<<"税込みか税抜きかを入力してください"<<std::endl;

    std::cout << "1: 税込\n";
    std::cout << "2: 税抜き\n";

    int taxTypeInput;
    std::cin >> taxTypeInput;

    switch (taxTypeInput)
    {
        case 1:
          expense.taxtype = TaxType::Included;
        break;

        case 2:
          expense.taxtype = TaxType::Excluded;
        break;

        default:
          expense.taxtype = TaxType::Included;
        break;
    }

    std::cout<<"税率"<<std::endl;

    std::cout << "1: 8%\n";
    std::cout << "2: 10%\n";

    int taxRateInput;
    std::cin >> taxRateInput;

    switch (taxRateInput)
    {
        case 1:
          expense.taxrate = TaxRate::Tax8;
        break;

        case 2:
          expense.taxrate = TaxRate::Tax10;
        break;

        default:
          expense.taxrate = TaxRate::Tax8;
        break;
    }

    if(taxTypeInput)


    std::cout<<"メモを入力してください"<<std::endl;
    std::cin>>expense.memo;

    std::cout << "\n----- 登録内容 -----\n";
    std::cout << "入力金額: " << expense.amount << "円\n";
    std::cout << "税込金額: " << getAmountIncludingTax(expense) << "円\n";
    std::cout << "税抜金額: " << getAmountExcludingTax(expense) << "円\n";
    std::cout<< "カテゴリ: "<< categoryToString(expense.category) << "\n";    
    std::cout << "品目: " << expense.item << "\n";
    std::cout << "メモ: " << expense.memo << "\n";

    expenses.push_back(expense); //expenses配列に入力内容を追加する
}

//カテゴリの日本語変換関数
std::string categoryToString(Category category)
{
    switch (category)
    {
    case Category::Food:
        return "食費";
    case Category::Entertainment:
        return "娯楽・交際費";
    case Category::DailyNecessities:
        return "日用品費";
    case Category::Transportation:
        return "交通費";
    case Category::Medical:
        return "医療費";
    case Category::Telecommunications:
        return "通信費";
    case Category::Utilities:
        return "水道光熱費";
    case Category::Other:
        return "その他";
    default:
        return "不明";
    }
}

std::string TaxTypeToString(TaxType taxtype)
{
    switch (taxtype)
    {
    case TaxType::Included:
        return "税込";
    case TaxType::Excluded:
        return "税抜";
    default:
        return "税抜";
    }
}

std::string TaxRateToString(TaxRate taxrate)
{
    switch (taxrate)
    {
    case TaxRate::Tax8:
        return "8%";
    case TaxRate::Tax10:
        return "10%";
    default:
        return "8%";
    }
}


//入力内容を表示する関数
void showExpenses()
{
    std::cout << "\n===== 一覧 =====\n";

    for (const auto& expense : expenses)
    {
        std::cout
            << expense.date <<","
            << expense.amount << "円 "
            << expense.item << " "
            << categoryToString(expense.category) << " "
            << TaxTypeToString(expense.taxtype) << " "
            << TaxRateToString(expense.taxrate) << " "
            << expense.memo << "\n";
    }
}
//入力したものの総額を表示する関数
int getTotalAmount()
{
    int total = 0;

    for (const auto& expense : expenses)
    {
        //total += expense.amount;
        total += getAmountIncludingTax(expense);
    }

    return total;
}

void exportCsv()
{
    //std::cout << "csv出力" << std::endl;
    std::ofstream file("expenses.csv");
    file << "date,amount,item,category,memo\n";

    for (const auto& expense : expenses)
    {
        file
            << expense.date << ","
            << getAmountIncludingTax(expense) << ","
            << expense.item << ","
            << categoryToString(expense.category) << ","
            << expense.memo << "\n";
    }
}

int getAmountIncludingTax(const Expense& expense)
{
    // 既に税込ならそのまま返す
    if (expense.taxtype == TaxType::Included)
    {
        return expense.amount;
    }

    double rate;

    switch (expense.taxrate)
    {
    case TaxRate::Tax8:
        rate = 1.08;
        break;

    case TaxRate::Tax10:
        rate = 1.10;
        break;

    default:
        rate = 1.10;
        break;
    }

    return static_cast<int>(expense.amount * rate);
}

int getAmountExcludingTax(const Expense& expense)
{
    // 既に税抜ならそのまま返す
    if (expense.taxtype == TaxType::Excluded)
    {
        return expense.amount;
    }

    double rate;

    switch (expense.taxrate)
    {
    case TaxRate::Tax8:
        rate = 1.08;
        break;

    case TaxRate::Tax10:
        rate = 1.10;
        break;

    default:
        rate = 1.10;
        break;
    }

    return static_cast<int>(expense.amount / rate);
}

};
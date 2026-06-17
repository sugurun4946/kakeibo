#pragma once

#include <vector>
#include <iostream>

#include "Expense.h"


class ExpenseManager
{
private:
    std::vector<Expense> expenses;

public:
void addExpense()
{
    Expense expense; //オブジェクト生成

    //std::cout << "\n" << i + 1 << "件目\n";
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

void showExpenses()
{
    std::cout << "\n===== 一覧 =====\n";

    for (const auto& expense : expenses)
    {
        std::cout
            << expense.amount << "円 "
            << expense.item << " "
            << expense.category << " "
            << expense.memo << "\n";
    }
}

int getTotalAmount()
    {
        int total = 0;

        for (const auto& expense : expenses)
        {
            total += expense.amount;
        }

        return total;
    }


};
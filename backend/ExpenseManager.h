#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Expense.h"

class ExpenseManager
{
private:
    std::vector<Expense> expenses;

public:
    void addExpense();                                 // 品目・価格を入力して、構造体に追加する関数
    void deleteExpense();                              // 入力内容を削除する関数
    std::string categoryToString(Category category);   // カテゴリの日本語変換関数
    std::string TaxTypeToString(TaxType taxtype);      // カテゴリの日本語変換関数
    std::string TaxRateToString(TaxRate taxrate);      // カテゴリの日本語変換関数
    void showExpenses();                               // 入力内容を表示する関数
    int getTotalAmount();                              // 入力したものの総額を表示する関数
    void exportCsv();                                  // csv出力する関数
    void loadCsv();                                    // csv出力する関数
    int getAmountIncludingTax(const Expense &expense); // 税込み金額の算出関数
    int getAmountExcludingTax(const Expense &expense); // 税抜き金額の算出関数
};
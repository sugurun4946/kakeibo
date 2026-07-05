#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sqlite3.h>

#include "Expense.h"

class ExpenseManager
{
private:
    std::vector<Expense> expenses;
    sqlite3 *db = nullptr; // nullptr で初期化しておくと安全です

public:
    // デストラクターを追加（DBのクローズ処理用）
    ~ExpenseManager();

    void addExpense();                                 // 品目・価格を入力して、構造体に追加する関数
    void deleteExpense();                              // 入力内容を削除する関数
    std::string categoryToString(Category category);   // カテゴリの日本語変換関数
    std::string TaxTypeToString(TaxType taxtype);      // 税区分の日本語変換関数
    std::string TaxRateToString(TaxRate taxrate);      // 税率の日本語変換関数
    void showExpenses();                               // 入力内容を表示する関数
    int getTotalAmount();                              // 入力したものの総額を表示する関数
    int getAmountIncludingTax(const Expense &expense); // 税込み金額の算出関数
    int getAmountExcludingTax(const Expense &expense); // 税抜き金額の算出関数
    
    // --- SQLite関連の関数 ---
    void initDb();                                     // DBの初期化関数
    void loadFromDb();                                 // アプリ起動時にSQLiteのデータを読み込んで expenses ベクターに格納する関数（セミコロンを追加）

    // --- CSV関連の関数（不要なら削除してもOKです） ---
    void exportCsv();                                  // csv出力する関数
    void loadCsv();                                    // csv読み込みする関数
};
#pragma once
#include<string>

//税込みか税抜きか
enum class TaxType
{
    Included, // 税込
    Excluded  // 税抜
};

//消費税率
enum class TaxRate
{
    Tax8, // 税込
    Tax10  // 税抜
};

//カテゴリのenum定義
enum class Category
{
    Food = 1,             // 食費
    Entertainment,        // 娯楽・交際費
    DailyNecessities,     // 日用品費
    Transportation,       // 交通費
    Medical,              // 医療費
    Telecommunications,   // 通信費
    Utilities,            // 水道光熱費
    Other                 // その他
};

//家計簿の構造体定義
struct Expense {
    std::string date;//日付
    int amount; //金額
    std::string item; //品名
    Category category; //カテゴリ
    TaxType taxtype; //税率
    TaxRate taxrate; //税込みか税抜きか
    std::string memo; //メモ
};


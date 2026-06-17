#pragma once
#include <string>

//家計簿の構造体定義
struct Expense {
    int amount; //金額
    std::string item; //品名
    std::string category; //カテゴリ
    std::string memo; //メモ
};
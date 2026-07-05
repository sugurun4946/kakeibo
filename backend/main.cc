#include <iostream>
#include <string>
#include <vector>

#include "ExpenseManager.h"

int main()
{
    ExpenseManager manager;

    // 起動時にデータベースを初期化し、保存されているデータを読み込む
    manager.initDb(); //データベースの初期化関数
    manager.loadFromDb();//保存されているデータベースを

    while (true)
    {
        std::cout << "\n===== 家計簿 =====\n";
        std::cout << "1: 追加\n";
        std::cout << "2: 一覧\n";
        std::cout << "3: 削除\n";
        std::cout << "4: Excel用CSV出力\n";
        std::cout << "5: 終了\n";

        int menu;
        // 入力エラー（文字入力など）による無限ループを防ぐ対策
        if (!(std::cin >> menu)) {
            std::cout << "数値入力をしてください。\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

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
            // SQLiteからデータを読み込んでExcelで開けるCSVを出力
            manager.exportCsv();
            break;

        case 5:
            std::cout << "アプリケーションを終了します。\n";
            return 0;

        default:
            std::cout << "無効な入力です。1〜5の数値を入力してください。\n";
            break;
        }
    }
}
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sqlite3.h>

#include "Expense.h"
#include "ExpenseManager.h"

void ExpenseManager::addExpense()
{
    Expense expense; // オブジェクト生成

    std::cout << "日付を入力してください" << std::endl;
    std::cin >> expense.date;

    std::cout << "金額を入力してください" << std::endl;
    std::cin >> expense.amount;

    std::cout << "品目を入力してください" << std::endl;
    std::cin >> expense.item;

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

    std::cout << "税込みか税抜きかを入力してください" << std::endl;

    std::cout << "1: 税込\n";
    std::cout << "2: 税抜き\n";

    int taxTypeInput;
    std::cin >> taxTypeInput;

    std::string taxTypeStr;

    switch (taxTypeInput)
    {
    case 1:
        expense.taxtype = TaxType::Included;
        taxTypeStr = "税込";
        break;

    case 2:
        expense.taxtype = TaxType::Excluded;
        taxTypeStr = "税抜";
        break;

    default:
        expense.taxtype = TaxType::Included;
        taxTypeStr = "税込";
        break;
    }

    std::cout << "税率" << std::endl;

    std::cout << "1: 8%\n";
    std::cout << "2: 10%\n";

    int taxRateInput;
    std::cin >> taxRateInput;

    std::string taxRateStr;

    switch (taxRateInput)
    {
    case 1:
        expense.taxrate = TaxRate::Tax8;
        taxRateStr = "8%";
        break;

    case 2:
        expense.taxrate = TaxRate::Tax10;
        taxRateStr = "10%";
        break;

    default:
        expense.taxrate = TaxRate::Tax8;
        taxRateStr = "8%";
        break;
    }

    std::cout << "メモを入力してください" << std::endl;
    std::cin >> expense.memo;
// --- ここからSQLiteへのインサート処理 ---
    const char* sql = "INSERT INTO expenses (date, amount, item, category, tax_type, tax_rate, memo) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, expense.date.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, expense.amount);
        sqlite3_bind_text(stmt, 3, expense.item.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, categoryToString(expense.category).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, taxTypeStr.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, taxRateStr.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, expense.memo.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            // データベースに挿入された自動採番のIDを取得してセット
            expense.id = static_cast<int>(sqlite3_last_insert_rowid(db));
            std::cout << "登録完了\n";
        } else {
            std::cout << "登録失敗\n";
        }
    }
    sqlite3_finalize(stmt);

    // 画面表示
    std::cout << "\n----- 登録内容 -----\n";
    std::cout << "入力金額: " << expense.amount << "円\n";
    std::cout << "税込金額: " << getAmountIncludingTax(expense) << "円\n";
    std::cout << "税抜金額: " << getAmountExcludingTax(expense) << "円\n";
    std::cout << "カテゴリ: " << categoryToString(expense.category) << "\n";
    std::cout << "品目: " << expense.item << "\n";
    std::cout << "メモ: " << expense.memo << "\n";

    expenses.push_back(expense); // メモリ上の配列にも追加
}

// カテゴリの日本語変換関数
std::string ExpenseManager::categoryToString(Category category)
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

std::string ExpenseManager::TaxTypeToString(TaxType taxtype)
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

std::string ExpenseManager::TaxRateToString(TaxRate taxrate)
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

// 入力内容を表示する関数
void ExpenseManager::showExpenses()
{
    std::cout << "\n===== 一覧 =====\n";

    for (size_t i = 0; i < expenses.size(); i++)
    {
        const auto &expense = expenses[i];
        std::cout
            << i << ":"
            << expense.date << ","
            << expense.amount << "円 "
            << expense.item << " "
            << categoryToString(expense.category) << " "
            << TaxTypeToString(expense.taxtype) << " "
            << TaxRateToString(expense.taxrate) << " "
            << expense.memo << "\n";
    }
}
// 入力したものの総額を表示する関数
int ExpenseManager::getTotalAmount()
{
    int total = 0;

    for (const auto &expense : expenses)
    {
        // total += expense.amount;
        total += getAmountIncludingTax(expense);
    }

    return total;
}

void ExpenseManager::exportCsv()
{
    // 出力するファイルを開く
    std::ofstream file("expenses.csv");
    if (!file.is_open()) {
        std::cout << "CSVファイルの作成に失敗しました\n";
        return;
    }

    // Excelで開いたときに日本語が文字化けしないように「BOM（Byte Order Mark）」を追加
    file << "\xEF\xBB\xBF";

    // ヘッダー（列名）の書き込み
    file << "日付,税込金額,税抜金額,品名,カテゴリ,税区分,税率,メモ\n";

    // SQLiteから全データを取得するクエリ
    const char* sql = "SELECT date, amount, item, category, tax_type, tax_rate, memo FROM expenses;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int amount = sqlite3_column_int(stmt, 1);
            std::string item = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            std::string tax_type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            std::string tax_rate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            
            const unsigned char* memoText = sqlite3_column_text(stmt, 6);
            std::string memo = memoText ? reinterpret_cast<const char*>(memoText) : "";

            // 税込・税抜の計算用に一時的な構造体を作る
            Expense e;
            e.amount = amount;
            e.taxtype = (tax_type == "税込") ? TaxType::Included : TaxType::Excluded;
            e.taxrate = (tax_rate == "8%") ? TaxRate::Tax8 : TaxRate::Tax10;

            int incTax = getAmountIncludingTax(e);
            int excTax = getAmountExcludingTax(e);

            // CSVに1行ずつ書き出し（カンマ区切り）
            file << date << ","
                 << incTax << ","
                 << excTax << ","
                 << item << ","
                 << category << ","
                 << tax_type << ","
                 << tax_rate << ","
                 << memo << "\n";
        }
    } else {
        std::cout << "データベースからのエクスポートに失敗しました\n";
    }

    sqlite3_finalize(stmt);
    std::cout << "Excel用CSVエクスポート完了（expenses.csv）\n";
}

int ExpenseManager::getAmountIncludingTax(const Expense &expense)
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

int ExpenseManager::getAmountExcludingTax(const Expense &expense)
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

void ExpenseManager::loadCsv()
{
    std::ifstream file("expenses.csv"); // ←これが必須

    if (!file.is_open())
    {
        std::cout << "CSVが見つかりません\n";
        return;
    }

    std::string line;

    // ヘッダー読み飛ばし
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        Expense e;

        std::string categoryStr;
        std::string taxTypeStr;
        std::string taxRateStr;

        // CSV構造:
        // date,amount,item,category,taxType,taxRate,memo

        std::getline(ss, e.date, ',');

        ss >> e.amount;
        ss.ignore(); // カンマ飛ばし

        std::getline(ss, e.item, ',');
        std::getline(ss, categoryStr, ',');
        std::getline(ss, taxTypeStr, ',');
        std::getline(ss, taxRateStr, ',');
        std::getline(ss, e.memo);

        // -------------------------
        // category復元
        // -------------------------
        if (categoryStr == "食費")
            e.category = Category::Food;
        else if (categoryStr == "娯楽・交際費")
            e.category = Category::Entertainment;
        else if (categoryStr == "日用品費")
            e.category = Category::DailyNecessities;
        else if (categoryStr == "交通費")
            e.category = Category::Transportation;
        else if (categoryStr == "医療費")
            e.category = Category::Medical;
        else if (categoryStr == "通信費")
            e.category = Category::Telecommunications;
        else if (categoryStr == "水道光熱費")
            e.category = Category::Utilities;
        else
            e.category = Category::Other;

        // -------------------------
        // taxType復元
        // -------------------------
        e.taxtype = (taxTypeStr == "税込")
                        ? TaxType::Included
                        : TaxType::Excluded;

        // -------------------------
        // taxRate復元
        // -------------------------
        e.taxrate =
            (taxRateStr == "8%")
                ? TaxRate::Tax8
                : TaxRate::Tax10;

        // -------------------------
        // vectorに復元
        // -------------------------
        expenses.push_back(e);
    }

    std::cout << "CSV読み込み完了: " << expenses.size() << "件\n";
}


    void ExpenseManager::deleteExpense()
{
    if (expenses.empty()) {
        std::cout << "削除するデータがありません\n";
        return;
    }

    int index;
    std::cout << "削除する番号を入力してください: ";
    std::cin >> index;

    if (index < 0 || index >= static_cast<int>(expenses.size())) {
        std::cout << "無効な番号です\n";
        return;
    }

    // 削除対象のDB上のIDを取得
    int targetId = expenses[index].id;

    // データベースから削除
    const char* sql = "DELETE FROM expenses WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, targetId);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            // メモリ上（vector）からも削除
            expenses.erase(expenses.begin() + index);
            std::cout << "削除しました\n";
        } else {
            std::cout << "データベースからの削除に失敗しました\n";
        }
    }
    sqlite3_finalize(stmt);
}


void ExpenseManager::initDb()
{
    if (sqlite3_open("expenses.db", &db))
    {
        std::cout << "DB接続失敗\n";
        return;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS expenses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date TEXT,"
        "amount INTEGER,"
        "item TEXT,"
        "category TEXT,"
        "tax_type TEXT,"
        "tax_rate TEXT,"
        "memo TEXT);";

    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

//FFI用の関数(税込みの計算)
extern "C" {
    int32_t c_get_amount_including_tax(int32_t amount, int32_t tax_type_raw, int32_t tax_rate_raw) {
        ExpenseManager manager;
        Expense expense;
        
        expense.amount = amount;
        expense.taxtype = (tax_type_raw == 1) ? TaxType::Included : TaxType::Excluded;
        expense.taxrate = (tax_rate_raw == 1) ? TaxRate::Tax8 : TaxRate::Tax10;

        return manager.getAmountIncludingTax(expense);
    }
}

// ExpenseManager.cc の末尾などに追加

void ExpenseManager::loadFromDb()
{
    expenses.clear();

    const char* sql = "SELECT id, date, amount, item, category, tax_type, tax_rate, memo FROM expenses;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "データ読み込み失敗\n";
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Expense e;
        e.id = sqlite3_column_int(stmt, 0);
        e.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        e.amount = sqlite3_column_int(stmt, 2);
        e.item = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        
        std::string categoryStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string taxTypeStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        std::string taxRateStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        
        const unsigned char* memoText = sqlite3_column_text(stmt, 7);
        e.memo = memoText ? reinterpret_cast<const char*>(memoText) : "";

        if (categoryStr == "食費") e.category = Category::Food;
        else if (categoryStr == "娯楽・交際費") e.category = Category::Entertainment;
        else if (categoryStr == "日用品費") e.category = Category::DailyNecessities;
        else if (categoryStr == "交通費") e.category = Category::Transportation;
        else if (categoryStr == "医療費") e.category = Category::Medical;
        else if (categoryStr == "通信費") e.category = Category::Telecommunications;
        else if (categoryStr == "水道光熱費") e.category = Category::Utilities;
        else e.category = Category::Other;

        e.taxtype = (taxTypeStr == "税込") ? TaxType::Included : TaxType::Excluded;
        e.taxrate = (taxRateStr == "8%") ? TaxRate::Tax8 : TaxRate::Tax10;

        expenses.push_back(e);
    }

    sqlite3_finalize(stmt);
    std::cout << "データベース読み込み完了: " << expenses.size() << "件\n";
}

//デストラクタ
ExpenseManager::~ExpenseManager() {
    if (db) {
        sqlite3_close(db);
    }
}
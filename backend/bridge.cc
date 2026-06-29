// src/bridge.cpp
#include <stdint.h>
#include "Expense.h"
#include "ExpenseManager.h"

// extern "C" をつけることで、Dartから関数名を見つけやすくします
extern "C" {

    // 税込金額を計算して返す窓口関数
    // 引数はDartから受け取りやすいように、すべて単純な数値（int32_t）にします
    __attribute__((visibility("default")))
    int32_t c_get_amount_including_tax(int32_t amount, int32_t tax_type, int32_t tax_rate) {
        
        Expense expense;
        expense.amount = amount;
        
        // Dartから届いた数値を、C++のenumにキャスト（変換）
        // 例: 1 -> Included, 2 -> Excluded
        expense.taxtype = static_cast<TaxType>(tax_type);
        expense.taxrate = static_cast<TaxRate>(tax_rate);

        ExpenseManager manager;
        return manager.getAmountIncludingTax(expense);
    }
}
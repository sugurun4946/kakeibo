import 'dart:ffi' as ffi;
import 'dart:io';
import 'package:flutter/material.dart';

// ─── FFI設定 ───
typedef CGetAmountFunc = ffi.Int32 Function(ffi.Int32, ffi.Int32, ffi.Int32);
typedef DartGetAmountFunc = int Function(int, int, int);

int callCplusplusCalculation(int price, int type, int rate) {
  try {
    final cppLibPath = '${Directory.current.path}/libnative_expense.so';
    final dylib = ffi.DynamicLibrary.open(cppLibPath);
    final DartGetAmountFunc getAmountIncludingTax = dylib
        .lookup<ffi.NativeFunction<CGetAmountFunc>>('c_get_amount_including_tax')
        .asFunction();
    return getAmountIncludingTax(price, type, rate);
  } catch (e) {
    print('❌ C++の呼び出しに失敗: $e');
    return 0;
  }
}

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Kakeibo Premium',
      debugShowCheckedModeBanner: false,
      theme: ThemeData.dark().copyWith(
        scaffoldBackgroundColor: const Color(0xFF12141C),
        appBarTheme: const AppBarTheme(
          backgroundColor: Color(0xFF12141C),
          elevation: 0,
        ),
      ),
      home: const DashboardScreen(),
    );
  }
}

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({super.key});

  @override
  State<DashboardScreen> createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  // 履歴データ
  final List<Map<String, dynamic>> _history = [
    {
      'title': 'カフェ・ランチ',
      'date': '今日',
      'amount': '¥ 1,200',
      'color': Colors.orangeAccent,
      'icon': Icons.restaurant
    },
    {
      'title': '交通費（ICチャージ）',
      'date': '昨日',
      'amount': '¥ 3,000',
      'color': Colors.blueAccent,
      'icon': Icons.train
    },
  ];

  void _showInputBottomSheet() {
    showModalBottomSheet(
      context: context,
      isScrollControlled: true,
      backgroundColor: const Color(0xFF1E2130),
      shape: const RoundedRectangleBorder(
        borderRadius: BorderRadius.vertical(top: Radius.circular(24)),
      ),
      builder: (BuildContext context) {
        return _ExpenseInputForm(
          // カテゴリ名、アイコン、色も受け取るように拡張
          onSave: (price, type, rate, categoryName, categoryIcon, categoryColor) {
            int resultAmount = callCplusplusCalculation(price, type, rate);

            setState(() {
              _history.insert(0, {
                'title': categoryName, // 選択したカテゴリ名
                'date': 'たった今',
                'amount': '¥ $resultAmount', // C++が計算した結果
                'color': categoryColor, // 選択した色
                'icon': categoryIcon, // 選択したアイコン
              });
            });
          },
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text(
          'KAKEIBO',
          style: TextStyle(letterSpacing: 2, fontWeight: FontWeight.bold, fontSize: 20),
        ),
      ),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(24.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('こんにちは、Suguru さん', style: TextStyle(color: Colors.grey, fontSize: 16)),
            const SizedBox(height: 8),
            const Text('今月の資産状況', style: TextStyle(fontSize: 24, fontWeight: FontWeight.bold)),
            const SizedBox(height: 24),

            // 総残高カード
            Container(
              width: double.infinity,
              padding: const EdgeInsets.all(24),
              decoration: BoxDecoration(
                gradient: const LinearGradient(
                  colors: [Color(0xFF6366F1), Color(0xFF4F46E5)],
                  begin: Alignment.topLeft,
                  end: Alignment.bottomRight,
                ),
                borderRadius: BorderRadius.circular(24),
                boxShadow: [
                  BoxShadow(
                    color: const Color(0xFF4F46E5).withOpacity(0.3),
                    blurRadius: 20,
                    offset: const Offset(0, 10),
                  )
                ],
              ),
              child: const Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text('総残高', style: TextStyle(color: Colors.white70, fontSize: 14)),
                  SizedBox(height: 8),
                  Text('¥ 128,450', style: TextStyle(color: Colors.white, fontSize: 36, fontWeight: FontWeight.bold)),
                ],
              ),
            ),
            const SizedBox(height: 32),

            // 履歴ヘッダー
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                const Text('最近の支出履歴', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                TextButton(
                  onPressed: _showInputBottomSheet,
                  child: const Row(
                    children: [
                      Icon(Icons.add, color: Colors.indigoAccent, size: 18),
                      SizedBox(width: 4),
                      Text('追加する', style: TextStyle(color: Colors.indigoAccent, fontWeight: FontWeight.bold)),
                    ],
                  ),
                ),
              ],
            ),
            const SizedBox(height: 12),

            // 動的な支出履歴リスト
            ..._history.map((item) => _buildHistoryItem(
                  item['icon'],
                  item['title'],
                  item['date'],
                  item['amount'],
                  item['color'],
                )),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _showInputBottomSheet,
        backgroundColor: const Color(0xFF4F46E5),
        child: const Icon(Icons.add, color: Colors.white),
      ),
    );
  }

  Widget _buildHistoryItem(IconData icon, String title, String date, String amount, Color iconBg) {
    return Container(
      margin: const EdgeInsets.only(bottom: 12),
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: const Color(0xFF1E2130),
        borderRadius: BorderRadius.circular(16),
      ),
      child: Row(
        children: [
          Container(
            padding: const EdgeInsets.all(10),
            decoration: BoxDecoration(
              color: iconBg.withOpacity(0.2),
              borderRadius: BorderRadius.circular(12),
            ),
            child: Icon(icon, color: iconBg),
          ),
          const SizedBox(width: 16),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(title, style: const TextStyle(fontWeight: FontWeight.bold, fontSize: 15)),
                const SizedBox(height: 4),
                Text(date, style: const TextStyle(color: Colors.grey, fontSize: 12)),
              ],
            ),
          ),
          Text(amount, style: const TextStyle(fontWeight: FontWeight.bold, fontSize: 16, color: Colors.white70)),
        ],
      ),
    );
  }
}

// ─── 入力フォーム（カテゴリ選択対応版） ───
class _ExpenseInputForm extends StatefulWidget {
  final Function(int price, int type, int rate, String catName, IconData catIcon, Color catColor) onSave;
  const _ExpenseInputForm({required this.onSave});

  @override
  State<_ExpenseInputForm> createState() => _ExpenseInputFormState();
}

class _ExpenseInputFormState extends State<_ExpenseInputForm> {
  final _priceController = TextEditingController();
  int _selectedType = 2; // デフォルト：税抜
  int _selectedRate = 2; // デフォルト：10%

  // カテゴリ定義のリスト
  final List<Map<String, dynamic>> _categories = [
    {'name': '食費', 'icon': Icons.restaurant, 'color': Colors.orangeAccent},
    {'name': '交通費', 'icon': Icons.train, 'color': Colors.blueAccent},
    {'name': '衣服・雑貨', 'icon': Icons.shopping_bag, 'color': Colors.purpleAccent},
    {'name': '趣味・娯楽', 'icon': Icons.sports_esports, 'color': Colors.pinkAccent},
    {'name': '日用品', 'icon': Icons.chair, 'color': Colors.tealAccent},
  ];
  
  int _selectedCategoryIndex = 0; // 初期値：食費

  @override
  void dispose() {
    _priceController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.only(
        top: 24, left: 24, right: 24,
        bottom: MediaQuery.of(context).viewInsets.bottom + 24,
      ),
      child: Column(
        mainAxisSize: MainAxisSize.min,
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          const Text('支出の記録', style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
          const SizedBox(height: 20),

          // 金額入力フィールド
          TextField(
            controller: _priceController,
            keyboardType: TextInputType.number,
            style: const TextStyle(fontSize: 24, fontWeight: FontWeight.bold),
            decoration: InputDecoration(
              prefixText: '¥ ',
              hintText: '0',
              hintStyle: const TextStyle(color: Colors.grey),
              filled: true,
              fillColor: const Color(0xFF12141C),
              border: OutlineInputBorder(borderRadius: BorderRadius.circular(16), borderSide: BorderSide.none),
              contentPadding: const EdgeInsets.all(20),
            ),
          ),
          const SizedBox(height: 20),

          // 🆕 カテゴリ選択（横スクロール）
          const Text('カテゴリ', style: TextStyle(color: Colors.grey, fontSize: 14)),
          const SizedBox(height: 8),
          SizedBox(
            height: 50,
            child: ListView.builder(
              scrollDirection: Axis.horizontal,
              itemCount: _categories.length,
              itemBuilder: (context, index) {
                final cat = _categories[index];
                final isSelected = _selectedCategoryIndex == index;
                return Padding(
                  padding: const EdgeInsets.only(right: 8.0),
                  child: ChoiceChip(
                    avatar: Icon(cat['icon'], color: isSelected ? Colors.white : cat['color'], size: 18),
                    label: Text(cat['name']),
                    selected: isSelected,
                    onSelected: (bool selected) {
                      if (selected) setState(() => _selectedCategoryIndex = index);
                    },
                    selectedColor: cat['color'].withOpacity(0.8),
                    backgroundColor: const Color(0xFF12141C),
                    labelStyle: TextStyle(color: isSelected ? Colors.white : Colors.grey, fontWeight: FontWeight.bold),
                    shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
                  ),
                );
              },
            ),
          ),
          const SizedBox(height: 20),

          // 税区分
          const Text('税区分', style: TextStyle(color: Colors.grey, fontSize: 14)),
          const SizedBox(height: 8),
          Row(
            children: [
              _buildChoiceChip('税込 (1)', 1, _selectedType == 1, () => setState(() => _selectedType = 1)),
              const SizedBox(width: 12),
              _buildChoiceChip('税抜 (2)', 2, _selectedType == 2, () => setState(() => _selectedType = 2)),
            ],
          ),
          const SizedBox(height: 20),

          // 税率区分
          const Text('税率', style: TextStyle(color: Colors.grey, fontSize: 14)),
          const SizedBox(height: 8),
          Row(
            children: [
              _buildChoiceChip('軽税 8% (1)', 1, _selectedRate == 1, () => setState(() => _selectedRate = 1)),
              const SizedBox(width: 12),
              _buildChoiceChip('標準 10% (2)', 2, _selectedRate == 2, () => setState(() => _selectedRate = 2)),
            ],
          ),
          const SizedBox(height: 32),

          // 保存ボタン
          SizedBox(
            width: double.infinity,
            height: 56,
            child: ElevatedButton(
              onPressed: () {
                final price = int.tryParse(_priceController.text) ?? 0;
                final selectedCat = _categories[_selectedCategoryIndex];
                
                // 選択したカテゴリ情報も一緒に渡す
                widget.onSave(
                  price, 
                  _selectedType, 
                  _selectedRate,
                  selectedCat['name'],
                  selectedCat['icon'],
                  selectedCat['color']
                );
                Navigator.pop(context);
              },
              style: ElevatedButton.styleFrom(
                backgroundColor: const Color(0xFF4F46E5),
                shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
              ),
              child: const Text('C++に計算させて保存', style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold, color: Colors.white)),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildChoiceChip(String label, int value, bool isSelected, VoidCallback onSelected) {
    return ChoiceChip(
      label: Text(label),
      selected: isSelected,
      onSelected: (bool selected) {
        if (selected) onSelected();
      },
      selectedColor: const Color(0xFF4F46E5),
      backgroundColor: const Color(0xFF12141C),
      labelStyle: TextStyle(color: isSelected ? Colors.white : Colors.grey, fontWeight: FontWeight.bold),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
    );
  }
}
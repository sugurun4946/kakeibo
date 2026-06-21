import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';

void main() {
  runApp(const MaterialApp(
    home: MainTabScreen(),
  ));
}

// 🛑 「const」を削除し、正しい構文に修正しました
class MainTabScreen extends StatefulWidget {
  const MainTabScreen({super.key});

  @override
  State<MainTabScreen> createState() => _MainTabScreenState();
}

class _MainTabScreenState extends State<MainTabScreen> {
  int _selectedIndex = 0;

  final List<Widget> _screens = [
    const ExpenseInputScreen(),    // 1つ目のタブ：入力画面
    const ExpenseAnalysisScreen(), // 2つ目のタブ：分析画面
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: _screens[_selectedIndex],
      bottomNavigationBar: BottomNavigationBar(
        currentIndex: _selectedIndex,
        onTap: (index) => setState(() => _selectedIndex = index),
        items: const [
          BottomNavigationBarItem(icon: Icon(Icons.edit), label: '入力'),
          BottomNavigationBarItem(icon: Icon(Icons.bar_chart), label: '分析'),
        ],
      ),
    );
  }
}

// ==========================================
// 1. 家計簿入力画面 (C++のロジック対応)
// ==========================================
class ExpenseInputScreen extends StatefulWidget {
  const ExpenseInputScreen({super.key});

  @override
  State<ExpenseInputScreen> createState() => _ExpenseInputScreenState();
}

class _ExpenseInputScreenState extends State<ExpenseInputScreen> {
  final TextEditingController _dateController = TextEditingController(text: '2026/06/21');
  final TextEditingController _amountController = TextEditingController();
  final TextEditingController _itemController = TextEditingController();
  final TextEditingController _memoController = TextEditingController();

  String _selectedCategory = '食費';
  String _selectedTaxType = '税込';
  String _selectedTaxRate = '10%';

  final List<String> _categories = ['食費', '娯楽・交際費', '日用品費', '交通費', '医療費', '通信費', '水道光熱費', 'その他'];
  final List<String> _taxTypes = ['税込', '税抜'];
  final List<String> _taxRates = ['8%', '10%'];

  final List<Map<String, dynamic>> _uiExpenses = [];

  void _addExpense() {
    final int amount = int.tryParse(_amountController.text) ?? 0;
    if (amount <= 0 || _itemController.text.isEmpty) return;

    setState(() {
      _uiExpenses.add({
        'date': _dateController.text,
        'amount': amount,
        'item': _itemController.text,
        'category': _selectedCategory,
        'taxType': _selectedTaxType,
        'taxRate': _selectedTaxRate,
        'memo': _memoController.text,
      });
      _amountController.clear();
      _itemController.clear();
      _memoController.clear();
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('家計簿入力 (C++連携)'), 
        backgroundColor: Colors.blueGrey[800], 
        foregroundColor: Colors.white
      ),
      body: Column(
        children: [
          Card(
            margin: const EdgeInsets.all(12),
            child: Padding(
              padding: const EdgeInsets.all(12.0),
              child: Column(
                children: [
                  Row(
                    children: [
                      Expanded(child: TextField(controller: _dateController, decoration: const InputDecoration(labelText: '日付'))),
                      const SizedBox(width: 12),
                      Expanded(child: TextField(controller: _itemController, decoration: const InputDecoration(labelText: '品目'))),
                    ],
                  ),
                  Row(
                    children: [
                      Expanded(child: TextField(controller: _amountController, keyboardType: TextInputType.number, decoration: const InputDecoration(labelText: '金額', suffixText: '円'))),
                      const SizedBox(width: 12),
                      Expanded(
                        child: DropdownButtonFormField<String>(
                          value: _selectedCategory,
                          decoration: const InputDecoration(labelText: 'カテゴリ'),
                          items: _categories.map((c) => DropdownMenuItem(value: c, child: Text(c, style: const TextStyle(fontSize: 12)))).toList(),
                          onChanged: (val) => setState(() => _selectedCategory = val!),
                        ),
                      ),
                    ],
                  ),
                  Row(
                    children: [
                      Expanded(
                        child: DropdownButtonFormField<String>(
                          value: _selectedTaxType,
                          decoration: const InputDecoration(labelText: '税区分'),
                          items: _taxTypes.map((t) => DropdownMenuItem(value: t, child: Text(t))).toList(),
                          onChanged: (val) => setState(() => _selectedTaxType = val!),
                        ),
                      ),
                      const SizedBox(width: 12),
                      Expanded(
                        child: DropdownButtonFormField<String>(
                          value: _selectedTaxRate,
                          decoration: const InputDecoration(labelText: '税率'),
                          items: _taxRates.map((r) => DropdownMenuItem(value: r, child: Text(r))).toList(),
                          onChanged: (val) => setState(() => _selectedTaxRate = val!),
                        ),
                      ),
                    ],
                  ),
                  TextField(controller: _memoController, decoration: const InputDecoration(labelText: 'メモ')),
                  const SizedBox(height: 12),
                  ElevatedButton(
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.blueGrey[700], minimumSize: const Size.fromHeight(40)),
                    onPressed: _addExpense,
                    child: const Text('C++へ追加', style: TextStyle(color: Colors.white)),
                  )
                ],
              ),
            ),
          ),
          Expanded(
            child: ListView.builder(
              itemCount: _uiExpenses.length,
              itemBuilder: (context, index) {
                final item = _uiExpenses[index];
                return ListTile(
                  title: Text('${item['item']} (${item['category']})'),
                  subtitle: Text('${item['date']} - ${item['taxType']}'),
                  trailing: Text('${item['amount']} 円', style: const TextStyle(fontWeight: FontWeight.bold)),
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}

// ==========================================
// 2. グラフ・分析画面 (C++集計データ用)
// ==========================================
class ExpenseAnalysisScreen extends StatefulWidget {
  const ExpenseAnalysisScreen({super.key});

  @override
  State<ExpenseAnalysisScreen> createState() => _ExpenseAnalysisScreenState();
}

class _ExpenseAnalysisScreenState extends State<ExpenseAnalysisScreen> {
  final Map<String, int> _monthlyData = {'4月': 45000, '5月': 62000, '6月': 38000};
  final Map<String, int> _categoryData = {'食費': 18000, '娯楽・交際費': 8000, '日用品費': 5000, '交通費': 3000, 'その他': 4000};
  final List<Color> _colors = [Colors.redAccent, Colors.orangeAccent, Colors.yellowAccent, Colors.blueAccent, Colors.grey];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('収支分析'), backgroundColor: Colors.blueGrey[800], foregroundColor: Colors.white),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              const Text('📈 月別の推移', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
              const SizedBox(height: 16),
              _buildBarChart(),
              const SizedBox(height: 32),
              const Text('🍕 カテゴリ別内訳', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
              const SizedBox(height: 16),
              _buildPieChart(),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildBarChart() {
    final months = _monthlyData.keys.toList();
    final values = _monthlyData.values.toList();
    return SizedBox(
      height: 150,
      child: BarChart(
        BarChartData(
          alignment: BarChartAlignment.spaceAround,
          maxY: 80000,
          barGroups: List.generate(months.length, (index) {
            return BarChartGroupData(x: index, barRods: [BarChartRodData(toY: values[index].toDouble(), color: Colors.teal, width: 25)]);
          }),
          titlesData: FlTitlesData(
            leftTitles: const AxisTitles(sideTitles: SideTitles(showTitles: false)),
            rightTitles: const AxisTitles(sideTitles: SideTitles(showTitles: false)),
            topTitles: const AxisTitles(sideTitles: SideTitles(showTitles: false)),
            bottomTitles: AxisTitles(
              sideTitles: SideTitles(showTitles: true, getTitlesWidget: (val, meta) => Text(months[val.toInt()], style: const TextStyle(fontSize: 12))),
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildPieChart() {
    final categories = _categoryData.keys.toList();
    final values = _categoryData.values.toList();
    final total = values.fold<int>(0, (sum, item) => sum + item);

    return Row(
      children: [
        Expanded(
          flex: 4,
          child: SizedBox(
            height: 140,
            child: PieChart(
              PieChartData(
                sectionsSpace: 2,
                centerSpaceRadius: 25,
                sections: List.generate(categories.length, (index) {
                  final pct = (values[index] / total * 100).toStringAsFixed(1);
                  return PieChartSectionData(
                    color: _colors[index % _colors.length],
                    value: values[index].toDouble(),
                    title: '$pct%',
                    radius: 40,
                    titleStyle: const TextStyle(fontSize: 10, fontWeight: FontWeight.bold),
                  );
                }),
              ),
            ),
          ),
        ),
        Expanded(
          flex: 5,
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: List.generate(categories.length, (index) {
              return Text('${categories[index]}: ${values[index]}円', style: const TextStyle(fontSize: 12));
            }),
          ),
        ),
      ],
    );
  }
}
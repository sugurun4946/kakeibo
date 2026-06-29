import 'dart:ffi' as ffi;
import 'dart:io';
import 'package:flutter/material.dart';

// ==========================================
// 1. C++（FFI）を呼び出すための設定
// ==========================================
typedef CGetAmountFunc = ffi.Int32 Function(ffi.Int32, ffi.Int32, ffi.Int32);
typedef DartGetAmountFunc = int Function(int, int, int);

// ボタンを押したときに実行される関数
void testCplusplusConnection() {
  try {
    // 🔴 Linux用に修正：「native_expense.dll」から「libnative_expense.so」に変更します
    final dylib = ffi.DynamicLibrary.open('libnative_expense.so');

    // c_get_amount_including_tax 関数を探して紐付ける
    final DartGetAmountFunc getAmountIncludingTax = dylib
        .lookup<ffi.NativeFunction<CGetAmountFunc>>('c_get_amount_including_tax')
        .asFunction();

    // 実際にC++に計算させてみる (1000円、税抜:2、税率10%:2)
    int result = getAmountIncludingTax(1000, 2, 2);
    
    print('====================================');
    print('★ C++ FFI 接続成功（Linux環境）');
    print('★ C++が計算した税込金額: $result 円');
    print('====================================');
  } catch (e) {
    print('❌ C++の呼び出しに失敗しました: $e');
  }
}

// ==========================================
// 2. Flutterの画面（UI）を作る部分
// ==========================================
void main() {
  runApp(const MyApp());
}

// アプリ全体のデザインやテーマを決めるウインドウの枠
class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: '家計簿 C++ FFI テスト',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const TestScreen(), // 最初に出す画面を指定
    );
  }
}

// テスト用の画面本体
class TestScreen extends StatelessWidget {
  const TestScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Day 5: C++接続テスト面'),
      ),
      // body: 画面の中身。真ん中（Center）に配置する
      body: Center(
        child: ElevatedButton(
          // ボタンが押されたら、上で定義したC++を呼ぶ関数を実行する
          onPressed: () {
            testCplusplusConnection();
          },
          style: ElevatedButton.styleFrom(
            padding: const EdgeInsets.symmetric(horizontal: 30, vertical: 15),
          ),
          child: const Text(
            'C++接続テストボタンを押す',
            style: TextStyle(fontSize: 18),
          ),
        ),
      ),
    );
  }
}
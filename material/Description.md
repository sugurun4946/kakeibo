# FFI 関連コードの説明

本資料は、今回の家計簿アプリの実装をもとに、Flutter(Dart)とC++間でFFI連携を行うための主要コードとその役割を初心者向けに解説したものです。

---

## 1. FFIとは
FFIの概要：何のために使うの？  
FFIを一言でいうと、「あるプログラミング言語から、別のプログラミング言語で作られたプログラム（関数）を直接呼び出して使うための仕組み」です。

通常、プログラミング言語はそれぞれ独自の「言語の壁（異なるメモリ管理や実行環境）」を持っています。しかし、FFIという架け橋を使うことで、言語間の壁を越えてデータをやり取りできるようになります。





## 2. C++ 側：共有ライブラリの窓口（ABIの固定）

C++の高度な機能（クラスやオーバーロード）は、コンパイル時に関数名が自動的に書き換わってしまう（名前修飾/マングル）ため、Dartから探せなくなります。そのため、**C言語の単純な互換ルール（C ABI）**に合わせた窓口（ラッパー関数）を用意します。

### ■ コード抜粋 (`ExpenseManager.cc`)
※ ExpenseManager.cc : 入力された価格や品目を処理するロジックが実装
※ getAmountIncludingTax : 税込み金額を計算する関数

```cpp
#include "ExpenseManager.h"

// ─── 【重要】ここからFFI用の窓口 ───
extern "C" {

    int c_get_amount_including_tax(int price, int type, int rate) {
        
        // ① 内部でC++のクラスをインスタンス化
        ExpenseManager manager;
        
        // ② クラスのメソッドを呼び出して計算結果をそのまま返す
        return manager.getAmountIncludingTax(price, type, rate);
    }

}
// ─── FFI用の窓口 ここまで ───
```
C++コードの役割解説  
- extern "C" { ... }：C++コンパイラに対し「関数名を勝手に書き換えるな」と指示します。これにより、バイナリの中に c_get_amount_including_tax という名前がそのまま綺麗に残るため、Dart側から文字列でピンポイント検索できるようになります。

- int c_get_amount_including_tax(...)：Dartから直接データを受け取るための窓口関数です。データのやり取りがしやすいよう、シンプルな int 型を引数と戻り値に指定しています。

- 関数の中身：外向けの窓口はC言語スタイルですが、内部（{ } の中）では ExpenseManager クラスを自由に動かすことができます。

## 3. Flutter(Dart) 側：ライブラリのロードと関数の紐付け
Dart側では、C++の関数が「どのような引数と戻り値を持つか」のルールブック（型定義）を用意し、メモリ上に読み込んだライブラリと関数を合体させます。

```
import 'dart:ffi' as ffi;
import 'dart:io';

// ① C++の世界での型ルールを定義（引数3つがInt32で、戻り値もInt32）
typedef CGetAmountFunc = ffi.Int32 Function(ffi.Int32, ffi.Int32, ffi.Int32);

// ② Dartの世界での型ルールを定義（いつもの int 型に変換するためのもの）
typedef DartGetAmountFunc = int Function(int, int, int);

void testCplusplusConnection() {
  try {
    // ③ 実行環境のパスから「libnative_expense.so」の絶対パスを作成
    final cppLibPath = '${Directory.current.path}/libnative_expense.so';

    // ④ 作成した絶対パスを使って、共有ライブラリをメモリ上にロードする
    final dylib = ffi.DynamicLibrary.open(cppLibPath);

    // ⑤ C++の関数名を探し、Dartの関数オブジェクト（getAmountIncludingTax）として紐付ける
    final DartGetAmountFunc getAmountIncludingTax = dylib
        .lookup<ffi.NativeFunction<CGetAmountFunc>>('c_get_amount_including_tax')
        .asFunction();

    // ⑥ 紐付けた関数を実行して、C++に直接計算を行わせる
    // 価格が1000円、2は税抜き、2は消費税率10%という意味
    int result = getAmountIncludingTax(1000, 2, 2);
    print('★ C++が計算した税込金額: $result 円');

  } catch (e) {
    print('❌ C++の呼び出しに失敗しました: $e');
  }
}
```

- ffi.Int32（①）：C++側の int（4バイト整数）に対応するDart FFI専用の型です。C++の世界のデータサイズを正確にDartに教えるために記述します。

- DynamicLibrary.open(...)（④）：ビルドして作ったC++のバイナリ部品（.so）をアプリの実行中に動的に読み込みます。

- dylib.lookup(...)（⑤）：ロードしたバイナリのメモリ内から、C++側で名前を固定した 'c_get_amount_including_tax' という名前（アドレス）を検索します。

- .asFunction()（⑤）：見つけたC++の関数の場所を、Dart側で安全に実行できる「普通のDartの関数」へと変換します。これで国境を超えたパイプラインが開通します。


## 3. なぜDartは「CのABI」しか理解できないのか？
結論から言うと、「C言語のABIが、OSやあらゆる言語における『世界共通語』だから」です。

理由①：C++のABIには標準（ルール）がない
C++はコンパイラ（GCC、Clang、MSVCなど）やバージョンによって、名前修飾（マングル）のルールや、メモリ上にクラスオブジェクトをどう配置するかというルール（ABI）がバラバラです。Dart側が「すべてのC++コンパイラのルール」に対応するのは不可能なため、C++をそのまま解釈することはできません。

理由②：CのABIは完全に標準化されている
一方で、C言語のABIは「データの並び順」や「関数の呼び出し方」がOSレベルで厳格に標準化されています。そのため、Dartに限らず、Python、Rust、Goなど、ほぼすべての言語のFFIは「共通の言語」としてCのABIを採用しています。
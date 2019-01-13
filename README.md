# dptl_1
deep pointer template library (DPTL) is a template library that can be used in C++11 and later C++.

Deep pointer is a smart pointer for handling data and its pointer collectively. Processing the pointed data like duplication, comparison, storing to containers and destruction can be done more intuitively and simply as operations on the pointer.

Deep pointer is designed with the aim of making less likely to occur problems even if C style is used in combination under C++ environment.
With C++11 and later, C++ has evolved greatly to handling complex data structures with sufficient performance, using rvalue references and smart pointers. However, the difference between the programming style of standard libraries (that center on values and references) and the style of C (centered around pointers) is deterministic. In the C++ standard programming style, C native pointer, string, and array should be shifted to smart pointer, std::string, and std::array.
Under such circumstances, many people feel that it is more difficult to write correct code in the complicated C++ language specification than in simple C style. There are many cases where it is difficult to acquire the skills for project members, because learning time to write appropriate code is too long. Coordination with libraries and system calls written in C may also be an issue.

Using deep pointer makes it possible to use native pointers, strings, and arrays safely and simply in Modern C++ environment. In particular, the containers of the standard template library will be able to handle native pointers, strings, and arrays almost transparently and without discomfort through deep pointers.

Please refer <a href="https://issay376.github.io/dptl_1/docs/readme_en.html">readme</a> for more details.

---

deep pointer template libray (以下DPTLと略す）は、C++11以降のC++で使用できるテンプレートライブラリーです。

deep pointerは、C++のポインターとポインターの指し先のデータをひとまとめにして扱うためのスマートポインターです。指し先のデータのメモリ管理、比較処理、コンテナへの格納などを、ポインターへの操作として、より直感的かつシンプルに行うことができます。

deep pointerの設計で、特に留意したのは、C と C++ とのギャップを埋めることです。
C++11 以降、右辺参照やスマートポインターによって、複雑なデータ構造とパフォーマンスの両立の面でも、C++は大きく進化しました。しかし、値とリファレンスを中心に置く標準ライブラリのプログラミング・スタイルと、ポインタを中心に置くCのスタイルとの違いは決定的で、C++の標準的なプログラミング・スタイルでは、Cネイティブのポインタ、文字列、配列などは使用を極力控え、スマートポインタ、stringクラス、arrayクラスに移行すべきとされるようになりました。
こうした中で、さらに複雑になった言語仕様を前に、Cのスタイルに比べて、C++ではシンプルで正しいコードを書くことが難しいと感じている方も多いのではないでしょうか。また、適切なレベルのコードが書けるまでの習得時間が長く、プロジェクト・メンバーのスキル確保に苦労されているケースも多いと思います。Cで書かれたライブラリやシステムコールとの連携も課題になる可能性があるでしょう。

deep pointer は、こうした問題に対して、C++ 上で C のスタイルを併用しても問題が生じにくくすることを目標に設計されています。
deep pointer を使うことで、ネイティブのポインタ、文字列、配列などを、Modern C++ の環境で安全かつシンプルに利用できるようになります。特に、標準テンプレートライブラリのコンテナは、deep pointer を介して、ほぼ透過的に違和感なくネイティブのポインタ、配列、文字列を扱うことができるようになります。

詳細は、<a href="https://issay376.github.io/dptl_1/docs/readme_ja.html">readme</a> をご覧ください。

---

deep pointer template library is open source software without warranties any kind.

© Kazunari Saitoh 2018 - 2019.
Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt ) 

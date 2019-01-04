# dptl_0.1
deep pointer template library (DPTL) is a template library that can be used in C++11 and later C++.

Deep pointer is a smart pointer for handling data and a pointer pointing the data collectively. Processing the pointed data like duplication, destruction, comparison, storing and retrieving from the container can be done more intuitively and simply as operations on the pointer.

In the design of deep pointer, I especially cared about filling the gap between C and C++. With C++11 and later, C++ has evolved greatly to handling complex data structures with sufficient performance, using rvalue references and smart pointers. However, the difference between the programming style of standard libraries (that center on values ​​and references) and the style of C (centered around pointers) is deterministic, and in the C++ standard programming style, C native pointer, string, and array should be shifted to smart pointer, string class, and array class. Under such circumstances, many people feel that it is more difficult to write correct code in the complicated C++ language specification than in simple C style. Also, there are many cases where it is difficult to acquire the skills of project members, because learning time to write appropriate level code is too long. Coordination with libraries and system calls written in C may also be an issue.

Deep pointer is designed with the aim of making less likely to occur problems even if C style is used in combination on C++. Using deep pointer makes it possible to use native pointers, strings, and arrays safely and simply in Modern C++ environment. In particular, the containers of the standard template library will be able to handle native pointers, strings, and arrays almost transparently and without discomfort through deep pointers. 

deep pointer template library is open source software without warranties any kind.

© Kazunari Saitoh 2018 - 2019.
Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt ) 

Please refer doc/readme_en.html or doc/readme_jp.html for more details.

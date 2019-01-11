# 编译大作业文档

## 小组成员

- 洪方舟
  - 学号：2016013259
  - 班级：软件62
- 李仁杰
  - 学号：2016013271
  - 班级：软件63
- 张佳麟
  - 学号：2016013256
  - 班级：软件62

## 项目简介

- 实现一个C语言到LLVM(Version 7.0)的编译器
- 使用LLVM-IR的C++API来进行中间代码的构建
- 能够实现大部分的C语言语法特性（图灵完备的C语言语法子集）
- 能够顺利编译指定的三个程序

## 开发环境

- flex 2.6.4
- bison (GNU Bison) 2.3
- clang version 7.0.0
- llvm version 7.0

## 运行方式

- 编译并运行指定的三个程序
  - 回文检测：`make target_p`
  - KMP：`make target_kmp`
  - 表达式求值：`make target_calc`
- 编译并运行自定义程序
  - 将所需要编译的文件放到`src`文件夹下，命名为`test.c`
  - 执行命令：`make run`
- 构建中可能出现的问题
  - 本项目基于LLVM7.0版本进行开发， 由于LLVM各个版本之间的API有一定的区别，因此环境中LLVM版本不对可能会导致本项目构建失败
  - 生成的LLVM文件存放在`out.ll`中

##支持的语法特性

- 基本数据类型
- 基本的隐式类型转换
- 基本的流程控制语句：`if, for, while, break, continue`
- 函数的声明、定义、外部函数声明（`extern`）
- 变量的声明（不支持声明时初始化）
- 几乎完整的表达式系统
- 支持数组
- 支持指针
- 语句块

## 系统运行概览

- 首先通过lex生成的词法分析器进行词法分析
- 通过bison生成的语法分析器进行语法与语义分析，构造抽象语法树
- 通过遍历抽象语法树，调用LLVM-IR的API，构建LLVM
- 通过LLVM-IR的API生成LLVM文件`*.ll`
- 调用clang编译`*.ll`生成可执行文件

## 系统架构

### 词法分析器

- 使用Lex实现了语法分析器
- 对应的源文件为`token.l`
- 从输入流中识别token后将token流交给语法分析器分析

### 语法分析&语义分析器

- 使用bison实现了语法分析器
- 对应的源文件为`grammar.y`
- 在语法分析时通过语法制导的语义分析构建一颗语法分析树

### 抽象语法树

我们定义的抽象语法树的类图如下

![编译器类图 (assets/编译器类图 (6).jpg)](../../../../../Desktop/编译器类图 (6).jpg)

我们主要的抽象结点类有

- `scNStatement`也即一个无返回值的语句
- `scNExpression`也即有返回值的语句

### Context

- 翻译过程中我们将需要维护的信息存储在`Context`当中
- Context中维护的信息包括
  - 符号表
  - Break-to Block (BTB) 记录了当前循环中如果执行`break`语句，所跳转到的位置
  - Continue-to Block (CTB) 记录了当前循环中如果执行`continue`语句，所跳转到的位置
  - LLVM-IR的API中定义的上下文与构建器

## 重难点问题解决

下面主要介绍我们处理C语言中指针与数组的方法

## 参考资料

- [Kaleidoscope Tutorial](http://llvm.org/docs/tutorial/LangImpl01.html)
- [LLVM Language Reference Manual](http://llvm.org/docs/LangRef.html)
- [LLVM Programmer’s Manual](http://llvm.org/docs/ProgrammersManual.html)
- [LLVM API Class Document](http://llvm.org/doxygen/)
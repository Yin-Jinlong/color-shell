# Color-Shell 彩色的Windows Shell ![Terminal](https://img.shields.io/badge/%3E__-3c3c3c)

[//]: # (概览)
![GitHub License](https://img.shields.io/github/license/Yin-Jinlong/color-shell)
![c/c++](https://img.shields.io/badge/c/c%2B%2B-v23-00589d)

[//]: # (仓库信息)
![GitHub repo size](https://img.shields.io/github/repo-size/Yin-Jinlong/color-shell)
![GitHub forks](https://img.shields.io/github/forks/Yin-Jinlong/color-shell)
![GitHub Repo stars](https://img.shields.io/github/stars/Yin-Jinlong/color-shell)

[//]: # (统计)
![Total Lines](https://img.shields.io/badge/total_lines-3,298-9a9a9a)
![Source Lines](https://img.shields.io/badge/source_lines-1,486-9a9a9a)

[//]: # (活动)
![GitHub commit activity (branch)](https://img.shields.io/github/commit-activity/m/Yin-Jinlong/color-shell)
![GitHub contributors](https://img.shields.io/github/contributors/Yin-Jinlong/color-shell)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/Yin-Jinlong/color-shell/main)

程序使用`UTF-8`环境。

## 项目结构🗂️

**项目📦**

- [commands](./commands/README.md)💼
    - [cmds](./commands/README.md)🧰
        - [clear](./commands/cmds/clear/README.md)📘
        - [mkdir](./commands/cmds/mkdir/README.md)📘
        - [pwd](./commands/cmds/pwd/README.md)📘
        - CMakeLists.txt⚙️
    - CMakeLists.txt⚙️
- [csh](./csh/README.md)💼
- [csh-lib](./csh-lib/README.md)💼
- include🏷️公开可用头文件(csh-lib)
- [extras](./extras/README.md)🧰
    - [git](./extras/git/README.md)📙
    - [node](./extras/node/README.md)📙
- out📁项目输出
    - bin🛞
    - extras🔧
    - LICENSE.md⚖️
- CMakeLists.txt⚙️
- [LICENSE.md](./LICENSE.md)⚖️

## 环境要求 🌏

- Win 10 以上 🪟
- 支持Unicode [ANSI](https://zh.wikipedia.org/wiki/ANSI%E8%BD%AC%E4%B9%89%E5%BA%8F%E5%88%97)
  转义的基本终端(PS,Windows Terminal)

## 编译 🔨

构建目标`build`，所有输出在`out`目录下

## Windows Terminal配置 ![Terminal](https://img.shields.io/badge/%3E__-3c3c3c)

1. 打开终端设置，添加新配置文件
2. 设置名称`ColorShell`
3. 命令行为本程序路径
4. 外观中配置Nerd字体(推荐[JetBrainsMono Nerd](https://www.nerdfonts.com/font-downloads))

## 进度 🏗️⏳♾️

**基本**

- ✅ 高亮显示*
- ✅ 当前用户名
- ✅ 工作目录
- ✅ 直接输入（不阻塞）
- ✅ 历史命令
- ✅ 持久化历史命令
- 🔲 命令补全
- ✅ 执行结果*
- ❌ 管道
- ❌ 重定向输入
- ❌ 重定向输出
- ❌ 配置
- ❌ 自定义配置
- ✅ [插件](./extras/README.md)*

<small style="color:rgba(128,128,128,0.5)">注：*为基本完成，未勾选标识正在进行或将要进行</small>

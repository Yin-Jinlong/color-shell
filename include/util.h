#pragma once

#include <predef.h>
#include <Windows.h>

/**
 * 获取当前目录
 * @return 当前目录
 */
extern DLL_OUT str getCurrentDirectory();

/**
 * 获取进程输出
 * @param cmdLine 命令行
 * @return 进程输出
 */
extern DLL_OUT str getProcessOutput(const str &cmdLine);

/**
 * 获取环境变量
 * @param name 环境变量名
 * @return 环境变量值
 */
extern DLL_OUT str getEnv(const str &name);

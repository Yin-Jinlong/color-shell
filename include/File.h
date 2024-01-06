#pragma once

#include <cstdlib>
#include <string>
#include <predef.h>
#include <Windows.h>
#include <FileStream.h>

#ifndef F_OK
#define F_OK 0
#endif

#ifndef W_OK
#define W_OK 2
#endif

#ifndef R_OK
#define R_OK 4
#endif

#ifndef RW_OK
#define RW_OK (W_OK | R_OK)
#endif

namespace csh {
    /**
     * 文件
     */
    class DLL_OUT File {
    private:
        /**
         * 路径
         */
        std::wstring path;
        /**
         * 文件状态
         */
        struct _stat32i64 stat{};

        /**
         * 更新文件状态
         */
        int update();
    public:
        /**
         * 构造函数
         *
         * @param path 路径
         */
        explicit File(std::wstring path);

        /**
         * 构造函数
         *
         * @param dir 目录
         * @param child 子文件
         */
        explicit File(std::wstring dir, std::wstring child);

        ~File();

        /**
         * 获取绝对路径
         *
         * @return 绝对路径
         */
        USE_RET std::wstring absolutePath() const;

        /**
         * 是否存在
         *
         * @return 是否存在
         */
        USE_RET bool exists() const;

        /**
         * 是否可读
         *
         * @return 是否可读
         */
        USE_RET bool canRead() const;

        /**
         * 是否可写
         *
         * @return 是否可写
         */
        USE_RET bool canWrite() const;

        /**
         * 是否目录
         *
         * @return 是否目录
         */
        USE_RET bool isDir();

        /**
         * 是否文件
         *
         * @return 是否文件
         */
        USE_RET bool isFile();

        /**
         * 文件度
         *
         * @return 长度
         */
        USE_RET u64 length();

        /**
         * 最后修改时间
         *
         * @return 最后修改时间
         */
        USE_RET u32 lastModified();

        /**
         * 删除文件
         *
         * @return 是否删除成功
         */
        USE_RET bool del() const;

        /**
         * 打开文件
         *
         * @param mode 模式
         * @return 文件流
         */
        csh::FileStream open(const wchar_t *mode);

        /**
         * 获取路径
         *
         * @return 路径
         */
        USE_RET std::wstring getPath() const;

        /**
         * 获取文件名
         *
         * @return 文件名
         */
        USE_RET std::wstring getFileName() const;

        /**
         * 获取父目录
         *
         * @return 父目录
         */
        USE_RET File getParent() const;

        /**
         * 比较
         *
         * @param other 其他文件
         * @return 是否相等
         */
        bool operator==(const File &other) const;

    };
}

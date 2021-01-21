#define _CRT_SECURE_NO_WARNINGS
#include "DeviceDriver.h"

const char * DeviceDriver::DISK_FILE_NAME = "unix2nd-fs.img";

DeviceDriver::DeviceDriver() {
    fp = fopen(DISK_FILE_NAME, "rb+");//rb+ 读写打开一个二进制文件，只允许读写数据。
}

DeviceDriver::~DeviceDriver() {
    if (fp) {
        fclose(fp);
    }
}

/* 检查镜像文件是否存在 */
bool DeviceDriver::Exists() {
    return fp != NULL;
}

/* 打开镜像文件 */
void DeviceDriver::Construct() {
    fp = fopen(DISK_FILE_NAME, "wb+"); //wb+ 读写打开或建立一个二进制文件，允许读和写。
    if (fp == NULL) {
        printf("打开或新建文件%s失败！", DISK_FILE_NAME);
        exit(-1);
    }
}

/* 实际写磁盘函数 */
void DeviceDriver::write(const void* buffer, unsigned int size, int offset, unsigned int origin) {
    if (offset >= 0) {
        fseek(fp, offset, origin);
    }
    fwrite(buffer, size, 1, fp);
}

/* 实际读磁盘函数 */
void DeviceDriver::read(void* buffer, unsigned int size, int offset, unsigned int origin) {
    if (offset >= 0) {
        fseek(fp, offset, origin);
    }
    fread(buffer, size, 1, fp);
}

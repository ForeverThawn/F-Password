#include <string.h>
#include <vector>
#include <iostream>

class MainSwitch {
public:
    char* arg = nullptr;    // 可用时返回的optarg

    /*
    * 把argc argv 和 options选项字符串传进来就行
    * 然后用<<指定字符开关判断就OK
    */
    MainSwitch(const int argc, char** argv, const char* options);

    bool operator << (const int option);     // 判断有没有option 有true 无false

private:
    std::vector<int> _opt;
    std::vector<char*> _optarg;
    int argc;
    char** argv;
    const char* opts;

    int opterr = 1; // 控制错误信息的输出，如果为0则不输出错误信息
    int optind = 1; // 保存下一个要处理的参数在argv中的下标
    int optopt;     // 记录产生错误的选项字符
    char* optarg;

    int getopt();
};
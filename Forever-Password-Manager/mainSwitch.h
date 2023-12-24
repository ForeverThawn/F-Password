#include <string.h>
#include <vector>
#include <iostream>

class MainSwitch {
public:
    char* arg = nullptr;    // ����ʱ���ص�optarg

    /*
    * ��argc argv �� optionsѡ���ַ�������������
    * Ȼ����<<ָ���ַ������жϾ�OK
    */
    MainSwitch(const int argc, char** argv, const char* options);

    bool operator << (const int option);     // �ж���û��option ��true ��false

private:
    std::vector<int> _opt;
    std::vector<char*> _optarg;
    int argc;
    char** argv;
    const char* opts;

    int opterr = 1; // ���ƴ�����Ϣ����������Ϊ0�����������Ϣ
    int optind = 1; // ������һ��Ҫ����Ĳ�����argv�е��±�
    int optopt;     // ��¼���������ѡ���ַ�
    char* optarg;

    int getopt();
};
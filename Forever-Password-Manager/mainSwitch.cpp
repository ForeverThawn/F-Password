#include "mainSwitch.h"


MainSwitch::MainSwitch(const int argc, char** argv, const char* options) {
    this->argc = argc;
    this->argv = argv;
    this->opts = options;
    int ret;
    while ((ret = getopt()) != -1) {
        _opt.push_back(ret);
        _optarg.push_back(optarg);
    }
}

bool MainSwitch::operator << (const int option) {    // 判断有没有option 有true 无false
    bool ret = false;
    int index = 0;
    //for (const auto& check : _opt) {
    for (; index < _opt.size(); index++) {
        if (_opt[index] == option) {
            ret = true;
            break;
        }
    }
    if (ret == true)
        arg = _optarg[index];
    return ret;
}

int MainSwitch::getopt() {
    static int sp = 1;
    int c;
    char* cp;

    if (sp == 1) {
        if (optind >= argc ||
            argv[optind][0] != '-' || argv[optind][1] == '\0')
            return EOF;
        else if (!strcmp(argv[optind], "--")) {
            optind++;
            return EOF;
        }
    }
    optopt = c = argv[optind][sp];
    if (c == ':' || !(cp = (char*)strchr(opts, c))) {   // strchr新版本c++返回const 我不想让他返回
        fprintf(stderr, ": illegal option -- %c\n", c);
        if (argv[optind][++sp] == '\0') {
            optind++;
            sp = 1;
        }
        return '?';
    }
    if (*++cp == ':') {
        if (argv[optind][sp + 1] != '\0')
            optarg = &argv[optind++][sp + 1];
        else if (++optind >= argc) {
            fprintf(stderr, ": option requires an argument -- %c\n", c);
            sp = 1;
            return '?';
        }
        else
            optarg = argv[optind++];
        sp = 1;
    }
    else {
        if (argv[optind][++sp] == '\0') {
            sp = 1;
            optind++;
        }
        optarg = NULL;
    }

    return c;
}
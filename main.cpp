#include "INode.h"
#include "File.h"
#include "FileSystem.h"
#include "OpenFileManager.h"
#include "FileManager.h"
#include "User.h"
#include <iostream>
#include <unordered_map>
#include <conio.h>
using namespace std;

DeviceDriver gc_DeviceDriver;
BufferManager gc_BufferManager;
OpenFileTable gc_OpenFileTable;
SuperBlock gc_SuperBlock;
FileSystem gc_FileSystem;
INodeTable gc_INodeTable;
FileManager gc_FileManager;
User gc_User;

void man(string command) {

    static string man =
        "Command       :  man -显示指令说明 \n"
        "Description   :  给出指令格式，帮助用户使用相应命令 \n"
        "Usage         :  man [指令] \n"
        "Parameter     :  [指令] 如下：  \n"
        "                 man          :  说明 \n"
        "                 fformat      :  格式化文件卷 \n"
        "                 exit         :  正确退出 \n"
        "                 mkdir        :  新建目录 \n"
        "                 cd           :  改变目录 \n"
        "                 ls           :  列出目录及文件 \n"
        "                 fcreate      :  新建文件 \n"
        "                 fdelete      :  删除文件 \n"
        "                 fopen        :  打开文件 \n"
        "                 fclose       :  关闭文件 \n"
        "                 fseek        :  移动读写指针 \n"
        "                 fwrite       :  写入文件 \n"
        "                 fread        :  读取文件 \n"
		"                 copy         :  拷贝文件 \n"
		"                 copyin       :  从外部操作系统拷贝文件 \n"
		"                 copyout      :  拷贝文件到外部操作系统 \n"
		"                 more         :  以ASCII码形式显示文件内容 \n"
        "                 dt|demoTest  :  范例测试 \n"
        "Usage Demo    :  man mkdir \n"
        ;

    static string fformat =
        "Command       :  fformat -格式化文件卷 \n"
        "Description   :  格式化文件卷，将清空所有文件及目录! \n"
        "Usage         :  fformat \n"
        "Parameter     :  无 \n"
        "Usage Demo    :  fformat \n"
        ;

    static string exit =
        "Command       :  exit -退出文件系统 \n"
        "Description   :  退出文件系统，退出时直接关闭窗口，\n"
        "Usage         :  exit \n"
        "Parameter     :  无 \n"
        "Usage Demo    :  exit \n"
        ;

    static string mkdir =
        "Command       :  mkdir -新建目录 \n"
        "Description   :  新建目录。若出现错误，提示错误信息! \n"
        "Usage         :  mkdir <目录名> \n"
        "Parameter     :  <目录名>  相对路径或绝对路径 \n"
        "Usage Demo    :  mkdir dir \n"
        "                 mkdir ../dir \n"
        "                 mkdir ../../dir \n"
        "                 mkdir /dir \n"
        "                 mkdir /dir1/dir \n"
        "Error Avoided :  目录名过长，目录路径不存在，目录超出根目录等 \n"
        ;


    static string cd =
        "Command       :  cd -改变目录 \n"
        "Description   :  改变当前工作目录。若出现错误，提示错误信息! \n"
        "Usage         :  cd <目录名> \n"
        "Parameter     :  <目录名>默认为当前目录；\n"
        "                 <目录名> 可以是相对路径，也可以是绝对路径 \n"
        "Usage Demo    :  ls \n"
        "                 ls ../dir \n"
        "                 ls ../../dir \n"
        "                 ls /dir \n"
        "                 ls /dir1/dir \n"
        "Error Avoided :  目录名过长，目录超出根目录，目录路径不存在等等 \n"
        ;

	static string ls =
		"Command       :  ls -列出目录内容 \n"
		"Description   :  列出当前目录中包含的文件名或目录名。若出现错误，提示错误信息! \n"
		"Usage         :  ls \n"
		"Parameter     :  无 \n"
		"Usage Demo    :  ls \n"
		;

    static string fcreate =
        "Command       :  fcreate -新建文件 \n"
        "Description   :  新建指定文件。若出现错误，提示错误信息! \n"
        "Usage         :  fcreate <文件名> <选项> \n"
        "Parameter     :  <文件名> 可以包含相对路径或绝对路径 \n"
        "                 <选项> -r 只读属性 \n"
        "                 <选项> -w 只写属性 \n"
        "                 <选项> -rw 读写属性 \n"
        "Usage Demo    :  fcreate newfile -rw \n"
        "                 fcreate ../newfile -rw \n"
        "                 fcreate ../../newfile -rw \n"
        "                 fcreate /newfile -rw \n"
        "                 fcreate /dir/newfile -rw \n"
        "Error Avoided :  文件名过长，目录路径不存在，目录超出根目录等 \n"
        ;

    static string fopen =
        "Command       :  fopen -打开文件 \n"
        "Description   :  打开指定文件。文件的读写前必须先fopen。\n"
        "                 若出现错误，提示错误信息! \n"
        "Usage         :  fopen <文件名> <选项> \n"
        "Parameter     :  <文件名> 可以包含相对路径或绝对路径 \n"
        "                 <选项> -r 只读属性 \n"
        "                 <选项> -w 只写属性 \n"
        "                 <选项> -rw == -r -w 读写属性 \n"
        "Usage Demo    :  fopen file -r \n"
        "                 fopen ../file -w \n"
        "                 fopen ../../file -rw \n"
        "                 fopen /file -r -w \n"
        "                 fopen /dir/file -rw \n"
        ;

	static string fdelete =
		"Command       :  fdelete -删除文件 \n"
		"Description   :  删除指定文件。若出现错误，提示错误信息! \n"
		"Usage         :  fdelete <文件名> \n"
		"Parameter     :  <文件名> 可以包含相对路径或绝对路径 \n"
		"Usage Demo    :  fdelete file \n"
		"                 fdelete ../file \n"
		"                 fdelete ../../file \n"
		"                 fdelete /file \n"
		"                 fdelete /dir/file \n"
		;

    static string fclose =
        "Command       :  fclose -关闭文件 \n"
        "Description   :  关闭指定文件。关闭已经打开的文件。 \n"
        "                 若出现错误，提示错误信息! \n"
        "Usage         :  fclose <file descriptor> \n"
        "Parameter     :  <file descriptor> 文件描述符 \n"
        "Usage Demo    :  fclose 1 \n"
        ;

    static string fseek =
        "Command       :  fseek -定位文件读写指针 \n"
        "Description   :  设置文件的指针位置。若出现错误，提示错误信息! \n"
        "Usage         :  fseek <file descriptor> <offset> <origin> \n"
        "Parameter     :  <file descriptor> fopen返回的文件描述符, \n"
        "                 <offset> 指定从 <origin> 开始的偏移量,可正可负, \n"
        "                 <origin> 指定起始位置 可为0(SEEK_SET), 1(SEEK_CUR), 2(SEEK_END) \n"
        "Usage Demo    :  fseek 1 500 0 \n"
        ;

    static string fwrite =
        "Command       :  fwrite -写入文件 \n"
        "Description   :  写入指定文件数据。若出现错误，提示错误信息! \n"
        "Usage         :  fwrite <file descriptor> <Infile> <size> \n"
        "Parameter     :  <file descriptor> open返回的文件描述符 \n"
        "                 <Infile> 指定写入内容为文件Infile中的内容 \n"
        "                 <size> 指定写入字节数，大小为 <size> \n"
        "Usage Demo    :  fwrite 1 Infile 123 \n"
        ;

    static string fread =
        "Command       :  fread -读取文件 \n"
        "Description   :  从指定文件中读取数据。若出现错误，提示错误信息! \n"
        "Usage         :  fread <file descriptor> [-o <Outfile>] <size> \n"
        "Parameter     :  <file descriptor> open返回的文件描述符 \n"
        "                 [-o <Outfile>] -o 指定输出方式为文件，文件名为 <Outfile> 默认为shell \n"
        "                 <size> 指定读取字节数，大小为 <size> \n"
        "Usage Demo    :  fread 1 -o Outfile 123 \n"
        "                 fread 1 123 \n"
        ;

    static string copy =
        "Command       :  copy -拷贝文件 \n"
        "Description   :  拷贝文件。若出现错误，提示错误信息! \n"
        "Usage         :  copy <srcfile> <desfile> \n"
        "Parameter     :  <srcfile> 源文件名 \n"
        "                 <desfile> 目的文件名 \n"
        "Usage Demo    :  copy srcfile desfile \n"
        ;
    static string copyin =
        "Command       :  copyin -从外部操作系统拷贝文件 \n"
        "Description   :  从外部操作系统拷贝文件到二级文件系统。若出现错误，提示错误信息! \n"
        "Usage         :  copyin <srcfile> <desfile> \n"
        "Parameter     :  <srcfile> 外部操作系统的源文件名 \n"
        "                 <desfile> 二级文件系统的目的文件名 \n"
        "Usage Demo    :  copyin srcfile desfile \n"
        ;

    static string copyout =
        "Command       :  copyout -拷贝文件到外部操作系统 \n"
        "Description   :  拷贝二级文件系统文件到外部操作系统。若出现错误，提示错误信息! \n"
        "Usage         :  copyout <srcfile> <desfile> \n"
        "Parameter     :  <srcfile> 二级文件系统的源文件名 \n"
        "                 <desfile> 外部操作系统的目的文件名 \n"
        "Usage Demo    :  copyout srcfile desfile \n"
        ;
    static string more =
        "Command       :  more -以ASCII码形式显示文件内容 \n"
        "Description   :  以ASCII码形式显示文件内容。若出现错误，提示错误信息! \n"
        "Usage         :  more <file> \n"
        "Parameter     :  <file> 要显示的文件名 \n"
        "Usage Demo    :  more file \n"
        ;
    static string demoTest =
        "Command       :  demoTest -范例测试 \n"
        "Description   :  通过一系列测试用例，展示系统功能 \n"
        "Usage         :  demoTest | dt \n"
        "Parameter     :  无 \n"
        "Usage Demo    :  dt \n"
        ;

    static unordered_map<string, string*>manMap({
        { "man", &man },
        { "dt", &demoTest },
        { "fformat", &fformat },
        { "exit", &exit },
        { "mkdir", &mkdir },
        { "cd", &cd },
        { "ls", &ls },
        { "fcreate", &fcreate },
        { "fdelete", &fdelete },
        { "fopen", &fopen },
        { "fclose", &fclose },
        { "fseek", &fseek },
        { "fwrite", &fwrite },
        { "fread", &fread },
		{ "copy", &copy },
		{ "copyin", &copyin },
		{ "copyout", &copyout },
		{ "more", &more },
        { "demoTest", &demoTest },
        });

    auto it = manMap.find(command);
    if (it == manMap.end()) {
        cout << "shell : " << command << " : don't find this commond \n";
        return;
    }
    cout << *it->second;
}

bool autoTest(string& cmd) {
    static int testNo = 0;
    static const char* testCmds[] = {
        "mkdir /bin",
        "ls",
        "mkdir etc",
        "cd etc",
        "ls",
        "mkdir /dev",
        "cd /",
        "mkdir home",
        "mkdir /home/texts",
        "copyin File.h /home/texts/file.h",
        "ls",
        "cd /home",
        "mkdir photos",
        "cd photos",
        "copyin tp.jpg tpnew.jpg",
        "cd /home",
        "mkdir reports",
        "ls",
        "cd reports",
        "copyin 1754272-王云杉.pdf 1754272-王云杉.pdf",       
        "ls",
        "cd /",
        "mkdir test",
        "cd test",
        "fcreate Jerry -rw",
        "fopen Jerry -rw",
        "fwrite 12 file2000.txt 800", 
        "fclose 12",
        "more Jerry",
        "fopen Jerry -rw",
        "fseek 12 500 0",
        "fread 12 20", 
        "fclose 12",
    };
    int cmdNums = sizeof(testCmds) / sizeof(char*);
    cmd = testCmds[testNo % cmdNums];
    return ++testNo <= cmdNums;
}

void cmdArgs(const string& cmd, vector<string>& args) {
    args.clear();
    string str;
    unsigned int p, q;
    for (p = 0, q = 0; q < cmd.length(); p = q + 1) {
        q = cmd.find_first_of(" \n", p);
        str = cmd.substr(p, q - p);
        if (!str.empty()) {
            args.push_back(str);
        }
        if (q == string::npos)
            return;
    }
}

int main() {
    User* user = &gc_User;

    string line = "man";
    vector<string> args;
    string cmd, arg1, arg2, arg3;
    int autoFlag = 0;
    cout << "++++++++++++++++++++ UNIX二级文件系统 ++++++++++++++++++++" << endl;

    while (1) {
        if (line == "")
            goto NEXT_INPUT;

        cmdArgs(line, args);
        cmd = args[0];
        arg1 = args.size() > 1 ? args[1] : "";
        arg2 = args.size() > 2 ? args[2] : "";
        arg3 = args.size() > 3 ? args[3] : "";
        if (cmd == "man") {
            man(arg1.empty() ? "man" : arg1);
        }
        else if (cmd == "fformat") {
            gc_OpenFileTable.Format();
            gc_INodeTable.Format();
            gc_BufferManager.FormatBuffer();
            gc_FileSystem.FormatDevice();
            cout << "Format finish, System must reboot, Press any key exit ...";
            _getch();

            exit(0);
        }
        else if (cmd == "exit") {
            exit(0);
        }
        else if (cmd == "dt" || cmd == "demoTest") {
            autoFlag = 1;
            cout << "DemoTest beginning, Please press enter to continue ... \n" ;
        }
        else if (cmd == "mkdir") {
            user->Mkdir(args[1]);
        }
        else if (cmd == "ls") {
            user->Ls();
        }
        else if (cmd == "cd") {
            user->Cd(arg1);
        }
        else if (cmd == "fcreate") {
            //create 没有需要模式的必要
            user->Fcreate(arg1, arg2 + arg3);
        }
        else if (cmd == "fdelete") {
            user->Fdelete(arg1);
        }
        else if (cmd == "fopen") {
            user->Fopen(arg1, line);
        }
        else if (cmd == "fclose") {
            user->Fclose(arg1);
        }
        else if (cmd == "fseek") {
            user->Fseek(arg1, arg2, arg3);
        }
        else if (cmd == "fread") {
            if (arg2 == "-o")
                user->Fread(arg1, arg3, args[4]);
            else
                user->Fread(arg1, "", arg2);
        }
        else if (cmd == "fwrite") {
            user->Fwrite(arg1, arg2, arg3);
        }
        else if (cmd == "more") {
            user->More(arg1);
        }
        else if (cmd == "copy") {
            user->Copy(arg1, arg2);
        }
        else if (cmd == "copyin") {
            user->Copyin(arg1, arg2);
        }
        else if (cmd == "copyout") {
            user->Copyout(arg1, arg2);
        }
        else if (cmd != "") {
            cout << "shell : " << cmd << " : don't find this commond \n";
        }

    NEXT_INPUT:
        cout << "[unix2nd-fs " << user->curDirPath << " ]$ ";
        if (autoFlag) {
            if (autoTest(line)) {
                cout << line;
                getchar();
            }
            else {
                cout << "DemoTest finished ... \n";
                autoFlag = 0;
                line = "";
            }
        }
        else {
            getline(cin, line);
        }
    }
    return 0;
}
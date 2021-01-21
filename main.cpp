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
        "Command       :  man -��ʾָ��˵�� \n"
        "Description   :  ����ָ���ʽ�������û�ʹ����Ӧ���� \n"
        "Usage         :  man [ָ��] \n"
        "Parameter     :  [ָ��] ���£�  \n"
        "                 man          :  ˵�� \n"
        "                 fformat      :  ��ʽ���ļ��� \n"
        "                 exit         :  ��ȷ�˳� \n"
        "                 mkdir        :  �½�Ŀ¼ \n"
        "                 cd           :  �ı�Ŀ¼ \n"
        "                 ls           :  �г�Ŀ¼���ļ� \n"
        "                 fcreate      :  �½��ļ� \n"
        "                 fdelete      :  ɾ���ļ� \n"
        "                 fopen        :  ���ļ� \n"
        "                 fclose       :  �ر��ļ� \n"
        "                 fseek        :  �ƶ���дָ�� \n"
        "                 fwrite       :  д���ļ� \n"
        "                 fread        :  ��ȡ�ļ� \n"
		"                 copy         :  �����ļ� \n"
		"                 copyin       :  ���ⲿ����ϵͳ�����ļ� \n"
		"                 copyout      :  �����ļ����ⲿ����ϵͳ \n"
		"                 more         :  ��ASCII����ʽ��ʾ�ļ����� \n"
        "                 dt|demoTest  :  �������� \n"
        "Usage Demo    :  man mkdir \n"
        ;

    static string fformat =
        "Command       :  fformat -��ʽ���ļ��� \n"
        "Description   :  ��ʽ���ļ�������������ļ���Ŀ¼! \n"
        "Usage         :  fformat \n"
        "Parameter     :  �� \n"
        "Usage Demo    :  fformat \n"
        ;

    static string exit =
        "Command       :  exit -�˳��ļ�ϵͳ \n"
        "Description   :  �˳��ļ�ϵͳ���˳�ʱֱ�ӹرմ��ڣ�\n"
        "Usage         :  exit \n"
        "Parameter     :  �� \n"
        "Usage Demo    :  exit \n"
        ;

    static string mkdir =
        "Command       :  mkdir -�½�Ŀ¼ \n"
        "Description   :  �½�Ŀ¼�������ִ�����ʾ������Ϣ! \n"
        "Usage         :  mkdir <Ŀ¼��> \n"
        "Parameter     :  <Ŀ¼��>  ���·�������·�� \n"
        "Usage Demo    :  mkdir dir \n"
        "                 mkdir ../dir \n"
        "                 mkdir ../../dir \n"
        "                 mkdir /dir \n"
        "                 mkdir /dir1/dir \n"
        "Error Avoided :  Ŀ¼��������Ŀ¼·�������ڣ�Ŀ¼������Ŀ¼�� \n"
        ;


    static string cd =
        "Command       :  cd -�ı�Ŀ¼ \n"
        "Description   :  �ı䵱ǰ����Ŀ¼�������ִ�����ʾ������Ϣ! \n"
        "Usage         :  cd <Ŀ¼��> \n"
        "Parameter     :  <Ŀ¼��>Ĭ��Ϊ��ǰĿ¼��\n"
        "                 <Ŀ¼��> ���������·����Ҳ�����Ǿ���·�� \n"
        "Usage Demo    :  ls \n"
        "                 ls ../dir \n"
        "                 ls ../../dir \n"
        "                 ls /dir \n"
        "                 ls /dir1/dir \n"
        "Error Avoided :  Ŀ¼��������Ŀ¼������Ŀ¼��Ŀ¼·�������ڵȵ� \n"
        ;

	static string ls =
		"Command       :  ls -�г�Ŀ¼���� \n"
		"Description   :  �г���ǰĿ¼�а������ļ�����Ŀ¼���������ִ�����ʾ������Ϣ! \n"
		"Usage         :  ls \n"
		"Parameter     :  �� \n"
		"Usage Demo    :  ls \n"
		;

    static string fcreate =
        "Command       :  fcreate -�½��ļ� \n"
        "Description   :  �½�ָ���ļ��������ִ�����ʾ������Ϣ! \n"
        "Usage         :  fcreate <�ļ���> <ѡ��> \n"
        "Parameter     :  <�ļ���> ���԰������·�������·�� \n"
        "                 <ѡ��> -r ֻ������ \n"
        "                 <ѡ��> -w ֻд���� \n"
        "                 <ѡ��> -rw ��д���� \n"
        "Usage Demo    :  fcreate newfile -rw \n"
        "                 fcreate ../newfile -rw \n"
        "                 fcreate ../../newfile -rw \n"
        "                 fcreate /newfile -rw \n"
        "                 fcreate /dir/newfile -rw \n"
        "Error Avoided :  �ļ���������Ŀ¼·�������ڣ�Ŀ¼������Ŀ¼�� \n"
        ;

    static string fopen =
        "Command       :  fopen -���ļ� \n"
        "Description   :  ��ָ���ļ����ļ��Ķ�дǰ������fopen��\n"
        "                 �����ִ�����ʾ������Ϣ! \n"
        "Usage         :  fopen <�ļ���> <ѡ��> \n"
        "Parameter     :  <�ļ���> ���԰������·�������·�� \n"
        "                 <ѡ��> -r ֻ������ \n"
        "                 <ѡ��> -w ֻд���� \n"
        "                 <ѡ��> -rw == -r -w ��д���� \n"
        "Usage Demo    :  fopen file -r \n"
        "                 fopen ../file -w \n"
        "                 fopen ../../file -rw \n"
        "                 fopen /file -r -w \n"
        "                 fopen /dir/file -rw \n"
        ;

	static string fdelete =
		"Command       :  fdelete -ɾ���ļ� \n"
		"Description   :  ɾ��ָ���ļ��������ִ�����ʾ������Ϣ! \n"
		"Usage         :  fdelete <�ļ���> \n"
		"Parameter     :  <�ļ���> ���԰������·�������·�� \n"
		"Usage Demo    :  fdelete file \n"
		"                 fdelete ../file \n"
		"                 fdelete ../../file \n"
		"                 fdelete /file \n"
		"                 fdelete /dir/file \n"
		;

    static string fclose =
        "Command       :  fclose -�ر��ļ� \n"
        "Description   :  �ر�ָ���ļ����ر��Ѿ��򿪵��ļ��� \n"
        "                 �����ִ�����ʾ������Ϣ! \n"
        "Usage         :  fclose <file descriptor> \n"
        "Parameter     :  <file descriptor> �ļ������� \n"
        "Usage Demo    :  fclose 1 \n"
        ;

    static string fseek =
        "Command       :  fseek -��λ�ļ���дָ�� \n"
        "Description   :  �����ļ���ָ��λ�á������ִ�����ʾ������Ϣ! \n"
        "Usage         :  fseek <file descriptor> <offset> <origin> \n"
        "Parameter     :  <file descriptor> fopen���ص��ļ�������, \n"
        "                 <offset> ָ���� <origin> ��ʼ��ƫ����,�����ɸ�, \n"
        "                 <origin> ָ����ʼλ�� ��Ϊ0(SEEK_SET), 1(SEEK_CUR), 2(SEEK_END) \n"
        "Usage Demo    :  fseek 1 500 0 \n"
        ;

    static string fwrite =
        "Command       :  fwrite -д���ļ� \n"
        "Description   :  д��ָ���ļ����ݡ������ִ�����ʾ������Ϣ! \n"
        "Usage         :  fwrite <file descriptor> <Infile> <size> \n"
        "Parameter     :  <file descriptor> open���ص��ļ������� \n"
        "                 <Infile> ָ��д������Ϊ�ļ�Infile�е����� \n"
        "                 <size> ָ��д���ֽ�������СΪ <size> \n"
        "Usage Demo    :  fwrite 1 Infile 123 \n"
        ;

    static string fread =
        "Command       :  fread -��ȡ�ļ� \n"
        "Description   :  ��ָ���ļ��ж�ȡ���ݡ������ִ�����ʾ������Ϣ! \n"
        "Usage         :  fread <file descriptor> [-o <Outfile>] <size> \n"
        "Parameter     :  <file descriptor> open���ص��ļ������� \n"
        "                 [-o <Outfile>] -o ָ�������ʽΪ�ļ����ļ���Ϊ <Outfile> Ĭ��Ϊshell \n"
        "                 <size> ָ����ȡ�ֽ�������СΪ <size> \n"
        "Usage Demo    :  fread 1 -o Outfile 123 \n"
        "                 fread 1 123 \n"
        ;

    static string copy =
        "Command       :  copy -�����ļ� \n"
        "Description   :  �����ļ��������ִ�����ʾ������Ϣ! \n"
        "Usage         :  copy <srcfile> <desfile> \n"
        "Parameter     :  <srcfile> Դ�ļ��� \n"
        "                 <desfile> Ŀ���ļ��� \n"
        "Usage Demo    :  copy srcfile desfile \n"
        ;
    static string copyin =
        "Command       :  copyin -���ⲿ����ϵͳ�����ļ� \n"
        "Description   :  ���ⲿ����ϵͳ�����ļ��������ļ�ϵͳ�������ִ�����ʾ������Ϣ! \n"
        "Usage         :  copyin <srcfile> <desfile> \n"
        "Parameter     :  <srcfile> �ⲿ����ϵͳ��Դ�ļ��� \n"
        "                 <desfile> �����ļ�ϵͳ��Ŀ���ļ��� \n"
        "Usage Demo    :  copyin srcfile desfile \n"
        ;

    static string copyout =
        "Command       :  copyout -�����ļ����ⲿ����ϵͳ \n"
        "Description   :  ���������ļ�ϵͳ�ļ����ⲿ����ϵͳ�������ִ�����ʾ������Ϣ! \n"
        "Usage         :  copyout <srcfile> <desfile> \n"
        "Parameter     :  <srcfile> �����ļ�ϵͳ��Դ�ļ��� \n"
        "                 <desfile> �ⲿ����ϵͳ��Ŀ���ļ��� \n"
        "Usage Demo    :  copyout srcfile desfile \n"
        ;
    static string more =
        "Command       :  more -��ASCII����ʽ��ʾ�ļ����� \n"
        "Description   :  ��ASCII����ʽ��ʾ�ļ����ݡ������ִ�����ʾ������Ϣ! \n"
        "Usage         :  more <file> \n"
        "Parameter     :  <file> Ҫ��ʾ���ļ��� \n"
        "Usage Demo    :  more file \n"
        ;
    static string demoTest =
        "Command       :  demoTest -�������� \n"
        "Description   :  ͨ��һϵ�в���������չʾϵͳ���� \n"
        "Usage         :  demoTest | dt \n"
        "Parameter     :  �� \n"
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
        "copyin 1754272-����ɼ.pdf 1754272-����ɼ.pdf",       
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
    cout << "++++++++++++++++++++ UNIX�����ļ�ϵͳ ++++++++++++++++++++" << endl;

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
            //create û����Ҫģʽ�ı�Ҫ
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
#include "User.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;

extern FileManager gc_FileManager;

User::User() {
    u_error = U_NOERROR;
    fileManager = &gc_FileManager;
	dirp = "/";
    curDirPath = "/";
    //fileManager->Open();
	cdir = fileManager->rootDirINode;
    pdir = NULL;
    Utility::memset(arg, 0, sizeof(arg));
}

User::~User() {
}


void User::Mkdir(string dirName) {
    if (!checkPathName(dirName)) {
        return;
    }
    arg[1] = INode::IFDIR;
    fileManager->Creat();
    IsError();
}

void User::Ls() {
    ls.clear();
    fileManager->Ls();
    if (IsError()) {
        return;
    }
    cout << ls << endl;
}

void User::Cd(string dirName) {
    if (!checkPathName(dirName)) {
        return;
    }
    fileManager->ChDir();
    IsError();
}

void User::Fcreate(string fileName, string mode) {
    if (!checkPathName(fileName)) {
        return;
    }
    int md = INodeMode(mode);
    if (md == 0) {
        cout << "this mode is undefined !  \n";
        return;
    }

    arg[1] = md;
    fileManager->Creat();
    IsError();
}

void User::Fdelete(string fileName) {
    if (!checkPathName(fileName)) {
        return;
    }
    fileManager->UnLink();
    IsError();
}

void User::Fopen(string fileName, string mode) {
    if (!checkPathName(fileName)) {
        return;
    }
    int md = FileMode(mode);
    if (md == 0) {
        cout << "this mode is undefined ! \n";
        return;
    }

    arg[1] = md;
    fileManager->Open();
    if (IsError())
        return;
    cout << "open success, return fd=" << ar0[EAX] << endl;
}

void User::Fclose(string sfd) {
    if (sfd.empty() || !isdigit(sfd.front())) {
        cout << "parameter fd can't be empty or be nonnumeric ! \n";
        return;
    }
    arg[0] = stoi(sfd);;
    fileManager->Close();
    IsError();
}

void User::Fseek(string sfd, string offset, string origin) {
    if (sfd.empty() || !isdigit(sfd.front())) {
        cout << "parameter fd can't be empty or be nonnumeric ! \n";
        return;
    }
    if (offset.empty()) {
        cout << "parameter offset can't be empty ! \n";
        return;
    }
    if (origin.empty() || !isdigit(origin.front())) {
        cout << "parameter origin can't be empty or be nonnumeric ! \n";
        return;
    }
    arg[0] = stoi(sfd);
    arg[1] = stoi(offset);
    arg[2] = stoi(origin);
    fileManager->Seek();
    IsError();
}

void User::Copy(string srcFile, string desfile) {
    if (!checkPathName(srcFile)) {
        return;
    }

    arg[1] = File::FREAD;
    fileManager->Open();
    if (IsError())
        return;
    int fread = ar0[EAX];

    if (!checkPathName(desfile)) {
        return;
    }

    arg[1] = INode::IREAD | INode::IWRITE;
    fileManager->Creat();
    if (IsError())
        return;
    arg[1] = File::FWRITE;
    fileManager->Open();
    if (IsError())
        return;
    int fwrite = ar0[EAX];

    int usize = 1024;

    char* buffer = new char[usize];


    arg[1] = (long)buffer;
    arg[2] = usize;
    ar0[User::EAX] = usize;

    int size = 0;
    while (ar0[User::EAX] == usize) {
        arg[0] = fread;
        arg[2] = usize;
        fileManager->Read();
        if (IsError())
            return;
        size += ar0[User::EAX];



        arg[0] = fwrite;
        arg[2] = ar0[User::EAX];
        fileManager->Write();
        if (IsError())
            return;
    }
    arg[0] = fread;
    fileManager->Close();
    if (IsError())
        return;
    arg[0] = fwrite;
    fileManager->Close();
    if (IsError())
        return;
    cout << "File " << srcFile << "copy to file " << desfile << " Success, Write " << size << " bytes done ! " << endl;
    delete[]buffer;
}


void User::Copyin(string srcFile,string desfile) {
    fstream fsRead(srcFile, ios::in | ios::binary);
    if (!fsRead) {
        cout << "file " << srcFile << " open failed ! \n";
        return;
    }

    fsRead.seekg(0, fsRead.end);
    size_t srcSize = fsRead.tellg();
    char* buffer = new char[srcSize];
    fsRead.seekg(0, 0);
    fsRead.read(buffer, srcSize);
    fsRead.close();


    if (!checkPathName(desfile)) {
        return;
    }

    arg[1] = INode::IREAD | INode::IWRITE;
    fileManager->Creat();
    if (IsError())
        return;
    arg[1] = File::FWRITE;
    fileManager->Open();
    if (IsError())
        return;

    arg[0] = ar0[EAX];
    arg[1] = (long)buffer;
    arg[2] = srcSize;
    fileManager->Write();

    if (IsError())
        return;
    cout << "External file " << srcFile << " copy to " << desfile << " Success, Write " << ar0[User::EAX] << " bytes done ! " << endl;
    fileManager->Close();
    IsError();
    delete[]buffer;
}

void User::More(string srcFile) {

    if (!checkPathName(srcFile)) {
        return;
    }

    arg[1] = File::FREAD;
    fileManager->Open();
    if (IsError())
        return;

    int usize = 1024;

    char* buffer = new char[usize+1];
    
    buffer[usize] = 0;
    arg[0] = ar0[EAX];
    arg[1] = (long)buffer;
    arg[2] = usize;
    ar0[User::EAX] = usize;

    while (ar0[User::EAX] == usize) {
        fileManager->Read();
        if (IsError())
            return;
        if (ar0[User::EAX] != usize)
            buffer[ar0[User::EAX]] =0;
        cout << buffer << endl;
        cout << "\nDisplay " << ar0[User::EAX] << " bytes," << "Press any key continue ...";
        _getch();
    }
    cout << endl;
    fileManager->Close();
    IsError();
 
    delete[]buffer;
}

void User::Copyout(string srcFile, string desfile){

    if (!checkPathName(srcFile)) {
        return;
    }

    arg[1] =  File::FREAD;
    fileManager->Open();
    if (IsError())
        return;

    fstream fsWrite(desfile, ios::out | ios::binary);
    if (!fsWrite) {
        cout << "file " << desfile << " open failed ! "<<endl;
        return;
    }
    int usize=1024;

    char* buffer = new char[usize];

    arg[0] = ar0[EAX];
    arg[1] = (long)buffer;
    arg[2] = usize;
    ar0[User::EAX] = usize;

    int size = 0;
    while (ar0[User::EAX] == usize) {
        fileManager->Read();
        if (IsError())
            return;
        size += ar0[User::EAX];
        fsWrite.write(buffer, ar0[User::EAX]);
    }
    fileManager->Close();
    IsError();
    fsWrite.close();
    cout << "File " << srcFile << "copy to external file " << desfile << " Success, Write " << size <<" bytes done ! "<<endl;
    delete[]buffer;
}


void User::Fwrite(string sfd, string inFile, string size) {
    if (sfd.empty() || !isdigit(sfd.front())) {
        cout << "parameter fd can't be empty or be nonnumeric ! \n";
        return;
    }
    int fd = stoi(sfd);

    int usize;
    if (size.empty() || (usize = stoi(size)) < 0) {
        cout << "parameter size must be greater or equal than 0 ! \n";
        return;
    }

    char *buffer = new char[usize];
    fstream fin(inFile, ios::in | ios::binary);
    if (!fin) {
        cout << "file " << inFile << " open failed ! \n";
        return;
    }
    fin.read(buffer, usize);
    fin.close();
    arg[0] = fd;
    arg[1] = (long)buffer;
    arg[2] = usize;
    fileManager->Write();

    if (IsError())
        return;
    cout << "write " << ar0[User::EAX] << "bytes success !" << endl;
    delete[]buffer;
}

void User::Fread(string sfd, string outFile, string size) {
    if (sfd.empty() || !isdigit(sfd.front())) {
        cout << "parameter fd can't be empty or be nonnumeric ! \n";
        return;
    }
    int fd = stoi(sfd);

    int usize;
    if (size.empty() || !isdigit(size.front()) || (usize = stoi(size)) < 0) {
        cout << "parameter size is not right \n";
        return;
    }
    char *buffer = new char[usize];
    arg[0] = fd;
    arg[1] = (long)buffer;
    arg[2] = usize;
    fileManager->Read();
    if (IsError())
        return;

    cout << "read " << ar0[User::EAX] << " bytes success : \n" ;
    if (outFile.empty()) {
        for (unsigned int i = 0; i < ar0[User::EAX]; ++i) {
            cout << (char)buffer[i];
        }
        cout << " \n";
        return;
    }
    fstream fout(outFile, ios::out | ios::binary);
    if (!fout) {
        cout << "file " << outFile << " open failed ! \n";
        return;
    }
    fout.write(buffer, ar0[User::EAX]);
    fout.close();
    cout << "read to " << outFile << " done ! \n";
    delete[]buffer;
}

int User::INodeMode(string mode) {
    int md = 0;
    if (mode.find("-r") != string::npos) {
        md |= INode::IREAD;
    }
    if (mode.find("-w") != string::npos) {
        md |= INode::IWRITE;
    }
    if (mode.find("-rw") != string::npos) {
        md |= (INode::IREAD | INode::IWRITE);
    }
    return md;
}

int User::FileMode(string mode) {
    int md = 0;
    if (mode.find("-r") != string::npos) {
        md |= File::FREAD;
    }
    if (mode.find("-w") != string::npos) {
        md |= File::FWRITE;
    }
    if (mode.find("-rw") != string::npos) {
        md |= (File::FREAD | File::FWRITE);
    }
    return md;
}

bool User::checkPathName(string path) {
    // FileManager 中函数不判断参数的合法性，最好在User中过滤，
    // 暂不考虑过多的参数不合法情况
    if (path.empty()) {
        cout << "parameter path can't be empty ! \n";
        return false;
    }

    if (path.substr(0, 2) != "..") {
        dirp = path;
    }
    else {
        string pre = curDirPath;
        unsigned int p = 0;
        //可以多重相对路径 .. ../ ../.. ../../
        for (; pre.length() > 3 && p < path.length() && path.substr(p, 2) == ".."; ) {
            pre.pop_back();
            pre.erase(pre.find_last_of('/') + 1);
            p += 2;
            p += p < path.length() && path[p] == '/';
        }
        dirp = pre + path.substr(p);
    }

    if (dirp.length() > 1 && dirp.back() == '/') {
        dirp.pop_back();
    }

    for (unsigned int p = 0, q = 0; p < dirp.length(); p = q + 1) {
        q = dirp.find('/', p);
        q = min(q, (unsigned int)dirp.length());
        if (q - p > DirectoryEntry::DIRSIZ) {
            cout << "the fileName or dirPath can't be greater than 28 size ! \n";
            return false;
        }
    }
    return true;
}

bool User::IsError() {
    if (u_error != U_NOERROR) {
        cout << "errno = " << u_error;
        EchoError(u_error);
        u_error = U_NOERROR;
        return true;
    }
    return false;
}

void User::EchoError(enum ErrorCode err) {
    string estr;
    switch (err) {
    case User::U_NOERROR:
        estr = " No u_error ";
        break;
    case User::U_ENOENT:
        estr = " No such file or directory ";
        break;
    case User::U_EBADF:
        estr = " Bad file number ";
        break;
    case User::U_EACCES:
        estr = " Permission denied ";
        break;
    case User::U_ENOTDIR:
        estr = " Not a directory ";
        break;
    case User::U_ENFILE:
        estr = " File table overflow ";
        break;
    case User::U_EMFILE:
        estr = " Too many open files ";
        break;
    case User::U_EFBIG:
        estr = " File too large ";
        break;
    case User::U_ENOSPC:
        estr = " No space left on device ";
        break;
    default:
        break;
    }
    cout << estr << endl;
}
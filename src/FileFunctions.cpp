/*
 * File: FileFunctions.cpp
 * Author: Dorian Galvez
 * Date: June 2009
 * Description: file system functions
 */

#include "FileFunctions.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

#ifdef WIN32
	#include <direct.h>
	#include "dirent_win.h"
	#define mkdir(a) _mkdir(a)
#else
	#include <dirent.h>
	#include <sys/stat.h>
#endif

using namespace std;

void FileFunctions::MkDir(const char *path)
{
	mkdir(path);
}

void FileFunctions::RmDir(const char *path)
{
	// empty path
	vector<string> files = FileFunctions::Dir(path, "");
	for(vector<string>::iterator it = files.begin(); it != files.end(); it++){
		remove(it->c_str());
	}
	rmdir(path);
}

void FileFunctions::RmFile(const char *path)
{
	remove(path);
}

bool FileFunctions::FileExists(const char *filename)
{
	std::fstream f(filename, ios::in);

	if(f.is_open()){
		f.close();
		return true;
	}else
		return false;
}

bool FileFunctions::DirExists(const char *path)
{
    DIR *dirp;
	if(dirp = opendir(path)){
		closedir(dirp);
		return true;
	}else
		return false;
}

std::vector<std::string> FileFunctions::Dir(const char *path, const char *right)
{
	DIR *dirp;
	struct dirent *entry;
	vector<string> ret;

	if(dirp = opendir(path)){
		while(entry = readdir(dirp)){
			string name(entry->d_name);
			string r(right);
			if((name.length() >= r.length()) && 
				(name.substr(name.length() - r.length()).compare(r) == 0))
			{
				ret.push_back(string(path) + "/" + entry->d_name);
			}
		}
		closedir(dirp);
	}
	return ret;
}
std::string FileFunctions::FileName(const std::string filepath)
{
	string::size_type p = filepath.find_last_of('/');
	string::size_type p2 = filepath.find_last_of('\\');
	if(p2 != string::npos && p2 > p) p = p2;
	return filepath.substr(p+1);
}

void FileFunctions::FileParts(const std::string filepath, std::string &path,
						   std::string &filename, std::string &ext)
{
	string::size_type p = filepath.find_last_of('/');
	string::size_type p2 = filepath.find_last_of('\\');
	if(p == string::npos || (p2 != string::npos && p2 > p)) p = p2;

	std::string filext;

	if(p == string::npos){
		path = "";
		filext = filepath;
	}else{
		path = filepath.substr(0, p);
		filext = filepath.substr(p+1);
	}

	p = filext.find_last_of('.');
	if(p == string::npos){
		filename = filext;
		ext = "";
	}else{
		filename = filext.substr(0, p);
		ext = filext.substr(p+1);
	}
}


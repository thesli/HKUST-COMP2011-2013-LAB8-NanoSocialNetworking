#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

class User{
public:
	int Id;
	string username;
	string password;
	string msg;
	User(string u,string p,string m);
	vector<int> pendingFdIdList;
	vector<int> fdIdList;
};

class SNS{
public:
	vector<User> uList;

	int findIdByUsername(string username);
	bool addUser(string username,string password,string msg);
	bool addFdToPending(int uId,int aId);
	bool confirmAddFd(int uId,int cId);
	vector<User> fdsListByUserId();
	bool deleteFriend(int uId,int dId);

	bool initView();
	bool updateMessageView();
	bool signUpView();
	bool uMenuView();
	bool loginView();
	bool addFdView();
	bool newsFeedView(int uId);
	bool deleteFdView(int uId);
	bool listFdView(int uId);
	bool handleRequestsView();
};


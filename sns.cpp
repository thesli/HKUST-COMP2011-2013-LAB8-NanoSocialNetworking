#include <vector>
#include <iostream>
#include <stdlib.h>
#include "sns.h"

using namespace std;


User cUser("","","");
SNS n;


User::User(string s,string p,string m){
	username = s;
	password = p;
	msg = m;
	Id = n.uList.size();
}

bool SNS::initView(){
	string welcome = "\n======Welcome to our Nano Social Network!=====\n1: No account? Sign up now!\n2: Log in.\n0: Exit Nano social network.\n=======================================\n";
	int option =-1;
	do{
		if(option==0||option==1||option==2){
			break;
		}
		cout << welcome;
		cout << "please enter your choice: ";
	}while(cin >> option);

	switch(option){
		case 0:
			exit(0);
			break;
		case 1:
			signUpView();
			break;
		case 2:
			loginView();
			break;
	}
}

bool SNS::signUpView(){
	string username;
	string password;
	string msg;
	cout << "=====Sign up new user=====" <<endl;
	cout << "Please enter new user name:";
	cin >> username;
	cout << "Please enter your password: ";
	cin >> password;
	cout << "Please your first message:" ;
	cin >> msg;
	if(addUser(username,password,msg)){
		cout << "Registered successfully! You can log in now.";
		initView();
	}
}

bool SNS::uMenuView(){
	string menuWelcome = "=====Menu=====\n1: Update message.\n2: Add Friend.\n3: Delete Friend.\n4: List my friends.\n0: Log out.\n";
	int option = -1;
	do{
		if(option == 3 ||option == 2 ||option == 1 ||option == 0 || option == 4){
			break;
		}
		cout << menuWelcome;
		cout << "Please enter your choice: ";
	}while(cin >> option);
	switch(option){
		case 0:
			initView();
			break;
		break;
		case 1:
			updateMessageView();
		break;
		case 2:
			addFdView();
		break;
		case 3:
			deleteFdView(cUser.Id);
		break;
		case 4:
			listFdView(cUser.Id);
		break;
	}
}

bool SNS::listFdView(int uId){
	cout << "=====List your friends=====" << endl;
	if(uList[uId].fdIdList.size()==0){
		cout << "You currently don't have any friend \n";
	}else{
		cout << "Your friends are \n";
	}
	for (int i = 0; i < uList[uId].fdIdList.size(); ++i)
	{
		cout << uList[uList[uId].fdIdList[i]].username << "\n";
	}
	uMenuView();
}

bool SNS::deleteFdView(int uId){
	cout << "=====Delete a friend=====" << endl;
	cout << "Please enter the name: ";
	string username;
	cin >> username;
	for (int i = 0; i < uList[uId].fdIdList.size(); ++i)
	{
		if(uList[uList[uId].fdIdList[i]].username==username){
			cout << uList[uList[uId].fdIdList[i]].username << " is no longer your friends. " << endl;
			deleteFriend(uId,uList[uId].fdIdList[i]);
			uMenuView();
		}
	}
	cout << "This user isn't found in your friend list\n";
	uMenuView();
}

bool SNS::updateMessageView(){
	int Id = cUser.Id;
	string msg;
	cout << "=====Update your message=====" << "\n";
	cout << "Please enter new message:";
	cin >> msg;
	uList[Id].msg = msg;
	cout << "Your message has been updated\n";
	uMenuView();
}

bool SNS::addFdView(){
	cout << "=====Add Friend====\n";
	cout << "please enter the username you want to add: ";
	string username;
	cin >> username;
	if(username!=cUser.username&&findIdByUsername(username)!=-1){
		addFdToPending(findIdByUsername(username),cUser.Id);
		cout << "Add Friend request sent\n";
	}else{
		cout << "That user isn't valid\n";
	}
	uMenuView();
}

bool SNS::handleRequestsView(){
	cout << "=====Handle add friends requests=====\n";
	if(cUser.pendingFdIdList.size()==0){
		cout << "No requests.\n";
	}else{
		for (int i = 0; i < cUser.pendingFdIdList.size(); ++i)
		{
			int aId = cUser.pendingFdIdList[i];
			char option;
			cout << uList[aId].username << " wants to be your friend. \n";
			cout << "Press 'y' to confirm, Press 'n' to decline: ";
			cin >> option;
			if(option == 'y'){
				confirmAddFd(cUser.Id,aId);
				cout << uList[aId].username << " is added as your friend\n";
			}else if(option=='n'){
				cUser.pendingFdIdList.erase(cUser.pendingFdIdList.begin()+i);
				i--;
			}else{
				continue;
			}
		}
	}
	newsFeedView(cUser.Id);
}

bool SNS::newsFeedView(int Id){
	cout << "=====NewsFeeds=====" << endl;
	if(uList[Id].fdIdList.size()==0){
		cout << "No user feed avaliable" << endl;
	}
	for (int i = 0; i < uList[Id].fdIdList.size(); ++i)
	{
		cout << uList[uList[Id].fdIdList[i]].username << " : " << uList[uList[Id].fdIdList[i]].msg << "\n";
	}
	uMenuView();
}

bool SNS::loginView(){
	string username;
	string password;
	cout << "=====User Log In=====" <<endl;
	cout << "Please enter your user name: ";
	cin >> username;
	cout << "Please enter your password:";
	cin >> password;
	if(uList[findIdByUsername(username)].password==password){
		cout << "Log in successfully. \n";
		cUser = uList[findIdByUsername(username)];
		handleRequestsView();
		return true;
	}else{
		cout << "Log in failed.";
		initView();
		return false;
	};


}

int SNS::findIdByUsername(string username){
	for (int i = 0; i < uList.size(); ++i)
	{
		if(uList[i].username==username){
			return i;
		}
	}
	return -1;
}

bool SNS::confirmAddFd(int uId,int cId){
	uList[uId].fdIdList.push_back(cId);
	uList[cId].fdIdList.push_back(uId);
	for (int i = 0; i < uList[uId].pendingFdIdList.size(); ++i)
	{
		if(uList[uId].pendingFdIdList[i]==cId){
			uList[uId].pendingFdIdList.erase(uList[uId].pendingFdIdList.begin()+i);
		}
	}
}

bool SNS::addFdToPending(int uId,int aId){
	uList[uId].pendingFdIdList.push_back(aId);
}

bool SNS::addUser(string u,string p,string m){
	bool valid;
	for (int i = 0; i < uList.size(); ++i)
	{
		if(uList[i].username==u){
			cout << "\nusername taken";
			return false;
		}
	}
	User k(u,p,m);
	uList.push_back(k);
	return true;
}

bool SNS::deleteFriend(int uId,int dId){
	for (int i = 0; i < uList[uId].fdIdList.size(); ++i)
	{
		if(uList[uId].fdIdList[i]==dId){
			uList[uId].fdIdList.erase(uList[uId].fdIdList.begin()+i);
		}
	}

	for (int i = 0; i < uList[dId].fdIdList.size(); ++i)
	{
		if(uList[dId].fdIdList[i]==uId){
			uList[dId].fdIdList.erase(uList[dId].fdIdList.begin()+i);
		}
	}
}

void mockData(){
	n.addUser("a","b","dfsadas");
	n.addUser("b","c","fdsffda");
	n.addUser("c","c","fdas");
	n.addUser("d","c","fdsfadfvcxbvffda");
	n.addUser("e","c","fds vfds fdsffda");
	n.addUser("f","c","fds afdsfffda");
	n.addUser("g","c","fds fad fffda");
	n.addFdToPending(0,1);
	n.addFdToPending(0,2);
	n.addFdToPending(0,5);
	n.addFdToPending(1,3);
	n.addFdToPending(1,6);
	n.addFdToPending(1,4);
	// n.confirmAddFd(0,2);
	// n.confirmAddFd(0,1);
	// n.confirmAddFd(0,5);
	// n.confirmAddFd(0,6);
	// n.deleteFriend(0,1);
}



int main(int argc, char const *argv[])
{
	mockData();
	n.initView();
	// cout << n.uList[1].fdIdList[0] << endl;
	// cout << n.uList[0].fdIdList[0] << endl;
	// cout << n.uList[2].fdIdList[0] << endl;
	// cout << n.uList[5].fdIdList[0] << endl;
	// cout << n.uList[0].fdIdList[1] << endl;
	// cout << n.uList[0].fdIdList[0];
	// cUser = n.uList[1];
	// n.listFdView();
	return 0;
}
#include <iostream>
#include<vector>
#include <stack>
#include <queue>
#include <fstream>
using namespace std;

/* ================= USER MANAGEMENT (LINKED LIST) ================= */

struct User {
    string username, password;
    User* next;
    User(string u, string p) : username(u), password(p), next(nullptr) {}
};

User* userHead = nullptr;

void loadUsers() {
    ifstream file("users.txt");
    string u, p;
    while (file >> u >> p) {
        User* n = new User(u, p);
        n->next = userHead;
        userHead = n;
    }
    file.close();
}

int main(){

  //ShopSmart
  

  return 0;
}

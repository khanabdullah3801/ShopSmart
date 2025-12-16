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
void registerUser() {
    string u, p;
    cout << "Username: ";
    cin.ignore();
    getline(cin,u);
    cout << "Password: ";
    getline(cin,p);

    User* n = new User(u, p);
    n->next = userHead;
    userHead = n;

    ofstream file("users.txt", ios::app);
    file << u << " " << p << endl;
    file.close();

    cout << "User registered successfully!\n";
}

bool loginUser() {
    string u, p;
    cout << "Username: ";
    cin.ignore();
    getline(cin,u);
    cout << "Password: ";
    getline(cin,p);

    User* t = userHead;
    while (t) {
        if (t->username == u && t->password == p)
            return true;
        t = t->next;
    }
    return false;
}

/* ================= PRODUCT INVENTORY (AVL TREE) ================= */

struct Product {
    int id, stock, sold, height;
    string category, name;
    float price;
    Product *left, *right;

    Product(int i, string c, string n, float p, int s)
        : id(i), category(c), name(n), price(p),
          stock(s), sold(0), height(1),
          left(nullptr), right(nullptr) {}
};

Product* root = nullptr;

/* ---------- AVL Utilities ---------- */

int height(Product* n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getBalance(Product* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

/* ---------- Rotations ---------- */

Product* rightRotate(Product* y) {
    Product* x = y->left;
    Product* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Product* leftRotate(Product* x) {
    Product* y = x->right;
    Product* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

/* --------- Insert ---------- */

Product* insertProduct(Product* node, Product* p) {
    if (!node) return p;

    if (p->id < node->id)
        node->left = insertProduct(node->left, p);
    else if (p->id > node->id)
        node->right = insertProduct(node->right, p);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // LL
    if (balance > 1 && p->id < node->left->id)
        return rightRotate(node);

    // RR
    if (balance < -1 && p->id > node->right->id)
        return leftRotate(node);

    // LR
    if (balance > 1 && p->id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL
    if (balance < -1 && p->id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/* ---------- Search ---------- */

Product* searchProduct(Product* r, int id) {
    if (!r || r->id == id) return r;
    if (id < r->id) return searchProduct(r->left, id);
    return searchProduct(r->right, id);
}

/* ---------- Display ---------- */

void displayInventory(Product* r) {
    if (!r) return;
    displayInventory(r->left);
    cout << r->id << " | " << r->category << " | "
         << r->name << " | $" << r->price
         << " | Stock: " << r->stock << endl;
    displayInventory(r->right);
}
//Search  By Category
void searchByCategory(Product* r, const string& cat) {
    if (!r) return;
    searchByCategory(r->left, cat);
    if (r->category == cat)
        cout << r->id << " | " << r->name
             << " | $" << r->price
             << " | Stock: " << r->stock << endl;
    searchByCategory(r->right, cat);
}

/* ---------- Delete ---------- */

Product* findMin(Product* r) {
    while (r->left) r = r->left;
    return r;
}
//Delete Product
Product* deleteProduct(Product* r, int id) {
    if (!r) return r;

    if (id < r->id)
        r->left = deleteProduct(r->left, id);
    else if (id > r->id)
        r->right = deleteProduct(r->right, id);
    else {
        if (!r->left || !r->right) {
            Product* temp = r->left ? r->left : r->right;
            if (!temp) {
                temp = r;
                r = nullptr;
            } else
                *r = *temp;
            delete temp;
        } else {
            Product* temp = findMin(r->right);
            r->id = temp->id;
            r->category = temp->category;
            r->name = temp->name;
            r->price = temp->price;
            r->stock = temp->stock;
            r->right = deleteProduct(r->right, temp->id);
        }
    }

    if (!r) return r;

    r->height = 1 + max(height(r->left), height(r->right));
    int balance = getBalance(r);

    // LL
    if (balance > 1 && getBalance(r->left) >= 0)
        return rightRotate(r);

    // LR
    if (balance > 1 && getBalance(r->left) < 0) {
        r->left = leftRotate(r->left);
        return rightRotate(r);
    }

    // RR
    if (balance < -1 && getBalance(r->right) <= 0)
        return leftRotate(r);

    // RL
    if (balance < -1 && getBalance(r->right) > 0) {
        r->right = rightRotate(r->right);
        return leftRotate(r);
    }

    return r;
}

/* ================= FILE LOADING ================= */

void loadProducts() {
    ifstream file("products.txt");
    int id, stock;
    string cat, name;
    float price;

    while (file >> id >> cat >> name >> price >> stock) {
        root = insertProduct(root,
            new Product(id, cat, name, price, stock));
    }
    file.close();
}

/* ================= CART & ORDER ================= */

vector<Product*> cart;
stack<string> undoStack;
queue<vector<Product*>> orderQueue;

void addToCart() {
    int id;
    cout << "Enter Product ID: ";
    cin >> id;

    Product* p = searchProduct(root, id);
    if (!p || p->stock <= 0) {
        cout << "Product unavailable!\n";
        return;
    }

    cart.push_back(p);
    undoStack.push("Added " + p->name);
    cout << "Added to cart successfully!\n";
}

void undoAction() {
    if (!undoStack.empty() && !cart.empty()) {
        cout << "Undo: " << undoStack.top() << endl;
        undoStack.pop();
        cart.pop_back();
    } else {
        cout << "Nothing to undo!\n";
    }
}

void placeOrder() {
    if (cart.empty()) {
        cout << "Cart is empty!\n";
        return;
    }

    float total = 0;
    cout << "\n========== ORDER SUMMARY ==========\n";

    for (auto p : cart) {
        cout << "Product: " << p->name
             << " | Price: $" << p->price << endl;
        p->stock--;
        p->sold++;
        total += p->price;
    }

    cout << "----------------------------------\n";
    cout << "Total Bill: $" << total << endl;
    cout << "Order Status: SUCCESS\n";
    cout << "==================================\n";

    orderQueue.push(cart);
    cart.clear();
}

void viewCart() {
    if (cart.empty()) {
        cout << "Your cart is empty!\n";
        return;
    }

    float total = 0;
    cout << "\n========== YOUR CART ==========\n";

    for (auto p : cart) {
        cout << "Product: " << p->name
             << " | Price: $" << p->price << endl;
        total += p->price;
    }

    cout << "-------------------------------\n";
    cout << "Total: $" << total << endl;
    cout << "===============================\n";
}

/* ================= ADMIN FUNCTIONS ================= */

void adminAddProduct() {
    int id, stock;
    float price;
    string cat, name;

    cout << "Product ID: ";
    cin >> id;

    if (searchProduct(root, id)) {
        cout << "ID already exists!\n";
        return;
    }

    cout << "Category: ";
    cin >> cat;
    cout << "Name: ";
    cin >> name;
    cout << "Price: ";
    cin >> price;
    cout << "Stock: ";
    cin >> stock;

    root = insertProduct(root,
        new Product(id, cat, name, price, stock));

    cout << "Product added successfully!\n";
}

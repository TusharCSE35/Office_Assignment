// #include <bits/stdc++.h>
// using namespace std;

// vector<int> v;

// void loadVectorFromFile(const string &filename) {
//     ifstream inFile(filename);
//     if (inFile.is_open()) {
//         int value;
//         while (inFile >> value) {
//             v.push_back(value);
//         }
//         inFile.close();
//         cout << "Data loaded from file successfully." << endl;
//     } else {
//         cout << "No existing data file found. Starting fresh." << endl;
//     }
// }


// void saveVectorToFile(const string &filename) {
//     ofstream outFile(filename);
//     if (outFile.is_open()) {
//         for (int val : v) {
//             outFile << val << " ";
//         }
//         outFile.close();
//         cout << "Data saved to file successfully." << endl;
//     } else {
//         cout << "Error: Could not open file to save data." << endl;
//     }
// }

// int main() {
//     const string dataFile = "vector_test.text";

//     loadVectorFromFile(dataFile);

//     do {
//         int test;
//         cout << "Enter 1.add, 2.find, 3.display and 4.exit: ";
//         cin >> test;
//         if (test == 1) {
//             int x;
//             cout << "Enter a value to add to the vector: ";
//             cin >> x;
//             v.push_back(x);
//             cout << "Added successfully to the vector." << endl;
//         } else if (test == 2) {
//             int y;
//             cout << "Enter the value to find: ";
//             cin >> y;

//             bool flag = false;
//             for (int i = 0; i < v.size(); i++) {
//                 if (y == v[i]) {
//                     cout << y << " is found in the vector." << endl;
//                     flag = true;
//                     break;
//                 }
//             }
//             if (!flag) {
//                 cout << "Value not found in the vector." << endl;
//             }
//         } else if (test == 3) {
//             cout << "Vector List: ";
//             for (int i = 0; i < v.size(); i++) {
//                 cout << v[i] << " ";
//             }
//             cout << endl;
//         } else if (test == 4) {
//             saveVectorToFile(dataFile);
//             cout << "Thank you for using the program. Exiting now." << endl;
//             break;
//         } else {
//             cout << "Invalid input. Try again." << endl;
//         }
//     } while (true);

//     return 0;
// }






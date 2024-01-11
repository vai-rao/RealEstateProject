/*
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

string cleanString(string s){
    string tempString;
    for (int i = 0; i < s.length(); ++i) {
        if(isdigit(s[i]) || isalpha(s[i]) || s[i] == ' ' || s[i] == '.'){
            tempString += s[i];
        }
    }

    return tempString;

}

bool isAWeekend(int day) {
    int weekends[9] = {0, 6, 7, 13, 14, 20, 21, 27, 28};
    for (int i = 0; i < 9; i++) {
        if (weekends[i] == day) {
            return true;
        }
    }
    return false;
}

struct Property {
    string address;
    float price;
    int tenants;
    char calendar[12][31];

    void printCalendar(){
        for(int month = 0; month < 12; month++){
            for(int day = 0; day < 31; day++){
                cout << calendar[month][day] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    void changeDS(int mIndex, int dIndex){
        if (isAWeekend(dIndex)) {
            calendar[mIndex][dIndex] = 'D';
        }
        else {
            calendar[mIndex][dIndex] = 'B';
        }
    };
};

void fill(Property& house) {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 31; j++) {
            house.calendar[i][j] = '*';
        }
    }
}

void BookCal(Property& house, int startD, int startM, int endD, int endM) {
    int days = 31 * (endM - startM) + (endD - startD) + 1;
    for (int i = 0; i < days; i++) {
        house.changeDS(startM - 1, startD - 1);
        startD++;
        if (startD == 31 && days > 1) {
            startD = 0; /// how does this make sense
            startM++;
        }
    }
}

bool isAvailable(Property &house, int startD, int startM, int endD, int endM){
    int duration = 31 * (endM - startM) + (endD - startD) + 1;
    if(duration == 372)
        return true;

    for (int i = 0; i < duration; ++i) {
        if(startD == 31 && startM != 12) {
            startM++;
            startD = 1;
        }
        if(house.calendar[startM - 1][startD - 1] != '*') //IS BOOKED
            return false;
        startD++;
    }
    return true; // IS NOT BOOKED
}

string NumberToMonth(int num) {
    switch (num + 1) {
        case 1:
            return "January";
        case 2:
            return "February";
        case 3:
            return "March";
        case 4:
            return "April";
        case 5:
            return "May";
        case 6:
            return "June";
        case 7:
            return "July";
        case 8:
            return "August";
        case 9:
            return "September";
        case 10:
            return "October";
        case 11:
            return "November";
        case 12:
            return "December";
    }
    return "";
}

int findHouse(vector<Property>& houses, string address) {
    for (int i = 0; i < houses.size(); i++) {
        if (houses[i].address.find(address) != string::npos) {
            return i;
        }
    }
    return -1;
}

float MonthCount(Property& house, int month) {
    float count = 0;
    for (int i = 0; i < 31; i++) {
        if (house.calendar[month][i] == 'B') {
            count += house.price;
        }
        if (house.calendar[month][i] == 'D') {
            count += house.price;
            count += house.price;
        }
    }
    return count;
}

void Addy() {
    cout << "Enter the address of the property you want to book or exit to exit:" << endl;
}

void showMenu() {
    cout << "Enter an option:" << endl;
    cout << "1. Show the calendar of the property on a given month" << endl;
    cout << "2. Book a period of time for a given property" << endl;
    cout << "q. Quit" << endl;
}

void showMonth(char calendar[][31], int month){
    for (int i = 0; i < 31; ++i) {
        if(i % 7 == 0 && i != 0)
            cout << endl;
        cout << calendar[month][i];
        if(i != 30)
            cout << " ";
    }
    cout << endl << endl;
}

bool invalidMonth(int startM, int endM) {
    if (startM > 31 || startM <= 0 || endM > 31 || endM <= 0) {
        return true;
    }
    return false;
}

bool invvalidDay(int startD, int endD) {
    if (startD > 31 || startD <= 0 || endD > 31 || endD <= 0) {
        return true;
    }
    return false;
}



int main() {
    ifstream fin("properties.txt"); //"properties.txt"
    vector<Property> properties;

    int startD, startM, endD, endM;
    for (int i = 0; i < 4; i++) {
        Property house;
        fill(house);

        getline(fin, house.address);


        house.address = cleanString(house.address);



        fin >> house.price;
        fin >> house.tenants;
        for (int j = 0; j < house.tenants; j++) {
            fin >> startD;
            fin >> startM;
            fin >> endD;
            fin >> endM;
            BookCal(house, startM, startD, endM, endD);
        }
        properties.push_back(house);
        fin.ignore(2, '\n');
        fin.ignore(2, '\n');
    }

    string address;

    do {
        int index = -1;
        do {
            Addy(); // printing out da string w spice
            getline(cin, address);

            address = cleanString(address);



            index = findHouse(properties, address);

            if (address.find("exit") != string::npos) {
                ofstream fout("yearly.txt");
                fout << fixed << setprecision(2);
                for (int i = 0; i < 4; i++) {
                    float total = 0;
                    fout << "Address: " << properties[i].address << endl;
                    for (int j = 0; j < 12; j++) {
                        if (MonthCount(properties[i], j) > 0) {
                            fout << NumberToMonth(j) << ": ";
                            fout << MonthCount(properties[i], j) << endl;
                            total += MonthCount(properties[i], j);
                            for (int k = 0; k < 28; k++) {
                                if (k % 7 == 0 && k != 0) {
                                    fout << endl;
                                }
                                fout << properties[i].calendar[j][k] << " ";
                            }
                            fout << endl;
                            fout << properties[i].calendar[j][28] << " "
                                 << properties[i].calendar[j][29] << " "
                                 << properties[i].calendar[j][30] << endl;
                            fout << endl;
                        }
                    }
                    fout << "Total: $" << total << endl << endl;
                }
                return 0;
            }
            if (index == -1) {
                cout << "Address not found" << endl;
            }
        } while (index == -1);

        char choice;
        int month;
        do {
            showMenu();
            cin >> choice;
            switch (choice) {
                case '1':
                    cout << "Enter the month number:" << endl;
                    cin >> month;
                    if(month <= 0 || month > 12) {
                        cout << "Invalid month number" << endl;
                        break;
                    }
                    showMonth(properties[index].calendar, month - 1);
                    break;
                case '2':
                    cout << "Enter the start month(#) followed by a space and the day:" << endl;
                    cin >> startM;
                    cin >> startD;
                    cout << "Enter the end month(#) followed by a space and day:" << endl;
                    cin >> endM;
                    cin >> endD;
                    if (invalidMonth(startM, endM)) {
                        cout << "Invalid month number" << endl;
                        break;
                    }
                    if (invvalidDay(startD, endD)) {
                        cout << "Invalid day number" << endl;
                        break;
                    }
                    if(!isAvailable(properties[index],startD,startM,endD,endM)){
                        cout << "Date range has dates that are already booked!" << endl;
                        break;
                    }
                    BookCal(properties[index], startD, startM, endD, endM);
                    cout << "Booked!" << endl;
                    break;
                case 'q':
                    break;
                default:
                    cout << "Invalid option" << endl;
                    break;
            }

        } while (choice != 'q');
        cin.ignore(2, '\n');
    } while (address.find("exit") == string::npos);

    return 0;
}
*/
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

string cleanString(string s){
    string tempString;
    for (int i = 0; i < s.length(); ++i) {
        if(isdigit(s[i]) || isalpha(s[i]) || s[i] == ' ' || s[i] == '.'){
            tempString += s[i];
        }
    }

    return tempString;

}

bool isAWeekend(int day) {
    int weekends[9] = {0, 6, 7, 13, 14, 20, 21, 27, 28};
    for (int i = 0; i < 9; i++) {
        if (weekends[i] == day) {
            return true;
        }
    }
    return false;
}

struct Property {
    string address;
    float price;
    int tenants;
    char calendar[12][31];

    void printCalendar(){
        for(int month = 0; month < 12; month++){
            for(int day = 0; day < 31; day++){
                cout << calendar[month][day] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    void changeDS(int mIndex, int dIndex){
        if (isAWeekend(dIndex)) {
            calendar[mIndex][dIndex] = 'D';
        }
        else {
            calendar[mIndex][dIndex] = 'B';
        }
    };
};

void fill(Property& house) {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 31; j++) {
            house.calendar[i][j] = '*';
        }
    }
}

void BookCal(Property& house, int startD, int startM, int endD, int endM) {
    int days = 31 * (endM - startM) + (endD - startD) + 1;
    for (int i = 0; i < days; i++) {
        house.changeDS(startM - 1, startD - 1);
        startD++;
        if (startD == 31 && days > 1) {
            startD = 0; /// how does this make sense
            startM++;
        }
    }
}

bool isAvailable(Property &house, int startD, int startM, int endD, int endM){
    int duration = 31 * (endM - startM) + (endD - startD) + 1;
    if(duration == 372)
        return true;

    for (int i = 0; i < duration; ++i) {
        if(startD == 31 && startM != 12) {
            startM++;
            startD = 1;
        }
        if(house.calendar[startM - 1][startD - 1] != '*') //IS BOOKED
            return false;
        startD++;
    }
    return true; // IS NOT BOOKED
}

string NumberToMonth(int num) {
    switch (num + 1) {
        case 1:
            return "January";
        case 2:
            return "February";
        case 3:
            return "March";
        case 4:
            return "April";
        case 5:
            return "May";
        case 6:
            return "June";
        case 7:
            return "July";
        case 8:
            return "August";
        case 9:
            return "September";
        case 10:
            return "October";
        case 11:
            return "November";
        case 12:
            return "December";
    }
    return "";
}

int findHouse(vector<Property>& houses, string address) {
    for (int i = 0; i < houses.size(); i++) {
        if (houses[i].address.find(address) != string::npos) {
            return i;
        }
    }
    return -1;
}

float MonthCount(Property& house, int month) {
    float count = 0;
    for (int i = 0; i < 31; i++) {
        if (house.calendar[month][i] == 'B') {
            count += house.price;
        }
        if (house.calendar[month][i] == 'D') {
            count += house.price;
            count += house.price;
        }
    }
    return count;
}

void Addy() {
    cout << "Enter the address of the property you want to book or exit to exit:" << endl;
}

void showMenu() {
    cout << "Enter an option:" << endl;
    cout << "1. Show the calendar of the property on a given month" << endl;
    cout << "2. Book a period of time for a given property" << endl;
    cout << "q. Quit" << endl;
}

void showMonth(char calendar[][31], int month){
    for (int i = 0; i < 31; ++i) {
        if(i % 7 == 0 && i != 0)
            cout << endl;
        cout << calendar[month][i];
        if(i != 30)
            cout << " ";
    }
    cout << endl << endl;
}

bool invalidMonth(int startM, int endM) {
    if (startM > 31 || startM <= 0 || endM > 31 || endM <= 0) {
        return true;
    }
    return false;
}

bool invvalidDay(int startD, int endD) {
    if (startD > 31 || startD <= 0 || endD > 31 || endD <= 0) {
        return true;
    }
    return false;
}



int main() {
    ifstream fin("properties.txt"); //"properties.txt"
    vector<Property> properties;

    int startD, startM, endD, endM;
    for (int i = 0; i < 4; i++) {
        Property house;
        fill(house);

        getline(fin, house.address);


        house.address = cleanString(house.address);



        fin >> house.price;
        fin >> house.tenants;
        for (int j = 0; j < house.tenants; j++) {
            fin >> startD;
            fin >> startM;
            fin >> endD;
            fin >> endM;
            BookCal(house, startM, startD, endM, endD);
        }
        properties.push_back(house);
        fin.ignore(2, '\n');
        fin.ignore(2, '\n');
    }

    string address;

    do {
        int index = -1;
        do {
            Addy(); // printing out da string w spice
            getline(cin, address);

            address = cleanString(address);



            index = findHouse(properties, address);

            if (address.find("exit") != string::npos) {
                ofstream fout("yearly.txt");
                fout << fixed << setprecision(2);
                for (int i = 0; i < 4; i++) {
                    float total = 0;
                    fout << "Address: " << properties[i].address << endl;
                    for (int j = 0; j < 12; j++) {
                        if (MonthCount(properties[i], j) > 0) {
                            fout << NumberToMonth(j) << ": ";
                            fout << MonthCount(properties[i], j) << endl;
                            total += MonthCount(properties[i], j);
                            for (int k = 0; k < 28; k++) {
                                if (k % 7 == 0 && k != 0) {
                                    fout << endl;
                                }
                                fout << properties[i].calendar[j][k] << " ";
                            }
                            fout << endl;
                            fout << properties[i].calendar[j][28] << " "
                                 << properties[i].calendar[j][29] << " "
                                 << properties[i].calendar[j][30] << endl;
                            fout << endl;
                        }
                    }
                    fout << "Total: $" << total << endl << endl;
                }
                return 0;
            }
            if (index == -1) {
                cout << "Address not found" << endl;
            }
        } while (index == -1);

        char choice;
        int month;
        do {
            showMenu();
            cin >> choice;
            switch (choice) {
                case '1':
                    cout << "Enter the month number:" << endl;
                    cin >> month;
                    if(month <= 0 || month > 12) {
                        cout << "Invalid month number" << endl;
                        break;
                    }
                    showMonth(properties[index].calendar, month - 1);
                    break;
                case '2':
                    cout << "Enter the start month and day:" << endl;
                    cin >> startM;
                    cin >> startD;
                    cout << "Enter the end month and day:" << endl;
                    cin >> endM;
                    cin >> endD;
                    if (invalidMonth(startM, endM)) {
                        cout << "Invalid month number" << endl;
                        break;
                    }
                    if (invvalidDay(startD, endD)) {
                        cout << "Invalid day number" << endl;
                        break;
                    }
                    if(!isAvailable(properties[index],startD,startM,endD,endM)){
                        cout << "Date range has dates that are already booked!" << endl;
                        break;
                    }
                    BookCal(properties[index], startD, startM, endD, endM);
                    cout << "Booked!" << endl;
                    break;
                case 'q':
                    break;
                default:
                    cout << "Invalid option" << endl;
                    break;
            }

        } while (choice != 'q');
        cin.ignore(2, '\n');
    } while (address.find("exit") == string::npos);

    return 0;
}
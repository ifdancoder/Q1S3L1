#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Person {
protected:
    string sc_nm, fr_nm;
    optional<string> md_nm;
public:
    Person(string sc_nm, string fr_nm, optional<string> md_nm = nullopt);
    string GetSecName() const;

    bool operator<(const Person& person);
    bool operator==(const Person& person);
    friend ostream& operator<<(ostream& out, const Person& person);
};

struct PhoneNumber {
protected:
    int cntr_cd, city_cd;
    string number;
    optional<int> ext_numb;
public:
    PhoneNumber(int cntr_cd, int city_cd, string number, optional<int> ext_numb = nullopt);

    void operator=(const PhoneNumber& pb);
    bool operator<(const PhoneNumber& pb);
    friend ostream& operator<<(ostream& out, const PhoneNumber& ph_numb);
};

class PhoneBook {
protected:
    vector <pair<Person, PhoneNumber>> pb;
public:
    PhoneBook(ifstream& file);
    void SortByName();
    void SortByPhone();
    tuple<string, PhoneNumber> GetPhoneNumber(string sec_name) const;
    void ChangePhoneNumber(Person pers, PhoneNumber pn);

    friend ostream& operator<<(ostream& out, const PhoneBook& pb);
};
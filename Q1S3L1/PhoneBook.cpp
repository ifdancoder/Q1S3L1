#include "PhoneBook.h"

Person::Person(string sc_nm, string fr_nm, optional<string> md_nm) :
    sc_nm(sc_nm), fr_nm(fr_nm), md_nm(md_nm) {
}

string Person::GetSecName() const {
    return sc_nm;
}

bool Person::operator<(const Person& person) {
    return tie(sc_nm, fr_nm, *md_nm) < tie(person.sc_nm, person.fr_nm, *person.md_nm);
}

bool Person::operator==(const Person& person) {
    return tie(sc_nm, fr_nm, *md_nm) == tie(person.sc_nm, person.fr_nm, *person.md_nm);
}

ostream& operator<<(ostream& out, const Person& person) {
    out << setw(16) << person.sc_nm << setw(16) << person.fr_nm << setw(16) << ((person.md_nm) ? *person.md_nm : "");
    return out;
}

PhoneNumber::PhoneNumber(int cntr_cd, int city_cd, string number, optional<int> ext_numb) :
    cntr_cd(cntr_cd), city_cd(city_cd), number(number), ext_numb(ext_numb) {
}

void PhoneNumber::operator=(const PhoneNumber& pb) {
    tie(cntr_cd, city_cd, number, ext_numb) = tie(pb.cntr_cd, pb.city_cd, pb.number, pb.ext_numb);
}

bool PhoneNumber::operator<(const PhoneNumber& pb) {
    return tie(cntr_cd, city_cd, number, *ext_numb) < tie(pb.cntr_cd, pb.city_cd, pb.number, *pb.ext_numb);
}

ostream& operator<<(ostream& out, const PhoneNumber& ph_numb) {
    string outtmp = "+" + to_string(ph_numb.cntr_cd) + "(" + to_string(ph_numb.city_cd) + ")" +
                    ph_numb.number + ((ph_numb.ext_numb) ? (" " +
                                      to_string(*ph_numb.ext_numb)) : "");
    out << setw(24) << outtmp;
    return out;
}

PhoneBook::PhoneBook(ifstream& file) {
    string tmp;
    while (getline(file, tmp)) {
        optional<string> md_nm = nullopt;
        optional<int> ext_numb = nullopt;

        string sc_nm, fr_nm, number;
        int cntr_cd, city_cd;

        stringstream s1, s2;

        s1 << tmp.substr(0, tmp.find("+") - 1);

        s1 >> sc_nm >> fr_nm;

        string md_nm_tmp;
        if (s1 >> md_nm_tmp) {
            md_nm = md_nm_tmp;
        }

        tmp = tmp.substr(tmp.find("+") + 1, tmp.size() - tmp.find("+"));
        tmp.replace(tmp.find("("), 1, " ");
        tmp.replace(tmp.find(")"), 1, " ");

        s2 << tmp;

        s2 >> cntr_cd >> city_cd >> number;

        int ext_numb_tmp;
        if (s2 >> ext_numb_tmp) {
            ext_numb = ext_numb_tmp;
        }

        pb.push_back(make_pair(Person(sc_nm, fr_nm, md_nm),
                               PhoneNumber(cntr_cd, city_cd, number, ext_numb)));
    }
}

void PhoneBook::SortByName() {
    sort(pb.begin(), pb.end(), [](pair<Person, PhoneNumber> pr1,
                                  pair<Person, PhoneNumber> pr2) {
                                  return pr1.first < pr2.first; });
}

void PhoneBook::SortByPhone() {
    sort(pb.begin(), pb.end(), [](pair<Person, PhoneNumber> pr1,
                                  pair<Person, PhoneNumber> pr2) {
                                  return pr1.second < pr2.second; });
}

tuple<string, PhoneNumber> PhoneBook::GetPhoneNumber(string sec_name) const {
    string str;
    PhoneNumber pn(0, 0, "");
    int cnt = 0;
    for_each(pb.begin(), pb.end(), [&pn, &cnt, &sec_name](pair<Person, PhoneNumber> pr) {
                                      cnt += ((pr.first.GetSecName() == sec_name) ? 1 : 0);
                                      if (cnt == 1) {
                                          pn = pr.second;
                                      }
                                      });
    switch (cnt) {
    case 0:
        str = "not found";
        break;
    case 1:
        str = "";
        break;
    default:
        str = "found more than 1";
        break;
    }
    return tie(str, pn);
}

void PhoneBook::ChangePhoneNumber(Person pers, PhoneNumber pn) {
    auto found = find_if(pb.begin(), pb.end(), [&pers](pair<Person, PhoneNumber> pr)
                                               { return pr.first == pers; });
    if (found != pb.end()) {
        (*found).second = pn;
    }
}

ostream& operator<<(ostream& out, const PhoneBook& phnbk) {
    for (auto tmp : phnbk.pb) {
        out << tmp.first << tmp.second << endl;
    }
    return out;
}
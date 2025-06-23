#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <iostream>
#include <complex>
#include <ctime>
using namespace std;


typedef complex<double> Complex;
bool operator<(const Complex &a, const Complex &b) {return abs(a) < abs(b);}
bool operator>(const Complex &a, const Complex &b) {return b < a;}
bool operator==(const Complex &a, const Complex &b) {return a.real() == b.real() && a.imag() == b.imag();}

struct PersonID {
    int series;
    int number;
    PersonID(): series(0), number(0) {}
    PersonID(int s, int n): series(s), number(n) {}

    bool operator==(const PersonID &other) const {
        return series == other.series && number == other.number;
    }
    bool operator!=(const PersonID &other) const {
        return !(*this == other);
    }
    bool operator<(const PersonID &other) const {
        if (series != other.series) return series < other.series;
        return number < other.number;
    }
    bool operator>(const PersonID &other) const {
        return other < *this;
    }
    friend ostream &operator<<(ostream &os, const PersonID &id) {
        os << id.series << "-" << id.number;
        return os;
    }
};

class Person {
    protected:
        PersonID id;
        string name;
        string patronymic;
        string surname;
        time_t birth;
    public:
        Person(): birth(0) {}
        Person(const PersonID &id, const string &name, const string &patronymic, const string &surname, time_t birth):
            id(id), name(name), patronymic(patronymic), surname(surname), birth(birth) {}

        const PersonID &GetPersonId() const {return id;}
        const string &GetName() const {return name;}
        const string &GetPatronymic() const {return patronymic;}
        const string &GetSurname() const {return surname;}
        time_t GetBirth() const {return birth;}
        string GetFullName() const {return name + " " + patronymic + " " + surname;}
        bool operator==(const Person &other) const {return id == other.id;}
        bool operator!=(const Person &other) const {return !(*this == other);}
        bool operator<(const Person &other) const {return id < other.id;}
        bool operator>(const Person &other) const {return other < *this;}
        friend ostream &operator<<(ostream &os, const Person &person) {
            os << person.GetFullName() << " (ID: " << person.id << ")";
            return os;
        }
};

class Student: public Person {
    private:
        int studid;
        string group;
        double grade;
    public:
        Student(): studid(0), grade(0.0) {}
        Student(const PersonID &id, const string &name, const string &patronymic,
                const string &surname, time_t birth, int i, const string &group, double grade):
                Person(id, name, patronymic, surname, birth), studid(i), group(group), grade(grade) {}

        const string &GetGroup() const {return group;}
        int GetStudentId() const {return studid;}
        double GetGrade() const {return grade;}
        friend ostream &operator<<(ostream &os, const Student &student) {
            os << "Студент: " << student.GetFullName()
            << " (ID: " << student.id << "), Группа: " << student.group
            << ", Средний балл: " << student.grade;
            return os;
        }
};

class Teacher: public Person {
private:
    string department;
    string position;
    int experience;
public:
    Teacher(): experience(0) {}
    Teacher(const PersonID &id, const string &name, const string &patronymic,
            const string &surname, time_t birth, const string &d, const string &p, int e):
            Person(id, name, patronymic, surname, birth), department(d), position(p), experience(e) {}

    const string &GetDepartment() const {return department;}
    const string &GetPosition() const {return position;}
    int GetExperience() const {return experience;}
    friend ostream &operator<<(ostream &os, const Teacher &teacher) {
        os << "Учитель: " << teacher.GetFullName()
           << " (ID: " << teacher.id << "), Кафедра: " << teacher.department
           << ", Должность: " << teacher.position
           << ", Стаж (в годах): " << teacher.experience;
        return os;
    }
};

#endif // CLASSES_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <string>
using namespace std;
struct Class
{
    double start_time;
    double end_time;
    int capacity;
    string course_name;
    Class &operator=(const Class &other)
    {
        if (this == &other)
            return *this;
        this->start_time = other.start_time;
        this->end_time = other.end_time;
        this->capacity = other.capacity;
        this->course_name = other.course_name;
        return *this;
    }
    Class() : start_time(0.0), end_time(0.0), capacity(0), course_name("") {}
    Class(double start,double end, int total,string course) : start_time(start), end_time(end), capacity(total),course_name(course) {}
    Class(const Class &other) : start_time(other.start_time), end_time(other.end_time), capacity(other.capacity), course_name(other.course_name) {}
};

struct Student
{
    vector<Class> CourseSchedule; 
    string student_name;
    double break_time; 
    double bus_start;
    double bus_return;
    Student(): CourseSchedule(), student_name(""), break_time(0.0), bus_start(0.0), bus_return(0.0) {}
    Student(const std::vector<Class>& schedule, string name,double break_time_val, double bus_start_time, double bus_return_time): 
    CourseSchedule(schedule), student_name(name),break_time(break_time_val), bus_start(bus_start_time), bus_return(bus_return_time) {}
    Student(vector<Class> schedule, string name, double student_break, double begin, double bus_return_time):
    CourseSchedule(schedule), student_name(name), break_time(student_break), bus_start(begin), bus_return(bus_return_time) {}
};

struct Bus
{
    double start_time;
    double return_time;
    double stop_time;
    int cost;
};

int main(int argc, char *argv[])
{

}
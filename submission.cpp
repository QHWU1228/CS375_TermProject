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
    Class(double start, double end, int total, string course) : start_time(start), end_time(end), capacity(total), course_name(course) {}
    Class(const Class &other) : start_time(other.start_time), end_time(other.end_time), capacity(other.capacity), course_name(other.course_name) {}
};

struct Student
{
    vector<Class> CourseSchedule;
    string student_name;
    double break_time_start;
    double break_time_end;
    double bus_start;
    double bus_return;
    int credits;
    Student() : CourseSchedule(), student_name(""), break_time_start(0.0), break_time_end(0.0), bus_return(0.0), credits(0.0) {}

    Student(vector<Class> schedule, string name, double student_break_start, double student_break_end, double begin, double bus_return_time, int credit_own) : CourseSchedule(schedule), student_name(name), break_time_start(student_break_start), break_time_end(student_break_end),
                                                                                                                                                               bus_start(begin), bus_return(bus_return_time), credits(credit_own) {}

    Student(const Student &other) : CourseSchedule(other.CourseSchedule), student_name(other.student_name),
                                    break_time_start(other.break_time_start), break_time_end(other.break_time_end), bus_start(other.bus_start),
                                    bus_return(other.bus_return), credits(other.credits) {}
    Student &operator=(const Student &other)
    {
        if (this != &other)
        {
            CourseSchedule = other.CourseSchedule;
            student_name = other.student_name;
            break_time_start = other.break_time_start;
            break_time_end = other.break_time_end;
            bus_start = other.bus_start;
            bus_return = other.bus_return;
            credits = other.credits;
        }
        return *this;
    }
};

struct Bus
{
    double start_time;
    double stop_time;
    double return_time;
    double cost;

    Bus &operator=(const Bus &other)
    {
        if (this == &other)
            return *this;
        this->start_time = other.start_time;
        this->stop_time = other.stop_time;
        this->return_time = other.return_time;
        this->cost = other.cost;
        return *this;
    }
    Bus() : start_time(0.0), stop_time(0.0), return_time(0.0), cost(0) {}
    Bus(double start, double stop, double r, int ride_cost) : start_time(start), stop_time(stop),
                                                              return_time(r), cost(ride_cost) {}
    Bus(const Bus &other) : start_time(other.start_time), stop_time(other.stop_time), return_time(other.return_time),
                            cost(other.cost) {}
};

struct Bus_and_Class
{
    Student student;
    Bus optimal_bus_start;
    Bus optimal_bus_return;
    Bus_and_Class &operator=(const Bus_and_Class &other)
    {
        if (this != &other)
        {
            student = other.student;
            optimal_bus_start = other.optimal_bus_start;
            optimal_bus_return = other.optimal_bus_return;
        }
        return *this;
    }
    Bus_and_Class() : student(), optimal_bus_start(),optimal_bus_return() {}
    Bus_and_Class(Student me, Bus start, Bus busReturn) : student(me), optimal_bus_start(start), optimal_bus_return(busReturn){}
    Bus_and_Class(const Bus_and_Class &other) : student(other.student), optimal_bus_start(other.optimal_bus_start),optimal_bus_return(other.optimal_bus_return) {}
};

int finding_courses(vector<Class> courses, string target)
{
    int find = -1;
    for (long unsigned int i = 0; i < courses.size(); i++)
    {
        if (courses[i].course_name == target)
        {
            if (courses[i].capacity > 0)
                return i;
            else
                return find;
        }
        else
            continue;
    }
    return find;
}

bool sortOnCredits(Student a, Student b)
{
    return a.credits > b.credits;
}

bool sortOnStartTimes(Class a, Class b)
{
    return a.start_time < b.start_time;
}

void joining_courses(vector<Class> &courses, Student &student)
{
    // current idea, use the vector<Class> CourseSchedule from the student as a planner for the student
    // so we are basically determining whether a student can take the courses listed in their current schedule
    vector<Class> schedule;
    vector<Class> planner = student.CourseSchedule;
    for (long unsigned int i = 0; i < planner.size(); i++)
    {
        int course_index = finding_courses(courses, planner[i].course_name);
        if (course_index != -1)
        {
            // finding whether there is conflict
            //  between the courses, bus, and break time
            if (schedule.empty())
                schedule.push_back(planner[i]);
            else
            {
                int last = schedule.size() - 1;
                if (schedule[last].end_time < planner[i].start_time)
                {
                    if (planner[i].end_time < student.break_time_start || planner[i].start_time > student.break_time_end)
                    {
                        if (planner[i].end_time < student.bus_start || planner[i].start_time > student.bus_return)
                        {
                            schedule.push_back(planner[i]);
                        }
                    }
                }
            }
        }
        else
            continue;
    }
    // turn the planner into actual schedule without time conflict
    student.CourseSchedule = schedule;
}

double minConvert(double time)
{
    return int(time) * 60 + (time - int(time)) * 100;
}

double costPerMinute(double StartTime, double EndTime, double cost)
{
    // Convert the time to minutes
    double startMinutes = minConvert(StartTime);
    double endMinutes = minConvert(EndTime);

    double costperMin = 0;
    return costperMin = cost / (endMinutes - startMinutes);
}

Bus optimal_bus_start(vector<Bus> bus_schedule)
{
    Bus optimialBus = bus_schedule[0];
    double minCostPerMinute = costPerMinute(bus_schedule[0].start_time, bus_schedule[0].stop_time, bus_schedule[0].cost);

    for (long unsigned int i = 0; i < bus_schedule.size(); i++)
    {
        double currentCostPerMinute = costPerMinute(bus_schedule[i].start_time, bus_schedule[i].stop_time, bus_schedule[i].cost);

        if (currentCostPerMinute < minCostPerMinute)
        {
            minCostPerMinute = currentCostPerMinute;
            optimialBus = bus_schedule[i];
        }
    }

    return optimialBus;
}

Bus optimal_bus_return(vector<Bus> bus_schedule)
{
    Bus optimialBus = bus_schedule[0];
    double minCostPerMinute = costPerMinute(bus_schedule[0].stop_time, bus_schedule[0].return_time, bus_schedule[0].cost);

    for (long unsigned int i = 0; i < bus_schedule.size(); i++)
    {
        double currentCostPerMinute = costPerMinute(bus_schedule[i].stop_time, bus_schedule[i].return_time, bus_schedule[i].cost);

        if (currentCostPerMinute < minCostPerMinute)
        {
            minCostPerMinute = currentCostPerMinute;
            optimialBus = bus_schedule[i];
        }
    }

    return optimialBus;
}

void schedule_bus_start(vector<Bus> bus_schedule, Student student, vector<Bus> &qualifyBus)
{
    for (long unsigned int i = 0; i < bus_schedule.size(); i++)
    {
        if (minConvert(bus_schedule[i].start_time) > minConvert(student.bus_start))
        {
            qualifyBus.push_back(bus_schedule[i]);
            break;
        }
    }
}

void schedule_bus_return(vector<Bus> bus_schedule, Student student, vector<Bus> &qualifyBus)
{
    for (long unsigned int i = 0; i < bus_schedule.size(); i++)
    {
        if (minConvert(bus_schedule[i].return_time) > minConvert(student.bus_return))
        {
            qualifyBus.push_back(bus_schedule[i]);
            break;
        }
    }
}

// next step: probably create a planner for each student in the students vector
int main()
{
    vector<Class> course;
    // having 10 courses now
    Class physics = Class(10.0, 11.30, 3, "Physic 131");
    course.push_back(physics);
    Class cs = Class(8.30, 9.30, 5, "Computer Science 375");
    course.push_back(cs);
    Class cal2 = Class(13.0, 14.30, 2, "Math 226");
    course.push_back(cal2);
    Class biolab = Class(15.0, 17.30, 4, "Bio Lab");
    course.push_back(biolab);
    Class chem = Class(15.0, 16.30, 6, "Chemistry");
    course.push_back(chem);
    Class number_system = Class(9.30, 11.10, 3, "Number System");
    course.push_back(biolab);
    Class cal3 = Class(14.0, 15.30, 7, "Math 323");
    course.push_back(cal3);
    Class film = Class(12.0, 14.0, 10, "Introduction to Film");
    course.push_back(film);
    Class geo = Class(10.0, 11.00, 1, "Intro to Geography");
    course.push_back(geo);
    Class literature = Class(15.0, 17.30, 9, "Intro to literature");
    course.push_back(literature);

    //---------------------------------------------------------
    vector<Bus> bus1; // Express
    bus1.push_back(Bus(15.15, 15.30, 15.40, 4));
    bus1.push_back(Bus(16.15, 16.30, 16.40, 4));
    bus1.push_back(Bus(17.15, 17.30, 17.40, 4));
    bus1.push_back(Bus(18.15, 18.30, 18.40, 4));
    bus1.push_back(Bus(19.15, 19.30, 19.40, 4));
    bus1.push_back(Bus(20.15, 20.30, 20.40, 4));

    vector<Bus> bus2; // Normal
    bus2.push_back(Bus(15.05, 15.40, 15.50, 2));
    bus2.push_back(Bus(16.05, 16.40, 16.50, 2));
    bus2.push_back(Bus(17.05, 17.40, 17.50, 2));
    bus2.push_back(Bus(18.05, 18.40, 18.50, 2));
    bus2.push_back(Bus(19.05, 19.40, 19.50, 2));
    bus2.push_back(Bus(20.05, 20.40, 20.50, 2));

    vector<Bus> bus3; // Local
    bus3.push_back(Bus(13.50, 14.40, 15.00, 1));
    bus3.push_back(Bus(14.50, 15.40, 16.00, 1));
    bus3.push_back(Bus(15.50, 16.40, 17.00, 1));
    bus3.push_back(Bus(16.50, 17.40, 18.00, 1));
    bus3.push_back(Bus(17.50, 18.40, 19.00, 1));
    bus3.push_back(Bus(18.50, 19.40, 20.00, 1));
    //---------------------------------------------------------
    vector<Student> students;
    // total of 15 students now
    // CourseSchedule, break_time_start, break_time_end, bus_start, bus_return, credits
    // physics, cs , cal2, biolab, chem, number_system , cal3,film, geo, literature
    students.push_back(Student({literature, cs, physics, cal2, geo}, "John", 12.0, 13.0, 15.0, 18.0, 15));
    students.push_back(Student({chem, number_system, cal3, film}, "Kevin", 8.0, 12.0, 14.0, 15.0, 8));
    students.push_back(Student({geo, biolab, cs, physics, literature}, "Tony", 13.3, 15.4, 18.0, 20.0, 12));
    students.push_back(Student({number_system, physics, chem, biolab, geo}, "David", 17.0, 20.0, 13.0, 14.0, 20));
    students.push_back(Student({geo, chem, number_system, cal3}, "James", 16.0, 18.0, 14.0, 14.4, 16));
    students.push_back(Student({cal2, film, geo, literature}, "Noah", 12.0, 12.50, 13.0, 14.35, 18));
    students.push_back(Student({number_system, physics, biolab, geo, cal3}, "Jack", 13.3, 15.0, 17.0, 18.5, 19));
    students.push_back(Student({cs, cal2, film, literature, physics}, "Jacob", 14.0, 15.5, 14.0, 16.0, 10));
    students.push_back(Student({literature, cal3, number_system, chem}, "Henry", 12.0, 13.3, 18.0, 19.5, 22));
    students.push_back(Student({biolab, chem, literature, geo}, "Sophia", 10.0, 12.5, 16.0, 17.3, 14));
    students.push_back(Student({chem, number_system, cal3, biolab}, "Aria", 14.0, 16.0, 12.45, 13.30, 12));
    students.push_back(Student({film, geo, literature, cal2, physics}, "Lily", 15.0, 16.3, 13.33, 14.22, 24));
    students.push_back(Student({cs, geo, chem, cal3, literature}, "Charles", 15.0, 17.0, 15.3, 15.5, 28));
    students.push_back(Student({film, number_system, physics, biolab, geo}, "Mike", 14.0, 16.5, 19.3, 20.0, 10));
    //---------------------------------------------------------
    sort(students.begin(), students.end(), sortOnCredits);
    for (long unsigned int i = 0; i < students.size(); i++)
    {
        sort(students[i].CourseSchedule.begin(), students[i].CourseSchedule.end(), sortOnStartTimes);
    }
    for (long unsigned int i = 0; i < students.size(); i++)
    {
        joining_courses(students[i].CourseSchedule, students[i]);
    }
    vector<Bus_and_Class> result_schedule;
    for (auto student : students)
    {
        vector<Bus> qualifyBus_start;
        vector<Bus> qualifyBus_return;

        schedule_bus_start(bus1, student, qualifyBus_start);
        schedule_bus_start(bus2, student, qualifyBus_start);
        schedule_bus_start(bus3, student, qualifyBus_start);

        schedule_bus_return(bus1, student, qualifyBus_return);
        schedule_bus_return(bus2, student, qualifyBus_return);
        schedule_bus_return(bus3, student, qualifyBus_return);

        Bus optimalBusStart = optimal_bus_start(qualifyBus_start);
        Bus optimalBusReturn = optimal_bus_return(qualifyBus_return);

        // cout << "Student: " << student.student_name << endl;
        // for (const auto &courses : student.CourseSchedule)
        // {
        //     cout << "Class: " << courses.course_name
        //          << ", " << courses.start_time
        //          << " - " << courses.end_time << endl;
        // }
        Bus_and_Class result;
        result.student = student;
        result.optimal_bus_start = optimalBusStart;
        result.optimal_bus_return = optimalBusReturn;
        result_schedule.push_back(result);

        // cout << endl;
        // cout << "Optimal Start Bus: Start Time - " << optimalBusStart.start_time << ", Stop Time - " << optimalBusStart.stop_time << ", Cost - " << optimalBusStart.cost << endl;
        // cout << "Optimal Return Bus: Stop Time - " << optimalBusReturn.stop_time << ", Return Time - " << optimalBusReturn.return_time << ", Cost - " << optimalBusReturn.cost << endl;
        // cout << "---------------------" << endl;
    }
    for(auto result : result_schedule)
    {
        cout << "Student: " << result.student.student_name << endl;
        for (const auto &courses : result.student.CourseSchedule)
        {
            cout << "Class: " << courses.course_name
                 << ", " << courses.start_time
                 << " - " << courses.end_time << endl;
        }
        cout << endl;
        cout << "Optimal Start Bus: Start Time - " << result.optimal_bus_start.start_time << ", Stop Time - " << result.optimal_bus_start.return_time << ", Cost - " << result.optimal_bus_start.cost << endl;
        cout << "Optimal Return Bus: Stop Time - " << result.optimal_bus_return.stop_time << ", Return Time - " << result.optimal_bus_return.return_time << ", Cost - " << result.optimal_bus_return.cost << endl;
        cout << "---------------------" << endl;
    }
}
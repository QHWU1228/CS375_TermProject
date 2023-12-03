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
    double break_time_start; 
    double break_time_end;
    double bus_start;
    double bus_return;
    int credits;
    Student(): CourseSchedule(), student_name(""), break_time_start(0.0), break_time_end(0.0), bus_return(0.0), credits(0.0) {}

    Student(vector<Class> schedule, string name, double student_break_start, double student_break_end, double begin, double bus_return_time,int credit_own):
    CourseSchedule(schedule), student_name(name), break_time_start(student_break_start),break_time_end(student_break_end),
    bus_start(begin), bus_return(bus_return_time), credits(credit_own) {}

    Student(const Student &other) : CourseSchedule(other.CourseSchedule), student_name(other.student_name),
    break_time_start(other.break_time_start), break_time_end(other.break_time_end), bus_start(other.bus_start),
    bus_return(other.bus_return), credits(other.credits) {}
};

struct Bus
{
    double start_time;
    double return_time;
    double stop_time;
    int cost;
     Bus &operator=(const Bus &other)
    {
        if (this == &other)
            return *this;
        this->start_time = other.start_time;
        this->return_time = other.return_time;
        this->stop_time = other.stop_time;
        this->cost = other.cost;
        return *this;
    }
    Bus() : start_time(0.0),return_time(0.0), stop_time(0.0), cost(0) {}
    Bus(double start, double r, double stop, int ride_cost) : start_time(start), return_time(r), 
    stop_time(stop), cost(ride_cost) {}
    Bus(const Bus &other): start_time(other.start_time),return_time(other.return_time),stop_time(other.stop_time),
    cost(other.cost) {}
};

int finding_courses(vector<Class>courses, string target)
{
    int find = -1;
    for(long unsigned int i = 0; i < courses.size(); i++)
    {
        if(courses[i].course_name == target)
        {
            if(courses[i].capacity > 0) return i;
            else return find;
        }
        else continue;
    }
    return find;
}

bool finding_conflict(vector<Class>schedule,Student student,Class course)
{
   for(long unsigned int i = 0; i < schedule.size();i++)
   {
        if(schedule[i].start_time < course.start_time && schedule[i].end_time > course.end_time)
        {
            return false;
        }
        else
        {
            if(student.break_time_start < course.start_time && student.break_time_end > course.end_time)
            {
                return false;
            }
            else continue;
        }
   }
   return true;
}
bool sortOnCredits(Student a, Student b)
{
    return a.credits > b.credits;
}

void joining_courses(vector<Class>&courses, Student& student)
{
    // current idea, use the vector<Class> CourseSchedule from the student as a planner for the student 
    // so we are basically determining whether a student can take the courses listed in their current schedule 
    vector<Class> schedule;
    vector<Class> planner = student.CourseSchedule;
    for(long unsigned int i = 0; i <planner.size(); i++)
    {
       int course_index = finding_courses(courses,planner[i].course_name);
       if (course_index != -1)
       {
            //finding whether there is conflict between the current schedule and planner
            bool result = finding_conflict(schedule, student, courses[course_index]);
            if(result)
            {
                schedule.push_back(courses[course_index]);
                courses[course_index].capacity--;
            }
       }
       else continue;
    }
    // turn the planner into actual schedule without time conflict
    student.CourseSchedule = schedule;
}

double minConvert(double time)
{
    return int(time) * 60 + (time - int(time)) * 100 ; 
}

double costPerMin(double StartTime, double EndTime, double cost)
{
    // Convert the time to minutes 
    double startMinutes = minConvert(StartTime);
    double endMinutes = minConvert(EndTime);

    double costperMin = 0 ; 
    return costperMin = cost/(endMinutes - startMinutes); 
}

Bus optimal_bus(vector<Bus>bus_schedule)
{
    Bus optimialBus = bus_schedule[0]; 
    double minCostPerMinute = costPerMinute(bus_schedule[0].start_time, bus_schedule[0].return_time, bus_schedule[0].cost);

    for(long unsigned int i = 0; i < bus_schedule.size(); i++ )
    {
        double currentCostPerMinute = costPerMinute(bus_schedule[i].start_time, bus_schedule[i].return_time, bus_schedule[i].cost);

        if(currentCostPerMinute < minCostPerMinute)
        {
            minCostPerMinute = currentCostPerMinute;
            optimialBus = bus_schedule[i]; 
        }
    }

    return optimialBus;
}

void schedule_bus(vector<Bus>bus_schedule, Student student, vector<Bus> &qualifyBus)
{   
    for(long unsigned int i = 0; i < bus_schedule.size(); i++ )
    {
        if(minConvert(bus_schedule[i].start_time) > minConvert(student.bus_start))
        {
            qualifyBus.push_back(bus_schedule[i]) ;
            break;
        }
    }
}

int main(int argc, char *argv[])
{

}
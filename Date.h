#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <vector>
#include <iomanip>

class Date {
public:
    enum class Month {January = 1, February, March, April, May, June, July, August, September, October, November, December};

    // Inner class for invalid date exception
    class InvalidDate {
    public:
        const char* notDate() const;
    };

    // Default constructor
    Date();
    
    // Overloaded constructor
    Date(int month, int day, int year);

    // Destructor
    ~Date();

    // Getter and Setter functions
    int getMonth() const;
    int getDay() const;
    int getYear() const;

    void setMonth(int m);
    void setDay(int d);
    void setYear(int y);

    // Function to list all dates in a year
    std::vector<Date> listAllDates(int y) const;

    // Function to show calendar for a year
    void showCalendar(int y) const;

    // Function to check if a year is a leap year
    static bool isALeapYear(int y);

    bool isValidDate(int m, int d, int y) const;
    int getDaysInMonth(int m, int y) const;
    std::string getMonthName(Month m) const;
    void incrementDate();
    void decrementDate();
    void addDays(int days);

    // toString function to display date in different formats
    std::string toString() const;

    // Overloaded << operator
    friend std::ostream& operator<<(std::ostream& out, const Date& date);

    // Overloaded >> operator
    friend std::istream& operator>>(std::istream& in, Date& date);

    // Overloaded prefix and postfix increment operators
    Date& operator++(); // Prefix
    Date operator++(int); // Postfix
    Date& operator--(); // Prefix
    Date operator--(int); // Postfix

    // Overloaded addition operator
    Date operator+(int days) const;

    // Overloaded subtraction operator
    Date operator-(int days) const;

private:
    int month, day, year;
};

#endif

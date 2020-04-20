#ifndef EXCEPTIONCLASSES_H
#define EXCEPTIONCLASSES_H

#include <stdexcept>

class TooManyColsException : public std::runtime_error {
public:
    TooManyColsException() : std::runtime_error("TooManyCols") { }
};

class NoTableException : public std::runtime_error {
public:
    NoTableException() : std::runtime_error("NoTable") { }
};

class NoDivisionException : public std::runtime_error {
public:
    NoDivisionException() : std::runtime_error("NoDivision") { }
};

class NoDataException : public std::runtime_error {
public:
    NoDataException() : std::runtime_error("NoDataException") { }
};

#endif // EXCEPTIONCLASSES_H

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

class PageUnavailibleException : public std::runtime_error {
public:
    PageUnavailibleException() : std::runtime_error("PageUnavailibleException") { }
};

class EmptyTableException : public std::runtime_error {
public:
    EmptyTableException() : std::runtime_error("EmptyTableException") { }
};

class FileUnavailibleException : public std::runtime_error {
public:
    FileUnavailibleException() : std::runtime_error("FileUnavailibleException") { }
};

class NoPluginsException : public std::runtime_error {
public:
    NoPluginsException() : std::runtime_error("NoPluginsException") { }
};

class DistributionCorruptedException : public std::runtime_error {
public:
    DistributionCorruptedException() : std::runtime_error("DistributionCorruptedException") { }
};

#endif // EXCEPTIONCLASSES_H

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

class EmptyTimestampException : public std::runtime_error {
public:
    EmptyTimestampException() : std::runtime_error("EmptyTimestampException") { }
};

class NoSuchPluginException : public std::runtime_error {
public:
    NoSuchPluginException() : std::runtime_error("NoSuchPluginException") { }
};

class SqlQueryErrorException : public std::runtime_error {
public:
    SqlQueryErrorException() : std::runtime_error("SqlQueryErrorException") { }
};

class NoSuchItemInStatisticsConfig : public std::runtime_error {
public:
    NoSuchItemInStatisticsConfig() : std::runtime_error("NoSuchItemInStatisticsConfig") { }
};

class NoSuchGroupInStatisticsConfig : public std::runtime_error {
public:
    NoSuchGroupInStatisticsConfig() : std::runtime_error("NoSuchGroupInStatisticsConfig") { }
};

class NoSuchCategoryInStatisticsConfig : public std::runtime_error {
public:
    NoSuchCategoryInStatisticsConfig() : std::runtime_error("NoSuchCategoryInStatisticsConfig") { }
};

class ItemAlreadyIsInStatisticsConfig : public std::runtime_error {
public:
    ItemAlreadyIsInStatisticsConfig() : std::runtime_error("ItemAlreadyIsInStatisticsConfig") { }
};

#endif // EXCEPTIONCLASSES_H

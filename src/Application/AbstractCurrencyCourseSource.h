#ifndef ABSTRACTCURRENCYCOURSESOURCE_H
#define ABSTRACTCURRENCYCOURSESOURCE_H

class AbstractCurrencyCourseSource
{
public:
    virtual double getCurrencyCourse(const char *const currency) = 0;
};

#endif // ABSTRACTCURRENCYCOURSESOURCE_H

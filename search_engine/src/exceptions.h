#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <cassert>

using namespace std;

class NoConfigFile: public exception
{
public:
    const char* what() const noexcept override
    {
        return "config file is missing";
    }
};

class ConfigFileNotOpen: public exception
{
public:
    const char* what() const noexcept override
    {
        return "File config.json isn`t open!";
    }
};

class NoConfigField: public exception
{
public:
    const char* what() const noexcept override
    {
        return "Config file is empty";
    }
};

class WrongStructConfig: public exception
{
public:
    const char* what() const noexcept override
    {
        return "Wrong struct in file - config.json";
    }
};

class NoRequestsFile: public exception
{
public:
    const char* what() const noexcept override
    {
        return "requests.json file not found!";
    }
};

class RequestsFileNotOpen: public exception
{
public:
    const char* what() const noexcept override
    {
        return "File requests.json isn`t open!";
    }
};

class NoRequestField: public exception
{
public:
    const char* what() const noexcept override
    {
        return "There is no field \"request\"  in the file requests.json";
    }
};

class WrongStructRequest: public exception
{
public:
    const char* what() const noexcept override
    {
        return "Wrong struct in file - request.json";
    }
};

#endif // EXCEPTIONS_H

#include <string.h>

#include "seg.h"


static int _check_size_(int begin, int end)
{
    if (begin > end)
        return SEG_ERROR_SIZE_INVALID;
    else
        return 0;
}

static int _check_name_(const char *name)
{
    if (name == NULL || strlen(name) == 0)
        return SEG_ERROR_NAME_TOO_SHORT;
    if (strlen(name) >= SEG_MAX_LEN)
        return SEG_ERROR_NAME_TOO_LONG;

    for (int i = 0; i < strlen(name); ++i) {
        if (isdigit(name[i]) != 0 && isalpha(name[i]) != 0 && name[i] != '_')
            return SEG_ERROR_NAME_INVALID;
    }

    return 0;
}

c_seg::c_seg(int begin, int end, const char *name)
{
    if (_check_size_(begin, end) != 0) {
        _begin = 0;
        _end = 0;
    }
    else {
        _begin = begin;
        _end = end;
    }

    bzero(_name, sizeof(_name));
    if (_check_name_(name) == 0) {
        strcpy(_name, name, strlen(name));
    }
}

c_seg::c_seg(const c_seg &other)
{
    _begin = other._begin;
    _end = other._end;
    bzero(_name, sizeof(_name));
    strcpy(_name, other._name);
}

c_seg::~c_seg()
{
}

c_seg &c_seg::operator=(const c_seg &other)
{
    _begin = other._begin;
    _end = other._end;
    strcpy(_name, other._name);
    _name[strlen(name)] = 0;
}

bool c_seg::operator==(const c_seg &other)
{
    if (_begin == other._begin && _end == other._end &&
        strcmp(_name, other._name) == 0)
        return true;
    else
        return false;
}

int c_seg::get_begin() const
{
    return _begin;
}

int c_seg::get_end() const
{
    return _end;
}

int c_seg::get_len() const
{
    return _end - _begin + 1;
}

const char *c_seg::get_name() const
{
    return _name;
}

int c_seg::set_begin(int begin)
{
    if (_check_size_(begin, _end) != 0)
        return SEG_ERROR_SIZE_INVALID;
    else {
        _begin = begin;
        return 0;
    }
}


int c_seg::set_end(int end)
{
    if (_check_size_(_begin, end) != 0)
        return SEG_ERROR_SIZE_INVALID;
    else {
        _end = end;
        return 0;
    }
}

int c_seg::resize(int begin, int end)
{
    if (_check_size_(begin, end) != 0)
        return SEG_ERROR_SIZE_INVALID;
    else {
        _begin = begin;
        _end = end;
        return 0;
    }
}

int c_seg::set_name(const char *name)
{
    if (_check_name_(name) != 0)
        return SEG_ERROR_NAME_INVALID;
    else {
        strcpy(_name, name);
        _name[strlen(name)] = 0;
    }
}

#ifndef _SEG_H_
#define _SEG_H_

class istream;
class ostream;

class c_seg
{
public:
    // constructors
    c_seg();
    c_seg(int begin, int end, char *name=NULL);
    c_seg(seg &other);

    // operator overload
    c_seg &operator=(const c_seg &other);
    bool operator==(const c_seg &other);
    friend istream &operator>>(istream &os, const c_seg &seg);
    friend ostream &operator<<(ostream &os, const c_seg &seg);

    // 
    bool intersect(const c_seg &other);
    c_seg diff(const c_seg &other);

    // get functions
    int get_begin() const;
    int get_end() const;
    const char *get_name() const;

    // set functions
    int set_begin(int begin);
    int set_end(int end);
    int set_name(char *name);
    int resize(int begin, int end, char *name=NULL);

    // destructor
    virtual ~c_seg();

private:
    int _begin;
    int _end;
    char _name[24];
};

class c_segset
{
public:
    c_segset();
    virtual ~c_segset();
};

#endif /* _SEG_H_ */

#ifndef _SEG_SET_H_
#define _SEG_SET_H_

#define SEG_SET_NAME_MAX_LEN 24

namespace std{
    class set;
};

class c_seg;

class c_seg_set
{
public:
    c_seg_set(const char *name = NULL);
    c_seg_set(const c_seg_set &other);
    c_seg_set(const c_seg &seg);
    virtual ~c_seg_set();

    c_seg_set &operator=(const c_seg_set &other);
    bool operator==(const c_seg_set &other);

private:
    std::set<c_seg> *_segs;
    char _name[SEG_SET_NAME_MAX_LEN];
};

#endif /* _SEG_SET_H_ */

#ifndef _SEG_H_
#define _SEG_H_

#define SEG_ERROR_SIZE_INVALID   0b00000100
#define SEG_ERROR_NAME_TOO_SHORT 0b00000101
#define SEG_ERROR_NAME_TOO_LONG  0b00000110
#define SEG_ERROR_NAME_INVALID   0b00000111

#define SEG_MAX_LEN 24

class c_seg
{
public:
    c_seg(int begin = 0, int end = 0, const char *name = NULL);
    c_seg(const c_seg &other);
    virtual ~c_seg();

    c_seg &operator=(const c_seg &other);
    bool operator==(const c_seg &other);

    int get_begin() const;
    int get_end() const;
    int get_len() const;
    const char *get_name() const;

    int set_begin(int begin);
    int set_end(int end);
    int resize(int begin, int end);
    int set_name(const char *name);
private:
    int _begin;
    int _end;
    char _name[SEG_MAX_LEN];
};

#endif /* _SEG_H_ */

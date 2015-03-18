
typedef struct weapon {
    char* name;
    struct weapon *next;
}*weapon;

typedef struct hero{
    int luck;
    char*name;
    int xcor;
    int ycor;
} *hero;

/* Node: Represents one key-value pair. */
typedef struct Node {
    int key;
    void *val;
    struct Node *next;
} Node;

/* Dict: Represents a collection of key-value pairs. */
typedef struct Dict {
    int cap;
    Node **arr;
    int size;
} Dict;

Dict *revdctcreate();
void revdctdestroy(Dict *);
void *revdctget(Dict *, int);
void revdctinsert(Dict *, int, void *);
void *revdctremove(Dict *, char *);
char **revdctkeys(Dict *);

void insertAscii(Dict *);
void writecodes(int short[], FILE *);
void encode(FILE *, FILE *);
void decode(FILE *, FILE *);
void bitUnpacking(unsigned char[], short int[]);
void insertAsciiRev(Dict *);

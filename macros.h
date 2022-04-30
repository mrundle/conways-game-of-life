#define err(fmt, args...) do {                 \
    fprintf(stderr, "%s:%s:%u: " fmt "\n",     \
        __FILE__, __func__, __LINE__, ##args); \
} while (0)

#define min(x, y) \
    ({ \
       typeof(x) _x = (x); \
       typeof(y) _y = (y); \
       _x < _y ? _x : _y; \
    })

#define max(x, y) \
    ({ \
       typeof(x) _x = (x); \
       typeof(y) _y = (y); \
       _x > _y ? _x : _y; \
    })

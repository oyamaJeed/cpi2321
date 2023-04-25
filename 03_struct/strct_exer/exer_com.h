struct st_exer {
	char from;
	char to;
	char msg[30];
};

typedef struct st_exer ST_EXER;

struct st_ary {
	char c_ary[32];
};

typedef struct st_ary ST_ARY;

union uni_exer {
	ST_ARY sary;
	ST_EXER exer;
};

typedef union uni_exer UNI_EXER;

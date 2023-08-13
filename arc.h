#pragma once
#ifndef _INC_ARC
#define _INC_ARC

#define VERSION "0.29.1"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <setjmp.h>

#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#ifdef _MSC_VER
#define strdup _strdup
#define popen _popen
#define pclose _pclose
#endif

enum type {
	T_NIL, 
	T_CONS,
	T_SYM,
	T_NUM,
	T_BUILTIN,
	T_CLOSURE,
	T_MACRO,
	T_STRING,
	T_INPUT,
	T_INPUT_PIPE,
	T_OUTPUT,
	T_TABLE,
	T_CHAR,
	T_CONTINUATION
};

typedef enum {
  ERROR_OK = 0, ERROR_SYNTAX, ERROR_UNBOUND, ERROR_ARGS, ERROR_TYPE, ERROR_FILE, ERROR_USER
} error;

typedef struct atom atom;
struct vector;
typedef error(*builtin)(struct vector *vargs, atom *result);

struct atom {
	enum type type;

	union {
		double number;
		struct pair *pair;
		char *symbol;
		struct str *str;
		builtin builtin;
		FILE *fp;
		struct table *table;
		char ch;
		jmp_buf *jb;
	} value;
};

struct vector {
	atom *data;
	atom static_data[8]; /* small size optimization */
	size_t capacity, size;
};

struct pair {
	struct atom car, cdr;
	char mark;
	struct pair *next;
};

struct str {
	char *value;
	char mark;
	struct str *next;
};

struct table_entry {
	struct atom k, v;
	struct table_entry *next;
};

struct table {
	size_t capacity;
	size_t size;
	struct table_entry **data;
	char mark;
	struct table *next;
};

/* simple string with length and capacity */
struct string {
	char *str;
	size_t len, cap;
};

/* forward declarations */
error apply(atom fn, struct vector *vargs, atom *result);
int listp(atom expr);
char *slurp_fp(FILE *fp);
char *slurp(const char *path);
error eval_expr(atom expr, atom env, atom *result);
void gc_mark(atom root);
void gc();
error macex(atom expr, atom *result);
char *to_string(atom a, int write);
void string_new(struct string* dst);
void string_cat(struct string *dst, char *src);
error macex_eval(atom expr, atom *result);
error arc_load_file(const char *path);
char *get_dir_path(char *file_path);
void arc_init(char *file_path);
#ifndef READLINE
char *readline(char *prompt);
#endif
char *readline_fp(char *prompt, FILE *fp);
error read_expr(const char *input, const char **end, atom *result);
void print_expr(atom a);
void print_error(error e);
int is(atom a, atom b);
int iso(atom a, atom b);
size_t hash_code(atom a);
atom make_table(size_t capacity);
void table_add(struct table *tbl, atom k, atom v);
struct table_entry *table_get(struct table *tbl, atom k);
struct table_entry *table_get_sym(struct table *tbl, char *k);
int table_set(struct table *tbl, atom k, atom v);
int table_set_sym(struct table *tbl, char *k, atom v);
void consider_gc();
atom cons(atom car_val, atom cdr_val);
/* end forward */

#define car(p) ((p).value.pair->car)
#define cdr(p) ((p).value.pair->cdr)
#define no(atom) ((atom).type == T_NIL)

extern const atom nil;

#endif

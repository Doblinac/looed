// rope.h
#ifndef ROPE_H
#define ROPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rope {
    int length;  // 字符串长度
    union {
        struct {
            struct Rope *left;
            struct Rope *right;
        } tree;
        char *string;
    } data;
    int type;  // 0 for string, 1 for tree
} Rope;

Rope *create_rope_from_string(const char *str);
Rope *create_rope_from_tree(Rope *left, Rope *right);
Rope *insert_rope(Rope *rope, int pos, const char *str);
Rope *delete_rope(Rope *rope, int start, int end);
Rope *copy_rope(Rope *rope, int start, int end);
int find_rope(Rope *rope, const char *str);
void free_rope(Rope *rope);
char *to_string(Rope *rope);
char *rope_to_string(Rope *rope);
void write_rope_to_file(Rope *rope, FILE *file) ;
void save_file(const char *filename) ;
void open_file(const char *filename) ;

#endif // ROPE_H
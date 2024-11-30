// rope.c
#include "rope.h"
#include "editor.h"

Rope *create_rope_from_string(const char *str) {
    Rope *rope = (Rope *)malloc(sizeof(Rope));
    rope->length = strlen(str);
    rope->data.string = strdup(str);
    rope->type = 0;
    return rope;
}

Rope *create_rope_from_tree(Rope *left, Rope *right) {
    Rope *rope = (Rope *)malloc(sizeof(Rope));
    rope->length = left->length + right->length;
    rope->data.tree.left = left;
    rope->data.tree.right = right;
    rope->type = 1;
    return rope;
}

Rope *insert_rope(Rope *rope, int pos, const char *str) {
    if (rope == NULL) {
        return create_rope_from_string(str);
    }

    if (pos <= 0) {
        return create_rope_from_tree(create_rope_from_string(str), rope);
    }

    if (pos >= rope->length) {
        return create_rope_from_tree(rope, create_rope_from_string(str));
    }

    if (rope->type == 0) {
        char *left_str = strndup(rope->data.string, pos);
        char *right_str = strdup(rope->data.string + pos);
        Rope *left_rope = create_rope_from_string(left_str);
        Rope *right_rope = create_rope_from_string(right_str);
        Rope *result = create_rope_from_tree(left_rope, create_rope_from_tree(create_rope_from_string(str), right_rope));
        free(left_str);
        free(right_str);
        free(rope->data.string);
        free(rope);
        return result;
    } else {
        if (pos <= rope->data.tree.left->length) {
            Rope *left = insert_rope(rope->data.tree.left, pos, str);
            Rope *result = create_rope_from_tree(left, rope->data.tree.right);
            free(rope);
            return result;
        } else {
            Rope *right = insert_rope(rope->data.tree.right, pos - rope->data.tree.left->length, str);
            Rope *result = create_rope_from_tree(rope->data.tree.left, right);
            free(rope);
            return result;
        }
    }
}

Rope *delete_rope(Rope *rope, int start, int end) {
    if (rope == NULL) {
        return NULL;
    }

    if (end <= 0 || start >= rope->length) {
        return rope;
    }

    if (start <= 0 && end >= rope->length) {
        free_rope(rope);
        return NULL;
    }

    if (rope->type == 0) {
        char *left_str = strndup(rope->data.string, start);
        char *right_str = strdup(rope->data.string + end);
        Rope *left_rope = create_rope_from_string(left_str);
        Rope *right_rope = create_rope_from_string(right_str);
        Rope *result = create_rope_from_tree(left_rope, right_rope);
        free(left_str);
        free(right_str);
        free(rope->data.string);
        free(rope);
        return result;
    } else {
        if (start < rope->data.tree.left->length) {
            if (end <= rope->data.tree.left->length) {
                Rope *left = delete_rope(rope->data.tree.left, start, end);
                Rope *result = create_rope_from_tree(left, rope->data.tree.right);
                free(rope);
                return result;
            } else {
                Rope *left = delete_rope(rope->data.tree.left, start, rope->data.tree.left->length);
                Rope *right = delete_rope(rope->data.tree.right, 0, end - rope->data.tree.left->length);
                Rope *result = create_rope_from_tree(left, right);
                free(rope);
                return result;
            }
        } else {
            Rope *right = delete_rope(rope->data.tree.right, start - rope->data.tree.left->length, end - rope->data.tree.left->length);
            Rope *result = create_rope_from_tree(rope->data.tree.left, right);
            free(rope);
            return result;
        }
    }
}

Rope *copy_rope(Rope *rope, int start, int end) {
    if (rope == NULL) {
        return NULL;
    }

    if (start < 0 || end > rope->length || start > end) {
        return NULL;
    }

    if (rope->type == 0) {
        int len = end - start;
        char *sub_str = strndup(rope->data.string + start, len);
        Rope *result = create_rope_from_string(sub_str);
        free(sub_str);
        return result;
    } else {
        if (end <= rope->data.tree.left->length) {
            return copy_rope(rope->data.tree.left, start, end);
        } else if (start >= rope->data.tree.left->length) {
            return copy_rope(rope->data.tree.right, start - rope->data.tree.left->length, end - rope->data.tree.left->length);
        } else {
            Rope *left = copy_rope(rope->data.tree.left, start, rope->data.tree.left->length);
            Rope *right = copy_rope(rope->data.tree.right, 0, end - rope->data.tree.left->length);
            return create_rope_from_tree(left, right);
        }
    }
}

int find_rope(Rope *rope, const char *str) {
    if (rope == NULL) {
        return -1;
    }

    if (rope->type == 0) {
        return strstr(rope->data.string, str) ? 0 : -1;
    } else {
        int left_result = find_rope(rope->data.tree.left, str);
        if (left_result != -1) {
            return left_result;
        }
        int right_result = find_rope(rope->data.tree.right, str);
        if (right_result != -1) {
            return rope->data.tree.left->length + right_result;
        }
        return -1;
    }
}

void free_rope(Rope *rope) {
    if (rope == NULL) {
        return;
    }

    if (rope->type == 1) {
        free_rope(rope->data.tree.left);
        free_rope(rope->data.tree.right);
    } else {
        free(rope->data.string);
    }
    free(rope);
}

char *to_string(Rope *rope) {
    if (rope == NULL) {
        return strdup("");
    }

    if (rope->type == 0) {
        return strdup(rope->data.string);
    } else {
        char *left_str = to_string(rope->data.tree.left);
        char *right_str = to_string(rope->data.tree.right);
        char *result = (char *)malloc(strlen(left_str) + strlen(right_str) + 1);
        strcpy(result, left_str);
        strcat(result, right_str);
        free(left_str);
        free(right_str);
        return result;
    }
}

char *rope_to_string(Rope *rope) {
    if (rope == NULL) {
        return strdup("");
    }

    if (rope->type == 0) {
        // 如果是字符串类型
        return strdup(rope->data.string);
    } else {
        // 如果是树类型
        char *left_str = rope_to_string(rope->data.tree.left);
        char *right_str = rope_to_string(rope->data.tree.right);

        char *result = malloc(strlen(left_str) + strlen(right_str) + 1);
        strcpy(result, left_str);
        strcat(result, right_str);

        free(left_str);
        free(right_str);

        return result;
    }
}

void write_rope_to_file(Rope *rope, FILE *file) {
    if (rope == NULL) {
        return;
    }

    if (rope->type == 0) {
        // 如果是字符串类型
        fwrite(rope->data.string, 1, rope->length, file);
    } else {
        // 如果是树类型
        write_rope_to_file(rope->data.tree.left, file);
        write_rope_to_file(rope->data.tree.right, file);
    }
}

void save_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    write_rope_to_file(editor->buffer, file);

    fclose(file);
}


void open_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char *buffer = (char *)malloc(1024 * 1024);  // 1MB buffer
    size_t bytes_read = fread(buffer, 1, 1024 * 1024, file);
    fclose(file);

    if (bytes_read == 0) {
        free(buffer);
        return;
    }

    editor->buffer = create_rope_from_string(buffer);
    free(buffer);
}

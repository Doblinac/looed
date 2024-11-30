#ifndef SEARCH_H
#define SEARCH_H

void handle_search_mode();
int search_in_rope(Rope *rope, const char *search_term, int *y, int *x);

#endif // SEARCH_H
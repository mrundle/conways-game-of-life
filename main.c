#include <assert.h>
#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>

#include "macros.h"

struct square {
    int cur_state;
    int new_state;
};

struct board {
    int ncols;
    int nrows;
    struct square **board;
};

struct board_iterator {
    int x;
    int y;
    struct board *board;
    struct square *square;
};

void msleep(const unsigned ms)
{
    usleep(ms * 1000);
}

/*
 * Run the provided callback function on each board square, providing a square descriptor.
 */
typedef int (*board_iterator_fn)(struct board_iterator *const iter);

int
board_run(struct board *const b, board_iterator_fn iterator_fn)
{
    for (int y = 0; y < b->nrows; y++) {
        for (int x = 0; x < b->ncols; x++) {
            struct square *const s = &b->board[y][x];
            struct board_iterator iter = {
                .x = x,
                .y = y,
                .board = b,
                .square = s,
            };
            iterator_fn(&iter);
        }
    }
}

struct board *
board_alloc(int nrows, int ncols)
{
    struct board *b = calloc(sizeof(*b), 1);
    if (!b) {
        return NULL;
    }
    b->ncols = ncols;
    b->nrows = nrows;
    b->board = calloc(sizeof(*b->board), b->nrows);
    if (!b->board) {
        err("failed alloc");
        return NULL;
    }
    for (int i = 0; i < b->nrows; i++) {
        b->board[i] = calloc(sizeof(*b->board[i]), b->ncols);
        if (!b->board[i]) {
            err("failed alloc");
            return NULL;
        }
    }
    return b;
}

void
board_free(struct board *const b)
{
    if (b) {
        for (int i = 0; i < b->nrows; i++) {
            free(b->board[i]);
        }
        free(b->board);
        free(b);
    }
}

int
board_printw(struct board *const b)
{
    if (!b) {
        err("bad input");
        return -EINVAL;
    }
    for (int y = 0; y < b->nrows; y++) {
        for (int x = 0; x < b->ncols; x++) {
            mvprintw(y, x, b->board[y][x].cur_state ? "X" : ".");
        }
    }
    return 0;
}

int
board_neighbors_alive(struct board *const b, int target_y, int target_x)
{
    if (!b) {
        err("bad input");
        return -EINVAL;
    }

    int n_alive = 0;

    for (int y = max(target_y - 1, 0); y <= min(target_y + 1, b->nrows - 1); y++) {
        for (int x = max(target_x - 1, 0); x <= min(target_x + 1, b->ncols - 1); x++) {
            if (y == target_y && x == target_x) {
                continue;
            }
            assert(y < b->nrows);
            assert(y >= 0);
            assert(x < b->ncols);
            assert(y >= 0);
            n_alive += b->board[y][x].cur_state;
        }
    }

    return n_alive;
}

int
board_compute_next_generation(struct board_iterator *const iter)
{
    struct square *sq = iter->square;
    bool is_alive = sq->cur_state;
    int neighbors_alive = board_neighbors_alive(iter->board, iter->y, iter->x);
    if (is_alive) {
        if (neighbors_alive > 3) {
            sq->new_state = false; /* >3: dies of overpopulation */
        } else if (neighbors_alive > 1) {
            sq->new_state = true; /* 2-3: lives on to next generation */
        } else {
            sq->new_state = false; /* <2: dies of underpopulation */
        }
    } else {
        sq->new_state = neighbors_alive == 3; /* alive via reproduction */
    }
    return 0;
}

int
board_promote_next_generation(struct board_iterator *const iter)
{
    iter->square->cur_state = iter->square->new_state;
}

int
board_randomize_square(struct board_iterator *const iter)
{
    iter->square->cur_state = rand() % 2;
    return 0;
}

int main(void)
{
    /* start curses mode */
    WINDOW *window = initscr();

    int maxx, maxy;
    getmaxyx(window, maxy, maxx);
    maxy--; /* may not be needed in all cases, but pretend */

    /* alloc board */
    struct board *b = board_alloc(maxy, maxx);
    if (!b) {
        return -1;
    }

    /* randomize board */
    board_run(b, board_randomize_square);

    /* run the game */
    while (true) {
        board_printw(b);
        refresh(); /* flush to the screen */
        board_run(b, board_compute_next_generation);
        board_run(b, board_promote_next_generation);
        msleep(500);
        //getch(); /* get user input (TODO) */
    }

    endwin();  /* end curses mode */
    board_free(b);

    return 0;
}

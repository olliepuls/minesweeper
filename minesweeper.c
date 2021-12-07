#include <stdio.h>
#include <stdlib.h>

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void board_init(int* board, int mines, int total_squares, int board_height, int board_width) {
    // Seed random number gen
    srand(0);
    // Populate mines and assign values to other squares
    while (mines > 0) {
        int index = rand() % total_squares;
        if (board[index] != -1) {
            board[index] = -1;
            mines -= 1;
        }
    }

    // Assign number vlaues depending on adjacent mines
    for (int i = 0; i < board_height; i++) {
        for (int j = 0; j < board_width; j ++) {
            // If tile is a mine, continue. Else, determine number of adjacent mines.
            if (board[i*board_width + j] == -1) {
                continue;
            } else {
                int adj_mines = 0;
                // Check surrounding tiles for mines
                for (int dy = max(0, i - 1); dy < min(board_height, i + 2); dy++) {
                    for (int dx = max(0, j - 1); dx < min(board_width, j + 2); dx++) {
                        if (board[dy*board_width + dx] == -1) {
                            adj_mines++;
                        }
                    }
                }
                board[i*board_width+j] = adj_mines;
            }
        }
    }
}

void other_boards_init(char *disp_board, double *prob_board, int total_squares) {
    for (int i = 0; i < total_squares; i++) {
        disp_board[i] = '-';
        prob_board[i] = 10.0;
    }
}

void print_display_board(char *board, int board_height, int board_width) {
    // Print col numbers and board top
    printf("    ");
    for (int i = 1; i < board_width + 1; i++) {
        printf(" %i ", i);
    }
    printf("\n");
    printf("    ");
    for (int i = 0; i < board_width; i++) {
        printf("___");
    }
    printf("\n");

    // Print rows of board
    for (int i = 0; i < board_height; i++) {
        printf(" %i |", i + 1);
        for (int j = 0; j < board_width; j++) {
            printf(" %c ", board[i*board_width + j]);
        }
        printf("|\n");
    }

    printf("   |");
    for (int i = 0; i < board_width; i++) {
        printf("___");
    }
    printf("|\n\n");
}

void print_actual_board(int *board, int board_height, int board_width) {
    // Print col numbers and board top
    printf("    ");
    for (int i = 1; i < board_width + 1; i++) {
        printf(" %i ", i);
    }
    printf("\n");
    printf("    ");
    for (int i = 0; i < board_width; i++) {
        printf("___");
    }
    printf("\n");

    // Print rows of board
    for (int i = 0; i < board_height; i++) {
        printf(" %i |", i + 1);
        for (int j = 0; j < board_width; j++) {
            if (board[i*board_width + j] >= 0) {
                printf(" %i ", board[i*board_width + j]);
            } else {
                printf(" X ");
            }
            
        }
        printf("|\n");
    }

    printf("   |");
    for (int i = 0; i < board_width; i++) {
        printf("___");
    }
    printf("|\n\n");
}



int main(int argc, char *argv[]) {
    // First arg is board width
    int board_width = atoi(argv[1]);
    // Second is height
    int board_height = atoi(argv[2]);
    // Third is difficulty
    int difficulty = atoi(argv[3]);
    
    // Create board data structures
    int total_squares = board_height*board_width;
    int board[total_squares];
    double probability_board[total_squares];
    char display_board[total_squares];

    // Convert Difficulty to number of mines
    int mines;
    if (difficulty < 0 || difficulty > 3) {
        printf("Difficulty must be between 0 (easy) and 3 (very hard)");
        return -1;
    } else {
        switch (difficulty) {
            case 0:
                mines = total_squares*0.1;
                break;
            case 1:
                mines = total_squares*0.25;
                break;
            case 2:
                mines = total_squares*0.5;
                break;
            case 3:
                mines = total_squares*0.7;
                break;
            default: 
                mines = total_squares;
                break;
        }
    }

    board_init(board, mines, total_squares, board_height, board_width);
    other_boards_init(display_board, probability_board, total_squares);
    print_display_board(display_board, board_height, board_width);
    print_actual_board(board, board_height, board_width);

    return 0;
}


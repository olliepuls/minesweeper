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

// Move data structure
struct Move {
    // Row and column of target tile
    int row;
    int col;
    // Type of move -> 0 means reveal, 1 means flag
    int type;
};

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

void take_move_input(struct Move *move) {
    int r, c, t;
    scanf("%i %i %i", &r, &c, &t);
    move->row = r-1;
    move->col = c-1;
    move ->type = t;
}

void calculate_move(struct Move *move, double *probability_board) {
    
}

int apply_move(struct Move *move, int *board, char *disp_board, int board_height, int board_width, int *flags_remaining) {
    // Check move is valid
    if (move->row < 0 || move->col < 0 || move->row > board_height-1 || move->col > board_width-1 || move->type > 2 || move->type < 0) {
        printf("Move is invalid. Please use only valid row and column indices and a 0, 1 or 2 to indicate reveals and flags adds/removes.\n");
    }

    int row = move->row;
    int col = move->col;

    int tile = board[row*board_width+col];

    // If revealing a tile
    if (move->type == 0 && disp_board[row*board_width+col] == '-') {
        // Check what tile is
        if (tile == -1) {
            return -1;
        } else if (tile == 0) {
            disp_board[row*board_width+col] = ' ';

            // Reveal all adjacent tiles.
            for (int i = max(0, row - 1); i < min(board_height, row + 2); i++) {
                for (int j = max(0, col - 1); j < min(board_width, col + 2); j++) {
                    // printf("%i %i\n", i, j);
                    if (!(i == row && j == col)) {
                        struct Move rev_tile;
                        rev_tile.row = i;
                        rev_tile.col = j;
                        rev_tile.type = 0;
                        apply_move(&rev_tile, board, disp_board, board_height, board_width, flags_remaining);
                    }
                }
            }
        } else {
            // Reveal Tile
            disp_board[row*board_width+col] = tile + '0';
        }
    } else if (move->type == 1 && disp_board[row*board_width+col] == '-'){
        // Flag the tile
        disp_board[row*board_width+col] = 'P';
        *flags_remaining -= 1;
    } else if (move->type == 2 && disp_board[row*board_width+col] == 'P'){
        // Flag the tile
        disp_board[row*board_width+col] = '-';
        *flags_remaining += 1;
    }

    // Check completion status
    int complete = 1;

    for (int i = 0; i < board_height*board_width; i++) {
        complete = complete && (disp_board[i] != '-');
    }

    complete = complete && *flags_remaining == 0;

    return complete;
}


int main(int argc, char *argv[]) {
    // First arg is board width
    int board_width = atoi(argv[1]);
    // Second is height
    int board_height = atoi(argv[2]);
    // Third is difficulty
    int difficulty = atoi(argv[3]);
    // Fourth is User play mode
    int play_mode = atoi(argv[4]);

    // Game Status Variable. 0 = In progress, -1 = Lost, 1 = Won.
    int game_status = 0;

    // Move Data structure
    struct Move curr_move;
    
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
                mines = total_squares*0.3;
                break;
            case 2:
                mines = total_squares*0.6;
                break;
            case 3:
                mines = total_squares*0.8;
                break;
            default: 
                mines = total_squares;
                break;
        }
    }

    board_init(board, mines, total_squares, board_height, board_width);
    other_boards_init(display_board, probability_board, total_squares);
    print_display_board(display_board, board_height, board_width);
    // print_actual_board(board, board_height, board_width);
    
    while (game_status == 0) {
        if (play_mode == 0) {
            take_move_input(&curr_move);
        } else {
            calculate_move(&curr_move, probability_board);
        }
        
        game_status = apply_move(&curr_move, board, display_board, board_height, board_width, &mines);
        print_display_board(display_board, board_height, board_width);
        // print_actual_board(board, board_height, board_width);
    }

    // Completion Message
    if (game_status == 1) {
        printf("\n\nCONGRATULATIONS YOU JUST WON!!\n\n");
    } else {
        printf("\n\nOUCH! YOU JUST STEPPED ON A MINE! :'(\n\n");
    }

    print_actual_board(board, board_height, board_width);

    return 0;
}


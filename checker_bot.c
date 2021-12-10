/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [Tony Yong He, 1269440]
  Dated:     [01/10/2021]

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include <math.h>
#include <ctype.h>
/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

#define ROW_DELIM "+---+---+---+---+---+---+---+---+"
#define COL_DELIM '|'
#define ACTION_DELIM "====================================="
#define NUM_BLACK 12    /* number of black pieces */
#define NUM_WHITE 12    /* number of white pieces */
#define ASCII_A 65
#define WHITE_ROW1 1    /* rows that contains white pieces in inital setup */
#define WHITE_ROW2 2
#define WHITE_ROW3 3
#define BLACK_ROW1 6    /* rows that contains black pieces in inital setup */
#define BLACK_ROW2 7
#define BLACK_ROW3 8

#define ERROR1 -1
#define ERROR2 -2
#define ERROR3 -3
#define ERROR4 -4
#define ERROR5 -5
#define ERROR6 -6

#define FST_COL 'A'     /* first column */
#define LST_COL 'H'     /* last column */
#define FST_ROW 1       /* first row */
#define LST_ROW 8       /* last row */

#define MOV_JUMP 1      /* row_change = col_change = 1 */
#define CAP_JUMP 2      /* row_change = col_change = 2 */
#define NO_JUMP 0       /* row_change = col_change = 0 */

#define ROOT_DEPTH 0        /* depth of root node */
#define NEXT_ACT_DEPTH 1    /* depth of node containing next computed action */
#define STAGE_0 0           /* no computed actions */
#define STAGE_1_2 1         /* have computed action(s) */

/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type
typedef struct node node_t;

struct node {
    char s_col;
    int s_row;
    char t_col;
    int t_row;
    int cost; /* propagated cost!*/
    board_t board;
    node_t *hchild;
    node_t *fchild;
    node_t *next;
};

void initial_setup(board_t);
void print_board(board_t);
int no_error(int, char, int, char, int, board_t); 
void print_error(int);
void apply_action(char, int, char, int, board_t);
char side(int);
int error5(int, int, int, int, int, board_t);
int error6(int, int, int, int, int, board_t);
void print_info(int, int, int, int, int, board_t, int);
int board_cost(board_t);

void minimax_tree(int, board_t, node_t*, int);
void all_direction(int, int, int, board_t, node_t*, int);
void copy_board(board_t, board_t);
node_t* new_root(board_t);
node_t* new_node(char, int, char, int, board_t);
void possible_action(int, int, int, board_t, node_t*, int, int, int);
void insert_node(node_t*, node_t*);
int minimax(node_t*, int, int);
int next_action(node_t*, int, board_t);
void recursive_free_tree(node_t*);
void free_tree(node_t*);

int
main(int argc, char *argv[]) {
    // YOUR IMPLEMENTATION OF STAGES 0-2
    int input, num_action=0, s_row, t_row, noerror, count, proceed=1;
    char s_col, t_col, command;
    board_t board;
    node_t* root;

    initial_setup(board);
    print_board(board);
    
    /* stage 0: read, analyse and print input data */
    while ((input=scanf("%c%d-%c%d ", &s_col, &s_row, &t_col, &t_row))==4) {
        noerror = no_error(num_action, s_col, s_row, t_col, t_row, board);
        if (noerror==1) {
            apply_action(s_col, s_row, t_col, t_row, board);
            print_info(num_action, s_col, s_row, t_col, t_row, board, STAGE_0);
            print_board(board);
        } else {
            print_error(noerror);
            return EXIT_FAILURE;
        }
        num_action++;
    }
    /* stage 1: compute the next action */
    if (input==1) {
        command = s_col;
        if (command=='A') {
            root = new_root(board);
            minimax_tree(num_action, board, root, NEXT_ACT_DEPTH);
            next_action(root, num_action, board);
        } else {
            /* stage 2: compute ten next actions or all actions until the end 
            of the game, whatever comes first */
            for (count=0; count<COMP_ACTIONS && proceed==1; count++) {
                root = new_root(board);
                minimax_tree(num_action, board, root, NEXT_ACT_DEPTH);
                proceed = next_action(root, num_action, board);
                num_action++;
            }
        }
    }
    /* Algorithms are fun! */
    return EXIT_SUCCESS;            // exit program with the success code
}
  
/* initialise the board */
void initial_setup(board_t board) {
    int row, col;

    /* print information about the board */
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", NUM_BLACK);
    printf("#WHITE PIECES: %d\n", NUM_WHITE);

    for (row=0; row<BOARD_SIZE; row++) {
        for (col=0; col<BOARD_SIZE; col++) {
            if ((row+1==WHITE_ROW1||row+1==WHITE_ROW3) && col%2!=0) {
                board[row][col] = CELL_WPIECE;
            } else if (row+1==WHITE_ROW2 && col%2==0) {
                board[row][col] = CELL_WPIECE;
            } else if ((row+1==BLACK_ROW1||row+1==BLACK_ROW3) && col%2==0) {
                board[row][col] = CELL_BPIECE;
            } else if (row+1==BLACK_ROW2 && col%2!=0) {
                board[row][col] = CELL_BPIECE;
            } else {
                board[row][col] = CELL_EMPTY;
            }
        }
    }
}

/* prints input board */
void print_board(board_t board) {
    int row, col;

    /* print column names */
    printf("  ");
    for (col=0; col<BOARD_SIZE; col++) {
        printf("%4c", ASCII_A+col);
    }
    printf("\n");

    /* print body of the board */
    for (row=0; row<BOARD_SIZE; row++) {
        printf("   %s\n", ROW_DELIM);
        for (col=0; col<BOARD_SIZE; col++) {
            if (col==0) {
                printf("%2d %c", row+1, COL_DELIM);   /* row numbers */
            }
            printf("%2c%2c", board[row][col], COL_DELIM);
        }
        printf("\n");
    }
    printf("   %s\n", ROW_DELIM);
}

/* returns the corresponding error number if error is detected, returns 1 
otherwise */
int no_error(int num_action, char s_col, int s_row, char t_col, int t_row, 
board_t board) {
    int scol=s_col-ASCII_A, tcol=t_col-ASCII_A, srow=s_row-1, trow=t_row-1;

    if ((s_col<FST_COL || s_col>LST_COL) || (s_row<FST_ROW || s_row>LST_ROW)) {
        return ERROR1;
    } else if ((t_col<FST_COL || t_col>LST_COL) || 
    (t_col<FST_ROW || t_row>LST_ROW)) {
        return ERROR2;
    } else if (board[srow][scol]==CELL_EMPTY) {
        return ERROR3;
    } else if (board[trow][tcol]!=CELL_EMPTY) {
        return ERROR4;
    } else if (error5(num_action, scol, srow, tcol, trow, board)==1) {
        return ERROR5;
    } else if (error6(num_action, scol, srow, tcol, trow, board)==1) {
        return ERROR6;
    }
    return 1;
}

/* print the error message */
void print_error(int error_num) {
    if (error_num==ERROR1) {
        printf("ERROR: Source cell is outside of the board.\n");
    } else if (error_num==ERROR2) {
        printf("ERROR: Target cell is outside of the board.\n");
    } else if (error_num==ERROR3) {
        printf("ERROR: Source cell is empty.\n");
    } else if (error_num==ERROR4) {
        printf("ERROR: Target cell is not empty.\n");
    } else if (error_num==ERROR5) {
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
    } else if (error_num==ERROR6) {
        printf("ERROR: Illegal action.\n");
    }
}

/* make the move/capture and update the board */
void apply_action(char s_col, int s_row, char t_col, int t_row, board_t board) {
    int scol=s_col-ASCII_A, tcol=t_col-ASCII_A, row_cap, col_cap;
    int srow=s_row-1, trow=t_row-1;

    board[trow][tcol] = board[srow][scol];
    board[srow][scol] = CELL_EMPTY;

    /* remove captured piece/tower if necessary */
    if (abs(tcol-scol)==CAP_JUMP) {
        row_cap = (srow+trow)/2;    /* row of captured cell */
        col_cap = (scol+tcol)/2;    /* column of captured cell */
        board[row_cap][col_cap] = CELL_EMPTY;
    }
    /* transform to tower if necessary */
    if (board[trow][tcol]==CELL_BPIECE && t_row==FST_ROW) {
        board[trow][tcol] = CELL_BTOWER;
    } else if (board[trow][tcol]==CELL_WPIECE && t_row==LST_ROW) {
        board[trow][tcol] = CELL_WTOWER;
    }
}

/* returns 1 if source cell holds opponent’s piece/tower, returns 0 otherwise */
int error5(int num_action, int scol, int srow, int tcol, int trow, 
board_t board) {
    char turn = side(num_action);

    /* determine if source cell = opponent’s piece/tower */
    if (turn==CELL_BPIECE && (board[srow][scol]==CELL_WPIECE || 
    board[srow][scol]==CELL_WTOWER)) {
        return 1;
    } else if (turn==CELL_WPIECE && (board[srow][scol]==CELL_BPIECE || 
    board[srow][scol]==CELL_BTOWER)) {
        return 1;
    }
    return 0;
}

/* returns the side (b/w) currently taking an action */
char side(int num_action) {
    if (num_action%2==0) {
        return CELL_BPIECE;
    } else {
        return CELL_WPIECE;
    }
}

/* returns 1 if action is illegal, returns 0 otherwise */
int error6(int num_action, int scol, int srow, int tcol, int trow, 
board_t board) {
    char cell=board[srow][scol], cap_cell;
    int col_cap, row_cap;

    /* check if movement is diagonal and change is within the range [1,2] */
    if (abs(tcol-scol)!=abs(trow-srow) || 
    (abs(tcol-scol)>CAP_JUMP) || abs(tcol-scol)==NO_JUMP) {
        return 1;
    }

    /* check if pieces are moving in the right direction */
    if (cell==CELL_BPIECE && srow-trow<NO_JUMP) {
        return 1;
    } else if (cell==CELL_WPIECE && trow-srow<NO_JUMP) {
        return 1;
    }

    /* check if capture is valid */
    if (abs(tcol-scol)==CAP_JUMP) {
        row_cap = (srow+trow)/2;   
        col_cap = (scol+tcol)/2;
        cap_cell = board[row_cap][col_cap];

        /* check if own piece/tower is captured or captured cell is empty */
        if ((cell==cap_cell || toupper(cell)==cap_cell) || 
        cap_cell==CELL_EMPTY) {
            return 1;
        }
    } 
    return 0;
}

/* prints info about the state of the board */
void print_info(int num_action, int s_col, int s_row, int t_col, int t_row, 
board_t board, int compute_action) {

    printf("%s\n", ACTION_DELIM);
    /* check if action is computed */
    if (compute_action==1) {
        printf("*** ");
    }
    /* print colour, action number, board cost */
    if (num_action%2==0) {
        printf("BLACK ");
    } else {
        printf("WHITE ");
    }
    printf("ACTION #%d: %c%d-%c%d\n", num_action+1, s_col, s_row, t_col, t_row);
    printf("BOARD COST: %d\n", board_cost(board));
}

/* returns the board cost */
int board_cost(board_t board) {
    int row, col, b=0, B=0, w=0, W=0;
    char cell;

    for (row=0; row<BOARD_SIZE; row++) {
        for (col=0; col<BOARD_SIZE; col++) {
            cell = board[row][col];
            if (cell==CELL_BPIECE) {
                b++;
            } else if (cell==CELL_BTOWER) {
                B++;
            } else if (cell==CELL_WPIECE) {
                w++;
            } else if (cell==CELL_WTOWER) {
                W++;
            }
        }
    }
    return b+COST_TOWER*B-w-COST_TOWER*W;
}

/* constructs a minimax tree */
void minimax_tree(int num_action, board_t board, node_t* pnode, int depth) {
    int row, col;
    char cell, turn;

    turn = side(num_action);
    /* scan all cells that can potentially take an action in this turn */
    if (depth<=TREE_DEPTH) { 
        for (row=0; row<BOARD_SIZE; row++) {
            for (col=0; col<BOARD_SIZE; col++) {
                cell = board[row][col];
                if (cell==turn || cell==toupper(turn)) {
                    all_direction(num_action, row, col, board, pnode, depth);
                }
            }  
        }
    }
}

/* explores all possible actions in a clockwise fashion, starting from 
north-east direction */
void all_direction(int num_action, int row, int col, board_t board, 
node_t* pnode, int depth) {
    int x, y;

    /* north east */
    x = y = MOV_JUMP;
    possible_action(num_action, row, col, board, pnode, depth, x, y);

    /* south east */
    x = MOV_JUMP;
    y = -MOV_JUMP;
    possible_action(num_action, row, col, board, pnode, depth, x, y);

    /* south west */
    x = y = -MOV_JUMP;
    possible_action(num_action, row, col, board, pnode, depth, x, y);

    /* north west */
    x = -MOV_JUMP;
    y = MOV_JUMP;
    possible_action(num_action, row, col, board, pnode, depth, x, y);
}

/* find all possible actions in the indicated direction and insert into the 
tree */
void possible_action(int num_action, int row, int col, board_t board, 
node_t* pnode, int depth, int x, int y) {
    int col_change, row_change, new_row, new_col, i;
    node_t* new;

    /* action is either a move or a capture */
    for (i=MOV_JUMP; i<=CAP_JUMP; i++) {
        col_change = x;
        row_change = y;
        col_change *= i;
        row_change *= i;
        new_col = col + col_change;
        new_row = row + row_change;
        if (no_error(num_action, col+ASCII_A, row+1, new_col+ASCII_A, 
        new_row+1, board)==1) {

            /* insert into the tree if there's no error */
            new = new_node(col+ASCII_A, row+1, new_col+ASCII_A, new_row+1, 
            board);
            insert_node(pnode, new);
            minimax_tree(num_action+1, new->board, new, depth+1);
        }
    }
}

/* content of the original board is replicated in the copy */
void copy_board(board_t original, board_t copy) {
    int row, col;
    for (row=0; row<BOARD_SIZE; row++) {
        for (col=0; col<BOARD_SIZE; col++) {
            copy[row][col] = original[row][col];
        }
    }
}

/* create a new root node and returns the pointer to that root */
node_t* new_root(board_t board) {
    node_t* root;

    root = (node_t*)malloc(sizeof(*root));
    assert(root!=NULL);
    copy_board(board, root->board);
    root -> hchild = NULL;
    root -> fchild = NULL;
    root -> next = NULL;
    return root;
}

/* returns the pointer to the new node */
node_t* new_node(char s_col, int s_row, char t_col, int t_row, board_t board) {
    node_t* new;
    
    new = (node_t*)malloc(sizeof(*new));
    assert(new!=NULL);
    new->s_col = s_col;
    new->s_row = s_row;
    new->t_col = t_col;
    new->t_row = t_row;
    copy_board(board, new->board);
    apply_action(s_col, s_row, t_col, t_row, new->board);
    new->hchild = NULL;
    new->fchild = NULL;
    new->next = NULL;

    return new;
}

/* insert into tree */
void insert_node(node_t* pnode, node_t* new) {
    assert(pnode!=NULL && new!=NULL);
    if (pnode->hchild==NULL && pnode->fchild==NULL) {
        pnode->hchild = new;
        pnode->fchild = new;
    } else {
        pnode->fchild->next = new;
        pnode->fchild = new;
    }
}

/* apply minimax algorithm on the tree */
int minimax(node_t* node, int depth, int num_action) {
    char turn=side(num_action);
    int cost, maxcost, mincost, leafcost;
    node_t* child=node->hchild;
    
    /* compute board cost for leaf nodes */ 
    if (depth==TREE_DEPTH) {
        leafcost = board_cost(node->board);
        return leafcost;
    } else if (child==NULL) {
        if (turn==CELL_BPIECE) {
            return INT_MIN;
        } else {
            return INT_MAX;
        }
    }
    /* calulate and store propagated cost */
    if (turn == CELL_BPIECE) {
        maxcost = INT_MIN;
        while (child) {
            cost = minimax(child, depth+1, num_action+1);
            if (cost > maxcost) {
                maxcost = cost;
            } 
            child = child->next;
        }
        node -> cost = maxcost;
        return maxcost;
    } else {
        mincost = INT_MAX;
        while (child) {
            cost = minimax(child, depth+1, num_action+1);
            if (cost < mincost) {
                mincost = cost;
            } 
            child = child -> next;
        }
        node -> cost = mincost;
        return mincost;
    }
}

/* compute the next action based on current board, returns 1 if an action was 
taken, and 0 otherwise */
int next_action(node_t* root, int num_action, board_t board) {
    node_t* child=root->hchild;
    int taken_action=0;
    char turn=side(num_action);
    
    assert(root!=NULL);
    minimax(root, ROOT_DEPTH, num_action);
    /* determine the first depth 1 node with the same propagated cost as root */
    while (child) {
        if (child->cost==root->cost) {
            apply_action(child->s_col, child->s_row, child->t_col, child->t_row, 
            board);
            print_info(num_action, child->s_col, child->s_row, child->t_col, 
            child->t_row, child->board, STAGE_1_2);
            print_board(child->board);
            taken_action=1;
            break;
        }
        child = child->next;
    }
    /* if no action was taken, then either black or white must have won */
    if (taken_action==0) {
        if (turn==CELL_BPIECE) {
            printf("WHITE WIN!\n");
        } else {
            printf("BLACK WIN!\n");
        }
    }
    free_tree(root);
    return taken_action;
}

/* free all child nodes */
void recursive_free_tree(node_t* root) {
    node_t* child=root->hchild, *next_node;
    while (child) {       
        recursive_free_tree(child); 
        next_node = child->next;
        free(child);
        child = next_node;
    }
}

/* release all memory space associated with the tree structure */
void free_tree(node_t* root) {
    assert(root!=NULL);
    recursive_free_tree(root);
    free(root);
    root = NULL;
}

/* THE END -------------------------------------------------------------------*/

#include <gtk/gtk.h>
#include <string.h>
#include "game_piece.h"
#include "board.h"

void btn_clicked(GtkWidget *widget, GtkEntry *entry);
void myCSS(void);
void createWind(GtkWidget **window, gint width, gint height);
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name);
void createLabel(GtkWidget **label, GtkWidget **window, const gchar *text);
void get_position(GtkWidget *button, gpointer data);
void updateLabel(GtkWidget **label, const gchar *text);
void Update_GUI();
void move_piece(int, int, int, int);
void initialize_board();
void king_piece(int, int);
int check_win();
void print_board();
void click_type();
void change_turn();
void remove_piece(int, int);

typedef enum boolean {
	true = 1,
	false = 0
} boolean;

boolean valid_move(int, int, int, int);

Color turn = white;
GtkWidget *board_buttons[8][8];
GtkWidget *label;

char turn_label[20];
int clicks[2][2];
int firstclick=0;
Board b;
Board * board =&b;

//!!!!MAIN!!!
int main(int argc, char *argv[]){
	firstclick = 0;


	GtkWidget *window,*label_window, *grid;
	GtkWidget *image;

	gtk_init(&argc, &argv);
	myCSS();
	initialize_board(board);
//Creates the window for the board
	createWind(&window, 600, 600);

	createWind(&label_window, 200, 200);
//Creates the grid for the board
	createGrid(&grid, &window, "myGrid");

	strcpy(turn_label, "White's turn");
	createLabel(&label, &label_window, turn_label);
//Creates an array of buttons to attach to the grid
	for(int i = 0; i<8; i++){
		for(int j = 0; j<8; j++){

			board_buttons[i][j]= gtk_button_new();

        		gtk_widget_set_size_request(board_buttons[i][j], 80, 80);
        		g_object_set (board_buttons[i][j], "margin", 1, NULL);

			//Controls when a button on the board is clicked
			g_signal_connect(board_buttons[i][j], "clicked", G_CALLBACK(get_position), NULL);


			//Generates "checkered" board
			if(i%2 == j%2){
				gtk_widget_set_name(board_buttons[i][j], "button_red");
					if(i<3){
						image = gtk_image_new_from_file("white_piece.png");
	                			gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), image);

					}//end blank rows if
					else if(i>4){
						image = gtk_image_new_from_file("purple_piece.png");
						gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), image);
					}//end else
			}//end piece placement if

			else{
				gtk_widget_set_name(board_buttons[i][j], "button_black");
			}//end else

			gtk_grid_attach(GTK_GRID(grid), board_buttons[i][j], j, i, 1, 1);
		}//end j loop
	}//end i loop

	gtk_widget_show_all(window);
	gtk_widget_show_all(label_window);
	gtk_main();
	return 0;

}



//!!!!!IMPORTANT!!!!!!!
// May need to free space for whatever calls get poisition
// This method gets the position in the array of the button clicked
void get_position(GtkWidget *widget, gpointer data){
//printf("in get position");


	for(int i = 0; i<8; i++){
		for(int j = 0; j<8; j++){
			if(widget == board_buttons[i][j]){
				if(firstclick==0){
					clicks[0][0] = i;
//					printf("%d", i);
					clicks[0][1] = j;
//					printf("%d\n", j);
					firstclick++;
				}//end if
				else{
					clicks[1][0] = i;
//					printf("%d", i);
					clicks[1][1] = j;
//					printf("%d\n", j);
					firstclick=0;

					if(valid_move(clicks[0][0], clicks[0][1], clicks[1][0], clicks[1][1]) == true){
						move_piece(clicks[0][0], clicks[0][1], clicks[1][0], clicks[1][1]);
						king_piece(clicks[1][0], clicks[1][1]);
						change_turn();
						if(turn == purple) {
							strcpy(turn_label, "Purple's turn");
							updateLabel(&label, turn_label);
						}
						else {
							strcpy(turn_label, "White's turn");
							updateLabel(&label, turn_label);
						}
						check_win();
						Update_GUI();
					}

					else{g_print("Invalid Move\n");}


				}//end else
			}//end widget comparison
		}//end j loop
	}//end i loop
}

//This sets up the CSS file that controls the style of the board
void myCSS(void){
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;

	provider = gtk_css_provider_new ();
	display = gdk_display_get_default ();
	screen = gdk_display_get_default_screen (display);
	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	const gchar *myCssFile = "mystyle.css";
	GError *error = 0;

	gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
	g_object_unref (provider);
}


//This creates the window for the board
void createWind(GtkWidget **window, gint width, gint height){
	*window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(*window), "Checkers");
	gtk_window_set_default_size(GTK_WINDOW(*window), width, height);
	gtk_window_set_resizable (GTK_WINDOW(*window), TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(*window), 5);
	g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

//This creates the grid that holds each button
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name){
	*grid = gtk_grid_new ();
	gtk_grid_set_row_homogeneous(GTK_GRID(*grid), FALSE);
    	gtk_grid_set_column_homogeneous(GTK_GRID(*grid), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER (*grid), 15);
    	gtk_grid_set_column_spacing(GTK_GRID(*grid), 5);
    	gtk_grid_set_row_spacing(GTK_GRID(*grid), 5);
    	gtk_widget_set_name(*grid, name);
    	g_object_set (*grid, "margin", 22, NULL);
    	gtk_container_add (GTK_CONTAINER (*window), *grid);
}

void createLabel(GtkWidget **label, GtkWidget **window, const gchar *text){
	*label = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(*label), text);
	gtk_container_add(GTK_CONTAINER (*window), *label);
}

void updateLabel(GtkWidget **label, const gchar *text){

	gtk_label_set_text(GTK_LABEL(*label), text);
}

//This controls a button click
void btn_clicked(GtkWidget *widget, GtkEntry *entry){
    	(void)widget;
    	const gchar *gstrTexto;

    	gstrTexto = gtk_entry_get_text(entry);
    	g_print("%s\n", gstrTexto);
    	gtk_editable_select_region(GTK_EDITABLE(entry) , 0, 3);
}

//This method updates the board after each move
void Update_GUI() {

	GtkWidget *piece_img;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board->board_array[i][j] == NULL) {
				gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), NULL);
			}
			else {
				  if ((board->board_array[i][j])->color == 1) {
					if ((board->board_array[i][j])->type == 0) {
						piece_img = gtk_image_new_from_file("white_piece.png");
						gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
					}
					else if ((board->board_array[i][j])->type == 1) {
						piece_img = gtk_image_new_from_file("white_king.png");
                                                gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
					}

				}
				else if (board->board_array[i][j]->color == purple) {
                                        if (board->board_array[i][j]->type == pawn) {
                                                piece_img = gtk_image_new_from_file("purple_piece.png");
                                                gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
                                        }
                                        else if (board->board_array[i][j]->type == king) {
                                                piece_img = gtk_image_new_from_file("purple_king.png");
                                                gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
                                        }

                                }

			}

		}

	}

}


/*  Board Functions */
// free pieces when they are removed?
void initialize_board() {
        for(int i = 0; i<8; i++){
                for(int j=0; j<8; j++){
                        if(i%2 == j%2 && i<3){
                                GamePiece *piece = malloc(sizeof(GamePiece));
				piece->color = white;
                                piece->type = pawn;

                                board->board_array[i][j] = piece;
			}
                        else if(i%2 == j%2 && i>4){
                                GamePiece *piece = malloc(sizeof(GamePiece));
                                piece->type = pawn;
                                piece->color = purple;

                                board->board_array[i][j] = piece;
                        }
                }
        }
}


//This method controls the movements of a piece
void move_piece(int start_row, int start_col, int end_row, int end_col){
//printf("in move piece");

        board->board_array[end_row][end_col] = board->board_array[start_row][start_col];
        board->board_array[start_row][start_col] = NULL;

}

//
//Thie method computes if the users move is valid. If so, the method returns 0, if not, the method
//returns 1.
//
boolean valid_move(int start_row, int start_col, int end_row, int end_col){
	int diff_row = end_row - start_row;
	int diff_col = end_col - start_col;

	if (board->board_array[start_row][start_col] == NULL) { //check if there is a piece at the starting location
		return false;
	}
	if (board->board_array[start_row][start_col]->color != turn) { //check if the piece at the starting location is the correct color for that turn
		return false;
	}
	if (end_row%2 != end_col%2) { //check if the ending location is a playable square
		return false;
	}
	if (board->board_array[end_row][end_col] != NULL) { //check if the ending location is occupied
		return false;
	}
	if (board->board_array[start_row][start_col]->type == king) {
		if (diff_row > 2 || diff_row < -2 || diff_col > 2 || diff_col < -2 || diff_col == 0 || diff_row == 0) {
			return false;
		}
		if ((diff_row == 2 || diff_row == -2) && (diff_col == 2 || diff_col == -2)) {
			if (board->board_array[start_row + diff_row/2][start_col + diff_col/2] == NULL || board->board_array[start_row + diff_row/2][start_col + diff_col/2]->color == board->board_array[start_row][start_col]->color) {
				return false;
			}
			else{
				remove_piece(start_row+diff_row/2, start_col+diff_col/2);
			}
		}
	}
	else {
		if (board->board_array[start_row][start_col]->color == white) {
			if (diff_row<0) {
				return false;
			}
			if (diff_row > 2 ||  diff_col > 2 || diff_col < -2 || diff_col == 0 || diff_row == 0) {
                        	return false;
	                }
        	        if (diff_row == 2 && (diff_col == 2 || diff_col == -2)) {
                	        if (board->board_array[start_row + diff_row/2][start_col + diff_col/2] == NULL || board->board_array[start_row + diff_row/2][start_col + diff_col/2]->color == board->board_array[start_row][start_col]->color) {
                        	        return false;
                       		}
				else{
					remove_piece(start_row+diff_row/2, start_col+diff_col/2);
				}
               		}

		}
		else {
			if (diff_row>0) {
                                return false;
                        }
                        if (diff_row < -2 ||  diff_col > 2 || diff_col < -2 || diff_col == 0 || diff_row == 0) {
                                return false;
                        }
                        if (diff_row == -2 && (diff_col == 2 || diff_col == -2)) {
                                if (board->board_array[start_row + diff_row/2][start_col + diff_col/2] == NULL || board->board_array[start_row + diff_row/2][start_col + diff_col/2]->color == board->board_array[start_row][start_col]->color) {
                                        return false;
                                }
				else{
					remove_piece(start_row+diff_row/2, start_col+diff_col/2);
				}
                        }

		}
	}
	return true;
}


void change_turn(){
	if(turn == white)
		turn = purple;
	else if(turn == purple)
		turn = white;
}

//
//This method kings a piece if it has reached the opposite end of the board
//
void king_piece(int end_row, int end_col){
	if(board->board_array[end_row][end_col]->color == white && end_row==7){
		board->board_array[end_row][end_col]->type = king;}

	else if(board->board_array[end_row][end_col]->color == purple && end_row == 0){
		board->board_array[end_row][end_col]->type = king;}

}

void remove_piece(int x, int y){
	free(board->board_array[x][y]);
	board->board_array[x][y] = NULL;
}

//
//This method checks to see if someone has won the game
//
int check_win(){
	int number_white=0;
	int number_purple=0;
	for(int i =0; i<8; i++){
		for(int j = 0; j<8; j++){
			if(board->board_array[i][j] != NULL){
				if(board->board_array[i][j]->color == white){number_white++;}
				else if(board->board_array[i][j]->color == purple){number_purple++;}
			}
		}
	}

	if(number_purple == 0){
		g_print("White wins!\n");
		strcpy(turn_label, "White wins!!");
		updateLabel(&label, turn_label);
		}
	else if(number_white == 0){
		g_print("Purple wins!\n");
		strcpy(turn_label, "Purple Wins!!");
		updateLabel(&label, turn_label);
	}
}


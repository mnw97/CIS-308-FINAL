
#include <gtk/gtk.h>
#include <string.h>
#include "game_piece.h"
#include "board.h"

void btn_clicked(GtkWidget *widget, GtkEntry *entry);
void myCSS(void);
void createWind(GtkWidget **window, gint width, gint height);
void createGrid(GtkWidget **grid, GtkWidget **window, const gchar *name);
void get_position(GtkWidget *button, gpointer data);
void Update_GUI();
void move_piece(int, int, int, int);
void initialize_board();
int valid_move();
void king_piece();
int check_win();
void print_board();
void click_type();



int firstclick;
int clicks[2][2];
GtkWidget *board_buttons[8][8];

Board b;
Board * board =&b;

//!!!!MAIN!!!
int main(int argc, char *argv[]){
	firstclick = 0;


	GtkWidget *window, *grid;

	GtkWidget *image;

	gtk_init(&argc, &argv);
	myCSS();
	initialize_board(board);
//Creates the window for the board
	createWind(&window, 600, 600);

//Creates the grid for the board
	createGrid(&grid, &window, "myGrid");


//Creates an array of buttons to attach to the grid
	for(int i = 0; i<8; i++){
		for(int j = 0; j<8; j++){

			board_buttons[i][j]= gtk_button_new();

        		gtk_widget_set_size_request(board_buttons[i][j], 100, 100);
        		g_object_set (board_buttons[i][j], "margin", 1, NULL);

			//Controls when a button on the board is clicked
			g_signal_connect(board_buttons[i][j], "clicked", G_CALLBACK(get_position), NULL);


			//Generates "checkered" board
			if(i%2 == j%2){
				gtk_widget_set_name(board_buttons[i][j], "button_red");
					if(i<3 || i>4){
						image = gtk_image_new_from_file("circle-64.png");
	                			gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), image);

					}//end blank rows if
			}//end piece placement if

			else{
				gtk_widget_set_name(board_buttons[i][j], "button_black");
			}//end else

			gtk_grid_attach(GTK_GRID(grid), board_buttons[i][j], j, i, 1, 1);
		}//end j loop
	}//end i loop

	gtk_widget_show_all(window);
	gtk_main();
	return 0;

}



//!!!!!IMPORTANT!!!!!!!
// May need to free space for whatever calls get poisition

void get_position(GtkWidget *widget, gpointer data){
/* 
if (board->board_array[0][0] == NULL) {
                printf("It's Null");
        }
else {
        printf("%d\n", board->board_array[0][0]->color);
        printf("%d\n", board->board_array[0][0]->type);
}
*/
	for(int i = 0; i<8; i++){
		for(int j = 0; j<8; j++){
			if(widget == board_buttons[i][j]){
				if(firstclick==0){
					clicks[0][0] = i;
					printf("%d", i);
					clicks[0][1] = j;
					printf("%d\n", j);
					firstclick++;
				}//end if
				else{
					clicks[1][0] = i;
					printf("%d", i);
					clicks[1][1] = j;
					printf("%d\n", j);
					firstclick=0;
					move_piece(clicks[0][0], clicks[0][1], clicks[1][0], clicks[1][1]);
				}//end else
			}//end widget comparison
		}//end j loop
	}//end i loop
}

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

void createWind(GtkWidget **window, gint width, gint height){
	*window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(*window), "Checkers");
	gtk_window_set_default_size(GTK_WINDOW(*window), width, height);
	gtk_window_set_resizable (GTK_WINDOW(*window), TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(*window), 5);
	g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

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

void btn_clicked(GtkWidget *widget, GtkEntry *entry){
    	(void)widget;
    	const gchar *gstrTexto;

    	gstrTexto = gtk_entry_get_text(entry);
    	g_print("%s\n", gstrTexto);
    	gtk_editable_select_region(GTK_EDITABLE(entry) , 0, 3);
}

void Update_GUI() {
/*	if (board->board_array[0][0] == NULL) {
		printf("It's Null");
	}
else {
	printf("%d\n", board->board_array[0][0]->color);
        printf("%d\n", board->board_array[0][0]->type);
}*/
	//printf("%d", board->board_array[0][0]->color);
	//printf("%d", board->board_array[0][0]->type);
/*	printf("I'm in update");
	GtkWidget *piece_img;
	printf("I'm starting the loops");
	for (int i = 0; i < 8; i++) {
		printf( "In i for ");
		for (int j = 0; j < 8; j++) {
			printf( "In j for ");
			if (board->board_array[i][j] == NULL) {
				printf("In second if ");
				gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), NULL);
			}
			else {
				  if ((board->board_array[i][j])->color == 1) {
					if ((board->board_array[i][j])->type == 0) {
					//	piece_img = gtk_image_new_from_file("circle-64.png");
					//	gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
					}
					else if ((board->board_array[i][j])->type == 1) {
					//	gtk_image_new_from_file("circle-64.png");
                                        //      gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
					}

				}
			/*	else if (board->board_array[i][j]->color == "purple") {
                                        if (board->board_array[i][j]->type == "pawn") {
                                                gtk_get_image_from_file("", piece_img);
                                                gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
                                        }
                                        else if (board->board_array[i][j]->type == king) {
                                                gtk_get_image_from_file("", piece_img);
                                                gtk_button_set_image(GTK_BUTTON(board_buttons[i][j]), piece_img);
                                        }

                                }

			}

		}

	}
*/
}




/*  Board Functions */
// free pieces when they are removed?

void initialize_board() {
        for(int i = 0; i<8; i++){
                for(int j=0; j<8; j++){
                        if(i%2 == j%2 && i<3){
                                GamePiece piece;
                                piece.type = pawn;
                                piece.color = white;

                                board->board_array[i][j] = &piece;
				printf("%d", board->board_array[i][j]->color);
	                        printf("%d\n", board->board_array[i][j]->type);

			}
                        else if(i%2 == j%2 && i>4){
                                GamePiece piece;
                                piece.type = pawn;
                                piece.color = purple;

                                board->board_array[i][j] = &piece;
				printf("%d", board->board_array[i][j]->color);
        	                printf("%d\n", board->board_array[i][j]->type);

                      }
                }
        }

printf("\n");
	 if (board->board_array[0][0] == NULL) {
                printf("It's Null");
        }
else {
        printf("%d\n", board->board_array[0][0]->color);
        printf("%d\n", board->board_array[0][0]->type);
	}

}




void move_piece(int start_row, int start_col, int end_row, int end_col){
        board->board_array[end_row][end_col] = board->board_array[start_row][start_col];
        board->board_array[start_row][start_col] = NULL;
	Update_GUI();
}

void click_type(int start_row, int start_col, int end_row, int end_col){
	printf("%d", board->board_array[start_row][start_col]->color);
        printf("%d", board->board_array[start_row][start_col]->type);

	printf("%d", board->board_array[end_row][end_col]->color);
        printf("%d", board->board_array[end_row][end_col]->type);

}

void print_board(){
	for(int i = 0; i<8; i++){
		printf("\n");
		for(int j = 0; j<8; j++){
			printf("%d", board->board_array[i][j]->color);
                        printf("%d", board->board_array[i][j]->type);
		}
	}


}
//
//Thie method computes if the users move is valid. If so, the method returns 0, if not, the method
//returns 1.
//
int valid_move(){

}

//
//This method kings a piece if it has reached the opposite end of the board
//
void king_piece(GamePiece * piece){

        piece->type = king;

}

//
//This method checks to see if someone has won the game
//
int check_win(){

}


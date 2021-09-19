/************************************************************
 Listing 4

 File        : proto.h

 Author      : Matt Weisfeld

 Description : Prototypes for internal functions
	       example.
************************************************************/
typedef struct {
	WPARAM message;
	int (*funcptr)(HWND);
} INFUNCS;

LRESULT CALLBACK InternalWndProc (HWND, UINT, WPARAM, LPARAM);

int p_file_new();
int p_file_open();
int p_file_save();
int p_file_save_as();
int p_file_save_all();
int p_file_print();
int p_file_printer_setup();
int p_file_exit();
int p_edit_undo();
int p_edit_redo();
int p_edit_cut();
int p_edit_copy();
int p_edit_paste();
int p_edit_clear();


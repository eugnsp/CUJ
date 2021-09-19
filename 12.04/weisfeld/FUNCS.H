/************************************************************
 Listing 5

 File        : funcs.h

 Author      : Matt Weisfeld

 Description : Map table to internal functions.
************************************************************/
INFUNCS infuncs[] = {
	{WM_FILE_NEW, p_file_new},
	{WM_FILE_OPEN, p_file_open},
	{WM_FILE_SAVE, p_file_save},
	{WM_FILE_SAVE_AS, p_file_save_as},
	{WM_FILE_SAVE_ALL, p_file_save_all},
	{WM_FILE_PRINT, p_file_print},
	{WM_FILE_PRINTER_SETUP, p_file_printer_setup},
	{WM_FILE_EXIT, p_file_exit},
	{WM_EDIT_UNDO, p_edit_undo},
	{WM_EDIT_REDO, p_edit_redo},
	{WM_EDIT_CUT, p_edit_cut},
	{WM_EDIT_COPY, p_edit_copy},
	{WM_EDIT_PASTE, p_edit_paste},
	{WM_EDIT_CLEAR, p_edit_clear},
	{NULL,NULL},
};

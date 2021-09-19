/**************************************************************
  Listing 3 - switch statement example
**************************************************************/
case WM_COMMAND:
{
   switch (wParam) {
     case WM_FILE_NEW:
	MessageBox (hwnd, "file_new", "COMMAND SELECTED", MB_OK);
     break;
     case WM_FILE_OPEN:
	MessageBox (hwnd, "file_open", "COMMAND SELECTED", MB_OK);
     break;
     case WM_FILE_SAVE:
	MessageBox (hwnd, "file_save", "COMMAND SELECTED", MB_OK);
     break;
     case WM_FILE_SAVE_AS:
	MessageBox (hwnd, "file_save_as", "COMMAND SELECTED", MB_OK);
     break;
     case WM_FILE_SAVE_ALL:
	MessageBox (hwnd, "file_save_all", "COMMAND SELECTED", MB_OK);
     break;
     case WM_FILE_PRINT:
	MessageBox (hwnd, "file_print", "COMMAND SELECTED", MB_OK);
     break;
     case WM_FILE_PRINTER_SETUP:
	MessageBox (hwnd, "file_printer_setup", "COMMAND SELECTED", MB_OK);
     break;
     case WM_FILE_EXIT:
	MessageBox (hwnd, "file_exit", "COMMAND SELECTED", MB_OK);
     break;
     case WM_EDIT_UNDO:
	MessageBox (hwnd, "edit_undo", "COMMAND SELECTED", MB_OK);
     break;
     case WM_EDIT_REDO:
	MessageBox (hwnd, "edit_redo", "COMMAND SELECTED", MB_OK);
     break;
     case WM_EDIT_CUT:
	MessageBox (hwnd, "edit_cut", "COMMAND SELECTED", MB_OK);
     break;
     case WM_EDIT_COPY:
	MessageBox (hwnd, "edit_copy", "COMMAND SELECTED", MB_OK);
     break;
     case WM_EDIT_PASTE:
	MessageBox (hwnd, "edit_paste", "COMMAND SELECTED", MB_OK);
     break;
     case WM_EDIT_CLEAR:
	MessageBox (hwnd, "edit_clear", "COMMAND SELECTED", MB_OK);
     break;
   } 
}

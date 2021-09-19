/************************************************************
 Listing 7

 File        : funcs.c

 Author      : Matt Weisfeld

 Description : Internal functions to replace switch
	       statement.
************************************************************/
#include <Windows.h>
#include "proto.h"

int p_file_new(HWND hwnd)
{
  MessageBox (hwnd, "file_new", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_file_open(HWND hwnd)
{
  MessageBox (hwnd, "file_open", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_file_save(HWND hwnd)
{
  MessageBox (hwnd, "file_save", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_file_save_as(HWND hwnd)
{
  MessageBox (hwnd, "file_save_as", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_file_save_all(HWND hwnd)
{
  MessageBox (hwnd, "file_save_all", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_file_print(HWND hwnd)
{
  MessageBox (hwnd, "file_print", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_file_printer_setup(HWND hwnd)
{
  MessageBox (hwnd, "file_printer_setup", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_file_exit(HWND hwnd)
{
  MessageBox (hwnd, "file_exit", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_edit_undo(HWND hwnd)
{
  MessageBox (hwnd, "edit_undo", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_edit_redo(HWND hwnd)
{
  MessageBox (hwnd, "edit_redo", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_edit_cut(HWND hwnd)
{
  MessageBox (hwnd, "edit_cut", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_edit_copy(HWND hwnd)
{
  MessageBox (hwnd, "edit_copy", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_edit_paste(HWND hwnd)
{
  MessageBox (hwnd, "edit_paste", "COMMAND SELECTED", MB_OK);
  return(0);
}
int p_edit_clear(HWND hwnd)
{
  MessageBox (hwnd, "edit_clear", "COMMAND SELECTED", MB_OK);
  return(0);
}


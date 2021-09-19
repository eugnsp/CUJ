/************************************************************
 Listing 6 

 File        : internal.c

 Author      : Matt Weisfeld

 Description : Demonstrate use of internal functions
		      instead of a switch statement.
************************************************************/

#define STRICT
#include <Windows.H>
#include "proto.h"
#include "internal.h"
#include "funcs.h"

/* global variables */

char achWndClass[] = "Internal:MAIN";
char achAppName[]  = "Menu using Internal Functions";

/* Main Function */

int PASCAL WinMain (HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, LPSTR lpszCmdLine,
	int  cmdShow)  {
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;

    if (!hPrevInstance)
        {
        wndclass.lpszClassName = achWndClass;
        wndclass.hInstance     = hInstance;
	wndclass.lpfnWndProc   = InternalWndProc;
	wndclass.hCursor       = NULL;
	wndclass.hIcon         = NULL;
	wndclass.lpszMenuName  = "#1";
        wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wndclass.style         = NULL;
        wndclass.cbClsExtra    = 0;
        wndclass.cbWndExtra    = 0;

        RegisterClass( &wndclass);
        }

    hwnd = CreateWindowEx(0L, achWndClass, achAppName, 
	WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT,     
        0, NULL,  NULL, hInstance, NULL);           

    ShowWindow (hwnd, cmdShow);

    while (GetMessage(&msg, 0, 0, 0))
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg); 
        }
    return 0;
    }

/* Process messages */

LRESULT CALLBACK InternalWndProc (HWND   hwnd, UINT   mMsg,
                 WPARAM wParam, LPARAM lParam)  
    {

    int i, status;

    switch (mMsg)
        {
        case WM_COMMAND:
	    {
            /* Loop through internal function table */
	    for (i=0; infuncs[i].message != NULL; i++) {
            	// If message found, execute function
		if (infuncs[i].message == wParam) {
		    status = (*infuncs[i].funcptr)(hwnd);
		    break;
            	}
	    }

            /* If message does not exists, internal error */
	    if (infuncs[i].message == NULL) {
		    MessageBox (hwnd, "Bad Message", 
				"INTERNAL ERROR", MB_OK);
		break;
            }

	    /* If file_exit is called, exit application */   	
	    if (wParam == WM_FILE_EXIT)
                {
                SendMessage (hwnd, WM_SYSCOMMAND, SC_CLOSE, 0L);
                }
	    }

            break; /* WM_COMMAND */

        case WM_DESTROY:
            PostQuitMessage(0);  
            break;

        default:
            return(DefWindowProc(hwnd,mMsg,wParam,lParam));
            break;
        }
    return 0L;
    }

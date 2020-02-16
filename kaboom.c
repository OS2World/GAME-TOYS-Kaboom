/* Display a transparent window */
#define INCL_BASE
#define INCL_PM
#define INCL_GPIPRIMITIVES      /* GPI primitive functions      */

#include "kaboom.rh"       /* resource identifiers */

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Turn off parameter not used warning in Borland C++
#pragma warn -par
#pragma warn -rch

//--------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------

CHAR   szAppName[MAXNAMEL] = "Kaboom"; // Buffer for client window class
HMQ    hmq;                           // Handle to the process' message queue
HAB    hab;                           // Handle to the anchor block
QMSG   qmsg;                          // Queue message holder
HBITMAP hbmcirc;                      // Global Bitmap for circuit board
HBITMAP hbmhole;                      // Global Bitmap for hole
HBITMAP hbmmush[MAX_MUSH+1],
	hbmmushm[MAX_MUSH+1];         // Explosion Mushroom cloud sequence.
int	clickcount=10;		      // Click1 countdown to total destruction

//--------------------------------------------------------------
//  Function Prototype declarations
//--------------------------------------------------------------

MRESULT EXPENTRY _export MainWndProc(HWND, ULONG, MPARAM, MPARAM);
void errorbox(HWND hwnd, PSZ psztext);
void UtlCreateMemPS(HAB hab, HPS hps, ULONG cx, ULONG cy, PHPS hpsbm);
void UtlDestroyMemPS(PHPS hpsbm);
void blast(POINTL ground0, POINTL killsz, HPS hps, HWND hwnd);

// Start of main program source

INT main(VOID)
{
	ULONG    flCtlData;                        /* Frame control data */
	HWND     hwndMain=0L;                // Handle to the main frame window
	HWND     hwndClient;                 // Handle to the main client window


	flCtlData = 0; /* no frame controls on window */

	hab = WinInitialize(0L);
	hmq = WinCreateMsgQueue(hab, 0L);
	WinRegisterClass(hab,
			(PSZ)szAppName,      // Name of class being registered
			(PFNWP)MainWndProc,  // Window Procedure function pointer
			CS_SIZEREDRAW |      // Class style
			CS_CLIPCHILDREN,     // Class style
			0L);                 // Extra bytes to reserve


	// Create a top-level frame window with a client window that
	// belongs to the window class created in the previous step
	// The window is "top level" due to indicating its parent is
	// the PM desktop (HWND_DESKTOP).

   hwndMain = WinCreateStdWindow(HWND_DESKTOP,   // Parent is desktop window
						  WS_VISIBLE |
						  WS_MAXIMIZED,
						  &flCtlData,            // Pointer to frame control style
						  szAppName,             // Name of client class
						  "KaBoom!!",            // Window Title
						  WS_VISIBLE,            // Make Client Window Visible
						  (HMODULE)0L,           // Resources are in executable
									 //   vs. dynamically loaded
						  ID_RESOURCE,           // Resource/Window identifier
						  (PHWND)&hwndClient);   // Returned Pointer to client window

 WinSetWindowPos(hwndMain, HWND_TOP, 0l, 0l, 0l, 0l, SWP_MAXIMIZE | SWP_ACTIVATE);
 WinSetFocus(HWND_DESKTOP, hwndClient);

   // Required Get/Dispatch Message loop
   while(WinGetMsg(hab,(PQMSG)&qmsg, 0L, 0L, 0L))
	   WinDispatchMsg(hab, (PQMSG)&qmsg);

   return(0);

}


MRESULT EXPENTRY _export MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
RECTL  rcl;            // Rectangle structure
POINTL ground0;        // Ground Zero of the explosion
POINTL killsz;         // Kill size (size of section of screen to trash)
HPS    hps;    	       // Handle to a paint structure
int    i;              // general loop counter
USHORT fsflags, usch;  // misc work vars for WM_CHAR message

   switch(msg)
   {

	  case WM_CREATE:

		/* load all bitmaps when window is created */

		hps=WinGetPS(hwnd);
		hbmcirc = GpiLoadBitmap(hps, NULLHANDLE, IDR_CIRCUIT, 0l, 0l);
		hbmhole = GpiLoadBitmap(hps, NULLHANDLE, IDR_HOLE, 0l, 0l);
		for (i=0; i <= MAX_MUSH; i++)
			{
			hbmmush[i] = GpiLoadBitmap(hps, NULLHANDLE, IDR_MUSH1+i, 0l, 0l);
			hbmmushm[i] = GpiLoadBitmap(hps, NULLHANDLE, IDR_MUSHM1+i, 0l, 0l);
			}
		WinReleasePS(hps);
		return (MRESULT) FALSE;
		break;


	  case WM_PAINT:
		 /* nothing is actually painted - the window is transparent */
		 hps=WinBeginPaint(hwnd,(HPS)NULL,&rcl);
		 WinEndPaint(hps);
		 break;

	  case WM_BUTTON1CLICK:

		/* make a small explosion with mouse at ground zero for
		   a double click of the first mouse button */

		/* if the countdown is zero - do button2 double click instead */

		if (clickcount-- == 0)
			{
			WinPostMsg(hwnd, WM_BUTTON2DBLCLK, mp1, mp2);
			return (MRESULT) TRUE;
			}

		hps=WinGetPS(hwnd);

		ground0.x = (LONG) SHORT1FROMMP(mp1); /* store mouse pointer */
		ground0.y = (LONG) SHORT2FROMMP(mp1); /* loc as ground zero */

		killsz.x = 50;
		killsz.y = 50;

		blast(ground0, killsz, hps, hwnd); /* blast a hole */

		WinReleasePS(hps);
		return (MRESULT) TRUE;
		break;


	  case WM_BUTTON2DBLCLK:

		/* Blast the entire screen for a double click of button 2 */
		/* after toasting the whole screen, end the program */

		hps=WinGetPS(hwnd);
		WinQueryWindowRect(hwnd,&rcl);

		killsz.x = rcl.xRight - rcl.xLeft;
		killsz.y = rcl.yTop - rcl.yBottom;

		ground0.x = rcl.xLeft + killsz.x/2; /* Bottom Center */
		ground0.y = rcl.yBottom;

		blast(ground0, killsz, hps, hwnd); /* blast a hole */

		WinReleasePS(hps);

		WinPostMsg(hwnd, WM_CLOSE, (MPARAM) NULL, (MPARAM) NULL);

		return (MRESULT) TRUE;
		break;


	  case WM_CHAR:
		/* Terminate the window when Ctrl-C Pressed */

		fsflags = SHORT1FROMMP(mp1);
		usch    = SHORT1FROMMP(mp2);

		if ((toupper(usch) == 'C')  &&
		    (fsflags & KC_CTRL) )
		      {
		      WinPostMsg(hwnd, WM_CLOSE, (MPARAM) NULL, (MPARAM) NULL);
		      return (MRESULT) TRUE;
		      }
		else
		      return (MRESULT) FALSE;

		break;

	  case WM_CLOSE:
		/* display an about box */

		WinMessageBox(HWND_DESKTOP, hwnd,
		 "KaBoom!\nby Vincent Greene\nTucson, AZ",
		 "About", 1, MB_OK |  MB_APPLMODAL | MB_INFORMATION);

		WinPostMsg(hwnd, WM_QUIT, (MPARAM) NULL, (MPARAM) NULL);

		return (MRESULT) FALSE;
		break;


	  case WM_QUIT:
		/* delete all bitmaps when window is closed */
		GpiDeleteBitmap(hbmcirc);
		GpiDeleteBitmap(hbmhole);
		for (i=0; i <= MAX_MUSH; i++)
			{
			GpiDeleteBitmap(hbmmush[i]);
			GpiDeleteBitmap(hbmmushm[i]);
			}

		return (MRESULT) FALSE;
		break;

	  default:
		 return WinDefWindowProc(hwnd, msg, mp1, mp2);
   }
   return (MRESULT)0L;
}


/* BLAST

   Blasts a hole of a given size in the specified window complete with nuclear
   explosion.

Parameters:
	ground0 - Ground Zero Point (Center Bottom) of explosion area
	killsz  - Kill Zone Size - size of explosion area
	hps     - Presentation space handle for window to blast
	hwnd    - window to blast hole in.

Globals required:
	hbmhole, hab, hbmmush[], hbmmushm[]

DEFINES required:
	MAX_MUSH - Highest element in mushroom bitmap handle arrays
*/

void blast(POINTL ground0, POINTL killsz, HPS hps, HWND hwnd)

{
HPS    hpsbm;          // temporary memory bitmap
RECTL  killz;          // Kill zone (section of screen to trash)
RECTL  hole;           // rectangle describing size of hole bitmap
RECTL  mush;           // rectangle describing size of mushroom bitmap
POINTL aptl[4];
int	i;
clock_t start;
BITMAPINFOHEADER2  pbmp2Data; /* Bit-map information header     */

/* get the size of the hole bitmap */

		pbmp2Data.cbFix = 16L;
		GpiQueryBitmapInfoHeader(hbmhole, &pbmp2Data);

		hole.xLeft = 0;
		hole.yBottom = 0;
		hole.xRight = pbmp2Data.cx - 1;
		hole.yTop = pbmp2Data.cy - 1;

/* calculate kill zone */

		killz.xLeft = ground0.x - killsz.x/2;
		killz.yBottom = ground0.y;
		killz.xRight = ground0.x + killsz.x/2;
		killz.yTop = ground0.y + killsz.y;


		UtlCreateMemPS(hab, hps, killsz.x+1, killsz.y+1, &hpsbm);

/* save the kill zone in memory to restore after the explosion */

		aptl[0].x = 0; /* lower left corner */
		aptl[0].y = 0;
		aptl[1] = killsz;
		aptl[2].x = killz.xLeft;
		aptl[2].y = killz.yBottom;
		aptl[3].x = killz.xRight;
		aptl[3].y = killz.yTop;
		if (GPI_ERROR == GpiBitBlt(hpsbm, hps, 4l, aptl, ROP_SRCCOPY, BBO_IGNORE))
			errorbox(hwnd, "hpsbm, hps GPIBitBlt");

/* get the size of the mushroom bitmaps */

		pbmp2Data.cbFix = 16L;
		GpiQueryBitmapInfoHeader(hbmmush[0], &pbmp2Data);

		mush.xLeft = 0;
		mush.yBottom = 0;
		mush.xRight = pbmp2Data.cx - 1;
		mush.yTop = pbmp2Data.cy - 1;

/* do the explosion */

		for (i=0; i <= MAX_MUSH; i++) /* repeat process for all bitmaps */
			{
			start = clock();
			aptl[0].x = killz.xLeft;
			aptl[0].y = killz.yBottom;
			aptl[1].x = killz.xRight - 1;
			aptl[1].y = killz.yTop - 1;
			aptl[2].x = mush.xLeft;
			aptl[2].y = mush.yBottom;
			aptl[3].x = mush.xRight;
			aptl[3].y = mush.yTop;
			if (GPI_ERROR == GpiWCBitBlt(hps, hbmmushm[i], 4l, aptl, ROP_SRCAND, BBO_IGNORE))
				errorbox(hwnd, "hps, hbmmush GPIWCBitBlt");
			if (GPI_ERROR == GpiWCBitBlt(hps, hbmmush[i], 4l, aptl, ROP_SRCPAINT, BBO_IGNORE))
				errorbox(hwnd, "hps, hbmmush GPIWCBitBlt");

			while (clock() < start+50); /* wait 50 clock ticks */

			/* restore the kill zone from memory */

			aptl[0].x = killz.xLeft;
			aptl[0].y = killz.yBottom;
			aptl[1].x = killz.xRight;
			aptl[1].y = killz.yTop;
			aptl[2].x = 0; /* lower left corner */
			aptl[2].y = 0;
			aptl[3] = killsz;
			if (GPI_ERROR == GpiBitBlt(hps, hpsbm, 4l, aptl, ROP_SRCCOPY, BBO_IGNORE))
				errorbox(hwnd, "hps, hpsbm GPIBitBlt");
			}


/* copy the hole bitmap into memory */

		aptl[0].x = 0; /* lower left corner */
		aptl[0].y = 0;
		aptl[1] = killsz;
		aptl[2].x = hole.xLeft;
		aptl[2].y = hole.yBottom;
		aptl[3].x = hole.xRight;
		aptl[3].y = hole.yTop;
		if (GPI_ERROR == GpiWCBitBlt(hpsbm, hbmhole, 4l, aptl, ROP_SRCCOPY, BBO_IGNORE))
			errorbox(hwnd, "hpsbm, hbmcirc GPIWCBitBlt");

/* Create a hole on screen by copying from memory bitmap */

		aptl[0].x = killz.xLeft;
		aptl[0].y = killz.yBottom;
		aptl[1].x = killz.xRight;
		aptl[1].y = killz.yTop;
		aptl[2].x = 0; /* lower left corner */
		aptl[2].y = 0;
		aptl[3] = killsz;
		if (GPI_ERROR == GpiBitBlt(hps, hpsbm, 4l, aptl, ROP_SRCAND, BBO_IGNORE))
			errorbox(hwnd, "hps, hbmhole GPIWCBitBlt");

/* merge circuit section with hole bitmap in memory */
		aptl[0].x = 0; /* lower left corner */
		aptl[0].y = 0;
		aptl[1] = killsz;
		aptl[2].x = killz.xLeft;
		aptl[2].y = killz.yBottom;
		aptl[3].x = killz.xRight;
		aptl[3].y = killz.yTop;
		if (GPI_ERROR == GpiWCBitBlt(hpsbm, hbmcirc, 4l, aptl, ROP_SRCERASE, BBO_IGNORE))
			errorbox(hwnd, "hpsbm, hbmcirc GPIWCBitBlt");

/* merge memory (circuit with hole in middle) with screen */

		aptl[0].x = killz.xLeft;
		aptl[0].y = killz.yBottom;
		aptl[1].x = killz.xRight;
		aptl[1].y = killz.yTop;
		aptl[2].x = 0; /* lower left corner */
		aptl[2].y = 0;
		aptl[3] = killsz;
		if (GPI_ERROR == GpiBitBlt(hps, hpsbm, 4l, aptl, ROP_SRCPAINT, BBO_IGNORE))
			errorbox(hwnd, "hps, hpsbm GPIBitBlt");
		UtlDestroyMemPS(&hpsbm);
}



/* UtlCreateMem -

  Creates a memory device context, presentation space, and temp bitmap of
  size cx, cy.  This function sets hpsbm (the last parm) to the created
  presentation space.  It requires the anchor block of the application,
  the handle of the presentation space to model the mem space from, and
  the size of the bitmap.

*/

void UtlCreateMemPS(HAB hab, HPS hps, ULONG cx, ULONG cy, PHPS hpsbm)

{
HDC    hdcmem;              /* Device-context handle                */
HBITMAP hbmtemp;
DEVOPENSTRUC dop = {NULL, "DISPLAY", NULL, NULL, NULL, NULL,
		     NULL, NULL, NULL};
SIZEL  sizl={0, 0};     /* use same page size as device        */
BITMAPINFOHEADER2  pbmp2Data; /* Bit-map information header     */
LONG devcaps;

/* Create memory space for temporary storage & bitmap manipulation */

		hdcmem = DevOpenDC(hab, OD_MEMORY, "*", 5L, (PDEVOPENDATA)&dop, hps);
		if (DEV_ERROR == hdcmem)
			errorbox(HWND_DESKTOP, "DevOpenDC ");

		*hpsbm = GpiCreatePS(hab, hdcmem, &sizl, GPIA_ASSOC | PU_PELS);
		if (GPI_ERROR == *hpsbm)
			errorbox(HWND_DESKTOP, "GpiCreatePS ");

/* create a temporary memory bitmap of the specified size */

		pbmp2Data.cbFix = 16L;
		pbmp2Data.cx = cx;
		pbmp2Data.cy = cy;
		DevQueryCaps(GpiQueryDevice(hps), CAPS_COLOR_PLANES, 1, &devcaps);
		pbmp2Data.cPlanes = (SHORT) devcaps;
		DevQueryCaps(GpiQueryDevice(hps), CAPS_COLOR_BITCOUNT, 1, &devcaps);
		pbmp2Data.cBitCount = (SHORT) devcaps;
		hbmtemp = GpiCreateBitmap(*hpsbm, &pbmp2Data, 0l, NULL,
						(PBITMAPINFO2) NULL);

/* associate the temporary bitmap with the memory presentation space */

		GpiSetBitmap(*hpsbm, hbmtemp);
}



/* UtlDestroyMemPS

     This functions destroys a memory presentaton space, device context,
     and bitmap previously created by the UtlCreateMemPS function.

*/

void UtlDestroyMemPS(PHPS hpsbm)

{
HDC    hdcmem;              /* Device-context handle                */
HBITMAP hbmtemp;

	hbmtemp = GpiSetBitmap(*hpsbm, NULLHANDLE);
	GpiDeleteBitmap(hbmtemp);

/* destroy the memory device and PS */
	hdcmem = GpiQueryDevice(*hpsbm);
	GpiDestroyPS(*hpsbm);
	DevCloseDC(hdcmem);
}


/* errorbox

	a quick and easy function to display an error message box
	required parameters are the handle to the window (HWND_DESKTOP is
	ok here) and the text to display (only the first 80 characters
	will be processed.)

*/

void errorbox(HWND hwnd, PSZ psztext)

{
ERRORID errid;
char	szMsg[100];

errid = WinGetLastError(hab);
sprintf(szMsg, "%.80s - 0x%.4lx", psztext, errid);
WinMessageBox(hwnd, hwnd, szMsg, NULL, 1, MB_OK);
}


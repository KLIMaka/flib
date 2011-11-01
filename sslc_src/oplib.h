#ifndef _OPLIB_H_
#define _OPLIB_H_

#include "opcodes.h"

//
// Definitions of the default library set of opcodes for the interpreter.
// These should not be changed.

enum {
   O_SAYQUIT = O_END_CORE,
   O_SAYEND,
   O_SAYSTART,
   O_SAYSTARTPOS,
   O_SAYREPLYTITLE,
   O_SAYGOTOREPLY,
   O_SAYREPLY,
   O_SAYOPTION,
   O_SAYMESSAGE,
   O_SAYREPLYWINDOW,    // sayreplywindow(sx, sy, w, h, "fill3x3 pattern");
   O_SAYOPTIONWINDOW,   // sayoptionwindow ''
   O_SAYBORDER,         // sayoptionborder(x, y)
   O_SAYSCROLLUP,
   O_SAYSCROLLDOWN,
   O_SAYSETSPACING,
   O_SAYOPTIONCOLOR,
   O_SAYREPLYCOLOR,
   O_SAYRESTART,
   O_SAYGETLASTPOS,
   O_SAYREPLYFLAGS,
   O_SAYOPTIONFLAGS,
   O_SAYMESSAGETIMEOUT,	// removes sayMsg in x time, unless x = 0

   O_CREATEWIN,
   O_DELETEWIN,
   O_SELECTWIN,
   O_RESIZEWIN,
   O_SCALEWIN,
   O_SHOWWIN,
   O_FILLWIN,
   O_FILLRECT,
   O_FILLWIN3X3,
   O_DISPLAY,
   O_DISPLAYGFX,
   O_DISPLAYRAW,
   O_LOADPALETTETABLE,
   O_FADEIN,
   O_FADEOUT,
   O_GOTOXY,
   O_PRINT,
   O_FORMAT,
   O_PRINTRECT,
   O_SETFONT,
   O_SETTEXTFLAGS,
   O_SETTEXTCOLOR,
   O_SETHIGHLIGHTCOLOR,
   O_STOPMOVIE,
   O_PLAYMOVIE,
   O_MOVIEFLAGS,
   O_PLAYMOVIERECT,
   O_PLAYMOVIEALPHA,
   O_PLAYMOVIEALPHARECT,
   O_ADDREGION,
   O_ADDREGIONFLAG,
   O_ADDREGIONPROC,
   O_ADDREGIONRIGHTPROC,
   O_DELETEREGION,
   O_ACTIVATEREGION,
	O_CHECKREGION,			// returns if a region exists or not

   O_ADDBUTTON,
   O_ADDBUTTONTEXT,
   O_ADDBUTTONFLAG,
   O_ADDBUTTONGFX,
   O_ADDBUTTONPROC,
   O_ADDBUTTONRIGHTPROC,
   O_DELETEBUTTON,

   O_HIDEMOUSE,
   O_SHOWMOUSE,
   O_MOUSESHAPE,
   O_REFRESHMOUSE,
   O_SETGLOBALMOUSEFUNC,

	O_ADDNAMEDEVENT,
	O_ADDNAMEDHANDLER,
	O_CLEARNAMED,
	O_SIGNALNAMED,

   O_ADDKEY,
   O_DELETEKEY,

   O_SOUNDPLAY,
   O_SOUNDPAUSE,
   O_SOUNDRESUME,
   O_SOUNDSTOP,
   O_SOUNDREWIND,
   O_SOUNDDELETE,
   O_SETONEOPTPAUSE,

   O_SELECTFILELIST,

   O_TOKENIZE,
   O_END_LIB  // don't use this anywhere or your head will explode
};

#endif

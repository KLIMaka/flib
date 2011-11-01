#ifndef _PARSELIB_H_
#define _PARSELIB_H_

#include "tokens.h"

enum {
    C_TEXT_SHADOW = C_END_CORE,     // constants given to settextflags
    C_TEXT_UNDERLINE,
    C_TEXT_MONOSPACE,
    C_TEXT_DIRECT,
    C_TEXT_NOFILL,
    C_TEXT_TRUNC,

    C_SOUND_NORMAL,
    C_SOUND_LOOP,
    C_SOUND_8_BIT,
    C_SOUND_16_BIT,
	 C_SOUND_MEMORY,
	 C_SOUND_STREAMED,
	 C_SOUND_MONO,
	 C_SOUND_STEREO,

    C_JUSTIFY_LEFT,
    C_JUSTIFY_RIGHT,
    C_JUSTIFY_CENTER,

    C_MOVIE_SCALE,
    C_MOVIE_DIRECT,
    C_MOVIE_CENTER,
    C_MOVIE_SUBTITLE,

    C_MOUSE_ON,
    C_MOUSE_OFF,
    C_MOUSE_PRESS,
    C_MOUSE_RELEASE,
    C_MOUSE_HELD,
    C_MOUSE_RIGHT,

    C_BUTTONTAB,
    C_REGIONTAB,

    C_END_LIB
};

enum {
    T_PLAYMOVIE = T_END_CORE,
    T_PLAYMOVIERECT,
    T_PLAYMOVIEALPHA,
    T_PLAYMOVIEALPHARECT,
    T_STOPMOVIE,
    T_MOVIEFLAGS,

    T_ADDREGION,
    T_ADDREGIONPROC,
    T_ADDREGIONFLAG,
    T_ADDREGIONRIGHTPROC,
    T_DELETEREGION,
    T_ACTIVATEREGION,
	 T_CHECKREGION,			// returns if a region exists or not

    T_CREATEWIN,
    T_SELECTWIN,
    T_DELETEWIN,
    T_RESIZEWIN,
    T_SCALEWIN,
    T_SHOWWIN,
    T_FILLWIN,
    T_FILLRECT,
    T_FILLWIN3X3,
    T_PRINT,
    T_FORMAT,
    T_PRINTRECT,
    T_SETFONT,
    T_SETTEXTFLAGS,
    T_SETTEXTCOLOR,
    T_SETHIGHLIGHTCOLOR,
    T_DISPLAY,
    T_DISPLAYGFX,
    T_DISPLAYRAW,
    T_LOADPALETTETABLE,
    T_FADEIN,
    T_FADEOUT,
    T_GOTOXY,
    T_ADDBUTTON,
    T_ADDBUTTONTEXT,
    T_ADDBUTTONGFX,
    T_ADDBUTTONPROC,
    T_ADDBUTTONFLAG,
    T_ADDBUTTONRIGHTPROC,
    T_DELETEBUTTON,

//dialog commands
    T_SAYSTART,
    T_SAYSTARTPOS,
    T_SAYREPLYTITLE,
    T_SAYGOTOREPLY,
    T_SAYOPTION,
    T_SAYREPLY,
    T_SAYEND,
    T_SAYQUIT,
    T_SAYMESSAGE,
    T_SAYREPLYWINDOW,  // sayreplywindow(sx, sy, w, h, "fill3x3 pattern");
    T_SAYOPTIONWINDOW, // sayoptionwindow ''
    T_SAYREPLYFLAGS,
    T_SAYOPTIONFLAGS,
    T_SAYMESSAGETIMEOUT,

    T_SAYBORDER,
    T_SAYSCROLLUP, // sayscrollup(up, down, hover);
    T_SAYSCROLLDOWN, // sayscrolldown(up, down, hover);
    T_SAYSETSPACING,
    T_SAYOPTIONCOLOR,
    T_SAYREPLYCOLOR,
    T_SAYRESTART,
    T_SAYGETLASTPOS,

// mouse commands
    T_HIDEMOUSE,
    T_SHOWMOUSE,
    T_REFRESHMOUSE,
    T_MOUSESHAPE,
    T_SETGLOBALMOUSEFUNC,

// named event commands
	 T_ADDNAMEDEVENT,
	 T_ADDNAMEDHANDLER,
	 T_CLEARNAMED,
	 T_SIGNALNAMED,

    T_ADDKEY,
    T_DELETEKEY,

    T_SOUNDPLAY,
    T_SOUNDPAUSE,
    T_SOUNDRESUME,
    T_SOUNDSTOP,
    T_SOUNDREWIND,
    T_SOUNDDELETE,
    T_SETONEOPTPAUSE,	//allow auto-selection for lone dialog options

    T_SELECTFILELIST,
    T_END_LIB
};

extern void parseLib(Procedure *, int);
extern void parseLibExpression(Procedure *, int);
extern int writeLibStatement(NodeList *n, int i, OutputStream *f);
int writeLibExpression(NodeList *n, int i, OutputStream *f);
extern void initLibExtras(void);


#endif

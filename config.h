#include "include.h"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const int startwithgaps	     = 0;	 /* 1 means gaps are used by default */
static const unsigned int gappx     = 5;       /* default gap between windows in pixels */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "monospace:size=10", "fontawesome:size=12" };
static const char dmenufont[]       = "monospace:size=10";
//background color
static const char col_gray1[]       = "#222222";
//inactive window border
static const char col_gray2[]       = "#444444";
//font color
static const char col_gray3[]       = "#bbbbbb";
//current tag and current window font color
static const char col_gray4[]       = "#eeeeee";
//top bar second color and inactive window border color
static const char col_main[]        = "#862d86";
static const char *colors[][3]      = {
	/*               fg         bg         border       */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_main,  col_main  },
};

static const char *const autostart[] = {
	"st", NULL,
	"picom", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "","", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 */
	/* class      instance    title       tags mask    switchtotag   isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       1 << 8,           1,           0,           -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ ">0<",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char dmenuhp[] = "firefox,hexchat,pcmanfm,mousepad,discord,whatsapp";
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_main, "-sf", col_gray4, "-hp", dmenuhp, NULL };
//static const char *dmenudesktop[] = { "j4-dmenu-desktop", "--dmenu=dmenu_run -i", "--term='st'", NULL  };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "90x27", NULL };
static const char *scrotcmd[]  = { "scrot", "-t", "25", NULL };
static const char *scrotfocusedcmd[]  = { "scrot", "--focused", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */

	//dmenu
	{ MODKEY,                       XK_p,      spawn,           {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,           SHCMD("j4-dmenu-desktop --dmenu=dmenu_run  --term='st'") },
	//spawn terminal
	{ MODKEY|ShiftMask,             XK_Return, spawn,           {.v = termcmd } },
	//scratchpad
	{ MODKEY,                       XK_grave,  togglescratch,   {.v = scratchpadcmd } },
	//toggle bar
	{ MODKEY,                       XK_b,      togglebar,       {0} },
	//stack focus
	{ MODKEY,                       XK_j,      focusstack,      {.i = +1 } },
        { MODKEY,                       XK_k,      focusstack,      {.i = -1 } },
	//shiftview
	{ MODKEY,              		XK_x,      shiftview,       { .i = +1 } },
	{ MODKEY,             	        XK_z,      shiftview,       { .i = -1 } },
	{ MODKEY|ControlMask,           XK_x,      shiftviewclient, { .i = +1 } },
	{ MODKEY|ControlMask,           XK_z,      shiftviewclient, { .i = -1 } },
        { MODKEY|ShiftMask,             XK_x,      shiftviewclient, { .i = +1 } },
        { MODKEY|ShiftMask,             XK_z,      shiftviewclient, { .i = -1 } },
	//???
	{ MODKEY,                       XK_i,      incnmaster,      {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,      {.i = -1 } },
	//adjust window size
	{ MODKEY,                       XK_h,      setmfact,        {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,        {.f = +0.05} },
	//zoom?
	{ MODKEY,     		        XK_Return, zoom,            {0} },
	//tab
	{ MODKEY,                       XK_Tab,    view,            {0} },
	//kill
	{ MODKEY|ShiftMask,             XK_q,      killclient,      {0} },
        { MODKEY|ShiftMask,             XK_c,      killclient,      {0} },
	// screenshot, print screen
	{ 0,         		        XK_Print,  spawn,	    {.v = scrotcmd } },
	{ ShiftMask,   		        XK_Print,  spawn,           {.v = scrotfocusedcmd } },
	{ ControlMask,  		XK_Print,  spawn,           SHCMD("sleep 1s;scrot --select") },
	//tile, floating or module
	{ MODKEY,                       XK_t,      setlayout,       {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,       {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,       {.v = &layouts[2]} },
	//toggle title, floating or module cycle
	{ MODKEY,                       XK_space,  setlayout,       {0} },
	//toggle floating?
	{ MODKEY|ShiftMask,             XK_space,  togglefloating,  {0} },
	//fullscreen
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,   {0} },
	//view all tags
	{ MODKEY,                       XK_0,      view,            {.ui = ~0 } },
	//????
	{ MODKEY|ShiftMask,             XK_0,      tag,             {.ui = ~0 } },
	//????
	{ MODKEY,                       XK_comma,  focusmon,        {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,          {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,          {.i = +1 } },
	// gaps
	{ MODKEY,                       XK_equal,  setgaps,         {.i = -2.5 } },
	{ MODKEY,                       XK_minus,  setgaps,         {.i = +2.5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,         {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,         {.i = GAP_TOGGLE} },
	//quit
        { MODKEY|ShiftMask,             XK_Delete,      quit,       {0} },

	//tags
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


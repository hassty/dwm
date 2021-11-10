/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 8;        /* horiz inner gap between windows */
static unsigned int gappiv    = 8;        /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static unsigned int smartgaps = 1;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char fontMain[]        = "RobotoMono Nerd Font Mono:size=10";
static char fontEmoji[]       = "JoyPixels:pixelsize=10:antialias=true:autohint=true";
static const char *fonts[]          = { fontMain, fontEmoji };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#ffffff";
static char selbgcolor[]            = "#333333";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "ﭮ", "", "", "", "" };
static const char *defaulttagapps[] = {"st", "pcmanfm", "freetube", "firefox", "discord", NULL, NULL, NULL, NULL };

static const unsigned int ulinepad	= 1;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class               instance  title          tags mask  isfloating  isterminal  noswallow  monitor  scratch key*/
	{ "Pcmanfm",           NULL,     NULL,          1 << 1,    0,          0,          -1,        -1,      0 },
	{ "FreeTube",          NULL,     NULL,          1 << 2,    0,          0,          -1,        -1,      0 },
	{ "firefox",           NULL,     NULL,          1 << 3,    0,          0,          -1,        -1,      0 },
	{ "discord",           NULL,     NULL,          1 << 4,    0,          0,          -1,        -1,      0 },
	{ "mpv",               NULL,     NULL,          1 << 5,    0,          0,           0,        -1,      0 },
	{ "obs",               NULL,     NULL,          1 << 6,    0,          0,          -1,        -1,      0 },
	{ "St",                NULL,     NULL,          0,         0,          1,          -1,        -1,      0 },
	{ "Gnome-calculator",  NULL,     NULL,          0,         1,          0,          -1,        -1,      0 },
	{ NULL,                NULL,     "scratchpad",  0,         1,          0,          -1,        -1,      't' },
	{ NULL,                NULL,     "vifm",        0,         1,          0,          -1,        -1,      'f' },
	{ NULL,                NULL,     "Event Tester",0,         0,          0,           1,        -1,      0 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

static const MonitorRule monrules[] = {
	/* monitor  tag  layout  mfact  nmaster  showbar  topbar */
	{  0,        1,  0,      -1,    -1,      -1,      -1     },
	{  0,        3,  1,      -1,    -1,      -1,      -1     },
	{  0,        4,  1,      -1,    -1,      -1,      -1     },
	{  0,        6,  1,      -1,    -1,       0,      -1     },
	{  0,        0,  7,      -1,    -1,       1,      -1     },
	{ -1,       -1,  0,      -1,    -1,      -1,      -1     }, // default
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-c", "-l", "10", "-h", "20", NULL };
static const char *termcmd[]  = { "st", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"t", "st", "-t", "scratchpad", NULL}; 
static const char *vifmcmd[]       = {"f", "st", "-t", "vifm", "-e", "/home/hasty/.config/vifm/scripts/vifmrun", NULL}; 

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
        { "fontMain",           STRING,  &fontMain },
        { "fontEmoji",          STRING,  &fontEmoji },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
        { "gappih",             INTEGER, &gappih },
        { "gappiv",             INTEGER, &gappiv },
        { "gappoh",             INTEGER, &gappoh },
        { "gappov",             INTEGER, &gappov },
        { "smartgaps",          INTEGER, &smartgaps },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

#include "movestack.c"
#include "shiftview.c"
static Key keys[] = {
    /* modifier                     key        function         argument */
    { MODKEY,                       33,        spawn,           { .v = dmenucmd } },
    { MODKEY,                       36,        spawn,           { .v = termcmd } },
    { MODKEY,                       27,        spawndefault,    { 0 } },
    { MODKEY,                       25,        togglescratch,   { .v = scratchpadcmd } },
    { MODKEY,                       26,        togglescratch,   { .v = vifmcmd } },
    { MODKEY,                       56,        togglebar,       { 0 } },
    { MODKEY,                       39,        swapfocus,       { 0 } },
    { MODKEY,                       44,        focusstack,      { .i = +1 } },
    { MODKEY,                       45,        focusstack,      { .i = -1 } },
    { MODKEY,                       65,        focusmaster,     { 0 } },
    { MODKEY,                       31,        incnmaster,      { .i = +1 } },
    { MODKEY,                       40,        incnmaster,      { .i = -1 } },
    { MODKEY,                       43,        setmfact,        { .f = -0.05 } },
    { MODKEY,                       46,        setmfact,        { .f = +0.05 } },
    { MODKEY|ShiftMask,             43,        setcfact,        { .f = +0.25 } },
    { MODKEY|ShiftMask,             46,        setcfact,        { .f = -0.25 } },
    { MODKEY|ShiftMask,             32,        setcfact,        { .f = 0.00 } },
    { MODKEY|ShiftMask,             44,        movestack,       { .i = +1 } },
    { MODKEY|ShiftMask,             45,        movestack,       { .i = -1 } },
    { MODKEY|ControlMask,           39,        togglecanfocusfloating, { 0 } },
    { MODKEY|ShiftMask,             36,        zoom,            { 0 } },
    { MODKEY|Mod1Mask,              19,        togglegaps,      { 0 } },
    { MODKEY,                       23,        view,            { 0 } },
    { MODKEY,                       54,        killclient,      { 0 } },
    { MODKEY,                       53,        killunsel,       { 0 } },
    { MODKEY,                       28,        setlayout,       { .v = &layouts[0] } },
    { MODKEY|ShiftMask,             41,        setlayout,       { .v = &layouts[1] } },
    { MODKEY|ControlMask,           59,        cyclelayout,     { .i = -1 } },
    { MODKEY|ControlMask,           60,        cyclelayout,     { .i = +1 } },
    { MODKEY|ControlMask,           65,        setlayout,       { 0 } },
    { MODKEY|ShiftMask,             65,        togglefloating,  { 0 } },
    { MODKEY|ShiftMask,             39,        togglesticky,    { 0 } },
    { MODKEY,                       41,        togglefullscr,   { 0 } },
    { MODKEY,                       19,        view,            { .ui = ~0 } },
    { MODKEY|ShiftMask,             19,        tag,             { .ui = ~0 } },
    { MODKEY,                       58,        focusmon,        { .i = -1 } },
    { MODKEY,                       61,        focusmon,        { .i = +1 } },
    { MODKEY|ShiftMask,             58,        tagmon,          { .i = -1 } },
    { MODKEY|ShiftMask,             61,        tagmon,          { .i = +1 } },
    { MODKEY,                       59,        shiftviewjump,   { .i = -1 } },
    { MODKEY,                       60,        shiftviewjump,   { .i = +1 } },
    { MODKEY|Mod1Mask,              59,        shiftview,       { .i = -1 } },
    { MODKEY|Mod1Mask,              60,        shiftview,       { .i = +1 } },
    { MODKEY|ShiftMask,             59,        tagtoleft,       { 0 } },
    { MODKEY|ShiftMask,             60,        tagtoright,      { 0 } },
    { MODKEY,                       116,       moveresize,      { .v = "0x 25y 0w 0h" } },
    { MODKEY,                       111,       moveresize,      { .v = "0x -25y 0w 0h" } },
    { MODKEY,                       114,       moveresize,      { .v = "25x 0y 0w 0h" } },
    { MODKEY,                       113,       moveresize,      { .v = "-25x 0y 0w 0h" } },
    { MODKEY|ShiftMask,             116,       moveresize,      { .v = "0x 0y 0w 25h" } },
    { MODKEY|ShiftMask,             111,       moveresize,      { .v = "0x 0y 0w -25h" } },
    { MODKEY|ShiftMask,             114,       moveresize,      { .v = "0x 0y 25w 0h" } },
    { MODKEY|ShiftMask,             113,       moveresize,      { .v = "0x 0y -25w 0h" } },
    { MODKEY|ControlMask,           111,       moveresizeedge,  { .v = "t" } },
    { MODKEY|ControlMask,           116,       moveresizeedge,  { .v = "b" } },
    { MODKEY|ControlMask,           113,       moveresizeedge,  { .v = "l" } },
    { MODKEY|ControlMask,           114,       moveresizeedge,  { .v = "r" } },
    { MODKEY|ControlMask|ShiftMask, 111,       moveresizeedge,  { .v = "T" } },
    { MODKEY|ControlMask|ShiftMask, 116,       moveresizeedge,  { .v = "B" } },
    { MODKEY|ControlMask|ShiftMask, 113,       moveresizeedge,  { .v = "L" } },
    { MODKEY|ControlMask|ShiftMask, 114,       moveresizeedge,  { .v = "R" } },
    TAGKEYS(                        10,        0)
    TAGKEYS(                        11,        1)
    TAGKEYS(                        12,        2)
    TAGKEYS(                        13,        3)
    TAGKEYS(                        14,        4)
    TAGKEYS(                        15,        5)
    TAGKEYS(                        16,        6)
    TAGKEYS(                        17,        7)
    TAGKEYS(                        18,        8)
    { MODKEY,                       32,        winview,         { 0 } },
    { MODKEY|ShiftMask,             24,        quit,            { 0 } },
    { MODKEY|ShiftMask,             27,        quit,            { 1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      { 0 } },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      { .v = &layouts[2] } },
	{ ClkWinTitle,          0,              Button2,        zoom,           { 0 } },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   { .i = 1 } },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   { .i = 2 } },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   { .i = 3 } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      { 0 } },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, { 0 } },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    { 0 } },
	{ ClkTagBar,            0,              Button1,        view,           { 0 } },
	{ ClkTagBar,            0,              Button3,        toggleview,     { 0 } },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            { 0 } },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      { 0 } },
    { ClkTagBar,            0,              Button1,        view,           { 0 } },
    { ClkTagBar,            0,              Button3,        toggleview,     { 0 } },
    { ClkTagBar,            MODKEY,         Button1,        tag,            { 0 } },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      { 0 } },
};

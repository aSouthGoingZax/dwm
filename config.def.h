/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 2;        /* border pixel of windows */
static const unsigned int snap     = 32;       /* snap pixel */
static const unsigned int gappih   = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv   = 20;       /* vert inner gap between windows */
static const unsigned int gappoh   = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov   = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps         = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar           = 1;        /* 0 means no bar */
static const int topbar            = 1;        /* 0 means bottom bar */
static const int bhpad             = 15;       /* amount to add on to default calculated by dwm */
static const char *fonts[]         = { "sans-serif:size=13", "monospace:size=13", "Uchen:size=18" };
//static const char dmenufont[]      = "monospace:size=14";
static const char *colors[][3]     = {
	/*                 fg         bg         border   */
	[SchemeNorm]   = { "#727a87", "#000000", "#1a1b26" },
	[SchemeSel]    = { "#eaf2ff", "#000000", "#0066ff" },
	[SchemeStatus] = { "#aeb6c3", "#000000", "#ff0000" },    // last colour is unused but cannot be empty
	[SchemeTitle]  = { "#d6deeb", "#000000", "#ff0000" },    // last colour is unused but cannot be empty
};
static const unsigned int baralpha = 0xcc;      /* 204 in hexadecimal, 204/255=0.8 */
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3] = {
	/*                 fg      bg        border     */
	[SchemeNorm]   = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]    = { OPAQUE, baralpha, borderalpha },
	[SchemeStatus] = { OPAQUE, baralpha, borderalpha },
	[SchemeTitle]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "༡", "༢", "༣", "༤", "༥", "༦", "༧", "༨", "༩" };
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	    WM_CLASS(STRING) = instance, class
	    WM_NAME(STRING) = title */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layouts */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "tile1",    tile },		/* first entry is default */
    { "tile2",    bstack },
    { "grid",     gaplessgrid },
    { "mono",     monocle },
    { "deck",     deck },
    { "centre",   centeredmaster },
    { "float",    NULL },		/* no layout function means floating behavior */
    { NULL,       NULL },
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
static char dmenumon[2]         = "0";    /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[]   = { "dmenu_run", "-i", "-c", "-l", "15", "-m", dmenumon, "-fn", dmenufont, "-nb", colour_2, "-nf", colour_1, "-sb", colour_6, "-sf", colour_4, NULL };
static const char *dmenucmd[]   = { "dmenu_run", "-i", "-m", dmenumon, NULL };
static const char *termcmd[]    = { "alacritty", NULL };
static const char *stermcmd[]   = { "st", NULL };
static const char *browsercmd[] = { "brave", NULL };
static const char *filemgrcmd[] = { "alacritty", "-e", "lf", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
    /* modifier                 key                 function            argument */
    { MODKEY,                   XK_space,           spawn,              {.v = dmenucmd } },
    { MODKEY,                   XK_Return,          spawn,              {.v = termcmd } },
    { Mod4Mask,                 XK_Return,          spawn,              {.v = stermcmd } },
    { MODKEY|ShiftMask,         XK_Return,          zoom,               {0} },
    { MODKEY,                   XK_Tab,             focusmaster,        {0} },
    { MODKEY|ShiftMask,         XK_Tab,             togglefullscr,      {0} },
    { MODKEY,                   XK_j,               focusstack,         {.i = +1 } },
    { MODKEY,                   XK_k,               focusstack,         {.i = -1 } },
    { MODKEY|ShiftMask,         XK_j,               pushdown,           {0} },
    { MODKEY|ShiftMask,         XK_k,               pushup,             {0} },
    { MODKEY,                   XK_h,               setmfact,           {.f = -0.05} },
    { MODKEY,                   XK_l,               setmfact,           {.f = +0.05} },
    { MODKEY|ShiftMask,         XK_h,               setcfact,           {.f = -0.25} },
    { MODKEY|ShiftMask,         XK_l,               setcfact,           {.f = +0.25} },
    { MODKEY|ControlMask,       XK_l,               setcfact,           {.f =  0.00} },
    { MODKEY,                   XK_bracketleft,     incrgaps,           {.i = -5 } },
    { MODKEY,                   XK_bracketright,    incrgaps,           {.i = +5 } },
    //{ MODKEY|Mod4Mask,          XK_bracketleft,     incrohgaps,         {.i = -20 } },
    //{ MODKEY|Mod4Mask,          XK_bracketright,    incrohgaps,         {.i = +20 } },
    { MODKEY|ControlMask,       XK_bracketleft,     incrovgaps,         {.i = -20 } },
    { MODKEY|ControlMask,       XK_bracketright,    incrovgaps,         {.i = +20 } },
    { MODKEY|Mod4Mask,          XK_bracketleft,     incrovgaps,         {.i = -940 } },
    { MODKEY|Mod4Mask,          XK_bracketright,    incrovgaps,         {.i = +940 } },
    { MODKEY|ShiftMask,         XK_bracketleft,     togglegaps,         {0} },
    { MODKEY|ShiftMask,         XK_bracketright,    defaultgaps,        {0} },
    { MODKEY|ShiftMask,         XK_c,               killclient,         {0} },
    { MODKEY,                   XK_t,               setlayout,          {.v = &layouts[0]} },
    { MODKEY|Mod4Mask,          XK_t,               setlayout,          {.v = &layouts[1]} },
    { MODKEY|ControlMask,       XK_t,               setlayout,          {.v = &layouts[2]} },
    { MODKEY,                   XK_y,               setlayout,          {.v = &layouts[3]} },
    { MODKEY|Mod4Mask,          XK_y,               setlayout,          {.v = &layouts[4]} },
    { MODKEY|ControlMask,       XK_y,               setlayout,          {.v = &layouts[5]} },
    { MODKEY,                   XK_f,               setlayout,          {.v = &layouts[6]} },
    { MODKEY|ShiftMask,         XK_f,               togglefloating,     {0} },
    { MODKEY,                   XK_o,               winview,            {0} },
    { MODKEY,                   XK_0,               view,               {.ui = ~0 } },
    { MODKEY|ShiftMask,         XK_0,               tag,                {.ui = ~0 } },
    { MODKEY,                   XK_comma,           focusmon,           {.i = -1 } },
    { MODKEY,                   XK_period,          focusmon,           {.i = +1 } },
    { MODKEY|ShiftMask,         XK_comma,           tagmon,             {.i = -1 } },
    { MODKEY|ShiftMask,         XK_period,          tagmon,             {.i = +1 } },
    TAGKEYS(                    XK_1,                                   0)
    TAGKEYS(                    XK_2,                                   1)
    TAGKEYS(                    XK_3,                                   2)
    TAGKEYS(                    XK_4,                                   3)
    TAGKEYS(                    XK_5,                                   4)
    TAGKEYS(                    XK_6,                                   5)
    TAGKEYS(                    XK_7,                                   6)
    TAGKEYS(                    XK_8,                                   7)
    TAGKEYS(                    XK_9,                                   8)
    { MODKEY,                   XK_b,               togglebar,          {0} },
    { MODKEY|ShiftMask,         XK_q,               quit,               {0} },
    { Mod4Mask,                 XK_w,               spawn,              {.v = browsercmd } },
    { Mod4Mask,                 XK_f,               spawn,              {.v = filemgrcmd } },
    { MODKEY|ControlMask,       XK_Delete,          spawn,              SHCMD("power-actions") },
    { 0,                XF86XK_AudioRaiseVolume,    spawn,              SHCMD("pactl set-sink-volume 0 +1%") },
    { 0,                XF86XK_AudioLowerVolume,    spawn,              SHCMD("pactl set-sink-volume 0 -1%") },
    { 0,                XF86XK_AudioMute,           spawn,              SHCMD("pactl set-sink-mute 0 toggle") },
    { Mod5Mask,	                XK_b,               spawn,              SHCMD("bookmark-launch") },
    { Mod5Mask,	                XK_l,               spawn,              SHCMD("login-helper") },
    { Mod5Mask,	                XK_m,               spawn,              SHCMD("drive-mount") },
    { Mod5Mask|ShiftMask,       XK_m,               spawn,              SHCMD("drive-unmount") },
    { Mod5Mask,	                XK_p,               spawn,              SHCMD("password-helper") },
    { Mod5Mask,	                XK_r,               spawn,              SHCMD("screen-temp") },
    { Mod5Mask,	                XK_t,               spawn,              SHCMD("term-progs") },
    { Mod5Mask,	                XK_v,               spawn,              SHCMD("volume-change") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

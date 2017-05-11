local ffi = require "ffi"

ffi.cdef [[

static const unsigned int XK_VoidSymbol = 0xffffff; /* Void symbol */

/*
 * TTY function keys, cleverly chosen to map to ASCII, for convenience of
 * programming, but could have been arbitrary (at the cost of lookup
 * tables in client code).
 */

static const unsigned int XK_BackSpace = 0xff08; /* Back space, back char */
static const unsigned int XK_Tab = 0xff09;
static const unsigned int XK_Linefeed = 0xff0a; /* Linefeed, LF */
static const unsigned int XK_Clear = 0xff0b;
static const unsigned int XK_Return = 0xff0d; /* Return, enter */
static const unsigned int XK_Pause = 0xff13;  /* Pause, hold */
static const unsigned int XK_Scroll_Lock = 0xff14;
static const unsigned int XK_Sys_Req = 0xff15;
static const unsigned int XK_Escape = 0xff1b;
static const unsigned int XK_Delete = 0xffff; /* Delete, rubout */

/* International & multi-key character composition */

static const unsigned int XK_Multi_key = 0xff20; /* Multi-key character compose */
static const unsigned int XK_Codeinput = 0xff37;
static const unsigned int XK_SingleCandidate = 0xff3c;
static const unsigned int XK_MultipleCandidate = 0xff3d;
static const unsigned int XK_PreviousCandidate = 0xff3e;

/* Japanese keyboard support */

static const unsigned int XK_Kanji = 0xff21;             /* Kanji, Kanji convert */
static const unsigned int XK_Muhenkan = 0xff22;          /* Cancel Conversion */
static const unsigned int XK_Henkan_Mode = 0xff23;       /* Start/Stop Conversion */
static const unsigned int XK_Henkan = 0xff23;            /* Alias for Henkan_Mode */
static const unsigned int XK_Romaji = 0xff24;            /* to Romaji */
static const unsigned int XK_Hiragana = 0xff25;          /* to Hiragana */
static const unsigned int XK_Katakana = 0xff26;          /* to Katakana */
static const unsigned int XK_Hiragana_Katakana = 0xff27; /* Hiragana/Katakana toggle */
static const unsigned int XK_Zenkaku = 0xff28;           /* to Zenkaku */
static const unsigned int XK_Hankaku = 0xff29;           /* to Hankaku */
static const unsigned int XK_Zenkaku_Hankaku = 0xff2a;   /* Zenkaku/Hankaku toggle */
static const unsigned int XK_Touroku = 0xff2b;           /* Add to Dictionary */
static const unsigned int XK_Massyo = 0xff2c;            /* Delete from Dictionary */
static const unsigned int XK_Kana_Lock = 0xff2d;         /* Kana Lock */
static const unsigned int XK_Kana_Shift = 0xff2e;        /* Kana Shift */
static const unsigned int XK_Eisu_Shift = 0xff2f;        /* Alphanumeric Shift */
static const unsigned int XK_Eisu_toggle = 0xff30;       /* Alphanumeric toggle */
static const unsigned int XK_Kanji_Bangou = 0xff37;      /* Codeinput */
static const unsigned int XK_Zen_Koho = 0xff3d;          /* Multiple/All Candidate(s) */
static const unsigned int XK_Mae_Koho = 0xff3e;          /* Previous Candidate */

/* 0xff31 thru 0xff3f are under XK_KOREAN */

/* Cursor control & motion */

static const unsigned int XK_Home = 0xff50;
static const unsigned int XK_Left = 0xff51;  /* Move left, left arrow */
static const unsigned int XK_Up = 0xff52;    /* Move up, up arrow */
static const unsigned int XK_Right = 0xff53; /* Move right, right arrow */
static const unsigned int XK_Down = 0xff54;  /* Move down, down arrow */
static const unsigned int XK_Prior = 0xff55; /* Prior, previous */
static const unsigned int XK_Page_Up = 0xff55;
static const unsigned int XK_Next = 0xff56; /* Next */
static const unsigned int XK_Page_Down = 0xff56;
static const unsigned int XK_End = 0xff57;   /* EOL */
static const unsigned int XK_Begin = 0xff58; /* BOL */

/* Misc functions */

static const unsigned int XK_Select = 0xff60; /* Select, mark */
static const unsigned int XK_Print = 0xff61;
static const unsigned int XK_Execute = 0xff62; /* Execute, run, do */
static const unsigned int XK_Insert = 0xff63;  /* Insert, insert here */
static const unsigned int XK_Undo = 0xff65;
static const unsigned int XK_Redo = 0xff66; /* Redo, again */
static const unsigned int XK_Menu = 0xff67;
static const unsigned int XK_Find = 0xff68;   /* Find, search */
static const unsigned int XK_Cancel = 0xff69; /* Cancel, stop, abort, exit */
static const unsigned int XK_Help = 0xff6a;   /* Help */
static const unsigned int XK_Break = 0xff6b;
static const unsigned int XK_Mode_switch = 0xff7e;   /* Character set switch */
static const unsigned int XK_script_switch = 0xff7e; /* Alias for mode_switch */
static const unsigned int XK_Num_Lock = 0xff7f;

/* Keypad functions, keypad numbers cleverly chosen to map to ASCII */

static const unsigned int XK_KP_Space = 0xff80; /* Space */
static const unsigned int XK_KP_Tab = 0xff89;
static const unsigned int XK_KP_Enter = 0xff8d; /* Enter */
static const unsigned int XK_KP_F1 = 0xff91;    /* PF1, KP_A, ... */
static const unsigned int XK_KP_F2 = 0xff92;
static const unsigned int XK_KP_F3 = 0xff93;
static const unsigned int XK_KP_F4 = 0xff94;
static const unsigned int XK_KP_Home = 0xff95;
static const unsigned int XK_KP_Left = 0xff96;
static const unsigned int XK_KP_Up = 0xff97;
static const unsigned int XK_KP_Right = 0xff98;
static const unsigned int XK_KP_Down = 0xff99;
static const unsigned int XK_KP_Prior = 0xff9a;
static const unsigned int XK_KP_Page_Up = 0xff9a;
static const unsigned int XK_KP_Next = 0xff9b;
static const unsigned int XK_KP_Page_Down = 0xff9b;
static const unsigned int XK_KP_End = 0xff9c;
static const unsigned int XK_KP_Begin = 0xff9d;
static const unsigned int XK_KP_Insert = 0xff9e;
static const unsigned int XK_KP_Delete = 0xff9f;
static const unsigned int XK_KP_Equal = 0xffbd; /* Equals */
static const unsigned int XK_KP_Multiply = 0xffaa;
static const unsigned int XK_KP_Add = 0xffab;
static const unsigned int XK_KP_Separator = 0xffac; /* Separator, often comma */
static const unsigned int XK_KP_Subtract = 0xffad;
static const unsigned int XK_KP_Decimal = 0xffae;
static const unsigned int XK_KP_Divide = 0xffaf;

static const unsigned int XK_KP_0 = 0xffb0;
static const unsigned int XK_KP_1 = 0xffb1;
static const unsigned int XK_KP_2 = 0xffb2;
static const unsigned int XK_KP_3 = 0xffb3;
static const unsigned int XK_KP_4 = 0xffb4;
static const unsigned int XK_KP_5 = 0xffb5;
static const unsigned int XK_KP_6 = 0xffb6;
static const unsigned int XK_KP_7 = 0xffb7;
static const unsigned int XK_KP_8 = 0xffb8;
static const unsigned int XK_KP_9 = 0xffb9;

/*
 * Auxiliary functions; note the duplicate definitions for left and right
 * function keys;  Sun keyboards and a few other manufacturers have such
 * function key groups on the left and/or right sides of the keyboard.
 * We've not found a keyboard with more than 35 function keys total.
 */

static const unsigned int XK_F1 = 0xffbe;
static const unsigned int XK_F2 = 0xffbf;
static const unsigned int XK_F3 = 0xffc0;
static const unsigned int XK_F4 = 0xffc1;
static const unsigned int XK_F5 = 0xffc2;
static const unsigned int XK_F6 = 0xffc3;
static const unsigned int XK_F7 = 0xffc4;
static const unsigned int XK_F8 = 0xffc5;
static const unsigned int XK_F9 = 0xffc6;
static const unsigned int XK_F10 = 0xffc7;
static const unsigned int XK_F11 = 0xffc8;
static const unsigned int XK_L1 = 0xffc8;
static const unsigned int XK_F12 = 0xffc9;
static const unsigned int XK_L2 = 0xffc9;
static const unsigned int XK_F13 = 0xffca;
static const unsigned int XK_L3 = 0xffca;
static const unsigned int XK_F14 = 0xffcb;
static const unsigned int XK_L4 = 0xffcb;
static const unsigned int XK_F15 = 0xffcc;
static const unsigned int XK_L5 = 0xffcc;
static const unsigned int XK_F16 = 0xffcd;
static const unsigned int XK_L6 = 0xffcd;
static const unsigned int XK_F17 = 0xffce;
static const unsigned int XK_L7 = 0xffce;
static const unsigned int XK_F18 = 0xffcf;
static const unsigned int XK_L8 = 0xffcf;
static const unsigned int XK_F19 = 0xffd0;
static const unsigned int XK_L9 = 0xffd0;
static const unsigned int XK_F20 = 0xffd1;
static const unsigned int XK_L10 = 0xffd1;
static const unsigned int XK_F21 = 0xffd2;
static const unsigned int XK_R1 = 0xffd2;
static const unsigned int XK_F22 = 0xffd3;
static const unsigned int XK_R2 = 0xffd3;
static const unsigned int XK_F23 = 0xffd4;
static const unsigned int XK_R3 = 0xffd4;
static const unsigned int XK_F24 = 0xffd5;
static const unsigned int XK_R4 = 0xffd5;
static const unsigned int XK_F25 = 0xffd6;
static const unsigned int XK_R5 = 0xffd6;
static const unsigned int XK_F26 = 0xffd7;
static const unsigned int XK_R6 = 0xffd7;
static const unsigned int XK_F27 = 0xffd8;
static const unsigned int XK_R7 = 0xffd8;
static const unsigned int XK_F28 = 0xffd9;
static const unsigned int XK_R8 = 0xffd9;
static const unsigned int XK_F29 = 0xffda;
static const unsigned int XK_R9 = 0xffda;
static const unsigned int XK_F30 = 0xffdb;
static const unsigned int XK_R10 = 0xffdb;
static const unsigned int XK_F31 = 0xffdc;
static const unsigned int XK_R11 = 0xffdc;
static const unsigned int XK_F32 = 0xffdd;
static const unsigned int XK_R12 = 0xffdd;
static const unsigned int XK_F33 = 0xffde;
static const unsigned int XK_R13 = 0xffde;
static const unsigned int XK_F34 = 0xffdf;
static const unsigned int XK_R14 = 0xffdf;
static const unsigned int XK_F35 = 0xffe0;
static const unsigned int XK_R15 = 0xffe0;

/* Modifiers */

static const unsigned int XK_Shift_L = 0xffe1;    /* Left shift */
static const unsigned int XK_Shift_R = 0xffe2;    /* Right shift */
static const unsigned int XK_Control_L = 0xffe3;  /* Left control */
static const unsigned int XK_Control_R = 0xffe4;  /* Right control */
static const unsigned int XK_Caps_Lock = 0xffe5;  /* Caps lock */
static const unsigned int XK_Shift_Lock = 0xffe6; /* Shift lock */

static const unsigned int XK_Meta_L = 0xffe7;  /* Left meta */
static const unsigned int XK_Meta_R = 0xffe8;  /* Right meta */
static const unsigned int XK_Alt_L = 0xffe9;   /* Left alt */
static const unsigned int XK_Alt_R = 0xffea;   /* Right alt */
static const unsigned int XK_Super_L = 0xffeb; /* Left super */
static const unsigned int XK_Super_R = 0xffec; /* Right super */
static const unsigned int XK_Hyper_L = 0xffed; /* Left hyper */
static const unsigned int XK_Hyper_R = 0xffee; /* Right hyper */

/*
 * Keyboard (XKB) Extension function and modifier keys
 * (from Appendix C of "The X Keyboard Extension: Protocol Specification")
 * Byte 3 = 0xfe
 */

static const unsigned int XK_ISO_Lock = 0xfe01;
static const unsigned int XK_ISO_Level2_Latch = 0xfe02;
static const unsigned int XK_ISO_Level3_Shift = 0xfe03;
static const unsigned int XK_ISO_Level3_Latch = 0xfe04;
static const unsigned int XK_ISO_Level3_Lock = 0xfe05;
static const unsigned int XK_ISO_Level5_Shift = 0xfe11;
static const unsigned int XK_ISO_Level5_Latch = 0xfe12;
static const unsigned int XK_ISO_Level5_Lock = 0xfe13;
static const unsigned int XK_ISO_Group_Shift = 0xff7e; /* Alias for mode_switch */
static const unsigned int XK_ISO_Group_Latch = 0xfe06;
static const unsigned int XK_ISO_Group_Lock = 0xfe07;
static const unsigned int XK_ISO_Next_Group = 0xfe08;
static const unsigned int XK_ISO_Next_Group_Lock = 0xfe09;
static const unsigned int XK_ISO_Prev_Group = 0xfe0a;
static const unsigned int XK_ISO_Prev_Group_Lock = 0xfe0b;
static const unsigned int XK_ISO_First_Group = 0xfe0c;
static const unsigned int XK_ISO_First_Group_Lock = 0xfe0d;
static const unsigned int XK_ISO_Last_Group = 0xfe0e;
static const unsigned int XK_ISO_Last_Group_Lock = 0xfe0f;

static const unsigned int XK_ISO_Left_Tab = 0xfe20;
static const unsigned int XK_ISO_Move_Line_Up = 0xfe21;
static const unsigned int XK_ISO_Move_Line_Down = 0xfe22;
static const unsigned int XK_ISO_Partial_Line_Up = 0xfe23;
static const unsigned int XK_ISO_Partial_Line_Down = 0xfe24;
static const unsigned int XK_ISO_Partial_Space_Left = 0xfe25;
static const unsigned int XK_ISO_Partial_Space_Right = 0xfe26;
static const unsigned int XK_ISO_Set_Margin_Left = 0xfe27;
static const unsigned int XK_ISO_Set_Margin_Right = 0xfe28;
static const unsigned int XK_ISO_Release_Margin_Left = 0xfe29;
static const unsigned int XK_ISO_Release_Margin_Right = 0xfe2a;
static const unsigned int XK_ISO_Release_Both_Margins = 0xfe2b;
static const unsigned int XK_ISO_Fast_Cursor_Left = 0xfe2c;
static const unsigned int XK_ISO_Fast_Cursor_Right = 0xfe2d;
static const unsigned int XK_ISO_Fast_Cursor_Up = 0xfe2e;
static const unsigned int XK_ISO_Fast_Cursor_Down = 0xfe2f;
static const unsigned int XK_ISO_Continuous_Underline = 0xfe30;
static const unsigned int XK_ISO_Discontinuous_Underline = 0xfe31;
static const unsigned int XK_ISO_Emphasize = 0xfe32;
static const unsigned int XK_ISO_Center_Object = 0xfe33;
static const unsigned int XK_ISO_Enter = 0xfe34;

static const unsigned int XK_dead_grave = 0xfe50;
static const unsigned int XK_dead_acute = 0xfe51;
static const unsigned int XK_dead_circumflex = 0xfe52;
static const unsigned int XK_dead_tilde = 0xfe53;
static const unsigned int XK_dead_perispomeni = 0xfe53; /* alias for dead_tilde */
static const unsigned int XK_dead_macron = 0xfe54;
static const unsigned int XK_dead_breve = 0xfe55;
static const unsigned int XK_dead_abovedot = 0xfe56;
static const unsigned int XK_dead_diaeresis = 0xfe57;
static const unsigned int XK_dead_abovering = 0xfe58;
static const unsigned int XK_dead_doubleacute = 0xfe59;
static const unsigned int XK_dead_caron = 0xfe5a;
static const unsigned int XK_dead_cedilla = 0xfe5b;
static const unsigned int XK_dead_ogonek = 0xfe5c;
static const unsigned int XK_dead_iota = 0xfe5d;
static const unsigned int XK_dead_voiced_sound = 0xfe5e;
static const unsigned int XK_dead_semivoiced_sound = 0xfe5f;
static const unsigned int XK_dead_belowdot = 0xfe60;
static const unsigned int XK_dead_hook = 0xfe61;
static const unsigned int XK_dead_horn = 0xfe62;
static const unsigned int XK_dead_stroke = 0xfe63;
static const unsigned int XK_dead_abovecomma = 0xfe64;
static const unsigned int XK_dead_psili = 0xfe64; /* alias for dead_abovecomma */
static const unsigned int XK_dead_abovereversedcomma = 0xfe65;
static const unsigned int XK_dead_dasia = 0xfe65; /* alias for dead_abovereversedcomma */
static const unsigned int XK_dead_doublegrave = 0xfe66;
static const unsigned int XK_dead_belowring = 0xfe67;
static const unsigned int XK_dead_belowmacron = 0xfe68;
static const unsigned int XK_dead_belowcircumflex = 0xfe69;
static const unsigned int XK_dead_belowtilde = 0xfe6a;
static const unsigned int XK_dead_belowbreve = 0xfe6b;
static const unsigned int XK_dead_belowdiaeresis = 0xfe6c;
static const unsigned int XK_dead_invertedbreve = 0xfe6d;
static const unsigned int XK_dead_belowcomma = 0xfe6e;
static const unsigned int XK_dead_currency = 0xfe6f;

/* extra dead elements for German T3 layout */
static const unsigned int XK_dead_lowline = 0xfe90;
static const unsigned int XK_dead_aboveverticalline = 0xfe91;
static const unsigned int XK_dead_belowverticalline = 0xfe92;
static const unsigned int XK_dead_longsolidusoverlay = 0xfe93;

/* dead vowels for universal syllable entry */
static const unsigned int XK_dead_a = 0xfe80;
static const unsigned int XK_dead_A = 0xfe81;
static const unsigned int XK_dead_e = 0xfe82;
static const unsigned int XK_dead_E = 0xfe83;
static const unsigned int XK_dead_i = 0xfe84;
static const unsigned int XK_dead_I = 0xfe85;
static const unsigned int XK_dead_o = 0xfe86;
static const unsigned int XK_dead_O = 0xfe87;
static const unsigned int XK_dead_u = 0xfe88;
static const unsigned int XK_dead_U = 0xfe89;
static const unsigned int XK_dead_small_schwa = 0xfe8a;
static const unsigned int XK_dead_capital_schwa = 0xfe8b;

static const unsigned int XK_dead_greek = 0xfe8c;

static const unsigned int XK_First_Virtual_Screen = 0xfed0;
static const unsigned int XK_Prev_Virtual_Screen = 0xfed1;
static const unsigned int XK_Next_Virtual_Screen = 0xfed2;
static const unsigned int XK_Last_Virtual_Screen = 0xfed4;
static const unsigned int XK_Terminate_Server = 0xfed5;

static const unsigned int XK_AccessX_Enable = 0xfe70;
static const unsigned int XK_AccessX_Feedback_Enable = 0xfe71;
static const unsigned int XK_RepeatKeys_Enable = 0xfe72;
static const unsigned int XK_SlowKeys_Enable = 0xfe73;
static const unsigned int XK_BounceKeys_Enable = 0xfe74;
static const unsigned int XK_StickyKeys_Enable = 0xfe75;
static const unsigned int XK_MouseKeys_Enable = 0xfe76;
static const unsigned int XK_MouseKeys_Accel_Enable = 0xfe77;
static const unsigned int XK_Overlay1_Enable = 0xfe78;
static const unsigned int XK_Overlay2_Enable = 0xfe79;
static const unsigned int XK_AudibleBell_Enable = 0xfe7a;

static const unsigned int XK_Pointer_Left = 0xfee0;
static const unsigned int XK_Pointer_Right = 0xfee1;
static const unsigned int XK_Pointer_Up = 0xfee2;
static const unsigned int XK_Pointer_Down = 0xfee3;
static const unsigned int XK_Pointer_UpLeft = 0xfee4;
static const unsigned int XK_Pointer_UpRight = 0xfee5;
static const unsigned int XK_Pointer_DownLeft = 0xfee6;
static const unsigned int XK_Pointer_DownRight = 0xfee7;
static const unsigned int XK_Pointer_Button_Dflt = 0xfee8;
static const unsigned int XK_Pointer_Button1 = 0xfee9;
static const unsigned int XK_Pointer_Button2 = 0xfeea;
static const unsigned int XK_Pointer_Button3 = 0xfeeb;
static const unsigned int XK_Pointer_Button4 = 0xfeec;
static const unsigned int XK_Pointer_Button5 = 0xfeed;
static const unsigned int XK_Pointer_DblClick_Dflt = 0xfeee;
static const unsigned int XK_Pointer_DblClick1 = 0xfeef;
static const unsigned int XK_Pointer_DblClick2 = 0xfef0;
static const unsigned int XK_Pointer_DblClick3 = 0xfef1;
static const unsigned int XK_Pointer_DblClick4 = 0xfef2;
static const unsigned int XK_Pointer_DblClick5 = 0xfef3;
static const unsigned int XK_Pointer_Drag_Dflt = 0xfef4;
static const unsigned int XK_Pointer_Drag1 = 0xfef5;
static const unsigned int XK_Pointer_Drag2 = 0xfef6;
static const unsigned int XK_Pointer_Drag3 = 0xfef7;
static const unsigned int XK_Pointer_Drag4 = 0xfef8;
static const unsigned int XK_Pointer_Drag5 = 0xfefd;

static const unsigned int XK_Pointer_EnableKeys = 0xfef9;
static const unsigned int XK_Pointer_Accelerate = 0xfefa;
static const unsigned int XK_Pointer_DfltBtnNext = 0xfefb;
static const unsigned int XK_Pointer_DfltBtnPrev = 0xfefc;

/* Single-Stroke Multiple-Character N-Graph Keysyms For The X Input Method */

static const unsigned int XK_ch = 0xfea0;
static const unsigned int XK_Ch = 0xfea1;
static const unsigned int XK_CH = 0xfea2;
static const unsigned int XK_c_h = 0xfea3;
static const unsigned int XK_C_h = 0xfea4;
static const unsigned int XK_C_H = 0xfea5;

/*
 * 3270 Terminal Keys
 * Byte 3 = 0xfd
 */

static const unsigned int XK_3270_Duplicate = 0xfd01;
static const unsigned int XK_3270_FieldMark = 0xfd02;
static const unsigned int XK_3270_Right2 = 0xfd03;
static const unsigned int XK_3270_Left2 = 0xfd04;
static const unsigned int XK_3270_BackTab = 0xfd05;
static const unsigned int XK_3270_EraseEOF = 0xfd06;
static const unsigned int XK_3270_EraseInput = 0xfd07;
static const unsigned int XK_3270_Reset = 0xfd08;
static const unsigned int XK_3270_Quit = 0xfd09;
static const unsigned int XK_3270_PA1 = 0xfd0a;
static const unsigned int XK_3270_PA2 = 0xfd0b;
static const unsigned int XK_3270_PA3 = 0xfd0c;
static const unsigned int XK_3270_Test = 0xfd0d;
static const unsigned int XK_3270_Attn = 0xfd0e;
static const unsigned int XK_3270_CursorBlink = 0xfd0f;
static const unsigned int XK_3270_AltCursor = 0xfd10;
static const unsigned int XK_3270_KeyClick = 0xfd11;
static const unsigned int XK_3270_Jump = 0xfd12;
static const unsigned int XK_3270_Ident = 0xfd13;
static const unsigned int XK_3270_Rule = 0xfd14;
static const unsigned int XK_3270_Copy = 0xfd15;
static const unsigned int XK_3270_Play = 0xfd16;
static const unsigned int XK_3270_Setup = 0xfd17;
static const unsigned int XK_3270_Record = 0xfd18;
static const unsigned int XK_3270_ChangeScreen = 0xfd19;
static const unsigned int XK_3270_DeleteWord = 0xfd1a;
static const unsigned int XK_3270_ExSelect = 0xfd1b;
static const unsigned int XK_3270_CursorSelect = 0xfd1c;
static const unsigned int XK_3270_PrintScreen = 0xfd1d;
static const unsigned int XK_3270_Enter = 0xfd1e;

/*
 * Latin 1
 * (ISO/IEC 8859-1 = Unicode U+0020..U+00FF)
 * Byte 3 = 0
 */
static const unsigned int XK_space = 0x0020;        /* U+0020 SPACE */
static const unsigned int XK_exclam = 0x0021;       /* U+0021 EXCLAMATION MARK */
static const unsigned int XK_quotedbl = 0x0022;     /* U+0022 QUOTATION MARK */
static const unsigned int XK_numbersign = 0x0023;   /* U+0023 NUMBER SIGN */
static const unsigned int XK_dollar = 0x0024;       /* U+0024 DOLLAR SIGN */
static const unsigned int XK_percent = 0x0025;      /* U+0025 PERCENT SIGN */
static const unsigned int XK_ampersand = 0x0026;    /* U+0026 AMPERSAND */
static const unsigned int XK_apostrophe = 0x0027;   /* U+0027 APOSTROPHE */
static const unsigned int XK_quoteright = 0x0027;   /* deprecated */
static const unsigned int XK_parenleft = 0x0028;    /* U+0028 LEFT PARENTHESIS */
static const unsigned int XK_parenright = 0x0029;   /* U+0029 RIGHT PARENTHESIS */
static const unsigned int XK_asterisk = 0x002a;     /* U+002A ASTERISK */
static const unsigned int XK_plus = 0x002b;         /* U+002B PLUS SIGN */
static const unsigned int XK_comma = 0x002c;        /* U+002C COMMA */
static const unsigned int XK_minus = 0x002d;        /* U+002D HYPHEN-MINUS */
static const unsigned int XK_period = 0x002e;       /* U+002E FULL STOP */
static const unsigned int XK_slash = 0x002f;        /* U+002F SOLIDUS */
static const unsigned int XK_0 = 0x0030;            /* U+0030 DIGIT ZERO */
static const unsigned int XK_1 = 0x0031;            /* U+0031 DIGIT ONE */
static const unsigned int XK_2 = 0x0032;            /* U+0032 DIGIT TWO */
static const unsigned int XK_3 = 0x0033;            /* U+0033 DIGIT THREE */
static const unsigned int XK_4 = 0x0034;            /* U+0034 DIGIT FOUR */
static const unsigned int XK_5 = 0x0035;            /* U+0035 DIGIT FIVE */
static const unsigned int XK_6 = 0x0036;            /* U+0036 DIGIT SIX */
static const unsigned int XK_7 = 0x0037;            /* U+0037 DIGIT SEVEN */
static const unsigned int XK_8 = 0x0038;            /* U+0038 DIGIT EIGHT */
static const unsigned int XK_9 = 0x0039;            /* U+0039 DIGIT NINE */
static const unsigned int XK_colon = 0x003a;        /* U+003A COLON */
static const unsigned int XK_semicolon = 0x003b;    /* U+003B SEMICOLON */
static const unsigned int XK_less = 0x003c;         /* U+003C LESS-THAN SIGN */
static const unsigned int XK_equal = 0x003d;        /* U+003D EQUALS SIGN */
static const unsigned int XK_greater = 0x003e;      /* U+003E GREATER-THAN SIGN */
static const unsigned int XK_question = 0x003f;     /* U+003F QUESTION MARK */
static const unsigned int XK_at = 0x0040;           /* U+0040 COMMERCIAL AT */
static const unsigned int XK_A = 0x0041;            /* U+0041 LATIN CAPITAL LETTER A */
static const unsigned int XK_B = 0x0042;            /* U+0042 LATIN CAPITAL LETTER B */
static const unsigned int XK_C = 0x0043;            /* U+0043 LATIN CAPITAL LETTER C */
static const unsigned int XK_D = 0x0044;            /* U+0044 LATIN CAPITAL LETTER D */
static const unsigned int XK_E = 0x0045;            /* U+0045 LATIN CAPITAL LETTER E */
static const unsigned int XK_F = 0x0046;            /* U+0046 LATIN CAPITAL LETTER F */
static const unsigned int XK_G = 0x0047;            /* U+0047 LATIN CAPITAL LETTER G */
static const unsigned int XK_H = 0x0048;            /* U+0048 LATIN CAPITAL LETTER H */
static const unsigned int XK_I = 0x0049;            /* U+0049 LATIN CAPITAL LETTER I */
static const unsigned int XK_J = 0x004a;            /* U+004A LATIN CAPITAL LETTER J */
static const unsigned int XK_K = 0x004b;            /* U+004B LATIN CAPITAL LETTER K */
static const unsigned int XK_L = 0x004c;            /* U+004C LATIN CAPITAL LETTER L */
static const unsigned int XK_M = 0x004d;            /* U+004D LATIN CAPITAL LETTER M */
static const unsigned int XK_N = 0x004e;            /* U+004E LATIN CAPITAL LETTER N */
static const unsigned int XK_O = 0x004f;            /* U+004F LATIN CAPITAL LETTER O */
static const unsigned int XK_P = 0x0050;            /* U+0050 LATIN CAPITAL LETTER P */
static const unsigned int XK_Q = 0x0051;            /* U+0051 LATIN CAPITAL LETTER Q */
static const unsigned int XK_R = 0x0052;            /* U+0052 LATIN CAPITAL LETTER R */
static const unsigned int XK_S = 0x0053;            /* U+0053 LATIN CAPITAL LETTER S */
static const unsigned int XK_T = 0x0054;            /* U+0054 LATIN CAPITAL LETTER T */
static const unsigned int XK_U = 0x0055;            /* U+0055 LATIN CAPITAL LETTER U */
static const unsigned int XK_V = 0x0056;            /* U+0056 LATIN CAPITAL LETTER V */
static const unsigned int XK_W = 0x0057;            /* U+0057 LATIN CAPITAL LETTER W */
static const unsigned int XK_X = 0x0058;            /* U+0058 LATIN CAPITAL LETTER X */
static const unsigned int XK_Y = 0x0059;            /* U+0059 LATIN CAPITAL LETTER Y */
static const unsigned int XK_Z = 0x005a;            /* U+005A LATIN CAPITAL LETTER Z */
static const unsigned int XK_bracketleft = 0x005b;  /* U+005B LEFT SQUARE BRACKET */
static const unsigned int XK_backslash = 0x005c;    /* U+005C REVERSE SOLIDUS */
static const unsigned int XK_bracketright = 0x005d; /* U+005D RIGHT SQUARE BRACKET */
static const unsigned int XK_asciicircum = 0x005e;  /* U+005E CIRCUMFLEX ACCENT */
static const unsigned int XK_underscore = 0x005f;   /* U+005F LOW LINE */
static const unsigned int XK_grave = 0x0060;        /* U+0060 GRAVE ACCENT */
static const unsigned int XK_quoteleft = 0x0060;    /* deprecated */
static const unsigned int XK_a = 0x0061;            /* U+0061 LATIN SMALL LETTER A */
static const unsigned int XK_b = 0x0062;            /* U+0062 LATIN SMALL LETTER B */
static const unsigned int XK_c = 0x0063;            /* U+0063 LATIN SMALL LETTER C */
static const unsigned int XK_d = 0x0064;            /* U+0064 LATIN SMALL LETTER D */
static const unsigned int XK_e = 0x0065;            /* U+0065 LATIN SMALL LETTER E */
static const unsigned int XK_f = 0x0066;            /* U+0066 LATIN SMALL LETTER F */
static const unsigned int XK_g = 0x0067;            /* U+0067 LATIN SMALL LETTER G */
static const unsigned int XK_h = 0x0068;            /* U+0068 LATIN SMALL LETTER H */
static const unsigned int XK_i = 0x0069;            /* U+0069 LATIN SMALL LETTER I */
static const unsigned int XK_j = 0x006a;            /* U+006A LATIN SMALL LETTER J */
static const unsigned int XK_k = 0x006b;            /* U+006B LATIN SMALL LETTER K */
static const unsigned int XK_l = 0x006c;            /* U+006C LATIN SMALL LETTER L */
static const unsigned int XK_m = 0x006d;            /* U+006D LATIN SMALL LETTER M */
static const unsigned int XK_n = 0x006e;            /* U+006E LATIN SMALL LETTER N */
static const unsigned int XK_o = 0x006f;            /* U+006F LATIN SMALL LETTER O */
static const unsigned int XK_p = 0x0070;            /* U+0070 LATIN SMALL LETTER P */
static const unsigned int XK_q = 0x0071;            /* U+0071 LATIN SMALL LETTER Q */
static const unsigned int XK_r = 0x0072;            /* U+0072 LATIN SMALL LETTER R */
static const unsigned int XK_s = 0x0073;            /* U+0073 LATIN SMALL LETTER S */
static const unsigned int XK_t = 0x0074;            /* U+0074 LATIN SMALL LETTER T */
static const unsigned int XK_u = 0x0075;            /* U+0075 LATIN SMALL LETTER U */
static const unsigned int XK_v = 0x0076;            /* U+0076 LATIN SMALL LETTER V */
static const unsigned int XK_w = 0x0077;            /* U+0077 LATIN SMALL LETTER W */
static const unsigned int XK_x = 0x0078;            /* U+0078 LATIN SMALL LETTER X */
static const unsigned int XK_y = 0x0079;            /* U+0079 LATIN SMALL LETTER Y */
static const unsigned int XK_z = 0x007a;            /* U+007A LATIN SMALL LETTER Z */
static const unsigned int XK_braceleft = 0x007b;    /* U+007B LEFT CURLY BRACKET */
static const unsigned int XK_bar = 0x007c;          /* U+007C VERTICAL LINE */
static const unsigned int XK_braceright = 0x007d;   /* U+007D RIGHT CURLY BRACKET */
static const unsigned int XK_asciitilde = 0x007e;   /* U+007E TILDE */

static const unsigned int XK_nobreakspace = 0x00a0; /* U+00A0 NO-BREAK SPACE */
static const unsigned int XK_exclamdown = 0x00a1;   /* U+00A1 INVERTED EXCLAMATION MARK */
static const unsigned int XK_cent = 0x00a2;         /* U+00A2 CENT SIGN */
static const unsigned int XK_sterling = 0x00a3;     /* U+00A3 POUND SIGN */
static const unsigned int XK_currency = 0x00a4;     /* U+00A4 CURRENCY SIGN */
static const unsigned int XK_yen = 0x00a5;          /* U+00A5 YEN SIGN */
static const unsigned int XK_brokenbar = 0x00a6;    /* U+00A6 BROKEN BAR */
static const unsigned int XK_section = 0x00a7;      /* U+00A7 SECTION SIGN */
static const unsigned int XK_diaeresis = 0x00a8;    /* U+00A8 DIAERESIS */
static const unsigned int XK_copyright = 0x00a9;    /* U+00A9 COPYRIGHT SIGN */
static const unsigned int XK_ordfeminine = 0x00aa;  /* U+00AA FEMININE ORDINAL INDICATOR */
static const unsigned int XK_guillemotleft = 
    0x00ab;                /* = U;+00AB LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
static const unsigned int XK_notsign = 0x00ac; /* U+00AC NOT SIGN */
static const unsigned int XK_hyphen = 0x00ad;  /* U+00AD SOFT HYPHEN */
static const unsigned int XK_registered = 0x00ae;     /* U+00AE REGISTERED SIGN */
static const unsigned int XK_macron = 0x00af;         /* U+00AF MACRON */
static const unsigned int XK_degree = 0x00b0;         /* U+00B0 DEGREE SIGN */
static const unsigned int XK_plusminus = 0x00b1;      /* U+00B1 PLUS-MINUS SIGN */
static const unsigned int XK_twosuperior = 0x00b2;    /* U+00B2 SUPERSCRIPT TWO */
static const unsigned int XK_threesuperior = 0x00b3;  /* U+00B3 SUPERSCRIPT THREE */
static const unsigned int XK_acute = 0x00b4;          /* U+00B4 ACUTE ACCENT */
static const unsigned int XK_mu = 0x00b5;             /* U+00B5 MICRO SIGN */
static const unsigned int XK_paragraph = 0x00b6;      /* U+00B6 PILCROW SIGN */
static const unsigned int XK_periodcentered = 0x00b7; /* U+00B7 MIDDLE DOT */
static const unsigned int XK_cedilla = 0x00b8;        /* U+00B8 CEDILLA */
static const unsigned int XK_onesuperior = 0x00b9;    /* U+00B9 SUPERSCRIPT ONE */
static const unsigned int XK_masculine = 0x00ba;      /* U+00BA MASCULINE ORDINAL INDICATOR */
static const unsigned int XK_guillemotright                                                       
    = 0x00bb; /* U+00BB RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
static const unsigned int XK_onequarter = 0x00bc;    /* U+00BC VULGAR FRACTION ONE QUARTER */
static const unsigned int XK_onehalf = 0x00bd;       /* U+00BD VULGAR FRACTION ONE HALF */
static const unsigned int XK_threequarters = 0x00be; /* U+00BE VULGAR FRACTION THREE QUARTERS */
static const unsigned int XK_questiondown = 0x00bf;  /* U+00BF INVERTED QUESTION MARK */
static const unsigned int XK_Agrave = 0x00c0;        /* U+00C0 LATIN CAPITAL LETTER A WITH GRAVE */
static const unsigned int XK_Aacute = 0x00c1;        /* U+00C1 LATIN CAPITAL LETTER A WITH ACUTE */
static const unsigned int XK_Acircumflex                                                         \
    = 0x00c2;                   /* U+00C2 LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
static const unsigned int XK_Atilde = 0x00c3;     /* U+00C3 LATIN CAPITAL LETTER A WITH TILDE */
static const unsigned int XK_Adiaeresis = 0x00c4; /* U+00C4 LATIN CAPITAL LETTER A WITH DIAERESIS   \
                                */
static const unsigned int XK_Aring = 0x00c5;      /* U+00C5 LATIN CAPITAL LETTER A WITH RING ABOVE */
static const unsigned int XK_AE = 0x00c6;         /* U+00C6 LATIN CAPITAL LETTER AE */
static const unsigned int XK_Ccedilla = 0x00c7;   /* U+00C7 LATIN CAPITAL LETTER C WITH CEDILLA */
static const unsigned int XK_Egrave = 0x00c8;     /* U+00C8 LATIN CAPITAL LETTER E WITH GRAVE */
static const unsigned int XK_Eacute = 0x00c9;     /* U+00C9 LATIN CAPITAL LETTER E WITH ACUTE */
static const unsigned int XK_Ecircumflex                                                         \
    = 0x00ca;                   /* U+00CA LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
static const unsigned int XK_Ediaeresis = 0x00cb; /* U+00CB LATIN CAPITAL LETTER E WITH DIAERESIS   \
                                */
static const unsigned int XK_Igrave = 0x00cc;     /* U+00CC LATIN CAPITAL LETTER I WITH GRAVE */
static const unsigned int XK_Iacute = 0x00cd;     /* U+00CD LATIN CAPITAL LETTER I WITH ACUTE */
static const unsigned int XK_Icircumflex                                                         \
    = 0x00ce;                   /* U+00CE LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
static const unsigned int XK_Idiaeresis = 0x00cf; /* U+00CF LATIN CAPITAL LETTER I WITH DIAERESIS   \
                                */
static const unsigned int XK_ETH = 0x00d0;        /* U+00D0 LATIN CAPITAL LETTER ETH */
static const unsigned int XK_Eth = 0x00d0;        /* deprecated */
static const unsigned int XK_Ntilde = 0x00d1;     /* U+00D1 LATIN CAPITAL LETTER N WITH TILDE */
static const unsigned int XK_Ograve = 0x00d2;     /* U+00D2 LATIN CAPITAL LETTER O WITH GRAVE */
static const unsigned int XK_Oacute = 0x00d3;     /* U+00D3 LATIN CAPITAL LETTER O WITH ACUTE */
static const unsigned int XK_Ocircumflex                                                         \
    = 0x00d4;                   /* U+00D4 LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
static const unsigned int XK_Otilde = 0x00d5;     /* U+00D5 LATIN CAPITAL LETTER O WITH TILDE */
static const unsigned int XK_Odiaeresis = 0x00d6; /* U+00D6 LATIN CAPITAL LETTER O WITH DIAERESIS   \
                                */
static const unsigned int XK_multiply = 0x00d7;   /* U+00D7 MULTIPLICATION SIGN */
static const unsigned int XK_Oslash = 0x00d8;     /* U+00D8 LATIN CAPITAL LETTER O WITH STROKE */
static const unsigned int XK_Ooblique = 0x00d8;   /* U+00D8 LATIN CAPITAL LETTER O WITH STROKE */
static const unsigned int XK_Ugrave = 0x00d9;     /* U+00D9 LATIN CAPITAL LETTER U WITH GRAVE */
static const unsigned int XK_Uacute = 0x00da;     /* U+00DA LATIN CAPITAL LETTER U WITH ACUTE */
static const unsigned int XK_Ucircumflex                                                         \
    = 0x00db;                   /* U+00DB LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
static const unsigned int XK_Udiaeresis = 0x00dc; /* U+00DC LATIN CAPITAL LETTER U WITH DIAERESIS   \
                                */
static const unsigned int XK_Yacute = 0x00dd;     /* U+00DD LATIN CAPITAL LETTER Y WITH ACUTE */
static const unsigned int XK_THORN = 0x00de;      /* U+00DE LATIN CAPITAL LETTER THORN */
static const unsigned int XK_Thorn = 0x00de;      /* deprecated */
static const unsigned int XK_ssharp = 0x00df;     /* U+00DF LATIN SMALL LETTER SHARP S */
static const unsigned int XK_agrave = 0x00e0;     /* U+00E0 LATIN SMALL LETTER A WITH GRAVE */
static const unsigned int XK_aacute = 0x00e1;     /* U+00E1 LATIN SMALL LETTER A WITH ACUTE */
static const unsigned int XK_acircumflex = 0x00e2; /* U+00E2 LATIN SMALL LETTER A WITH CIRCUMFLEX   \
                                 */
static const unsigned int XK_atilde = 0x00e3;      /* U+00E3 LATIN SMALL LETTER A WITH TILDE */
static const unsigned int XK_adiaeresis = 0x00e4;  /* U+00E4 LATIN SMALL LETTER A WITH DIAERESIS */
static const unsigned int XK_aring = 0x00e5;       /* U+00E5 LATIN SMALL LETTER A WITH RING ABOVE */
static const unsigned int XK_ae = 0x00e6;          /* U+00E6 LATIN SMALL LETTER AE */
static const unsigned int XK_ccedilla = 0x00e7;    /* U+00E7 LATIN SMALL LETTER C WITH CEDILLA */
static const unsigned int XK_egrave = 0x00e8;      /* U+00E8 LATIN SMALL LETTER E WITH GRAVE */
static const unsigned int XK_eacute = 0x00e9;      /* U+00E9 LATIN SMALL LETTER E WITH ACUTE */
static const unsigned int XK_ecircumflex = 0x00ea; /* U+00EA LATIN SMALL LETTER E WITH CIRCUMFLEX   \
                                 */
static const unsigned int XK_ediaeresis = 0x00eb;  /* U+00EB LATIN SMALL LETTER E WITH DIAERESIS */
static const unsigned int XK_igrave = 0x00ec;      /* U+00EC LATIN SMALL LETTER I WITH GRAVE */
static const unsigned int XK_iacute = 0x00ed;      /* U+00ED LATIN SMALL LETTER I WITH ACUTE */
static const unsigned int XK_icircumflex = 0x00ee; /* U+00EE LATIN SMALL LETTER I WITH CIRCUMFLEX   \
                                 */
static const unsigned int XK_idiaeresis = 0x00ef;  /* U+00EF LATIN SMALL LETTER I WITH DIAERESIS */
static const unsigned int XK_eth = 0x00f0;         /* U+00F0 LATIN SMALL LETTER ETH */
static const unsigned int XK_ntilde = 0x00f1;      /* U+00F1 LATIN SMALL LETTER N WITH TILDE */
static const unsigned int XK_ograve = 0x00f2;      /* U+00F2 LATIN SMALL LETTER O WITH GRAVE */
static const unsigned int XK_oacute = 0x00f3;      /* U+00F3 LATIN SMALL LETTER O WITH ACUTE */
static const unsigned int XK_ocircumflex = 0x00f4; /* U+00F4 LATIN SMALL LETTER O WITH CIRCUMFLEX   \
                                 */
static const unsigned int XK_otilde = 0x00f5;      /* U+00F5 LATIN SMALL LETTER O WITH TILDE */
static const unsigned int XK_odiaeresis = 0x00f6;  /* U+00F6 LATIN SMALL LETTER O WITH DIAERESIS */
static const unsigned int XK_division = 0x00f7;    /* U+00F7 DIVISION SIGN */
static const unsigned int XK_oslash = 0x00f8;      /* U+00F8 LATIN SMALL LETTER O WITH STROKE */
static const unsigned int XK_ooblique = 0x00f8;    /* U+00F8 LATIN SMALL LETTER O WITH STROKE */
static const unsigned int XK_ugrave = 0x00f9;      /* U+00F9 LATIN SMALL LETTER U WITH GRAVE */
static const unsigned int XK_uacute = 0x00fa;      /* U+00FA LATIN SMALL LETTER U WITH ACUTE */
static const unsigned int XK_ucircumflex = 0x00fb; /* U+00FB LATIN SMALL LETTER U WITH CIRCUMFLEX   \
                                 */
static const unsigned int XK_udiaeresis = 0x00fc;  /* U+00FC LATIN SMALL LETTER U WITH DIAERESIS */
static const unsigned int XK_yacute = 0x00fd;      /* U+00FD LATIN SMALL LETTER Y WITH ACUTE */
static const unsigned int XK_thorn = 0x00fe;       /* U+00FE LATIN SMALL LETTER THORN */
static const unsigned int XK_ydiaeresis = 0x00ff;  /* U+00FF LATIN SMALL LETTER Y WITH DIAERESIS */

]]

-- Only the western characters (default ones) are included here. This file can 
-- be expanded at will, but was kept short to improve load times.

return ffi.C

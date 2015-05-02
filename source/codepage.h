/*
 * codepage.h
 *
 * This module is licensed under the GNU General Public License
 * Version 2.  Please see the file "COPYING" in this directory for
 * more information about the GNU General Public License Version 2.
 *
 *     Copyright (C) 2015  Kevin Lamonte
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef __CODEPAGE_H__
#define __CODEPAGE_H__

/* Includes --------------------------------------------------------------- */

#include <stddef.h>             /* wchar_t */
#include <stdint.h>             /* uint32_t */
#include "qcurses.h"            /* attr_t */

/* Defines ---------------------------------------------------------------- */

/* Control characters used in lots of places */
#define C_NUL   0x00
#define C_SOH   0x01
#define C_STX   0x02
#define C_EOT   0x04
#define C_ACK   0x06
#define C_LF    0x0A
#define C_CR    0x0D
#define C_XON   0x11            /* DC1 */
#define C_XOFF  0x13            /* DC3 */
#define C_NAK   0x15
#define C_CAN   0x18
#define C_SUB   0x1A
#define C_ESC   0x1B

/* Globals ---------------------------------------------------------------- */

/* CP437 characters!  Yee! */
/*
 * I've been using the medium-density hatch forever, but I think it's actually
 * the low-density hatch that Qmodem used.
 */
/* #define HATCH                        0xB1 */
#define HATCH                   0xB0
#define DOUBLE_BAR              0xCD
#define BOX                     0xFE
#define CHECK                   0xFB
#define TRIPLET                 0xF0
#define OMEGA                   0xEA
#define PI                      0xE3
#define UPARROW                 0x18
#define DOWNARROW               0x19
#define RIGHTARROW              0x1A
#define LEFTARROW               0x1B
#define SINGLE_BAR              0xC4
#define BACK_ARROWHEAD          0x11
#define LRCORNER                0xD9
#define DEGREE                  0xF8
#define PLUSMINUS               0xF1

#define Q_WINDOW_TOP            DOUBLE_BAR
#define Q_WINDOW_LEFT_TOP       0xD5
#define Q_WINDOW_RIGHT_TOP      0xB8
#define Q_WINDOW_SIDE           0xB3
#define Q_WINDOW_LEFT_BOTTOM    0xD4
#define Q_WINDOW_RIGHT_BOTTOM   0xBE
#define Q_WINDOW_LEFT_TEE       0xC6
#define Q_WINDOW_RIGHT_TEE      0xB5
#define Q_WINDOW_LEFT_TOP_DOUBLESIDE    0xD6
#define Q_WINDOW_RIGHT_TOP_DOUBLESIDE   0xB7

/* VT52 uses "blank" in the docs, so I figured I'll leave a spot for it. */
#define BLANK                   0x20

/* CP437 translation map */
extern wchar_t cp437_chars[256];

/* VT52 drawing characters */
extern wchar_t vt52_special_graphics_chars[128];

/* DEC VT100/VT220 translation maps --------------------------------------- */

/* US - Normal "international" (ASCII) */
extern wchar_t dec_us_chars[128];

/* VT100 drawing characters */
extern wchar_t dec_special_graphics_chars[128];

/* Dec Supplemental (DEC multinational) */
extern wchar_t dec_supplemental_chars[128];

/* UK */
extern wchar_t dec_uk_chars[128];

/* DUTCH */
extern wchar_t dec_nl_chars[128];

/* FINNISH */
extern wchar_t dec_fi_chars[128];

/* FRENCH */
extern wchar_t dec_fr_chars[128];

/* FRENCH_CA */
extern wchar_t dec_fr_CA_chars[128];

/* GERMAN */
extern wchar_t dec_de_chars[128];

/* ITALIAN */
extern wchar_t dec_it_chars[128];

/* NORWEGIAN */
extern wchar_t dec_no_chars[128];

/* SPANISH */
extern wchar_t dec_es_chars[128];

/* SWEDISH */
extern wchar_t dec_sv_chars[128];

/* SWISS */
extern wchar_t dec_swiss_chars[128];

typedef enum Q_CODEPAGES {
        Q_CODEPAGE_CP437,               /* PC VGA */
        Q_CODEPAGE_ISO8859_1,           /* ISO-8859-1 */
        Q_CODEPAGE_DEC,                 /* DEC character sets for VT10x/VT220 */

        /* DOS codepages */
        Q_CODEPAGE_CP720,               /* Arabic */
        Q_CODEPAGE_CP737,               /* Greek */
        Q_CODEPAGE_CP775,               /* Baltic Rim */
        Q_CODEPAGE_CP850,               /* Western European */
        Q_CODEPAGE_CP852,               /* Central European */
        Q_CODEPAGE_CP857,               /* Turkish */
        Q_CODEPAGE_CP858,               /* Western European with euro */
        Q_CODEPAGE_CP860,               /* Portuguese */
        Q_CODEPAGE_CP862,               /* Hebrew */
        Q_CODEPAGE_CP863,               /* Quebec French */
        Q_CODEPAGE_CP866,               /* Cyrillic */

        /* Windows codepages */
        Q_CODEPAGE_CP1250,              /* Central/Eastern European */
        Q_CODEPAGE_CP1251,              /* Cyrillic */
        Q_CODEPAGE_CP1252,              /* Western European */

        /* Other codepages */
        Q_CODEPAGE_KOI8_R,              /* Russian */
        Q_CODEPAGE_KOI8_U,              /* Ukrainian */

} Q_CODEPAGE;
#define Q_CODEPAGE_MAX (Q_CODEPAGE_KOI8_U + 1)

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

/* Functions -------------------------------------------------------------- */

extern int utf8_encode(const wchar_t ch, char * utf8_buffer);
extern uint32_t utf8_decode(uint32_t* state, uint32_t* codep, uint32_t byte);
extern char * codepage_string(Q_CODEPAGE codepage);
extern Q_CODEPAGE codepage_from_string(const char * string);
extern wchar_t codepage_map_char(const unsigned char ch);
extern void codepage_keyboard_handler(const int keystroke, const int flags);
extern void codepage_refresh();

#endif /* __CODEPAGE_H__ */

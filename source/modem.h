/*
 * modem.h
 *
 * This module is licensed under the GNU General Public License Version 2.
 * Please see the file "COPYING" in this directory for more information about
 * the GNU General Public License Version 2.
 *
 *     Copyright (C) 2015  Kevin Lamonte
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __MODEM_H__
#define __MODEM_H__

/* Includes --------------------------------------------------------------- */

#ifndef Q_NO_SERIAL
#include <termios.h>

#include "common.h"             /* Q_BOOL */

/* Defines ---------------------------------------------------------------- */

/*
 * I define these myself because I don't want to expose a struct termios to
 * the user.
 */
typedef enum {
    Q_BAUD_300,
    Q_BAUD_1200,
    Q_BAUD_2400,
    Q_BAUD_4800,
    Q_BAUD_9600,
    Q_BAUD_19200,
    Q_BAUD_38400,
    Q_BAUD_57600,
    Q_BAUD_115200,
    Q_BAUD_230400
} Q_BAUD_RATE;

typedef enum {
    Q_PARITY_NONE,
    Q_PARITY_EVEN,
    Q_PARITY_ODD,
    Q_PARITY_MARK,
    Q_PARITY_SPACE
} Q_PARITY;

typedef enum {
    Q_DATA_BITS_8,
    Q_DATA_BITS_7,
    Q_DATA_BITS_6,
    Q_DATA_BITS_5
} Q_DATA_BITS;

typedef enum {
    Q_STOP_BITS_1,
    Q_STOP_BITS_2
} Q_STOP_BITS;

/**
 * The pins on a 9-pin RS-232 connector.
 */
struct rs232_pins {
    Q_BOOL LE;
    Q_BOOL DTR;
    Q_BOOL RTS;
    Q_BOOL ST;
    Q_BOOL SR;
    Q_BOOL CTS;
    Q_BOOL DCD;
    Q_BOOL RI;
    Q_BOOL DSR;
};

/**
 * Modem configuration settings.
 */
struct q_modem_config_struct {
    Q_BOOL rtscts;              /* true = use RTS/CTS flow control */
    Q_BOOL xonxoff;             /* true = use XON/XOFF flow control */
    Q_BOOL lock_dte_baud;       /* true = lock DTE baud rate on connect */

    wchar_t * name;             /* "My Brand Foo Modem" */
    char * dev_name;            /* "/dev/modem" */
    char * lock_dir;            /* "/var/lock" */
    char * init_string;         /* "ATZ^M" */
    char * hangup_string;       /* "~+~+~+~~~ATH0^M" */
    char * dial_string;         /* "ATDT" */
    char * host_init_string;    /* "ATE1Q0V1M1H0S0=0^M" */
    char * answer_string;       /* "ATA^M" */

    Q_BAUD_RATE default_baud;
    Q_DATA_BITS default_data_bits;
    Q_STOP_BITS default_stop_bits;
    Q_PARITY default_parity;

};

/**
 * Serial port configuration.
 */
struct q_serial_port_struct {
    Q_BOOL rtscts;              /* true = use RTS/CTS flow control */
    Q_BOOL xonxoff;             /* true = use XON/XOFF flow control */
    Q_BOOL lock_dte_baud;       /* true = lock DTE baud rate on connect */

    Q_BAUD_RATE baud;
    Q_DATA_BITS data_bits;
    Q_STOP_BITS stop_bits;
    Q_PARITY parity;

#ifndef Q_PDCURSES_WIN32
    struct termios original_termios;    /* What was left on the port */
    struct termios qodem_termios;       /* What the user requests */
#endif /* Q_PDCURSES_WIN32 */

    /**
     * The state of the RS-232 pins.
     */
    struct rs232_pins rs232;

    /**
     * The DCE (modem <--> modem) baud rate.
     */
    int dce_baud;
};

/* DEBUG: No error correction, don't lock DTE port */
/*
#define MODEM_DEFAULT_INIT_STRING       "AT &F &B0 &H0&R1 &K0 &M0 E1 F1" \
                                        "Q0 V1 X4 &A3 &C1 &D2 &R2 &S0 ^M"
*/

/*
 * Normal case: use maximum error correction and compression, hardware flow
 * control, lock DTE port
 */
#define MODEM_DEFAULT_INIT_STRING       "AT &F &B1 &H1&R2 &K1 &M4 E1 F1" \
                                        "Q0 V1 X4 &A3 &C1 &D2 &R2 &S0 ^M"

#define MODEM_DEFAULT_HANGUP_STRING     "+~+~+~~~~ATH0^M"
#define MODEM_DEFAULT_DIAL_STRING       "ATDT"
#define MODEM_DEFAULT_HOST_INIT_STRING  "ATE1Q0V1M1H0S0=0^M"
#define MODEM_DEFAULT_ANSWER_STRING     "ATA^M"

#define MODEM_DEFAULT_NAME              L"The Modem"
#define MODEM_DEFAULT_DEVICE_NAME       "/dev/ttyS0"
#define MODEM_DEFAULT_LOCK_DIR          "/var/lock"

/* Globals ---------------------------------------------------------------- */

/**
 * The global modem configuration settings.
 */
extern struct q_modem_config_struct q_modem_config;

/**
 * The global serial port settings.
 */
extern struct q_serial_port_struct q_serial_port;

/* Functions -------------------------------------------------------------- */

/**
 * Keyboard handler for the Alt-O modem settigns dialog.
 *
 * @param keystroke the keystroke from the user.
 * @param flags KEY_FLAG_ALT, KEY_FLAG_CTRL, etc.  See input.h.
 */
extern void modem_config_keyboard_handler(const int keystroke, const int flags);

/**
 * Draw screen for the Alt-O modem settigns dialog.
 */
extern void modem_config_refresh();

/**
 * Create the config file for the modem (modem.cfg).
 */
extern void create_modem_config_file();

/**
 * Load the modem setting from the config file (modem.cfg).
 */
extern void load_modem_config();

/**
 * Open the serial port.
 *
 * @return true if the port was successfully opened
 */
extern Q_BOOL open_serial_port();

/**
 * Configure the serial port with the values in q_serial_port.
 *
 * @return true if the port was successfully re-configured
 */
extern Q_BOOL configure_serial_port();

/**
 * Close the serial port.
 *
 * @return true if the serial port was able to be re-configured with its
 * original terminal settings before it was closed
 */
extern Q_BOOL close_serial_port();

/**
 * Query the serial port and set the values of q_serial_port.rs232.
 *
 * @return true if the RS-232 state was able to be read
 */
extern Q_BOOL query_serial_port();

/**
 * Try to hang up the modem, first by dropping DTR and then if that doesn't
 * work by sending the hangup string.
 */
extern void hangup_modem();

/**
 * Return a string for a Q_BAUD_RATE enum.
 *
 * @param baud Q_BAUD_2400 etc.
 * @return "2400" etc.
 */
extern char * baud_string(const Q_BAUD_RATE baud);

/**
 * Return a string for a Q_DATA_BITS enum.
 *
 * @param bits Q_DATA_BITS_8 etc.
 * @return "8" etc.
 */
extern char * data_bits_string(const Q_DATA_BITS bits);

/**
 * Return a string for a Q_PARITY enum.
 *
 * @param parity Q_PARITY_NONE etc.
 * @param short_form if true, return a single capital letter, otherwise
 * return a lowercase string.
 * @return "N" or "none", etc.
 */
extern char * parity_string(const Q_PARITY parity, const Q_BOOL short_form);

/**
 * Return a string for a Q_STOP_BITS enum.
 *
 * @param bits Q_STOP_BITS_1 etc.
 * @return "1" etc.
 */
extern char * stop_bits_string(const Q_STOP_BITS bits);

#endif /* Q_NO_SERIAL */

#endif /* __MODEM_H__ */

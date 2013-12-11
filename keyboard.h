#ifndef __KEYBOARD_H
#define __KEYBOARD_H

enum KEY
{
	ESC,
	LCTRL,
	LSHIFT,
	RSHIFT,
	OPTION,
	UNKN
};

char getc(void);

#endif /* __KEYBOARD_H */
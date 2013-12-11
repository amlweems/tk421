#ifndef __KERNEL_H
#define __KERNEL_H

unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

#endif /* __KERNEL_H */
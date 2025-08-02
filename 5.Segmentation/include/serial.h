#ifndef SERIAL_H
#define SERIAL_H

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void serial_configure_fifo(unsigned short com);
void serial_configure_modem(unsigned short com);
void serial_init();
int serial_write(const char *buf, unsigned int len);

#endif

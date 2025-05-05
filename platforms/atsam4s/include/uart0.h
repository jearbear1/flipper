 #ifndef __UART0_H__
 #define __UART0_H__
 
 #include <stdint.h>
 #include "compiler.h"
 #include "status_codes.h"

 
int uart0_configure(void);
 
int uart0_setbaud(uint32_t baud);
 
int uart0_reset(void);
 
int uart0_ready(void);
 
void uart0_enable(void);
 
void uart0_disable(void);
 
void uart0_put(uint8_t byte);
 
uint8_t uart0_get(void);
 
int uart0_write(void *src, uint32_t length);
 
int uart0_read(void *dst, uint32_t length);
 

#endif /* __UART0_H__ */
 
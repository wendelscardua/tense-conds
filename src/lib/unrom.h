#ifndef _UNROM_H_
#define _UNROM_H_

void bankswitch(unsigned char bank);
void bankswitch_no_save(unsigned char bank);
void restore_bank();
void farcall(void);

#endif

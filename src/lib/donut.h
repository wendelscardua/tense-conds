#ifndef _LIB_DONUT_H_
#define _LIB_DONUT_H_

extern void * donut_stream_ptr;
extern char donut_block_buffer[64];
#pragma zpsym("donut_stream_ptr")

void donut_decompress_to_ppu(int num_blocks);
#endif

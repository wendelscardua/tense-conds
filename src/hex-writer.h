#define WRITE_NIBBLE_AS_HEX(VALUE,X,Y,TEMP)                  \
  {                                                          \
    TEMP = VALUE;                                            \
    if (TEMP <= 9) {                                         \
      one_vram_buffer(TEMP + 0x10, NTADR_A(X,Y));            \
    } else {                                                 \
      one_vram_buffer(TEMP - 10 + 0x21, NTADR_A(X,Y));       \
    }                                                        \
  }

#define WRITE_BYTE_AS_HEX(VALUE,X,Y,TEMP)               \
  {                                                     \
    WRITE_NIBBLE_AS_HEX((VALUE)&0x0f,(X)+1,Y,TEMP);     \
    WRITE_NIBBLE_AS_HEX((VALUE)>>4,X,Y,TEMP);           \
  }

#define WRITE_INT_AS_HEX(VALUE,X,Y,TEMP)            \
  {                                                 \
    WRITE_BYTE_AS_HEX((VALUE)&0xff,(X)+2,Y,TEMP);   \
    WRITE_BYTE_AS_HEX((VALUE)>>8,X,Y,TEMP);         \
  }

#include "avrlib/base.h"

static const uint8_t N = 128;

constexpr uint8_t chars[N] = {
   0x00,   //0x00 ' '  
   0x00,   //0x01 ' '
   0x00,   //0x02 ' '  
   0x00,   //0x03 ' '
   0x00,   //0x04 ' '  
   0x00,   //0x05 ' '
   0x00,   //0x06 ' '  
   0x00,   //0x07 ' '
   0x00,   //0x08 ' '  
   0x00,   //0x09 ' '
   0x00,   //0x0A ' '  
   0x00,   //0x0B ' '
   0x00,   //0x0C ' '  
   0x00,   //0x0D ' '
   0x00,   //0x0E ' '  
   0x00,   //0x0F ' '
   0x00,   //0x10 ' '  
   0x00,   //0x11 ' '
   0x00,   //0x12 ' '  
   0x00,   //0x13 ' '
   0x00,   //0x14 ' '  
   0x00,   //0x15 ' '
   0x00,   //0x16 ' '  
   0x00,   //0x17 ' '
   0x00,   //0x18 ' '  
   0x00,   //0x19 ' '
   0x00,   //0x1A ' '  
   0x00,   //0x1B ' '
   0x00,   //0x1C ' '  
   0x00,   //0x1D ' '
   0x00,   //0x1E ' '  
   0x00,   //0x1F ' '
   0x00,   //0x20 ' '
   0x86,   //0x21 '!'
   0x22,   //0x22 '"'
   0x7E,   //0x23 '#' ?
   0x2D,   //0x24 '$' ?
   0xD2,   //0x25 '%' ?
   0x7B,   //0x26 '&' ?
   0x20,   //0x27 '''
   0x39,   //0x28 '('
   0x0F,   //0x29 ')'
   0x63,   //0x2A '*' ?
   0x00,   //0x2B '+' ?
   0x10,   //0x2C ','
   0x40,   //0x2D '-'
   0x80,   //0x2E '.'
   0x52,   //0x2F '/' ?
   0x3F,   //0x30 '0'
   0x06,   //0x31 '1'
   0x5B,   //0x32 '2'
   0x4F,   //0x33 '3'
   0x66,   //0x34 '4'
   0x6D,   //0x35 '5'
   0x7D,   //0x36 '6'
   0x07,   //0x37 '7'
   0x7F,   //0x38 '8'
   0x6F,   //0x39 '9'
   0x09,   //0x3A ':' ?
   0x0D,   //0x3B ';' ?
   0x58,   //0x3C '<' ?
   0x48,   //0x3D '='
   0x4C,   //0x3E '>' ?
   0xD3,   //0x3F '?' ?
   0x5F,   //0x40 '@' ?
   0x77,   //0x41 'A'
   0x7C,   //0x42 'B'
   0x39,   //0x43 'C'
   0x5E,   //0x44 'D'
   0x79,   //0x45 'E'
   0x71,   //0x46 'F'
   0x3D,   //0x47 'G'
   0x76,   //0x48 'H'
   0x30,   //0x49 'I'
   0x1E,   //0x4A 'J'
   0x75,   //0x4B 'K' ?
   0x38,   //0x4C 'L'
   0x37,   //0x4D 'M'
   0x54,   //0x4E 'N'
   0x3F,   //0x4F 'O'
   0x73,   //0x50 'P'
   0x67,   //0x51 'Q' ?
   0x50,   //0x52 'R'
   0x6D,   //0x53 'S'
   0x78,   //0x54 'T'
   0x3E,   //0x55 'U'
   0x1C,   //0x56 'V' ? (u)
   0x2A,   //0x57 'W' ?
   0x76,   //0x58 'X' ? (like H)
   0x6E,   //0x59 'Y'
   0x5B,   //0x5A 'Z' ?
   0x39,   //0x5B '['
   0x64,   //0x5C '\' ?
   0x0F,   //0x5D ']'
   0x23,   //0x5E '^' ?
   0x08,   //0x5F '_'
   0x02,   //0x60 '`' ?
   0x77,   //0x61 'a'
   0x7C,   //0x62 'b'
   0x58,   //0x63 'c'
   0x5E,   //0x66 'd'
   0x79,   //0x65 'e'
   0x71,   //0x66 'f'
   0x3D,   //0x67 'g'
   0x74,   //0x68 'h'
   0x10,   //0x69 'i'
   0x1E,   //0x6A 'j'
   0x75,   //0x6B 'k' ?
   0x38,   //0x6C 'l'
   0x37,   //0x6D 'm'
   0x54,   //0x6E 'n'
   0x5C,   //0x6F 'o'
   0x73,   //0x70 'p'
   0x67,   //0x71 'q'
   0x50,   //0x72 'r'
   0x6D,   //0x73 's'
   0x78,   //0x74 't'
   0x3E,   //0x77 'u' ? (like U)
   0x1C,   //0x76 'v' ? (like u)
   0x2A,   //0x77 'w' ?
   0x76,   //0x78 'x' ? (like H)
   0x6E,   //0x79 'y'
   0x5B,   //0x7A 'z' ?
   0x46,   //0x7B '{' ?
   0x30,   //0x7C '|'
   0x70,   //0x7D '}' ?
   0x01,   //0x7E '~' ?
   0x00,   //0x7F 'DEL'
};


template<uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g>
struct A 
{
  uint8_t Segments[7] = {a, b, c, d, e, f, g};
  
  constexpr A() : arr()
  {
    for(auto i=0; i!=N; ++i)
    {
      uint8_t chr = chars[i];
      uint8_t sum = 0;
      for(int8_t s=0; s<7; ++s)
      {
        const uint8_t mask = 0x1 << s;
        sum += (chr & mask) == 0 ? Segments[s] : 0;
      }
      arr[i] = sum;
    }
  }
  uint8_t arr[N];
  
 
};
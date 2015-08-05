
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "defs.h"

#define log(format, args...) printf(format "\n", ##args)


char EAT_ptr[] = "abcdefghijklmnopqrstuvwxyz~!@#$%^&*(ABCDEFGHIJKLMNOPQRSTUVWXYZ)-|";

int decrypt_getlen(const char *a1)
{
  int result; // eax@2
  const char * i; // eax@3

  if ( !((unsigned int)a1 & 3) )
    {
      for ( i = a1; ; i += 4 )
        {
        LABEL_6:
          if ( ((*(_DWORD *)i + 0x7EFEFEFF) ^ ~*(_DWORD *)i) & 0x81010100 )
            {
              if ( !*(_BYTE *)i )
                goto LABEL_14;
              if ( !*(_BYTE *)(i + 1) )
                return (i - a1 + 1);
              if ( !*(_BYTE *)(i + 2) )
                return (i - a1 + 2);
              if ( !*(_BYTE *)(i + 3) )
                return (i - a1 + 3);
            }
        }
    }
  result = 0;
  if ( *(_BYTE *)a1 )
    {
      i = a1;
      do
        {
          ++i;
          if ( !((unsigned int)i & 3) )
            goto LABEL_6;
        }
      while ( *(_BYTE *)i );
    LABEL_14:
      result = i - a1;
    }
  return result;
}

char* decrypt_str(/*int a1, */int *str, int str_len)
{
  int v3; // ebx@1
  int tmp_str_len; // edi@1
  int *str_ptr; // ebp@1
  int i; // esi@6
  int v7; // eax@8
  char *v8; // edx@8
  char *v9; // eax@8
  int v10; // ecx@14
  int v11; // edx@14
  int v12; // eax@14
  int v13; // edx@18
  int v14; // eax@19
  int v16; // [sp+18h] [bp-44h]@5
  char *buf; // [sp+1Ch] [bp-40h]@3
  int tmp; // [sp+2Ch] [bp-30h]@1
  int v19[4];
  //int v19; // [sp+30h] [bp-2Ch]@9
  //int v20; // [sp+34h] [bp-28h]@14
  //int v21; // [sp+38h] [bp-24h]@14
  //int v22; // [sp+3Ch] [bp-20h]@16

  // sub_BA4();
  // v3 = a1 + 20859;
  v3 = 0x5FF4;
  tmp_str_len = str_len;
  str_ptr = str;
  tmp = 0;
  if ( !str_len || !str)
    return 0;
  buf = (char *) malloc(((unsigned int)(3 * str_len) >> 2) + 1);
  if (buf)
  {
    if ( str_len > 0 )
    {
      v16 = 0;
      while ( 1 )
      {
        i = 0;
        do
        {
          if (tmp_str_len)
          {
            v7 = *(_BYTE *) str_ptr;
            v8 = (&EAT_ptr)[v3 - 24564];
            *((_BYTE *)&tmp + i) = v7;
            v9 = strchr(v8, v7);
            if ( !v9 )
              return 0;
            str_ptr = (int *)((char *) str_ptr + 1);
            --tmp_str_len;
            v19[i] = v9 - (&EAT_ptr)[v3 - 24564];
          }
          ++i;
        }
        while ( i != 4 );
        if ( (_BYTE) tmp == 124 || BYTE1(tmp) == 124 || BYTE2(tmp) == 124 && BYTE3(
                                                                                 tmp) != 124 )
          return 0;
        // v10 = v20;
        v10 = v19[1];
        *((_BYTE *) buf + v16) = 4 * v19[0] | (v19[1] >> 4);
        // v11 = v21;
        v11 = v19[2];
        LOBYTE(v12) = 0;
        if ( v19[2] != 64 )
          v12 = (v19[2] >> 2) | 16 * v10;
        *((_BYTE *) buf + v16 + 1) = v12;
        LOBYTE(v12) = 0;
        if ( v19[3] != 64 )
          v12 = v19[3] | (v11 << 6);
        *((_BYTE *) buf + v16 + 2) = v12;
        v13 = v16 + 3;
        v16 += 3;
        if ( !tmp_str_len)
        {
          v14 = v13;
          goto LABEL_20;
        }
      }
    }
    v14 = 0;
    LABEL_20:
    *((_BYTE *) buf + v14) = 0;
  }
  return buf;
}

void d(const char *s)
{
  int len = decrypt_getlen(s);
  log("[%s] ==> [%s]", s, decrypt_str((int *) s, len));
}

void panda(int a1, int a2)
{
  return;
}

char* init_class_act_ptr(char *a1)
{
  char *result; // eax@1

  result = a1;
  *(_DWORD *)a1 = 0;
  *(_DWORD *)(a1 + 4) = 0;
  *(_DWORD *)(a1 + 8) = 1866799676;
  *(_DWORD *)(a1 + 12) = 1295657836;
  *(_DWORD *)(a1 + 16) = 2066365275;
  *(_DWORD *)(a1 + 20) = 1593797758;

  return result;
}

void memcpy_maybe(char *dst, char *src, unsigned int len)
{
  unsigned int v3; // edx@2

  if (len)
  {
    v3 = 0;
    do
    {
      *(_DWORD *)(dst + v3) =
          (*(_BYTE *)(src + v3 + 3) << 24) | *(_BYTE *)(src + v3) | (*(_BYTE *)(src + v3 + 2) << 16) | (*(_BYTE *)(src + v3 + 1) << 8);
      v3 += 4;
    }
    while ( len > v3 );
  }
}

int main(int argc, char *argv[])
{
  d("lTnAySfOzcZP$xnI!S@K!gZD");
  d("wwvP#gZAyxA^~xm^~gBP#gZO$sMk#gZJ!TjO!T@^~xm^ysbJ$xnQzxjVlaF*#xq^#gZAyxA^~xm^ysbD~wzQiq||");
  d("zteRn(^RyCySzdAQm(z*ytARzt^OyweQndaNndv*ogu|");
  d("lTnAySfOzcZP$xnI!S@K!gZD");
  d("uefusdQL@TBP#gvJlSjF!^||");
  d("lTnV@TrB!sZ*~wUL@CQ|");
  d("@CQ|");
  d("#TjL!C@&");

  return 0;
}




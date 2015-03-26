/*
 ***************************************************************************
 **
 ** md5.c -- the source code for MD5 routines
 ** Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm.
 **
 ***************************************************************************
 */
#include "stdafx.h"
#include "md5.h"

typedef unsigned long int UINT4;

typedef struct {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void __stdcall MD5Init (MD5_CTX *);
void __stdcall MD5Update (MD5_CTX *,unsigned char *,unsigned int);
void __stdcall MD5Final (unsigned char *,MD5_CTX *);
void __stdcall MD5Transform(UINT4 *,UINT4 *);

static unsigned char PADDING[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* F, G, H and I are basic MD5 functions */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#ifdef  PRINT_DETAILS
#include <stdlib.h>
void DumpState( UINT4 a, UINT4 b, UINT4 c, UINT4 d,
                int x, UINT4 m, int s, UINT4 k )
{
    printf( "a=%08X b=%08X c=%08X d=%08X M[%2d]=%08X s=%2d K=%08X\n",
            a, b, c, d, x, m, s, k );
}
#else
#define DumpState( a, b, c, d, x, m, s, k )
#endif

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac) \
  {(a) += F ((b), (c), (d)) + in[x] + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
   DumpState(a, b, c, d, x, in[x], s, ac); \
  }
#define GG(a, b, c, d, x, s, ac) \
  {(a) += G ((b), (c), (d)) + in[x] + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
   DumpState(a, b, c, d, x, in[x], s, ac); \
  }
#define HH(a, b, c, d, x, s, ac) \
  {(a) += H ((b), (c), (d)) + in[x] + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
   DumpState(a, b, c, d, x, in[x], s, ac); \
  }
#define II(a, b, c, d, x, s, ac) \
  {(a) += I ((b), (c), (d)) + in[x] + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
   DumpState(a, b, c, d, x, in[x], s, ac); \
  }

/* The routine MD5Init initializes the message-digest context
   mdContext. All fields are set to zero.
 */
void __stdcall MD5Init (MD5_CTX * mdContext)
{
  mdContext->i[0] = mdContext->i[1] = (UINT4)0;

  /* Load magic initialization constants.
   */
  mdContext->buf[0] = (UINT4)0x67452301;
  mdContext->buf[1] = (UINT4)0xefcdab89;
  mdContext->buf[2] = (UINT4)0x98badcfe;
  mdContext->buf[3] = (UINT4)0x10325476;
}

/* The routine MD5Update updates the message-digest context to
   account for the presence of each of the characters inBuf[0..inLen-1]
   in the message whose digest is being computed.
 */
void __stdcall MD5Update (MD5_CTX * mdContext, unsigned char * inBuf, unsigned int inLen)
{
  UINT4 in[16];
  int mdi;
  unsigned int i, ii;

  /* compute number of bytes mod 64 */
  mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

  /* update number of bits */
  if ((mdContext->i[0] + ((UINT4)inLen << 3)) < mdContext->i[0])
    mdContext->i[1]++;
  mdContext->i[0] += ((UINT4)inLen << 3);
  mdContext->i[1] += ((UINT4)inLen >> 29);

#ifdef  LITTLE_ENDIAN
  /* Speedup for little-endian machines suggested in MD5 report --P Karn */
	if(mdi == 0 && ((int)inBuf & 3) == 0){
		while(inLen >= 64){
			MD5Transform(mdContext->buf,(UINT4 *)inBuf);
			inLen -= 64;
			inBuf += 64;
		}               
	}
#endif  /* LITTLE_ENDIAN */
  while (inLen--) {
    /* add new character to buffer, increment mdi */
    mdContext->in[mdi++] = *inBuf++;

    /* transform if necessary */
    if (mdi == 0x40) {
      for (i = 0, ii = 0; i < 16; i++, ii += 4)
        in[i] = (((UINT4)mdContext->in[ii+3]) << 24) |
		(((UINT4)mdContext->in[ii+2]) << 16) |
		(((UINT4)mdContext->in[ii+1]) << 8) |
		((UINT4)mdContext->in[ii]);
      MD5Transform (mdContext->buf, in);
      mdi = 0;
    }
  }
}

/* The routine MD5Final terminates the message-digest computation and
   ends with the desired message digest in mdContext->digest[0...15].
 */
void __stdcall MD5Final(unsigned char * Digest, MD5_CTX * mdContext)
{
  UINT4 in[16];
  int mdi;
  unsigned int i, ii;
  unsigned int padLen;

  /* save number of bits */
  in[14] = mdContext->i[0];
  in[15] = mdContext->i[1];

  /* compute number of bytes mod 64 */
  mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

  /* pad out to 56 mod 64 */
  padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
  MD5Update (mdContext, PADDING, padLen);

  /* append length in bits and transform */
  for (i = 0, ii = 0; i < 14; i++, ii += 4)
    in[i] = (((UINT4)mdContext->in[ii+3]) << 24) |
	    (((UINT4)mdContext->in[ii+2]) << 16) |
	    (((UINT4)mdContext->in[ii+1]) << 8) |
	    ((UINT4)mdContext->in[ii]);
  MD5Transform (mdContext->buf, in);

  /* store buffer in digest */
  for (i = 0, ii = 0; i < 4; i++, ii += 4)
  {
    Digest[ii+0] = (unsigned char)(mdContext->buf[i] & 0xFF);
    Digest[ii+1] = (unsigned char)((mdContext->buf[i] >> 8) & 0xFF);
    Digest[ii+2] = (unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
    Digest[ii+3] = (unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
  }

#ifdef  PRINT_DETAILS
  printf( "Digest:\na=%08X b=%08X c=%08X d=%08X\n",
          mdContext->buf[0], mdContext->buf[1], mdContext->buf[2],
          mdContext->buf[3] );
#endif
}

/* Basic MD5 step. Transforms buf based on in.
 */
void __stdcall MD5Transform (UINT4 * buf, UINT4 * in)
{
  UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

  /* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#ifdef  PRINT_DETAILS
  printf( "Transform(FF):\na=%08X b=%08X c=%08X d=%08X\n", a, b, c, d );
#endif

  FF ( a, b, c, d,  0, S11, 3614090360); /* 1 */
  FF ( d, a, b, c,  1, S12, 3905402710); /* 2 */
  FF ( c, d, a, b,  2, S13,  606105819); /* 3 */
  FF ( b, c, d, a,  3, S14, 3250441966); /* 4 */
  FF ( a, b, c, d,  4, S11, 4118548399); /* 5 */
  FF ( d, a, b, c,  5, S12, 1200080426); /* 6 */
  FF ( c, d, a, b,  6, S13, 2821735955); /* 7 */
  FF ( b, c, d, a,  7, S14, 4249261313); /* 8 */
  FF ( a, b, c, d,  8, S11, 1770035416); /* 9 */
  FF ( d, a, b, c,  9, S12, 2336552879); /* 10 */
  FF ( c, d, a, b, 10, S13, 4294925233); /* 11 */
  FF ( b, c, d, a, 11, S14, 2304563134); /* 12 */
  FF ( a, b, c, d, 12, S11, 1804603682); /* 13 */
  FF ( d, a, b, c, 13, S12, 4254626195); /* 14 */
  FF ( c, d, a, b, 14, S13, 2792965006); /* 15 */
  FF ( b, c, d, a, 15, S14, 1236535329); /* 16 */

  /* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#ifdef  PRINT_DETAILS
  printf( "Transform(GG):\na=%08X b=%08X c=%08X d=%08X\n", a, b, c, d );
#endif

  GG ( a, b, c, d,  1, S21, 4129170786); /* 17 */
  GG ( d, a, b, c,  6, S22, 3225465664); /* 18 */
  GG ( c, d, a, b, 11, S23,  643717713); /* 19 */
  GG ( b, c, d, a,  0, S24, 3921069994); /* 20 */
  GG ( a, b, c, d,  5, S21, 3593408605); /* 21 */
  GG ( d, a, b, c, 10, S22,   38016083); /* 22 */
  GG ( c, d, a, b, 15, S23, 3634488961); /* 23 */
  GG ( b, c, d, a,  4, S24, 3889429448); /* 24 */
  GG ( a, b, c, d,  9, S21,  568446438); /* 25 */
  GG ( d, a, b, c, 14, S22, 3275163606); /* 26 */
  GG ( c, d, a, b,  3, S23, 4107603335); /* 27 */
  GG ( b, c, d, a,  8, S24, 1163531501); /* 28 */
  GG ( a, b, c, d, 13, S21, 2850285829); /* 29 */
  GG ( d, a, b, c,  2, S22, 4243563512); /* 30 */
  GG ( c, d, a, b,  7, S23, 1735328473); /* 31 */
  GG ( b, c, d, a, 12, S24, 2368359562); /* 32 */

  /* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#ifdef  PRINT_DETAILS
  printf( "Transform(HH):\na=%08X b=%08X c=%08X d=%08X\n", a, b, c, d );
#endif

  HH ( a, b, c, d,  5, S31, 4294588738); /* 33 */
  HH ( d, a, b, c,  8, S32, 2272392833); /* 34 */
  HH ( c, d, a, b, 11, S33, 1839030562); /* 35 */
  HH ( b, c, d, a, 14, S34, 4259657740); /* 36 */
  HH ( a, b, c, d,  1, S31, 2763975236); /* 37 */
  HH ( d, a, b, c,  4, S32, 1272893353); /* 38 */
  HH ( c, d, a, b,  7, S33, 4139469664); /* 39 */
  HH ( b, c, d, a, 10, S34, 3200236656); /* 40 */
  HH ( a, b, c, d, 13, S31,  681279174); /* 41 */
  HH ( d, a, b, c,  0, S32, 3936430074); /* 42 */
  HH ( c, d, a, b,  3, S33, 3572445317); /* 43 */
  HH ( b, c, d, a,  6, S34,   76029189); /* 44 */
  HH ( a, b, c, d,  9, S31, 3654602809); /* 45 */
  HH ( d, a, b, c, 12, S32, 3873151461); /* 46 */
  HH ( c, d, a, b, 15, S33,  530742520); /* 47 */
  HH ( b, c, d, a,  2, S34, 3299628645); /* 48 */

  /* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
#ifdef  PRINT_DETAILS
  printf( "Transform(II):\na=%08X b=%08X c=%08X d=%08X\n", a, b, c, d );
#endif

  II ( a, b, c, d,  0, S41, 4096336452); /* 49 */
  II ( d, a, b, c,  7, S42, 1126891415); /* 50 */
  II ( c, d, a, b, 14, S43, 2878612391); /* 51 */
  II ( b, c, d, a,  5, S44, 4237533241); /* 52 */
  II ( a, b, c, d, 12, S41, 1700485571); /* 53 */
  II ( d, a, b, c,  3, S42, 2399980690); /* 54 */
  II ( c, d, a, b, 10, S43, 4293915773); /* 55 */
  II ( b, c, d, a,  1, S44, 2240044497); /* 56 */
  II ( a, b, c, d,  8, S41, 1873313359); /* 57 */
  II ( d, a, b, c, 15, S42, 4264355552); /* 58 */
  II ( c, d, a, b,  6, S43, 2734768916); /* 59 */
  II ( b, c, d, a, 13, S44, 1309151649); /* 60 */
  II ( a, b, c, d,  4, S41, 4149444226); /* 61 */
  II ( d, a, b, c, 11, S42, 3174756917); /* 62 */
  II ( c, d, a, b,  2, S43,  718787259); /* 63 */
  II ( b, c, d, a,  9, S44, 3951481745); /* 64 */

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

/*
 ***********************************************************************
 ** Start of MD5 wrapper functions
 ***********************************************************************
 */

unsigned char *
__stdcall
MD5_Hash( unsigned char * Message,
          unsigned long   Length,
          unsigned char * Digest )
{
    MD5_CTX context;

    MD5Init( &context );
    MD5Update( &context, Message, Length );
    MD5Final( Digest, &context );

    return Digest;
}

unsigned char *
__stdcall
MD5_XOR(  unsigned char * text,         /* pointer to data stream        */
          unsigned long   text_len,     /* length of data stream         */
          unsigned char * key,          /* pointer to authentication key */
          unsigned long   key_len,      /* length of authentication key  */
          unsigned char * digest )      /* caller digest to be filled in */
{
    MD5_CTX context;
    unsigned char Buffer[64];
    unsigned long i, j;

    if( text_len > 64 ) text_len = 64;

    for( i = 0; i < text_len; i++ ) Buffer[i] = text[i];

    while( i < 64 ) Buffer[i++] = 0;

    if( key_len > 64 ) key_len = 64;
    for( i = 0; i < key_len; i++ ) Buffer[i] ^= key[i];

    MD5Init( &context );
    MD5Update( &context, Buffer, 64 );

    /* store buffer in digest */
    for (i = 0, j = 0; i < 4; i++, j += 4)
    {
        digest[j+0] = (unsigned char)((context.buf[i]      ) & 0xFF);
        digest[j+1] = (unsigned char)((context.buf[i] >>  8) & 0xFF);
        digest[j+2] = (unsigned char)((context.buf[i] >> 16) & 0xFF);
        digest[j+3] = (unsigned char)((context.buf[i] >> 24) & 0xFF);
    }

    return digest;
}

unsigned char *
__stdcall
MD5_HMAC( unsigned char * text,         /* pointer to data stream        */
          unsigned long   text_len,     /* length of data stream         */
          unsigned char * key,          /* pointer to authentication key */
          unsigned long   key_len,      /* length of authentication key  */
          unsigned char * key1,         /* inner hashed key1             */
          unsigned char * key2,         /* inner hashed key2             */
          unsigned char * digest )      /* caller digest to be filled in */
{
    MD5_CTX context;
    unsigned char k_ipad[64];    /* inner padding key XORd with ipad */
    unsigned char k_opad[64];    /* outer padding key XORd with opad */
    unsigned char tk[16];
    unsigned long i;

    /* if key is longer than 64 bytes reset it to key=MD5(key) */
    if (key_len > 64)
    {
        MD5_Hash(key, key_len,tk);
        key = tk;
        key_len = 16;
    }

    /*
     * the MD5_HMAC transform looks like:
     *
     * MD5(K XOR opad, MD5(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    for( i = 0; i < key_len; i++ )
    {
        k_ipad[i] = key[i] ^ 0x36;
        k_opad[i] = key[i] ^ 0x5c;
    }

    while( i < 64 )
    {
        k_ipad[i] = 0x36;
        k_opad[i] = 0x5c;
        i++;
    }

    // perform inner MD5
    MD5Init(&context);                   /* init context for 1st pass */
    MD5Update(&context, k_ipad, 64);     /* start with inner pad */

    // KEY1 is the current state of the hash
    if( key1 ) for (i = 0; i < 4; i++ )
    {
        *key1++ = (unsigned char)((context.buf[i]      ) & 0xFF);
        *key1++ = (unsigned char)((context.buf[i] >>  8) & 0xFF);
        *key1++ = (unsigned char)((context.buf[i] >> 16) & 0xFF);
        *key1++ = (unsigned char)((context.buf[i] >> 24) & 0xFF);
    }

    MD5Update(&context, text, text_len); /* then text of datagram */
    MD5Final(digest, &context);          /* finish up 1st pass */

    // perform outer MD5
    MD5Init(&context);                   /* init context for 2nd pass */
    MD5Update(&context, k_opad, 64);     /* start with outer pad */

    if( key2 ) for (i = 0; i < 4; i++ )
    {
        *key2++ = (unsigned char)((context.buf[i]      ) & 0xFF);
        *key2++ = (unsigned char)((context.buf[i] >>  8) & 0xFF);
        *key2++ = (unsigned char)((context.buf[i] >> 16) & 0xFF);
        *key2++ = (unsigned char)((context.buf[i] >> 24) & 0xFF);
    }

    MD5Update(&context, digest, 16);     /* then results of 1st hash */
    MD5Final(digest, &context);          /* finish up 2nd pass */

    return digest;
}

unsigned char *
__stdcall
MD5_CHAP( unsigned char   identifier,
          unsigned char * secret,
          unsigned long   secret_len,
          unsigned char * challenge,
          unsigned long   challenge_len,
          unsigned char * digest )
{
    MD5_CTX context;

    MD5Init( &context );

    MD5Update( &context, &identifier, 1 );
    MD5Update( &context, secret, secret_len );
    MD5Update( &context, challenge, challenge_len );
    MD5Final( digest, &context );

    return digest;
}


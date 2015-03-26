#ifdef __cplusplus
extern "C" {
#endif

/*
 ***********************************************************************
 ** MD5 wrapper functions
 ***********************************************************************
 */
unsigned char *
__stdcall
MD5_Hash( unsigned char * Data,
          unsigned long   Length,
          unsigned char * Digest );

unsigned char *
__stdcall
MD5_XOR(  unsigned char * text,         /* pointer to data stream        */
          unsigned long   text_len,     /* length of data stream         */
          unsigned char * key,          /* pointer to authentication key */
          unsigned long   key_len,      /* length of authentication key  */
          unsigned char * digest );     /* caller digest to be filled in */

unsigned char *
__stdcall
MD5_HMAC( unsigned char * text,         /* pointer to data stream        */
          unsigned long   text_len,     /* length of data stream         */
          unsigned char * key,          /* pointer to authentication key */
          unsigned long   key_len,      /* length of authentication key  */
          unsigned char * key1,         /* inner hashed key (optional)   */
          unsigned char * key2,         /* outer hashed key (optional)   */
          unsigned char * digest );     /* caller digest to be filled in */

unsigned char *
__stdcall
MD5_CHAP( unsigned char   identifier,
          unsigned char * secret,
          unsigned long   secret_len,
          unsigned char * challenge,
          unsigned long   challenge_len,
          unsigned char * digest );

#ifdef __cplusplus
}
#endif


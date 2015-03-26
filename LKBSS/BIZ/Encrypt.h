// Encrypt.h: interface for the Encrypt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENCRYPT_H__22B1C620_406E_42F4_AEA3_284745B681FC__INCLUDED_)
#define AFX_ENCRYPT_H__22B1C620_406E_42F4_AEA3_284745B681FC__INCLUDED_

#include "des.h"
 
#include "Base64.h"
 
class Encrypt{
public :
    char* encrypt ( unsigned char key[8], const char* data);
    char* decrypt ( unsigned char key[8], const char* data);
    CBase64 base;
};


#endif // !defined(AFX_ENCRYPT_H__22B1C620_406E_42F4_AEA3_284745B681FC__INCLUDED_)




// Encrypt.cpp: implementation of the Encrypt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Encrypt.h"
#include <stdio.h>

char* Encrypt::encrypt ( unsigned char key[8], const char* data)
{
    //���ַ�ָ��ת��Ϊ�ַ�����
    char* datavv;
    datavv =(char *) calloc(100,1);
    memcpy(datavv,data,strlen(data));
    datavv[strlen(datavv)] = '\0';
    //DES����
    DES des ;
    des.encrypt(key,(unsigned char *)datavv,1);
    //base64����
    base.Encode(datavv);
    char* res =(char *) base.EncodedMessage();
    
    /*
    //���ַ�ָ��ת��Ϊ�ַ�����
    char* datavv = data;
    data =(char *) calloc(100,1);
    memcpy(data,datavv,strlen(datavv));
    data[strlen(data)] = '\0';
    //DES����
    DES des ;
    des.encrypt(key,(unsigned char *)data,1);
    //base64����
    base.Encode(data);
    char* res =(char *) base.EncodedMessage();
    */
    return res;
};
char* Encrypt::decrypt ( unsigned char key[8], const char* data)
{
    //base64����
    base.Decode(data);
    char* datares=(char *) base.DecodedMessage();
    //���ַ�ָ��ת��Ϊ�ַ�����
    char* datarea = datares;
    datares = (char *)malloc(256);
    memcpy(datares,datarea,strlen(datarea));
    datares[strlen(datarea)] = '\0';
    //DES����
    DES des ;
    des.decrypt(key,(unsigned char*) datares,1);
    return datares;
};

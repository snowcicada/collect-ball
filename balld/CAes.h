#pragma once

/*encryption key length*/
#define LEVEL_128_KEYLEN	16
#define LEVEL_192_KEYLEN	24
#define LEVEL_256_KEYLEN	32

/*encrypt level*/
#define LEVEL_128			128
#define LEVEL_192			192
#define LEVEL_256			256

//Ĭ����Կ
#define DEFAULT_AESKEY      (U8 *)("ABCDabcd1234!@#$")

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned long int U32;
#endif

typedef struct
{
    U32 erk[64];     /* encryption round keys */
    U32 drk[64];     /* decryption round keys */
    int nr;             /* number of rounds */
} aes_context;

class CAes
{
public:
    CAes();
	//����Aes��Կ����ܼ���
	bool	SetAesKey(const U8 *key, int level = LEVEL_128);
	//�ӽ���ԭʼ����
	void	Encrypt(const string &src, string &dst);
    void	Decrypt(const string &src, string &dst);
	//���ܳ�ʮ������ ����
    void	EncryptToHex(const string &src, string &dst);
    bool	DecryptFromHex(const string &src, string &dst);
	//�ӽ����ļ�
	bool	EncryptFile(const char *path);
    bool	DecryptFile(const char *path);
	//�ļ����ܳ�ʮ������ ���� �ʺ�С�ļ��ļӽ���
	bool	EncryptFileToHex(const char *path);
	bool	DecryptFileFromHex(const char *path);
    //Ĭ����Կ�ӽ���
    static void EncryptToHexDefault(const string &src, string &dst);
    static bool DecryptFromHexDefault(const string &src, string &dst);
	//ʮ�����Ƶ�ת��
	static string ToHex(const string &str);
	static string FromHex(const string &hex);
	//�ж�
	static bool IsHexData(const string &hex);
	static bool IsAesData(const string &aes);

protected:
    void	encrypt_data(U8 *src, U8 *dst, int len, int level);
    void	decrypt_data(U8 *src, U8 *dst, int len, int level);
    int		aes_set_key(aes_context *ctx, U8 *key, int nbits);
    void	aes_encrypt(aes_context *ctx, U8 input[16], U8 output[16]);
    void	aes_decrypt(aes_context *ctx, U8 input[16], U8 output[16]);

private:
	aes_context m_ctx;
	int			m_iLevel;
};

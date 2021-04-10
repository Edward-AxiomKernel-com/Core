#include "Cryptographer.h"

#include <dll.h> // must be loaded as first header file of Crypto++
#include <secblock.h>
#include <hkdf.h>

namespace 
{
	const QString kMasterPassword = "camProxEmployeesHaveMassiveCocksExceptForCarl_IfYouReadThisMailHim:zupan@camprox.com";
    const QString kSalt = "weNeedSom3SaltOnTheTaaaable!";

	CryptoPP::SecByteBlock createKeyOutOfMasterPassword()
	{
		CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(key, key.size(), (const CryptoPP::byte*)kMasterPassword.data(), kMasterPassword.size(), (const CryptoPP::byte*)kSalt.data(), kSalt.size(), NULL, 0);

		return key;
	}
}

QByteArray Cryptographer::encryptFromPasswordAES(const QString text)
{
    CryptoPP::SecByteBlock key = createKeyOutOfMasterPassword();

    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption;
    cfbEncryption.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH);

    std::string plainText = text.toStdString();
    std::string cipherText;

    CryptoPP::StreamTransformationFilter stfEncryptor(cfbEncryption, new CryptoPP::StringSink(cipherText));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plainText.c_str()), plainText.length() + 1);
    stfEncryptor.MessageEnd();

    return cipherText.c_str();
}

QString Cryptographer::decryptFromPasswordAES(const QByteArray text)
{
    CryptoPP::SecByteBlock key = createKeyOutOfMasterPassword();

    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption; // (key, key.size(), iv);
    cfbDecryption.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH);

    std::string cipherText = text.toStdString();
    std::string decryptedText;

    CryptoPP::StreamTransformationFilter stfDecryptor(cfbDecryption, new CryptoPP::StringSink(decryptedText));
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(cipherText.c_str()), cipherText.size());
    stfDecryptor.MessageEnd();

    return QString::fromStdString(decryptedText);
}

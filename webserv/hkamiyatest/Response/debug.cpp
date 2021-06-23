#include <string>
#include <iostream>

std::string Base64Encode(std::string szStr)
{
	std::string szB64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	char *szEnc;
	int iLen, i, j;
	iLen = szStr.size();
	szEnc = new char [(int)((float)iLen * 1.5 + 4)];

	j = 0;
	for(i = 0; i < (iLen - (iLen % 3));  i+=3)
	{
		szEnc[j] = szB64[(szStr[i] & 0xfc) >> 2];
		szEnc[j + 1] = szB64[((szStr[i] & 0x03) << 4) | ((szStr[i + 1] & 0xf0) >> 4)];
		szEnc[j + 2] = szB64[((szStr[i + 1] & 0x0f) << 2) | ((szStr[i + 2] & 0xc0) >> 6)];
		szEnc[j + 3] = szB64[(szStr[i + 2] & 0x3f)];
		j += 4;
	}
	i = iLen - (iLen % 3);
	switch (iLen % 3)
	{
		case 2:
		{
			szEnc[j] = szB64[ (szStr[i] & 0xfc) >> 2];
			szEnc[j + 1] = szB64[ ((szStr[i] & 0x03) << 4) | ((szStr[i + 1] & 0xf0) >> 4) ];
			szEnc[j + 2] = szB64[(szStr[i + 1] & 0x0f) << 2];
			szEnc[j + 3] = szB64[64];
			break;
		}
		case 1:
		{
			szEnc[j] = szB64[ (szStr[i] & 0xfc) >> 2];
			szEnc[j + 1] = szB64[ (szStr[i] & 0x03) << 4];
			szEnc[j + 2] = szB64[64];
			szEnc[j + 3] = szB64[64];
		}
	default:
		break;
	}
	szEnc[j + 4] = '\0';
	std::string ret = std::string(szEnc);
	delete [] szEnc;
	return (ret);

}

int main()
{
  std::cout << Base64Encode("u") << std::endl;
}
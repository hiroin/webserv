#include <gtest/gtest.h>
#include "../getChunked.hpp"

// googletestのインストール手順
// https://rinatz.github.io/cpp-book/test-how-to-gtest/
// g++ -std=c++11 ../HTTPMessageParser.cpp HTTPMessageParser_test.cpp -o test -L/usr/local/lib -pthread -lgtest -lgtest_main

TEST(getChunked, parseChunkedData) {

  std::string clientBody;
  recvData r;
  getChunked gc;
  gc.setClientBody(&clientBody);
  gc.setRecvData(&r);

  r.setRecvData("7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  r.setRecvData("f\r\n123456789012345\r\nf\r\n123456789012345\r\n0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("123456789012345123456789012345", clientBody);
  clientBody.clear();
  gc.clear();

  r.setRecvData("7;aaa\r\nMozilla\r\n9;aaa\r\nDeveloper\r\n7;aaa\r\nNetwork\r\n0;aaa\r\n");
  gc.setRecvData(&r);
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // chunksizeが1足りない
  r.setRecvData("6\r\nMozilla\r\n0\r\n");
  gc.setRecvData(&r);
  EXPECT_EQ(400, gc.parseChunkedData());
  clientBody.clear();
  gc.clear();

  // chunksizeに不正な文字
  r.setRecvData("g\r\nMozilla\r\n0\r\n");
  gc.setRecvData(&r);
  EXPECT_EQ(400, gc.parseChunkedData());
  clientBody.clear();
  gc.clear();

  // chunkdataが\r\nで終わっていない(\nを\r\nとしてみなすので許容する)
  r.setRecvData("8\r\nMozilla\r\n0\r\n");
  gc.setRecvData(&r);
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("Mozilla\r", clientBody);
  clientBody.clear();
  gc.clear();

  // 改行文字がすべて\r
  r.setRecvData("7\rMozilla\r9\rDeveloper\r7\rNetwork\r0\r");
  gc.setRecvData(&r);
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // 改行文字がすべて\n
  r.setRecvData("7\nMozilla\n9\nDeveloper\n7\nNetwork\n0\n");
  gc.setRecvData(&r);
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // 改行文字が混在
  r.setRecvData("7\r\nMozilla\n9\rDeveloper\r\n7\nNetwork\r0\r\n");
  gc.setRecvData(&r);
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // データが足りない
  r.setRecvData("7");
  gc.setRecvData(&r);
  EXPECT_EQ(-1, gc.parseChunkedData());

  // データを追加
  r.setRecvData("7\r\nMozilla\n9\rDeveloper\r\n7\nNetwork\r0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // データが足りない
  r.setRecvData("7\r\n");
  gc.setRecvData(&r);
  EXPECT_EQ(-1, gc.parseChunkedData());

  // データを追加
  r.setRecvData("Mozilla\n9\rDeveloper\r\n7\nNetwork\r0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // データが足りない
  r.setRecvData("7\r\nMozi");
  gc.setRecvData(&r);
  EXPECT_EQ(-1, gc.parseChunkedData());

  // データを追加
  r.setRecvData("Mozilla\n9\rDeveloper\r\n7\nNetwork\r0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // データが足りない
  r.setRecvData("7\r\nMozilla");
  gc.setRecvData(&r);
  EXPECT_EQ(-1, gc.parseChunkedData());

  // データを追加
  r.setRecvData("\r9\rDeveloper\r\n7\nNetwork\r0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // データが足りない
  r.setRecvData("7\r\nMozilla\n");
  gc.setRecvData(&r);
  EXPECT_EQ(-1, gc.parseChunkedData());

  // データを追加
  r.setRecvData("9\rDeveloper\r\n7\nNetwork\r0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

  // データが足りない
  r.setRecvData("7\r\nMozilla\n9\rDeveloper\r\n7\nNetwork\r0");
  gc.setRecvData(&r);
  EXPECT_EQ(-1, gc.parseChunkedData());

  // データを追加
  r.setRecvData("0\r\n");
  EXPECT_EQ(200, gc.parseChunkedData());
  EXPECT_EQ("MozillaDeveloperNetwork", clientBody);
  clientBody.clear();
  gc.clear();

}

TEST(getChunked, hex2int) {

  getChunked gc;
  EXPECT_EQ(0, gc.hex2int('0'));
  EXPECT_EQ(9, gc.hex2int('9'));
  EXPECT_EQ(10, gc.hex2int('a'));
  EXPECT_EQ(11, gc.hex2int('b'));
  EXPECT_EQ(12, gc.hex2int('c'));
  EXPECT_EQ(13, gc.hex2int('d'));
  EXPECT_EQ(14, gc.hex2int('e'));
  EXPECT_EQ(15, gc.hex2int('f'));
  EXPECT_EQ(10, gc.hex2int('A'));
  EXPECT_EQ(11, gc.hex2int('B'));
  EXPECT_EQ(12, gc.hex2int('C'));
  EXPECT_EQ(13, gc.hex2int('D'));
  EXPECT_EQ(14, gc.hex2int('E'));
  EXPECT_EQ(15, gc.hex2int('F'));
  EXPECT_EQ(-1, gc.hex2int('g'));
}

TEST(getChunked, hexstring2int) {

  getChunked gc;
  EXPECT_EQ(0, gc.hexstring2int("0"));
  EXPECT_EQ(9, gc.hexstring2int("9"));
  EXPECT_EQ(10, gc.hexstring2int("a"));
  EXPECT_EQ(15, gc.hexstring2int("f"));
  EXPECT_EQ(16, gc.hexstring2int("10"));
  EXPECT_EQ(100, gc.hexstring2int("64"));
  EXPECT_EQ(2147483647, gc.hexstring2int("7FFFFFFF"));
  EXPECT_EQ(-1, gc.hexstring2int("80000000"));
  EXPECT_EQ(-1, gc.hexstring2int("80000000000000000000000000000000000000000"));
  EXPECT_EQ(-1, gc.hexstring2int("g"));
  EXPECT_EQ(-1, gc.hexstring2int("-1"));
}

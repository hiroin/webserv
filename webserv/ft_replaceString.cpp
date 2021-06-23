#include <string>
 
/*
    string中の特定文字列をstringで置換する
*/
std::string ReplaceString
(
    std::string String1   // 置き換え対象
  , std::string String2   // 検索対象
  , std::string String3   // 置き換える内容
)
{
  std::string::size_type  Pos( String1.find( String2 ) );
  while( Pos != std::string::npos )
  {
    String1.replace( Pos, String2.length(), String3 );
    Pos = String1.find( String2, Pos + String3.length() );
  }
  return String1;
}
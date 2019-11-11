#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <immintrin.h>
#include <cassert>
#include <fstream>
#include <string>

using namespace std;

union My64Bit {
  unsigned long long tmp_u_LL;
  char charArr[8];
}my64Bit;

static const int mySize = 150 * 1000 * 1000;

int main(){
  vector<string> outputStr;
  outputStr.reserve(mySize);
  queue<string> tmpStorage;
  for( int i = 0; i < mySize; ++ i )
    outputStr.push_back( "adta -s " );

  for( int i = 0, j = 0; i < mySize; ++i ){
    if( j > i/2 ){
      _rdrand64_step(&(my64Bit.tmp_u_LL));
      if( my64Bit.tmp_u_LL %2 ){
        outputStr.at(i).assign(string("adtd -s "));
        j--;
        continue;
      }
    }
    ++j;
  }

  for( auto& i : outputStr ){
    if( i[3] == 'a' ){
      _rdrand64_step(&(my64Bit.tmp_u_LL));
      for( int j = 0; j < 8; ++ j)
        my64Bit.charArr[j] = (  ((unsigned)my64Bit.charArr[j])%26  ) + 'a';
      tmpStorage.push( string( my64Bit.charArr , 6 ) );
      i.append(tmpStorage.back());
    }else if( i[3] == 'd' ){
      i.append(tmpStorage.front());
      tmpStorage.pop();
    }else {
      assert( 0 && "wtf" );
    }
  }

  ofstream ofs( "../tests/mydo2019.txt", ofstream::out | ofstream::trunc );
  for( auto& it : outputStr )
    ofs << it << endl;

  return 0;
}

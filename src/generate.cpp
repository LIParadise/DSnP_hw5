#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <immintrin.h>
#include <cassert>
#include <fstream>

using namespace std;

int main(){
  vector<string> outputStr;
  outputStr.reserve(100*10000);
  queue<unsigned long long> tmpStorage;
  for( int i = 0; i < 100*10000; ++ i )
    outputStr.push_back( "adta -r " );

  unsigned long long tmp_u_LL;
  for( int i = 0, j = 0; i < 100*10000; ++i ){
    if( j > i/2 ){
      _rdrand64_step(&tmp_u_LL);
      if( tmp_u_LL %2 ){
        outputStr.at(i).assign(string("adtd -r "));
        j--;
        continue;
      }
    }
    ++j;
  }

  for( auto& i : outputStr ){
    if( i[3] == 'a' ){
      _rdrand64_step(&tmp_u_LL);
      tmpStorage.push(tmp_u_LL);
      i.append(to_string(tmpStorage.back()%(65536*2)));
    }else if( i[3] == 'd' ){
      i.append(to_string(tmpStorage.front()%(65536*2)));
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

if [ $# -ne 1 ]; then
  echo "usage: ./tests.sh <a/b/d>"
  exit
fi

if [ ${1} = a ]
then
  echo "case array"
  ref/adtTest.array -f tests/do1 >& log/ref1
  ref/adtTest.array -f tests/do2 >& log/ref2
  ref/adtTest.array -f tests/do3 >& log/ref3
  ref/adtTest.array -f tests/do4 >& log/ref4
# ref/adtTest.array -f tests/do6 >& log/ref6
  echo "ref done"
  ./adtTest.array     -f tests/do1 >& log/my1
  ./adtTest.array     -f tests/do2 >& log/my2
  ./adtTest.array     -f tests/do3 >& log/my3
  ./adtTest.array     -f tests/do4 >& log/my4
# ./adtTest.array     -f tests/do6 >& log/my6 
  echo "mine done"
  echo ""
elif [ ${1} = b ]
then
  echo "case bst"
  ref/adtTest.bst -f tests/do1 >& log/ref1
  ref/adtTest.bst -f tests/do2 >& log/ref2
  ref/adtTest.bst -f tests/do3 >& log/ref3
  ref/adtTest.bst -f tests/do4 >& log/ref4
# ref/adtTest.bst -f tests/do6 >& log/ref6
  echo "ref done"
  ./adtTest.bst     -f tests/do1 >& log/my1
  ./adtTest.bst     -f tests/do2 >& log/my2
  ./adtTest.bst     -f tests/do3 >& log/my3
  ./adtTest.bst     -f tests/do4 >& log/my4
# ./adtTest.bst     -f tests/do6 >& log/my6 
  echo "mine done"
  echo ""
elif [ ${1} = d ]
then
  echo "case dlist"
  ref/adtTest.dlist -f tests/do1 >& log/ref1
  ref/adtTest.dlist -f tests/do2 >& log/ref2
  ref/adtTest.dlist -f tests/do3 >& log/ref3
  ref/adtTest.dlist -f tests/do4 >& log/ref4
# ref/adtTest.dlist -f tests/do6 >& log/ref6
  echo "ref done"
  ./adtTest.dlist     -f tests/do1 >& log/my1
  ./adtTest.dlist     -f tests/do2 >& log/my2
  ./adtTest.dlist     -f tests/do3 >& log/my3
  ./adtTest.dlist     -f tests/do4 >& log/my4
# ./adtTest.dlist     -f tests/do6 >& log/my6 
  echo "mine done"
  echo ""
else
  exit
fi
exit

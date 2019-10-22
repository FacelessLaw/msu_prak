echo "Building..."
make clean
make

echo ""
echo ""

BASE_DIR=`pwd`
echo ""
echo "Start testing..."
echo ""
TEST_DIR=$BASE_DIR/tests
TEST_FILES=`ls $TEST_DIR`
for filename in $TEST_FILES; do
    echo ""
    echo "#######################################################################"
    echo ""
    echo "For $TEST_DIR/$filename"
    sort $TEST_DIR/$filename > pBash
    valgrind --log-file=vlog --leak-check=full -v cat $TEST_DIR/$filename | ./m > resFile
    echo "------------"
    echo "| Result:: |"
    echo "------------"
    cat resFile
    echo "------------"
    echo "| ResultEnd   |"
    echo "------------"
    echo ""
    echo "***********************"
    echo "--------------------"
    echo "| valgrind:result  |"
    echo "--------------------"
    tail -2 ./vlog
done
echo ""
echo "#######################################################################"
echo ""
    
rm resFile vlog

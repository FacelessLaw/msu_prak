make clean
make

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
    valgrind --log-file=log --leak-check=full -v ./m $TEST_DIR/$filename $filename 228 1488
    echo "------------"
    echo "| diffBegin |"
    echo "------------"
    diff -u $TEST_DIR/$filename $filename
    echo "------------"
    echo "| diffEnd   |"
    echo "------------"
    echo ""
    echo "***********************"
    echo "--------------------"
    echo "| valgrind:result  |"
    echo "--------------------"
    tail -2 ./log
    rm $filename
done
echo ""
echo "#######################################################################"
echo ""
    
rm pMy log

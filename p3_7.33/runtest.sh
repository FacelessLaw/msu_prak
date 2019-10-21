make clean
make

rm -r tests/
cp -r test_buf/ tests

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
    
    echo "---------------"
    echo "| oldFileBegin |"
    echo "---------------"
    echo ""
    cat $TEST_DIR/$filename
    echo ""
    echo "-------------"
    echo "| oldFileEnd|"
    echo "------------"
    echo ""
    valgrind --log-file=log --leak-check=full -v ./m $TEST_DIR/$filename > pMy
    echo "----------------"
    echo "| newFileBegin |"
    echo "----------------"
    echo ""
    cat $TEST_DIR/$filename
    echo ""
    echo "-------------"
    echo "| newFileEnd |"
    echo "-------------"
    echo ""
    
    echo "***********************"
    echo "--------------------"
    echo "| valgrind:result  |"
    echo "--------------------"
    tail -2 ./log
done
echo ""
echo "#######################################################################"
echo ""
    
rm pMy log

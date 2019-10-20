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
    valgrind --log-file=log --leak-check=full -v cat $TEST_DIR/$filename | ./m > pMy
    echo "------------"
    echo "| resBegin |"
    echo "------------"
    cat pMy
    echo "------------"
    echo "| resEnd   |"
    echo "------------"
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

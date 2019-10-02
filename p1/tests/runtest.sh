BASE_DIR=`pwd`
echo ""
echo "Start testing..."
echo ""
TEST_DIR=$BASE_DIR/tests
TEST_FILES=`ls $TEST_DIR`
for filename in $TEST_FILES; do
    echo "#####################################################"
    echo "For $TEST_DIR/$filename"
    sort $TEST_DIR/$filename > pBash
    valgrind --log-file=log --leak-check=full -v cat $TEST_DIR/$filename | ./m > pMy
    echo "------------"
    echo "| DiffBegin |"
    echo "------------"
    diff -u pBash pMy
    echo "------------"
    echo "| DiffEnd   |"
    echo "------------"
    echo ""
    echo "***********************"
    echo "------------"
    echo "| valgrind:result  |"
    echo "------------"
    tail -2 ./log
done

rm pBash pMy log

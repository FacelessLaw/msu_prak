BASE_DIR=`pwd`
TOOLS_DIR=$BASE_DIR/tools
tools=`ls $TOOLS_DIR`
echo ""
echo "Start testing..."
echo ""
        
for tool in $tools; do
    TEST_DIR=$BASE_DIR/tests/$tool
    TEST_FILES=`ls $TEST_DIR`
    echo "Result for tool: $tool"
    echo "#####################################################"
    for filename in $TEST_FILES; do
        echo "For $TEST_DIR/$filename"
        echo "----------------"
        echo "| Bash result: |"
        echo "----------------"
        
        echo ""
        eval "$tool $TEST_DIR/$filename"
        echo ""
        
        echo "......"
        
        echo "----------------"
        echo "| Your result: |"
        echo "----------------"
        echo ""
        
        cat $TEST_DIR/$filename | ./m $tool
        echo ""
        echo "......"    
        echo ""
        echo "================================================"
        
    done
    echo "#####################################################"

    echo "*"
    echo "*"
    echo "*"
    echo "*"
    echo "*"
    echo "*"
    
done
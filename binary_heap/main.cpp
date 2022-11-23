#include <bits/stdc++.h>

template<typename scopedType>
class binary_heap
{
private:
    bool empty;
    
    scopedType value;
    size_t size;

    std::shared_ptr<binary_heap> leftChild;
    std::shared_ptr<binary_heap> rightChild;

    const size_t sizeOfLeft()
    {
        if(leftChild)
            return leftChild->size;

        return 0;
    }
    
    const size_t sizeOfRight()
    {
        if(rightChild)
            return rightChild->size;

        return 0;
    }

protected:
    void bubbleUp()
    {
        scopedType tmp;

        if(leftChild && !leftChild->empty && leftChild->value < value)
        {
            tmp = value;
            value = leftChild->value;
            leftChild->value = tmp;
        }

        if(rightChild && !rightChild->empty && rightChild->value < value)
        {
            tmp = value;
            value = rightChild->value;
            rightChild->value = tmp;
        }
    }

    void bubbleDown()
    {
        if(leftChild && !leftChild->empty)
        {
            if(rightChild && !rightChild->empty && rightChild->value < leftChild->value)
            {
                value = rightChild->value;
                rightChild->bubbleDown();
            }
            else
            {
                value = leftChild->value;
                leftChild->bubbleDown();
            }
        }
        else if(rightChild && !rightChild->empty)
        {
            if(leftChild && !leftChild->empty && leftChild->value < rightChild->value)
            {
                value = leftChild->value;
                leftChild->bubbleDown();
            }
            else
            {
                value = rightChild->value;
                rightChild->bubbleDown();
            }
        }
        else
            empty = true;

        if(leftChild && leftChild->empty) 
            leftChild = nullptr;
        
        if(rightChild && rightChild->empty)
            rightChild = nullptr;

        size = sizeOfLeft() + sizeOfRight() + 1;
    }

public:
    binary_heap()
    {
        empty = true;
        size = 0;
    }

    binary_heap(const scopedType & value)
    {
        this->value = value;
        empty = false;
        size = 1;
    }

    void insert(const scopedType & value)
    {
        if(empty)
        {
            this->value = value;
            empty = false;
            return;
        }

        if(sizeOfLeft() > sizeOfRight())
        {
            if(rightChild)
                rightChild->insert(value);
            else
                rightChild = std::make_shared<binary_heap<scopedType>>(value);
        }
        else
        {
            if(leftChild)
                leftChild->insert(value);
            else
                leftChild = std::make_shared<binary_heap<scopedType>>(value);
        }

        empty = false;

        size = sizeOfLeft() + sizeOfRight() + 1;

        bubbleUp();
    }

    const scopedType & findMin()
    {
        if(empty)
            throw std::out_of_range("heap is empty");

        return value;
    }
    
    scopedType extractMin()
    {
        scopedType ret = findMin();

        bubbleDown();

        return ret;
    }

    const void print(int space = 0) const
    {
        space += 10;
    
        if(rightChild)
            rightChild->print(space);
    
        std::cerr << std::endl;
        for (int i = 10; i < space; i++)
            std::cerr << " ";
        std::cerr << value << "\n";
        
        if(leftChild)
            leftChild->print(space);
    }

};

#ifndef __TEST__

void test1()
{
    int i;
    binary_heap<int> test;

    for(i = 0; i < 100; i++)
        test.insert(std::rand() % 20);

    for(i = 0; i < 100; i++)
        test.extractMin();
    
    try
    {
        test.extractMin();
    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << '\n';
    }
    
}

signed main()
{
    test1();

    return 0;
}
#endif

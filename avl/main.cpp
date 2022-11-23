#include <bits/stdc++.h>
template < typename Key >
struct Node
{
    std::shared_ptr<Node<Key>> left;
    std::shared_ptr<Node<Key>> right;

    Key value;

    size_t height;
    size_t size;

    Node(const Key & value)
    {
        height = 1;
        size = 1;
        this->value = value;
        left = nullptr;
        right = nullptr;
    }
};

template < typename Key >
class avl_tree
{
    std::shared_ptr<Node<Key>> root;

    const size_t height(const std::shared_ptr<Node<Key>> & node) const
    {
        if (node == nullptr)
            return 0;

        return node->height;
    }

    const size_t & greater(const size_t & left, const size_t & right) const
    {
        if(left > right)
            return left;

        return right;
    }

    const size_t sizeOfSubtree(const std::shared_ptr<Node<Key>> & node) const
    {
        if(!node)
            return 0;

        return node->size;
    }

    const std::shared_ptr<Node<Key>> findSmallest(const std::shared_ptr<Node<Key>> & node)
    {
        auto tmp = node;

        while(tmp->left)
            tmp = tmp->left;

        return tmp;
    }

    std::shared_ptr<Node<Key>> leftRotate(std::shared_ptr<Node<Key>> & parent)
    {
        std::shared_ptr<Node<Key>> child = parent->right;
        
        if(!child)
            return rightRotate(parent);

        std::shared_ptr<Node<Key>> grandChild = child->left;
        
        child->left = parent;
        
        parent->right = grandChild;
            
        parent->height = greater(height(parent->left), height(parent->right)) + 1;
        child->height = greater(height(child->left), height(child->right)) + 1;

        parent->size = sizeOfSubtree(parent->left) + sizeOfSubtree(parent->right) + 1;
        child->size = sizeOfSubtree(child->left) + sizeOfSubtree(child->right) + 1;
    
        return child;
    }

    std::shared_ptr<Node<Key>> rightRotate(std::shared_ptr<Node<Key>> & parent)
    {
        std::shared_ptr<Node<Key>> child = parent->left;

        if(!child)
            return leftRotate(parent);

        std::shared_ptr<Node<Key>> grandChild = child->right;
    
        child->right = parent;
        
        parent->left = grandChild;

        parent->height = greater(height(parent->left), height(parent->right)) + 1;
        child->height = greater(height(child->left), height(child->right)) + 1;

        parent->size = sizeOfSubtree(parent->left) + sizeOfSubtree(parent->right) + 1;
        child->size = sizeOfSubtree(child->left) + sizeOfSubtree(child->right) + 1;
    
        return child;
    }

    std::shared_ptr<Node<Key>> updateNode(std::shared_ptr<Node<Key>> & node)
    {
        long int heightRatio;
        node->height = greater(height(node->left), height(node->right)) + 1;
        node->size = sizeOfSubtree(node->right) + sizeOfSubtree(node->left) + 1;
        
        heightRatio = height(node->left) - height(node->right);

        if(heightRatio > 1)
        {
            if(node->value < node->left->value)
                return rightRotate(node);
            else if(node->value > node->left->value)
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        else if(heightRatio < -1)
        {
            if(node->value > node->right->value)
                return leftRotate(node);
            else if(node->value < node->right->value)
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        
        return node;
    }
    
    std::shared_ptr<Node<Key>> removeRec(std::shared_ptr<Node<Key>> & node, const Key & key)
    {
        if (!node)
            return node;

        if (key < node->value)
           node->left = removeRec(node->left, key);
        else if (key > node->value)
           node->right = removeRec(node->right, key);
        else
        {
            if (!node->left)
               return node->right;

            if (!node->right)
               return node->left;
            
            auto tmp = findSmallest(node->right);
            node->value = tmp->value;
            node->right = removeRec(node->right, tmp->value);
        }
        
        return updateNode(node);
    }

    std::shared_ptr<Node<Key>> insertRec(std::shared_ptr<Node<Key>> & node, const Key & key)
    {
        if (!node)
            return std::make_shared<Node<Key>>(key);
        
        if (key < node->value)
        {
            node->left = insertRec(node->left, key);
        }
        else if (key > node->value)
        {
            node->right = insertRec(node->right, key);
        }
        else
            return node;

        return updateNode(node);
    }

    const Key & getKey(const std::shared_ptr<Node<Key>> & node, const size_t & targetIndex, size_t & lastIndexVisited) const
    {
        if(lastIndexVisited < targetIndex)
        {
            lastIndexVisited = lastIndexVisited + sizeOfSubtree(node->right->left) + 1;

            return getKey(node->right, targetIndex, lastIndexVisited);
        }
        else if(lastIndexVisited > targetIndex)
        {
            lastIndexVisited = lastIndexVisited - (sizeOfSubtree(node->left->right) + 1);

            return getKey(node->left, targetIndex, lastIndexVisited);
        }

        return node->value;
    }

    const size_t order_of_key_rec(const std::shared_ptr<Node<Key>> & node, const Key & value, size_t & lastIndexVisited) const
    {
        if(value < node->value)
        {
            if(!node->left)
                throw std::out_of_range("Key is not present.");

            lastIndexVisited = lastIndexVisited - (sizeOfSubtree(node->left->right) + 1);

            return order_of_key_rec(node->left, value, lastIndexVisited);
        }
        else if(value > node->value)
        {
            if(!node->right)
                throw std::out_of_range("Key is not present.");

            lastIndexVisited = lastIndexVisited + sizeOfSubtree(node->right->left) + 1;

            return order_of_key_rec(node->right, value, lastIndexVisited);
        }

        return lastIndexVisited;
    }

    const void print2DUtil(const std::shared_ptr<Node<Key>> & root, int space) const
    {
        if (!root)
            return;
    
        space += 10;
    
        print2DUtil(root->right, space);
    
        std::cerr << std::endl;
        for (int i = 10; i < space; i++)
            std::cerr << " ";
        std::cerr << root->value << "\n";
        print2DUtil(root->left, space);
    }

    const void print2DUtilClass(const std::shared_ptr<Node<Key>> & root, int space) const
    {
        if (!root)
            return;
    
        space += 10;
    
        print2DUtilClass(root->right, space);
    
        std::cerr << std::endl;
        for (int i = 10; i < space; i++)
            std::cerr << " ";
        std::cerr << "node" << "\n";
        print2DUtilClass(root->left, space);
    }

public:
    avl_tree<Key> & insert(const Key & target)
    {
        root = insertRec(root, target);
        
        return *this;
    }

    avl_tree<Key> & remove(const Key & target)
    {
        root = removeRec(root, target);

        return *this;
    }

    const size_t size() const
    {
        return sizeOfSubtree(root);
    }

    const size_t order_of_key(const Key & key) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        return order_of_key_rec(root, key, startingIndex);
    }

    const Key & find(const size_t & target) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        if(target >= sizeOfSubtree(root))
            throw std::out_of_range("Target is too big stepbro.");

        return getKey(root, target, startingIndex);
    }

    const Key & operator[](const size_t & target) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        if(target >= sizeOfSubtree(root))
            throw std::out_of_range("Target is too big stepbro.");

        return getKey(root, target, startingIndex);
    }

    const Key & begin()
    {
        std::shared_ptr<Node<Key>> ret = root;

        while(ret->left)
            ret = ret->left;

        return ret->value;
    }

    const Key & last()
    {
        std::shared_ptr<Node<Key>> ret = root;

        while(ret->right)
            ret = ret->right;

        return ret->value; 
    }

    const void print() const
    {
        std::cerr << "Size: " << sizeOfSubtree(root) << std::endl << "Height: " << root->height << std::endl;

        print2DUtil(root, 0);

        std::cerr << std::endl << std::endl;
    }

    const void printClass() const
    {
        std::cerr << "Size: " << sizeOfSubtree(root) << std::endl << "Height: " << root->height << std::endl;

        print2DUtilClass(root, 0);

        std::cerr << std::endl << std::endl;
    }
};

#ifndef __TEST__

void test1()
{
    avl_tree<int> test;

    for(int i = 0; i < 100000; i++)
        test.insert(i);

    assert(test[42069] == 42069);
    assert(test.order_of_key(42069) == 42069);

    try { test[100000]; } catch(const std::exception& e) {}

    try { test.order_of_key(100000); } catch(const std::exception& e) {}   
}

void test2()
{
    avl_tree<int> test;

    test.insert(0).insert(1).insert(2).insert(3).insert(4).insert(5).insert(6).insert(7).insert(8).insert(9).insert(10).insert(11).insert(12).insert(13).insert(14).insert(15).insert(16).insert(17).insert(18); 
    assert(test.begin() == 0);
    assert(test.last() == 18);
    assert(test.size() == 19);

    for(int i = 0; i < 19; i++)
        assert(test[i] > -1);

    test.remove(18);
    test.remove(14); test.remove(10); test.remove(6); test.remove(2); test.insert(18); test.insert(14); test.insert(10); test.insert(6); test.insert(2); test.remove(2); test.remove(6); test.remove(10); test.remove(14); test.remove(18); test.insert(2); test.insert(6); test.insert(10); test.insert(14); test.insert(18); test.remove(2); test.insert(2); test.remove(6); test.insert(6); test.remove(10); test.insert(10); test.remove(14); test.insert(14); test.remove(18); test.insert(18); 
    
    try { test.remove(100000); } catch(const std::exception& e) {}

    while(test.size())
        try { test.remove(std::rand() % 19); } catch(const std::exception& e) {}        

    while(test.size() < 19)
        try { test.insert(std::rand() % 19); } catch(const std::exception& e) {}        

    assert(test[0] == 0); assert(test[1] == 1); assert(test[2] == 2); assert(test[3] == 3); assert(test[4] == 4); assert(test[5] == 5); assert(test[6] == 6); assert(test[7] == 7); assert(test[8] == 8); assert(test[9] == 9); assert(test[10] == 10); assert(test[11] == 11); assert(test[12] == 12); assert(test[13] == 13); assert(test[14] == 14); assert(test[15] == 15); assert(test[16] == 16); assert(test[17] == 17); assert(test[18] == 18); 
    
    assert(test.order_of_key(0) == 0); assert(test.order_of_key(1) == 1); assert(test.order_of_key(2) == 2); assert(test.order_of_key(3) == 3); assert(test.order_of_key(4) == 4); assert(test.order_of_key(5) == 5); assert(test.order_of_key(6) == 6); assert(test.order_of_key(7) == 7); assert(test.order_of_key(8) == 8); assert(test.order_of_key(9) == 9); test.remove(7);
}

void test3()
{
    avl_tree<std::string> test;

    test.insert("coke"); test.insert("ham"); test.insert("bread"); test.insert("shroom"); test.remove("bread"); test.insert("bread");
    
    test.remove("bread");
}

void test4()
{
    avl_tree<std::pair<size_t, std::string>> test;

    test.insert(std::make_pair(32, "coke")); test.insert(std::make_pair(1, "bread")); test.insert(std::make_pair(2, "ham")); test.insert(std::make_pair(12, "mushrooms"));
    test.remove(std::make_pair(2, "ham"));
    test.insert(std::make_pair(13, "bread"));

    assert(test.size() == 4);
}

void test5()
{
    avl_tree<size_t> test;
    
    test.insert(32).insert(1).insert(2).insert(12);
    test.print();
    test.remove(2);
    test.print();
    test.insert(13);
    test.print();
    
    assert(test.size() == 4);
}

void test6()
{
    avl_tree<size_t> test;
    
    test.insert(32).insert(2).insert(12);
    
    test.insert(13);
    
    test.print();

    assert(test.size() == 4);
}

signed main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}
#endif

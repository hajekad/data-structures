#include <bits/stdc++.h>

template < typename Amount, typename Key >
struct Node
{
    std::shared_ptr<Node<Amount, Key>> left;
    std::shared_ptr<Node<Amount, Key>> right;

    Amount amount;
    Amount sumOfSubtree;
    Key key;

    size_t height;
    size_t size;

    Node(const Amount & amount, const Key & key)
    {
        height = 1;
        size = 1;
        this->amount = amount;
        this->sumOfSubtree = amount;
        this->key = key;
        left = nullptr;
        right = nullptr;
    }

    const int compare(const  std::shared_ptr<Node<Amount, Key>> & right) const
    {
        if(this->amount < right->amount)
            return -1;

        if(this->amount > right->amount)
            return 1;

        if(this->key < right->key)
            return -1;

        if(this->key > right->key)
            return 1;

        return 0;

    }

    const int compare(const Amount & amount, const Key & key) const
    {
        if(this->amount < amount)
            return -1;

        if(this->amount > amount)
            return 1;

        if(this->key < key)
            return -1;

        if(this->key > key)
            return 1;

        return 0;

    }
};

template < typename Amount, typename Key >
class ordered_map
{
    std::shared_ptr<Node<Amount, Key>> root;

    const Amount getSumOfSubtree(const std::shared_ptr<Node<Amount, Key>> & node) const
    {
        if(!node)
            throw std::out_of_range("node is null therefore im gonna die");

        if(!node->left && !node->right)
            return node->amount;

        if(!node->left)
            return node->amount + node->right->sumOfSubtree;

        if(!node->right)
            return node->amount + node->left->sumOfSubtree;

        return node->amount + node->left->sumOfSubtree + node->right->sumOfSubtree;
    }

    const size_t height(const std::shared_ptr<Node<Amount, Key>> & node) const
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

    const size_t sizeOfSubtree(const std::shared_ptr<Node<Amount, Key>> & node) const
    {
        if(!node)
            return 0;

        return node->size;
    }

    const std::shared_ptr<Node<Amount, Key>> findSmallest(const std::shared_ptr<Node<Amount, Key>> & node)
    {
        auto tmp = node;

        while(tmp->left)
            tmp = tmp->left;

        return tmp;
    }

    std::shared_ptr<Node<Amount, Key>> leftRotate(std::shared_ptr<Node<Amount, Key>> & parent)
    {
        std::shared_ptr<Node<Amount, Key>> child = parent->right;
        
        if(!child)
            return rightRotate(parent);

        std::shared_ptr<Node<Amount, Key>> grandChild = child->left;
        
        parent->right = grandChild;

        child->left = updateNode(parent);
                    
        parent->height = greater(height(parent->left), height(parent->right)) + 1;
        child->height = greater(height(child->left), height(child->right)) + 1;

        parent->size = sizeOfSubtree(parent->left) + sizeOfSubtree(parent->right) + 1;
        child->size = sizeOfSubtree(child->left) + sizeOfSubtree(child->right) + 1;
    
        return updateNode(child);
    }

    std::shared_ptr<Node<Amount, Key>> rightRotate(std::shared_ptr<Node<Amount, Key>> & parent)
    {
        std::shared_ptr<Node<Amount, Key>> child = parent->left;

        if(!child)
            return leftRotate(parent);

        std::shared_ptr<Node<Amount, Key>> grandChild = child->right;
    
        parent->left = grandChild;

        child->right = updateNode(parent);
        
        parent->height = greater(height(parent->left), height(parent->right)) + 1;
        child->height = greater(height(child->left), height(child->right)) + 1;

        parent->size = sizeOfSubtree(parent->left) + sizeOfSubtree(parent->right) + 1;
        child->size = sizeOfSubtree(child->left) + sizeOfSubtree(child->right) + 1;
    
        return updateNode(child);
    }

    std::shared_ptr<Node<Amount, Key>> updateNode(std::shared_ptr<Node<Amount, Key>> & node)
    {
        long int heightRatio;
        std::shared_ptr<Node<Amount, Key>> ret = node;
                
        heightRatio = height(node->left) - height(node->right);

        if(heightRatio > 1)
        {
            int leftCmp = node->compare(node->left);

            if(leftCmp < 0)
                ret = rightRotate(node);
            else if(leftCmp > 0)
            {
                node->left = leftRotate(node->left);
                ret = rightRotate(node);
            }
        }
        else if(heightRatio < -1)
        {
            int rightCmp = node->compare(node->right);

            if(rightCmp > 0)
                ret = leftRotate(node);
            else if(rightCmp < 0)
            {
                node->right = rightRotate(node->right);
                ret = leftRotate(node);
            }
        }

        node->height = greater(height(node->left), height(node->right)) + 1;
        node->size = sizeOfSubtree(node->right) + sizeOfSubtree(node->left) + 1;

        try{ ret->sumOfSubtree = getSumOfSubtree(ret); } catch(const std::exception& e) { ret->sumOfSubtree = ret->amount; }
        
        return ret;
    }
    
    std::shared_ptr<Node<Amount, Key>> removeRec(std::shared_ptr<Node<Amount, Key>> & node,  const Amount & amount, const Key & key)
    {
        if (!node)
            return node;

        int cmp = node->compare(amount, key);

        if (cmp > 0)
           node->left = removeRec(node->left, amount, key);
        else if (cmp < 0)
           node->right = removeRec(node->right, amount, key);
        else
        {
            if (!node->left)
               return node->right;

            if (!node->right)
               return node->left;
            
            auto tmp = findSmallest(node->right);
            node->key = tmp->key;
            node->amount = tmp->amount;
            node->right = removeRec(node->right, tmp->amount, tmp->key);
        }
        
        return updateNode(node);
    }

    std::shared_ptr<Node<Amount, Key>> insertRec(std::shared_ptr<Node<Amount, Key>> & node, const Amount & amount, const Key & key)
    {
        if (!node)
            return std::make_shared<Node<Amount, Key>>(amount, key);
        
        int cmp = node->compare(amount, key);

        if (cmp > 0)
        {
            node->left = insertRec(node->left, amount, key);
        }
        else if (cmp < 0)
        {
            node->right = insertRec(node->right, amount, key);
        }
        else
            return node;

        return updateNode(node);
    }

    const Key & getKey(const std::shared_ptr<Node<Amount, Key>> & node, const size_t & targetIndex, size_t & lastIndexVisited) const
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

        return node->key;
    }

    const Amount & getAmount(const std::shared_ptr<Node<Amount, Key>> & node, const size_t & targetIndex, size_t & lastIndexVisited) const
    {
        if(lastIndexVisited < targetIndex)
        {
            lastIndexVisited = lastIndexVisited + sizeOfSubtree(node->right->left) + 1;

            return getAmount(node->right, targetIndex, lastIndexVisited);
        }
        else if(lastIndexVisited > targetIndex)
        {
            lastIndexVisited = lastIndexVisited - (sizeOfSubtree(node->left->right) + 1);

            return getAmount(node->left, targetIndex, lastIndexVisited);
        }

        return node->amount;
    }

    const size_t order_of_key_rec(const std::shared_ptr<Node<Amount, Key>> & node, const Amount & amount, const Key & key, size_t & lastIndexVisited) const
    {
        int cmp = node->compare(amount, key);
        if(cmp > 0)
        {
            if(!node->left)
                throw std::out_of_range("Key is not present.");

            lastIndexVisited = lastIndexVisited - (sizeOfSubtree(node->left->right) + 1);

            return order_of_key_rec(node->left, amount, key, lastIndexVisited);
        }
        else if(cmp < 0)
        {
            if(!node->right)
                throw std::out_of_range("Key is not present.");

            lastIndexVisited = lastIndexVisited + sizeOfSubtree(node->right->left) + 1;

            return order_of_key_rec(node->right, amount, key, lastIndexVisited);
        }

        return lastIndexVisited;
    }

    const Amount prefixSumRec(const std::shared_ptr<Node<Amount, Key>> & node, const size_t & from, const size_t & to, const size_t & lastIndexVisited) const
    {
        Amount ret = 0;

        //std::cerr << "Index: " << lastIndexVisited << " Value: " << node->amount << " From: " << from << " To: " << to << std::endl;

        if(lastIndexVisited >= from && lastIndexVisited <= to)
        {
            ret += node->amount;
        }

        if(node->left)
        {
            size_t leftIndex = lastIndexVisited - sizeOfSubtree(node->left->right) - 1;

            if(lastIndexVisited - sizeOfSubtree(node->left) >= from && lastIndexVisited - 1 <= to)
            {
                ret += getSumOfSubtree(node->left);
            }
            else if(node->left && lastIndexVisited > from)
            {
                ret += prefixSumRec(node->left, from, to, leftIndex);
            }
        }

        if(node->right)
        {
            size_t rightIndex = lastIndexVisited + sizeOfSubtree(node->right->left) + 1;

            if(lastIndexVisited + sizeOfSubtree(node->right) + 1 <= to && lastIndexVisited + 1 >= from)
            {
                //std::cerr << getSumOfSubtree(node->right) << std::endl;
                ret += getSumOfSubtree(node->right);
            }
            else if(node->right && lastIndexVisited < to)
            {
                ret += prefixSumRec(node->right, from, to, rightIndex);
            }
        }

        return ret;
    }

    size_t first_same_rec(const std::shared_ptr<Node<Amount, Key>> & node, const Amount & target)
    {
        if(target < node->amount)
        {

        }
        else if(target > node->amount)
        {
            
        }

        return 0;
    }

    size_t last_same_rec(const std::shared_ptr<Node<Amount, Key>> & node, const Amount & target)
    {
        return 0;
    }

    const void print2DUtil(const std::shared_ptr<Node<Amount, Key>> & root, int space) const
    {
        if (!root)
            return;
    
        space += 10;
    
        print2DUtil(root->right, space);
    
        std::cerr << std::endl;
        for (int i = 10; i < space; i++)
            std::cerr << " ";
        std::cerr << root->amount << ' ' << root->key << ' ' << root->sumOfSubtree << "\n";
        print2DUtil(root->left, space);
    }

public:
    ordered_map<Amount, Key> & insert(const Amount & amount, const Key & key)
    {
        root = insertRec(root, amount, key);
        
        return *this;
    }

    ordered_map<Amount, Key> &remove(const Amount &amount, const Key &key)
    {
        root = removeRec(root, amount, key);

        return *this;
    }

    const size_t size() const
    {
        return sizeOfSubtree(root);
    }

    const Amount prefixSumAmount(const size_t & from, const size_t & to) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        if(from > to || from < 0 || to >= size() || !root)
            throw std::out_of_range("findKey()");

        return prefixSumRec(root, from, to, startingIndex);
    }

    const size_t order_of_key(const Amount & amount, const Key & key) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        return order_of_key_rec(root, amount, key, startingIndex);
    }

    const Key & findKey(const size_t & target) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        if(target >= size())
            throw std::out_of_range("finKey()");

        return getKey(root, target, startingIndex);
    }

    const Amount & findAmount(const size_t & target) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        if(target >= size())
            throw std::out_of_range("findAmount()");

        return getAmount(root, target, startingIndex);
    }

    size_t first_same(const Amount & target) const
    {
        return first_same_rec(root, target);
    }
    
    size_t last_same(const Amount & target) const
    {
        return last_same_rec(root, target);
    }

    const Key & operator[](const size_t & target) const
    {
        size_t startingIndex = sizeOfSubtree(root->left);

        if(target >= size())
            throw std::out_of_range("operator[]");

        return getKey(root, target, startingIndex);
    }

    const void print() const
    {
        std::cerr << "Size: " << sizeOfSubtree(root) << std::endl << "Height: " << root->height << std::endl;

        print2DUtil(root, 0);

        std::cerr << std::endl << std::endl;
    }
};

#ifndef __TEST__

void test1()
{
  ordered_map<size_t, std::string> T;
  T.insert(15, "34werdfq");
  T.insert(7 , "324rfc");
  T.insert(23, "dfasgt443");
  T.insert(3 , "fcvdert3425");
  T.insert(11, "gdsfg4523");
  T.insert(19, "234t43tg");
  T.insert(27, "cvsdgsdrgjjregi");
  T.insert(1 , "sdfgsdfggswrertc");
  T.insert(5 , "sdrgfvsdfg");
  T.insert(9 , "srdfgv");
  T.insert(13, "drfgr");
  T.insert(17, "sdrgvsdrgtsr");
  T.insert(21, "srdtgsdrt");
  T.insert(25, "serfgtsd");
  T.insert(29, "dfgsrg");

  T.print();

  auto tmp = T.prefixSumAmount(5,10);

  std::cerr << "T.sold(0,14)  : " << T.prefixSumAmount(0,14)  << std::endl;
  std::cerr << "T.sold(0,6)  : " << T.prefixSumAmount(0,6)  << std::endl;
  std::cerr << "T.sold(8,14)  : " << T.prefixSumAmount(8,14)  << std::endl;
  std::cerr << "T.sold(0,3)  : " << T.prefixSumAmount(0,3) << std::endl;
  std::cerr << "T.sold(0,14)  : " << T.prefixSumAmount(0,14)  << std::endl;
  std::cerr << "T.sold(0,14)  : " << T.prefixSumAmount(0,14)  << std::endl;
  std::cerr << "T.sold(1,2)  : " << T.prefixSumAmount(1,2)  << std::endl;
  std::cerr << "T.sold(5,10) : " << tmp << std::endl;
  std::cerr << "T.sold(4,12) : " << T.prefixSumAmount(4,12) << std::endl;
  std::cerr << "T.sold(6,7)  : " << T.prefixSumAmount(6,7)  << std::endl;
  std::cerr << "T.sold(9,13) : " << T.prefixSumAmount(9,13) << std::endl;
  std::cerr << "T.sold(4,14) : " << T.prefixSumAmount(4,14) << std::endl;
  std::cerr << "T.sold(5,13) : " << T.prefixSumAmount(5,13) << std::endl;
  std::cerr << "T.sold(10,11): " << T.prefixSumAmount(10,11)<< std::endl;
  std::cerr << "T.sold(13,14): " << T.prefixSumAmount(13,14)<< std::endl;
  std::cerr << "T.sold(5,8)  : " << T.prefixSumAmount(5,8)  << std::endl;
  std::cerr << "T.sold(1,14) : " << T.prefixSumAmount(1,14) << std::endl;
  std::cerr << "T.sold(7,12) : " << T.prefixSumAmount(7,12) << std::endl;
}

signed main()
{
    test1();

    return 0;
}
#endif

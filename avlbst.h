#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* predecessor( AVLNode<Key, Value>* node );
    void fixRemove( int d, AVLNode<Key, Value>* node);
    void rotateToLeft( AVLNode<Key, Value>* n);
    void rotateToRight( AVLNode<Key, Value>* node);
    void fix( AVLNode<Key, Value>* p, AVLNode<Key, Value>* c);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* p = NULL;
    AVLNode<Key, Value>* newC =new AVLNode<Key, Value>( new_item.first, new_item.second, NULL );
    AVLNode<Key, Value>* c = static_cast<AVLNode<Key, Value>*>(this->root_);
    newC->setBalance(0); newC->setRight(NULL); newC->setLeft(NULL);

    if( this->root_ == NULL ){this->root_ = newC;}
    else
    {			while( c != NULL )
			{
				p = c;
				if( (p->getRight() == NULL) && ( p->getKey() < new_item.first )  ) 
				{
						newC->setParent( p );
						p->setRight( newC );
						break;
				}
				else if(  p->getKey() < new_item.first  )
				{
						c = p->getRight();
				}
				if((p->getLeft() == NULL) && ( new_item.first < p->getKey() ) )
				{
						newC->setParent( c );
            p->setLeft( newC );
						break;
				}
        else if( new_item.first < p->getKey() )
				{
						c = p->getLeft();
				}
        if( new_item.first == p->getKey() )
				{
						p->setValue( new_item.second );
                        delete newC;
						return;
				}
				
		}

		if( p->getBalance() == 1 || p->getBalance() == -1 )
		{
				p->setBalance(0);
				return;
		}
		else
		{
				if( p->getLeft() != newC )
				{p->setBalance(1);}
				else
				{p->setBalance(-1);}
				fix(p, newC);
			}
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateToRight(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* leftChild = node->getLeft();
    leftChild->setParent(parent);
    
    if (!parent) {
        this->root_ = leftChild;
    } else if (parent->getRight() == node) {
        parent->setRight(leftChild);
    } else {
        parent->setLeft(leftChild);
    }

    AVLNode<Key, Value>* rightGrandChild = leftChild->getRight();
    leftChild->setRight(node);
    node->setLeft(rightGrandChild);
    node->setParent(leftChild);
    
    if (rightGrandChild) {
        rightGrandChild->setParent(node);
    }
}




template<class Key, class Value>
void AVLTree<Key, Value>::rotateToLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* rightChild = node->getRight();
    rightChild->setParent(parent);

    if (!parent) {
        this->root_ = node->getRight();
    } else if (parent->getRight() == node) {
        parent->setRight(rightChild);
    } else {
        parent->setLeft(rightChild);
    }

    AVLNode<Key, Value>* leftGrandChild = rightChild->getLeft();

    rightChild->setLeft(node);
    node->setRight(leftGrandChild);
    node->setParent(rightChild);

    if (leftGrandChild) {
        leftGrandChild->setParent(node);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::fix( AVLNode<Key, Value>* p, AVLNode<Key, Value> *c )
{

    if(  p->getParent() == NULL || p == NULL )
    {
        return;
    }
    AVLNode<Key, Value>* gp = p->getParent();
    if( gp->getRight() == p )
    {
        gp->setBalance( gp->getBalance() + 1 );

        if( gp->getBalance() == 1 )
        {
            fix( gp, p );
            return;
        }

        if( gp->getBalance() == 0 ){return;}

        if( c == p->getRight() )
        {
            rotateToLeft( gp );
            p->setBalance(0);
            gp->setBalance(0);
        }
        else
        {
            rotateToRight(p);
            rotateToLeft( gp );
            if( c->getBalance() == 0 )
            {
                p->setBalance(0);
                gp->setBalance(0);
            }
            if ( c->getBalance() == -1 )
            {
                p->setBalance(1);
                gp->setBalance(0);
            }
            if( c->getBalance() == 1 )
            {
                p->setBalance(0);
                gp->setBalance(-1);
            }
            c->setBalance(0);
        }
    }
        
    else 
    {
        gp->setBalance( gp->getBalance() - 1 );
        if( gp->getBalance() == -1 )
        {
            fix( gp, p );
            return;
        }
        if( gp->getBalance() == 0 ){return;}
        if( p->getLeft() == c)
        {
            rotateToRight( gp );
            gp->setBalance(0);
            p->setBalance(0);
        }
        else
        {
            rotateToLeft(p);
            rotateToRight(gp);
            if( c->getBalance() == 0 )
            {
                p->setBalance(0);
                gp->setBalance(0);
            }
            if(c->getBalance() == 1)
            {
                p->setBalance(-1);
                gp->setBalance(0);
            }
            if( c->getBalance() == -1 )
            {
                p->setBalance(0);
                gp->setBalance(1);
            }
            
            c->setBalance(0);
        }
    }
    
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    int d;
  AVLNode<Key, Value>* t = static_cast<AVLNode<Key, Value>*>( this->internalFind(key));
  AVLNode<Key, Value>* c = NULL;
  if( t == NULL )
  {
    return;
  }

  if( t->getRight() != NULL && t->getLeft() != NULL)
  {
    AVLNode<Key, Value>* p = predecessor( t );
    nodeSwap( t, p );
  }
  AVLNode<Key, Value>* pa = t->getParent();
  if(t->getRight() != NULL && t->getLeft() == NULL )
  {
    c = t->getRight();
  }
  if( t->getLeft() != NULL && t->getRight() == NULL)
  {
    c = t->getLeft();
  }

  if( pa == NULL )
  {
        this->root_ = c;  
    }  

  else
  {
        if( pa->getRight()==t )
        {
            d = -1;
            pa->setRight( c );
        }
        else
        {
            d = 1;
            pa->setLeft( c );
        }
  }
  if( c != NULL )
  {
    c->setParent( pa );
  }
 delete t; fixRemove(  d, pa );
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixRemove( int d, AVLNode<Key, Value>* node )
{
    int nodeD=0;

    if( node == NULL )
    {
        return;
    }
    int balance = node->getBalance();
    AVLNode<Key, Value>* parent = node->getParent();
    if( parent!= NULL )
    {
        if(parent->getRight() == node )
        {
            nodeD = -1;
        }
        else
        {
            nodeD = 1;
            
        }
    }
  
    if( balance == 1 && d == 1 )
    {
        AVLNode<Key, Value>* c = node->getRight();
        int cBalance = c->getBalance();
        if( cBalance == 1 )
        {
            rotateToLeft(node);
            node->setBalance(0);
            c->setBalance(0);
        }
        else if( cBalance == 0 )
        {
            rotateToLeft(node);
            node->setBalance(1);
            c->setBalance(-1);
            return;
        }   
        else if( cBalance == -1 )
        {
            AVLNode<Key, Value>* g = c->getLeft();
            int gBalance = g->getBalance();
            rotateToRight(c);
            rotateToLeft(node);
            if( gBalance == -1)
            {
                node->setBalance(0);
                c->setBalance(1);
                g->setBalance(0);
            }
            if( gBalance == 0 )
            {
                node->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            if( gBalance == 1 )
            {
                node->setBalance(-1);
                c->setBalance(0);
                g->setBalance(0);
            }
        }
        fixRemove( nodeD, parent );
    }
    else if( balance == -1 && d == -1 )
    {
        AVLNode<Key, Value>* child = node->getLeft();
        int cBalance = child->getBalance();
        if( cBalance == -1)
        {
            rotateToRight(node);
            child->setBalance(0);
            node->setBalance(0);
        }
        else if( cBalance == 0 )
        {
            rotateToRight(node);
            node->setBalance(-1);
            child->setBalance(1);
            return;
        }
        else if( child->getBalance() == 1 )
        {
            AVLNode<Key, Value>* g = child->getRight();
            int gBalance = g->getBalance();
            rotateToLeft(child);
            rotateToRight(node);
                if( gBalance == 1)
                {
                    node->setBalance(0);
                    child->setBalance(-1);
                    g->setBalance(0);
                }
                if( gBalance == 0 )
                {
                    node->setBalance(0);
                    child->setBalance(0);
                    g->setBalance(0);
                }
                if( gBalance == -1 )
                {
                    node->setBalance(1);
                    child->setBalance(0);
                    g->setBalance(0);
                }
        }
        fixRemove( nodeD, parent);
    }
    else {
      if (balance == -1 && d == 1) {
          node->setBalance(0);
          fixRemove(nodeD, parent);
      }
      if (balance == 1 && d == -1) {
          node->setBalance(0);
          fixRemove(nodeD, parent);
      }
      if (balance == 0) {
          node->setBalance(d);
          return;
      }
    } 
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessor( AVLNode<Key, Value>* n )
{
    if( n->getLeft() == NULL )
    {   
      AVLNode<Key, Value>* p = n->getParent();
        while( n == p->getLeft() && n->getParent() != NULL)
        {
            n = p;
            p = p->getParent();
        }
        return p;
    }
    else
    {
       n = n->getLeft();
        while( n->getRight() != NULL )
        {
            n = n->getRight();
        }
        return n;
        
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

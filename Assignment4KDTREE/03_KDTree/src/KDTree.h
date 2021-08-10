/**
 * File: KDTree.h
 * Author: (your name here)
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED
#include <stack>
#include "Point.h"
#include "BoundedPQueue.h"
#include <stdexcept>
#include <cmath>
#include <vector>
// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.
using namespace std;
template <size_t N,typename ElemType>class KDTree;
//实现一个KDTREE节点
template <size_t N,typename ElemType>class Node{
  friend  class KDTree<N,ElemType>;
private:
  Point<N> pt;
  ElemType elemtype;
  Node<N,ElemType> *left;
  Node<N,ElemType> *right;
  size_t compare;
  Node(Point<N>point,ElemType type,size_t com , Node<N,ElemType> *l = nullptr,Node<N,ElemType> *r = nullptr):pt(point),elemtype(type),compare(com),left(l),right(r){

  }
};

template <size_t N, typename ElemType>
class KDTree {
public:
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    typedef pair<Point<N>,ElemType> kdpair;
    typedef typename vector<kdpair>::iterator iterator;
    KDTree();
    
    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();
    
    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);
    
    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;
    
    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;
    
    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;
    
    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);
    
    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);
    
    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;
    
    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;

private:
    Node<N,ElemType> *root;
    size_t ds;
    size_t sz;
    void DeleteNode(Node<N,ElemType> *node);
    Node<N,ElemType> *search(const Point<N>& pt)const;
    void copy(Node<N,ElemType> *dst,const Node<N,ElemType> *src);
};

/** KDTree class implementation details */

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    sz = 0;
    ds = N;
    root = nullptr;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    sz = 0;
    DeleteNode(root);
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return ds;
}

template <size_t N, typename ElemType>
void KDTree<N,ElemType>::DeleteNode(Node<N,ElemType> *node){
    if(node == nullptr)return;
    if(node->left != nullptr)DeleteNode(node->left);
    if(node->right != nullptr)DeleteNode(node->right);
    delete node;

}
template <size_t N, typename ElemType>
size_t KDTree<N,ElemType>::size() const{
    return sz;
}
template <size_t N, typename ElemType>
bool KDTree<N,ElemType>::empty() const{
    return sz == 0;
};
template <size_t N, typename ElemType>
bool KDTree<N,ElemType>::contains(const Point<N>& pt) const{
    if(search(pt) == nullptr){
        return false;
    }else{
        return true;
    }
}
template <size_t N, typename ElemType>
Node<N,ElemType> *KDTree<N,ElemType>::search(const Point<N>& pt)const{
    Node<N,ElemType> *temp = root;
    while(temp != nullptr){
        if(temp->pt == pt){
            break;
        }
        size_t comp = temp->compare;
        if(pt[comp] <= temp->pt[comp]){
            temp = temp->left;
        }else{
            temp = temp->right;
        }
    }
    return temp;
}
template <size_t N, typename ElemType>
void KDTree<N,ElemType>::insert(const Point<N>& pt, const ElemType& value){
   Node<N,ElemType> *temp = root;
   Node<N,ElemType> *pre;
   size_t n = 0;
   while(temp != nullptr){
       if(temp->pt == pt){
           break;;
       }
       size_t comp = temp->compare;
       pre = temp;
       if(pt[comp] <= temp->pt[comp]){
           temp = temp->left;
       }else{
           temp = temp->right;
       }
       n++;
   }
   if(temp != nullptr){
       temp->pt = pt;
       temp->elemtype = value;
   }else{
       sz++;
       if(root == nullptr){
           root = new Node<N,ElemType>(pt,value,n%N);
       }else{
           int compareN = pre->compare;
           if(pt[compareN] >pre->pt[compareN]){
               //添加到右子树
               pre->right = new Node<N,ElemType>(pt,value,n%N);
           }else{
               pre->left =  new Node<N,ElemType>(pt,value,n%N);
           }
       }
   }
}
template <size_t N, typename ElemType>
ElemType& KDTree<N,ElemType>::operator[](const Point<N>& pt){
    Node<N,ElemType> *temp = root;
    Node<N,ElemType> *pre;
    size_t n = 0;
    while(temp != nullptr){
        if(temp->pt == pt){
            break;;
        }
        size_t comp = temp->compare;
        pre = temp;
        if(pt[comp] <= temp->pt[comp]){
            temp = temp->left;
        }else{
            temp = temp->right;
        }
        n++;
    }
    if(temp != nullptr){
        return temp->elemtype;
    }else{
        sz++;
        if(root == nullptr){
            root = new Node<N,ElemType>(pt,ElemType(),n%N);
            temp = root;
        }else{
            size_t compareN = pre->compare;
            if(pt[compareN] >pre->pt[compareN]){
                //添加到右子树
                pre->right = new Node<N,ElemType>(pt,ElemType(),n%N);
                temp = pre->right;
            }else{
                pre->left =  new Node<N,ElemType>(pt,ElemType(),n%N);
                temp = pre->left;
            }
        }
    }
    return temp->elemtype;
}
template <size_t N, typename ElemType>
ElemType& KDTree<N,ElemType>::at(const Point<N>& pt){
    Node<N,ElemType> * search_node = search(pt);
    if(search_node == nullptr){
        throw out_of_range("out_of_range");
    }else{
        return search_node->elemtype;
    }
}
template <size_t N, typename ElemType>
const ElemType& KDTree<N,ElemType>::at(const Point<N>& pt)const{
    return const_cast<KDTree<N, ElemType>* >(this)->at(pt);
}
template <size_t N, typename ElemType>
ElemType KDTree<N,ElemType>::kNNValue(const Point<N>& key, size_t k) const{
    BoundedPQueue<ElemType> bpq(k);
    Node<N,ElemType> *curr = root;
    stack<Node<N,ElemType> *> path;
    while(curr != nullptr){
        bpq.enqueue(curr->elemtype,Distance(key,curr->pt));
        path.push(curr);
        if(key[curr->compare] <= curr->pt[curr->compare]){
            curr = curr->left;
        }else{
            curr = curr->right;
        }
    }
    //(curri-yi) 这个条件要在path为空之后才能确定
    while(!path.empty()){
        Node<N,ElemType> *temp = path.top();
        path.pop();
        double dis = fabs(temp->pt[temp->compare] - key[temp->compare]);
        Node<N,ElemType> *searchNode = nullptr;
        if(dis < bpq.worst() || bpq.size() != bpq.maxSize()){
            if(key[temp->compare] <= temp->pt[temp->compare]){
                searchNode = temp->right;
            }else{
                searchNode = temp->left;
            }
            while(searchNode !=nullptr){
                bpq.enqueue(searchNode->elemtype,Distance(key,searchNode->pt));
                path.push(searchNode);
                if(key[searchNode->compare] <= searchNode->pt[searchNode->compare]){
                    searchNode = searchNode->left;
                }else{
                    searchNode = searchNode->right;
                }
            }
        }
    }
    ElemType e;
    int max = -1;
    map<ElemType,int> times;
    while(!bpq.empty()){
        ElemType tempE = bpq.dequeueMin();
        times[tempE]++;
        if(times[tempE] >max){
            e = tempE;
            max = times[tempE];
        }
    }
    return e;
}
template <size_t N, typename ElemType>
KDTree<N,ElemType>::KDTree(const KDTree &rhs){
    root = nullptr;
    this->operator=(rhs);
}
template <size_t N, typename ElemType>
void KDTree<N,ElemType>::copy(Node<N,ElemType> *dst,const Node<N,ElemType> *src){
    if(src == nullptr)return;
    dst = new Node<N,ElemType>(src->pt,src->elemtype,src->compare);
    copy(dst->left,src->left);
    copy(dst->right,src->right);
}
template <size_t N, typename ElemType>
KDTree<N,ElemType>& KDTree<N,ElemType>::operator=(const KDTree& rhs){
    if(this == &rhs)return *this;
    Node<N,ElemType> *new_KDTree = nullptr;
    try{
        copy(new_KDTree,rhs.root);
        DeleteNode(root);
        sz= rhs.sz;
        ds= rhs.ds;
        root = new_KDTree;
    }catch(...){
        DeleteNode(new_KDTree);
        throw;
    }

    return *this;
}
#endif // KDTREE_INCLUDED

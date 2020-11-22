#include <iostream>
#include <vector>
#include <math.h>       /* floor */
using namespace std; 

#define COUNT 1

class Node  
{  
    public: 
        int key;  
        int count;
        int height;
        Node *left;  
        Node *right;  
};  

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->key << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printBT(const Node* node)
{
    printBT("", node, false);    
}


void printTree(Node *root, int space)
{
   if (root == NULL)
      return;
   space += COUNT;
   printTree(root->right, space);
   for (int i = COUNT; i < space; i++)
      cout << "\t";
   cout << root->key << "\n";
   printTree(root->left, space);
}

int getCount(Node *node) 
{
    if(node == NULL)
        return 0;
    return node->count;
} 

int getHeight(Node *node) 
{
    if(node == NULL)
        return 0;
    return node->height;
}  

Node* newNode(int key)  
{  
    Node* node = new Node(); 
    node->key = key;  
    node->count = 1;
    node->height = 0;
    node->left = NULL;  
    node->right = NULL;  
    return(node);  
} 

void inOrder(Node* node, vector<Node*> &nodes) 
{ 
    if(node != NULL) {
        inOrder(node->left, nodes); 
        nodes.push_back(node); 
        inOrder(node->right, nodes); 
    }
}  

int getVerticesCount(Node *N)  
{  
    if (N == NULL)  
        return 0;  
    return N->count;  
} 

int _getCount(Node *node)
{
    if(node == NULL)
        return 0;
    return 1 + _getCount(node->left) + _getCount(node->right);
}

int _getHeight(Node *node)
{
    if(node == NULL)
        return 0;
    return 1 + max(_getHeight(node->left), _getHeight(node->right));
}

Node* makeBST(vector<Node*> &nodes, int from, int to) 
{ 
    if (from > to) 
        return NULL; 
  
    int mid = (from + to + 1) / 2; 
    Node *node = newNode(nodes[mid]->key);
    node->key = nodes[mid]->key;
  
    node->left  = makeBST(nodes, from, mid-1); 
    node->right = makeBST(nodes, mid+1, to); 

    // Update the number of vertices in left and right subtree
    node->count = 1 + _getCount(node->left) + _getCount(node->right);
    node->height = 1 + max(_getHeight(node->left), _getHeight(node->right));
  
    return node; 
} 

Node* updateCount(Node *node) 
{
    if(node == NULL)
        return node;
    // Update the number of vertices in left and right subtree
    node->count = 1 + _getCount(node->left) + _getCount(node->right);
    node->height = 1 + max(_getHeight(node->left), _getHeight(node->right));
    updateCount(node->left);
    updateCount(node->right);
    return node;
}

Node* buildBalancedBST(Node* node) 
{ 
    vector<Node *> nodes; 
    inOrder(node, nodes); 
  
    int n = nodes.size(); 
    Node *newNode = makeBST(nodes, 0, n-1); 
    /*cout << "\t-------->\n";
    printTree(newNode, 1);
    cout << "L: " << getCount(newNode->left) << ", R: " << getCount(newNode->right) << endl;
    newNode = updateCount(newNode);
    cout << "L: " << getCount(newNode->left) << ", R: " << getCount(newNode->right) << endl;*/
    return newNode;
} 


Node* insertion(Node* node, int key)  
{  
    /* 1. LEAF */
    if (node == NULL)  
        return(newNode(key));  
  
    /* 2. INNER NODE */
    if (key < node->key)  
        node->left = insertion(node->left, key);  
    else if (key > node->key)  
        node->right = insertion(node->right, key);  
    /* 3. EXISTING NODE */
    else 
        return node;  
  
    // Update the number of vertices in left and right subtree
    node->count = 1 + getCount(node->left) + getCount(node->right);
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Check the condition and make a perfect BTS
    if( abs(getCount(node->left) - getCount(node->right)) > floor( (max(getCount(node->left), getCount(node->right)) + 1) / 2) ) {
        printf("--------------\n");
        return buildBalancedBST(node);
    }
  
    return node;  
}  

Node * getMinimumNode(Node* node)  
{  
    Node* current = node;  
  
    while (current->left != NULL)  
        current = current->left;  
  
    return current;  
} 

Node* deletion(Node* node, int key)  
{        
    if (node == NULL)  
        return node;  
  
    if ( key < node->key )  
        node->left = deletion(node->left, key);  
  
    else if( key > node->key )  
        node->right = deletion(node->right, key);  
  
    else
    {  
        if( (node->left == NULL) ||  (node->right == NULL) )  
        {  
            Node *child = node->left ? node->left : node->right;  
  
            /* 1. LEAF */ 
            if (child == NULL)  
            {  
                child = node;  
                node = NULL;  
            }  
            /* 2. HAS ONE CHILD */
            else
                *node = *child; 
            free(child);  
        }  
        /* 3. INNER NODE (TWO CHILDREN) */
        else
        {  
            Node* child = getMinimumNode(node->right);  
            node->key = child->key;    
            node->right = deletion(node->right,  
                                     child->key);  
        }  
    }  
  
    if (node == NULL)  
        return node;  
    
    // Update the number of vertices in left and right subtree
    node->count = 1 + getCount(node->left) + getCount(node->right);
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Check the condition and make a perfect BTS
    if( abs(getCount(node->left) - getCount(node->right)) > floor( (max(getCount(node->left), getCount(node->right)) + 1) / 2) ) {
        printf("--------------\n");
        return buildBalancedBST(node);
    }

    return node;  
}  
 
void preOrder(Node* node) 
{ 
    if (node == NULL) 
        return; 
    printf("%d ", node->key); 
    preOrder(node->left); 
    preOrder(node->right); 
}

int main(void) 
{ 
    int N;
    scanf("%d\n", &N);
    Node *root = NULL;

    for(int i=0; i<N; i++) {
        int operation;
        scanf("%d ", &operation);
        if(operation < 0) {
            root = deletion(root, abs(operation));
        } else {
            root = insertion(root, operation);
        }
        printf("%d -> root %d L: %d , R: %d\n", operation, root->key, getCount(root->left), getCount(root->right));
        //printTree(root, 1);
        //preOrder(root);
    }
    /*printf("\n");
    preOrder(root);
    printf("\n");*/
    printBT(root);
    int L = getCount(root->left);
    cout << "\n";
    int R = getCount(root->right);
    cout << "\n";
    printf("%d %d %d\n", getHeight(root), L, R);

    return 0;
}

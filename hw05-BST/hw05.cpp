#include <iostream>
#include <vector>
#include <math.h>       /* floor */
#include <queue>
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

Node* insertion(Node* node, int key);

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

void inOrder(Node* node, vector<int> &nodes) 
{ 
    if(node != NULL) {
        inOrder(node->left, nodes); 
        nodes.push_back(node->key); 
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

void deleteTree(Node *node) 
{
    if(node == NULL)
        return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

vector<int> splitNodes(vector<int> l, int from, int to) 
{
    vector<int> ret;
    for(int i=from; i<=to; i++) {
        ret.push_back(l[i]);
    }
    return ret;
}

int getDepth(int verticesCnt)
{
    int cnt = 0;
    while(verticesCnt >= 2) {
        verticesCnt = verticesCnt / 2;
        cnt++;
    }
    return cnt;
}

int getRootIndex(vector<int> nodes) 
{
    int verticesCnt = (int) nodes.size();
    int d = getDepth(verticesCnt);
    int WoLastLayer = (int) pow(2, d) - 1;
    int lastLayer = verticesCnt - WoLastLayer;
    int rightSubtree = ((lastLayer / (int) pow(2, d-1)) < 1) ? 0 : (lastLayer - (int) pow(2, d-1));
    rightSubtree += (int) pow(2, d-1) - 1;
    return verticesCnt - rightSubtree - 1;
}

void printNodes(vector<int> nodes) 
{
    for(auto node : nodes)
        cout << node << "->";
    cout << endl;
}

Node* makeBalancedTree(vector<int> nodes){
    if((int)nodes.size() == 1) {
        Node *node = newNode((int) nodes.at(0));
        return node;
    } else if((int)nodes.size() == 0) {
        return NULL;
    }
    int rindex = getRootIndex(nodes);
    Node *node = newNode((int) nodes.at(rindex));
    vector<int> leftSubtree = splitNodes(nodes, 0, rindex-1); 
    vector<int> rightSubtree = splitNodes(nodes, rindex+1, (int) nodes.size()-1); 
    /*cout << "leftSubtree: ";
    printNodes(leftSubtree);
    cout << "rightSubtree: ";
    printNodes(rightSubtree);
    cout << endl;*/
    node->left = makeBalancedTree(leftSubtree);
    node->right = makeBalancedTree(rightSubtree);
    node->count = 1 + getCount(node->left) + getCount(node->right);
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    return node;
}

Node* buildBalancedBST(Node* node) 
{ 
    vector<int> nodes; 
    inOrder(node, nodes); 
    Node *newNode = makeBalancedTree(nodes);
    deleteTree(node);    
    /*cout << "\t-------->\n";
    printTree(newNode, 1);
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
        //printf("--------------\n");
        Node *nNode = buildBalancedBST(node);
        //printTree(nNode, 1);
        return nNode;
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
        //printf("--------------\n");
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
        //printf("%d -> root %d L: %d , R: %d\n", operation, root->key, getCount(root->left), getCount(root->right));
        //printTree(root, 1);
        //preOrder(root);
    }
    /*printf("\n");
    preOrder(root);
    printf("\n");*/
    //printBT(root);
    int L = getCount(root->left);
    //cout << "\n";
    int R = getCount(root->right);
    //cout << "\n";
    printf("%d %d %d\n", getHeight(root), L, R);

    return 0;
}

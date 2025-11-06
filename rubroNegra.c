#include <stdio.h>
#include <stdlib.h>

enum colors {RED, BLACK};
typedef enum colors Color;

typedef struct node {
    int value;
    Color color;
    struct node* dad;
    struct node* right;
    struct node* left;
} Node;

typedef struct tree {
    struct node* root;
    struct node* sheet;
} Tree;

Tree* createTree() {
    Tree *tree =  (Tree*) malloc(sizeof(Tree));
    Node *sheet = (Node*) malloc(sizeof(Node));
    sheet->color = BLACK;
    sheet->value = 0;
    sheet->dad = NULL;
    sheet->left = sheet;
    sheet->right = sheet;

    tree->root = sheet;
    tree->sheet = sheet;
    return tree;
}

Node* createNode(Tree *tree, Node *dad, int value) {
    Node* node = (Node*) malloc(sizeof(Node));

    node->dad = dad;
    node->value = value;
    node->left = tree->sheet;
    node->right = tree->sheet;
    node->color = RED;

    return node;
}

int empty(Tree *tree) {
    return (tree->root == tree->sheet);
}

Node* rse(Node* node, Tree *tree) {
    Node* right = node->right; //Pega o nó a direta do pai
    node->right = right->left; //Coloca o valor a esquerda do nó direto na direita do nó pai

    if (right->left != tree->sheet) {
        right->left->dad = node; //O filho à esquerda de "Direita" adota um nodevo pai
    }

    right->dad = node->dad; //atualiza o pai da "Direita" para seu avô

    if (node->dad == tree->sheet) {
        tree->root = right; //se o avô for nulo ele será a nodeva raiz
    } else if (node == node->dad->left) {
        node->dad->left = right; //Muda o filho do Avô
    } else {
        node->dad->right = right; //Muda o filho do Avô
    }

    right->left = node; //O nó pai virará filho à esquerda da "Direita"
    node->dad = right; //O filho coloca a 'Direita' como pai

    return right;
}

Node* rsd(Node* node, Tree *tree) {
    Node* left = node->left;
    node->left = left->right;

    if (left->right != tree->sheet) {
        left->right->dad = node;
    }

    left->dad = node->dad;

    if (node->dad == tree->sheet) {
        tree->root = left; //se o avô for nulo ele será a nodeva raiz
    } else if (node == node->dad->left) {
        node->dad->left = left; //Muda o filho do Avô
    } else {
        node->dad->right = left; //Muda o filho do Avô
    }

    left->right = node;
    node->dad = left;

    return left;
}

Node* rde(Node* node, Tree *tree) {
    node->right = rsd(node->right, tree);
    return rse(node, tree);
}

Node* rdd(Node* node, Tree *tree) {
    node->left = rse(node->left, tree);
    return rsd(node, tree);
}

//retornar nó pai
Node* runThrough(Node *node, int value, Tree *tree) {
    if(node->value > value){
        if(node->left == tree->sheet) return node;
        return runThrough(node->left, value, tree);
    }

    if(node->value < value){
        if(node->right == tree->sheet) return node;
        return runThrough(node->right, value, tree);
    }

    return node;
}

void runThroughPrint(Node *node, Tree *tree) {
    printf("\nFilho = %d; ", node->value);

    if (node->dad != tree->sheet) {
        printf("Dad = %d", node->dad->value);
    }

    if(node->left != tree->sheet){
        runThroughPrint(node->left, tree);
    }
    if(node->right != tree->sheet){
        runThroughPrint(node->right, tree);
    }
}

void balance(Tree *tree, Node* node) {
    while (node->dad->color == RED) {

        if (node->dad == tree->root) break;

        Node *grandpa = node->dad->dad;
        if (grandpa == tree->sheet) break;

        if (node->dad == grandpa->left) {
            Node* uncle = grandpa->right;

            if (uncle->color == RED) {
                uncle->color = BLACK;
                node->dad->color = BLACK;
                grandpa->color = RED;
                node = grandpa;
            } else {
                if (node == node->dad->right) {
                    node = node->dad;
                    rse(node, tree);
                }
                node->dad->color = BLACK;
                grandpa->color = RED;
                rsd(grandpa, tree);
            }
        } else {
            Node* uncle = grandpa->left;

            if (uncle->color == RED) {
                uncle->color = BLACK;
                node->dad->color = BLACK;
                grandpa->color = RED;
                node = grandpa;
            } else {
                if (node == node->dad->left) {
                    node = node->dad;
                    rsd(node, tree);
                }
                node->dad->color = BLACK;
                grandpa->color = RED;
                rse(grandpa, tree);
            }
        }
    }
    tree->root->color = BLACK;
}

Node *findDad(Tree *tree, int value) {
    Node *current = tree->root;
    Node *dad = tree->sheet;

    while (current != tree->sheet) {
        dad = current;
        if (value < current->value) {
            current = current->left;
        } else if (value > current->value) {
            current = current->right;
        } else {
            return NULL;
        }
    }

    return dad;
}

Node* add(Tree *tree, int value) {
    printf("\nAdding %d", value);

    if(tree->root == tree->sheet){
        Node *node = createNode(tree, tree->sheet, value);
        tree->root = node;
        node->color = BLACK;
        return node;
    }

    Node *dad = findDad(tree, value);
    if (dad == NULL) return NULL;
    Node *node = createNode(tree, dad, value);

    if (value < dad->value) {
        dad->left = node;
    } else {
        dad->right = node;
    }

    balance(tree, node);

    return node;
}

void transplant(Node *nodeToRemove, Node *nodeSubstitute, Tree *tree) {
    if (nodeToRemove->dad == tree->sheet) {
        tree->root = nodeSubstitute;
    } else if (nodeToRemove == nodeToRemove->dad->left) {
        nodeToRemove->dad->left = nodeSubstitute;
    } else {
        nodeToRemove->dad->right = nodeSubstitute;
    }
    nodeSubstitute->dad = nodeToRemove->dad;
}

Node *minimum(Tree *tree, Node *node) {
    while (node->left != tree->sheet) {
        node = node->left;
    }
    return node;
}

void deleteFixup(Tree *tree, Node *substitute) {
    while (substitute != tree->root && substitute->color == BLACK) {
        if (substitute->dad->left == substitute) {
            Node *brother = substitute->dad->right;

            if (brother->color == RED) {
                brother->color = BLACK;
                substitute->dad->color = RED;
                rse(substitute->dad, tree);
                brother = substitute->dad->right;
            }

            if (brother->left->color == BLACK && brother->right->color == BLACK) {
                brother->color = RED;
                substitute = substitute->dad;
            } else {
                if (brother->right->color == BLACK) {
                    brother->left->color = BLACK;
                    brother->color = RED;
                    rsd(brother, tree);
                    brother = substitute->dad->right;
                }

                brother->color = substitute->dad->color;
                substitute->dad->color = BLACK;
                brother->right->color = BLACK;
                rse(substitute->dad, tree);
                substitute = tree->root;
            }
        } else {
            Node *brother = substitute->dad->left;

            if (brother->color == RED) {
                brother->color = BLACK;
                substitute->dad->color = RED;
                rsd(substitute->dad, tree);
                brother = substitute->dad->left;
            }

            if (brother->right->color == BLACK && brother->left->color == BLACK) {
                brother->color = RED;
                substitute = substitute->dad;
            } else {
                if (brother->left->color == BLACK) {
                    brother->right->color = BLACK;
                    brother->color = RED;
                    rsd(brother, tree);
                    brother = substitute->dad->left;
                }

                brother->color = substitute->dad->color;
                substitute->dad->color = BLACK;
                brother->left->color = BLACK;
                rsd(substitute->dad, tree);
                substitute = tree->root;
            }
        }
    }
    substitute->color = BLACK;
}

void delete(Tree *tree,Node* nodeToRemove) {
    Node *aux = nodeToRemove;
    Node *substitute;
    Color origin_color = nodeToRemove->color;

    if (nodeToRemove->left == tree->sheet) {
        substitute = nodeToRemove->right;
        transplant(nodeToRemove, nodeToRemove->right, tree);
    } else if (nodeToRemove->right == tree->sheet) {
        substitute = nodeToRemove->left;
        transplant(nodeToRemove, nodeToRemove->left, tree);
    } else {
        aux = minimum(tree, nodeToRemove->right);
        origin_color = aux->color;
        substitute = aux->right;

        if (aux->dad != nodeToRemove) {
            transplant(aux, aux->right, tree);
            aux->right = nodeToRemove->right;
            aux->right->dad = aux;
        }

        transplant(nodeToRemove, aux, tree);
        aux->left = nodeToRemove->left;
        aux->left->dad = aux;
        aux->color = nodeToRemove->color;
    }

    if (origin_color == BLACK) {
        deleteFixup(tree, substitute);
    }

    free(nodeToRemove);
}

int main() {
    Tree* tree = createTree();

    for(int i = 1; i < 10; i++){
        add(tree, i);
    };

    runThroughPrint(tree->root, tree);

    Node *nodeRemove = runThrough(tree->root, 6, tree);
    delete(tree, nodeRemove);

    runThroughPrint(tree->root, tree);


    return 0;
}

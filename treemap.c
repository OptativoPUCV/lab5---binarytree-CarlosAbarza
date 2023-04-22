#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *tree = (TreeMap*) malloc(sizeof(TreeMap));
    tree->lower_than = lower_than;
    return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if (!searchTreeMap(tree, key)) {
    TreeNode *aux = createTreeNode(key, value);
    if (tree->lower_than(key,tree->current->pair->key)) {
      tree->current->left = aux;
      aux->parent = tree->current;
    }
    else {
      tree->current->right = aux;
      aux->parent = tree->current;
    }
    tree->current = aux;
  }
  return;
}

TreeNode * minimum(TreeNode * x){
  if (!x) 
    return NULL;
  TreeNode *aux = x;
  while (aux->left) {
    aux = aux->left;
  }
  return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if (is_equal(tree, tree->root->pair->key, node->pair->key)) {
    if (!node->left && !node->right) {
      tree->root = NULL;
    }
    else if (!node->left) {
      tree->root = node->right;
      tree->root->parent = NULL;
    }
    else if (!node->right) {
      tree->root = node->left;
      tree->root->parent = NULL;
    }
    else {
      TreeNode *aux = minimum(node->right);
      tree->root = aux;
      aux->left = node->left;
      aux->right = node->right;
      aux->parent = NULL;
      free(minimum(aux->right));
    }
  }

  else {
    if (!node->left && !node->right) {
      if (tree->lower_than(node->pair->key, node->parent->pair->key)) {
        node->parent->left = NULL;
      }
      else {
        node->parent->right = NULL;
      }
    }
    else if (!node->left) {
      TreeNode *aux = node->right;
      aux->parent = node->parent;
      if (tree->lower_than(aux->pair->key, aux->parent->pair->key)) {
        aux->parent->left = aux;
      }
      else {
        aux->parent->right = aux;
      }
    }
    else if (!node->right) {
      TreeNode *aux = node->left;
      aux->parent = node->parent;
      if (tree->lower_than(aux->pair->key, aux->parent->pair->key)) {
        aux->parent->left = aux;
      }
      else {
        aux->parent->right = aux;
      }
    }
    else {
      TreeNode *aux = minimum(node->right);
      aux->left = node->left;
      aux->right = node->right;
      aux->parent = node->parent;
      free(minimum(aux->right));
    }
  }
  free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode *aux = tree->root;
  while (aux) {
    if (is_equal(tree, key, aux->pair->key)){
      tree->current = aux;
      return aux->pair;
    }
    
    if (tree->lower_than(key,aux->pair->key)) {
      tree->current = aux;
      aux = aux->left;
    }
    else {
      tree->current = aux;
      aux = aux->right;
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  Pair *aux = searchTreeMap(tree, key);
  if (!aux) {
    if (tree->lower_than(tree->current->pair->key,key)) {
      nextTreeMap(tree);
    }
  }
  if (!tree->current)
    return NULL;
  return tree->current->pair;
  

  /*Pair *ub = NULL;
  TreeNode *aux = tree->root;
  while (aux) {
    if (is_equal(tree, key, aux->pair->key)){
      tree->current = aux;
      return aux->pair;
    }
    
    if (tree->lower_than(key,aux->pair->key)) {
      if (!ub) 
        ub = aux->pair;
      else {
        if (!tree->lower_than(ub, aux->pair->key)) {
          ub = aux->pair;
        }
      }
      
      tree->current = aux;
      aux = aux->left;
    }
    else {
      tree->current = aux;
      aux = aux->right;
    }
  }
  return ub;*/
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *aux = minimum(tree->root);
  tree->current = aux;
  return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if (!tree->current->right) {
    while (tree->current->parent) {
      if (tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key)){
        tree->current = tree->current->parent;
        return tree->current->pair;
      }
      tree->current = tree->current->parent;
    }
  }
  else {
    tree->current = minimum(tree->current->right);
    return tree->current->pair;
  }
  return NULL;
}

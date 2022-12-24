/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree != NULL)
  {
    if (tree->key == key)
      {
        *value = tree->value;
        return true;
      }
      else
      {
        if (tree->key > key)
          return bst_search(tree->left, key, value); // pujdeme do levyho podstromu
        else
          return bst_search(tree->right, key, value); //pujdeme do praveho podstromu
      }
  }
  else return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (*tree != NULL){// nude existuje 
    if ((*tree)->key > key)
      bst_insert(&(*tree)->left, key, value); // pujdeme do levyho podstromu
    else if ((*tree)->key < key)
      bst_insert(&(*tree)->right, key, value); //pujdeme do praveho podstromu
    else
      (*tree)->value = value; //nasli co hledali, assign hodnotu    
  }
  else //node neexistuje 
    {
      bst_node_t *NewElementPtr = (bst_node_t*)malloc(sizeof(bst_node_t));
      if (NewElementPtr != NULL) //kontrola allokace pameti
      {
        NewElementPtr->key = key;
        NewElementPtr->value = value;
        NewElementPtr->right = NULL; //podstrom jeste nema children
        NewElementPtr->left = NULL;
        *tree = NewElementPtr;
      }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if(*tree != NULL)
  {
    if ((*tree)->right != NULL) //pohyb po nejpravejisimu branchu
      bst_replace_by_rightmost(target, &(*tree)->right);
    else
    {
      bst_node_t *previousElementPtr = (*tree);
      target->key = (*tree)->key;
      target->value = (*tree)->value;
      *tree = (*tree)->left;
      free(previousElementPtr); 
    }
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree != NULL)
  { 
    bst_node_t *previousElementPtr = NULL;
    if (key == (*tree)->key) //uzel ma hodnotu key
    {
      if ((*tree)->left == NULL && (*tree)->right == NULL) //jestli childfree
      {
        previousElementPtr = *tree;
        *tree = (*tree)->right;
        free(previousElementPtr);
      }
      else if((*tree)->left != NULL && (*tree)->right == NULL)// ma jenom leveho syna
      {
        previousElementPtr = *tree;
        *tree = (*tree)->left;
        free(previousElementPtr);
      }
      else if ((*tree)->left == NULL && (*tree)->right != NULL) // ma jenom praveho syna
      {
        previousElementPtr = *tree;
        *tree = (*tree)->right;
        free(previousElementPtr);
      }
      else //ma a i pravyho a i levyho syny
        bst_replace_by_rightmost(*tree, &(*tree)->left);
    }
    else if ((*tree)->key > key) // pujdeme do levyho podstromu
      bst_delete(&((*tree)->left), key);
    else // pujdeme do praveho podstromu
      bst_delete(&((*tree)->right), key);
  }

}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  while (*tree != NULL)
  {
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    free(*tree);
    *tree = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  //root left right
  if (tree != NULL)
  {
    bst_print_node(tree);
    if (tree->left != NULL)
    {
      bst_preorder(tree->left);
      bst_preorder(tree->right);
    }   
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  //left root right
  if (tree != NULL)
  {
    if (tree->left != NULL)
    {
      bst_inorder(tree->left);
      bst_print_node(tree);
      bst_inorder(tree->right); 
    }
    else
      bst_print_node(tree);
   
    
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  //left right root
  if (tree != NULL)
  {
    if (tree->left != NULL)
    {
      bst_postorder(tree->left);
      bst_postorder(tree->right);
      bst_print_node(tree);
    }
    else
      bst_print_node(tree);
  }
}

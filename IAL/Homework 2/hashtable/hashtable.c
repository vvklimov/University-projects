/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  if (table != NULL)
  {
  for (int i = 0; i < HT_SIZE; i++)
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
ht_item_t *found = (*table)[get_hash(key)];
    while (found != NULL && strcmp(found->key,key))// pokud nenarazime na null nebo pokud nenajdeme prvok
        found = found->next;
    return found;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if (table != NULL) //kontrola ze tabulka vubec existuje 
  {
    ht_item_t *found = ht_search(table, key); //hledame prvek v tabulce
    if (found != NULL)
      found->value = value;
    else
    {
        found = (ht_item_t*)malloc(sizeof(ht_item_t)); //allocujeme pamet pod novy prvek
      if (found != NULL) //vydeleni pameti probehlo uspesne?
        {
          found->key = key;
          found->value = value;
          if ((*table)[get_hash(key)] != NULL)// uz misto nejakym prvkom bylo obsazeno, posuneme dal
            found->next = (*table)[get_hash(key)];
          else
            found->next = NULL;
          
          (*table)[get_hash(key)] = found;
        }
    
    }
  
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *found = ht_search(table, key); //hledame prvek v tabulce
  if (found != NULL)
    return &(found->value); // existuje - vracime hodnotu prvku, otherwise - vraceme null
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
ht_item_t *found = (*table)[get_hash(key)];
ht_item_t *previusElement = NULL;
    while ((strcmp(found->key, key) != 0) && found != NULL) //vyhledani hodnoty key bud pokud nenajdeme, nebo pokud nenarazime na NULL
    {
      previusElement = found; // predcozi prvek 
      found = found->next; //posouvani ukazatelu na dalsi prvek
    }
    if (found != NULL) //nic se nedeje pokud je null
    {
      if (previusElement != NULL) 
        previusElement->next = found->next; //prvek nebyl prvnim
      else
        (*table)[get_hash(key)] = found->next; // prvek byl prvnim 
      free(found);
    }
      


    
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if (table != NULL)
  {
    ht_item_t *activeElementPtr = NULL;
    ht_item_t *nextElementPtr = NULL;
   for (int i = 0; i < HT_SIZE; i++) // pohyb jako po vysce tabulky
   {
    activeElementPtr = (*table)[i];
    while(activeElementPtr != NULL) //mame zachovavat ukazatel na pristi element polozky pred jeho uvolnenim jinak ho ztratime, vypada jako pohyb po syrce 
    {
      nextElementPtr = activeElementPtr->next;
      free(activeElementPtr);
      activeElementPtr = nextElementPtr; 
    }
    (*table)[i] = NULL;
   }
  }

}
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	if (list->firstElement != NULL) // make sure ze list list obsahuje aspon jeden prvek aby bylo co rusit
	{	
		list->lastElement = NULL;
		list->activeElement = list->firstElement; // udelame prvni element aktivnym
		while(list->activeElement != NULL) 
		{	
			/*prvni element dostane hodnotu dalsiho elementu, na poslednim taktu bude mit hodnotu NULL*/
			list->firstElement = list->activeElement->nextElement;  // muzeme si predstavit jako posun vlevo
			free(list->activeElement); //uvolnujeme pamet na predchozi prvni prvek
			list->activeElement = list->firstElement;
		}
	}
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr newElementPtr = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // vydelim pamet na novy prvek
	if (newElementPtr!= NULL)
	{
		newElementPtr->data = data;	//assign hodnoty parametru data do novyho elementu
		newElementPtr->nextElement = list->firstElement; //dalsi element po novymu ukazuje na prvni element
		newElementPtr->previousElement = NULL; // element pred novym nikam ne ukazuje
		if (list->firstElement != NULL) //seznam uz byl neprazdny
			list->firstElement->previousElement = newElementPtr; //element pred prvnim ukazuje na novy element
		else
			list->lastElement = newElementPtr; //posledni element ukazuje na novy pokud seznam byl prazdny
		list->firstElement = newElementPtr; //prvni element ukazuje na novy
	}
	else
		DLL_Error();
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr newElementPtr = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // vydelim pamet na novy prvek
	if (newElementPtr!= NULL)
	{
		newElementPtr->data = data;	//assign hodnoty parametru data do novyho elementu
		newElementPtr->nextElement = NULL; //dalsi element po novymu nikam ne ukazuje
		newElementPtr->previousElement = list->lastElement; // predchozi element pred novym ukazuje na posledni element
		if (list->firstElement != NULL) //seznam uz byl neprazdny
			list->lastElement->nextElement = newElementPtr; //dalsi element po poslednimu ukazuje na novy element 
		else
			list->firstElement = newElementPtr; //prvni element ukazuje na novy pokud seznam byl prazdny
		list->lastElement = newElementPtr; //posledni element element ukazuje na novy
	}
	else
		DLL_Error();
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement != NULL)
		*dataPtr = list->firstElement->data;
	else
		DLL_Error();
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->firstElement != NULL)
		*dataPtr = list->lastElement->data;
	else
		DLL_Error();
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement != NULL)
	{
		if (list->firstElement == list->activeElement)
			list->activeElement = NULL;
		DLLElementPtr nextElement = list->firstElement->nextElement;
		free(list->firstElement);
		list->firstElement = nextElement;
		if (list->firstElement == NULL) //pokud seznam prazdny tak i lastElement mel by byt NULL
			list->lastElement = NULL;
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->lastElement != NULL)
	{
		if (list->lastElement == list->activeElement) //dame pryc aktivitu z lastElement pokud tam je
			list->activeElement = NULL;
		if (list->firstElement == list->lastElement)
			list->firstElement = NULL;
		DLLElementPtr previousElement = list->lastElement->previousElement; //umistime hodnotu predchoziho lastElement prkvu 
		free(list->lastElement);
		list->lastElement = previousElement;
		if (list->lastElement != NULL) //aby nebylo segmentation fault
			list->lastElement->nextElement = NULL;
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if ((list->activeElement != NULL) &&
		(list->activeElement != list->lastElement))
		{
			DLLElementPtr nextAfterNextElement = list->activeElement->nextElement->nextElement;
			free(list->activeElement->nextElement);
			if(nextAfterNextElement == NULL) //pokud jsme uvolnili posledni prvek, tak poslednim stane aktivni
				list->lastElement = list->activeElement;
			else
				nextAfterNextElement->previousElement = list->activeElement; //nechceme ztratit ukazatel, jinak bude chyba double (free)
			list->activeElement->nextElement = nextAfterNextElement;
		}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if ((list->activeElement != NULL) &&
		(list->activeElement != list->firstElement))
		{
			DLLElementPtr previousBeforePreviousElement = list->activeElement->previousElement->previousElement;
			free(list->activeElement->previousElement);
			if(previousBeforePreviousElement == NULL) //pokud jsme uvolnili prvni prvek, tak prvnim stane aktivni
				list->firstElement = list->activeElement;
			else
				previousBeforePreviousElement->nextElement = list->activeElement; //nechceme ztratit ukazatel, jinak bude chyba double (free)
			list->activeElement->previousElement = previousBeforePreviousElement;
		}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if (list->activeElement != NULL)
	{
		DLLElementPtr newElementPtr = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // vydelim pamet na novy prvek
		if (newElementPtr!= NULL)
			{
				newElementPtr->data = data;
				newElementPtr->nextElement = NULL;
				newElementPtr->previousElement = list->activeElement;
				if ((list->activeElement == list->firstElement) && (list->activeElement == list->lastElement))
				{
					list->firstElement->nextElement = newElementPtr;
					list->lastElement = newElementPtr;
				}
				else if ((list->activeElement == list->lastElement) && (list->activeElement != list->firstElement))
				{
					list->lastElement->nextElement =newElementPtr->previousElement;
					list->lastElement = newElementPtr;

				}
				else if ((list->activeElement != list->lastElement) && (list->activeElement == list->firstElement))
				{
					list->firstElement->nextElement = newElementPtr;
					newElementPtr->previousElement = list->firstElement;

				}
				else
					list->activeElement->nextElement = newElementPtr;
				list->activeElement->nextElement = newElementPtr;
				
				
			}
		else
			DLL_Error();
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (list->activeElement != NULL)
	{
		DLLElementPtr newElementPtr = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // vydelim pamet na novy prvek
		if (newElementPtr!= NULL)
			{
				newElementPtr->data = data;
				newElementPtr->nextElement = list->activeElement;
				//newElementPtr->previousElement = NULL;
				if ((list->activeElement == list->firstElement) && (list->activeElement == list->lastElement)) //ve seznamu 1 prvek
				{
					list->firstElement->previousElement = newElementPtr;
					list->firstElement = newElementPtr;
				}
				else if ((list->activeElement == list->lastElement) && (list->activeElement != list->firstElement))// v seznamu >=2 prvky a aktivni posledni
				{
					list->lastElement = newElementPtr->nextElement;
					list->lastElement->previousElement = newElementPtr;

				}
				else if ((list->activeElement != list->lastElement) && (list->activeElement == list->firstElement)) // v seznamu >=2 prvky a aktivni prvni
				{
					list->firstElement->previousElement = newElementPtr;
					list->firstElement = newElementPtr;

				}
				list->activeElement->previousElement = newElementPtr;
			}
		else
			DLL_Error();
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement != NULL)
		*dataPtr = list->activeElement->data;
	else
		DLL_Error();
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if(list->activeElement != NULL)
		list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL)
			list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement != NULL)
			list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return (list->activeElement != NULL);
}

/* Konec c206.c */

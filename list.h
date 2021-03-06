/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ16        */
/*Jména řešitelů: Sebastián Kisela, Ondrej Svoreň, Daniel Rudík,    */
/*                  Patrik Roman, Martin Chudý                      */
/*Loginy řešitelů: xkisel02, xsvore01, xrudik00, xroman10, xchudy04 */
/********************************************************************/
#ifndef list_h
#define list_h
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include "error.h"
// #include "interpret.list_h"

#define TRUE 1
#define FALSE 0
                                           /* Indikace, zda byl příklad řešen */
                                   /* Detects whether the task is implemented */
extern int solved;                         
                                                            /* Indikace chyby */
                                                                /* Error flag */
extern int errflg;
                                                             /* Prvek seznamu */
                                                /* Definition of list element */
typedef struct TElem {
    struct TElem *next;
    int data;
    int operation;
    void *add1;
    void *add2;
    void *add3;
} *TListItem;	               
                                                            /* Vlastní seznam */
                                                        /* Definition of list */
typedef struct { 
    struct TElem *Act;
    struct TElem *First;
} TList;
                                                   /* Funkce pro implementaci */
                                               /* Functions to be implemented */
void insert_instruction(TList *L, TListItem new);
TListItem create_instruction(int op, void * add1,void * add2,void * add3);

TList* InitList ();
void DisposeList (TList *);
void InsertFirst (TList *L, int operation, void* ad1, void *ad2, void *ad3);
void First (TList *);
// void CopyFirst (TList *, int *);
void DeleteFirst (TList *);
// void PostDelete (TList *);
TListItem PostInsert (TList *, int op,void* add1,void* add2,void* add3);
void Succ (TList *);
void Copy (TList *, int *);	
void Actualize (TList *, int);
int  Active (TList *);


#endif
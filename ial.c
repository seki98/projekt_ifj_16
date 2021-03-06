/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ16        */
/*Jména řešitelů: Sebastián Kisela, Ondrej Svoreň, Daniel Rudík,    */
/*                  Patrik Roman, Martin Chudý                      */
/*Loginy řešitelů: xkisel02, xsvore01, xrudik00, xroman10, xchudy04 */
/********************************************************************/

#include "ial.h"
//#include "builtin.h"
//#include "defs.h"
// tTablePtr BSTPrepare (tTablePtr new_node){

// 	new_node = malloc(sizeof(struct tTable));
// 	if(new_node)
// 		return new_node;
// }

//Knuth-Morris-Prattov algoritmus na vyhladavanie podretazca v retazci
int find(TVariable* s, TVariable* search){
    char* str = s->value.s;
    char* search_str = search->value.s;

    int size_s = strlen(str);
    int size_search = strlen(search_str);
    if(size_s < size_search)
        ret_error(10);

    if(size_search == 0)
        return 0;
    bool match = FALSE;
    //vytvorenie fail vektoru
    int *fail_v = malloc((size_search+1)* sizeof(int));
	if(fail_v == NULL){
		ret_error(INTERNAL_ERROR);
	}

    fail_v[0] = -1;
    for (int k = 0; k < size_search; ++k) {
        fail_v[k+1] = fail_v[k] + 1;
        while((fail_v[k+1] > 0) && (search_str[k] != search_str[fail_v[k+1]-1])) {
            fail_v[k+1] = fail_v[fail_v[k+1]-1]+1;
        }
    }


    //samotne vyhladavanie v retazci
    int i = 0;
    int j = 0;
    while (i<size_s && j<size_search) {
        //zhoda
	if (str[i] == search_str[j]) {
            match = TRUE;

            j++;
            i++;
        }
	//nezhoda
        else {
            if(j == 0)
                i++;
            match = FALSE;
            j = fail_v[j + 1];
        }
    }

        if(match == TRUE) {
            free(fail_v);
            return i-size_search;
        }

    free(fail_v);
    return -1;
}
//list-merge sort algoritmus na zoradenie retazca
char* sort(TVariable* s){
    char* str = s->value.s;

    int lngth = length(str);
    sorting(str, lngth);

    return str;
}

void sorting(char* s, int length){

    if(length <= 1)
        return;

    //delenie retazca na dve casti
    int mid = length/2;
    char *left = (char*) malloc(mid);
	if(left == NULL){
		ret_error(INTERNAL_ERROR);
	}
    for (int i = 0; i < mid; ++i) {
        left[i] = s[i];
    }

    char *right = (char*) malloc(length-mid);
	if(right == NULL){
		ret_error(INTERNAL_ERROR);
	}
    for (int j = mid; j < length; ++j) {
        right[j-mid] = s[j];
    }

    sorting(left, mid);
    sorting(right, length-mid);
    merge(s, left, mid, right, length-mid);
    free(left);
    free(right);

}
//spajanie + zoradovanie retazcov
void merge(char* s, char* left, int l_length, char* right, int r_length){
    int i = 0;
    int j = 0;

    while((i < l_length) && (j < r_length)){
        if(left[i] < right[j]){
            s[i+j] = left[i];
            i++;
        }
        else{
            s[i+j] = right[j];
            j++;
        }
    }
    //doplnenie retazca
    if (i < l_length){
        while (i < l_length) {
            s[i + j] = left[i];
            i++;
        }
    }
    else {
        while (j < r_length) {
            s[i + j] = right[j];
            j++;
        }
    }
}

void BSTInit (tTablePtr *RootPtr) {

	(*RootPtr) = NULL;
}



tTablePtr BSTSearch (tTablePtr RootPtr, char *node)	{
	//printf("som v BSTSearch\n");

	if( (RootPtr == NULL) || (node == NULL)  )
		return NULL;

	if( !strcmp(node, RootPtr->name) )
	{
		return RootPtr;
	}

	if( strcmp(node, RootPtr->name) < 0)
		if(BSTSearch(RootPtr->LPtr, node))
			return BSTSearch(RootPtr->LPtr, node);

	if( strcmp(node, RootPtr->name) > 0)
		if(BSTSearch(RootPtr->RPtr, node))
			return BSTSearch(RootPtr->RPtr, node);

	return NULL;
} 

void BSTRootNode(tTablePtr *RootPtr, tTablePtr *new, char *K){

	BSTInit(new);

	if( !(*new) )
	{
		(*new) = malloc(sizeof(struct tTable));
		if((*new) == NULL){
			ret_error(INTERNAL_ERROR);
		}
		(*new)->name = K;
		(*new)->LPtr = NULL;
		(*new)->RPtr = NULL;
		(*new)->Root = NULL;
		(*RootPtr)->Root = (*new);

		return;
	}


}




void BSTInsert (tTablePtr* RootPtr, tTablePtr* new, char *K)	{
	
	
	//printf("INSERT INTO %s\n",K);


	if( !(*RootPtr) )
	{
		(*new) = malloc(sizeof(struct tTable));
		if((*new) == NULL){
			ret_error(INTERNAL_ERROR);
		}
		(*new)->type = NODE_TYPE_NULL;
		(*new)->defined = 0;
		(*new)->name = K;
		(*new)->LPtr = NULL;
		(*new)->RPtr = NULL;
		(*new)->Root = NULL;
		(*RootPtr) = (*new);

		return;
	}

	
	
	if( !strcmp(K, (*RootPtr)->name) )
	{
		// printf("%s already exists %s %d\n",K, __FILE__, __LINE__);
		return;
	}

	if( strcmp(K, (*RootPtr)->name) < 0 )
		BSTInsert(&(*RootPtr)->LPtr, &(*new), K);
	
	if( strcmp(K, (*RootPtr)->name) > 0 )
		BSTInsert(&(*RootPtr)->RPtr, &(*new), K);
	
}

void ReplaceByRightmost (tTablePtr PtrReplaced, tTablePtr *RootPtr) {

	if( !(*RootPtr) )
		return;
	
	if((*RootPtr)->RPtr)
	{
		ReplaceByRightmost( PtrReplaced, &(*RootPtr)->RPtr);
	}
	else 
	{
	
		PtrReplaced->name = (*RootPtr)->name;
		PtrReplaced->data = (*RootPtr)->data;
		tTablePtr del = (*RootPtr);
		(*RootPtr) = (*RootPtr)->LPtr;
		
		free(del);

	}


	
}

void BSTDelete (tTablePtr *RootPtr, char* K) 		{


	if(! (*RootPtr) )
		return;

	if( strcmp(K, (*RootPtr)->name) < 0 )
	{
		BSTDelete(&(*RootPtr)->LPtr, K);
	}

	if( strcmp(K, (*RootPtr)->name) > 0 )
	{
		BSTDelete(&(*RootPtr)->RPtr, K);
	}

	if( !strcmp(K, (*RootPtr)->name) )
	{
		
		tTablePtr del = *RootPtr;

		if( (*RootPtr)->LPtr && (*RootPtr)->RPtr )
		{
			ReplaceByRightmost( (*RootPtr), &(*RootPtr)->LPtr );
		}
		else if( (*RootPtr)->LPtr )
		{
			(*RootPtr) = (*RootPtr)->LPtr;
		}
		else if( (*RootPtr)->RPtr )
		{
			(*RootPtr) = (*RootPtr)->RPtr;

		}
		if( !(*RootPtr)->LPtr && !(*RootPtr)->RPtr)  
		{
			(*RootPtr) = (*RootPtr)->LPtr;
			free(del);
		}
	}



} 


void BSTDispose (tTablePtr *RootPtr) 
{	
		if(*RootPtr)
		{
			
			BSTDispose(&(*RootPtr)->RPtr);
			BSTDispose(&(*RootPtr)->LPtr);

			if((*RootPtr)->Root)
			{
				BSTDispose(&(*RootPtr)->Root);
			}

			if((*RootPtr)->type == NODE_TYPE_VARIABLE)
			{
				freeVar((*RootPtr)->data.v);
			}
		
			else if((*RootPtr)->type == NODE_TYPE_FUNCTION)
			{
				freeFunc((*RootPtr)->data.f);
			}
			
			else if((*RootPtr)->type == NODE_TYPE_CLASS)
			{
				freeClass((*RootPtr)->data.c);
			}
		
			if((*RootPtr))
			{
				free(*RootPtr);
				(*RootPtr) = NULL;	
			}
		}
}
/******************************************************************************/
/**********************END OF CONTROLLED PART OF IAL.C*************************/
/******************************************************************************/

void freeVar(TVariable *var)
{
	if(var->type == VARTYPE_STRING)
	{
		char* string = var->value.s;
		free(string);
	}
	// printf("FREE VAR\n");
}

void freeFunc(TFunction *func)
{
	
	// TList *list = func->list;
	// TListItem ins = list->First;
	// TListItem nextIns = NULL;
	// while(ins)
	// {
	// 	nextIns = ins->next;
	// 	if(ins)
	// 	free(ins);
	// 	if(nextIns)
	// 		ins = nextIns;
	// 	else
	// 		break;
	// }
	
	// for(int i = func->stack->top; i >=0; i--)
	// {
	// 	TVariable *var = func->stack->data[i];
	// 	if(var)
	// 	free(var);
	// }
	// free(func->list);
	// free(func->stack);
	free(func);
	// printf("FREE FUNC\n");
}

void freeClass(TClass *cls)
{
	// for(int i = cls->stack->top; i >=0; i--)
	// {
	// 	TVariable *var = cls->stack->data[i];
	// 	// free(var);
	// }
	free(cls->stack);
	// printf("FREE CLASS\n");
}

TStack* copyStack(TStack* oldStack)
{
	TStack * newStack = stackInit();
	
	for(int i = 0; i <= oldStack->top; i++)
	{
		TVariable *var = oldStack->data[i];
		TVariable *newVar = malloc(sizeof(TVariable));
		if(newVar == NULL){
			ret_error(INTERNAL_ERROR);
		}
		newVar->name = NULL;
		newVar->className = NULL;
		if(var->className)
		{
			newVar->className = malloc(sizeof(char)*100);
			if(newVar->className == NULL){
				ret_error(INTERNAL_ERROR);
			}
			newVar->className = strcpy(newVar->className, var->className);
		}
		if(var->name)
		{
			newVar->name = malloc(sizeof(char)*100);
			if(newVar->name == NULL){
				ret_error(INTERNAL_ERROR);
			}
			newVar->name = strcpy(newVar->name,var->name);
		}
		newVar->value = 	var->value;
		newVar->type =		var->type;
		newVar->position =	var->position;
		newVar->constant =	var->constant;
		newVar->defined =	var->defined;
		newVar->declared =	var->declared;

		stackPush(newStack, newVar);
	}
	// printf("oldstack\n");
	// for(int i = 0; i <= oldStack->top ; i++)
	// {
	// 	TVariable *var = oldStack->data[i];
	// 	printf("name:%s type:%d position:%d\n",var->name, var->type, var->position);
	// }
	// printf("newstack\n");
	// for(int i = 0; i <= newStack->top ; i++)
	// {
	// 	TVariable *var = newStack->data[i];
	// 	printf("name:%s type:%d position:%d\n",var->name, var->type, var->position);
	// }
	return newStack;
}

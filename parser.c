/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ16        */
/*Jména řešitelů: Sebastián Kisela, Ondrej Svoreň, Daniel Rudík,    */
/*                  Patrik Roman, Martin Chudý                      */
/*Loginy řešitelů: xkisel02, xsvore01, xrudik00, xroman10, xchudy04 */
/********************************************************************/

#include "parser.h"
#include "error.h"

tTablePtr globTable;//v nej su tabulky tried | v kazdej tabulke triedy su jej funkcie a glob premenne.
Ttoken *token;//globalna premenna struktury token - do nej sa priradzuje vysledok getToken();
TStack *gStack;//zasobnik, pravdepodobne na prehladu o tom, ktora funkcia sa prave spracuvava

tTablePtr create_class_table(char* name)//navratova hodnota - uzol stromu
{
    tTablePtr node = BSTSearch(globTable, name);
    if(node == NULL){
        tTablePtr classTable;//vytvorenie pointera
        BSTInit(&classTable);//inicializacia(uzol = NULL)
        BSTInsert(&classTable, &classTable, name);//pridanie uzlu do tabulky a nastavenie atributu - name
        return classTable;
    }
    return node;//vrati uzol tabulky triedy
}

void parser_init()
{
	
	gStack = stackInit();
	
	BSTInit(&globTable);
	
	BSTInsert(&globTable, &globTable, "GlobTable");
	
	/*keywords to globtable*/
	// keywords_init();
	
}

void parser_finish() {
    BSTDispose(&globTable);

}

TFunction *new_function(Ttoken *token)
{	/*allocate the space for a new function*/
	TFunction *f;
	f = malloc(sizeof(TFunction));

	tTablePtr loc_table;
	BSTInit(&loc_table);

	TStack *stack = stackInit();
	
	f->stack = stack;
	/*assign the table to the function*/
	f->defined = 0;
	f->type = RET_INT;
	f->table = loc_table;
	f->name = token->data;
	return f;
}

TVariable *new_variable(Ttoken *token)
{	
	TVariable *v;
	v = malloc(sizeof(TVariable));
	v->declared = 0;
    v->name = token->data;
	return v;

}

void store_function(/*stack*/TFunction *f, tTablePtr *table) {
    tTablePtr new_func;
    BSTInit(&new_func);

    BSTInsert(table, &new_func, f->name);
    stackPush(gStack, f);
}
void store_variable(/*stack*/TVariable *v, tTablePtr *table)
{
	
	if( BSTSearch(*table, v->name) )
	{
		ret_error(SYNTAX_ERROR);
	}
		
	tTablePtr new_var;
	BSTInit(&new_var);

	BSTInsert(table, &new_var, v->name);
	new_var->data.v = v;
	
	
}


/*--------------------automat-----------------------*/
void starter() {
    token = get_token();
    while (token->type == KEYWORD_CLASS) {

        token = get_token();
        if (token->type != TOKEN_ID) {
            ret_error(SYNTAX_ERROR);
        }
        if (BSTSearch(classTable, token->data)) {
            ret_error(SEMANTIC_DEF_ERROR);
        }
        tTablePtr table = create_class_table(token->data);
        token = get_token();
        if (token->type != TOKEN_L_CURLY) {
            ret_error(SYNTAX_ERROR);
        }
        token = get_token();
        while (token->type == KEYWORD_STATIC) {
            token = get_token();
            Declaration(table, token);
            token = get_token();
        }
        if (token->type != TOKEN_R_CURLY) {
            ret_error(SYNTAX_ERROR);
        }
        token = get_token();
    }
//        if (token->data == TOKEN_EOF){
//            ret_error(0);
//        }
    //dosli sme na koniec programu
}

void Declaration(tTablePtr table, Ttoken *token) {
    Ttoken *tokenID;
    tTablePtr node = NULL;
    char *type = NULL;
    if ((token->type != TOKEN_TYPE) && ((token->type != KEYWORD_BOOLEAN) || (token->type != KEYWORD_VOID))) {  //TOKEN_TYPE <= int, string, double; TOKEN_BOOL plati len pre funkciu
        ret_error(SYNTAX_ERROR);
    }
    if ((token->type == KEYWORD_BOOLEAN) || (token->type == KEYWORD_VOID)) {
        type = token->data;
        token = get_token();
        if (token->type != TOKEN_ID) {
            ret_error(SYNTAX_ERROR);
        }
        tokenID = token;
        node = BSTSearch(table, tokenID);
        if (node != NULL) {
            if (node->data.v->declared == 1) {
                ret_error(SEMANTIC_DEF_ERROR);
            }
        }
        token = get_token();
        if (token->type != TOKEN_L_ROUND) {
            ret_error(SYNTAX_ERROR);
        }
        TFunction *f = funcDef(table, tokenID, type);
        f->defined = 1;
    } else {
        type = token->data;   //typ premennej resp funkcie

        token = get_token();
        if (token->type != TOKEN_ID) {
            ret_error(SYNTAX_ERROR);
        }

        tokenID = token;
        node = BSTSearch(table, tokenID);
        if (node != NULL) {
            if (node->data.v->declared == 1) {
                ret_error(SEMANTIC_DEF_ERROR);
            }
        }

        token = get_token();
        if (token->type != TOKEN_L_ROUND) {
            TVariable *v = variableDecl(table, tokenID, type);
            v->declared = 1;
        }
        TFunction *f = funcDef(table, tokenID, type);
        f->defined = 1;
    }
}

TVariable *variableDecl(tTablePtr table, Ttoken *tokenID, char *type) {
    TVariable *var;
    tTablePtr node = NULL;
    node = BSTSearch(table, tokedID->data);
    if(node == NULL){
        var = new_variable(tokenID);
        var->type = type;
    }
    else{
        var = node->data.v;
    }

    Ttoken *token = get_token();
    if (token->type == TOKEN_ASSIGN) {
        expression(var);
        token = get_token();
        if (token->type != TOKEN_SEM_CL) {
            ret_error(SYNTAX_ERROR);
        }

    } else if (token->type != TOKEN_SEM_CL) {
        ret_error(SEMANTIC_DEF_ERROR);
    }
    store_variable(var, &table);

    return var;
}


TFunction *funcDef(tTablePtr table, TToken *tokenID, char *funcType) {
    TFunction *f;
    TVariable *v;
    int paramsCount = 0;
    //char *type = NULL;
    f = new_function(tokenID);
    //f->type = funcType;
    f->params[paramsCount] = funcType;


    token = get_token();
    while (token->type != TOKEN_R_ROUND) {
        paramsCount++;
        params(f, token, paramsCount);
        token = get_token();
        if ((token->type != TOKEN_COLON) && (token->type != TOKEN_R_ROUND)) {
            ret_error(SYNTAX_ERROR);
        }
    }
    f->numOfParams = paramsCount;

    token = get_token();
    if (token->type != TOKEN_L_CURLY) {
        ret_error(SYNTAX_ERROR);
    }
    token = get_token();
    while (token->type == TOKEN_TYPE) { //deklaracie a definicie lokalnych premennych
        char *type = token->data;
        token = get_token();

        if (token->type != TOKEN_ID) {
            ret_error(SYNTAX_ERROR);
        }
        Ttoken *tokenID = token;
        if((BSTSearch(f->table, tokenID->data))){
            ret_error(SEMANTIC_DEF_ERROR);
        }
        v = variableDecl(f->table, tokenID, type);
        v->declared = 1;
        token = get_token();
    }

    switch (token->type) {
        case TOKEN_ID:
            tokenID = token;
            tTablePtr node = NULL;
            token = get_token();
            if(token->type == TOKEN_DOT) {
                char *className = tokenID->data;
                tTablePtr tableOfClass = create_class_table(className);
                token = getToken();
                if (token->type != TOKEN_ID) {
                    ret_error(SYNTAX_ERROR);
                }
                //tokenID = token;
                //node = BSTSearch(tableOfClass, tokenID->data);
                //if(node == NULL){
                token = get_token();
                if (token->type == TOKEN_ASSIGN) {
                    unget_token(token);
                    variableDecl(tableOfClass, tokenID, NULL);
                    if (token->type != TOKEN_SEM_CL) {
                        ret_error(SYNTAX_ERROR);
                    }
                }
                else if (token->type == TOKEN_L_ROUND) {
                    unget_token(token);
                    unget_token(token);
                    expression(NULL);
                    unget_token(token);
                    if (token->type != TOKEN_SEM_CL) {
                        ret_error(SYNTAX_ERROR);
                    }
                }
                else {
                    ret_error(SYNTAX_ERROR);
                }
            }

            node = BSTSearch(f->table, tokenID->data);   //neexistuje staticka premmenna s nazvom token->data v danej triede
            if (node == NULL) {

                node = BSTSearch(table, token->data);
                if (node == NULL) {

                    token = get_token();

                    if(token->type == TOKEN_ASSIGN){
                        unget_token(token);
                        variableDecl(f->table, tokenID, NULL);
                        if (token->type != TOKEN_SEM_CL) {
                            ret_error(SYNTAX_ERROR);
                        }
                    }
                    else if (token->type == TOKEN_L_ROUND) {
                        unget_token(token);
                        unget_token(token);
                        expression(NULL);
                        unget_token(token);

                        if (token->type != TOKEN_SEM_CL) {
                            ret_error(SYNTAX_ERROR);
                        }
                    }
                    else{
                        ret_error(SYNTAX_ERROR);
                    }
                }
                else {
                    token = get_token();
                    if (token->type == TOKEN_ASSIGN) {
                        unget_token(token);
                        variableDecl(table, tokenID, NULL);
                        token = get_token();
                        if (token->type != TOKEN_SEM_CL) {
                            ret_error(SYNTAX_ERROR);
                        }
                    } else if (token->type == TOKEN_L_ROUND) {
                        unget_token(token);
                        unget_token(token);
                        expression(NULL);
                        unget_token(token);
                        token = get_token();
                        if (token->type != TOKEN_SEM_CL) {
                            ret_error(SYNTAX_ERROR);
                        }
                    }
                    else{
                        ret_error(SYNTAX_ERROR);
                    }
                }
            } else {

                token = get_token();
                if (token->type == TOKEN_ASSIGN) {
                    unget_token(token);
                    variableDecl(f->table, tokenID, NULL);
                    token = get_token();
                    if (token->type != TOKEN_SEM_CL) {
                        ret_error(SYNTAX_ERROR);
                    }
                } else if (token->type == TOKEN_L_ROUND) {
                    unget_token(token);
                    unget_token(token);
                    expression(NULL);
                    unget_token(token);
                    token = get_token();
                    if (token->type != TOKEN_SEM_CL) {
                        ret_error(SYNTAX_ERROR);
                    }
                }
                else{
                    ret_error(SYNTAX_ERROR);
                }
            }
            break;


        case KEYWORD_IF:
            if_statement(token);
            break;
        case KEYWORD_FOR:
            for_statement();
            break;
        case KEYWORD_WHILE:
            while_statement();
            break;
        case KEYWORD_BREAK:
            //vytvori instrukciu break
            break;
        case KEYWORD_CONTINUE:
            //vytvori instrukciu continue
            break;
        case KEYWORD_DO:
            do_statement();
            break;
        case KEYWORD_ELSE:
            else_statement();
            break;
        case KEYWORD_RETURN:
            //vytvori instrukciu return
            break;

    }
    store_function(f, &table);
    return f;
    //...


}


//void funcCall(TFunction *f, Ttable *table, Ttoken *token, char *type) {
//    tTablePtr node = NULL;
//    Ttoken *token;
//    if(token->type != TOKEN_L_ROUND){
//        ret_error(SYNTAX_ERROR);
//    }
//    while (token->type == TOKEN_ID) {
//        node = BSTSearch(table, tokenID->data);
//        if (node == NULL) {
//            node->declared = 0;
//            //char *type = NULL;
//            variableDecl(f->table, tokenID, NULL);
//        }
//
//        token = get_token();
//        if (token->type != TOKEN_COLON) {
//            break;
//        }
//        token = get_token();
//    }
//        if (token->type != TOKEN_R_ROUND) {
//            ret_error(SYNTAX_ERROR);
//        }
//
//        token = get_token();
//
//    if (token->type != TOKEN_SEMICOLON) {
//        ret_error(SYNTAX_ERROR);
//    }
//}


    void params(TFunction *f, Ttoken *token, int numOfParam) { //spracovanie parametrov funkcie

        tTablePtr table = f->table;
        TVariable *var;
        if (token->type != TOKEN_TYPE) {
            ret_error(SEMANTIC_DEF_ERROR);
        }
        //char *type = token->data;
        f->params[numOfParam] = token->data;
        var->type = type;
        Ttoken *token = get_token();
        var = new_variable(token);
        if (token->type != TOKEN_ID) {
            ret_error(SYNTAX_ERROR);
        }

        store_variable(var, &table);
        var->declared = 1;

    }

//    void if_statement(Ttoken *token) {
//        TVariable *var;
//        var = new_variable(token);
//        token = get_token();
//        if (token->type != TOKEN_L_ROUND) {
//            ret_error(SYNTAX_ERROR);
//        }
//        expression(&var, token);
//
//        TInstruction *label;
//        label->operation = ins_lab;
//        label->add1 = NULL;
//        label->add2 = NULL;
//        label->add3 = NULL;
//
//        create_ins(ins_jmp, var, NULL, label);
//        create_ins(ins_lab, NULL, NULL, NULL);
//
//
//    }


/*--------------------/automat-----------------------*/

void parse()
{
	
	parser_init();

	parser_finish();

    }
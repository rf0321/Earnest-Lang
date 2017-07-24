#include "MEM.h"
#include "DBG.h"
#include "earnest.h"
/*
これはパースツリーの構築をするコードです。
*/
DeclarationList*eaf_chain_declaration(DeclarationList *list,Declaration *decl){
    DeclarationList *new_item;
    DeclarationList *pos;

    new_item=eaf_malloc(sizeof(DeclarationList));
    new_item->declaration=decl;
    new_item->next=NULL;

    if(list==NULL){
        return new_item;
    }
    for(pos=list; pos->next !=NULL; pos=pos->next);
    pos->next=new_item;

    return list;
}
Declaration*
eaf_alloc_declaration(TypeSpecifier *type,char *identifier){
    Declaration *decl;
    decl=eaf_malloc(sizeof(Declaration));
    decl->name=identifier;
    decl->type=type;
    decl->variable_index=-1;

    return decl;
}
static FunctionDefinition*
create_function_definition(EVM_BasicType type,char *identifier,ParameterList *parameter_list,Block *block){
    FunctionDefinition *fd;
    EAF_compiler *compiler;

    compiler=eaf_get_current_compiler();

    fd=eaf_malloc(sizeof(FunctionDefinition));
    fd->type=eaf_alloc_type_specifier(type);
    fd->name=identifier;
    fd->block=block;
    fd->index=compiler->function_count;
    
    compiler->function_count++;
    fd->local_variable_count=0;
    fd->local_variable=NULL;
    fd->next=NULL;

    return fd;
}
void eaf_define_function(EVM_BasicType type,char *identifier,ParameterList *parameter_list,Block *block){
    FunctionDefinition *fd;
    FunctionDefinition *pos;

    if(eaf_search_function(identifier)||eaf_search_declaration(identifier,NULL)){
        eaf_compile_error(eaf_get_current_compiler()->current_line_number,FUNCTION_MULTIPLR_DEFINE_ERR,
        STRING_MESSAGE_ARGUMENT,"name",identifier,MESSAGE_ARGUMENT_END);
        return;
    }
    fd=create_function_definition(type,identifier,parameter_list,block);
    if(block){
       block->type=FUNCTION_BLOCK;
       block->parent.function.function=fd;
    }
    compiler=eaf_get_current_compiler();
        if (compiler->function_list) {
        for (pos = compiler->function_list; pos->next; pos = pos->next)
            ;
        pos->next = fd;
    } else {
        compiler->function_list = fd;
    }
}
ParameterList *
eaf_create_parameter(EVM_BasicType type, char *identifier)
{
    ParameterList       *p;

    p = eaf_malloc(sizeof(ParameterList));
    p->name = identifier;
    p->type = eaf_alloc_type_specifier(type);
    p->line_number = eaf_get_current_compiler()->current_line_number;
    p->next = NULL;

    return p;
}
ParameterList *
eaf_chain_parameter(ParameterList *list, EVM_BasicType type,
                    char *identifier)
{
    ParameterList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = eaf_create_parameter(type, identifier);

    return list;
}
ArgumentList *
eaf_create_argument_list(Expression *expression)
{
    ArgumentList *al;

    al = eaf_malloc(sizeof(ArgumentList));
    al->expression = expression;
    al->next = NULL;

    return al;
}
ArgumentList *
eaf_chain_argument_list(ArgumentList *list, Expression *expr)
{
    ArgumentList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = eaf_create_argument_list(expr);

    return list;
}
StatementList *
eaf_create_statement_list(Statement *statement)
{
    StatementList *sl;

    sl = eaf_malloc(sizeof(StatementList));
    sl->statement = statement;
    sl->next = NULL;

    return sl;
}
StatementList *
eaf_chain_statement_list(StatementList *list, Statement *statement)
{
    StatementList *pos;

    if (list == NULL)
        return eaf_create_statement_list(statement);

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = eaf_create_statement_list(statement);

    return list;
}
Expression *
eaf_alloc_expression(ExpressionKind kind)
{
    Expression  *exp;

    exp = eaf_malloc(sizeof(Expression));
    exp->type = NULL;
    exp->kind = kind;
    exp->line_number = eaf_get_current_compiler()->current_line_number;

    return exp;
}
Expression *
eaf_create_comma_expression(Expression *left, Expression *right)
{
    Expression *exp;

    exp = eaf_alloc_expression(COMMA_EXPRESSION);
    exp->u.comma.left = left;
    exp->u.comma.right = right;

    return exp;
}
Expression *
eaf_create_assign_expression(Expression *left, AssignmentOperator operator,
                            Expression *operand)
{
    Expression *exp;

    exp = eaf_alloc_expression(ASSIGN_EXPRESSION);
    exp->u.assign_expression.left = left;
    exp->u.assign_expression.operator = operator;
    exp->u.assign_expression.operand = operand;

    return exp;
}
Expression *
eaf_create_binary_expression(ExpressionKind operator,
                             Expression *left, Expression *right)
{
#if 0
    if ((left->kind == INT_EXPRESSION
         || left->kind == DOUBLE_EXPRESSION)
        && (right->kind == INT_EXPRESSION
            || right->kind == DOUBLE_EXPRESSION)) {
        EAF_Value v;
        v = eaf_eval_binary_expression(eaf_get_current_compiler(),
                                       NULL, operator, left, right);
        /* Overwriting left hand expression. */
        *left = convert_value_to_expression(&v);

        return left;
    } else {
#endif
        Expression *exp;
        exp = eaf_alloc_expression(operator);
        exp->u.binary_expression.left = left;
        exp->u.binary_expression.right = right;
        return exp;
#if 0
    }
#endif
}
Expression *
eaf_create_minus_expression(Expression *operand)
{
#if 0
    if (operand->kind == INT_EXPRESSION
        || operand->kind == DOUBLE_EXPRESSION) {
        EAF_Value       v;
        v = eaf_eval_minus_expression(eaf_get_current_compiler(),
                                      NULL, operand);
        /* Notice! Overwriting operand expression. */
        *operand = convert_value_to_expression(&v);
        return operand;
    } else {
#endif
        Expression      *exp;
        exp = eaf_alloc_expression(MINUS_EXPRESSION);
        exp->u.minus_expression = operand;
        return exp;
#if 0
    }
#endif
}

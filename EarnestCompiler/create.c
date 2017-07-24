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
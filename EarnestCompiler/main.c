#include <stdio.h>
#include <locale.h>
#include "EAF.h"
#include "MEM.h"

int main(int argc,char **argv){
    EAF_Compiler *compiler;
    FILE *fp;
    EVM_Executable *exe;
    EVM_VirtualMachine *evm;
    
    if(argc < 2){
        fprintf(stderr,"usage:Earnest-Lang[file]"argv[0]);
        exit(1);
    }
    fp=fopen(argv[1],"r");

    if(fp==NULL){
        fprintf(stderr,"%s not found.\n",argv[1]);
        exit(1);
    }
    setlocale(LC_CTYPE,"");
    compiler=EAF_create_compiler();
    exe=EAF_compile(compiler,fp);
    evm=EVM_create_virtual_machine();
    EVM_add_executable(evm,exe);
    EVM_execute(evm);
    EAF_dispose_compiler(compiler);
    EVM_dispose_virtual_machine(evm);

    MEM_check_all_blocks();
    MEM_dump_blocks(stdout);

    return 0;
}
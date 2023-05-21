#include "hash.hpp"
#include "header.hpp"
void le_arquivo(char* caminho){
    FILE* entrada = fopen(caminho, "r");
    FILE* saida = fopen("reg.txt", "w");
    fseek(entrada,0,SEEK_SET);
    bloco_t bloco;
    if(!entrada){return;}
    int i = 0;
    int contador_bloco = 0;
    while(!feof(entrada)){
        for(i = 0; i < QNT_REGISTROS_POR_BLOCO; i++){
            if(feof(entrada)) break;
            fscanf(entrada, " %d\n", &bloco.regs[i].id);
            fscanf(entrada, " %[^\n]", bloco.regs[i].titulo);
            fscanf(entrada, " %d\n", &bloco.regs[i].ano);
            fscanf(entrada, " %[^\n]", bloco.regs[i].autores);
            fscanf(entrada, " %d\n", &bloco.regs[i].citacoes);
            fscanf(entrada, " %[^\n]", bloco.regs[i].atualizacao);
            fscanf(entrada, " %[^\n]", bloco.regs[i].snippet);
            fscanf(entrada, "%*c");      
        }
        if(i < QNT_REGISTROS_POR_BLOCO){
            while(i < QNT_REGISTROS_POR_BLOCO){
                bloco.regs[i].id = -1;
                i++;
            }
        }
        fwrite(&bloco,TAM_BLOCO, 1, saida);
        contador_bloco++;
    }
    fclose(entrada);
    fclose(saida);
}

void gera_hash(){
    FILE* arq = fopen("reg.txt", "r");
    fseek(arq,0,SEEK_END);
    int posicao = ftell(arq)/TAM_BLOCO;
    if(posicao < 0){return;}
    inicia_hash(posicao*2);
    fseek(arq,0,SEEK_SET);
    bloco_t bloco;
    for(int i = 0; i < posicao; i++){
        int err = fread(&bloco, TAM_BLOCO, 1, arq);
        if(err != 1){return;}
        for(int j = 0; j < QNT_REGISTROS_POR_BLOCO; j++){
            int id = bloco.regs[j].id;
            if(bloco.regs[j].id == -1) break;
            else{
                adiciona_chave_hash(id,i);
            }
        }
    }
}

int main(int argc, char *argv[]){
    le_arquivo(argv[1]);
    gera_hash();
}
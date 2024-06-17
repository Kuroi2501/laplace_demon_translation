#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>


struct bloco le_bloco(char *nome_do_bloco);
void Escolhe_bloco(int seletor);
void Inserter(char* arq_dump, char* arq_trad, struct bloco parametros);
void calcula_ponteiro(char* arquivo_mod, unsigned int ponteiro, unsigned int ponteiro_mod);
void Dumper(char* caminho, struct bloco parametros);

typedef struct bloco{
	unsigned int offset_inicio;
	unsigned int offset_fim;
};

struct bloco le_bloco(char *nome_do_bloco){
	FILE *arquivo;
	char s[100];
	struct bloco parametros;
	char *h;
	char nome[50] = "Blocos_Laplace.txt";
	
	arquivo = fopen(nome, "rt");
	while (fgets(s, 100, arquivo) != NULL){
		
		if(strstr(s, nome_do_bloco) != NULL && (h=strstr(s, "_inicio=")) != NULL){ //Bloco_01_inicio=
			sscanf(s+strlen(h), "%XX", &parametros.offset_inicio);
		}
		if(strstr(s, nome_do_bloco) != NULL && (h=strstr(s, "_fim=")) != NULL){ //Bloco_01_fim=
			sscanf(s+strlen(h), "%XX", &parametros.offset_fim);
			break;
		}
	}
	fclose(arquivo);
	return parametros;	
}

void Escolhe_bloco(int seletor){
	int opc;
	int i=1;
	int tam_blocos=0;
	char nome[80];
	char nome_trad[80];
	char caminho[100] = "scripts_originais\\";
	char caminho_trad[100] = "scripts_traduzidos\\"; 
	char caminho_orig[50] = "";
	char opc_convertido[2];
	struct bloco parametros;
	
	do{
    	system("cls");
    	printf("Digite o número do Script desejado (de 1 a 11):\n");
    	printf("\t 0. Todos\n");
        printf("\t 999. Voltar\n");
		scanf("%d", &opc);
		
		if(opc != 0){
			
			if(seletor == 111){
				strcpy(nome, "Bloco_");
				sprintf(opc_convertido, "%d", opc);
				strcat(nome, opc_convertido);
				strcat(caminho, nome);
				strcat(caminho, "_orig.txt");
				
				parametros = le_bloco(nome);
				Dumper(caminho, parametros);
				printf("Script Dumpado com êxito!\nPressione qualquer tecla para voltar!\n");
				getch();
				break;
			}
			
			if(seletor == 222){
	            strcpy(nome, "Bloco_");
				sprintf(opc_convertido, "%d", opc);
				strcat(nome, opc_convertido);
	            strcat(caminho, nome);
	            strcat(caminho, "_orig.txt");
	            strcat(caminho_trad, nome);
	            strcat(caminho_trad, ".txt");
	
	            parametros = le_bloco(nome);
	            Inserter(caminho, caminho_trad, parametros);
	            printf("Script Inserido com exito!\n");
	            getch();
	            break;
        	}
		}
		else{
			for(i=1;i<=11;i++){
				if(seletor == 111){
		            strcpy(nome, "Bloco_");
					sprintf(opc_convertido, "%d", i);
					strcat(nome, opc_convertido);
		            strcat(caminho, nome);
		            strcat(caminho, "_orig.txt");
		
		            parametros = le_bloco(nome);
		            Dumper(caminho, parametros);
		            
		            printf("Dumpando: %s\n", caminho);
		            
		            strcpy(caminho, "scripts_originais\\");
					strcpy(nome, "");
        		}
				
				if(seletor == 222){
		            strcpy(nome, "Bloco_");
					sprintf(opc_convertido, "%d", i);
					strcat(nome, opc_convertido);
		            strcat(caminho, nome);
		            strcat(caminho, "_orig.txt");
		            strcat(caminho_trad, nome);
		            strcat(caminho_trad, ".txt");
		
		            parametros = le_bloco(nome);
		            Inserter(caminho, caminho_trad, parametros);
		            
		            printf("Inserindo: %s\n", caminho_trad);
		            
		            strcpy(caminho, "scripts_originais\\");
					strcpy(nome, "");
        		}
			}
			printf("Processo finalizado!!\n");
			getch();
			break;
		}
    }
    while(opc != 999);
	
    strcpy(caminho, "");
	strcpy(nome, "");   
}

void Inserter(char* arq_dump, char* arq_trad, struct bloco parametros){
	FILE *arq, *out;
	unsigned char s[200];
	int cont = 0;
	unsigned char *memoria;
	unsigned int c, tam, concat, offset_ini_texto, byte1, byte2, byte3;
	unsigned int ponteiro_mod = NULL;
	unsigned int ponteiro_orig;
	int i, cont1;
	char nome[100] = "Laplace's Demon (English v.2.0)_insert.sfc";
	unsigned char tabela[256] = "@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"0123456789ABCDEF"
								"GHIJKLMNOPQRSTUV"
								"WXYZá!?,.\"êéí-ã/"
								"abcdefghijklmnop"
								"qrstuvwxyz@@@@@@"
								"@@:+àóúôõ@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@ ";
	
	
	if ((arq = fopen (arq_dump,"r")) == NULL){
        printf ("Erro na abertura do arquivo modificado!\n\n");
        exit (0);        
    }
    
    if ((out = fopen (nome,"r+b")) == NULL){
        printf ("Erro na criação do arquivo!\n");
        exit (0);        
    }
    
    fseek(out, 0, SEEK_END);
    tam = ftell(out);
    
    fseek(out, 0, SEEK_SET);
    memoria = (unsigned char*) malloc (sizeof(unsigned char)*tam);
    cont1 = fread (memoria, sizeof(unsigned char), tam, out);
	
	byte1 = memoria[parametros.offset_inicio];
	byte2 = memoria[parametros.offset_inicio+1];
	byte3 = memoria[parametros.offset_inicio+2];
	
//	printf("Byte1: %.2x\nByte2: %.2x\nByte2: %.2x\n", byte1, byte2, byte3);
//	getch();
	
	concat = ((byte3 << 16) | (byte2 << 8) | byte1);	 
	offset_ini_texto = ((concat & 0x7f0000) >> 1) + (concat & 0x7fff);
	
	printf("Inserindo...\n");
	
	fseek (out, SEEK_SET + offset_ini_texto, SEEK_SET);
	
//	printf("Inicio do bloco: %.8x\n", offset_ini_texto);
//	getch();
	
	while (fgets(s, 200, arq) != NULL){
		
		if(!strcmp(s, "------------------\n") || !strcmp(s, "------------------")){
			continue;
		}
		
		for(i=0;i<(int)strlen(s)-1;i++){
			
			if(s[i] == '{'){
				
				if(s[i+3] == '}'){
					sscanf(&s[i+1], "%xx", &c);
					fputc(c, out);
					
					if(c == 0x00){
						ponteiro_mod = ftell(out);
					}
					i= i+3;
				}
			
				if(s[i] == '{' && s[i+9] == '}'){
					sscanf(&s[i+1], "%x", &ponteiro_orig);
//					printf("Nome do binario: %s\nPonteiro_orig: %.8x\nPonteiro_mod: %.8x\n", nome, ponteiro_orig, ponteiro_mod);
//					getch();
					//calcula_ponteiro(nome, ponteiro_orig, ponteiro_mod);
					i=i+9;
				}	
			}
			
			else{
				c = s[i];
				cont = 0;
            	while (tabela[cont] != c) cont++;
            	fputc (cont, out);
			}
		}	
	}

	fclose(arq);
	fclose(out);
	free(memoria);
	
	printf("A inserção foi realizada com êxito!\nPressione qualquer tecla para encerrar.\n");
	getch();
	
}


void Dumper(char* caminho, struct bloco parametros){
	FILE *arquivo_binario, *arquivo_saida;
    unsigned char *memoria;
    unsigned int cont, i, j, tam, byte1, byte2, byte3, concat, offset_ini_texto;
//	unsigned int offset_ini;
//	unsigned int offset_fim;
    unsigned char tabela[256] = "@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"0123456789ABCDEF"
								"GHIJKLMNOPQRSTUV"
								"WXYZ@@@,.@@@@@@@"
								"abcdefghijklmnop"
								"qrstuvwxyz@@@@@@"
								"@@@@@@@'@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@@"
								"@@@@@@@@@@@@@@@ ";
								
	if ((arquivo_binario = fopen("Laplace's Demon (English v.2.0).sfc", "rb")) == NULL){
	    printf ("Erro na abertura do arquivo binário!\n\n");
	    exit (0);
  	}

    fseek(arquivo_binario, 0, SEEK_END);
    tam = ftell(arquivo_binario);
    
    fseek(arquivo_binario, 0, SEEK_SET);
    memoria = (unsigned char*) malloc (sizeof(unsigned char)*tam);
    cont = fread (memoria, sizeof(unsigned char), tam, arquivo_binario);
    fclose(arquivo_binario);
    
    arquivo_saida = fopen(caminho, "wt");
    
    for(i=parametros.offset_inicio;i<=parametros.offset_fim;i=i+3){
    	
    	byte1 = memoria[i];
    	byte2 = memoria[i+1];
    	byte3 = memoria[i+2];
    	
    	concat = ((byte3 << 16) | (byte2 << 8) | byte1);	 
    	offset_ini_texto = ((concat & 0x7f0000) >> 1) + (concat & 0x7fff);
    	
		fprintf(arquivo_saida, "{%.8x}\n", i);
    	 
    	for(j=offset_ini_texto;memoria[j]!=0x00;j++){
			
			if(memoria[j] == 0x03 || memoria[j] == 0x02){
				fprintf(arquivo_saida, "{%.2x}\n", memoria[j]);
			}
			
			else{
				if(tabela[memoria[j]] != '@'){
					fprintf(arquivo_saida, "%c", tabela[memoria[j]]);
				}
				else{
					fprintf(arquivo_saida, "{%.2x}", memoria[j]);
				}
			}
			
		}
		
		fprintf(arquivo_saida, "{00}\n------------------\n");
    }
    
    fclose(arquivo_saida);
    free(memoria);

}


int main() {
	int opc;
	int i;
	
	setlocale(LC_ALL, "Portuguese");
	
	mkdir("scripts_originais");
	mkdir("scripts_traduzidos");
	
	do{
	system("cls");
	printf("     Escolha uma das opções:\n\n");
	printf("\t1. Dumpar textos\n");
	printf("\t2. Inserir textos\n");
	printf("\t9. Sair\n");
	scanf("%d", &opc);
	
	  if(opc==1){
		Escolhe_bloco(111);
	  }
	  else if(opc == 2){
	    Escolhe_bloco(222);
	  }
	}
	while(opc != 9);
	getch();
}

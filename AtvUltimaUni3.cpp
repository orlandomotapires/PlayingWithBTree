#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <math.h>

/* Made by: 
  Antonio Hor cio Magalh es��
  Fernando Antonio Marques Schettini| GitHub: https://github.com/FernandoSchett
  Orlando Mota Pires| GitHub: https://github.com/orlandomotapires
*/

#define M 2
#define MM  4
#define FALSE 0
#define TRUE  1

typedef struct{
  int codigo;
  char descricao[100];
  float custo;
}DadosObra;
typedef DadosObra TipoChave;
typedef struct TipoRegistro {
  TipoChave Chave;
} TipoRegistro;
typedef struct TipoPagina *TipoApontador;
typedef struct TipoPagina {
  short n;
  TipoRegistro r[MM];
  TipoApontador p[MM + 1];
} TipoPagina;
void Inicializa(TipoApontador *Dicionario){  
  *Dicionario = NULL; 
}
void Pesquisa(TipoRegistro *x, TipoApontador Ap){ 
  long i = 1;
  if (Ap == NULL) { 
    return;
  }
  while (i < Ap->n && x->Chave.codigo > Ap->r[i-1].Chave.codigo) 
    i++;
    if (x->Chave.codigo == Ap->r[i-1].Chave.codigo) { 
    *x = Ap->r[i-1];
    return;
  }
  if (x->Chave.codigo < Ap->r[i-1].Chave.codigo) 
    Pesquisa(x, Ap->p[i-1]);
  else 
    Pesquisa(x, Ap->p[i]);
} 
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir){ 
  short NaoAchouPosicao;
  int k;
  k = Ap->n;  NaoAchouPosicao = (k > 0);
  while (NaoAchouPosicao) { 
    if (Reg.Chave.codigo >= Ap->r[k-1].Chave.codigo) { 
      NaoAchouPosicao = FALSE;
      break;
    }
    Ap->r[k] = Ap->r[k-1];
    Ap->p[k+1] = Ap->p[k];
    k--;
    if (k < 1) NaoAchouPosicao = FALSE;
  }
  Ap->r[k] = Reg; 
  Ap->p[k+1] = ApDir;
  Ap->n++;
} 
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro 
*RegRetorno,  TipoApontador *ApRetorno){ 
  long i = 1; long j;
  TipoApontador ApTemp;
  if (Ap == NULL) { 
    *Cresceu = TRUE; (*RegRetorno) = Reg; (*ApRetorno) = NULL;
    return;
  }
  while (i < Ap->n && Reg.Chave.codigo > Ap->r[i-1].Chave.codigo)  
    i++;
  if (Reg.Chave.codigo == Ap->r[i-1].Chave.codigo) { 
    printf(" Erro: Registro ja esta presente\n"); *Cresceu = FALSE;
    return;
  }
  if (Reg.Chave.codigo < Ap->r[i-1].Chave.codigo) i--;
  Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
  if (!*Cresceu) return;
  if (Ap->n < MM) { /* Pagina tem espaco */
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    *Cresceu = FALSE;
    return;
  }
  /* Overflow: Pagina tem que ser dividida */
  ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
  ApTemp->n = 0;  
  ApTemp->p[0] = NULL;
  if (i < M + 1) { 
    InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  } 
  else 
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  for (j = M + 2; j <= MM; j++) 
    InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
  Ap->n = M;  
  ApTemp->p[0] = Ap->p[M+1];
  *RegRetorno = Ap->r[M];  
  *ApRetorno = ApTemp;
}
void Insere(TipoRegistro Reg, TipoApontador *Ap) { 
  short Cresceu;
  TipoRegistro RegRetorno;
  TipoPagina *ApRetorno, *ApTemp;
  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
  if (Cresceu){  /* Arvore cresce na altura pela raiz */  
    ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
    ApTemp->n = 1; 
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    ApTemp->p[0] = *Ap;  *Ap = ApTemp;
  }
}
void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, int PosPai, short 
*Diminuiu){ 
  TipoPagina *Aux;  long DispAux, j;
    if (PosPai < ApPai->n)  /* Aux = TipoPagina a direita de ApPag */
  { 
  Aux = ApPai->p[PosPai+1];  DispAux = (Aux->n - M + 1) / 2;
      ApPag->r[ApPag->n] = ApPai->r[PosPai];
      ApPag->p[ApPag->n + 1] = Aux->p[0];  
  ApPag->n++;
      if (DispAux > 0)  /* Existe folga: transfere de Aux para ApPag */
      { 
  for (j = 1; j < DispAux; j++)
          InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
        ApPai->r[PosPai] = Aux->r[DispAux-1];  
  Aux->n -= DispAux;
        for (j = 0; j < Aux->n; j++) Aux->r[j] = Aux->r[j + DispAux];
        for (j = 0; j <= Aux->n; j++) Aux->p[j] = Aux->p[j + DispAux];
        *Diminuiu = FALSE;
      }
      else /* Fusao: intercala Aux em ApPag e libera Aux */
        { 
  for (j = 1; j <= M; j++)
            InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
          free(Aux);
          for (j = PosPai + 1; j < ApPai->n; j++) 
            { 
  ApPai->r[j-1] = ApPai->r[j]; 
      ApPai->p[j] = ApPai->p[j+1]; 
    }
          ApPai->n--;
          if (ApPai->n >= M) *Diminuiu = FALSE;
        }
    }
    else /* Aux = TipoPagina a esquerda de ApPag */
      { 
  Aux = ApPai->p[PosPai-1]; 
  DispAux = (Aux->n - M + 1) / 2;
  for (j = ApPag->n; j >= 1; j--) ApPag->r[j] = ApPag->r[j-1];
  ApPag->r[0] = ApPai->r[PosPai-1];
  for (j = ApPag->n; j >= 0; j--) ApPag->p[j+1] = ApPag->p[j];
  ApPag->n++;
  if (DispAux > 0) /* Existe folga: transf. de Aux para ApPag */
  { 
  for (j = 1; j < DispAux; j++)
            InsereNaPagina(ApPag, Aux->r[Aux->n - j], 
          Aux->p[Aux->n - j + 1]);
          ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
          ApPai->r[PosPai-1] = Aux->r[Aux->n - DispAux];
          Aux->n -= DispAux;  
  *Diminuiu = FALSE;
        }
        else /* Fusao: intercala ApPag em Aux e libera ApPag */
          { 
  for (j = 1; j <= M; j++)
              InsereNaPagina(Aux, ApPag->r[j-1], ApPag->p[j]);
            free(ApPag);  ApPai->n--;
            if (ApPai->n >= M)  *Diminuiu = FALSE;
          }
      }
}
void Antecessor(TipoApontador Ap, int Ind, TipoApontador ApPai, short *Diminuiu){
  if (ApPai->p[ApPai->n] != NULL) 
    { 
  Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
      if (*Diminuiu) 
      Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu);
      return;
    }
    Ap->r[Ind-1] = ApPai->r[ApPai->n - 1]; 
    ApPai->n--;  *Diminuiu = (ApPai->n < M);
} 
void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu){ 
  long j, Ind = 1;
  TipoApontador Pag;
  if (*Ap == NULL) { 
  printf("Erro: registro nao esta na arvore\n"); *Diminuiu = FALSE;
  return;
  }
    Pag = *Ap;
    while (Ind < Pag->n && Ch.codigo > Pag->r[Ind-1].Chave.codigo) Ind++;
    if (Ch.codigo == Pag->r[Ind-1].Chave.codigo) { 
    if (Pag->p[Ind-1] == NULL)   /* TipoPagina folha */
      { 
  Pag->n--;
        *Diminuiu = (Pag->n < M);
        for (j = Ind; j <= Pag->n; j++) 
          { 
  Pag->r[j-1] = Pag->r[j];  
  Pag->p[j] = Pag->p[j+1]; 
  }
        return;
      }
      /* TipoPagina nao e folha: trocar com antecessor */
      Antecessor(*Ap, Ind, Pag->p[Ind-1], Diminuiu);
      if (*Diminuiu) 
      Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);
      return;
  }
  if (Ch.codigo > Pag->r[Ind-1].Chave.codigo) Ind++;
  Ret(Ch, &Pag->p[Ind-1], Diminuiu);
  if (*Diminuiu) Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);
}  
void Retira(TipoChave Ch, TipoApontador *Ap){ 
  short Diminuiu;
  TipoApontador Aux;
  Ret(Ch, Ap, &Diminuiu);
  if (Diminuiu && (*Ap)->n == 0)  /* Arvore diminui na altura */
  { 
  Aux = *Ap;   
  *Ap = Aux->p[0]; 
  free(Aux);
  }
}  
//Biblioteca de fila
#define MAX 100 // every time "MAX" appears, compilator will read "100".
typedef TipoApontador tp_item; //Defining a new variable, tp_item. Doing this, you 

typedef struct{  //Defining a new variable, tp_queue.
    tp_item item[MAX]; // As a queue, it needs a vector, for storing 

    int start, end; // It needs 2 integers as well, they 

    int size; // finally, it need a integer that keeps 

} tp_queue;
void start_queue(tp_queue *f){   // Function that receives a queue then reset 

    f -> start = f -> end = MAX-1;
    f -> size = 0;
    
}
int empty_queue(tp_queue *f){  // Function that receive a queue, then verifies if 
    if(f -> start == f-> end){
return 1;  //"1" == Success
}
    return 0;  // "0" == Failure
    
}
int next(int pos){ //Function that receives a integer then returns the next 
    if(pos >= MAX -1){  // It will fall for pos >= 99, cuz an queue element 
return 0;  //0 = failure
}
    return ++pos;  //note: "++pos" != "pos++"
}
int full_queue(tp_queue *f){ //Function that receives a queue and verifies if its 

    if(next(f -> end) == f -> start){
        return 1;
    }
    return 0;
    
// 0 = not full.
// 1 = full.
}
int queue_insert(tp_queue *f, tp_item e){ //Function that receives a queue and a 

    
if(full_queue(f)){
return 0; //failure,you cant add elements from a full queue. 
}
f -> end = next(f -> end);
    f -> item[f->end] = e;
    f -> size++;
    return 1;  //success.
}
int queue_remove(tp_queue *f, tp_item *e){ //Function that receives a queue and a 

    if(empty_queue(f)){
return 0;  //failure,you cant remove elements from a empty queue.
}
    f -> start = next(f->start);
    *e = f -> item[f->start];
    f -> size--;
    return 1;  //success.
    
}
//Funcoes Proprias
void paraTudo(){
  system("pause");
  system("cls");
}
void insereArvore(TipoApontador *Dicionario, TipoRegistro dados){
  Insere(dados, Dicionario);
}
void convertString(char conteudoBruto[], char ini[]){
  int i,maiorq,id;
    maiorq = id = 0;
    for(i = 1; i < strlen(conteudoBruto); i ++){
      if(conteudoBruto[i] == '>')
        maiorq = 1;
      else if(conteudoBruto[i] == '<'){
        ini[id] = '\0';
        break;
      }
      else if(maiorq == 1){
        ini[id] = conteudoBruto[i];
        id++;
      }
    }
  
}
int convertCodigo(char codigo[]){
  int i, pot, codigoInt,menorQ,valor;
    menorQ = pot = codigoInt = 0;
    for(i = strlen(codigo) - 2; i >= 0; i --){
      if(codigo[i] == '<')
        menorQ = 1;
      else if(codigo[i] == '>')
        break;        
      else if(menorQ == 1){
        valor = (codigo[i] - 48);
        codigoInt += ( valor * pow(10,pot));
        pot ++;
      }
    }
  return codigoInt;
}
float convertCusto(char conteudoBruto[]){
  int i, pot, menorQ, quantidadeDeCasaDecimais, acheiVirgula, custoInteiro, 
valor,parada;
  float custoFloat;
    menorQ = pot = custoFloat = quantidadeDeCasaDecimais = acheiVirgula = 
custoInteiro = parada = 0;
    quantidadeDeCasaDecimais = 1;
    for(i = strlen(conteudoBruto) - 2; i >= 0; i --){
if(conteudoBruto[i] == '>')
         break; 
      if(conteudoBruto[i] == '<')
        menorQ = 1;
        else if(menorQ == 1 && conteudoBruto[i] != ','){
          valor = (conteudoBruto[i] - 48);
          custoInteiro += ( valor * pow(10,pot));
          pot ++;
      }
      if(conteudoBruto[i] == ','){
        acheiVirgula = 1;
        parada = 1;
      }
      if(acheiVirgula != 1 && menorQ == 1 && parada!= 1)
        quantidadeDeCasaDecimais++;
             
      
    }
    pot = pow(10,quantidadeDeCasaDecimais-2);
    custoFloat = custoInteiro;
    custoFloat = (float)custoFloat/pot;
    
  return custoFloat;
}
void lerXml(TipoApontador *Dicionario, char nomeArq[]){
    char conteudoBruto[100];
    int i;
    TipoRegistro dados;
    FILE *arq = fopen(nomeArq, "r");
    fgets(conteudoBruto, 100, arq);
    fgets(conteudoBruto, 100, arq);
    fgets(conteudoBruto, 100, arq);
   
    while(!feof(arq)){
  
      fscanf(arq, "%s", conteudoBruto);
      dados.Chave.codigo = convertCodigo(conteudoBruto);
      fscanf(arq, "%s", conteudoBruto);
      convertString(conteudoBruto, dados.Chave.descricao);   
      
      fscanf(arq, "%s", conteudoBruto);  
      dados.Chave.custo = convertCusto(conteudoBruto);
     
      fscanf(arq, "%s", conteudoBruto); 
      fscanf(arq, "%s", conteudoBruto);
  
   if(!feof(arq))
       insereArvore(Dicionario, dados);
      
    }
    
}
void pesquisaXml(TipoApontador Dicionario){
  printf("Escreva o codigo da etapa: \n");
  TipoRegistro dado;
  dado.Chave.custo = -1;
  scanf("%d", &dado.Chave.codigo);
  Pesquisa(&dado, Dicionario);
  if(dado.Chave.custo == -1){
    printf("Etapa nao esta presente no projeto\n");
  }
  else{
    printf("Etapa encontrada\n");
    printf("Codigo: %d\n", dado.Chave.codigo);
    printf("Descricao: %s\n", dado.Chave.descricao);
    printf("Custo: %.2f\n", dado.Chave.custo);
  }
}
int tem_filho(TipoApontador Ap){
int cont = 0;
while(cont != MM){
if(Ap->p[cont] != NULL){
//printf("Tem Filho\n");
return 1;
}
cont++;
}
//printf("Nao tem Filho\n");
return 0;
}
void SomaCusto(TipoApontador Ap, float *soma){ 
int i = 1, j = 0;
while( j != Ap->n){
//printf("Cod: %d\n", Ap->r[j].Chave.codigo);
(*soma) = (*soma) + Ap->r[j].Chave.custo;
j++;
}
if(!tem_filho(Ap)){
return;
}
j = 0;
while(j != (MM + 1)){
if(Ap->p[j] != NULL){
SomaCusto(Ap->p[j], soma);
}
j++;
}
}
void exibe_nivel(tp_queue *fila){
int cont;
TipoApontador aux;
while(!empty_queue(fila)){
queue_remove(fila, &aux);
cont = 0;
while(cont != (MM + 1)){
if(aux->p[cont] != NULL){
queue_insert(fila, aux->p[cont]);
}
cont++;
}
cont = 0;
while(cont != aux->n){
printf("Cod: %d\n", aux->r[cont].Chave.codigo);
printf("Desc: %s\n", aux->r[cont].Chave.descricao);
printf("Custo: %.2f\n", aux->r[cont].Chave.custo);
cont++;
}
}
}
void menu(char nomeArq[], TipoApontador *Dicionario){
  int op=1, nivel;
  float soma;
  tp_queue fila;
  start_queue(&fila);
  while(op != 5){
    printf("\nDigite 1 para ler o arquivo\nDigite 2 para pesquisar\nDigite 3 para visualizar todos os dados\nDigite 4 para visualizar o custo total da obra\nDigite 5 para encerrar o programa\n");
    scanf(" %d", &op);
    soma = nivel = 0;
    switch(op){
      case 1:
        lerXml(Dicionario, nomeArq);
        printf("Etapas inseridas com sucesso\n");
        paraTudo();
        break;
      case 2:
        pesquisaXml(*Dicionario);
        paraTudo();
        break;
      case 3:
       queue_insert(&fila, *Dicionario);
        exibe_nivel(&fila);
        paraTudo();
        break;
      case 4:
        SomaCusto(*Dicionario, &soma);
        printf("Soma dos custos: %.2f R$\n", soma);
        paraTudo();
        break;
      case 5:
        printf("Saindo...\n");
        paraTudo();
        return;
        break;
      default:
        printf("Opcao invalida!\n");
        break;
    }
  }
}

int main(){
  TipoApontador Dicionario;
  Inicializa(&Dicionario);
  
  char nomeArq[100];
  printf("Digite o nome do arquivo: ");
  scanf(" %s", nomeArq);
  float soma=0;
  menu(nomeArq, &Dicionario);
  return 0;
}
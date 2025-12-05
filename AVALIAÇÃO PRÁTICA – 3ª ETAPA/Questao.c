
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARQ_CONTAS "contas.dat"
#define ARQ_CONFIG "config.txt"
#define SENHA_ADMIN "root"

typedef struct {
 int numero;
 char nome[50];
 float saldo;
 int ativo;

}  Conta;
Conta CONTA_VAZIA = {0, "", 0.0, 0};
int gerarNumeroConta() {
   FILE *config = fopen(ARQ_CONFIG, "r");
  int proximoNumero = 1;   
  if (config != NULL) {
     fscanf(config, "%d", &proximoNumero);
     fclose(config);
 }
 int numeroAtual = proximoNumero;
  config = fopen(ARQ_CONFIG, "w");
    if (config != NULL) {
     fprintf(config, "%d", numeroAtual + 1);
     fclose(config);
 } else {
     printf("ERRO NO SISTEMA \n");
 }  
    return numeroAtual;
}
Conta buscarConta(int numero) {
  FILE *arquivo = fopen(ARQ_CONTAS, "rb");
  if (!arquivo) return CONTA_VAZIA; 
  Conta conta;
   while (fread(&conta, sizeof(Conta), 1, arquivo)) { 
     if (conta.numero == numero) {
      fclose(arquivo);
     return conta;
  }
    } 
  fclose(arquivo);
  return CONTA_VAZIA;
}
void salvarConta(Conta contaAtualizada) {
    FILE *arquivo = fopen(ARQ_CONTAS, "rb+");
    
 if (!arquivo) {
      arquivo = fopen(ARQ_CONTAS, "ab"); 
     if (!arquivo) return;
     fwrite(&contaAtualizada, sizeof(Conta), 1, arquivo);
     fclose(arquivo);
      return;
    }
    Conta contaLida;
    int encontrou = 0;
    while (fread(&contaLida, sizeof(Conta), 1, arquivo)) {
     if (contaLida.numero == contaAtualizada.numero) {
       encontrou = 1;
       break;
  }
}
  if (encontrou) {
      fseek(arquivo, -sizeof(Conta), SEEK_CUR); 
     fwrite(&contaAtualizada, sizeof(Conta), 1, arquivo);
 }    
  fclose(arquivo);
}
void criarConta() {
  Conta conta;
  conta.numero = gerarNumeroConta(); 
   printf("Nome do dono da conta: ");
   fgets(conta.nome, 50, stdin);
     conta.nome[strcspn(conta.nome, "\n")] = '\0'; 
    
      conta.saldo = 0.0;
      conta.ativo = 1;
    
  FILE *arquivo = fopen(ARQ_CONTAS, "ab");
    if (arquivo) {
     fwrite(&conta, sizeof(Conta), 1, arquivo);
     fclose(arquivo);
     printf("\nConta criada  Numero: %d\n", conta.numero);
} else { printf("Erro ao abrir arquivo para salvar a nova conta.\n");
    }
}
 void depositar() {
  int num;
  float valor;
  printf("Numero da conta para deposito: ");
  scanf("%d", &num);  
  Conta conta = buscarConta(num);  
  if (conta.numero != 0 && conta.ativo == 1) {
      printf("Valor a depositar: ");
      scanf("%f", &valor);     
   if (valor > 0) {
     conta.saldo += valor;
      salvarConta(conta);
        printf("Deposito de R$ %.2f realizado. saldo atual: R$ %.2f\n", valor, conta.saldo);
     } else {
      printf("Valor invalido.\n");
      }
  } else {
        printf("Conta inexistente.\n");
  }
}
void sacar() {
  int num;
   float valor;
   printf("Numero da conta para o saque: ");
   scanf("%d", &num);
  Conta conta = buscarConta(num);   
  if (conta.numero != 0 && conta.ativo == 1) {
        printf("Valor a sacar: ");
        scanf("%f", &valor);   
  if (valor > 0 && valor <= conta.saldo) {
      conta.saldo -= valor;
      salvarConta(conta);
       printf("Saque de R$ %.2f realizado. Novo saldo: R$ %.2f\n", valor, conta.saldo);
 } else if (valor > conta.saldo) {
            printf("Saldo insuficiente.\n");
 } else {
            printf("Valor invalido.\n");
  }
  } else {
        printf("Conta inexistente .\n");
 }
}
void consultarSaldo() {
  int num;
  printf("Numero da conta para consulta: ");
  scanf("%d", &num);   
   Conta conta = buscarConta(num);  
  if (conta.numero != 0 && conta.ativo == 1) {
     printf("\nConta: %d\n", conta.numero);
      printf("Titular: %s\n", conta.nome);
     printf("Saldo: R$ %.2f\n", conta.saldo);
  } else {
        printf("Conta inexistente.\n");
  }
}
void listarContas() {
    char senha[50];
     printf("digite a senha de admin: ");
    scanf("%s", senha);  
    if (strcmp(senha, SENHA_ADMIN) != 0) {
     printf("errou!\n");
        return;
  }
    FILE *arquivo = fopen(ARQ_CONTAS, "rb");
    if (!arquivo) {
     printf("Sem contas cadastradas.\n");
        return;
 }
    Conta conta;
    printf("\n  Lista de Contas  \n");
    while (fread(&conta, sizeof(Conta), 1, arquivo)) {
     if (conta.ativo == 1) {
       printf("Num: %d | Titular: %s | Saldo: R$ %.2f\n", conta.numero, conta.nome, conta.saldo);
 }
  }
 
    
  fclose(arquivo);
}

int main() {
  int opcao;
    do {
     printf("\n\nBanco Do TI\n");
    printf("1. Nova Conta\n");
     printf("2. Depositar\n");
     printf("3. Sacar\n");
    printf("4. Consultar Saldo\n");
     printf("5. visualizar todas as contas\n");
    printf("0. Sair\n");
     printf("Digite um numero: ");
       
   if (scanf("%d", &opcao) != 1) {
     printf("\nNumero invalido,digite outro\n");
     while (getchar() != '\n');
      continue;
}
  switch (opcao) {
     case 1:
      while (getchar() != '\n'); 
        criarConta();
         break;
     case 2:
             depositar();
            break;
    case 3:
         sacar();
         break;
    case 4:
         consultarSaldo();
         break;
     case 5:
          while (getchar() != '\n'); 
         listarContas();
            break;
     case 0:
            printf("\nPrograma finalizado.\n");
            break;
    default:
        printf("\nOpcao Inexistente, tente outro numero.\n");
 }      
} while (opcao != 0);
    return 0;
}
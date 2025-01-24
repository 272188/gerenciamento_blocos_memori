#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_BLOCOS 1000
#define TAMANHO_BLOCO 1


typedef struct BlocoMemoria {
    char status;
    int blocoInicial;
    int blocoFinal;
    void *enderecoInicial;
    void *enderecoFinal;
} BlocoMemoria;

typedef struct Arv45 {
    int nInfos;
    BlocoMemoria infos[4];
    struct Arv45 *Esq, *Centro1, *Centro2, *Centro3, *Dir;
} Arv45;

Arv45* criaNoBloco(BlocoMemoria bloco) {
    Arv45 *no = (Arv45*) malloc(sizeof(Arv45));
    if (no != NULL) {
        no->infos[0] = bloco;
        no->nInfos = 1;
        no->Esq = no->Centro1 = no->Centro2 = no->Centro3 = no->Dir = NULL;
    }
    return no;
}

int ehFolha(Arv45 *no) {
    return (no->Esq == NULL && no->Centro1 == NULL && no->Centro2 == NULL && no->Centro3 == NULL && no->Dir == NULL);
}

void adicionaInfoBloco(Arv45 *no, BlocoMemoria bloco, Arv45 *filhoDir) {
    if (bloco.blocoInicial < no->infos[0].blocoInicial) {
        no->infos[3] = no->infos[2];
        no->Centro3 = no->Centro2;
        no->infos[2] = no->infos[1];
        no->Centro2 = no->Centro1;
        no->infos[1] = no->infos[0];
        no->Centro1 = no->Esq;
        no->infos[0] = bloco;
        no->Esq = filhoDir;
    } else if (no->nInfos == 1 || bloco.blocoInicial < no->infos[1].blocoInicial) {
        no->infos[3] = no->infos[2];
        no->Centro3 = no->Centro2;
        no->infos[2] = no->infos[1];
        no->Centro2 = no->Centro1;
        no->infos[1] = bloco;
        no->Centro1 = filhoDir;
    } else if (no->nInfos == 2 || bloco.blocoInicial < no->infos[2].blocoInicial) {
        no->infos[3] = no->infos[2];
        no->Centro3 = no->Centro2;
        no->infos[2] = bloco;
        no->Centro2 = filhoDir;
    } else {
        no->infos[3] = bloco;
        no->Dir = filhoDir;
    }
    no->nInfos++;
}

Arv45* divideNoBloco(Arv45 *no, BlocoMemoria bloco, BlocoMemoria *sobe, Arv45 *filhoDir) {
    Arv45 *novoNo; 
    novoNo = criaNoBloco(no->infos[3]);

    if (bloco.blocoInicial > no->infos[2].blocoInicial) {
        *sobe = no->infos[2];
        novoNo->Esq = no->Centro3;
        novoNo->Centro1 = no->Dir;
        adicionaInfoBloco(novoNo, bloco, filhoDir);
    } else if (bloco.blocoInicial > no->infos[1].blocoInicial) {
        *sobe = bloco;
        novoNo->Esq = no->Centro3;
        novoNo->Centro1 = no->Dir;
        adicionaInfoBloco(novoNo, no->infos[2], no->Centro2);
    } else if (bloco.blocoInicial > no->infos[0].blocoInicial) {
        *sobe = no->infos[1];
        novoNo->Esq = no->Centro2;
        novoNo->Centro1 = no->Centro3;
        novoNo->Centro2 = no->Dir;
        adicionaInfoBloco(novoNo, no->infos[2], no->Centro2);
        no->infos[1] = bloco;
        no->Centro1 = filhoDir;
    } else {
        *sobe = no->infos[0];
        novoNo->Esq = no->Centro2;
        novoNo->Centro1 = no->Centro3;
        novoNo->Centro2 = no->Dir;
        adicionaInfoBloco(novoNo, no->infos[2], no->Centro2);
        no->infos[0] = bloco;
        no->Esq = filhoDir;
    }

    no->nInfos = 2;
    return novoNo;
}

Arv45* insereArv45(Arv45* pai, Arv45** raiz, BlocoMemoria bloco, BlocoMemoria *sobe) {
    Arv45* novoNo;
    novoNo = NULL;

    if (*raiz == NULL){
        *raiz = criaNoBloco(bloco);
    } else {
        if (ehFolha(*raiz)) {
            if ((*raiz)->nInfos < 4) {
                adicionaInfoBloco(*raiz, bloco, NULL);
            } else {
                novoNo = divideNoBloco(*raiz, bloco, sobe, NULL);
                if (pai == NULL) {
                    Arv45 *novaRaiz = criaNoBloco(*sobe);
                    novaRaiz->Esq = *raiz;
                    novaRaiz->Centro1 = novoNo;
                    *raiz = novaRaiz;
                    novoNo = NULL;
                }
            }
        } else {
            if (bloco.blocoInicial < (*raiz)->infos[0].blocoInicial) {
                novoNo = insereArv45(*raiz, &((*raiz)->Esq), bloco, sobe);
            } else if ((*raiz)->nInfos == 1 || bloco.blocoInicial < (*raiz)->infos[1].blocoInicial) {
                novoNo = insereArv45(*raiz, &((*raiz)->Centro1), bloco, sobe);
            } else if ((*raiz)->nInfos == 2 || bloco.blocoInicial < (*raiz)->infos[2].blocoInicial) {
                novoNo = insereArv45(*raiz, &((*raiz)->Centro2), bloco, sobe);
            } else {
                novoNo = insereArv45(*raiz, &((*raiz)->Dir), bloco, sobe);
            }

            if (novoNo != NULL) {
                if ((*raiz)->nInfos < 4) {
                    adicionaInfoBloco(*raiz, *sobe, novoNo);
                    novoNo = NULL;
                } else {
                    novoNo = divideNoBloco(*raiz, *sobe, sobe, novoNo);
                    if (pai == NULL) {
                        Arv45 *novaRaiz = criaNoBloco(*sobe);
                        novaRaiz->Esq = *raiz;
                        novaRaiz->Centro1 = novoNo;
                        *raiz = novaRaiz;
                        novoNo = NULL;
                    }
                }
            }
        }
    }

    return novoNo;
}

void imprimeNo(Arv45 *no) {
	int i;
    printf("[ ");
    for (i = 0; i < no->nInfos; ++i) {
        printf("(%c, %d-%d, %lu-%lu) ", no->infos[i].status, no->infos[i].blocoInicial, no->infos[i].blocoFinal,
               (unsigned long)no->infos[i].enderecoInicial, (unsigned long)no->infos[i].enderecoFinal);
    }
    printf("] ");
}

void imprimeArvore(Arv45 *raiz) {
    if (raiz != NULL) {
        imprimeNo(raiz);
        imprimeArvore(raiz->Esq);
        imprimeArvore(raiz->Centro1);
        imprimeArvore(raiz->Centro2);
        imprimeArvore(raiz->Centro3);
        imprimeArvore(raiz->Dir);
    }
}

BlocoMemoria* buscaBloco(Arv45 *raiz, void *enderecoInicial) {
    if (raiz == NULL) {
        return NULL;
    }
    int i;

    for (i = 0; i < raiz->nInfos; ++i) {
        if (raiz->infos[i].enderecoInicial == enderecoInicial) {
            return &(raiz->infos[i]);
        }
    }

    if (enderecoInicial < raiz->infos[0].enderecoInicial) {
        return buscaBloco(raiz->Esq, enderecoInicial);
    } else if (raiz->nInfos == 1 || enderecoInicial < raiz->infos[1].enderecoInicial) {
        return buscaBloco(raiz->Centro1, enderecoInicial);
    } else if (raiz->nInfos == 2 || enderecoInicial < raiz->infos[2].enderecoInicial) {
        return buscaBloco(raiz->Centro2, enderecoInicial);
    } else if (enderecoInicial >= raiz->infos[2].enderecoInicial) {
        return buscaBloco(raiz->Dir, enderecoInicial);
    }

    return NULL;
}

void liberaBlocos(Arv45 *raiz, void *enderecoInicial, int numBlocos) {
    BlocoMemoria *bloco = buscaBloco(raiz, enderecoInicial);
    if (bloco == NULL || bloco->status == 'L') {
        printf("Erro: bloco não encontrado ou já está livre.\n");
        return;
    }

    int blocosOcupados = bloco->blocoFinal - bloco->blocoInicial + 1;
    if (numBlocos > blocosOcupados) {
        printf("Erro: número de blocos a serem liberados excede os blocos ocupados.\n");
        return;
    }

    bloco->blocoFinal = bloco->blocoInicial + numBlocos - 1;
    bloco->status = 'L';

    printf("Blocos liberados com sucesso: (%c, %d-%d, %lu-%lu)\n", bloco->status, bloco->blocoInicial, bloco->blocoFinal,
           (unsigned long)bloco->enderecoInicial, (unsigned long)bloco->enderecoFinal);
}

void cadastraNosArvore(Arv45 **raiz) {
    BlocoMemoria bloco;
    char status;
    unsigned long enderecoInicial, enderecoFinal;

    printf("Cadastro do primeiro bloco de memória:\n");
    printf("Status (O para ocupado, L para livre): ");
    scanf(" %c", &status);
    printf("Endereco inicial (em decimal): ");
    scanf("%lu", &enderecoInicial);
    printf("Endereco final (em decimal): ");
    scanf("%lu", &enderecoFinal);

    bloco.status = status;
    bloco.enderecoInicial = (void *)enderecoInicial;
    bloco.enderecoFinal = (void *)enderecoFinal;
    bloco.blocoInicial = 0;
    bloco.blocoFinal = (enderecoFinal - enderecoInicial) / TAMANHO_BLOCO;

    *raiz = criaNoBloco(bloco);

    int blocoAtual = bloco.blocoFinal + 1;
    enderecoInicial = enderecoFinal + 1;

    while (enderecoInicial < TOTAL_BLOCOS * TAMANHO_BLOCO) {
        printf("Informe o status do próximo bloco (L para livre, O para ocupado): ");
        scanf(" %c", &status);
        printf("Endereco final (em decimal): ");
        scanf("%lu", &enderecoFinal);

        bloco.status = status;
        bloco.enderecoInicial = (void *)enderecoInicial;
        bloco.enderecoFinal = (void *)enderecoFinal;
        bloco.blocoInicial = blocoAtual;
        bloco.blocoFinal = blocoAtual + (enderecoFinal - enderecoInicial) / TAMANHO_BLOCO;

        insereArv45(NULL, raiz, bloco, NULL);

        blocoAtual = bloco.blocoFinal + 1;
        enderecoInicial = enderecoFinal + 1;
    }
}

void imprimeBlocosArvore(Arv45 *no) {
    if (no == NULL) {
        return;
    }

    int i;
    for (i = 0; i < no->nInfos; ++i) {
        if (i > 0 && no->infos[i].blocoInicial > no->infos[i - 1].blocoFinal + 1) {
            printf("(Livre, %d-%d) ", no->infos[i - 1].blocoFinal + 1, no->infos[i].blocoInicial - 1);
        }
        printf("(Ocupado, %d-%d) ", no->infos[i].blocoInicial, no->infos[i].blocoFinal);
    }

    // Verifica se há blocos livres após o último bloco ocupado
    if (i > 0 && no->infos[i - 1].blocoFinal < TOTAL_BLOCOS) {
        printf("(Livre, %d-%d) ", no->infos[i - 1].blocoFinal + 1, TOTAL_BLOCOS);
    }

    // Chama recursivamente para os filhos
    imprimeBlocosArvore(no->Esq);
    imprimeBlocosArvore(no->Centro1);
    imprimeBlocosArvore(no->Centro2);
    imprimeBlocosArvore(no->Centro3);
    imprimeBlocosArvore(no->Dir);
}


void alocaBlocos(Arv45 *raiz, int numBlocos) {
    BlocoMemoria *blocoAlocado = NULL;

    Arv45 *noAtual = raiz;
    int indiceNo = 0;

    while (noAtual != NULL) {
        // Busca pelo primeiro bloco livre
        while (indiceNo < noAtual->nInfos && noAtual->infos[indiceNo].status == 'O') {
            indiceNo++;
        }

        if (indiceNo < noAtual->nInfos) {
            // Verifica se é possível alocar os blocos necessários
            int blocosDisponiveis = noAtual->infos[indiceNo].blocoFinal - noAtual->infos[indiceNo].blocoInicial + 1;

            if (blocosDisponiveis >= numBlocos) {
                blocoAlocado = &noAtual->infos[indiceNo];
                // Marca como alocado
                blocoAlocado->status = 'O';
                blocoAlocado->blocoFinal = blocoAlocado->blocoInicial + numBlocos - 1;
                printf("Blocos alocados com sucesso: (%c, %d-%d, %lu-%lu)\n", blocoAlocado->status, blocoAlocado->blocoInicial, blocoAlocado->blocoFinal,
                       (unsigned long)blocoAlocado->enderecoInicial, (unsigned long)blocoAlocado->enderecoFinal);
                return;
            }
        }

        // Vai para o próximo nó ou nó da próxima linha
        if (indiceNo < noAtual->nInfos) {
            noAtual = noAtual->Centro1;
        } else {
            noAtual = noAtual->Centro2;
        }
    }

    printf("Não foi possível alocar %d blocos consecutivos.\n", numBlocos);
}


int main() {
    Arv45 *raiz = NULL;
    int escolha = 0;

    cadastraNosArvore(&raiz);

    while (escolha != -1) {
        printf("\n1. Alocar blocos\n");
        printf("2. Liberar blocos\n");
        printf("-1. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                {
                    int numBlocos;
                    printf("Digite a quantidade de blocos que deseja alocar: ");
                    scanf("%d", &numBlocos);
                    alocaBlocos(raiz, numBlocos);
                    break;
                }
            case 2:
                {
                    unsigned long enderecoInicial;
                    int numBlocos;
                    printf("Digite o endereço inicial do bloco que deseja liberar: ");
                    scanf("%lu", &enderecoInicial);
                    printf("Digite a quantidade de blocos que deseja liberar: ");
                    scanf("%d", &numBlocos);
                    liberaBlocos(raiz, (void*)enderecoInicial, numBlocos);
                    break;
                }
            case -1:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }

        printf("\nTodos os blocos de memória da árvore 4-5:\n");
        imprimeBlocosArvore(raiz);
        printf("\n");
    }

    return 0;
}

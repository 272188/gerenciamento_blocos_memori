/*
Suponha que uma mem�ria seja dividida em blocos l�gicos de 1Mbyte, e que o primeiro bloco � o bloco 0, suponha tamb�m que o gerenciador 
de mem�ria de um Sistema Operacional mantenha uma �rvore 2-3 com n�s para blocos livres e ocupados da mem�ria. 
Cada n� da �rvore 2-3 cont�m os seguintes campos: 
O- para ocupado ou L- para livre, o n�mero do bloco inicial, o n�mero do bloco final, endere�o inicial (correspondente ao endere�o inicial 
do primeiro bloco do n�) e endere�o final (correspondente ao endere�o final do �ltimo bloco do n�). 
a) Fa�a um programa em C que cadastra os n�s da �rvore, onde o usu�rio deve informar se o primeiro n� � livre ou ocupado, o endere�o inicial 
e final do n�. Os demais n�s ser�o contabilizados pelo sistema se s�o livres ou ocupados e o usu�rio deve apenas informar o endere�o final 
de cada um. O cadastro termina quando o usu�rio informar como endere�o final de um n� o �ltimo endere�o da mem�ria. 
b) Fa�a uma fun��o que o usu�rio informe a quantidade de n�s que ele precisa alocar e retorne as informa��es do n� que atenda as necessidades 
do usu�rio e ent�o modifique a situa��o do referido n� de Livre para Ocupado. 
  (i) Lembre-se que a �rvore deve manter n�s intercalados de acordo com a situa��o do n�, ou seja, se a situa��o de um n� muda ent�o os n�s 
  adjacentes a ele deve ser concatenados. Consequentemente os n�s da �rvore ser�o modificados. 
  (ii)Lembre-se tamb�m se o n� escolhido possui uma quantidade maior de blocos do que o solicitado pelo usu�rio os n�s �rvore devem ser 
  atualizados de forma que mantenha blocos adjacentes livres ou ocupados em um mesmo n�.
c) Fa�a uma fun��o em que o usu�rio informe blocos que devem ser liberados.
  (i) Lembre-se que a �rvore deve manter n�s intercalados de acordo com a situa��o do n�, ou seja, se a situa��o de um n� muda ent�o os n�s 
  adjacentes a ele deve ser concatenados. Consequentemente os n�s da �rvore ser�o modificados. 
  (ii)Lembre-se tamb�m se o n� escolhido possui uma quantidade maior de blocos do que o solicitado pelo usu�rio os n�s �rvore devem ser 
  atualizados de forma que mantenha blocos adjacentes livres ou ocupados em um mesmo n�. 
*/

#include <stdio.h>
#include <stdlib.h>


#define TOTAL_BLOCOS 1000
#define TAMANHO_BLOCO 1

typedef struct DadosNo {
    char estado; // 'L' para livre, 'O' para ocupado
    int blocoInicial;
    int blocoFinal;
    int enderecoInicial;
    int enderecoFinal;
} DadosNo;

typedef struct arv23 {
    DadosNo info1, info2;
    struct arv23 *esq, *cen, *dir;
    int nInfo;
} Arv23;

// Fun��o para criar um novo n� de dados
DadosNo* criarNo(char estado, int blocoInicial, int blocoFinal, int enderecoInicial, int enderecoFinal) {
    DadosNo *novoDados = (DadosNo*) malloc(sizeof(DadosNo));
    if (novoDados != NULL) {
        novoDados->estado = estado;
        novoDados->blocoInicial = blocoInicial;
        novoDados->blocoFinal = blocoFinal;
        novoDados->enderecoInicial = enderecoInicial;
        novoDados->enderecoFinal = enderecoFinal;
    }
    return novoDados;
}

// Fun��o para criar uma nova �rvore
Arv23* criaNoArv23(DadosNo* novoDados) {
    Arv23* novaArvore = (Arv23*)malloc(sizeof(Arv23));
    if (novaArvore != NULL) {
        novaArvore->info1 = *novoDados;
        novaArvore->nInfo = 1;
        novaArvore->esq = novaArvore->cen = novaArvore->dir = NULL;
    }
    return novaArvore;
}

// Fun��o para verificar se um n� � folha
int isLeaf(Arv23 *no) {
    return (no->esq == NULL && no->cen == NULL && no->dir == NULL);
}

// Fun��o para adicionar uma chave ao n�
Arv23* adicionaChave(Arv23* no, DadosNo* dados, Arv23* novoFilho) {
    if (no == NULL) {
        printf("Atencao: Parametro 'no' eh nulo na funcao adicionaChave.\n");
    } else if (dados == NULL) {
        printf("Atencao: Parametro 'dados' eh nulo na funcao adicionaChave.\n");
    } else {
        if (no->nInfo == 1) {
            if (dados->enderecoInicial < no->info1.enderecoInicial) {
                // Desloca info1 para info2 e insere a nova chave em info1
                no->info2 = no->info1;
                no->info1 = *dados;
                no->dir = no->cen;
                no->cen = novoFilho;
                printf("Chave adicionada em info1.\n");
            } else {
                // Insere a nova chave em info2
                no->info2 = *dados;
                no->dir = novoFilho;
                printf("Chave adicionada em info2.\n");
            }
            no->nInfo = 2;
        } else {
            printf("Aviso: Fun��o adicionaChave chamada em um no que j� possui duas chaves e precisa ser dividido.\n");
        }
    }
    return no;
}

// Fun��o para quebrar um n�
Arv23* quebraNo(Arv23** no, DadosNo* dados, DadosNo** promove, Arv23* novoFilho) {
    if (no == NULL || *no == NULL || dados == NULL || promove == NULL) {
        printf("Atencao: Parametros invalidos na funcao quebraNo.\n");
        return NULL;
    }

    Arv23* novoNo = (Arv23*)malloc(sizeof(Arv23));
    if (novoNo == NULL) {
        printf("Atencao: Falha ao alocar memoria para novo no na funcao quebraNo.\n");
        return NULL;
    }

    DadosNo chavePromovida;
    if (dados->enderecoInicial < (*no)->info1.enderecoInicial) {
        // Caso 1: A nova chave � menor que info1
        chavePromovida = (*no)->info1;
        (*no)->info1 = *dados;
        novoNo->info1 = (*no)->info2;
        (*no)->cen = novoFilho;
        printf("Quebra de no: chave promovida de info1.\n");
    } else if ((*no)->nInfo == 2 && dados->enderecoInicial < (*no)->info2.enderecoInicial) {
        // Caso 2: A nova chave � maior que info1, mas menor que info2
        chavePromovida = *dados;
        novoNo->info1 = (*no)->info2;
        novoNo->esq = novoFilho;
        novoNo->cen = (*no)->dir;
        printf("Quebra de no: chave promovida entre info1 e info2.\n");
    } else {
        // Caso 3: A nova chave � maior que info2
        chavePromovida = (*no)->info2;
        novoNo->info1 = *dados;
        novoNo->esq = (*no)->dir;
        novoNo->cen = novoFilho;
        printf("Quebra de no: chave promovida de info2.\n");
    }

    novoNo->nInfo = 1;
    (*no)->nInfo = 1;
    **promove = chavePromovida;

    return novoNo;
}


// Fun��o auxiliar de inser��o recursiva
Arv23* inserirAux(Arv23** no, DadosNo* dados, DadosNo** promove) {
    Arv23* novoFilho = NULL;

    if (*no == NULL) {
        *no = criaNoArv23(dados);
    } else if (isLeaf(*no)) {
        if ((*no)->nInfo == 1) {
            *no = adicionaChave(*no, dados, NULL);
        } else {
            novoFilho = quebraNo(no, dados, promove, NULL);
        }
    } else {
        if ((*no)->nInfo == 1) {
            novoFilho = dados->enderecoInicial < (*no)->info1.enderecoInicial ? inserirAux(&(*no)->esq, dados, promove) : inserirAux(&(*no)->cen, dados, promove);
        } else {
            if (dados->enderecoInicial < (*no)->info1.enderecoInicial) {
                novoFilho = inserirAux(&(*no)->esq, dados, promove);
            } else if (dados->enderecoInicial < (*no)->info2.enderecoInicial) {
                novoFilho = inserirAux(&(*no)->cen, dados, promove);
            } else {
                novoFilho = inserirAux(&(*no)->dir, dados, promove);
            }
        }

        if (novoFilho != NULL) {
            if ((*no)->nInfo == 1) {
                *no = adicionaChave(*no, *promove, novoFilho);
                *promove = NULL;
            } else {
                novoFilho = quebraNo(no, *promove, promove, novoFilho);
            }
        }
    }

    return novoFilho;
}

// Fun��o de inser��o na �rvore 2-3
Arv23* inserir(Arv23* raiz, DadosNo* dados) {
    if (raiz == NULL) {
        raiz = criaNoArv23(dados);
        printf("Novo no criado como raiz.\n");
    } else {
        DadosNo* promove = NULL;
        Arv23* novoFilho = inserirAux(&raiz, dados, &promove);
        if (promove != NULL) {
            Arv23* novaRaiz = criaNoArv23(promove);
            novaRaiz->esq = raiz;
            novaRaiz->cen = novoFilho;
            raiz = novaRaiz;
            printf("Nova raiz criada apos quebra de no.\n");
        }
    }
    return raiz;
}


// Fun��o auxiliar para encontrar o n� onde o bloco est� localizado
Arv23* encontrarNo(Arv23 *arvore, int blocoInicial) {
    Arv23 *resultado = NULL;

    if (arvore != NULL) {
        if (blocoInicial >= arvore->info1.blocoInicial && blocoInicial <= arvore->info1.blocoFinal) {
            resultado = arvore;
        } else if (arvore->nInfo == 2 && blocoInicial >= arvore->info2.blocoInicial && blocoInicial <= arvore->info2.blocoFinal) {
            resultado = arvore;
        } else if (blocoInicial <= arvore->info1.blocoFinal) {
            resultado = encontrarNo(arvore->esq, blocoInicial);
        } else if (arvore->nInfo == 2 && blocoInicial <= arvore->info2.blocoFinal) {
            resultado = encontrarNo(arvore->cen, blocoInicial);
        } else {
            resultado = encontrarNo(arvore->dir, blocoInicial);
        }
    }

    return resultado;
}

                                                                ////fun��es para remo��o

// Fun��o para encontrar o pai de um n�
Arv23* encontrarPai(Arv23 *arvore, Arv23 *no) {
    Arv23 *pai = NULL;
    int i;

    if (arvore != NULL && no != NULL) {
        if (arvore->esq == no || arvore->cen == no || arvore->dir == no) {
            pai = arvore;
        } else {
            Arv23 *subarvore[] = {arvore->esq, arvore->cen, arvore->dir};
            for (i = 0; i < 3 && pai == NULL; i++) {
                pai = encontrarPai(subarvore[i], no);
            }
        }
    }

    return pai;
}

// Fun��o para ajustar as informa��es do pai ap�s a remo��o de um n�
void ajustarInformacoesDoPai(Arv23 *pai) {
    if (pai != NULL) {
        if (pai->info1.blocoInicial == 0 && pai->nInfo > 1) {
            pai->info1 = pai->info2;
            pai->cen = pai->dir;
            pai->info2.blocoInicial = 0;
            pai->info2.blocoFinal = 0;
            pai->nInfo--;
        } else if (pai->info2.blocoInicial == 0 && pai->nInfo == 2) {
            pai->cen = pai->dir;
            pai->dir = NULL;
            pai->info2.blocoInicial = 0;
            pai->info2.blocoFinal = 0;
            pai->nInfo--;
        } else if (pai->cen == NULL && pai->dir != NULL) {
            pai->cen = pai->dir;
            pai->dir = NULL;
            pai->nInfo--;
        }
    }
}

// Fun��o para remover uma chave de um n�
void removerChaveDeNo(Arv23 *pai, Arv23 *no) {
    if (pai != NULL && no != NULL) {
        if (pai->esq == no) {
            pai->esq = NULL;
        } else if (pai->cen == no) {
            pai->cen = NULL;
        } else if (pai->dir == no) {
            pai->dir = NULL;
        } else {
            printf("Erro: o no nao eh filho direto do pai.\n");
        }

        // Ajustar as informa��es do pai ap�s a remo��o
        ajustarInformacoesDoPai(pai);
    } else {
        printf("Erro: pai ou no sao nulos na remocao da chave do no.\n");
    }
}

// Fun��o para unir informa��es entre o n� e o irm�o
void unirInformacoes(Arv23 *irmao, Arv23 *no) {
    if (irmao == NULL || no == NULL) {
        printf("Erro: parametros invalidos para unir informacoes.\n");
    } else {
        // Verificar e transferir informa��es do n� para o irm�o
        if (irmao->nInfo < 2) {
            if (irmao->nInfo == 0) {
                irmao->info1 = no->info1;
            } else if (irmao->nInfo == 1) {
                irmao->info2 = no->info1;
            }
            irmao->nInfo++;
        } else {
            printf("Erro: o irmao ja possui todas as informacoes preenchidas.\n");
        }

        // Limpar as informa��es do n� ap�s a transfer�ncia
        no->info1 = (DadosNo){0};
        if (no->nInfo > 1) {
            no->info2 = (DadosNo){0};
        }
        no->nInfo--;

        printf("Informacoes unidas entre o no e o irmao.\n");
    }
}

void redistribuirInformacoes(Arv23 *pai, Arv23 *no, Arv23 *irmao);

void ajustarPai(Arv23 *pai, Arv23 *no) {
    Arv23 *irmao = NULL;

    if (pai->esq == no && pai->cen != NULL) {
        irmao = pai->cen;
    } else if (pai->cen == no && pai->dir != NULL) {
        irmao = pai->dir;
    } else if (pai->dir == no) {
        irmao = pai->cen;
    }

    if (irmao != NULL) {
        if (irmao->nInfo == 1) {
            unirInformacoes(irmao, no);
            removerChaveDeNo(pai, no);
        } else {
            redistribuirInformacoes(pai, no, irmao);
        }
    }
}

void ajustarArvore(Arv23 *no) {
    if (no != NULL) {
	    // Se o n� possui apenas um filho ap�s altera��es, consolide ou redistribua
	    if (no->nInfo == 0) {
	        Arv23 *pai = encontrarPai(no, no);
	        if (pai == NULL) {
	            // Caso especial: n� raiz vazio
	            if (no->esq != NULL) {
	                *no = *(no->esq); // Promova o filho esquerdo
	            } else if (no->cen != NULL) {
	                *no = *(no->cen); // Promova o filho central
	            } else if (no->dir != NULL) {
	                *no = *(no->dir); // Promova o filho direito
	            }
	        } else {
	        	ajustarPai(pai, no);
	        }
	    }

	    // Recurse para ajustar filhos
	    ajustarArvore(no->esq);
	    ajustarArvore(no->cen);
	    ajustarArvore(no->dir);
	} 
  
}

// Fun��o para redistribuir informa��es entre o n� e o irm�o
void redistribuirInformacoes(Arv23 *pai, Arv23 *no, Arv23 *irmao) {
    int parametrosValidos = (pai != NULL && no != NULL && irmao != NULL);
    int posNo = -1;

    if (parametrosValidos) {
        if (pai->esq == no) posNo = 0;
        else if (pai->cen == no) posNo = 1;
        else if (pai->dir == no) posNo = 2;

        if (posNo != -1) {
            if (posNo == 0) {
                no->info2 = pai->info1;
                no->nInfo++;
                pai->info1 = irmao->info1;
                irmao->info1 = irmao->info2;
                irmao->nInfo--;
                printf("Redistribuido: no na posicao 0 e irmao a direita.\n");
            } else if (posNo == 1) {
                no->info2 = pai->info2;
                no->nInfo++;
                pai->info2 = irmao->info1;
                irmao->info1 = irmao->info2;
                irmao->nInfo--;
                printf("Redistribuido: no na posicao 1 e irmao a direita.\n");
            } else if (posNo == 2) {
                no->info2 = pai->info2;
                no->nInfo++;
                pai->info2 = irmao->info1;
                irmao->info1 = irmao->info2;
                irmao->nInfo--;
                printf("Redistribuido: no na posicao 2 e irmao a direita.\n");
            }
        } else {
            printf("Erro: o no nao eh filho do pai especificado.\n");
        }
    } else {
        printf("Erro: parametros invalidos.\n");
    }
    if (pai != NULL){
		ajustarArvore(pai);
	}
}


// Fun��o para unir o n� com seu irm�o
void unirComIrmao(Arv23 *arvore, Arv23 *no) {
    if (arvore == NULL || no == NULL) {
        printf("Erro: arvore ou no sao nulos na uniao com o irmao.\n");
    } else {
        Arv23 *pai = encontrarPai(arvore, no);
        if (pai == NULL) {
            printf("Erro: pai do no nao encontrado na uniao com o irmao.\n");
        } else {
            Arv23 *irmao = NULL;
            int posNo = -1;

            if (pai->esq == no) {
                posNo = 0;
            } else if (pai->cen == no) {
                posNo = 1;
            } else if (pai->dir == no) {
                posNo = 2;
            } else {
                printf("Erro: no nao encontrado na arvore.\n");
            }

            if (posNo != -1) {
                if (posNo == 0 && pai->cen != NULL) {
                    irmao = pai->cen;
                } else if (posNo == 1 && pai->dir != NULL) {
                    irmao = pai->dir;
                } else {
                    printf("Erro: nao foi possivel encontrar o irmao do no.\n");
                }

                if (irmao != NULL) {
                    if (irmao->nInfo < 2) {
                        unirInformacoes(irmao, no);
                        removerChaveDeNo(pai, no);
                        ajustarArvore(pai);
                    } else {
                        redistribuirInformacoes(pai, no, irmao);
                    }
                }
            }
        }
    }
}

// Fun��o para remover um n� espec�fico da �rvore
void removerNo(Arv23 *arvore, Arv23 *no) {
    if (arvore == NULL || no == NULL) {
        printf("Erro: arvore ou no sao nulos na remocao.\n");
    } else {
        Arv23 *pai = encontrarPai(arvore, no);
        if (pai == NULL) {
            printf("Erro: pai do no nao encontrado.\n");
        } else {
            if (pai->esq == no) {
                pai->esq = NULL;
            } else if (pai->cen == no) {
                pai->cen = NULL;
            } else if (pai->dir == no) {
                pai->dir = NULL;
            } else {
                printf("Erro: o no nao eh filho direto do pai.\n");
            }

            ajustarArvore(pai);
            printf("No removido com sucesso.\n");
        }
    }
}

void liberarArvore(Arv23 *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->cen);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}



                                               ////Fun��es principais para o menu na main

//CASE 1 (item A)
// Fun��o para determinar o estado do pr�ximo bloco
char determinarEstadoProximoBloco(Arv23 *arvore) {
    if (arvore == NULL || arvore->info1.estado == '\0') {
        return 'L';
    }

    DadosNo *ultimoNo = &arvore->info1;

    if (arvore->nInfo == 1 && arvore->info1.estado != '\0') {
        ultimoNo = &arvore->info1;
    } else if (arvore->nInfo == 2 && arvore->info2.estado != '\0') {
        ultimoNo = &arvore->info2;
    }

    return (ultimoNo->estado == 'L') ? 'O' : 'L';
}

//CASE 2 (item B)

// Fun��o para encontrar o n� pelo endere�o
Arv23* encontrarNoPorEndereco(Arv23 *arvore, int endereco) {
    Arv23 *resultado = NULL;

    if (arvore != NULL) {
        if (endereco >= arvore->info1.enderecoInicial && endereco <= arvore->info1.enderecoFinal) {
            resultado = arvore;
        } else if (arvore->nInfo == 2 && endereco >= arvore->info2.enderecoInicial && endereco <= arvore->info2.enderecoFinal) {
            resultado = arvore;
        } else if (endereco < arvore->info1.enderecoInicial) {
            resultado = encontrarNoPorEndereco(arvore->esq, endereco);
        } else if (arvore->nInfo == 2 && endereco < arvore->info2.enderecoInicial) {
            resultado = encontrarNoPorEndereco(arvore->cen, endereco);
        } else {
            resultado = encontrarNoPorEndereco(arvore->dir, endereco);
        }
    }

    return resultado;
}

// Fun��o para unir blocos adjacentes
void unirBlocosAdjacentes(Arv23 *no) {
    if (no != NULL) {
        if (no->nInfo == 2 && no->info1.estado == 'L' && no->info2.estado == 'L') {
            no->info1.blocoFinal = no->info2.blocoFinal;
            no->info1.enderecoFinal = no->info2.enderecoFinal;
            no->nInfo--;
        }

        if (no->esq && no->esq->nInfo == 1 && no->esq->info1.estado == 'L' && no->info1.estado == 'L') {
            no->esq->info1.blocoFinal = no->info1.blocoFinal;
            no->esq->info1.enderecoFinal = no->info1.enderecoFinal;
            no->nInfo--;
            no->info1 = no->info2;
            no->cen = no->dir;
            no->dir = NULL;
        }

        if (no->cen && no->cen->nInfo == 1 && no->cen->info1.estado == 'L' && no->info2.estado == 'L') {
            no->cen->info1.blocoFinal = no->info2.blocoFinal;
            no->cen->info1.enderecoFinal = no->info2.enderecoFinal;
            no->nInfo--;
            no->info2 = (DadosNo){0};
            no->dir = NULL;
        }

        if (no->esq && no->cen && no->esq->info1.estado == 'L' && no->cen->info1.estado == 'L') {
            no->esq->info1.blocoFinal = no->cen->info1.blocoFinal;
            no->esq->info1.enderecoFinal = no->cen->info1.enderecoFinal;
            no->cen = no->dir;
            no->dir = NULL;
            no->nInfo--;
        }

        if (no->cen && no->dir && no->cen->info1.estado == 'L' && no->dir->info1.estado == 'L') {
            no->cen->info1.blocoFinal = no->dir->info1.blocoFinal;
            no->cen->info1.enderecoFinal = no->dir->info1.enderecoFinal;
            no->dir = NULL;
            no->nInfo--;
        }

        unirBlocosAdjacentes(no->esq);
        unirBlocosAdjacentes(no->cen);
        unirBlocosAdjacentes(no->dir);
    }
}


// Fun��o para alocar blocos de mem�ria
void alocarBloco(Arv23 *arvore, int blocoInicial, int blocosSolicitados) {
    Arv23 *no = encontrarNoPorEndereco(arvore, blocoInicial);

    if (no != NULL) {
        DadosNo *dadosSelecionados = NULL;
        int totalBlocosDisponiveis = 0;

        // Verifica se o bloco solicitado est� em info1
        if (no->info1.estado == 'L' && blocoInicial >= no->info1.blocoInicial && blocoInicial <= no->info1.blocoFinal) {
            dadosSelecionados = &no->info1;
            totalBlocosDisponiveis = no->info1.blocoFinal - blocoInicial + 1;
        } else if (no->nInfo == 2 && no->info2.estado == 'L' && blocoInicial >= no->info2.blocoInicial && blocoInicial <= no->info2.blocoFinal) {
            // Verifica se o bloco solicitado est� em info2
            dadosSelecionados = &no->info2;
            totalBlocosDisponiveis = no->info2.blocoFinal - blocoInicial + 1;
        }

        if (dadosSelecionados != NULL) {
            if (totalBlocosDisponiveis >= blocosSolicitados) {
                dadosSelecionados->estado = 'O';
                int blocosRestantes = totalBlocosDisponiveis - blocosSolicitados;

                if (blocosRestantes > 0) {
                    int novoBlocoInicial = blocoInicial + blocosSolicitados;
                    int novoEnderecoInicial = dadosSelecionados->enderecoInicial + blocosSolicitados * TAMANHO_BLOCO;
                    DadosNo novo = {'L', novoBlocoInicial, dadosSelecionados->blocoFinal, novoEnderecoInicial, dadosSelecionados->enderecoFinal};

                    dadosSelecionados->blocoFinal = blocoInicial + blocosSolicitados - 1;
                    dadosSelecionados->enderecoFinal = novoEnderecoInicial - TAMANHO_BLOCO;

                    inserir(arvore, &novo);
                }
                unirBlocosAdjacentes(arvore);
                printf("Bloco alocado: %d-%d\n", blocoInicial, blocoInicial + blocosSolicitados - 1);
            } else {
                printf("Blocos insuficientes.\n");
            }
        } else {
            printf("Bloco nao encontrado ou j� est� ocupado.\n");
        }
    } else {
        printf("Bloco nao encontrado.\n");
    }
}


//CASE 3 (item C)
// Fun��o para liberar um bloco de mem�ria
void liberarBloco(Arv23 *arvore, int endereco) {
    if (arvore == NULL) {
        printf("Erro: a arvore fornecida eh nula.\n");
    } else {
        Arv23 *no = encontrarNoPorEndereco(arvore, endereco);
        int blocoLiberado = 0;

        if (no != NULL) {
            if (endereco >= no->info1.enderecoInicial && endereco <= no->info1.enderecoFinal) {
                no->info1.estado = 'L';
                blocoLiberado = 1;
            } else if (no->nInfo == 2 && endereco >= no->info2.enderecoInicial && endereco <= no->info2.enderecoFinal) {
                no->info2.estado = 'L';
                blocoLiberado = 1;
            }

            if (blocoLiberado) {
                unirBlocosAdjacentes(arvore);
                printf("Bloco liberado: Endere�o %d.\n", endereco);
            } else {
                printf("Erro: bloco com endereco %d nao encontrado.\n", endereco);
            }
        } else {
            printf("Erro: bloco com endereco %d nao encontrado.\n", endereco);
        }
    }
}

//CASE 4
// Fun��o para imprimir a �rvore 2-3
void imprimirArvore(Arv23* raiz, int nivel) {
    int nivelAtual = nivel;
    int i;
    if (raiz != NULL) {
        // Imprime as informa��es do primeiro bloco do n�
        for (i = 0; i < nivelAtual; i++) {
            printf("  ");
        }
        printf("[%c %d-%d %d-%d]\n",
               raiz->info1.estado,
               raiz->info1.blocoInicial,
               raiz->info1.blocoFinal,
               raiz->info1.enderecoInicial,
               raiz->info1.enderecoFinal);

        // Imprime as informa��es do segundo bloco do n�, se houver
        if (raiz->nInfo == 2) {
            for (i = 0; i < nivelAtual; i++) {
                printf("  ");
            }
            printf("[%c %d-%d %d-%d]\n",
                   raiz->info2.estado,
                   raiz->info2.blocoInicial,
                   raiz->info2.blocoFinal,
                   raiz->info2.enderecoInicial,
                   raiz->info2.enderecoFinal);
        }

        // Chamada recursiva para imprimir os filhos
        if (raiz->esq != NULL) {
            imprimirArvore(raiz->esq, nivelAtual + 1);
        }
        if (raiz->cen != NULL) {
            imprimirArvore(raiz->cen, nivelAtual + 1);
        }
        if (raiz->nInfo == 2 && raiz->dir != NULL) {
            imprimirArvore(raiz->dir, nivelAtual + 1);
        }
    } else {
        for (i = 0; i < nivelAtual; i++) {
            printf("  ");
        }
        printf("Erro: Arvore vazia!\n");
    }
}

// Fun��o principal
int main() {
    Arv23 *arvore = NULL;
    DadosNo *dados;
    char estado;
    int opcao, blocoInicial, blocoFinal, enderecoInicial, enderecoFinal, blocosSolicitados;
    int primeiroNo = 1;
    int ULTIMO_ENDERECO = 1024;

    while (1) {
        printf("Menu:\n");
        printf("1. Inserir bloco\n");
        printf("2. Alocar bloco\n");
        printf("3. Liberar bloco\n");
        printf("4. Imprimir arvore\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (primeiroNo) {
                    printf("Digite o estado (L para livre, O para ocupado): ");
                    scanf(" %c", &estado);
                    printf("Digite o endereco inicial: ");
                    scanf("%d", &enderecoInicial);
                } else {
                    estado = determinarEstadoProximoBloco(arvore);
                    printf("Estado do proximo bloco sera: %c\n", estado);
                    enderecoInicial = enderecoFinal + 1;
                }
                printf("Digite o endereco final: ");
                scanf("%d", &enderecoFinal);

                blocoInicial = enderecoInicial / TAMANHO_BLOCO;
                blocoFinal = enderecoFinal / TAMANHO_BLOCO;

                dados = criarNo(estado, blocoInicial, blocoFinal, enderecoInicial, enderecoFinal);
                arvore = inserir(arvore, dados);
                primeiroNo = 0;

                if (enderecoFinal == ULTIMO_ENDERECO) {
                    printf("Cadastro finalizado.\n");
                    return 0;
                }
                break;
            
            
            case 2:
                printf("Digite o bloco inicial para alocacao: ");
                scanf("%d", &blocoInicial);
                printf("Digite a quantidade de blocos solicitados: ");
                scanf("%d", &blocosSolicitados);
                alocarBloco(arvore, blocoInicial, blocosSolicitados);
                break;

            case 3:
                printf("Digite o endereco do bloco a ser liberado: ");
                scanf("%d", &enderecoInicial);
                liberarBloco(arvore, enderecoInicial);
                break;
            
            case 4:
                imprimirArvore(arvore, 0);
                break;

            case 5:
                printf("Encerrando o programa.\n");
                liberarArvore(arvore);
                exit(0);

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}





#include "Ils.h"
#include <random>
#include<bits/stdc++.h>
#include <algorithm>
#include <functional>

Ils::Ils() {
	
};


bool Ils::comparaCustos( const InsertionInfo& insertion1, const  InsertionInfo& insertion2) {

	return insertion1.custo < insertion2.custo;

}




double Ils::calculateSwapCost(Solucao *s, int i, int j,  int **custos) {
	
	// Caso tenhamos a sequencia 1 9 5 8 7 1
	// Queremos trocar 9 e 5, i = 1 e j = 2
	// Pegamos o valor do vertice anterior a 9 que é i - 1
	// Pegamos o valor do vertice posterior a 5 que é j + 1
	// E calculamos c[1][5] + c[9][8] - (c[1][9] + c[5][8]), caso seja menor do que zero, é porque houve uma melhora
	// É necessárior retirar 1 de todos os verices, pois os vertices vão de 1 até n, e na matriz irá de 0 até n - 1.
	int verticeAnteriorA = s->sequencia[i - 1] - 1;
	int verticeA = s->sequencia[i] - 1;

	int verticePosteriorB = s->sequencia[j + 1] - 1;
	int verticeB = s->sequencia[j] - 1;
	// Custo do novo, menos o custo do anterior
	double custo = (custos[verticeAnteriorA][verticeB] + custos[verticeA][verticePosteriorB]) - (custos[verticeAnteriorA][verticeA] + custos[verticeB][verticePosteriorB]);
	return custo;

}
bool Ils::bestImprovementSwap(Solucao *s, int **custos) {

	double bestDelta = 0;

	int best_i, best_j;
	//1 2 3 4 5 1
	//Tem que ser até -1, pois vamos trocar entre os extremos 1
	// Realiza todas as trocas e pega a melhor troca possivel
	for(int i = 1; i < s->sequencia.size() - 1; i++) {

		for(int j = i + 1; j < s->sequencia.size() - 1; j++) {
			// Envia os vertices de troca, caso tenhamos 1 9 5 8 1, vamos enviar o valor do 9 e o valor do 5 e não 1 e 2 que são os valores de i e j
			double delta = calculateSwapCost(s, i, j ,custos);

			if(delta < bestDelta) {
				
				bestDelta = delta;
				best_i = i;
				best_j = j;
			}

		}
	}
	
	// Caso houve uma melhora, troca os vértices e retorna true, caso não retorna false
	if(bestDelta < 0) {
		
		std::swap(s->sequencia[best_i], s->sequencia[best_j]);
		return true;
	}

	return false;
}




double Ils::calculate2OptCost(Solucao *s, int i, int j, int **custos) {

	// Caso tenhamos 1 8 5 9 7 1 e i = 1 e j = 4
	// 1 8 7 9 5 1
	// Assim o novo custo, será c[8][7] + c[5][1] - c[8][5] + c[7][1]
	int verticePosteriorA = s->sequencia[i + 1] - 1;
	int verticeA = s->sequencia[i] - 1;

	int verticePosteriorB = s->sequencia[j + 1] - 1;
	int verticeB = s->sequencia[j] - 1;

	// Custo será o novo - antigo
	double custo = (custos[verticeA][verticeB] + custos[verticePosteriorA][verticePosteriorB]) - (custos[verticeA][verticePosteriorA] + custos[verticeB][verticePosteriorB]);
	return custo;
}




bool Ils::bestImprovement2Opt(Solucao *s, int **custos) {

	double bestDelta = 0; // Valor do melhor custo

	int best_i, best_j; // Vertices que possuem o melhor custo

	// Realiza o 2-opt para todos os vertices não adjacentes e verifica o menor custo
	for(int i = 1; i < s->sequencia.size() - 1; i++) {

		for(int j = i + 2; j < s->sequencia.size() - 1; j++ ) {
			
			double delta = calculate2OptCost(s, i, j, custos);

			if(delta < bestDelta) {
				
				bestDelta = delta;
				best_i = i;
				best_j = j;
			}
				

		}
	}
	
	// Com o menor custo, realiza o algoritmo 2-opt
	int posicaoInicial = best_i + 1;
	int posicaoFinal = best_j;
	if(bestDelta < 0) {
		
		while(posicaoInicial <= posicaoFinal) {
			
			std::swap(s->sequencia[posicaoInicial], s->sequencia[posicaoFinal]);
			posicaoInicial++;
			posicaoFinal--;
		}

		return true;

	}

	return false;
}





void Ils::buscaLocal(Solucao *s, int **custos) {
	
	std::vector < int > NL = {1,2,3,4,5};

	bool improved = false;

	while(NL.empty() == false) {

		int n = rand() % NL.size();

		switch(NL[n]) {

			case 1:
				improved = bestImprovementSwap(s, custos);
				break;
			case 2:
				improved = bestImprovement2Opt(s, custos);
				break;
		}


		if(improved) {
			NL = {1,2,3,4,5};
		}
		else {
			NL.erase(NL.begin() + n);
		}
	}

}


void Ils::ordenaEmOrdemCrescente(std::vector < InsertionInfo > &custoInsercao) {
	
	// Sort irá utilizar a função booleana para realizar a comparação de custos e deixar em ordem crescente
	std::sort(custoInsercao.begin(), custoInsercao.end(), comparaCustos);

}








std::vector < InsertionInfo > Ils::calcularCustoInsercao(Solucao *s, std::vector <int > &cl, int **custo) {
	
	// Vetor que contém as informações de cada valor de Cl, ou seja, de cada vertice que falta ser inserido, ou seja, seu custo de inserção e o par de vertices no qual foi inserido entre
	std::vector < InsertionInfo > custoInsercao1((s->sequencia.size() - 1) * cl.size());

	int l = 0; // Utilizado para percorrer o vector custoInsercao
	
	//12341
	for(int a = 0, b = 1; a < s->sequencia.size() - 1; a++, b++) {
		
		// Vertices i e j
		int i = s->sequencia[a];
		int j = s->sequencia[b];
	
		/* Necessário regular os valores, pois numa instancia de 280 os valores de cl vão de 1 até 280, mas a matriz irá de 0 a 279	*/
		
		// K assume valor de um verte que falta ser inserido na solucao inicial
		for(auto k : cl) {
			

			custoInsercao1[l].custo = custo[i - 1][k - 1] + custo[j - 1][k - 1] - custo[i - 1][j - 1]; // Custo de inserção daquele vértice k

			custoInsercao1[l].verticeInserido = k; // Vertice inserido
			
			custoInsercao1[l].arestaRemovida = a; // Aresta que será removida
			l++;

		}


	}
	
	return custoInsercao1;
}



//1 2 3 4
void Ils::inserirNaSolucao(Solucao *s1, InsertionInfo * selecionado) {
	
	s1->sequencia.insert(s1->sequencia.begin() + selecionado->arestaRemovida + 1, selecionado->verticeInserido);
}


void Ils::construcao(Solucao *s1, int dimensao, int **custos) {

	
	escolher3NosAleatorios(s1, dimensao);

	std::vector < int > cl; // Cl são os vertices que precisam ser inseridos

	for(int i = 1; i <= dimensao; i++) {
		
		// Caso o valor não exista
		if(!std::binary_search(s1->sequencia.begin(), s1->sequencia.end(), i)) {
			
			// Insere em cl
			cl.push_back(i);
		}
	}


	while(!cl.empty()) {
		
		// Computa os pares k , {i, j} que possui o custo de inserir k em i e j, qual vertice foi retirado e assim por diante
		std::vector < InsertionInfo > custoInsercao = calcularCustoInsercao(s1, cl, custos);
		// Ordena o custo em ordem crescente
		ordenaEmOrdemCrescente(custoInsercao);

		// Retira um determinado valor alpha
		double alpha = (double) std::rand() / RAND_MAX;
		// PEga um valor aleatorio, arredondando para maior do vector de custo
		int selecionado = std::rand() % ((int) std::ceil(alpha * custoInsercao.size()));
		// Adiciona o valor encontrado na solução
		inserirNaSolucao(s1, &custoInsercao[selecionado]);
		// E retira o vertice inserido de CL
		for(int i = 0; i < cl.size(); i++) {

			if(cl[i] == custoInsercao[selecionado].verticeInserido) {

				cl.erase(cl.begin() + i);
			}
		}

	}
	
	

}


void Ils::escolher3NosAleatorios(Solucao *s1, int dimensao) {
	
	/* Gera numero aleatorio entre 2 e a dimensão do grafo */	
	std::random_device rd;

	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(2,dimensao);
	
	/* Quantidade de valores que ja foram adicionados 	*/
	int quantidadeAdicionada = 0;
	
	int i = 1;
	
	/* Verifica a existencia, se já foi adicionado ou não 	*/
	bool existe;

	while(quantidadeAdicionada < 3) {
		
		existe = false;		
		int valor = dist(gen);

		for(int i = 0; i < s1->sequencia.size(); i++) {

			if(valor == s1->sequencia[i]) {
				
				existe = true;
			}
		}
		
		// Caso não exista na sequencia, adicione
		if(!existe) {

			s1->sequencia.insert(s1->sequencia.begin() + i, valor);
			quantidadeAdicionada++;
			i++;

		}

	}
}




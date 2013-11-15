#include<stdio.h>
#include<vector>
#include<algorithm>
using namespace std;

vector< vector<int> > g;
vector<int> grau;
vector<int> cor;

void printa(vector<int> v);

int dfs(int src){//Segfault, ta voando
	int mc = 1;
	printf("%d",cor[src]);
	if(cor[src]!=0) return cor[src];
	//else{
		for(int i=0;g[src].size()-1;i++){
			//if(cor[g[src][i]]==0) cor
			int a = dfs(g[src][i]);
			if(mc<a) mc=a;
		}
	//}
	return mc;
}
void init_cor(int src){
	for(int i=0;i<cor.size()-1;i++){
		cor[i]=0;
	}
	cor[src]=1;
} 
int encontra_k(){
	int v = 0;
	init_cor(v);
	int k = dfs(v);
	return k;
}

vector<int> encontra_clique(){
	vector<int> S;
	vector<int> marcados;
	//acha o maior grau em g
	int mi,mg=0;
	for(int i=0;i<g.size();i++){
		if(grau[i]>mg){
			mg=grau[i];
			mi=i;
		}
	}
	
	printf("%d\n",mi);
	printa(grau);
	
	//poe em S, caso base do guloso
	S.push_back(mi);
	int blei=3;
	while(blei--){
		marcados.push_back(S[S.size()-1]);//marca o ultimo inserido
		
		printf("marcados-----------\n");
		printa(marcados);
		
		vector<int> viz;
		//p cada elemento em S, poe todos os seus vizinhos em viz	
		for (int i=0;i<S.size();i++){
			for(int j=0;j<g[S[i]].size();j++){
				viz.push_back(g[S[i]][j]);
			}
		}
		//temos um vetor de vizinhos, mtos deles repetidos,
		//queremos ver quais sao vizinhos de todos em S, ou seja, quais aparecem #S vezes
		//printa(viz);	
		sort(viz.begin(),viz.end());//ordena
	
		printf("vizinhos -----------------\n");
		printa(viz);
		
		int t_uc=0,c=viz[0],ac;
		vector<int> sus;
	
		printf("tamanho de S: %d\n",S.size());
	
		//t_uc quantidade de contados, c quem vai ser contado, ac qnt de vezes q c aparece em viz
		while(t_uc<viz.size()){//enquanto todo mundo n tiver sido contado
			ac = count(viz.begin()+t_uc, viz.end(), c);//conta esse,mas vai diminuindo o espectro da busca, ja q ta ordenado
			t_uc+=ac;
	
			printf("contou %d ocorrencias de %d em vizinhos, no total tem %d\n",ac,c,t_uc);
		
			if(ac==S.size()){//se for vizinho de td mundo
				sus.push_back(c);//eh um suspeito 
			}
			c=viz[t_uc];//agora vamos contar o proximo numero, pulando as repeticoes dos ultimos
		}
	
		printf("suspeitos------------ \n");
		printa(sus);
	
		//agora temos uma serie de suspeitos e temos q achar o de maior grau, para dar o passo do guloso
		//dai tiramos quem ja esta marcado
		vector<int> sus2;
		for(int i=0;i<sus.size();i++){
			if(find(marcados.begin(),marcados.end(),sus[i])!=marcados.end())//se sus[i] nao tiver em marcados ?
				sus2.push_back(sus[i]);
		}
		
		printf("suspeitos n marcados ------------\n");
		printa(sus2);
			
		int mg = 0;
		int mi;
		if(sus2.size()==0) break;//n ha mais ngm no grafo q pode fazer parte do clique
		for(int i=0;i<sus2.size();i++){
			if(grau[sus2[i]]>=mg){
				mg=grau[sus2[i]];
				mi=sus2[i];
			}
		}

		printf("novo membro de S: %d\n\n\n\n",mi);

		//dai zeramos
		viz.clear();
		sus.clear();
		sus2.clear();
		//e damos o passo
		S.push_back(mi);
	}
	return S;
}
/*
void add_aresta(int src, int dst){
	vector<int>::iterator i = find(g.begin(),g.end(),src);
	if(i==g.end()){
		g.push_back(src);
		g[g.size()-1].push_back(dst);
		grau.push_back(1);
	}
	else{
		g[i].push_back(dst);
		grau[i]++;
	}
}*/
void add_aresta(int src, int dst){
	g[src].push_back(dst);
}
void le_grafo(int arestas){
	int a,b;
	for(int i=0;i<arestas;i++){
		scanf("%d %d",&a,&b);
		add_aresta(a,b);
		add_aresta(b,a);
		grau[a]++;
		grau[b]++;
	}
}
void init_grafo(int vertices){
	for(int i=0;i<vertices;i++){
		g.push_back(vector<int>());
	}
}
void init_grau(int vertices){
	for(int i=0;i<vertices;i++){
		grau.push_back(0);
	}
}
void printa(vector<int> v){
	for(int i=0;i<v.size();i++){
		printf("%d ",v[i]);
	}
	printf("\n--%d\n",v.size());
}
int main(){
	int v,e;
	scanf("%d %d",&v,&e);
	init_grafo(v);
	init_grau(v);
	//init_cor();
	le_grafo(e);
	/*
	vector<int> cl = encontra_clique();
	printf("clique\n");
	printa(cl);
	*/
	int k = encontra_k();
	printf("k = %d\n",k);
	return 0;
}

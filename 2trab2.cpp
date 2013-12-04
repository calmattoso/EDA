#include<stdio.h>
#include<vector>
#include<algorithm>
using namespace std;




vector< vector<int> > g;
vector<int> grau;
vector<int> cor;

void printa(vector<int> v);

void dfs(int src){
	int mc = 1;
	int b;
	vector<int> cor_viz;
	for(int i=0;i<g[src].size();i++){
		int a = g[src][i];
			cor_viz.push_back(cor[a]);		
		}
	sort(cor_viz.begin(),cor_viz.end());
	for(int i=0;i<cor_viz.size();i++){
		if(cor_viz[i]==mc) mc++;
	}
	cor[src]=mc;
	for(int i=0;i<g[src].size()-1;i++){
		if(cor[g[src][i]]==0) dfs(g[src][i]);
	}
}
void init_cor(int src){
	cor.resize(g.size());
	for(int i=0;i<cor.size()-1;i++){
		cor[i]=0;//g.size()+1;//inf, N+1, N eh qnd o grafo eh completo
	}
	cor[src]=1;
} 
int encontra_k(){
	int v = 0;
	init_cor(v);
	dfs(v);
	sort(cor.begin(), cor.end());
	int k = cor[cor.size()-1];
	cor.clear();
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

	//poe em S, caso base 
	S.push_back(mi);
	
	while(1){
		marcados.push_back(S[S.size()-1]);//marca o ultimo inserido

		vector<int> viz;
		
		//p cada elemento em S, poe todos os seus vizinhos em viz	
		for (int i=0;i<S.size();i++){
			for(int j=0;j<g[S[i]].size();j++){
				viz.push_back(g[S[i]][j]);
			}
		}
		
		//temos um vetor de vizinhos, mtos deles repetidos,
		//queremos ver quais sao vizinhos de todos em S, ou seja, quais aparecem #S vezes

		sort(viz.begin(),viz.end());
		
		int t_uc=0,c=viz[0],ac;
		vector<int> sus;
	
		//t_uc quantidade de contados, c quem vai ser contado, ac qnt de vezes q c aparece em viz
		while(t_uc<viz.size()){//enquanto todo mundo n tiver sido contado
			ac = count(viz.begin()+t_uc, viz.end(), c);//conta esse,mas vai diminuindo o espectro da busca, ja q ta ordenado
			t_uc+=ac;
		
			if(ac==S.size()){//se for vizinho de td mundo
				sus.push_back(c);//eh um suspeito 
			}
			if(t_uc<viz.size()) c=viz[t_uc];//agora vamos contar o proximo numero, pulando as repeticoes dos ultimos
		}
	
		//agora temos uma serie de suspeitos e temos q achar o de maior grau, para dar o passo do guloso
		//dai tiramos quem ja esta marcado
		vector<int> sus2;
		for(int i=0;i<marcados.size();i++){
			sus.erase(remove(sus.begin(), sus.end(), marcados[i]), sus.end());
		}
		
		int mg = 0;
		int mi;

		if(sus.size()==0) {
			break;//n ha mais ngm no grafo q pode fazer parte do clique
		}
		for(int i=0;i<sus.size();i++){
			if(grau[sus[i]]>=mg){
				mg=grau[sus[i]];
				mi=sus[i];
			}
		}

		//dai zeramos
		viz.clear();
		sus.clear();

		//e damos o passo
		S.push_back(mi);
	}
	return S;
}

vector<int> encontra_cliqueq(vector< vector<int> > h){ //versao generica,enxuta e com firulas
	vector<int> S;
	vector<int> marcados;
	int mi,mg=0;
	for(int i=0;i<h.size();i++){
		if(h[i].size()>mg){
			mg=h[i].size();
			mi=i;
		}
	}
	
	S.push_back(mi);
	while(1){
		marcados.push_back(S[S.size()-1]);

		vector<int> viz;

		for (int i=0;i<S.size();i++){
			for(int j=0;j<h[S[i]].size();j++){
				viz.push_back(h[S[i]][j]);
			}
		}
	
		sort(viz.begin(),viz.end());
	
		int t_uc=0,c=viz[0],ac;
		vector<int> sus;

		while(t_uc<viz.size()){
			ac = count(viz.begin()+t_uc, viz.end(), c);
			t_uc+=ac;
			if(ac==S.size()) sus.push_back(c);
			if(t_uc<viz.size())c=viz[t_uc];
		}

		for(int i=0;i<marcados.size();i++) sus.erase(remove(sus.begin(), sus.end(), marcados[i]), sus.end());
		
		int mg = 0;
		int mi;

		if(sus.size()==0) break;
		
		for(int i=0;i<sus.size();i++){
			if(h[sus[i]].size()>=mg){
				mg=h[sus[i]].size();
				mi=sus[i];
			}
		}
		viz.clear();
		sus.clear();
		S.push_back(mi);
	}
	return S;
}
//versao com grafo nao necessariamente ordenado e completo
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
		//faz tudo ser de 0 a N-1, em vez de 1 a N
		a--;
		b--;
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
int acha(int n, vector<int> a){
	for(int i=0;i<a.size();i++){
		if(a[i]==n) return i;
	}
	return -1;
}
void printa_m(vector< vector<int> > m,int ini, int fim){

	for(int i=ini;i<fim;i++){
		printf("[%d]: ",i);
		for(int j=0; j<m[i].size();j++){
			printf("%d,",m[i][j]);
		}
		printf("\n");
	}
}

int size_m(vector< vector<int> > a){
	int r=0;
	for(int i=0;i<a.size();i++)
		r+=a[i].size();
	return r;
}
vector< vector<int> > Scm;
vector< vector<int> > vl;
vector< vector<int> > encontra_cobertura_minimal(){
	
	
	int q = 0;
	vector<int> marcados;
	
	
	for(int i=0;i<g.size();i++){
		
		vl.push_back(vector<int>());
		
		for(int j=0;j<g[i].size();j++){
			vl[i].push_back(g[i][j]);
		}
	}/*
	for(int i=0;i<g.size();i++){//no final o numero de particoes sera sempre menor q g.size(), mas os po
		Scm.push_back(vector<int>());
	}*/
	while(size_m(vl)!=0){
		//printa_m(vl,0,vl.size());
		//printf("vl s: %d\n",size_m(vl));
		//printf("Scm s: %d\n",size_m(Scm));
		//printf("total: %d\n",size_m(vl)+size_m(Scm));
		Scm.push_back(vector<int>());
		Scm[q] = encontra_cliqueq(vl);
		
		//faz vl/Sq
		//tiramos os vertices de Sq
		// tirando todas as aparicoes de cada vertice em Sq em cada outro vertice em Sq
		//printa(Scm[q]);
		for(int i=0;i<Scm[q].size();i++){

			//printf(".");
			//printa_m(vl,0,vl.size());
			
			//ta voando no meio desse for
			
			for(int j=0;j<Scm[q].size();j++){
				int a = acha(Scm[q][j],vl[Scm[q][i]]); 
				//printf("%d esta na %d posicao em \n",Scm[q][i],a);
				//printa(vl[j]);
				if(a!=-1) {	
					vl[Scm[q][i]].erase(vl[Scm[q][i]].begin()+a);
				//	printf("tirou %d(%d) de %d \n",Scm[q][i],vl[j][a],a);
				}
			}
		}
		//printa_m(vl,0,vl.size());
		//printf("\n..%d\n",q);
		q++;
	}

	//printf("saiu\n");
	//printa_m(Scm,0,Scm.size());
	
	Scm.resize(q);	
	for(int p=0;p<q;p++){
		vector<int> viz;
		for (int i=0;i<Scm[p].size();i++){
			for(int j=0;j<g[Scm[p][i]].size();j++){
				viz.push_back(g[ Scm[p][i] ][ j ]);
			}
		}
		//ta errado
		sort(viz.begin(),viz.end());
		
		int t_uc=0,c=viz[0],ac;
		vector<int> sus;
		while(t_uc<viz.size()){//enquanto todo mundo n tiver sido contado
			ac = count(viz.begin()+t_uc, viz.end(), c);//conta esse,mas vai diminuindo o espectro da busca, ja q ta ordenado
			t_uc+=ac;
			if(ac==Scm[p].size()){//se for vizinho de td mundo
				sus.push_back(c);//eh um suspeito 
			}
			if(t_uc<viz.size()) c=viz[t_uc];//agora vamos contar o proximo numero, pulando as repeticoes dos ultimos
		}
		/*
		for(int i=0;i<marcados.size();i++){
			sus.erase(remove(sus.begin(), sus.end(), marcados[i]), sus.end());
		}
		*/	
		int mg = 0;
		int mi;

		if(sus.size()==0) {	
			continue;//n ha mais ngm no grafo q pode fazer parte do clique
		}
		for(int i=0;i<sus.size();i++){
			if(grau[sus[i]]>=mg){
				mg=grau[sus[i]];
				mi=sus[i];
			}
		}
		//dai zeramos
		viz.clear();
		sus.clear();
		//sus2.clear();
		//e damos o passo
		Scm[p].push_back(mi);
	}
	
	return Scm;
}


int main(){
	int v,e;
	scanf("%d %d",&v,&e);
	init_grafo(v);
	init_grau(v);
	le_grafo(e);
	
	vector<int> cl = encontra_cliqueq(g);
	
	

	vector< vector<int> > cob_min = encontra_cobertura_minimal();
	
	//retira duplicatas
	sort(cob_min.begin(), cob_min.end());
	for(int i = 0;i<cob_min.size();i++){
		sort(cob_min[i].begin(), cob_min[i].end());
	}
	cob_min.erase(std::unique(cob_min.begin(), cob_min.end()), cob_min.end());
	
	//printf("clique:\n");	
	printa(cl);
	
	//printf("cobertura minimal:\n");
	//printa_m(cob_min,0,cob_min.size());

	//printf("numero de particoes: ");
	printf("%d\n",cob_min.size());
	//printf("tamanhos das particoes:\n");
	
	double med = 0;
	for(int i = 0;i<cob_min.size();i++){
		med+=cob_min[i].size();
		printf("%d ",cob_min[i].size());
	}
	med = med/cob_min.size();
	printf("\n");
	//printf("tamanho medio das particoes: ");
	printf("%lf\n", med);
	

	
	int k = encontra_k();
	//printf("k = ");
	printf("%d\n",k);
	
	return 0;
}

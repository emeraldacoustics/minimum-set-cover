#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int maxN = 300;
const int maxM = 300;

int N, M, K;
int grid[maxN][maxM];
int valcnt;
int val[maxN*maxM];
vector<int> E[maxN];
int R, C;
bool vis[maxN];
int rlnk[maxN];
int clnk[maxM];
bool rslt[maxN];
bool cslt[maxM];

bool found(int u)
{
	int i;
	int v;

	for(i=0 ; i<E[u].size() ; ++i)
	{
		v = E[u][i];

		if(!vis[v])
		{
			vis[v] = true;

			if(clnk[v]==-1 || found(clnk[v]))
			{
				rlnk[u] = v;
				clnk[v] = u;
				return true;
			}
		}
	}

	return false;
}

bool possible(int x)
{
	int i, j;
	int rlt;

	for(i=0 ; i<N ; ++i)
	{
		E[i].clear();
		for(j=0 ; j<M ; ++j)
		{
			if(grid[i][j] > x)
				E[i].push_back(j);
		}
	}

	rlt = 0;
	memset(rlnk, -1, sizeof(rlnk[0])*N);
	memset(clnk, -1, sizeof(clnk[0])*M);
	for(i=0 ; i<N ; ++i)
	{
		memset(vis, false, sizeof(vis[0])*M);
		if(found(i))
			++rlt;
	}

	return rlt<=K;
}

void dfs(int u)
{
	int i;
	int v;

	vis[u] = true;
	for(i=0 ; i<E[u].size() ; ++i)
	{
		v = E[u][i];

		if(!cslt[v])
		{
			cslt[v] = true;
			++C;
		}
		if(!vis[clnk[v]])
			dfs(clnk[v]);
	}
}

void get_cover(void)
{
	int i;

	R = 0;
	C = 0;
	memset(rslt, false, sizeof(rslt[0])*N);
	memset(cslt, false, sizeof(cslt[0])*M);
	memset(vis, false, sizeof(vis[0])*N);
		
	for(i=0 ; i<N ; ++i)
	{
		if(rlnk[i]==-1 && !vis[i])
			dfs(i);
	}

	for(i=0 ; i<N ; ++i)
	{
		if(rlnk[i]!=-1 && !vis[i])
		{
			rslt[i] = true;
			++R;
		}
	}

	for(i=0 ; i<N&&(R+C)<K ; ++i)
	{
		if(!rslt[i])
		{
			rslt[i] = true;
			++R;
		}
	}

	for(i=0 ; i<M&&(R+C)<K ; ++i)
	{
		if(!cslt[i])
		{
			cslt[i] = true;
			++C;
		}
	}
}

int main(void)
{
	int CN;
	int low, high, mid;
	int i, j;

	for(CN=1 ; scanf("%d %d %d", &N, &M, &K)==3 ; ++CN)
	{
		valcnt = 0;
		for(i=0 ; i<N ; ++i)
		{
			for(j=0 ; j<M ; ++j)
			{
				scanf("%d", grid[i]+j);
				val[valcnt] = grid[i][j];
				++valcnt;
			}
		}

		sort(val, val+valcnt);
		valcnt = unique(val, val+valcnt)-val;

		low = -1;
		high = valcnt;
		for( ; low<high-1 ; )
		{
			mid = low+(high-low)/2;

			if(possible(val[mid]))
				high = mid;
			else
				low = mid;
		}

		possible(val[high]);
		get_cover();

		printf("%d\n", val[high]);
		
		printf("%d", R);
		for(i=0 ; i<N ; ++i)
		{
			if(rslt[i])
				printf(" %d", i+1);
		}
		printf("\n");

		printf("%d", C);
		for(i=0 ; i<M ; ++i)
		{
			if(cslt[i])
				printf(" %d", i+1);
		}
		printf("\n");
	}

	return 0;
}
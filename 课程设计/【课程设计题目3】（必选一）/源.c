#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<windows.h>
#define MAX 16
#define INFINITY 10000

struct MGraph//邻接矩阵
{
	char* vexs[MAX];
	int** arcs;
	int vexnum;
};

struct MGraph* MGraphCreat()//创建邻接矩阵
{
	FILE* fp;
	if (!(fp = fopen("data.txt", "r")))
	{
		printf("File opened failed!\n");
		exit(0);
	}
	struct MGraph* Graph = (struct MGraph*)malloc(sizeof(struct MGraph));
	fscanf(fp, "%d", &Graph->vexnum);
	if (Graph->vexnum > MAX)
	{
		printf("Array out of bounds!\n");
		exit(0);
	}
	for (int i = 0; i < Graph->vexnum; i++)
	{
		char* str = (char*)malloc(sizeof(char) * 100);
		fscanf(fp, "%s", str);
		Graph->vexs[i] = str;
	}
	int** matrix = (int**)malloc(sizeof(int*) * Graph->vexnum);
	for (int i = 0; i < Graph->vexnum; i++)
	{
		matrix[i] = (int*)malloc(sizeof(int) * Graph->vexnum);
		for (int j = 0; j < Graph->vexnum; j++)
		{
			fscanf(fp, "%d", &matrix[i][j]);
			if (!matrix[i][j])matrix[i][j] = INFINITY;
		}
	}
	fclose(fp);
	Graph->arcs = matrix;
	return Graph;
}

struct CloseEdge//记录最小权值边的信息
{
	int distance;
	int row;
	int col;
};

void Prime(struct MGraph* Graph)//Prime算法
{
	int vexnum = Graph->vexnum;
	int** matrix = Graph->arcs;
	bool* visited = (bool*)malloc(sizeof(bool) * vexnum);
	struct CloseEdge* closeEdge = (struct CloseEdge*)malloc(sizeof(struct CloseEdge) * vexnum);
	for (int i = 0; i < vexnum; i++)
	{
		visited[i] = false;
		closeEdge[i].distance = INFINITY;
		closeEdge[i].row = i;
		closeEdge[i].col = i;
	}
	int mark = 0, count = vexnum - 1, sum = 0;
	printf("Result of Prime: \n");
	while (count--)
	{
		visited[mark] = true;
		int i = mark, min = INFINITY;
		for (int j = 0; j < vexnum; j++)
		{
			if (!visited[j] && matrix[i][j] < closeEdge[j].distance)
			{
				closeEdge[j].distance = matrix[i][j];
				closeEdge[j].row = i;
				closeEdge[j].col = j;
			}
			if (closeEdge[j].distance < min)
			{
				min = closeEdge[j].distance;
				mark = j;
			}
		}
		sum = sum + closeEdge[mark].distance;
		closeEdge[mark].distance = INFINITY;
		printf("(%s,%s)\n", Graph->vexs[closeEdge[mark].row], Graph->vexs[closeEdge[mark].col]);
	}
	printf("Distance: %d\n\n", sum);
}

struct TArry//三元组表
{
	int distance;
	int vertx01;
	int vertx02;
};

void copy(struct TArry* tArry01, struct TArry* tArry02)//将tArry02的信息复制给tArry01
{
	tArry01->distance = tArry02->distance;
	tArry01->vertx01 = tArry02->vertx01;
	tArry01->vertx02 = tArry02->vertx02;
}

void QuickSort(struct TArry* tArry, int top, int bottom)//快速排序
{
	if (top < bottom)
	{
		int i = top, j = bottom;
		struct TArry* flag = (struct TArry*)malloc(sizeof(struct TArry));
		struct TArry* temp = (struct TArry*)malloc(sizeof(struct TArry));
		copy(flag, &tArry[i]);
		while (i < j)
		{
			while (tArry[j].distance >= flag->distance)j--;
			if (i < j)
			{
				copy(temp, &tArry[i]);
				copy(&tArry[i++], &tArry[j]);
				copy(&tArry[j], temp);
			}
			while (tArry[i].distance < flag->distance)i++;
			if (i < j)
			{
				copy(temp, &tArry[j]);
				copy(&tArry[j--], &tArry[i]);
				copy(&tArry[i], temp);
			}
		}
		copy(&tArry[i], flag);
		QuickSort(tArry, top, i - 1);
		QuickSort(tArry, i + 1, bottom);
	}
}

bool judgement(struct TArry* tArry, int* unionFind, int i)//判断两个顶点是否属于同一集合
{
	int vertx01 = tArry[i].vertx01, vertx02 = tArry[i].vertx02;
	int temp01 = unionFind[vertx01], temp02 = unionFind[vertx02];
	while (temp01 != unionFind[temp01])
	{
		temp01 = unionFind[temp01];
	}
	while (temp02 != unionFind[temp02])
	{
		temp02 = unionFind[temp02];
	}
	if (temp01 == temp02)return false;
	else
	{
		unionFind[temp01] = unionFind[temp02];
		return true;
	}
}

void Kruskal(struct MGraph* Graph)//Kruskal算法
{
	int vexnum = Graph->vexnum;
	int** matrix = Graph->arcs;
	int arcnum = vexnum * (vexnum - 1) / 2;
	struct TArry* tArry = (struct TArry*)malloc(sizeof(struct TArry) * arcnum);
	int* unionFind = (int*)malloc(sizeof(int) * vexnum);
	int index = 0;
	for (int i = 0; i < vexnum; i++)
	{
		unionFind[i] = i;
		for (int j = i + 1; j < vexnum; j++)
		{
			tArry[index].distance = matrix[i][j];
			tArry[index].vertx01 = i;
			tArry[index].vertx02 = j;
			index++;
		}
	}
	QuickSort(tArry, 0, arcnum - 1);
	int count = 1, sum = 0;
	printf("Result of Kruskal: \n");
	for (int i = 0; i < arcnum && count < vexnum; i++)
	{
		if (judgement(tArry, unionFind, i))
		{
			printf("(%s,%s)\n", Graph->vexs[tArry[i].vertx01], Graph->vexs[tArry[i].vertx02]);
			count++;
			sum = sum + tArry[i].distance;
		}
	}
	printf("Distance: %d\n\n", sum);
}

void Dijkstra(struct MGraph* Graph)//Dijkstra算法
{
	int vexnum = Graph->vexnum;
	int** matrix = Graph->arcs;
	bool* visited = (bool*)malloc(sizeof(bool) * vexnum);
	int* bestDistance = (int*)malloc(sizeof(int) * vexnum);
	int* prior = (int*)malloc(sizeof(int) * vexnum);
	int count = vexnum - 1, best = 0, origin = 0;
	for (int i = 0; i < vexnum; i++)
	{
		visited[i] = false;
		bestDistance[i] = matrix[origin][i];
		prior[i] = origin;
	}
	prior[origin] = -1;
	int index = origin;
	printf("Result of Dijkstra: \n");
	while (count--)
	{
		visited[index] = true;
		int i = index, min = INFINITY;
		for (int j = 0; j < vexnum; j++)
		{
			if (!visited[j])
			{
				if (matrix[i][j] + best < bestDistance[j])
				{
					bestDistance[j] = matrix[i][j] + best;
					prior[j] = i;
				}
				if (bestDistance[j] < min)
				{
					min = bestDistance[j];
					index = j;
				}
			}		
		}
		best = bestDistance[index];
		printf("From %s to %s: %d\n", Graph->vexs[origin], Graph->vexs[index], best);
	}
	printf("\n");
}

struct ArcNode//节点
{
	int adjvex;
	struct ArcNode* nextarc;
	int distance;
};

struct VNode//首节点
{
	char* vertx;
	int indegree;
	struct ArcNode* firstarc;
};

struct ALGraph//邻接表
{
	struct VNode* vertices;
	int vexnum;
};

struct ALGraph* ALGraphCreat()//创建邻接表
{
	FILE* fp;
	fp = fopen("data.txt", "rb");
	if (!fp)
	{
		printf("File open failed!\n");
		exit(0);
	}
	struct ALGraph* Graph = (struct ALGraph*)malloc(sizeof(struct ALGraph));
	fscanf(fp, "%d", &Graph->vexnum);
	struct VNode* NodeList = (struct VNode*)malloc(sizeof(struct VNode) * Graph->vexnum);
	for (int i = 0; i < Graph->vexnum; i++)
	{
		char* str = (char*)malloc(sizeof(char) * 100);
		fscanf(fp, "%s", str);
		NodeList[i].vertx = str;
		NodeList[i].indegree = 0;
		NodeList[i].firstarc = NULL;
	}
	int** matrix = (int**)malloc(sizeof(int*) * Graph->vexnum);
	for (int i = 0; i < Graph->vexnum; i++)
	{
		matrix[i] = (int*)malloc(sizeof(int) * Graph->vexnum);
		for (int j = 0; j < Graph->vexnum; j++)
		{
			fscanf(fp, "%d", &matrix[i][j]);
			if (!matrix[i][j])matrix[i][j] = INFINITY;
		}
	}
	int vex01, vex02;
	fscanf(fp, "%d%d", &vex01, &vex02);
	while (vex01 != vex02)
	{
		struct ArcNode* NewNode = (struct ArcNode*)malloc(sizeof(struct ArcNode));
		NewNode->adjvex = vex02;
		NewNode->nextarc = NodeList[vex01].firstarc;
		NewNode->distance = matrix[vex01][vex02];
		NodeList[vex01].firstarc = NewNode;
		NodeList[vex02].indegree++;
		fscanf(fp, "%d%d", &vex01, &vex02);
	}
	Graph->vertices = NodeList;
	fclose(fp);
	return Graph;
}

void TopologicalSort(struct ALGraph* Graph)//拓扑排序
{
	int vexnum = Graph->vexnum, top = 0;
	int* stack = (int*)malloc(sizeof(int) * MAX);
	struct VNode* NodeList = Graph->vertices;
	for (int i = 0; i < vexnum; i++)
	{
		if (NodeList[i].indegree == 0)
		{
			stack[top++] = i;
			NodeList[i].indegree--;
		}
	}
	printf("Result of TopologicalSort: \n");
	while (top--)
	{
		int index = stack[top];
		printf("%s->", NodeList[index].vertx);
		struct ArcNode* Node = NodeList[index].firstarc;
		while (Node != NULL)
		{
			index = Node->adjvex;
			NodeList[index].indegree--;
			Node = Node->nextarc;
		}
		for (int i = 0; i < vexnum; i++)
		{
			if (NodeList[i].indegree == 0)
			{
				stack[top++] = i;
				NodeList[i].indegree--;
			}
		}
	}
	printf("End\n\n");
}

void DFS(struct VNode* NodeList, struct ArcNode* Node, int sum, int top, int* temp, int* stack, int* path)//深度优先搜索
{
	if (Node == NULL)
	{
		if (sum > temp[0])
		{
			temp[0] = sum;
			for (int i = 0; i < top; i++)
			{
				path[i] = stack[i];
				temp[2] = temp[1];
			}
			path[top] = '\0';
		}
		return;
	}
	while (Node != NULL)
	{
		int index = Node->adjvex;
		stack[top++] = index;
		sum = sum + Node->distance;
		DFS(NodeList, NodeList[index].firstarc, sum, top, temp, stack, path);
		sum = sum - Node->distance;
		Node = Node->nextarc;
		top--;
	}
}

void CriticalPath(struct ALGraph* Graph)//关键路径
{
	int vexnum = Graph->vexnum, top = 0;
	int* stack = (int*)malloc(sizeof(int) * MAX);
	int* path = (int*)malloc(sizeof(int) * MAX);
	int* temp = (int*)malloc(sizeof(int) * 3);
	struct VNode* NodeList = Graph->vertices;
	for (int i = 0; i < 3; i++)
		temp[i] = 0;
	for (int i = 0; i < vexnum; i++)
	{
		if (NodeList[i].indegree == 0)
		{
			temp[1] = i;
			struct ArcNode* Node = NodeList[i].firstarc;
			DFS(NodeList, Node, 0, top, temp, stack, path);
		}
	}
	printf("Result of CriticalPath: \n");
	printf("%s->", NodeList[temp[2]].vertx);
	while (path[top] != '\0')
	{
		int index = path[top];
		printf("%s->", NodeList[index].vertx);
		top++;
	}
	printf("End\n\n");
}

void SolutionForOneAndTwo()//基于邻接矩阵创建的图所得的方案集合
{
	struct MGraph* Graph = MGraphCreat();
	Prime(Graph);
	Kruskal(Graph);
	Dijkstra(Graph);
}

void SolutionForThree()//基于邻接表法创建的图所得的方案集合
{
	struct ALGraph* Graph = ALGraphCreat();
	CriticalPath(Graph);
	TopologicalSort(Graph);
}

int main()
{
	SetConsoleOutputCP(65001);
	SolutionForOneAndTwo();
	SolutionForThree();
	system("pause");
	return 0;
}
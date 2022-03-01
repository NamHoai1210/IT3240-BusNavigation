
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jval.h"
#include"dllist.h"
#include"jrb.h"
#define INFINITY_VALUE 1000000
#define MAXN 100
typedef enum { false, true } bool;
//----------------------------------------------------------
typedef struct{
    JRB edges;
    JRB vertices;
} Graph;
typedef struct{
  int point;
  double weight;
}Path;
typedef struct{
  int point;
  double weight;
  char sta[256];
}PathBus;
typedef struct{
  char bus[10];
  char start[256];
  char end[256];
}Bus;
typedef struct{
  double cost;
  int length;
  Path path[100];
} Object;
typedef struct{
  char str[10];
} StringArray;
typedef struct{
  char n[256];
  int size;
  StringArray arr[20];
}station;
typedef station* Station;
//----------------------------------------------------------
Graph createGraph ();
void dropGraph (Graph graph);
void addVertex (Graph graph, int id, Station name);
Station getVertex(Graph graph, int id);
int getVertexID(Graph graph, char* c);
void addEdge (Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int outdegree(Graph graph, int v, int* output);
void Relax(Graph G, Object ob[],int u,int v);
void TopologicalSortUtil(Graph g,int v,int visited[],Dllist stack);
void TopologicalSort(Graph g,int a,Dllist stack);
double shortestPath(Graph graph, int  s, int t, Path* path, int* length);
double PathOfItem(Graph g,char *c,int s1,int s2,PathBus* path,int*length);
void printShortestPath(Graph g, int s1, int s2);
int input_station(Graph g);
void ReadfileBusLine(Graph g);
void ReadfileBus(Bus* bus,int *length_bus);
void menu();
//----------------------------------------------------------
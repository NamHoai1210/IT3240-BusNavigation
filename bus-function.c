#include"bus-header.h"
Graph createGraph(){
    Graph new;
    new.edges=make_jrb();
    new.vertices=make_jrb();
    return new;
}
JRB jrb_find_id(JRB tree, Jval value){
  JRB tmp;
  jrb_traverse(tmp,tree){
    if((tmp->key).i==(value).i){
      return tmp;
    }
  }
  return NULL;
}
void dropGraph(Graph graph){
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}
void addVertex(Graph graph, int id, Station name){
    jrb_insert_int(graph.vertices,id,new_jval_v(name));
}
Station getVertex(Graph graph, int id){
    JRB node=jrb_find_id(graph.vertices,new_jval_i(id));
    if(node==NULL) return NULL;
    return (Station)jval_v(node->val);
}
int getVertexID(Graph graph, char* c){
    JRB node;
    Station st=NULL;
    jrb_traverse(node,graph.vertices){
      st=jval_v(node->val);
      if(strcmp(st->n,c)==0)
      return jval_i(node->key);
    }
    return INFINITY_VALUE;
}
void addEdge(Graph graph, int v1, int v2,double weight){
    JRB node=jrb_find_int(graph.edges,v1);
    JRB tree,tmp;
    if(node==NULL){
        tree = make_jrb();
        jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }else{
        tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
}
double getEdgeValue(Graph graph, int v1, int v2){
    JRB node1=jrb_find_int(graph.edges,v1);
    if(node1==NULL){
        return INFINITY_VALUE;
    }
    JRB tree,node;
    tree = (JRB) jval_v(node1->val);
    double total = INFINITY_VALUE;
    node=jrb_find_int(tree,v2);
    if(node!=NULL) total=jval_d(node->val);
    return total;
}
int outdegree(Graph graph, int v, int* output){
  JRB node,tree;
  node = jrb_find_int(graph.edges, v);
  if(node==NULL)return 0;
    else tree = (JRB)jval_v(node->val);
  int total=0;
  jrb_traverse(node, tree) output[total++] = jval_i(node->key);
  return total;
}

void Relax(Graph G, Object ob[],int u,int v){
  double p=getEdgeValue(G,u,v);
  double q=p+ob[u].cost;
  if(q<ob[v].cost){
    ob[v]=ob[u];
    ob[v].cost=q;
    ob[v].length++;
    ob[v].path[ob[v].length].point=v;
    ob[v].path[ob[v].length].weight=p;
    }
}
double shortestPath(Graph graph, int  s, int t, Path* path, int* length){
  Dllist list=new_dllist();
  Dllist node,ptr;
  int x,n,i,a[MAXN];
  int delmin;
  bool final[MAXN];
  Object ob[MAXN];
  for(i=0;i<MAXN;i++){
    ob[i].cost=INFINITY_VALUE;
    ob[i].length=0;
    ob[i].path[0].point=s;
    ob[i].path[0].weight=0;
    final[i]=false;
  }
  ob[s].cost=0;
  dll_append(list,new_jval_i(s));
  while(dll_empty(list)==0){ 
    delmin=INFINITY_VALUE;
    dll_traverse(ptr,list){
      x=jval_i(ptr->val);
      if(ob[x].cost<delmin){
        delmin=ob[x].cost;
        node=ptr;
      }
    }
    x=jval_i(node->val);
    dll_delete_node(node);
    final[x]=true;
    if(x==t) break;
      n=outdegree(graph,x,a);
      for(i=0;i<n;i++) 
      if(final[a[i]]==false){
        Relax(graph,ob,x,a[i]);
        dll_append(list,new_jval_i(a[i]));
      }
  }
  for(i=0;i<=ob[t].length;i++){
    path[i]=ob[t].path[i];
  }
  *length=ob[t].length;
  return ob[t].cost;
}
double PathOfItem(Graph g,char *c,int s1,int s2,PathBus* path,int*length){
  if(s1==s2) return INFINITY_VALUE;
  *length=1;
  Station st;
  st=getVertex(g,s1);
  path[0].point=s1;
  strcpy(path[0].sta,st->n);
  int x,i;
  double dist=0;
  JRB node,tree,tmp;
  while(1){
    x=*length-1;
    if(path[x].point==s2){
      break;
    }
    node=jrb_find_int(g.edges,path[x].point);
    if(node==NULL){return INFINITY_VALUE;}
    else{
      tree=(JRB)jval_v(node->val);
      jrb_traverse(tmp,tree){
        st=getVertex(g,jval_i(tmp->key));
        for(i=0;i<st->size;i++){
        if(strcmp((st->arr[i]).str,c)==0){
          path[*length].point=jval_i(tmp->key);
          path[*length].weight=jval_d(tmp->val);
          strcpy(path[*length].sta,st->n);
          *length=(*length)+1;
          dist+= jval_d(tmp->val);
          break;
        }
      }
      if(*length>x+1) break;
      }
      if(*length==x+1) return INFINITY_VALUE;
    }
  }
  return dist;
}

void ReadfileBusLine(Graph g){
  FILE *fpt=fopen("busline.txt","r");
  char station1[256],station2[256],s[256],distChar[256],s1[MAXN];
  int id1=0,id2=0;
  int test=0,key,i;
  double dist;

  while(1){
    if(feof(fpt))break;
    test++;

    fscanf(fpt,"%[^\n]*c",s);fgetc(fpt);
    fscanf(fpt,"%lf",&dist);fgetc(fpt);
    //printf("%s %g\n",s,dist);
    for(i=0;i<strlen(s);i++)
      if(s[i]!='/') station1[i]=s[i];
        else{
          station1[i]='\0';
          key=i+1;
          break;
        }

      for(i=key;i<strlen(s);i++) station2[i-key]=s[i];
          station2[i-key]='\0';

    id1=getVertexID(g,station1);
    id2=getVertexID(g,station2);
    if(id1==INFINITY_VALUE||id2==INFINITY_VALUE){
      printf(" !!! ERROR in line %d\n",test);
      continue;
    }
    addEdge(g,id1,id2,dist);
  }
  fclose(fpt);
}
void ReadfileBus(Bus* bus,int *length_bus){
  FILE *fpt=fopen("bus.txt","r");
  char s[1000]; int i,key;
  *length_bus=0;
  while(1){
    if(feof(fpt)) break;

    fscanf(fpt,"%[^\n]*c",bus[*length_bus].bus);fgetc(fpt);
      //printf("%s ",bus[*length_bus].bus);
    fscanf(fpt,"%[^\n]*c",s);fgetc(fpt);
      //printf("%s\n",s);

    for(i=0;i<strlen(s);i++)
      if(s[i]!='/') bus[*length_bus].start[i]=s[i];
        else{
          bus[*length_bus].start[i]='\0';
          key=i+1;
          break;
        }

      for(i=key;i<strlen(s);i++) bus[*length_bus].end[i-key]=s[i];
          bus[*length_bus].end[i-key]='\0';    
      //printf("%s %s\n",bus[*length_bus].start,bus[*length_bus].end);

    *length_bus=*length_bus+1;
  }

  fclose(fpt);
}
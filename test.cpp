#include <bits/stdc++.h>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

#define ui unsigned int
 #define listSize 4294967296  // 定义数组的大小 = 4294967295 + 1 (unsigned int 的最大值 + 1)
#define Size 2000000

typedef struct node
{
    int id2;
    int money = 0;
}Node;

int cmp(Node node1, Node node2){
    return node1.id2 < node2.id2;
}

bool validIds[] = {0,0,1,1,0,0,0,0,1,0,0,0,};

// 发送向量的结构体
typedef struct edge{
    ui id2;
    ui money;
    struct edge* next = NULL;
}EDGE;


// 反向图中的边, 指向发送者们
typedef struct re_edge{
    ui id1;
    struct re_edge* next = NULL;
}RE_EDGE;

map<ui, EDGE*>Map;

int main(){
    

    ui kk[listSize];
    ui k1[listSize];
    ui k2[listSize];
    ui k3[listSize];
    ui k4[listSize];
    ui k5[listSize];
    ui k6[listSize];
    ui k7[listSize];
    ui k8[listSize];
    ui k9[listSize];
    ui k10[listSize];
    ui k11[listSize];
    
    // 建立一个结构体链表
    clock_t start;
   


//建立一个结构体数组
    start = clock();
    EDGE* L[Size];
    for(int i=0; i<Size; i++){
        EDGE* temp_edge = (EDGE*)malloc(sizeof(EDGE*));
        temp_edge->money = i + 1;
        L[i] = temp_edge;
    }
    cout << "list write: " << clock() - start << "ms" << endl;

    start = clock();
    for(int i=0; i<Size; i++){
        ui M = L[i]->money;
    }
    cout << "list read: " << clock() - start << "ms" << endl;



     start = clock();
    EDGE head_edge;
    head_edge.money = 0+1;
    EDGE* tail = &head_edge;
    for(int i=1; i<Size; i++){
        EDGE* temp_edge = (EDGE*)malloc(sizeof(EDGE*));
        temp_edge->money = i+1;
        tail->next = temp_edge;
        tail = tail->next;
    }
    cout << "chain write: " << clock() - start << "ms" << endl;

    start = clock();
    ui M;
    tail = &head_edge;
    for(int i=0; i<Size-1; i++){
        M = tail->money;
        tail = tail->next;
    }
    clock_t end = clock();
    cout << "chain read: " << end - start << "ms" << endl;

  
    // //利用结构体列表直接存
    start = clock();
    // EDGE U[Size];
    // for(int i=0; i<Size; i++){
    //     // EDGE* temp_edge = (EDGE*)malloc(sizeof(EDGE*));
    //     // U[i] = *temp_edge;
    //     U[i].money = i+1;
    // }
    cout << "struct list write: " << clock() - start << "ms" << endl;
    
    // start = clock();
    //  for(int i=0; i<Size; i++){
    //     ui M = U[i].money;
    // }
    // cout << "struct list read: " << clock() - start << "ms" << endl;
    sleep(10); 


    return 0;
}

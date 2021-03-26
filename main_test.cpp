#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<unordered_map>
using namespace std;

typedef unsigned int uint;

//正向边
typedef struct edge{
	int id2;
	uint money;
	edge* next = NULL;
}EDGE;

//反向边
typedef struct re_edge{
    int id1;
	re_edge* next = NULL;
}RE_EDGE;


EDGE Graph[4000001];
RE_EDGE Graph2[4000001];




uint ids1[2000001];
uint ids2[2000001];
uint transfer_amount[2000001];

uint* ids1_ptr=ids1;
uint* ids2_ptr=ids2;
uint* money_ptr=transfer_amount;

//记录映射的id
unordered_map<uint, int> idHash;


uint valid_ids[4000001];  //记录所有的合法id
bool id_is_recorded[4000001];
uint* valid_ids_ptr = valid_ids;

int valid_Num = 0;  // 记录合法id数量

bool inPath1[4000001];
bool inPath2[4000001];
bool inPath3[4000001];
bool inPath4[4000001];

//保存current节点
EDGE* current_node1;
EDGE* current_node2;
EDGE* current_node3;
EDGE* current_node4;


//保存root的下一个节点
EDGE* root_next1;
EDGE* root_next2;
EDGE* root_next3;
EDGE* root_next4;

//保存it的下一个节点
EDGE* it_next1;
EDGE* it_next2;
EDGE* it_next3;
EDGE* it_next4;

EDGE* last_valid_node1;
EDGE* last_valid_node2;
EDGE* last_valid_node3;
EDGE* last_valid_node4;

//浮动参数
double factor1 = 0;
double factor2 = 0;
double factor3 = 0;
double factor4 = 0;

int  inDegrees[4000001];
int  outDegrees[4000001];

int path1[7];
int path2[7];
int path3[7];
int path4[7];
int* path_pointer1 = path1;
int* path_pointer2 = path2;
int* path_pointer3 = path3;
int* path_pointer4 = path4;

int ansCount1 = 0;
int ansCount2 = 0;
int ansCount3 = 0;
int ansCount4 = 0;

// circleSize1[0] 表示线程1中长度为3的环构成的总字符数，以此类推
int circleSize1[5] = { 0,0,0,0,0 };
int circleSize2[5] = { 0,0,0,0,0 };
int circleSize3[5] = { 0,0,0,0,0 };
int circleSize4[5] = { 0,0,0,0,0 };

int charSize[4000001];  //记录数字对应的char大小
char char_Int[4000001][10];  //记录id对应的char数组

int inNeighborhoods1[4000001];
int lastPoint1[4000001];
int withLastOne1[4000001];
int withLastSecond1[4000001];
double result1 = 0;

int inNeighborhoods2[4000001];
int lastPoint2[4000001];
int withLastOne2[4000001];
int withLastSecond2[4000001];
double result2 = 0;

int inNeighborhoods3[4000001];
int lastPoint3[4000001];
int withLastOne3[4000001];
int withLastSecond3[4000001];
double result3 = 0;

int inNeighborhoods4[4000001];
int lastPoint4[4000001];
int withLastOne4[4000001];
int withLastSecond4[4000001];
double result4 = 0;

//int ans3[3*8000000];
int ans31[24000000];
int ans32[24000000];
int ans33[24000000];
int ans34[24000000];
//int ans4[4*8000000];
int ans41[32000000];
int ans42[32000000];
int ans43[32000000];
int ans44[32000000];
//int ans5[5*8000000];
int ans51[40000000];
int ans52[40000000];
int ans53[40000000];
int ans54[40000000];
//int ans6[6*8000000];
int ans61[48000000];
int ans62[48000000];
int ans63[48000000];
int ans64[48000000];
//int ans7[7*8000000];
int ans71[56000000];
int ans72[56000000];
int ans73[56000000];
int ans74[56000000];


int* ans1[] = { ans31,ans41,ans51,ans61,ans71 };
int* ans2[] = { ans32,ans42,ans52,ans62,ans72 };
int* ans3[] = { ans33,ans43,ans53,ans63,ans73 };
int* ans4[] = { ans34,ans44,ans54,ans64,ans74 };

int circleCount1[5] = { 0,0,0,0,0 };
int circleCount2[5] = { 0,0,0,0,0 };
int circleCount3[5] = { 0,0,0,0,0 };
int circleCount4[5] = { 0,0,0,0,0 };


//去除入度为0的id

void rm_indegree_eq_0(int cur_id)
{
	
    EDGE* recver_ptr=Graph[cur_id].next;
	for (;recver_ptr!=NULL;recver_ptr=recver_ptr->next){
	    int recver_id=recver_ptr->id2;
		inDegrees[recver_id]=max(inDegrees[recver_id] - 1,0);
		if (id_is_recorded[recver_id] && inDegrees[recver_id] == 0)  //若cur_id是有效的但是入度为0
        {
            id_is_recorded[recver_id] = false; // 将cur_id标记为无效点
            rm_indegree_eq_0(recver_id); //递归去除入度为0的点
        }	
	}
}

//去除出度为0的id
void rm_oudegree_eq_0(uint cur_id)
{
	RE_EDGE* sender_ptr=Graph2[cur_id].next;
    for (; sender_ptr != NULL; sender_ptr=sender_ptr->next)
    {
        int sender_id = sender_ptr->id1;
        outDegrees[sender_id] = max(outDegrees[sender_id] - 1,0);
        if (id_is_recorded[sender_id] && outDegrees[sender_id] == 0)  //若cur_id是无效的但出度为0
        {
            id_is_recorded[sender_id] = false; // 将cur_id标记为无效点
            rm_oudegree_eq_0(sender_id); //递归去除入度为0的点
        }
    }
}


void change(uint N)
{
    int size = 0;
    uint n = N;
	if(n==0){
	   char_Int[idHash[N]][0] =  '0';
	   charSize[idHash[N]] = 1;
	}else{
	    while (n){
            char_Int[idHash[N]][size++] = n % 10 + '0';
            n /= 10;
        }
        charSize[idHash[N]] = size;
	}
}

int char_num_of_int(uint n)
{
    int char_num = 0;
    while (n)
    {
        char_num++;
        n /= 10;
    }
    return char_num + 1;
}

//写入带回车的数字
int write_with_enter(char* addr, int n)
{
    int char_cache[10], idx = 0;
    int char_num = 0;
    while (n)
    {
        char_cache[idx++] = n % 10 + '0';
        n /= 10;
    }
    char_num = idx;
    while (idx)
    {
        *addr++ = char_cache[--idx];
    }
    *addr++ = '\n';
    return char_num + 1;
}

void markNeighborhoods1(int root, int current, int length)
{
 
    for (RE_EDGE* it = Graph2[current].next; it != NULL; it=it->next)
    {

        if (it->id1 < root || it->id1 == root) continue;

        if (inPath1[it->id1]) continue;

        if (length == 1)
        {
            lastPoint1[it->id1] = root;
            inNeighborhoods1[it->id1] = root;
        }
        else if (length == 2)
        {  
            withLastOne1[it->id1] = root;
            inNeighborhoods1[it->id1] = root;
        }
        else
        {
            withLastSecond1[it->id1] = root;
            inNeighborhoods1[it->id1] = root;
            continue;
        }

        inPath1[it->id1] = true;

        markNeighborhoods1(root, it->id1, length + 1);

        inPath1[it->id1] = false;
    }
}

void markNeighborhoods2(int root, int current,  int length)
{

    for (RE_EDGE* it = Graph2[current].next; it != NULL; it=it->next)
    {

        if (it->id1 < root || it->id1 == root) continue;

        if (inPath2[it->id1]) continue;

        if (length == 1)
        {
            lastPoint2[it->id1] = root;
            inNeighborhoods2[it->id1] = root;
        }
        else if (length == 2)
        {  
            withLastOne2[it->id1] = root;
            inNeighborhoods2[it->id1] = root;
        }
        else
        {
            withLastSecond2[it->id1] = root;
            inNeighborhoods2[it->id1] = root;
            continue;
        }

        inPath2[it->id1] = true;

        markNeighborhoods2(root, it->id1, length + 1);

        inPath2[it->id1] = false;
    }
}

void markNeighborhoods3(int root, int current,  int length)
{

    for (RE_EDGE* it = Graph2[current].next; it != NULL; it=it->next)
    {

        if (it->id1 < root || it->id1 == root) continue;

        if (inPath3[it->id1]) continue;

        if (length == 1)
        {
            lastPoint3[it->id1] = root;
            inNeighborhoods3[it->id1] = root;
        }
        else if (length == 2)
        {  
            withLastOne3[it->id1] = root;
            inNeighborhoods3[it->id1] = root;
        }
        else
        {
            withLastSecond3[it->id1] = root;
            inNeighborhoods3[it->id1] = root;
            continue;
        }

        inPath3[it->id1] = true;

        markNeighborhoods3(root, it->id1, length + 1);

        inPath3[it->id1] = false;
    }
}

void markNeighborhoods4(int root, int current,  int length)
{

    for (RE_EDGE* it = Graph2[current].next; it != NULL; it=it->next)
    {

        if (it->id1 < root || it->id1 == root) continue;

        if (inPath4[it->id1]) continue;

        if (length == 1)
        {
            lastPoint4[it->id1] = root;
            inNeighborhoods4[it->id1] = root;
        }
        else if (length == 2)
        {  
            withLastOne4[it->id1] = root;
            inNeighborhoods4[it->id1] = root;
        }
        else
        {
            withLastSecond4[it->id1] = root;
            inNeighborhoods4[it->id1] = root;
            continue;
        }

        inPath4[it->id1] = true;

        markNeighborhoods4(root, it->id1, length + 1);

        inPath4[it->id1] = false;
    }
}

void dfs1(int root, int current,  int depth)
{

    inPath1[current] = true;
    *path_pointer1++ = current;

    if (depth < 7)
    {
        EDGE* it = Graph[current].next;

        //it->money为0，当前邻接点搜索完毕，结束循环。
        for (; it!=NULL; it=it->next)
        {
            if (depth == 1) {
                root_next1 = it; //保存root的下一个节点
            }

            if (it->id2 <= root) continue;

            if (inPath1[it->id2]) continue;

            if (depth > 3 && inNeighborhoods1[it->id2] != root) continue;

            // 判断权重比例是否符合要求
            if (depth >= 2 && depth <= 5) {

                // 判断路径：path1[depth-2]->current_node1->*it
                factor1 = (double)it->money / current_node1->money;
                if (factor1 > 3 || factor1 < 0.2) continue;
            }

            if (depth == 5 && lastPoint1[it->id2] != root && withLastOne1[it->id2] != root) continue;

            if (depth == 6 && lastPoint1[it->id2] != root) continue;

            if (depth >= 2 && lastPoint1[it->id2] == root)
            {
                //寻找it_next1
                it_next1 = Graph[it->id2].next;
                for (; it_next1 != NULL; it_next1=it_next1->next) {
                    if (it_next1->id2 == root) {
                        break;
                    }
                }
                //判断路径:current->it->it_next1
                factor1 = (double)it_next1->money / it->money;
                if (factor1 > 3 || factor1 < 0.2) continue;
                //判断路径:it->it_next1->root_next1
                factor1 = (double)root_next1->money / it_next1->money;
                if (factor1 > 3 || factor1 < 0.2) continue;

                int* ans_ptr = ans1[depth - 2] + (depth + 1) * circleCount1[depth - 2]; // 使用指针操作
                int* path_ptr = path1;
                for (int i = 0; i < depth; i++)
                {
                    *ans_ptr++ = *path_ptr++;
                    circleSize1[depth - 2] += charSize[path1[i]] + 1;// 更新 circleSize1
                }
                circleSize1[depth - 2] += charSize[it->id2] + 1;

                *ans_ptr = it->id2;
                ansCount1++;

                circleCount1[depth - 2]++;

                if (depth == 4 && withLastOne1[it->id2] != root && withLastSecond1[it->id2] != root) continue;

                if (depth == 5 && withLastOne1[it->id2] != root) continue;

                if (depth == 6) continue;
            }
            current_node1 = it;
            dfs1(root, it->id2, depth + 1);
        }
    }
    inPath1[current] = false;
    path_pointer1--;
}
void dfs2(int root, int current, int depth)
{

    inPath2[current] = true;
    *path_pointer2++ = current;

    if (depth < 7)
    {
        EDGE* it = Graph[current].next;

        for (;it != NULL; it=it->next)
        {
            if (depth == 1) {
                root_next2 = it; //保存root的下一个节点
            }

            if (it->id2 <= root) continue;
            if (inPath2[it->id2]) continue;

            if (depth > 3 && inNeighborhoods2[it->id2] != root) continue;

            // 判断权重比例是否符合要求
            if (depth >= 2 && depth <= 5) {

                // 判断路径：path1[depth-2]->current_node2->it
                factor2 = (double)it->money / current_node2->money;
                if (factor2 > 3 || factor2 < 0.2) continue;
            }

            if (depth == 5 && lastPoint2[it->id2] != root && withLastOne2[it->id2] != root) continue;

            if (depth == 6 && lastPoint2[it->id2] != root) continue;

            if (depth >= 2 && lastPoint2[it->id2] == root)
            {
                it_next2 = Graph[it->id2].next;
                for (; it_next2 != NULL; it_next2=it_next2->next) {
                    if (it_next2->id2 == root) {
                        break;
                    }
                }
                //判断路径:current->it->it_next2
                factor2 = (double)it_next2->money / it->money;
                if (factor2 > 3 || factor2 < 0.2) continue;
                //判断路径:it->it_next2->root_next2
                factor2 = (double)root_next2->money / it_next2->money;
                if (factor2 > 3 || factor2 < 0.2) continue;

                int* ans_ptr = ans2[depth - 2] + (depth + 1) * circleCount2[depth - 2]; // 使用指针操作
                int* path_ptr = path2;
                for (int i = 0; i < depth; i++)
                {
                    *ans_ptr++ = *path_ptr++;
                    circleSize2[depth - 2] += charSize[path2[i]] + 1;
                }
                circleSize2[depth - 2] += charSize[it->id2] + 1;
                *ans_ptr = it->id2;

                ansCount2++;

                circleCount2[depth - 2]++;

                if (depth == 4 && withLastOne2[it->id2] != root && withLastSecond2[it->id2] != root) continue;

                if (depth == 5 && withLastOne2[it->id2] != root) continue;

                if (depth == 6) continue;
            }

            current_node2 = it;
            dfs2(root, it->id2, depth + 1);
        }
    }
    inPath2[current] = false;
    path_pointer2--;
}

void dfs3(int root, int current,  int depth)
{

    inPath3[current] = true;
    *path_pointer3++ = current;

    if (depth < 7)
    {

        EDGE* it = Graph[current].next;

        for (;it != NULL; it=it->next)
        {
            if (depth == 1) {
                root_next3 = it; //保存root的下一个节点
            }

            if (it->id2 <= root) continue;
            if (inPath3[it->id2]) continue;

            if (depth > 3 && inNeighborhoods3[it->id2] != root) continue;

            // 判断权重比例是否符合要求
            if (depth >= 2 && depth <= 5) {

                // 判断路径：path3[depth-2]->current_node3->*it

                factor3 = (double)it->money / current_node3->money;
                if (factor3 > 3 || factor3 < 0.2) continue;
            }

            if (depth == 5 && lastPoint3[it->id2] != root && withLastOne3[it->id2] != root) continue;

            if (depth == 6 && lastPoint3[it->id2] != root) continue;

            if (depth >= 2 && lastPoint3[it->id2] == root)
            {

                it_next3 = Graph[it->id2].next;
                for (; it_next3 != NULL; it_next3=it_next3->next) {
                    if (it_next3->id2 == root) {
                        break;
                    }
                }
                //判断路径:current->it->it_next3
                factor3 = (double)it_next3->money / it->money;
                if (factor3 > 3 || factor3 < 0.2) continue;
                //判断路径:it->it_next3->root_next3
                factor3 = (double)root_next3->money / it_next3->money;
                if (factor3 > 3 || factor3 < 0.2) continue;

                int* ans_ptr = ans3[depth - 2] + (depth + 1) * circleCount3[depth - 2]; // 使用指针操作
                int* path_ptr = path3;
                for (int i = 0; i < depth; i++)
                {
                    *ans_ptr++ = *path_ptr++;
                    circleSize3[depth - 2] += charSize[path3[i]] + 1;
                }
                circleSize3[depth - 2] += charSize[it->id2] + 1;
                *ans_ptr = it->id2;

                ansCount3++;

                circleCount3[depth - 2]++;

                if (depth == 4 && withLastOne3[it->id2] != root && withLastSecond3[it->id2] != root) continue;

                if (depth == 5 && withLastOne3[it->id2] != root) continue;

                if (depth == 6) continue;
            }
            current_node3 = it;
            dfs3(root, it->id2, depth + 1);
        }
    }
    inPath3[current] = false;
    path_pointer3--;
}

void dfs4(int root, int current, int depth)
{

    inPath4[current] = true;
    *path_pointer4++ = current;

    if (depth < 7)
    {
        EDGE* it = Graph[current].next;

        for (; it != NULL; it = it->next)
        {
            if (depth == 1) {
                root_next4 = it; //保存root的下一个节点
            }

            if (it->id2 <= root) continue;

            if (inPath4[it->id2]) continue;

            if (depth > 3 && inNeighborhoods4[it->id2] != root) continue;

            // 判断权重比例是否符合要求
            if (depth >= 2 && depth <= 5) {

                // 判断路径：path4[depth-2]->current_node4->it
                factor4 = (double)it->money / current_node4->money;
                if (factor4 > 3 || factor4 < 0.2) continue;
            }

            if (depth == 5 && lastPoint4[it->id2] != root && withLastOne4[it->id2] != root) continue;

            if (depth == 6 && lastPoint4[it->id2] != root) continue;

            if (depth >= 2 && lastPoint4[it->id2] == root)
            {

                it_next4 = Graph[it->id2].next;
                for (; it_next4 != NULL; it_next4=it_next4->next) {
                    if (it_next4->id2 == root) {
                        break;
                    }
                }
                //判断路径:current->it->it_next4
                factor4 = (double)it_next4->money / it->money;
                if (factor4 > 3 || factor4 < 0.2) continue;
                //判断路径:it->it_next4->root_next4
                factor4 = (double)root_next4->money / it_next4->money;
                if (factor4 > 3 || factor4 < 0.2) continue;

                int* ans_ptr = ans4[depth - 2] + (depth + 1) * circleCount4[depth - 2]; // 使用指针操作
                int* path_ptr = path4;
                for (int i = 0; i < depth; i++)
                {
                    *ans_ptr++ = *path_ptr++;
                    circleSize4[depth - 2] += charSize[path4[i]] + 1;
                }
                circleSize4[depth - 2] += charSize[it->id2] + 1;
                *ans_ptr = it->id2;

                ansCount4++;

                circleCount4[depth - 2]++;

                if (depth == 4 && withLastOne4[it->id2] != root && withLastSecond4[it->id2] != root) continue;

                if (depth == 5 && withLastOne4[it->id2] != root) continue;

                if (depth == 6) continue;
            }
            current_node4 = it;
            dfs4(root, it->id2, depth + 1);
        }
    }
    inPath4[current] = false;
    path_pointer4--;
}

/*--------------------- 正向图排序 & 去除其中的无效点 ------------------------*/


void sort1(uint* begin, uint* end){
    while (begin != end){
        int cur_id = idHash[*begin++];
        // 将正向图中合法id前移
		EDGE* id_ptr = &Graph[cur_id];
		while(id_ptr->next!=NULL){
			if(!id_is_recorded[id_ptr->next->id2]){
				EDGE* q=id_ptr->next;
				id_ptr->next=id_ptr->next->next;
				delete q;	
			}else
				id_ptr=id_ptr->next;
		}
    }
}

void sort2(uint* begin, uint* end){
    while (begin != end){
        int cur_id = idHash[*begin++];
        // 将正向图中合法id前移
		EDGE* id_ptr = &Graph[cur_id];
		while(id_ptr->next!=NULL){
			if(!id_is_recorded[id_ptr->next->id2]){
				EDGE* q=id_ptr->next;
				id_ptr->next=id_ptr->next->next;
				delete q;	
			}else
				id_ptr=id_ptr->next;
		}
    }
}

void sort3(uint* begin, uint* end)
{
    while (begin != end){
        int cur_id = idHash[*begin++];
        // 将正向图中合法id前移
		EDGE* id_ptr = &Graph[cur_id];
		while(id_ptr->next!=NULL){
			if(!id_is_recorded[id_ptr->next->id2]){
				EDGE* q=id_ptr->next;
				id_ptr->next=id_ptr->next->next;
				delete q;	
			}else
				id_ptr=id_ptr->next;
		}
    }
}


void sort4(uint*  begin)
{
    while (*begin != 0){
		
        int cur_id = idHash[*begin++];
        // 将正向图中合法id前移
		EDGE* id_ptr = &Graph[cur_id];
		while(id_ptr->next!=NULL){
			if(!id_is_recorded[id_ptr->next->id2]){
				EDGE* q=id_ptr->next;
				id_ptr->next=id_ptr->next->next;
				delete q;	
			}else
				id_ptr=id_ptr->next;
		}
    }
}


/*--------------------- 找环 ---------------------------*/


void search_1(uint* begin, uint* end)
{
    while (begin != end)
    {
        int current = idHash[*begin++];
        markNeighborhoods1(current, current, 1);
        dfs1(current, current, 1);
    }
}

void search_2(uint* begin, uint* end)
{
    while (begin != end)
    {
        int current = idHash[*begin++];
        markNeighborhoods2(current, current, 1);
        dfs2(current, current, 1);
    }
}
void search_3(uint* begin, uint* end)
{
    while (begin != end)
    {
        int current = idHash[*begin++];
        markNeighborhoods3(current, current, 1);
        dfs3(current, current, 1);
    }
}
void search_4(uint* begin)
{
    while (*begin != 0)
    {
        int current = idHash[*begin++];
        markNeighborhoods4(current, current, 1);
        dfs4(current, current, 1);
    }
}




void write_circle34(char* addr34)
{

    //写环ans31
    int* num_ptr = ans31;
    for (int i = 0; i < circleCount1[0]; i++)
    {

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
    //写环ans32
    num_ptr = ans32;
    for (int i = 0; i < circleCount2[0]; i++)
    {

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
    //写环ans33
    num_ptr = ans33;
    for (int i = 0; i < circleCount3[0]; i++)
    {

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
    //写环ans34
    num_ptr = ans34;
    for (int i = 0; i < circleCount4[0]; i++)
    {

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }

    //写环ans41
    num_ptr = ans41;
    for (int i = 0; i < circleCount1[1]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
    //写环ans42
    num_ptr = ans42;
    for (int i = 0; i < circleCount2[1]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
    //写环ans43
    num_ptr = ans43;
    for (int i = 0; i < circleCount3[1]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
    //写环ans44
    num_ptr = ans44;
    for (int i = 0; i < circleCount4[1]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }

}

void write_circle5(char* addr5)
{

    //写环ans51
    int* num_ptr = ans51;
    for (int i = 0; i < circleCount1[2]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
    //写环ans52
    num_ptr = ans52;
    for (int i = 0; i < circleCount2[2]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
    //写环ans53
    num_ptr = ans53;
    for (int i = 0; i < circleCount3[2]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
    //写环ans54
    num_ptr = ans54;
    for (int i = 0; i < circleCount4[2]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
}

void write_circle6(char* addr6)
{
    //写环ans61
    int* num_ptr = ans61;
    for (int i = 0; i < circleCount1[3]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
    //写环ans62
    num_ptr = ans62;
    for (int i = 0; i < circleCount2[3]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
    //写环ans63
    num_ptr = ans63;
    for (int i = 0; i < circleCount3[3]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
    //写环ans64
    num_ptr = ans64;
    for (int i = 0; i < circleCount4[3]; i++)
    {

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }

}

void write_circle7(char* addr7)
{

    //写环ans71
    int* num_ptr = ans71;
    for (int i = 0; i < circleCount1[4]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
    //写环ans72
    num_ptr = ans72;
    for (int i = 0; i < circleCount2[4]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
    //写环ans73
    num_ptr = ans73;
    for (int i = 0; i < circleCount3[4]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
    //写环ans74
    num_ptr = ans74;
    for (int i = 0; i < circleCount4[4]; i++)
    {
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for (int i = charSize[*num_ptr] - 1; i >= 0; i--)
        {
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
}




/*--------------------------------------- main -------------------------------------------*/
int main()
{
    int read_fd = open("/data/test_data.txt", O_RDONLY, 0666);

    struct stat st;
    stat("/data/test_data.txt", &st);
    int read_fileSize = st.st_size;

    char* data = (char*)mmap(NULL, read_fileSize, PROT_READ, MAP_PRIVATE, read_fd, 0);
    uint id1, id2, money;

    while (*data != '\0')
    {
        id1 = 0;
        while (*data != ','){
            id1 *= 10;
            id1 += *data++ - '0';
        }

        ++data;

        id2 = 0;
        while (*data != ','){
            id2 *= 10;
            id2 += *data++ - '0';
        }

        ++data;

        money = 0;
        while (*data != '\r' && *data != '\0')
        {
            money *= 10;
            money += *data++ - '0';
        }
        
		//如果到文末则 mone y为 0
        if (money == 0)
            break;
        
		
		*ids1_ptr++=id1;
		*ids2_ptr++=id2;
		*money_ptr++=money;
        
		*valid_ids_ptr++=id1;
        *valid_ids_ptr++=id2;

        data += 2;
    }
    munmap(data, read_fileSize);  // 关闭读mmap

    //valid_ids排序并去重
    sort(valid_ids, valid_ids_ptr);
	int nodeCount=1;
	uint *valid_ids_end_ptr=valid_ids;
	uint *ptr=valid_ids+1;
    
	while(ptr!=valid_ids_ptr){
		if(*valid_ids_end_ptr==*ptr){
			ptr++;
		}else{
			++valid_ids_end_ptr;
			*valid_ids_end_ptr=*ptr++;
            ++nodeCount;
		}
	}
    valid_ids_ptr=valid_ids_end_ptr+1;

    //id映射
    for (int i = 1;i <= nodeCount;i++) {  
		idHash[valid_ids[i]] = i;
        id_is_recorded[i] = true;
    }

    //建图
	uint* ids1_end=ids1_ptr;
	ids1_ptr=ids1;
	ids2_ptr=ids2;
	money_ptr=transfer_amount;
	
	while(ids1_ptr!=ids1_end){
		
		int hash_id1=idHash[*ids1_ptr];
		int hash_id2=idHash[*ids2_ptr];
		
		inDegrees[hash_id2]++;
		outDegrees[hash_id1]++;
		
		if (Graph[hash_id1].next == NULL) {
		    EDGE *new_node = new EDGE;
		    new_node->id2 = hash_id2;
		    new_node->money = *money_ptr;
		    Graph[hash_id1].next =new_node;
	    }else {
		    EDGE *new_node = new EDGE;
		    new_node->id2 = hash_id2;
		    new_node->money = *money_ptr;
		    if (new_node->id2 < Graph[hash_id1].next->id2) {
			   new_node->next = Graph[hash_id1].next;
			   Graph[hash_id1].next = new_node;
		   }else{
			   EDGE* p = Graph[hash_id1].next;
			   while (p->next != NULL) {
				    if (new_node->id2 > p->next->id2) 
					    p = p->next;
				    else
					    break;
			    } 
			    new_node->next = p->next;
			    p->next = new_node;
		   }
		}
		
		if (Graph2[hash_id2].next == NULL) {
		    RE_EDGE *new_node = new RE_EDGE;
		    new_node->id1=hash_id1;
		    Graph2[hash_id2].next =new_node;
	    }else {
		    RE_EDGE *new_node = new RE_EDGE;
		    new_node->id1=hash_id1;
			new_node->next=Graph2[hash_id2].next;
			Graph2[hash_id2].next=new_node;
		}
		
		ids1_ptr++;
		ids2_ptr++;
        money_ptr++;
		
	}
	
    // 进行出入度为0 的结点的递归去除

    for (auto id_ptr = idHash.begin();id_ptr != idHash.end();id_ptr++) {
        
		int cur_id = id_ptr->second;
        if (id_is_recorded[cur_id] && inDegrees[cur_id] == 0)
        {
            id_is_recorded[cur_id] = false; // 将cur_id标记为无效点
            rm_indegree_eq_0(cur_id); //递归去除入度为0的点
        }
        if (id_is_recorded[cur_id] && outDegrees[cur_id] == 0)
        {
            id_is_recorded[cur_id] = false; // 将cur_id标记为无效点
            rm_oudegree_eq_0(cur_id); //递归去除出度为0的点
        }
    }

    //找到所所有合法id, 将其移到前面
    uint* last_valid_id_ptr = valid_ids;
	uint* id_ptr = valid_ids;
	
	while(id_ptr!=valid_ids_ptr){
		if(id_is_recorded[idHash[*id_ptr]]){
			change(*id_ptr);
            *last_valid_id_ptr++ = *id_ptr;
            ++valid_Num;
		}
		id_ptr++;	
	}
	
    *last_valid_id_ptr = 0;  // 将最后一个合法id的后面一个id标记为-1, 便于search_4的判断

    uint piece = valid_Num / 4; //获取 valid_Num 的均分大小 piece

    uint* seg1 = valid_ids + piece;
    uint* seg2 = seg1 + piece;
    uint* seg3 = seg2 + piece;

    // 删除邻接表中无效节点
    thread r1(sort1, valid_ids, seg1);
    thread r2(sort2, seg1, seg2);
    thread r3(sort3, seg2, seg3);
    thread r4(sort4, seg3);
    r1.join();
    r2.join();
    r3.join();
    r4.join();

    //遍历 valid_ids ，搜索以其中每个节点为根节点的环

    thread s1(search_1, valid_ids, seg1);
    thread s2(search_2, seg1, seg2);
    thread s3(search_3, seg2, seg3);
    thread s4(search_4, seg3);
    s1.join();
    s2.join();
    s3.join();
    s4.join();

    int ansCount = ansCount1 + ansCount2 + ansCount3 + ansCount4;
    //cout<<"ansCount: " << ansCount<<endl;    

    int Circle3_size = circleSize1[0] + circleSize2[0] + circleSize3[0] + circleSize4[0];
    int Circle4_size = circleSize1[1] + circleSize2[1] + circleSize3[1] + circleSize4[1];
    int Circle5_size = circleSize1[2] + circleSize2[2] + circleSize3[2] + circleSize4[2];
    int Circle6_size = circleSize1[3] + circleSize2[3] + circleSize3[3] + circleSize4[3];
    int Circle7_size = circleSize1[4] + circleSize2[4] + circleSize3[4] + circleSize4[4];

    //  所有环总字符数+ansCount字符数 即为 fileSize
    int fileSize = Circle3_size + Circle4_size + Circle5_size + Circle6_size + Circle7_size;
    fileSize += char_num_of_int(ansCount);
    
	//cout<<fileSize<<endl;
    int fd = open("/projects/student/result.txt", O_RDWR | O_CREAT, 0666);

    int u = ftruncate(fd, fileSize);

    char* addr = (char*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //写入环数
    addr += write_with_enter(addr, ansCount);

    // 写入5环的开始地址
    char* addr5 = addr + Circle3_size + Circle4_size;
    char* addr6 = addr5 + Circle5_size;
    char* addr7 = addr6 + Circle6_size;


    //依次写入各环数据
    thread c1(write_circle34, addr);
    thread c2(write_circle5, addr5);
    thread c3(write_circle6, addr6);
    thread c4(write_circle7, addr7);
    c1.join();
    c2.join();
    c3.join();
    c4.join();

    close(fd);
}

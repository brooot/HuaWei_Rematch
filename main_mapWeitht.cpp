#include <bits/stdc++.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

int Graph[300000][50];
int *p[300000];
int Graph2[300000][50];
int *p2[300000];

bool id_is_recorded[300000];  //标记id是否被记录过
int valid_ids[300000]; //记录所有的合法id
int* valid_ids_ptr = valid_ids;



// 所有“出度数，入度数皆大于0，且出度数组中至少包含一个既有出度又有入度的节点”的集合，用于之后的DFS遍历。

int valid_Num=0;  // 记录合法id数量

// 前后路径的转账金额比例上下限
int floor_rate=0.2;
int upper_rate=3;

// 带权有向图
map<int,map<int,int> > weight;

bool inPath1[300000];
bool inPath2[300000];
bool inPath3[300000];
bool inPath4[300000];

int inDegrees[300000];
int outDegrees[300000];

int path1[7];
int path2[7];
int path3[7];
int path4[7];
int *path_pointer1 = path1;
int *path_pointer2 = path2;
int *path_pointer3 = path3;
int *path_pointer4 = path4;

int ansCount1=0;
int ansCount2=0;
int ansCount3=0;
int ansCount4=0;

// circleSize1[0] 表示线程1中长度为3的环构成的总字符数，以此类推
int circleSize1[5]={0,0,0,0,0};
int circleSize2[5]={0,0,0,0,0};
int circleSize3[5]={0,0,0,0,0};
int circleSize4[5]={0,0,0,0,0};

int charSize[300000]; // 记录数字对应的char大小
char char_Int[300000][6]; //记录id对应的char数组

int inNeighborhoods1[300000];
int lastPoint1[300000];
int withLastOne1[300000];
int withLastSecond1[300000];

int inNeighborhoods2[300000];
int lastPoint2[300000];
int withLastOne2[300000];
int withLastSecond2[300000];

int inNeighborhoods3[300000];
int lastPoint3[300000];
int withLastOne3[300000];
int withLastSecond3[300000];

int inNeighborhoods4[300000];
int lastPoint4[300000];
int withLastOne4[300000];
int withLastSecond4[300000];

//int ans3[3*500000];
int ans31[3*500000];
int ans32[3*500000];
int ans33[3*500000];
int ans34[3*500000];
//int ans4[4*500000];
int ans41[4*500000];
int ans42[4*500000];
int ans43[4*500000];
int ans44[4*500000];
//int ans5[5*1000000];
int ans51[5*1000000];
int ans52[5*1000000];
int ans53[5*1000000];
int ans54[5*1000000];
//int ans6[6*2000000];
int ans61[6*2000000];
int ans62[6*2000000];
int ans63[6*2000000];
int ans64[6*2000000];
//int ans7[7*3000000];
int ans71[7*3000000];
int ans72[7*3000000];
int ans73[7*3000000];
int ans74[7*3000000];


int *ans1[]={ans31,ans41,ans51,ans61,ans71};
int *ans2[]={ans32,ans42,ans52,ans62,ans72};
int *ans3[]={ans33,ans43,ans53,ans63,ans73};
int *ans4[]={ans34,ans44,ans54,ans64,ans74};

int circleCount1[5]={0,0,0,0,0};
int circleCount2[5]={0,0,0,0,0};
int circleCount3[5]={0,0,0,0,0};
int circleCount4[5]={0,0,0,0,0};

// 输出指针
char* addr;

#define DEPTH_HIGH_LIMIT 7
#define DEPTH_LOW_LIMIT 3



//去除入度为0的id
void rm_indegree_eq_0(int cur_id){
    for(int* sender_ptr=Graph[cur_id]; *sender_ptr; sender_ptr++){
        int sender_id = *sender_ptr;
        inDegrees[sender_id] = max(inDegrees[sender_id]-1, 0);
        if(id_is_recorded[sender_id] && inDegrees[sender_id]==0){
            id_is_recorded[sender_id] = false;
            rm_indegree_eq_0(sender_id); //递归去除入度为0的点
        }
    }
}


//去除出度为0的id
void rm_oudegree_eq_0(int cur_id){
    for(int* recver_ptr=Graph2[cur_id]; *recver_ptr; recver_ptr++){
        int recver_id = *recver_ptr;
        outDegrees[recver_id] = max(outDegrees[recver_id]-1, 0);
        if(id_is_recorded[recver_id] && outDegrees[recver_id]==0){
            id_is_recorded[recver_id] = false;
            rm_oudegree_eq_0(recver_id); //递归去除入度为0的点
        }
    }
}




void change(int N){
    int size = 0;
    int n = N;
    while(n){
        char_Int[N][size++] = n%10 + '0';
	    n /= 10;
    }
    charSize[N] = size;

}

int char_num_of_int(int n){
    int char_num = 0;
    while(n){
        char_num++;
        n /= 10;
    }
    return char_num+1;
}

//写入带回车的数字
int write_with_enter( char* addr, int n ){
    int char_cache[7],  idx = 0;
    int char_num = 0;
    while(n){
        char_cache[idx++] = n%10 + '0';
	    n /= 10;
    }
    char_num = idx;
    while(idx){
        *addr++ = char_cache[--idx];
    }
    *addr++ = '\n';
    return char_num+1;
}

void markNeighborhoods1(int root,int current,int length){
    int *it;
    it=Graph2[current];
    for(;*it!=0;it++){

        if(*it<root||*it==root) continue;

        if(inPath1[*it]) continue;


        if(length==1){

            lastPoint1[*it]=root;
            inNeighborhoods1[*it]=root;

            /*
            if(root==2547){
                cout<<"root:"<<*it<<lastPoint1[*it]<<endl;
            }*/


        }else if(length==2){

            withLastOne1[*it]=root;
            inNeighborhoods1[*it]=root;

        }else{

            withLastSecond1[*it]=root;
            inNeighborhoods1[*it]=root;
            continue;
        }

        inPath1[*it]=true;

        markNeighborhoods1(root,*it,length+1);

        inPath1[*it]=false;
    }
}

void markNeighborhoods2(int root,int current,int length){
    int *it;
    it=Graph2[current];
    for(;*it!=0;it++){

        if(*it<root||*it==root) continue;

        if(inPath2[*it]) continue;

        if(length==1){
            lastPoint2[*it]=root;
            inNeighborhoods2[*it]=root;

        }else if(length==2){

            withLastOne2[*it]=root;
            inNeighborhoods2[*it]=root;

        }else{

            withLastSecond2[*it]=root;
            inNeighborhoods2[*it]=root;
            continue;
        }

        inPath2[*it]=true;

        markNeighborhoods2(root,*it,length+1);

        inPath2[*it]=false;
    }
}

void markNeighborhoods3(int root,int current,int length){
    int *it;
    it=Graph2[current];
    for(;*it!=0;it++){

        if(*it<root||*it==root) continue;

        if(inPath3[*it]) continue;

        if(length==1){
            lastPoint3[*it]=root;
            inNeighborhoods3[*it]=root;

        }else if(length==2){

            withLastOne3[*it]=root;
            inNeighborhoods3[*it]=root;

        }else{

            withLastSecond3[*it]=root;
            inNeighborhoods3[*it]=root;
            continue;
        }

        inPath3[*it]=true;

        markNeighborhoods3(root,*it,length+1);

        inPath3[*it]=false;
    }
}

void markNeighborhoods4(int root,int current,int length){
    int *it;
    it=Graph2[current];
    for(;*it!=0;it++){

        if(*it<root||*it==root) continue;

        if(inPath4[*it]) continue;

        if(length==1){
            lastPoint4[*it]=root;
            inNeighborhoods4[*it]=root;

        }else if(length==2){

            withLastOne4[*it]=root;
            inNeighborhoods4[*it]=root;

        }else{

            withLastSecond4[*it]=root;
            inNeighborhoods4[*it]=root;
            continue;
        }

        inPath4[*it]=true;

        markNeighborhoods4(root,*it,length+1);

        inPath4[*it]=false;
    }
}

void dfs1(int root,int current,int depth){

    inPath1[current]=true;
    *path_pointer1++ = current;

    if(depth<DEPTH_HIGH_LIMIT){

        int *it=Graph[current];
        for(;*it!=0;it++){


            if(*it<=root) continue;
            if(inPath1[*it]) continue;

            // 判断权重比例是否符合要求
            if(depth>=2&&depth<=6){

                // 两段路径：path1[depth-2]->current->*it
                int lastWeight=weight[path1[depth-2]][current];
                int currentWeight=weight[current][*it];

                double result=(double)currentWeight/lastWeight;

                if(result>3||result<0.2) continue;
            }


            if(depth>3&&inNeighborhoods1[*it]!=root) continue;

            if(depth==5&&lastPoint1[*it]!=root&&withLastOne1[*it]!=root) continue;

            if(depth==6&&lastPoint1[*it]!=root) continue;


            if(depth>=2&&lastPoint1[*it]==root){

                // 两段路径：current->*it->root
                double currentResult=(double)weight[*it][root]/weight[current][*it];

                // 两段路径：*it->root->path1[1]
                double result=(double)weight[root][path1[1]]/weight[*it][root];

                // 如果该环合法，则记录它
                if(currentResult>=0.2&&currentResult<=3&&result>=0.2&&result<=3){

                    int* ans_ptr = ans1[depth-2] + (depth+1)*circleCount1[depth-2]; // 使用指针操作
                    int* path_ptr = path1;
                    for(int i=0;i<depth;i++){
                        *ans_ptr++ = *path_ptr++;
                        circleSize1[depth-2]+=charSize[path1[i]]+1;
                    }
                    circleSize1[depth-2]+=charSize[*it]+1;
                    *ans_ptr = *it;

                    ansCount1++;

                    circleCount1[depth-2]++;

                    // 构成5环但无法继续构成6,7环
                    if(depth==4&&withLastOne1[*it]!=root&&withLastSecond1[*it]!=root) continue;

                    // 构成6环但无法继续构成7环
                    if(depth==5&&withLastOne1[*it]!=root) continue;

                    if(depth==6) continue;

                }
            }


            dfs1(root,*it,depth+1);
        }
    }
    inPath1[current]=false;
    path_pointer1--;
}
void dfs2(int root,int current,int depth){

    inPath2[current]=true;
    *path_pointer2++=current;

    if(depth<DEPTH_HIGH_LIMIT){

        int* it=Graph[current];
        for(int* it=Graph[current]; *it!=0; it++){

            if(*it<=root) continue;
            if(inPath2[*it]) continue;

            // 判断权重比例是否符合要求
            if(depth>=2&&depth<=6){

                // 两段路径：path2[depth-2]->current->*it
                int lastWeight=weight[path2[depth-2]][current];
                int currentWeight=weight[current][*it];

                double result=(double)currentWeight/lastWeight;
                if(result>3||result<0.2) continue;
            }

            if(depth>3&&inNeighborhoods2[*it]!=root) continue;

            if(depth==5&&lastPoint2[*it]!=root&&withLastOne2[*it]!=root) continue;

            if(depth==6&&lastPoint2[*it]!=root) continue;


            if(depth>=2&&lastPoint2[*it]==root){

                // 两段路径：current->*it->root
                double currentResult=(double)weight[*it][root]/weight[current][*it];

                // 两段路径：*it->root->path1[1]
                double result=(double)weight[root][path2[1]]/weight[*it][root];

                // 如果该环合法，则记录它
                if(currentResult>=0.2&&currentResult<=3&&result>=0.2&&result<=3){

                    int* ans_ptr = ans2[depth-2] + (depth+1)*circleCount2[depth-2]; // 使用指针操作
                    int* path_ptr = path2;
                    for(int i=0;i<depth;i++){
                        *ans_ptr++ = *path_ptr++;
                        circleSize2[depth-2]+=charSize[path2[i]]+1;
                    }
                    circleSize2[depth-2]+=charSize[*it]+1;
                    *ans_ptr = *it;

                    ansCount2++;

                    circleCount2[depth-2]++;

                    // 构成5环但无法继续构成6,7环
                    if(depth==4&&withLastOne2[*it]!=root&&withLastSecond2[*it]!=root) continue;

                    // 构成6环但无法继续构成7环
                    if(depth==5&&withLastOne2[*it]!=root) continue;

                    if(depth==6) continue;

                }
            }

            dfs2(root,*it,depth+1);
        }
    }
    inPath2[current]=false;
    path_pointer2--;
}

void dfs3(int root,int current,int depth){

    inPath3[current]=true;
    *path_pointer3++=current;

    if(depth<DEPTH_HIGH_LIMIT){

        for(int* it=Graph[current]; *it!=0; it++){

            if(*it<=root) continue;

            if(inPath3[*it]) continue;

            // 判断权重比例是否符合要求
            if(depth>=2&&depth<=6){

                // 两段路径：path3[depth-2]->current->*it
                int lastWeight=weight[path3[depth-2]][current];
                int currentWeight=weight[current][*it];

                double result=(double)currentWeight/lastWeight;

                if(result>3||result<0.2) continue;
            }

            if(depth>3&&inNeighborhoods3[*it]!=root) continue;

            if(depth==5&&lastPoint3[*it]!=root&&withLastOne3[*it]!=root) continue;

            if(depth==6&&lastPoint3[*it]!=root) continue;


            // 有成环的可能
            if(depth>=2&&lastPoint3[*it]==root){

                // 两段路径：current->*it->root
                double currentResult=(double)weight[*it][root]/weight[current][*it];

                // 两段路径：*it->root->path1[1]
                double result=(double)weight[root][path3[1]]/weight[*it][root];

                // 如果该环合法，则记录它
                if(currentResult>=0.2&&currentResult<=3&&result>=0.2&&result<=3){

                    int* ans_ptr = ans3[depth-2] + (depth+1)*circleCount3[depth-2]; // 使用指针操作
                    int* path_ptr = path3;
                    for(int i=0;i<depth;i++){
                        *ans_ptr++ = *path_ptr++;
                        circleSize3[depth-2]+=charSize[path3[i]]+1;
                    }
                    circleSize3[depth-2]+=charSize[*it]+1;
                    *ans_ptr = *it;

                    ansCount3++;

                    circleCount3[depth-2]++;

                    // 构成5环但无法继续构成6,7环
                    if(depth==4&&withLastOne3[*it]!=root&&withLastSecond3[*it]!=root) continue;

                    // 构成6环但无法继续构成7环
                    if(depth==5&&withLastOne3[*it]!=root) continue;

                    if(depth==6) continue;


                }
            }


            dfs3(root,*it,depth+1);
        }
    }
    inPath3[current]=false;
    path_pointer3--;
}

void dfs4(int root,int current,int depth){

    inPath4[current]=true;
    *path_pointer4++=current;

    if(depth<DEPTH_HIGH_LIMIT){

        for(int* it=Graph[current]; *it!=0; it++){

            if(*it<=root) continue;
            if(inPath4[*it]) continue;

            // 判断权重比例是否符合要求
            if(depth>=2&&depth<=6){

                // 两段路径：path4[depth-2]->current->*it
                int lastWeight=weight[path4[depth-2]][current];
                int currentWeight=weight[current][*it];

                double result=(double)currentWeight/lastWeight;
                if(result>3||result<0.2) continue;
            }

            if(depth>3&&inNeighborhoods4[*it]!=root) continue;


            if(depth==5&&lastPoint4[*it]!=root&&withLastOne4[*it]!=root) continue;

            if(depth==6&&lastPoint4[*it]!=root) continue;

            if(depth>=2&&lastPoint4[*it]==root){

                // 两段路径：current->*it->root
                double currentResult=(double)weight[*it][root]/weight[current][*it];

                // 两段路径：*it->root->path1[1]
                double result=(double)weight[root][path4[1]]/weight[*it][root];

                // 如果该环合法，则记录它
                if(currentResult>=0.2&&currentResult<=3&&result>=0.2&&result<=3){

                    int* ans_ptr = ans4[depth-2] + (depth+1)*circleCount4[depth-2]; // 使用指针操作
                    int* path_ptr = path4;
                    for(int i=0;i<depth;i++){
                        *ans_ptr++ = *path_ptr++;
                        circleSize4[depth-2]+=charSize[path4[i]]+1;
                    }
                    circleSize4[depth-2]+=charSize[*it]+1;
                    *ans_ptr = *it;

                    ansCount4++;

                    circleCount4[depth-2]++;

                    // 构成5环但无法继续构成6,7环
                    if(depth==4&&withLastOne4[*it]!=root&&withLastSecond4[*it]!=root) continue;

                    // 构成6环但无法继续构成7环
                    if(depth==5&&withLastOne4[*it]!=root) continue;

                    if(depth==6) continue;

                }
            }

            dfs4(root,*it,depth+1);
        }
    }
    inPath4[current]=false;
    path_pointer4--;
}

void search_1(int* begin, int* end){
    while(*begin!=*end){


        int current = *begin++;
        markNeighborhoods1(current, current,1);

        dfs1(current,current,1);
    }
}

void search_2(int* begin, int* end){
    while(*begin!=*end){
        int current = *begin++;
        markNeighborhoods2(current, current,1);

        dfs2(current,current,1);
    }
}
void search_3(int* begin, int* end){
    while(*begin!=*end){
        int current = *begin++;
        markNeighborhoods3(current, current,1);

        dfs3(current,current,1);
    }
}
void search_4(int* begin){
    int rec_num = 0;
    while(*begin){
        int current = *begin++;
        markNeighborhoods4(current, current,1);

        dfs4(current,current,1);
    }
}


void write_circle34(char *addr34){
    /*----------------------长度为 3 ---------------------------*/

	//写环ans31
    int* num_ptr = ans31;
    for(int i=0; i<circleCount1[0]; i++){

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans32
	num_ptr = ans32;
	for(int i=0; i<circleCount2[0]; i++){

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans33
	num_ptr = ans33;
	for(int i=0; i<circleCount3[0]; i++){

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans34
	num_ptr = ans34;
	for(int i=0; i<circleCount4[0]; i++){

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }

/*----------------------长度为 4 ---------------------------*/
	//写环ans41
	num_ptr = ans41;
    for(int i=0; i<circleCount1[1]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans42
	num_ptr = ans42;
    for(int i=0; i<circleCount2[1]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans43
	num_ptr = ans43;
    for(int i=0; i<circleCount3[1]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans44
	num_ptr = ans44;
    for(int i=0; i<circleCount4[1]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }

}

void write_circle5(char *addr5){

/*----------------------长度为 5 ---------------------------*/
	//写环ans51
	int* num_ptr = ans51;
    for(int i=0; i<circleCount1[2]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
	//写环ans52
	num_ptr = ans52;
    for(int i=0; i<circleCount2[2]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
	//写环ans53
	num_ptr = ans53;
    for(int i=0; i<circleCount3[2]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
	//写环ans54
	num_ptr = ans54;
    for(int i=0; i<circleCount4[2]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
}

void write_circle6(char *addr6){
    /*----------------------长度为 6 ---------------------------*/
	//写环ans61
	int* num_ptr = ans61;
    for(int i=0; i<circleCount1[3]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
	//写环ans62
	num_ptr = ans62;
    for(int i=0; i<circleCount2[3]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
	//写环ans63
	num_ptr = ans63;
    for(int i=0; i<circleCount3[3]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
	//写环ans64
	num_ptr = ans64;
    for(int i=0; i<circleCount4[3]; i++){

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }

}

void write_circle7(char *addr7){
    /*----------------------长度为 7 ---------------------------*/

	//写环ans71
	int* num_ptr = ans71;
    for(int i=0; i<circleCount1[4]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
	//写环ans72
	num_ptr = ans72;
    for(int i=0; i<circleCount2[4]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
	//写环ans73
	num_ptr = ans73;
    for(int i=0; i<circleCount3[4]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
	//写环ans74
	num_ptr = ans74;
    for(int i=0; i<circleCount4[4]; i++){
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(int i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
}

void sort1(int end){
    for(int i=0; i<end; i++){
        int cur_id=valid_ids[i];
        int* last_valid_id_ptr = Graph[cur_id];
        for(int* id_ptr=Graph[cur_id]; *id_ptr; id_ptr++){
            if(id_is_recorded[*id_ptr]){
                *last_valid_id_ptr++ = *id_ptr;
            }
        }
        *last_valid_id_ptr = 0;
        if(outDegrees[cur_id]>1)
            sort(Graph[cur_id],Graph[cur_id]+outDegrees[cur_id]);
    }
}

void sort23(int begin, int end){
    for(int i=begin; i<end; i++){
        int cur_id=valid_ids[i];
        int* last_valid_id_ptr = Graph[cur_id];
        for(int* id_ptr=Graph[cur_id]; *id_ptr; id_ptr++){
            if(id_is_recorded[*id_ptr]){
                *last_valid_id_ptr++ = *id_ptr;
            }
        }
        *last_valid_id_ptr = 0;
        if(outDegrees[cur_id]>1)
            sort(Graph[cur_id],Graph[cur_id]+outDegrees[cur_id]);
    }
}
void sort4(int begin){
    for(int i=begin; i<valid_Num; i++){
        int cur_id=valid_ids[i];
        int* last_valid_id_ptr = Graph[cur_id];
        for(int* id_ptr=Graph[cur_id]; *id_ptr; id_ptr++){
            if(id_is_recorded[*id_ptr]){
                *last_valid_id_ptr++ = *id_ptr;
            }
        }
        *last_valid_id_ptr = 0;
        if(outDegrees[cur_id]>1)
            sort(Graph[cur_id],Graph[cur_id]+outDegrees[cur_id]);
    }
}

int main()
{


    int read_fd=open("test_data.txt",O_RDONLY, 0666);
    long read_fileSize = lseek(read_fd, 0, SEEK_END);

    char *data = (char *) mmap( NULL,  read_fileSize ,PROT_READ, MAP_PRIVATE, read_fd, 0 );
    int id1, id2;
    int money=0;
    while(*data!='\0'){

        id1 = 0;
        while(*data!=','){
            id1 *= 10;
            id1 += *data++ -'0';
        }

        ++data;

        id2 = 0;
        while(*data!=','){
            id2 *= 10;
            id2 += *data++ -'0';
        }

        //记录新id
        if (!id_is_recorded[id1]){
            id_is_recorded[id1] = true;
            *valid_ids_ptr++ = id1;
        }
        if(!id_is_recorded[id2]){
            id_is_recorded[id2] = true;
            *valid_ids_ptr++ = id2;
        }

        //记录id的出入度
        outDegrees[id1]++;
        inDegrees[id2]++;

        // 记录正向图
        if(p[id1]==NULL){
            p[id1]=Graph[id1]; // 记录对应指针位置
        }
        *p[id1]++ = id2;  // 存储对应接受者

        // 记录反向图
        if(p2[id2]==NULL){
            p2[id2]=Graph2[id2];
        }
        *p2[id2]++ = id1; // 存储发送者

        ++data;

        // 读取转账金额
        money=0;
        while(*data!='\r'){
            money *= 10;
            money += *data-'0';
            data++;
        }

        // 添加对应 id1->id2 对应权重
        weight[id1][id2]=money;

	data += 2;
    }
	munmap(data, read_fileSize);  // 关闭读mmap

    int valid_1 = 0;

    // 进行出入度为0 的结点的递归去除
    for(int* id_ptr=valid_ids; id_ptr!=valid_ids_ptr; id_ptr++){
        valid_1++;
        int cur_id = *id_ptr;
        if(id_is_recorded[cur_id] && inDegrees[cur_id]==0){
            id_is_recorded[cur_id] = false;
            rm_indegree_eq_0(cur_id); //递归去除入度为0的点
        }
        if(id_is_recorded[cur_id] && outDegrees[cur_id]==0){
            id_is_recorded[cur_id] = false;
            rm_oudegree_eq_0(cur_id); //递归去除出度为0的点
        }
    }



    int* last_valid_id_ptr = valid_ids;
    for(int* id_ptr=valid_ids; id_ptr!=valid_ids_ptr; id_ptr++){
        if(id_is_recorded[*id_ptr]){
            change(*id_ptr);
            *last_valid_id_ptr++ = *id_ptr;
            ++valid_Num;
        }
    }


    *last_valid_id_ptr = 0;  // 将最后一个合法id的后面一个id标记为0

    // 对所有合法 id 排序
    sort(valid_ids, last_valid_id_ptr);


    int seg_1 = valid_Num/4;
    int seg_2 = valid_Num/2;
    int seg_3 = seg_1 +seg_2;

    // 邻接表排序
    thread r1(sort1, seg_1);
    thread r2(sort23, seg_1, seg_2);
    thread r3(sort23, seg_2, seg_3);
    thread r4(sort4, seg_3);
    r1.join();
    r2.join();
    r3.join();
    r4.join();


    int p=valid_Num/4;
    int q=valid_Num%4;
    int* seg1 = valid_ids + p;
    int* seg2 = seg1 + p;
    int* seg3 = seg2 + p;


    //遍历 valid_ids ，搜索以其中每个节点为根节点的环

	thread s1(search_1, valid_ids, seg1);
	thread s2(search_2, seg1, seg2);
	thread s3(search_3, seg2, seg3);
	thread s4(search_4, seg3);
	s1.join();
	s2.join();
	s3.join();
	s4.join();

	int ansCount=ansCount1+ansCount2+ansCount3+ansCount4;

	cout<<ansCount<<endl;

    int Circle3_size=circleSize1[0]+circleSize2[0]+circleSize3[0]+circleSize4[0];
    int Circle4_size=circleSize1[1]+circleSize2[1]+circleSize3[1]+circleSize4[1];
    int Circle5_size=circleSize1[2]+circleSize2[2]+circleSize3[2]+circleSize4[2];
    int Circle6_size=circleSize1[3]+circleSize2[3]+circleSize3[3]+circleSize4[3];
    int Circle7_size=circleSize1[4]+circleSize2[4]+circleSize3[4]+circleSize4[4];

    //  所有环总字符数+ansCount字符数 即为 fileSize
    int fileSize=Circle3_size+Circle4_size+Circle5_size+Circle6_size+Circle7_size;
    fileSize += char_num_of_int(ansCount);

	int fd = open("result_t.txt",O_RDWR | O_CREAT, 0666);

    int u=ftruncate(fd,fileSize);

    addr = (char*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //写入环数
    addr += write_with_enter(addr, ansCount);

    // 写入34环的开始地址
    char* addr34=addr;
    // 写入5环的开始地址
    char* addr5=addr+Circle3_size+Circle4_size;
    char* addr6=addr+Circle3_size+Circle4_size+Circle5_size;
    char* addr7=addr+Circle3_size+Circle4_size+Circle5_size+Circle6_size;


	//依次写入各环数据
    thread c1(write_circle34,addr34);
	thread c2(write_circle5,addr5);
	thread c3(write_circle6,addr6);
	thread c4(write_circle7,addr7);
	c1.join();
	c2.join();
	c3.join();
	c4.join();

    close(fd);

}

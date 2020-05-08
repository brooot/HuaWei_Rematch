#include <bits/stdc++.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

#define ui unsigned int

ui Graph[50000][31];
ui *p[50000];
ui Graph2[50000][31];
ui *p2[50000];

bool id_is_recorded[50000];  //标记id是否被记录过
ui valid_ids[50000]; //记录所有的合法id
ui* valid_ids_ptr = valid_ids;

ui valid_Num=0;  // 记录合法id数量

bool inPath1[50000];
bool inPath2[50000];
bool inPath3[50000];
bool inPath4[50000];

ui inDegrees[50000];
ui outDegrees[50000];

ui path1[7];
ui path2[7];
ui path3[7];
ui path4[7];
ui *path_pointer1 = path1;
ui *path_pointer2 = path2;
ui *path_pointer3 = path3;
ui *path_pointer4 = path4;

ui ansCount1=0;
ui ansCount2=0;
ui ansCount3=0;
ui ansCount4=0;

// circleSize1[0] 表示线程1中长度为3的环构成的总字符数，以此类推
ui circleSize1[5]={0,0,0,0,0};
ui circleSize2[5]={0,0,0,0,0};
ui circleSize3[5]={0,0,0,0,0};
ui circleSize4[5]={0,0,0,0,0};

ui charSize[50000]; // 记录数字对应的char大小
char char_Int[50000][6]; //记录id对应的char数组

ui inNeighborhoods1[50000];
ui lastPoint1[50000];
ui withLastOne1[50000];
ui withLastSecond1[50000];

ui inNeighborhoods2[50000];
ui lastPoint2[50000];
ui withLastOne2[50000];
ui withLastSecond2[50000];

ui inNeighborhoods3[50000];
ui lastPoint3[50000];
ui withLastOne3[50000];
ui withLastSecond3[50000];

ui inNeighborhoods4[50000];
ui lastPoint4[50000];
ui withLastOne4[50000];
ui withLastSecond4[50000];

//ui ans3[3*500000];
ui ans31[1500000];
ui ans32[1500000];
ui ans33[1500000];
ui ans34[1500000];
//ui ans4[4*500000];
ui ans41[2000000];
ui ans42[2000000];
ui ans43[2000000];
ui ans44[2000000];
//ui ans5[5*1000000];
ui ans51[5000000];
ui ans52[5000000];
ui ans53[5000000];
ui ans54[5000000];
//ui ans6[6*2000000];
ui ans61[12000000];
ui ans62[12000000];
ui ans63[12000000];
ui ans64[12000000];
//ui ans7[7*3000000];
ui ans71[21000000];
ui ans72[21000000];
ui ans73[21000000];
ui ans74[21000000];


ui *ans1[]={ans31,ans41,ans51,ans61,ans71};
ui *ans2[]={ans32,ans42,ans52,ans62,ans72};
ui *ans3[]={ans33,ans43,ans53,ans63,ans73};
ui *ans4[]={ans34,ans44,ans54,ans64,ans74};

ui circleCount1[5]={0,0,0,0,0};
ui circleCount2[5]={0,0,0,0,0};
ui circleCount3[5]={0,0,0,0,0};
ui circleCount4[5]={0,0,0,0,0};

struct node
{
    un
};



//去除入度为0的id
void rm_indegree_eq_0(ui cur_id){
    for(ui* recver_ptr=Graph[cur_id]; *recver_ptr; recver_ptr++){
        ui recver_id = *recver_ptr;
        inDegrees[recver_id] = max(inDegrees[recver_id]-1, 0);
        if(id_is_recorded[recver_id] && inDegrees[recver_id]==0){ //若cur_id是有效的但是入度为0
            id_is_recorded[recver_id] = false; // 将cur_id标记为无效点
            rm_indegree_eq_0(recver_id); //递归去除入度为0的点
        }
    }
}


//去除出度为0的id
void rm_oudegree_eq_0(ui cur_id){
    for(ui* sender_ptr=Graph2[cur_id]; *sender_ptr; sender_ptr++){
        ui sender_id = *sender_ptr;
        outDegrees[sender_id] = max(outDegrees[sender_id]-1, 0);
        if(id_is_recorded[sender_id] && outDegrees[sender_id]==0){ //若cur_id是无效的但出度为0
            id_is_recorded[sender_id] = false; // 将cur_id标记为无效点
            rm_oudegree_eq_0(sender_id); //递归去除入度为0的点
        }
    }
}


void change(ui N){
    ui size = 0;
    ui n = N;
    while(n){
        char_Int[N][size++] = n%10 + '0';
	    n /= 10;
    }
    charSize[N] = size;

}

ui char_num_of_int(ui n){
    ui char_num = 0;
    while(n){
        char_num++;
        n /= 10;
    }
    return char_num+1;
}

//写入带回车的数字
ui write_with_enter( char* addr, ui n ){
    ui char_cache[7],  idx = 0;
    ui char_num = 0;
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

void markNeighborhoods1(ui root,ui current,ui length){
    
    for(ui* it=Graph2[current]; *it; it++){

        if(*it<root||*it==root) continue;

        if(inPath1[*it]) continue;

        if(length==1){
            lastPoint1[*it]=root;
            inNeighborhoods1[*it]=root;

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

void markNeighborhoods2(ui root,ui current,ui length){

    for(ui* it=Graph2[current]; *it; it++){

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

void markNeighborhoods3(ui root,ui current,ui length){
    
    for(ui* it=Graph2[current]; *it; it++){

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

void markNeighborhoods4(ui root,ui current,ui length){

       for(ui* it=Graph2[current]; *it; it++){

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

void dfs1(ui root,ui current,ui depth){

    inPath1[current]=true;
    *path_pointer1++ = current;

    if(depth<7){
        
        for(ui *it=Graph[current]; *it; it++){

            if(*it<=root) continue;
            if(inPath1[*it]) continue;

            if(depth>3&&inNeighborhoods1[*it]!=root) continue;

            if(depth==5&&lastPoint1[*it]!=root&&withLastOne1[*it]!=root) continue;

            if(depth==6&&lastPoint1[*it]!=root) continue;

            if(depth>=2&&lastPoint1[*it]==root){

                ui* ans_ptr = ans1[depth-2] + (depth+1)*circleCount1[depth-2]; // 使用指针操作
                ui* path_ptr = path1;
                for(ui i=0;i<depth;i++){
                    *ans_ptr++ = *path_ptr++;
		            circleSize1[depth-2]+=charSize[path1[i]]+1;// 更新 circleSize1
                }
                circleSize1[depth-2]+=charSize[*it]+1;

	            *ans_ptr = *it;
                ansCount1++;

                circleCount1[depth-2]++;

                if(depth==4&&withLastOne1[*it]!=root&&withLastSecond1[*it]!=root) continue;

                if(depth==5&&withLastOne1[*it]!=root) continue;

                if(depth==6) continue;
            }

            dfs1(root,*it,depth+1);
        }
    }
    inPath1[current]=false;
    path_pointer1--;
}

void dfs2(ui root,ui current,ui depth){

    inPath2[current]=true;
    *path_pointer2++=current;

    if(depth<7){

        for(ui* it=Graph[current]; *it; it++){

            if(*it<=root) continue;
            if(inPath2[*it]) continue;

            if(depth>3&&inNeighborhoods2[*it]!=root) continue;

            if(depth==5&&lastPoint2[*it]!=root&&withLastOne2[*it]!=root) continue;

            if(depth==6&&lastPoint2[*it]!=root) continue;

            if(depth>=2&&lastPoint2[*it]==root){

                ui* ans_ptr = ans2[depth-2] + (depth+1)*circleCount2[depth-2]; // 使用指针操作
                ui* path_ptr = path2;
                for(ui i=0;i<depth;i++){
                    *ans_ptr++ = *path_ptr++;
		            circleSize2[depth-2]+=charSize[path2[i]]+1;
                }
                circleSize2[depth-2]+=charSize[*it]+1;
		        *ans_ptr = *it;

                ansCount2++;

                circleCount2[depth-2]++;

                if(depth==4&&withLastOne2[*it]!=root&&withLastSecond2[*it]!=root) continue;

                if(depth==5&&withLastOne2[*it]!=root) continue;

                if(depth==6) continue;
            }

            dfs2(root,*it,depth+1);
        }
    }
    inPath2[current]=false;
    path_pointer2--;
}

void dfs3(ui root,ui current,ui depth){

    inPath3[current]=true;
    *path_pointer3++=current;

    if(depth<7){

        for(ui* it=Graph[current]; *it; it++){

            if(*it<=root) continue;
            if(inPath3[*it]) continue;

            if(depth>3&&inNeighborhoods3[*it]!=root) continue;

            if(depth==5&&lastPoint3[*it]!=root&&withLastOne3[*it]!=root) continue;

            if(depth==6&&lastPoint3[*it]!=root) continue;

            if(depth>=2&&lastPoint3[*it]==root){

                ui* ans_ptr = ans3[depth-2] + (depth+1)*circleCount3[depth-2]; // 使用指针操作
                ui* path_ptr = path3;
                for(ui i=0;i<depth;i++){
                    *ans_ptr++ = *path_ptr++;
					circleSize3[depth-2]+=charSize[path3[i]]+1;
                }
                circleSize3[depth-2]+=charSize[*it]+1;
				*ans_ptr = *it;

                ansCount3++;

                circleCount3[depth-2]++;

                if(depth==4&&withLastOne3[*it]!=root&&withLastSecond3[*it]!=root) continue;

                if(depth==5&&withLastOne3[*it]!=root) continue;

                if(depth==6) continue;
            }

            dfs3(root,*it,depth+1);
        }
    }
    inPath3[current]=false;
    path_pointer3--;
}

void dfs4(ui root,ui current,ui depth){

    inPath4[current]=true;
    *path_pointer4++=current;

    if(depth<7){

        for(ui* it=Graph[current]; *it; it++){

            if(*it<=root) continue;
            if(inPath4[*it]) continue;

            if(depth>3&&inNeighborhoods4[*it]!=root) continue;

            if(depth==5&&lastPoint4[*it]!=root&&withLastOne4[*it]!=root) continue;

            if(depth==6&&lastPoint4[*it]!=root) continue;

            if(depth>=2&&lastPoint4[*it]==root){

                ui* ans_ptr = ans4[depth-2] + (depth+1)*circleCount4[depth-2]; // 使用指针操作
                ui* path_ptr = path4;
                for(ui i=0;i<depth;i++){
                    *ans_ptr++ = *path_ptr++;
                    circleSize4[depth-2]+=charSize[path4[i]]+1;
                }
                circleSize4[depth-2]+=charSize[*it]+1;
                *ans_ptr = *it;

                ansCount4++;

                circleCount4[depth-2]++;

                if(depth==4&&withLastOne4[*it]!=root&&withLastSecond4[*it]!=root) continue;

                if(depth==5&&withLastOne4[*it]!=root) continue;

                if(depth==6) continue;
            }

            dfs4(root,*it,depth+1);
        }
    }
    inPath4[current]=false;
    path_pointer4--;
}

/*--------------------- 正向图排序 & 去除其中的无效点 ------------------------*/
void sort1(ui* begin, ui* end){
    while(*begin!=*end){
        // 将正向图中合法id前移
        ui* last_valid_id_ptr = Graph[*begin];
        for(ui* id_ptr=Graph[*begin]; *id_ptr; id_ptr++)
            if(id_is_recorded[*id_ptr])
                *last_valid_id_ptr++ = *id_ptr;

        *last_valid_id_ptr = 0; //最后一个合法id后一位置为0

        sort(Graph[*begin++], last_valid_id_ptr);
    }
}

void sort23(ui* begin, ui* end){
    while(*begin!=*end){
        // 将正向图中合法id前移
        ui* last_valid_id_ptr = Graph[*begin];
        for(ui* id_ptr=Graph[*begin]; *id_ptr; id_ptr++)
            if(id_is_recorded[*id_ptr])
                *last_valid_id_ptr++ = *id_ptr;

        *last_valid_id_ptr = 0; //最后一个合法id后一位置为0

        sort(Graph[*begin++], last_valid_id_ptr);
    }
}


void sort4(ui* begin){
    while(*begin){
        ui* last_valid_id_ptr = Graph[*begin];
        for(ui* id_ptr=Graph[*begin]; *id_ptr; id_ptr++)
            if(id_is_recorded[*id_ptr])
                *last_valid_id_ptr++ = *id_ptr;

        *last_valid_id_ptr = 0; //最后一个合法id后一位置为0

        sort(Graph[*begin++], last_valid_id_ptr);
    }
}


/*--------------------- 找环 ---------------------------*/


void search_1(ui* begin, ui* end){
    while(*begin!=*end){
        ui current = *begin++;
        markNeighborhoods1(current, current,1);
        dfs1(current,current,1);
    }
}

void search_2(ui* begin, ui* end){
    while(*begin!=*end){
        ui current = *begin++;
        markNeighborhoods2(current, current,1);
        dfs2(current,current,1);
    }
}
void search_3(ui* begin, ui* end){
    while(*begin!=*end){
        ui current = *begin++;
        markNeighborhoods3(current, current,1);
        dfs3(current,current,1);
    }
}
void search_4(ui* begin){
    while(*begin){
        ui current = *begin++;
        markNeighborhoods4(current, current,1);
        dfs4(current,current,1);
    }
}




void write_circle34(char *addr34){

	//写环ans31
    ui* num_ptr = ans31;
    for(ui i=0; i<circleCount1[0]; i++){

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans32
	num_ptr = ans32;
	for(ui i=0; i<circleCount2[0]; i++){

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans33
	num_ptr = ans33;
	for(ui i=0; i<circleCount3[0]; i++){

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans34
	num_ptr = ans34;
	for(ui i=0; i<circleCount4[0]; i++){

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }

	//写环ans41
	num_ptr = ans41;
    for(ui i=0; i<circleCount1[1]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans42
	num_ptr = ans42;
    for(ui i=0; i<circleCount2[1]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans43
	num_ptr = ans43;
    for(ui i=0; i<circleCount3[1]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }
	//写环ans44
	num_ptr = ans44;
    for(ui i=0; i<circleCount4[1]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr34++ = char_Int[*num_ptr][i];
        }
        *addr34++ = '\n';
        num_ptr++;
    }

}

void write_circle5(char *addr5){

	//写环ans51
	ui* num_ptr = ans51;
    for(ui i=0; i<circleCount1[2]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
	//写环ans52
	num_ptr = ans52;
    for(ui i=0; i<circleCount2[2]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
	//写环ans53
	num_ptr = ans53;
    for(ui i=0; i<circleCount3[2]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
	//写环ans54
	num_ptr = ans54;
    for(ui i=0; i<circleCount4[2]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr5++ = char_Int[*num_ptr][i];
        }
        *addr5++ = '\n';
        num_ptr++;
    }
}

void write_circle6(char *addr6){
	//写环ans61
	ui* num_ptr = ans61;
    for(ui i=0; i<circleCount1[3]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
	//写环ans62
	num_ptr = ans62;
    for(ui i=0; i<circleCount2[3]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
	//写环ans63
	num_ptr = ans63;
    for(ui i=0; i<circleCount3[3]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }
	//写环ans64
	num_ptr = ans64;
    for(ui i=0; i<circleCount4[3]; i++){

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr6++ = char_Int[*num_ptr][i];
        }
        *addr6++ = '\n';
        num_ptr++;
    }

}

void write_circle7(char *addr7){

	//写环ans71
	ui* num_ptr = ans71;
    for(ui i=0; i<circleCount1[4]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
	//写环ans72
	num_ptr = ans72;
    for(ui i=0; i<circleCount2[4]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
	//写环ans73
	num_ptr = ans73;
    for(ui i=0; i<circleCount3[4]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
	//写环ans74
	num_ptr = ans74;
    for(ui i=0; i<circleCount4[4]; i++){
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;
        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = ',';
        num_ptr++;

        for(ui i=charSize[*num_ptr]-1; i>=0; i--){
            *addr7++ = char_Int[*num_ptr][i];
        }
        *addr7++ = '\n';
        num_ptr++;
    }
}




/*--------------------------------------- main -------------------------------------------*/
ui main()
{
    clock_t start_time = clock();
    ui read_fd=open("test_data_new.txt",O_RDONLY, 0666);
    long read_fileSize = lseek(read_fd, 0, SEEK_END);

    char *data = (char *) mmap( NULL,  read_fileSize ,PROT_READ, MAP_PRIVATE, read_fd, 0 );
    ui id1, id2;
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

        if(id1<=50000&&id2<=50000){
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
        }
        while(*data++!='\n'){}
    }
	munmap(data, read_fileSize);  // 关闭读mmap
  

    // 进行出入度为0 的结点的递归去除
    for(ui* id_ptr=valid_ids; id_ptr!=valid_ids_ptr; id_ptr++){
        ui cur_id = *id_ptr;
        if(id_is_recorded[cur_id] && inDegrees[cur_id]==0){
            id_is_recorded[cur_id] = false; // 将cur_id标记为无效点
            rm_indegree_eq_0(cur_id); //递归去除入度为0的点
        }
        if(id_is_recorded[cur_id] && outDegrees[cur_id]==0){
            id_is_recorded[cur_id] = false; // 将cur_id标记为无效点
            rm_oudegree_eq_0(cur_id); //递归去除出度为0的点
        }
    }

    //找到所所有合法id, 将其移到前面
    ui* last_valid_id_ptr = valid_ids;
    for(ui* id_ptr=valid_ids; id_ptr!=valid_ids_ptr; id_ptr++){
        if(id_is_recorded[*id_ptr]){
            change(*id_ptr);
            *last_valid_id_ptr++ = *id_ptr;
            ++valid_Num;
        }
    }
    *last_valid_id_ptr = 0;  // 将最后一个合法id的后面一个id标记为0, 便于search_4的判断


    // 对所有合法 id 排序
    sort(valid_ids, last_valid_id_ptr);

    ui piece = valid_Num/4; //获取 valid_Num 的均分大小 piece


    ui* seg1 = valid_ids + piece;
    ui* seg2 = seg1 + piece;
    ui* seg3 = seg2 + piece;



    // 邻接表排序
    thread r1(sort1, valid_ids, seg1);
    thread r2(sort23, seg1, seg2);
    thread r3(sort23, seg2, seg3);
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
	ui ansCount=ansCount1+ansCount2+ansCount3+ansCount4;

    ui Circle3_size=circleSize1[0]+circleSize2[0]+circleSize3[0]+circleSize4[0];
    ui Circle4_size=circleSize1[1]+circleSize2[1]+circleSize3[1]+circleSize4[1];
    ui Circle5_size=circleSize1[2]+circleSize2[2]+circleSize3[2]+circleSize4[2];
    ui Circle6_size=circleSize1[3]+circleSize2[3]+circleSize3[3]+circleSize4[3];
    ui Circle7_size=circleSize1[4]+circleSize2[4]+circleSize3[4]+circleSize4[4];

    //  所有环总字符数+ansCount字符数 即为 fileSize
    ui fileSize=Circle3_size+Circle4_size+Circle5_size+Circle6_size+Circle7_size;
    fileSize += char_num_of_int(ansCount);

	ui fd = open("result.txt",O_RDWR | O_CREAT, 0666);

    ui u=ftruncate(fd,fileSize);

    char* addr = (char*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //写入环数
    addr += write_with_enter(addr, ansCount);

    // 写入5环的开始地址
    char* addr5=addr+Circle3_size+Circle4_size;
    char* addr6=addr5+Circle5_size;
    char* addr7=addr6+Circle6_size;


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
    clock_t end = clock();
    cout << "total: " << double(end-start_time)/CLOCKS_PER_SEC << " s" << endl;
}

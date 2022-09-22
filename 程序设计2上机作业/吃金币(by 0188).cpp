#include<stdio.h>
int n=0;
int a[50][50];
int max1[99][5000];
int max(int b,int c,int d,int e){
    if (c>b)
        b=c;
    if (d>b)
        b=d;
    if (e>b)
        b=e;
    return b;
}
void search(){
    int number=0;
    int u,v;
    for (u=0;u<2*n-1;u++){
    for (v=0;v<=(n+1)*51;v++){
            max1[u][v]=-n*n;
        }
    }
    if (a[0][0]!=-1){
        max1[0][51]=a[0][0];
    }
    else{
        return;
    }
    for (number=1;number<2*n-1;number++){
        for (u=0;u<=number,u<=n-1;u++){
            for (v=0;v<=number,v<=n-1;v++){
                if (a[u][number-u]!=-1&&a[v][number-v]!=-1){
                    if (u == v)
                        max1[number][(u+1)*50+v+1]=max(max1[number-1][u*50+v],max1[number-1][u*50+v+1]
                        ,max1[number-1][(u+1)*50+v],max1[number-1][(u+1)*50+v+1])+a[u][number-u];
                    if (u != v)
                        max1[number][(u+1)*50+v+1]= max(max1[number-1][u*50+v],max1[number-1][u*50+v+1]
                        ,max1[number-1][(u+1)*50+v],max1[number-1][(u+1)*50+v+1])+a[u][number-u]+a[v][number-v];
                }
                else{
                }
            }
        }
    }
}
int main(){
	//	freopen("in.dat","r",stdin);
	//freopen("out_rjx.dat","w",stdout);
    char character='1';
    int i,j;
    //initialize the character
    while (character!='\n'){
        scanf("%d",&a[0][n]);
        n ++;
        character=getchar();
    }
    //row index
    if (n == 1){
        if (a[0][0] == 1){
            printf("1\n");
        }
        else{
            printf("0\n");
        }
        return 0;
    }
    //deal with some special situation
    for (i = 0;i < n-1;i ++){
        for (j = 0;j < n;j ++){
            scanf("%d",&a[i+1][j]);
        }
    }
    getchar();
    search();
    if (max1[2*n-2][n*51] < 0){
        printf("0\n");
        return 0;
    }
    printf("%d\n",max1[2*n-2][n*51]);
}

#include<stdio.h>
int input[250][250];
int count = 0;//对吃掉的金币进行计数
int max = 0;//最大值
int line;//输入行数
int flag = 0;//标记是否已经到达过右下
void road( int n, int m );

int main( void )
{
    int i = 0;
    int j = 0;
    char ch = ' ';
    for( j = 0; j < 250; j++ )
    {
        for( i = 0; i < 250; i++ )
            input[i][j] = 2;
    }
    for( i = 0, j = 0; ch == ' '; i++ )
    {
        scanf( "%d", &input[j][i] );
        ch = getchar();
    }
    line = i;
    for( j = 1; j < line; j++ )
    {
        for( i = 0; i < line; i++ )
            scanf( "%d", &input[j][i] );
    }
    road( 0, 0 );
    printf( "%d", max );
    return 0; 
}

void road( int n, int m)
{
    int temp;
    if( (n == line - 1) && (m == line - 1) )
    {
        if( flag == 0 )
        { 
            flag = 1;
            count += input[n][m];
            road( 0, 0 );
        }
        else//閫掑綊鍑哄彛
        {
            if( max < count )
                max = count;
            return;
        }
    }
    if( (input[n][m] == 2) || (input[n][m] == -1) )
        return;
    count += input[n][m];
    temp = input[n][m];
    input[n][m] = 0;
    road( n, m + 1 );
    road( n + 1, m );
    input[n][m] = temp;
    count -= temp;
}

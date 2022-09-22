#include<iostream>
#define WEST 1
#define NORTH 2
#define EAST 4
#define SOUTH 8
using namespace std;
int max_connected(int vis[50][50],int puzzle[50][50],int x,int y);
int max_area(int puzzle[50][50],int* block_cnt=NULL);
int rows,cols;
int puzzle[50][50];
int main()
{
	cin>>cols>>rows;
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			cin>>puzzle[i][j];
		}
	}
	int block_cnt,max_S;
	max_S=max_area(puzzle,&block_cnt);
	cout<<block_cnt<<endl<<max_S<<endl;
	max_S=0;
	int tmp_S,ans_row,ans_col,ans_dir;
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			for(int dir=WEST;dir<=SOUTH;dir=dir<<1) //对每一个方向搜索 
			{
				if(puzzle[i][j]&dir) //如果这里有墙，考虑拆墙，否则不浪费时间 
				{
					if(dir==NORTH&&i==0||dir==SOUTH&&i==rows-1||dir==WEST&&j==0||dir==EAST&&j==cols-1) continue; //不能拆掉边界 
					puzzle[i][j]^=dir;
					if(dir==WEST) puzzle[i][j-1]^=EAST; //注意拆一堵墙需要改变两个方块的值 
					else if(dir==EAST) puzzle[i][j+1]^=WEST;
					else if(dir==SOUTH) puzzle[i+1][j]^=NORTH;
					else if(dir==NORTH) puzzle[i-1][j]^=SOUTH;
                    int vis[50][50]={0};
					tmp_S=max_connected(vis,puzzle,i,j); //计算拆完后本方块所在连通块的面积 
					if(tmp_S>max_S||tmp_S==max_S&&(ans_col>j||ans_col==j&&ans_row<i))
					{
						max_S=tmp_S;
						ans_row=i;
						ans_col=j;
						ans_dir=dir;
					} 
					if(ans_dir==WEST) //注意答案里方向没有西和南 
					{
						ans_col--;
						ans_dir=EAST;
					}
					if(ans_dir==SOUTH)
					{
						ans_row++;
						ans_dir=NORTH;
					}
					if(tmp_S==max_S&&dir==NORTH&&ans_col==j&&ans_row==i) //同一小方块北方优先 
					{
						ans_row=i;
						ans_col=j;
						ans_dir=dir;
					}
					puzzle[i][j]|=dir;
					if(dir==WEST) puzzle[i][j-1]|=EAST;
					else if(dir==EAST) puzzle[i][j+1]|=WEST;
					else if(dir==SOUTH) puzzle[i+1][j]|=NORTH;
					else if(dir==NORTH) puzzle[i-1][j]|=SOUTH;
				}
			}
			
		}
	}
	cout<<max_S<<endl<<ans_row+1<<" "<<ans_col+1<<" ";
	if(ans_dir&WEST) cout<<"W"<<endl<<endl;
	else if(ans_dir&NORTH) cout<<"N"<<endl;
	else if(ans_dir&EAST) cout<<"E"<<endl;
	else cout<<"S"<<endl;
	return 0;
}
int max_connected(int vis[50][50],int puzzle[50][50],int x,int y)
{
	int ans=1;
	vis[x][y]=1;
	//对某个方向进行扩展 
	if((puzzle[x][y]&NORTH)==0&&vis[x-1][y]==0)
	{
		vis[x-1][y]=1;
		ans+=max_connected(vis,puzzle,x-1,y);
	}
	if((puzzle[x][y]&WEST)==0&&vis[x][y-1]==0)
	{
		vis[x][y-1]=1;
		ans+=max_connected(vis,puzzle,x,y-1);
	}
	if((puzzle[x][y]&EAST)==0&&vis[x][y+1]==0)
	{
		vis[x][y+1]=1;
		ans+=max_connected(vis,puzzle,x,y+1);
	}
	if((puzzle[x][y]&SOUTH)==0&&vis[x+1][y]==0)
	{
		vis[x+1][y]=1;
		ans+=max_connected(vis,puzzle,x+1,y);
	}
	return ans;
}
int max_area(int puzzle[50][50],int* block_cnt)
{
	int cnt=0;
	int ans=0;
	int vis[50][50]={0};
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			if(!vis[i][j])
			{
				cnt++;
				ans=max(ans,max_connected(vis,puzzle,i,j));
			}
		}
	}
	if(block_cnt!=NULL) *block_cnt=cnt;
	return ans;
}

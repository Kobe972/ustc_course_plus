#include <iostream>
#include <queue>
using namespace std;
int a[2000005]; 
int main() {
    int m,n;
    cin>>n>>m;
    for(int i=0;i<n;i++) cin>>a[i];
    // 实现单调队列
    deque<int> dq;

    for(int i = 0; i < n; i++) {
	if(i>=1) while(!dq.empty() && dq.back() > a[i-1]) {
            dq.pop_back();
        }
        // 当前元素入队
        if(i>=1) dq.push_back(a[i-1]);
        if(i >= m) {
            cout << dq.front() << " "<<endl;
            if(a[i - m] == dq.front()) {
                dq.pop_front();
            }
        }
        else  if(i==0) cout<<"0"<<endl;
        else cout << dq.front() << " "<<endl;
    }
    return 0;
}

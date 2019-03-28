#include <iostream>
#include <vector>
using namespace std;

void sortable(vector<int> &vec){
	int i=1,j;
	for(;i<vec.size();++i){
		if(vec[i-1]>vec[i]){
			--i;
			break;
		}
	}
	if(i==vec.size()){
		cout<<"yes"<<endl<<1<<" "<<1;
		return;
	}
	cout<<i<<endl;
	j=i;
	for(;i<vec.size()-1;++i){
		if(vec[i]<vec[i+1])
			break;
	}
	if(i==vec.size()-1){
		if(vec[i]>vec[j-1])
			cout<<"yes"<<endl<<j+1<<" "<<i+1;
		else
			cout<<"no";
		return;
	}
	for(;i<vec.size()-1;++i){
		if(vec[i]>vec[i+1]){
			cout<<"no";
			return;
		}
	}
	if(vec[i])
	cout<<i<<" "<<j;
}

int main(int argc, char *argv[]) {
	int n;
	cin>>n;
	vector<int> arey(n);
	for(int i=0;i<n;++i){
		cin>>arey[i];
	}
	sortable(arey);
	return 0;
}


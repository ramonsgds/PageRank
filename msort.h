#include <vector>
#include <cmath>
using namespace std;

template<class T, class Comparator>
void mergeSort(vector<T>& myArray, Comparator& comp){

	if ( myArray.size() < 2){
		return;
	}
	vector<T> lhs;
	vector<T> rhs;
	int m = (int)floor(myArray.size()/2);
	for ( int i =0; i < m; i++){
		lhs.push_back(myArray[i]);
	}
	for ( int j = m; j < myArray.size(); j++){
		rhs.push_back(myArray[j]);
	}
	mergeSort(lhs, comp);
	mergeSort(rhs, comp);
	merge(myArray,lhs, rhs, comp);	
	
}

template<class T, class Comparator>
void merge(vector<T>& myArray, vector<T>& lhs, vector<T>& rhs, Comparator& comp){
	int k = 0;
	int i = 0;
	int j = 0;
	while( i < lhs.size() && j < rhs.size() ){
	    if ( comp(lhs[i],rhs[j])){
	    	myArray[k] = lhs[i];
	    	k++;
	    	i++;
	    }
	    else{
	    	myArray[k] = rhs[j];
	    	j++;
	    	k++;
	    }
	}
	while( i < lhs.size()){
		myArray[k] = lhs[i];
		i++;
		k++;
	}
	while(j < rhs.size()){
		myArray[k] = rhs[j];
		j++;
		k++;
	}
}

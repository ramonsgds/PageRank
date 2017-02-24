#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

void parse(string filename, set<string>& myset, vector<string>& myvectorfiles);

int main(int argc, char* argv[]){

	if(argc < 2){
      cerr << "usage: ./search index_file...Please provide an index file" << endl;
      return 1;
    }

    bool indexit = false;
    bool outputit = false;
    string configstr = "";
    ifstream ifile(argv[1]);
	string 	indfile = "";
	string outfile = "";
    while(true){
  	    getline(ifile,configstr);
  	    if (ifile.fail()){
  	    	break;
  	    }
  	    if (configstr == ""){
  	    	//EMPTY LINE
  	    }
  	    else if (configstr[0] == '#'){
            //IGNORE COMMENTS, WHICH START BY "#"
            cout << "COMMENT" << endl;
  	    }
  	    else if (configstr.substr(0,10) == "INDEX_FILE" && indexit == false){
  	    	
  	    	if (configstr.find('#') <= configstr.length()){
  	    		int hashit = configstr.find('#');
  	    		configstr.erase(hashit);
  	    	}	
  	    	int execposition = configstr.find("./");
  	    	indexit = true;
  	    	string indextext = configstr.substr(execposition+2);
  	    	if (indextext.find(' ') <= indextext.length()){
  	    		indextext.erase(indextext.find(' '));
  	    	}
  	    	indfile = indextext;	
  	    }
  	    else if (configstr.substr(0,11) == "OUTPUT_FILE" && outputit == false ){
  	    	if (configstr.find('#') <= configstr.length()){
  	    		int hashit = configstr.find('#');
  	    		configstr.erase(hashit);
  	    	}	
  	    	int execposition = configstr.find("=");
  	    	outputit = true;
  	    	string outtext = configstr.substr(execposition+1);
  	    	while (outtext.find(' ') < outtext.length()){
  	    		if (outtext[0] == ' '){
  	    			outtext.erase(0,1);
  	    		}
  	    		else if (outtext[outtext.length()-1] == ' '){
  	    			outtext.erase(outtext.length()-1);
  	    		}
  	    	}
  	    	outfile = outtext;
	  	    
  	    }
  	    else{
  	    	 //VARIABLES ASSIGNED
  	    	if (configstr.find('#') < configstr.length()){
  	    		cout <<"HERE" << endl;
  	    		int hashit = configstr.find('#');
  	    		configstr.erase(hashit);
  	    	}
  	    }
  	
    }


	ifstream secifile(indfile.c_str());
	string filename = "";
	vector<string> myvectorfiles;
	set<string> myset;
	while(true){
		getline(secifile,filename);
		if (secifile.fail()){
			break;
		}
		parse(filename, myset, myvectorfiles);
	}	
	for(vector<string>::iterator it = myvectorfiles.begin(); it != myvectorfiles.end(); ++it){
		cout << *it << endl;
	}

	ofstream ofile(outfile.c_str());
	for(vector<string>::iterator it = myvectorfiles.begin(); it != myvectorfiles.end(); ++it){
		ofile << (*it) << endl;
	}
	
	return 0;	
}



void parse(string filename, set<string>& myset, vector<string>& myvectorfiles){
	
	string holder;
	const unsigned int npos = -1;
	ifstream ifile(filename.c_str());
	cout << "FILE:" << filename << endl;

	if (ifile.fail()){
		cerr << "File does not exist" << endl;
     	myset.insert(filename);
	}
	else{
		if ( myset.find(filename) == myset.end()){
			myvectorfiles.push_back(filename);
			myset.insert(filename);
		}
	}

	while(true){

		ifile >> holder;
		if (ifile.fail()){
			break;
		}
		if ( holder.find('(') != npos && holder.find('(') == holder.find(']') +1){
			int pos = holder.find('(')+1;
			string outlink = holder.substr(pos);
			int end = outlink.find(')');
			outlink.erase(end);
			if (myset.find(outlink) == myset.end()){
				parse(outlink, myset, myvectorfiles);
			}
		}
	}
	cout << endl;
	cout << endl;

}
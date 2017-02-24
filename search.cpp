#include <iostream>
#include "mainwindow.h"
#include <fstream>
using namespace std;



int main(int argc, char* argv[])
{
    if(argc < 2){
      cerr << "usage: ./search index_file...Please provide an index file" << endl;
      return 1;
    }
    string outputfile = "";
    bool outputit = false;
    bool indexit = false;
    string configstr = "";
    ifstream ifile(argv[1]);
    string fileindex = "";
    int step_number = 0;
    double restart_probability = 0.0;

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
  	    	fileindex = indextext;	
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
  	    	outputfile = outtext;
	  	    
  	    }

  	    else if (configstr.substr(0,19) == "RESTART_PROBABILITY"){
  	    	if (configstr.find('#') <= configstr.length()){
  	    		int hashit = configstr.find('#');
  	    		configstr.erase(hashit);
  	    	}
  	    	int execposition = configstr.find("=");
  	    	string restarttext = configstr.substr(execposition+1);
  	    	while (restarttext.find(' ') < restarttext.length()){
  	    		if (restarttext[0] == ' '){
  	    			restarttext.erase(0,1);
  	    		}
  	    		else if (restarttext[restarttext.length()-1] == ' '){
  	    			restarttext.erase(restarttext.length()-1);
  	    		}
  	    	}
  	    	restart_probability = atof(restarttext.c_str()); 

  	    }
  	    else if (configstr.substr(0,11) == "STEP_NUMBER"){
  	    	if (configstr.find('#') <= configstr.length()){
  	    		int hashit = configstr.find('#');
  	    		configstr.erase(hashit);
  	    	}
  	    	int execposition = configstr.find("=");
  	    	string steptext = configstr.substr(execposition+1);
  	    	while (steptext.find(' ') < steptext.length()){
  	    		if (steptext[0] == ' '){
  	    			steptext.erase(0,1);
  	    		}
  	    		else if (steptext[steptext.length()-1] == ' '){
  	    			steptext.erase(steptext.length()-1);
  	    		}
  	    	}
  	    	step_number = atoi(steptext.c_str()); 
  	    }
  	    else{
  	    	 //VARIABLES ASSIGNED
  	    	if (configstr.find('#') < configstr.length()){
  	    		int hashit = configstr.find('#');
  	    		configstr.erase(hashit);
  	    		cout << configstr << endl;
  	    	}
  	    }
  	
    }

    QApplication app(argc,argv);
    MainWindow main_window(fileindex.c_str(), restart_probability, step_number);
    main_window.show();
    return app.exec();

  /****************   Add code to ***************************/
  /* Initialize your search engine and parse all the pages */
 

  
  return 0;
}



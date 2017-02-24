#include "pageparser.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;
#include "util.h"
MDPageParser::~MDPageParser(){

};

void clean(string& str){
   
   for ( unsigned int i =0; i< str.length(); i++){
         if ( ( (int)str[i] >= 65 && (int)str[i] <= 90 ) || ( (int)str[i] >= 97 && (int)str[i] <= 122 ) ){
               
         }
         else{
               str.erase(i,1);
               i--;
         }
   }  


}
void separate(string strholder, myset<string>& allWords){
      string temp = "";
      
      for ( unsigned int i =0; i < strholder.length(); i++) { 
                     
                      if ( ( (int)strholder[i] >= 65 && (int)strholder[i] <= 90 ) ||  ( (int)strholder[i] >= 97 && (int)strholder[i] <= 122 ) || ( (int)strholder[i] >= 48 && (int)strholder[i] <= 57) ){
                            temp += strholder[i];
                      }
                      else{
                           if ( temp != ""){
                              string lowertemp = convToLower(temp);
                              allWords.insert(lowertemp);
                              temp = "";
                           }
                      }
      }
      if ( temp != ""){
          string lowertemp = convToLower(temp);
          allWords.insert(lowertemp);
      }
}

void MDPageParser::parse(string filename, myset<string>& allWords, myset<string>& allLinks){
   
   static const size_t npos = -1;
   ifstream ifile(filename.c_str());
   vector<string> vecresult;
   vector<string> vecstr;
   string str = "";
   string preparelink = "";
   int counter = 0;
   bool getlinkbrackets = false;
   bool getlinkparentheses = false;

   while(true){ 
      counter++;
      ifile >> str;
      //cout << "***" << str << endl;
      
      if (ifile.fail()){
         //cout << "Extraction failed" << endl;
         break;
      }
      if (getlinkbrackets == true || getlinkparentheses  == true){
          preparelink += str + " ";
          //cout << "third if" << endl;
      } 
      if ( (str[0] == '[' && str.find(']') == npos) ) {
          preparelink += str + " ";
          getlinkbrackets = true;  
          //cout << "sec if" << endl;
      }
      if ( ( str[0] == '(' && str.find(')')  == npos ) ){
          preparelink += str + " ";
          getlinkparentheses = true;  
          //cout << "sec if" << endl;
         
      }
     
      if (preparelink == "" && getlinkbrackets == false && getlinkparentheses == false ){
          //cout << "fourth if" << endl;
          if (str.find(']') != npos  ){
               //cout << "fifth if" << endl;
               vecstr.push_back(str);
               str = "";
          }
          else if ( (str.find('[') == npos && str.find(']') == npos) ){
               //cout << "excep if" << endl;
               vecstr.push_back(str);
               str = "";
          }
      }
      
      if (preparelink == "" && getlinkparentheses == false && getlinkbrackets == false ){
          //cout << "fourth if" << endl;
          if ( str.find(')') != npos && str != "" ){
               //cout << "fifth if" << endl;
               vecstr.push_back(str);
               str = "";
          }
          else if ( (str.find('(') == npos && str.find(')') == npos && str != "" )){
               //cout << "excep if" << endl;
               vecstr.push_back(str);
               str = "";
          }
      }
      
      
      
      if ( str.find(']') != npos ){
         //cout << "I AM THE MOST MOTHERFUCKER" << endl;
         //cout << str[str.find(']')+1] << endl;
         //cout << str.find(')') << endl;
         if (str[str.find(']')+1] == '(' && str.find(')') == npos ){
             getlinkparentheses = true;
             //cout <<"UHUUUHU" << endl;
             goto jump;
         }
         else{       
             getlinkbrackets = false;
             //cout << "sixth if" << endl;
             if (preparelink != ""){
                 //cout << "seventh if" << endl;
                 preparelink.erase(preparelink.length() -1,1);
                 vecstr.push_back(preparelink);
             }
             preparelink = "";
         } 
      }
      if (  str.find(')') != npos){
         getlinkparentheses = false;
         getlinkbrackets = false;
         //cout << "sixth if" << endl;
         if (preparelink != ""){
               //cout << "seventh if" << endl;
               preparelink.erase(preparelink.length() -1,1);
               vecstr.push_back(preparelink);
         }
         preparelink = "";
      } 
      jump: 
          counter--;
         //cout << "JUMPED" << endl;
   }    
       
   //for ( unsigned int t = 0; t < vecstr.size(); t++) {
       // cout << "AFTER PREPARATION: " << vecstr[t] << endl;
  // } 
   //cout << endl;
   //cout << endl;

     
   for ( unsigned int m = 0; m < vecstr.size(); m++) {
     
       string temp = "";
       string anchor = "";
       string text = "";
       string website = "";
       string rest = "";
       string testing;
       stringstream ss;
       ss << vecstr[m];
       ss >> testing;    
      
          if ( vecstr[m][0] == '[' ){
                  unsigned int j =0;
                  
                  for ( j= 1; j < vecstr[m].find(']'); j++){
                          anchor += vecstr[m][j];
                  }
                  //there is a link afterwards
                  if ( vecstr[m][j+1] == '(' ) {
                      for ( unsigned int k = j+2; k < vecstr[m].find(')') ; k++) {
                              website += vecstr[m][k];
                      };
                     
                      allLinks.insert(website);
                      
                      if ( vecstr[m].find(')') != vecstr[m].length() - 1 && vecstr[m][vecstr[m].find(')') + 1] != '.' && vecstr[m][vecstr[m].find(')') + 1] != ','  ){
                        for ( unsigned int r = vecstr[m].find(')') + 1; r < vecstr[m].length(); r++){
                                  rest += vecstr[m][r];
                         } 
                         separate(rest, allWords);
                      }
                  }
                // no link afterwards.
                     /*string resttemp = "";
                     for ( int f = 0; f < anchor.length(); f++){
                             if ( anchor[f] == ' ' ){
                                  if ( resttemp != ""){
                                        allWords.insert(resttemp);
                                        resttemp = "";
                                  }
                             }
                             else{
                                   resttemp += anchor[f];
                                   if ( f == anchor.length() - 1 ){
                                        if ( resttemp != ""){
                                             allWords.insert(resttemp);
                                             resttemp = "";
                                        }
                                   }
                             }
                     } // CHECK ABOVE CODE*/
                     separate(anchor, allWords);     
                     if ( vecstr[m].find(']') != vecstr[m].length() - 1 && vecstr[m][vecstr[m].find(']') + 1] != '.' && vecstr[m][vecstr[m].find(']') + 1] != ',' ){
                        rest =  "";
                        for ( unsigned int r = vecstr[m].find(')') + 1; r < vecstr[m].length(); r++){
                                
                                  rest += vecstr[m][r];
                         } 
                         separate(rest, allWords);
                     }
                     
                
            
            
           }   
           else  if ( vecstr[m][0] == '(' ) {
                   
                   for ( unsigned int q = 1; q < vecstr[m].find(')'); q++) {
                        website += vecstr[m][q];
                   }
                   separate(website, allWords);
                     
                   
                   
                   if ( vecstr[m].find(')') != vecstr[m].length() - 1  && vecstr[m][vecstr[m].find(')') + 1] != '.' && vecstr[m][vecstr[m].find(')') + 1] != ','){
                        for ( unsigned int r = vecstr[m].find(')') + 1; r < vecstr[m].length(); r++){
                                  rest += vecstr[m][r];
                         } 
                         separate(rest, allWords);
                   }
            }
            
            
            else {
                     separate(testing, allWords);  
            }
             
            
                  
     }  // outer for loop

  }
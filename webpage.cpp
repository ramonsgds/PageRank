#include "webpage.h"
#include <fstream>

using namespace std;

WebPage::WebPage(){
  myfilename = "";
}

WebPage::WebPage(string filename){
	myfilename = filename;
}

WebPage::~WebPage(){
}

void WebPage::filename(string fname){
	myfilename = fname;
}

string WebPage::filename() const{
	return myfilename;
}

void WebPage::all_words(const myset<string>& words){
        myWords.clear();
		for (set<string>::iterator it = words.begin(); it != words.end(); ++it){
				myWords.insert(*it);
		}
}

myset<string> WebPage::all_words() const{
	 return myWords;

}

void WebPage::add_incoming_link(WebPage* wp){
      myincomingLinks.insert(wp);

}

myset<WebPage*> WebPage::incoming_links() const{
	return myincomingLinks;
}

void WebPage::add_outgoing_link(WebPage* wp){
	myoutgoingLinks.insert(wp);

}

myset<WebPage*> WebPage::outgoing_links() const{
      return myoutgoingLinks;
}

ostream& operator<< (ostream & os, const WebPage & page){	 
       static const size_t npos = -1;
       string str = page.filename();
       ifstream ifile(str.c_str());
       string myholder = "";
       while (true){
       		getline(ifile, myholder);
       		if (ifile.fail()){
       			break;
       		}
                  if (myholder == ""){
                        os << endl;
                        os << endl;
                  }
                  else{ 
       		      if ( myholder.find('(') != npos) {
                               while ( myholder.find('(') != npos &&  myholder[myholder.find('(') - 1] == (']') ){
                                    int openparentheses = myholder.find('(');
                                    int closedparentheses = myholder.find(')');
       			       	myholder.erase( myholder.begin()+openparentheses, myholder.begin()+closedparentheses+1);
       			      }
       			       os << myholder << " ";
       		            
                        }
       		      else{
       		      	os << myholder << " ";
       		      }
                  }
       }
       return os;
}
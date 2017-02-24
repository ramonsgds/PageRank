#include "searcheng.h"
#include <fstream>

using namespace std;

SearchEng::SearchEng(){

};

SearchEng::~SearchEng(){
	/*for (set<WebPage*>::iterator delit = searchengwp.begin(); delit != searchengwp.end(); delit++){
		  myset<WebPage*> deloutlinks = (*delit)->outgoing_links();
		  for (set<WebPage*>::iterator outits = deloutlinks.begin(); outits != deloutlinks.end(); ++outits){
		  			delete (*outits);
		  }
		  delete (*delit);
	}
	*/
};


void SearchEng::add_parse_from_index_file(string index_file, PageParser* parser){
	  ifstream ifile(index_file.c_str());
	  string wpfname;

	  while(true){
	  	ifile >> wpfname;
	  	if (ifile.fail()){
	  		break;
	  	}
	  	add_parse_page(wpfname, parser);
	  }



}

 void SearchEng::add_parse_page(string filename, PageParser* parser){
 	    myset<string> searchengWords;
 	    myset<string> searchengLinks;
 		parser->parse(filename, searchengWords, searchengLinks);
 		WebPage* wp;

 		ifstream secifile(filename.c_str());
 		if (secifile.fail()){
 			return;
 		}

 		if (stringSet.find(filename) == stringSet.end()){
 			wp = new WebPage(filename);
 			stringSet.insert(filename);
 			wpSet.insert(wp);
 		}
 		else{
 			for (myset<WebPage*>::iterator outsetit = wpSet.begin(); outsetit != wpSet.end(); ++outsetit){
					if ( (*outsetit)->filename() == filename){
					 	wp = *outsetit;
					}
			}
 		}
 		wp->all_words(searchengWords);

 		for (set<string>::iterator myit = searchengLinks.begin(); myit != searchengLinks.end(); ++myit){
 		 					
			if (stringSet.find((*myit)) == stringSet.end()){
				ifstream thirdifile((*myit).c_str());
				if(thirdifile.fail()){
					cout << *myit << "Failed" << endl;
				}
				else{
					WebPage* outlinkwp = new WebPage((*myit));
 		 			wp->add_outgoing_link(outlinkwp);
 		 			outlinkwp->add_incoming_link(wp);
 		 			stringSet.insert(*myit);
 		 			wpSet.insert(outlinkwp);
				}
				
			}
			else{
				for (myset<WebPage*>::iterator outsetit = wpSet.begin(); outsetit != wpSet.end(); ++outsetit){
					if ( (*outsetit)->filename() == *myit){
					 	wp->add_outgoing_link((*outsetit));
					 	(*outsetit)->add_incoming_link(wp);
					}
				}
			}
 		 				
 		}
 		 //incoming links should be defined here?
 		 searchengwp.insert(wp);

 		for (set<string>::iterator wordsit = searchengWords.begin(); wordsit != searchengWords.end(); ++wordsit){
 			myset<WebPage*> wpholder;
 			wpholder.insert(wp);
 			if ( mymap.find((*wordsit)) == mymap.end()){
 			 	mymap.insert(make_pair(*wordsit, wpholder));
	        }
	        else{
	       		myset<WebPage*> oldwpholder = mymap.find(*wordsit)->second;
	       		myset<WebPage*> newwpholder = oldwpholder.set_union(wpholder);
	       		mymap.erase(*wordsit);
	       		mymap.insert(make_pair(*wordsit, newwpholder));
	       		//mymap.find(*it)->second = newwpholder;
	        }
 		}
 }

 void SearchEng::copyMap(map<string,myset<WebPage*> >& outsidemap ){
 		for (map<string, myset<WebPage*> >::iterator it = mymap.begin(); it != mymap.end(); ++it){
 			 outsidemap.insert(make_pair(it->first,it->second));
 		}
 }

 void SearchEng::copyAllWp(myset<WebPage*>& outsideWpSet){
 		outsideWpSet.clear();
 	for (myset<WebPage*>::iterator sit = searchengwp.begin(); sit != searchengwp.end(); ++sit){
 		outsideWpSet.insert(*sit);
 	}

 }

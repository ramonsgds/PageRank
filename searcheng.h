#ifndef SEARCHENG_H
#define SEARCHENG_H

#include <map>
#include <vector>
#include <string>
#include "webpage.h"
#include "myset.h"
#include "pageparser.h"
#include "util.h"

class SearchEng {
 public:
  SearchEng();
  ~SearchEng();
  void add_parse_from_index_file(std::string index_file, 
				 PageParser* parser);
  void add_parse_page(std::string filename, 
		      PageParser* parser);

  /**** Add other desired member functions here *****/
  void copyMap(std::map<std::string,myset<WebPage*> >& outsidemap);
  void copyAllWp(myset<WebPage*>& outsideWpSet);

 private:
  /**** Add other desired data members here *****/
 	myset<WebPage*> searchengwp;
 	myset<std::string> stringSet;
 	myset<WebPage*> wpSet;
 	std::map<std::string, myset<WebPage*> > mymap;
};

#endif

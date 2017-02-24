#include "mainwindow.h"

using namespace std;


void pageRank(vector<WebPage*>& candidatepages, double RESTART_PROBABILITY, int STEP_NUMBER, map<WebPage*,double>& pagerankmap){


    pagerankmap.clear();
    map<WebPage*,double> tempmap;
    int N = candidatepages.size();
    for(vector<WebPage*>::iterator it = candidatepages.begin(); it != candidatepages.end(); ++it){
        pagerankmap.insert(make_pair((*it), 1.0/N));
        tempmap.insert(make_pair((*it), 1.0/N));
    }

    for(int i =0; i < STEP_NUMBER; i++){
       for(map<WebPage*,double>::iterator mapit = pagerankmap.begin(); mapit != pagerankmap.end(); ++mapit){
             myset<WebPage*> pagerankinclinks = (mapit->first)->incoming_links();
             double pagevalue = 0;
             pagevalue += RESTART_PROBABILITY/N;
             bool selfloop = false;
             for (myset<WebPage*>::iterator innerincit = pagerankinclinks.begin(); innerincit != pagerankinclinks.end(); ++innerincit ){
                  myset<WebPage*> incoutlinks = (*innerincit)->outgoing_links();
                  bool incselfloop = false;
                  for(myset<WebPage*>::iterator incoutit = incoutlinks.begin(); incoutit != incoutlinks.end(); ++incoutit){
                       if((*incoutit) == (*innerincit)){
                          incselfloop = true;
                       }
                  }

                  if(incselfloop == false){
                     pagevalue += (1- RESTART_PROBABILITY)*(tempmap.find(*innerincit)->second)/( ((tempmap.find(*innerincit)->first)->outgoing_links()).size()+1 );
                  }
                  else{
                     pagevalue += (1- RESTART_PROBABILITY)*(tempmap.find(*innerincit)->second)/( ((tempmap.find(*innerincit)->first)->outgoing_links()).size());
                  }
                 
                  if ((*innerincit) == mapit->first){
                    selfloop = true;
                  }
             }
             if (selfloop == false){
                if ((((mapit->first)->outgoing_links()).size()) == 0){
                  pagevalue += (1- RESTART_PROBABILITY)*(tempmap.find(mapit->first)->second)/(1);
                }
                else{
                 pagevalue += (1- RESTART_PROBABILITY)*(tempmap.find(mapit->first)->second)/( (((tempmap.find(mapit->first))->first)->outgoing_links()).size()+1);
                }
             }
             
             mapit->second = pagevalue;  
       }
       tempmap.clear();
       for (map<WebPage*,double>::iterator copyit = pagerankmap.begin(); copyit != pagerankmap.end(); ++copyit){
            tempmap.insert(make_pair(copyit->first, copyit->second));
       } 

    }
    double sum = 0;
    for (map<WebPage*,double>::iterator showit = pagerankmap.begin(); showit != pagerankmap.end(); ++showit){
          cout << (showit->first)->filename() << " :" << showit->second << endl;
          sum += showit->second;
    }
    cout << "Sum: " << sum << endl;

}


struct LessThan{
	bool operator()(WebPage* wp1, WebPage* wp2){
		return wp1->filename() < wp2->filename();
	}
};

struct hasLessIncLinks{
  bool operator()(WebPage* wp1, WebPage* wp2){
    return (wp1->incoming_links()).size() < (wp2->incoming_links()).size();
  }
};

struct hasLessOutLinks{
  bool operator()(WebPage* wp1, WebPage* wp2){
    return (wp1->outgoing_links()).size() < (wp2->outgoing_links()).size();
  }
};

struct higherPageRank{
  higherPageRank(map<WebPage*,double>& pagerankmap){
    pagemap = pagerankmap;
  }
  bool operator()(WebPage* wp1, WebPage* wp2){
    return ((pagemap.find(wp1))->second) > ((pagemap.find(wp2))->second);
  }
  map<WebPage*,double> pagemap;
}; 


MainWindow::MainWindow(const char* fname, double RESTART_PROBABILITY, int STEP_NUMBER){
   	  
  this->stepNumber_ = STEP_NUMBER;
  this->restartProbability_ =  RESTART_PROBABILITY;

  overallLayout = new QHBoxLayout();

	resultsLayout = new QVBoxLayout();
	overallLayout->addLayout(resultsLayout);
	resultsLabel = new QLabel("Results");
	QFont restitleFont;
	restitleFont.setBold(true);
	restitleFont.setPointSize(12);
	resultsLabel->setFont(restitleFont);
	quitButton = new QPushButton("Quit");
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quitApp()));
	resquitLayout = new QHBoxLayout();
	resquitLayout->addWidget(resultsLabel);
	resquitLayout->addWidget(quitButton);
	resultsLayout->addLayout(resquitLayout);
	resultsModeLayout = new QHBoxLayout();
  pagerankOrderButton = new QRadioButton(tr("PageRank"));
  pagerankOrderButton->setChecked(true);
  connect(pagerankOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeOrderList()));
  filenameOrderButton = new QRadioButton(tr("filename"));
  connect(filenameOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeOrderList()));
	outlinksOrderButton = new QRadioButton(tr("outgoingLinks"));
  connect(outlinksOrderButton, SIGNAL(toggled(bool)),this, SLOT(changeOrderList()));
	inglinksOrderButton = new QRadioButton(tr("incomingLinks"));
  connect(inglinksOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeOrderList()));
  resultsModeLayout->addWidget(pagerankOrderButton);
	resultsModeLayout->addWidget(filenameOrderButton);
	resultsModeLayout->addWidget(outlinksOrderButton);
	resultsModeLayout->addWidget(inglinksOrderButton);
	resultsModeBox = new QGroupBox();
	resultsModeBox->setLayout(resultsModeLayout);
	resultsLayout->addWidget(resultsModeBox);
	results = new QListWidget();
  connect(results, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(displayNewWindow(QListWidgetItem*)));
	resultsLayout->addWidget(results);

	verticalLine = new QFrame(this);
	verticalLine->setFrameShape(QFrame::VLine);
	verticalLine->setFrameShadow(QFrame::Sunken);
	verticalLine->setLineWidth(1);
	overallLayout->addWidget(verticalLine);


	searchLayout = new QVBoxLayout();
	overallLayout->addLayout(searchLayout);
	searchLabel = new QLabel("Search");
  searchLabel->setMaximumHeight(30);
	QFont titleFont;
	titleFont.setBold(true);
	titleFont.setPointSize(12);
	searchLabel->setFont(titleFont);
	searchLayout->addWidget(searchLabel);
	searchBoxLayout = new QHBoxLayout();
  singleButton = new QRadioButton(tr("Single"));
	andButton = new QRadioButton(tr("AND"));
	orButton = new QRadioButton(tr("OR"));
  searchBoxLayout->addWidget(singleButton);
	searchBoxLayout->addWidget(andButton);
	searchBoxLayout->addWidget(orButton);
	searchMode = new QGroupBox();
	searchMode->setLayout(searchBoxLayout);
	searchLayout->addWidget(searchMode);
	searchWords =  new QLineEdit("");
	searchLayout->addWidget(searchWords);
	searchButton = new QPushButton("Search");
	connect(searchButton,SIGNAL(clicked()),this, SLOT(doSearch()));
  connect(searchWords, SIGNAL(returnPressed()),this, SLOT(doSearch()));
	searchLayout->addWidget(searchButton);

  setWindowTitle("Main Window");
	setLayout(overallLayout);


	string indexfname = fname;
  SearchEng searchengine;
  PageParser* mypageparser = new MDPageParser;
  searchengine.add_parse_from_index_file(indexfname, mypageparser);
  searchengine.copyMap(searchmap);
  searchengine.copyAllWp(allPossibleWp);

	display = new QWidget();
	overallDisplayLayout =  new QHBoxLayout();
  showLayout = new QVBoxLayout();
  displayTitle = new QLabel();
  QFont displaytitlefont;
  displaytitlefont.setBold(true);
  displaytitlefont.setPointSize(11);
  displayTitle->setFont(displaytitlefont);
  quitLayout = new QHBoxLayout();
  quitLayout->addWidget(displayTitle);
  quitDisplayButton = new QPushButton("Quit");
  connect(quitDisplayButton, SIGNAL(clicked()), this, SLOT(quitDisplay()));
  quitLayout->addWidget(quitDisplayButton);
	displayedText = new QTextEdit();
  displayedText->setMinimumWidth(500);
  showLayout->addLayout(quitLayout);
  showLayout->addWidget(displayedText);


	overallDisplayLayout->addLayout(showLayout);

  displayVerticalLine = new QFrame(this);
  displayVerticalLine->setFrameShape(QFrame::VLine);
  displayVerticalLine->setFrameShadow(QFrame::Sunken);
  displayVerticalLine->setLineWidth(1);
  overallDisplayLayout->addWidget(displayVerticalLine);

	linksLayout = new QVBoxLayout();
  overallDisplayLayout->addLayout(linksLayout);

  outglinksLabel = new QLabel("Outgoing Links");
  linksLayout->addWidget(outglinksLabel);
  outLinksOrderLayout =  new QHBoxLayout();
  filenameOutLinksOrderButton = new QRadioButton(tr("Filename"));
  connect(filenameOutLinksOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeOutLinksDisplayOrderList()));
  outLinksOutLinksOrderButton = new QRadioButton(tr("Outgoing Links"));
  connect(outLinksOutLinksOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeOutLinksDisplayOrderList()));
  incLinksOutLinksOrderButton = new QRadioButton(tr("Incoming Links"));
  connect(incLinksOutLinksOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeOutLinksDisplayOrderList()));
  outLinksOrderLayout->addWidget(filenameOutLinksOrderButton);
  outLinksOrderLayout->addWidget(outLinksOutLinksOrderButton);
  outLinksOrderLayout->addWidget(incLinksOutLinksOrderButton);
  outLinksGroupBox = new QGroupBox();
  outLinksGroupBox->setLayout(outLinksOrderLayout);
  linksLayout->addWidget(outLinksGroupBox);
  outgLinks = new QListWidget();
  linksLayout->addWidget(outgLinks);


  incLinksLabel = new QLabel("Incoming Links");
  linksLayout->addWidget(incLinksLabel);
  incLinksOrderLayout = new QHBoxLayout();
  filenameIncLinksOrderButton = new QRadioButton(tr("Filename"));
  connect(filenameIncLinksOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeIncLinksDisplayOrderList()));
  outLinksIncLinksOrderButton = new QRadioButton(tr("Outgoing Links"));
  connect(outLinksIncLinksOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeIncLinksDisplayOrderList()));
  incLinksIncLinksOrderButton = new QRadioButton(tr("Incoming Links"));
  connect(incLinksIncLinksOrderButton, SIGNAL(toggled(bool)), this, SLOT(changeIncLinksDisplayOrderList()));
  incLinksOrderLayout->addWidget(filenameIncLinksOrderButton);
  incLinksOrderLayout->addWidget(outLinksIncLinksOrderButton);
  incLinksOrderLayout->addWidget(incLinksIncLinksOrderButton);
  incLinksGroupBox = new QGroupBox();
  incLinksGroupBox->setLayout(incLinksOrderLayout);
  linksLayout->addWidget(incLinksGroupBox);
  incLinks = new QListWidget();
	linksLayout->addWidget(incLinks);
	
  connect(outgLinks, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(displayNewWindow(QListWidgetItem*)));
  connect(incLinks, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(displayNewWindow(QListWidgetItem*)));

	display->setLayout(overallDisplayLayout);
	display->hide();
  delete mypageparser;
}


MainWindow::~MainWindow(){
};

void MainWindow::doSearch(){
	string searchterms = (searchWords->text()).toStdString();
	results->clear();
	webListHolder.clear();


  if (singleButton->isChecked()){
      string strsep = "";
      myset<WebPage*> wpresult;
      bool wpfilled = false;
      for (unsigned int i = 0; i< searchterms.length(); i++){
            if (searchterms[i] == ' ' && strsep != ""){
                string lowerstr = convToLower(strsep);
                strsep = "";
                if (wpfilled == true){
                   QMessageBox::information(this,"Error", "More than one word entered for a Single search!");
                   wpresult.clear();
                }
                else{
                      wpfilled = true;
                      if (searchmap.find(lowerstr) != searchmap.end()){
                          myset<WebPage*> tempset = searchmap.find(lowerstr)->second; //SHALLOW OR DEEP COPY??
                          wpresult = tempset;
                      }
                }
                
            }
            else{
               if (searchterms[i] != ' '){
                    strsep += searchterms[i];
               }
            }
      }
      if ( strsep != ""){
            string lowerstr = convToLower(strsep);
            strsep = "";
            if (wpfilled == true){
               QMessageBox::information(this,"Error", "More than one word entered for a Single search!");
               wpresult.clear();
            }
            else{
                wpfilled = true;
                if (searchmap.find(lowerstr) != searchmap.end()){
                    myset<WebPage*> tempset = searchmap.find(lowerstr)->second; //SHALLOW OR DEEP COPY??
                    wpresult = tempset;
                }
            }           
      }
      myset<WebPage*> visitedwp;
      for(myset<WebPage*>::iterator it = wpresult.begin(); it != wpresult.end(); ++it){
          if(visitedwp.find(*it) == visitedwp.end()){
              webListHolder.push_back(*it);
              visitedwp.insert(*it);
          }

          myset<WebPage*> candidatesIncLinks = (*it)->incoming_links();
          for(myset<WebPage*>::iterator candincit = candidatesIncLinks.begin(); candincit != candidatesIncLinks.end(); ++candincit){
              if (visitedwp.find(*candincit) == visitedwp.end()){
                  visitedwp.insert(*candincit);
                  webListHolder.push_back(*candincit);
              }
          }

          myset<WebPage*> candidatesOutLinks = (*it)->outgoing_links();
          for(myset<WebPage*>::iterator candoutit = candidatesOutLinks.begin(); candoutit != candidatesOutLinks.end(); ++candoutit){
              if (visitedwp.find(*candoutit) == visitedwp.end()){
                  webListHolder.push_back(*candoutit);
                  visitedwp.insert(*candoutit);
              }   
          }

      }
      pageRank(webListHolder, restartProbability_, stepNumber_, pagerankmap);
      higherPageRank op(pagerankmap);
      if (webListHolder.size() > 1){
           mergeSort(webListHolder, op);
      }
      for(vector<WebPage*>::iterator secit = webListHolder.begin(); secit != webListHolder.end(); ++secit){
          stringstream ss1;
          string incsizestr = "";
          string outsizestr = "";
          string pagerankstr = "";
          ss1 << (*secit)->incoming_links().size();
          ss1 >> incsizestr;
          stringstream ss2;
          ss2 << (*secit)->outgoing_links().size();
          ss2 >> outsizestr;
          stringstream ss3;
          ss3 << (pagerankmap.find(*secit))->second;
          ss3 >> pagerankstr;
          string label = (*secit)->filename() + ", #Outgoing Links: " + outsizestr + ", #Incoming Links: " + incsizestr + ", PageRank: " + pagerankstr + ".";
          results->addItem(QString::fromStdString(label));
      }
  }


	else if (andButton->isChecked()){
        string strsep = "";
        myset<WebPage*> wpresult;
        bool emptysearch = false;
        bool firstiteration =  false;
        for ( unsigned int i = 0 ; i < searchterms.length(); i++){
               if (searchterms[i] == ' ' && strsep != ""){
                   string lowerstr = convToLower(strsep);
                   strsep = "";
                   if (searchmap.find(lowerstr) != searchmap.end()){
                          myset<WebPage*> tempset = searchmap.find(lowerstr)->second; //SHALLOW OR DEEP COPY??
                          if (wpresult.empty() && firstiteration == false){
                             wpresult = tempset; //SHALLOW OR DEEP COPY?
                             firstiteration = true;
                          }
                          else{
                              wpresult = wpresult.set_intersection(tempset); 
                          }
                   }
                   else{
                        emptysearch = true;
                   }
                   
               }
               else{
                   if (searchterms[i] != ' '){
                      strsep += searchterms[i];
                   }
               }
        }
        if ( strsep != ""){
               string lowerstr = convToLower(strsep);
                   strsep = "";
                   if (searchmap.find(lowerstr) != searchmap.end()){
                          myset<WebPage*> tempset = searchmap.find(lowerstr)->second; //SHALLOW OR DEEP COPY??
                          if (wpresult.empty() && firstiteration == false){
                             wpresult = tempset; //SHALLOW OR DEEP COPY?
                          }
                          else{
                              wpresult = wpresult.set_intersection(tempset);  
                          }
                   }
                   else{
                        emptysearch = true;
                   }
        }
        if ( emptysearch == true){
            wpresult.clear();
        }

        myset<WebPage*> visitedwp;
        for(myset<WebPage*>::iterator it = wpresult.begin(); it != wpresult.end(); ++it){
          if(visitedwp.find(*it) == visitedwp.end()){
              webListHolder.push_back(*it);
              visitedwp.insert(*it);
          }

          myset<WebPage*> candidatesIncLinks = (*it)->incoming_links();
          for(myset<WebPage*>::iterator candincit = candidatesIncLinks.begin(); candincit != candidatesIncLinks.end(); ++candincit){
              if (visitedwp.find(*candincit) == visitedwp.end()){
                  visitedwp.insert(*candincit);
                  webListHolder.push_back(*candincit);
              }
          }

          myset<WebPage*> candidatesOutLinks = (*it)->outgoing_links();
          for(myset<WebPage*>::iterator candoutit = candidatesOutLinks.begin(); candoutit != candidatesOutLinks.end(); ++candoutit){
              if (visitedwp.find(*candoutit) == visitedwp.end()){
                  webListHolder.push_back(*candoutit);
                  visitedwp.insert(*candoutit);
              }   
          }
        }
        pageRank(webListHolder, restartProbability_, stepNumber_, pagerankmap);
        higherPageRank op(pagerankmap);
        mergeSort(webListHolder, op);
        for(vector<WebPage*>::iterator secit = webListHolder.begin(); secit != webListHolder.end(); ++secit){
            stringstream ss1;
            string incsizestr = "";
            string outsizestr = "";
            string pagerankstr = "";
            ss1 << (*secit)->incoming_links().size();
            ss1 >> incsizestr;
            stringstream ss2;
            ss2 << (*secit)->outgoing_links().size();
            ss2 >> outsizestr;
            stringstream ss3;
            ss3 << (pagerankmap.find(*secit))->second;
            ss3 >> pagerankstr;
            string label = (*secit)->filename() + ", #Outgoing Links: " + outsizestr + ", #Incoming Links: " + incsizestr + ", PageRank: " + pagerankstr + ".";
            results->addItem(QString::fromStdString(label));
        }
          

	}

	else if (orButton->isChecked()){
		    string strsep = "";
        myset<WebPage*> wpresult;
        for (unsigned int i = 0; i< searchterms.length(); i++){
            if (searchterms[i] == ' ' && strsep != ""){
                string lowerstr = convToLower(strsep);
                strsep = "";
                if (searchmap.find(lowerstr) != searchmap.end()){
                    myset<WebPage*> tempset = searchmap.find(lowerstr)->second; //SHALLOW OR DEEP COPY??
                    if (wpresult.empty()){
                       wpresult = tempset; //SHALLOW OR DEEP COPY?
                    }
                    else{
                       wpresult = wpresult.set_union(tempset);
                    }
                }
                
            }
            else{
               if (searchterms[i] != ' '){
                    strsep += searchterms[i];
               }
            }
        }
        if ( strsep != ""){
            string lowerstr = convToLower(strsep);
            strsep = "";
            if (searchmap.find(lowerstr) != searchmap.end()){
                    myset<WebPage*> tempset = searchmap.find(lowerstr)->second; //SHALLOW OR DEEP COPY??
                    if (wpresult.empty()){
                       wpresult = tempset; //SHALLOW OR DEEP COPY?
                    }
                    else{
                       wpresult = wpresult.set_union(tempset);
                    }
            }
        }
        myset<WebPage*> visitedwp;
        for(myset<WebPage*>::iterator it = wpresult.begin(); it != wpresult.end(); ++it){
        	if(visitedwp.find(*it) == visitedwp.end()){
              webListHolder.push_back(*it);
              visitedwp.insert(*it);
          }

          myset<WebPage*> candidatesIncLinks = (*it)->incoming_links();
          for(myset<WebPage*>::iterator candincit = candidatesIncLinks.begin(); candincit != candidatesIncLinks.end(); ++candincit){
              if (visitedwp.find(*candincit) == visitedwp.end()){
                  visitedwp.insert(*candincit);
                  webListHolder.push_back(*candincit);
              }
          }

          myset<WebPage*> candidatesOutLinks = (*it)->outgoing_links();
          for(myset<WebPage*>::iterator candoutit = candidatesOutLinks.begin(); candoutit != candidatesOutLinks.end(); ++candoutit){
              if (visitedwp.find(*candoutit) == visitedwp.end()){
                  webListHolder.push_back(*candoutit);
                  visitedwp.insert(*candoutit);
              }   
          }

        }
        pageRank(webListHolder, restartProbability_, stepNumber_, pagerankmap);
        higherPageRank op(pagerankmap);
        if (webListHolder.size() > 1){
             mergeSort(webListHolder, op);
        }
        for(vector<WebPage*>::iterator secit = webListHolder.begin(); secit != webListHolder.end(); ++secit){
            stringstream ss1;
            string incsizestr = "";
            string outsizestr = "";
            string pagerankstr = "";
            ss1 << (*secit)->incoming_links().size();
            ss1 >> incsizestr;
            stringstream ss2;
            ss2 << (*secit)->outgoing_links().size();
            ss2 >> outsizestr;
            stringstream ss3;
            ss3 << (pagerankmap.find(*secit))->second;
            ss3 >> pagerankstr;
            string label = (*secit)->filename() + ", #Outgoing Links: " + outsizestr + ", #Incoming Links: " + incsizestr + ", PageRank: " + pagerankstr + ".";
            results->addItem(QString::fromStdString(label));
        }

	}
	else{
        QMessageBox::information(this,"Error", "Invalid query prompt entered!");       
	}
    
}

void MainWindow::quitApp(){
	QApplication::exit();

  delete searchLabel;
  delete searchWords;
  delete singleButton;
  delete andButton;
  delete orButton;
  delete searchBoxLayout;
  delete searchMode;
  delete searchButton;
  delete searchLayout;
  delete verticalLine;

  delete resultsLabel;
  delete quitButton;
  delete resquitLayout;
  delete pagerankOrderButton;
  delete filenameOrderButton;
  delete outlinksOrderButton;
  delete inglinksOrderButton;
  delete resultsModeLayout;
  delete resultsModeBox;
  delete results;
  delete resultsLayout;
  delete overallLayout;
    

 
  delete displayTitle;
  delete quitDisplayButton;
  delete quitLayout;
  delete displayedText;
  delete showLayout;

  delete displayVerticalLine;

  delete outglinksLabel;
  delete filenameOutLinksOrderButton;
  delete outLinksOutLinksOrderButton;
  delete incLinksOutLinksOrderButton;
  delete outLinksOrderLayout;
  delete outLinksGroupBox;
  delete outgLinks;

  delete incLinksLabel;
  delete filenameIncLinksOrderButton;
  delete outLinksIncLinksOrderButton;
  delete incLinksIncLinksOrderButton;
  delete incLinksOrderLayout;
  delete incLinksGroupBox;
  delete incLinks;
 
  delete linksLayout;
  delete overallDisplayLayout;
  delete display;

  for(myset<WebPage*>::iterator delit =  allPossibleWp.begin(); delit != allPossibleWp.end(); ++delit){
     delete (*delit);
  }
}


void MainWindow::changeOrderList(){
    if (!webListHolder.empty()){
        
        results->clear();
        
        if (pagerankOrderButton->isChecked()){
            higherPageRank op(pagerankmap);
            if (webListHolder.size() >1){
                mergeSort(webListHolder,op);
            }
            for(vector<WebPage*>::iterator secit = webListHolder.begin(); secit != webListHolder.end(); ++secit){
                stringstream ss1;
                string incsizestr = "";
                string outsizestr = "";
                string pagerankstr = "";
                ss1 << (*secit)->incoming_links().size();
                ss1 >> incsizestr;
                stringstream ss2;
                ss2 << (*secit)->outgoing_links().size();
                ss2 >> outsizestr;
                stringstream ss3;
                ss3 << (pagerankmap.find(*secit))->second;
                ss3 >> pagerankstr;
                string label = (*secit)->filename() + ", #Outgoing Links: " + outsizestr + ", #Incoming Links: " + incsizestr + ", PageRank: " + pagerankstr + ".";
                results->addItem(QString::fromStdString(label));
            }
        }
        else if (filenameOrderButton->isChecked()){
            LessThan op;
            if (webListHolder.size() > 1){
               mergeSort(webListHolder, op);
            }
            for(vector<WebPage*>::iterator secit = webListHolder.begin(); secit != webListHolder.end(); ++secit){
                stringstream ss1;
                string incsizestr = "";
                string outsizestr = "";
                string pagerankstr = "";
                ss1 << (*secit)->incoming_links().size();
                ss1 >> incsizestr;
                stringstream ss2;
                ss2 << (*secit)->outgoing_links().size();
                ss2 >> outsizestr;
                stringstream ss3;
                ss3 << (pagerankmap.find(*secit))->second;
                ss3 >> pagerankstr;
                string label = (*secit)->filename() + ", #Outgoing Links: " + outsizestr + ", #Incoming Links: " + incsizestr + ", PageRank: " + pagerankstr + ".";
                results->addItem(QString::fromStdString(label));
            }
        }
        else if (outlinksOrderButton->isChecked()){
            hasLessOutLinks op;
            if (webListHolder.size() > 1){
               mergeSort(webListHolder, op);
            }
            for(vector<WebPage*>::iterator secit = webListHolder.begin(); secit != webListHolder.end(); ++secit){
                stringstream ss1;
                string incsizestr = "";
                string outsizestr = "";
                string pagerankstr = "";
                ss1 << (*secit)->incoming_links().size();
                ss1 >> incsizestr;
                stringstream ss2;
                ss2 << (*secit)->outgoing_links().size();
                ss2 >> outsizestr;
                stringstream ss3;
                ss3 << (pagerankmap.find(*secit))->second;
                ss3 >> pagerankstr;
                string label = (*secit)->filename() + ", #Outgoing Links: " + outsizestr + ", #Incoming Links: " + incsizestr + ", PageRank: " + pagerankstr + ".";
                results->addItem(QString::fromStdString(label));
            }
        }
        else{
            hasLessIncLinks op;
            if (webListHolder.size() > 1){
               mergeSort(webListHolder, op);
            }
            for(vector<WebPage*>::iterator secit = webListHolder.begin(); secit != webListHolder.end(); ++secit){
                stringstream ss1;
                string incsizestr = "";
                string outsizestr = "";
                string pagerankstr = "";
                ss1 << (*secit)->incoming_links().size();
                ss1 >> incsizestr;
                stringstream ss2;
                ss2 << (*secit)->outgoing_links().size();
                ss2 >> outsizestr;
                stringstream ss3;
                ss3 << (pagerankmap.find(*secit))->second;
                ss3 >> pagerankstr;
                string label = (*secit)->filename() + ", #Outgoing Links: " + outsizestr + ", #Incoming Links: " + incsizestr + ", PageRank: " + pagerankstr + ".";
                results->addItem(QString::fromStdString(label));
            }
        }
    }
}


void MainWindow::displayNewWindow(QListWidgetItem* item){
    string strfname = (item->text()).toStdString();
    WebPage* mywp = NULL;
    stringstream myfilename(strfname);
    string holder = "";
    getline(myfilename,strfname, ',');
    displayedIncLinksVector.clear();
    displayedOutLinksVector.clear();
    item->setSelected(false);
    outgLinks->clear();
    incLinks->clear();
    displayTitle->clear();

    displayTitle->setText(QString::fromStdString(strfname));
    for (myset<WebPage*>::iterator it = allPossibleWp.begin(); it != allPossibleWp.end(); ++it){
      if ( (*it)->filename() == strfname){
            mywp = *it;
      }
    }
    if(mywp != NULL){
      stringstream ss;
      ss << *mywp;
      string txt = ss.str();
      displayedText->setPlainText(QString::fromStdString(txt)); 

      displayedOutLinks = mywp->outgoing_links();
  
      displayedIncLinks = mywp->incoming_links();

      for (myset<WebPage*>::iterator outit = displayedOutLinks.begin(); outit != displayedOutLinks.end(); ++outit){
          displayedOutLinksVector.push_back(*outit);
      }
      for (myset<WebPage*>::iterator incit = displayedIncLinks.begin(); incit != displayedIncLinks.end(); ++incit){
          displayedIncLinksVector.push_back(*incit);
      }

      if (filenameOutLinksOrderButton->isChecked()){
          LessThan op;
          mergeSort(displayedOutLinksVector, op);
      }
      else if (outLinksOutLinksOrderButton->isChecked()){
          hasLessOutLinks op;
          mergeSort(displayedOutLinksVector, op);
      }
      else{
          hasLessIncLinks op;
          mergeSort(displayedOutLinksVector, op);
      }

      if (filenameIncLinksOrderButton->isChecked()){
          LessThan op;
          mergeSort(displayedIncLinksVector,op);
      }
      else if (outLinksIncLinksOrderButton->isChecked()){
          hasLessOutLinks op;
          mergeSort(displayedIncLinksVector,op);
      }
      else{
          hasLessIncLinks op;
          mergeSort(displayedIncLinksVector,op);
      }

      for(vector<WebPage*>::iterator vecoutit = displayedOutLinksVector.begin(); vecoutit != displayedOutLinksVector.end(); ++vecoutit){
          outgLinks->addItem(QString::fromStdString((*vecoutit)->filename()));
      }
      for(vector<WebPage*>::iterator vecincit = displayedIncLinksVector.begin(); vecincit != displayedIncLinksVector.end(); ++vecincit){
          incLinks->addItem(QString::fromStdString((*vecincit)->filename()));
      }
      display->show();
      display->setWindowTitle(QString::fromStdString(strfname));  
       
    }
}


void MainWindow::changeOutLinksDisplayOrderList(){
    if (display->isVisible()){
            outgLinks->clearSelection();
            outgLinks->clearFocus();
            outgLinks->clear();
    }

    if (filenameOutLinksOrderButton->isChecked()){
        LessThan op;
        mergeSort(displayedOutLinksVector, op);
    }
    else if (outLinksOutLinksOrderButton->isChecked()){
        hasLessOutLinks op;
        mergeSort(displayedOutLinksVector, op);
    }
    else{
        hasLessIncLinks op;
        mergeSort(displayedOutLinksVector, op);
    }   

    for(vector<WebPage*>::iterator vecoutit = displayedOutLinksVector.begin(); vecoutit != displayedOutLinksVector.end(); ++vecoutit){
        outgLinks->addItem(QString::fromStdString((*vecoutit)->filename()));
    }
    
}


void MainWindow::changeIncLinksDisplayOrderList(){

   
    if (display->isVisible()){
            incLinks->clearSelection();
            incLinks->clearFocus();
            incLinks->clear();
    }

    if (filenameIncLinksOrderButton->isChecked()){
        LessThan op;
        mergeSort(displayedIncLinksVector,op);
    }
    else if (outLinksIncLinksOrderButton->isChecked()){
        hasLessOutLinks op;
        mergeSort(displayedIncLinksVector,op);       
    }
    else{
        hasLessIncLinks op;
        mergeSort(displayedIncLinksVector,op);
    }

    for(vector<WebPage*>::iterator vecincit = displayedIncLinksVector.begin(); vecincit != displayedIncLinksVector.end(); ++vecincit){
        incLinks->addItem(QString::fromStdString((*vecincit)->filename()));
    }
  
}


void MainWindow::quitDisplay(){

    display->hide();
}
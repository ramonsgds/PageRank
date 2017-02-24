#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QGroupBox>
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <QApplication>
#include <string>
#include <map>
#include <vector>
#include "msort.h"
#include <string>
#include "webpage.h"
#include "searcheng.h"
#include "util.h"
#include <sstream>
//include vector/list/map/set ?!?

class MainWindow: public QWidget
{
	Q_OBJECT
public:
	MainWindow(const char* fname, double RESTART_PROBABILITY, int STEP_NUMBER);
	~MainWindow();

private slots:
	void doSearch();
    void quitApp();
    void changeOrderList();
    void displayNewWindow(QListWidgetItem* item);
    void changeOutLinksDisplayOrderList();
    void changeIncLinksDisplayOrderList();
    void quitDisplay();

private:

    map<string, myset<WebPage*> > searchmap;
    vector<WebPage*> webListHolder;


    QLabel* searchLabel;
	QLineEdit* searchWords;
	QGroupBox* searchMode;
    QRadioButton* singleButton;
    QRadioButton* andButton;
    QRadioButton* orButton;
    QHBoxLayout* searchBoxLayout;
    QPushButton* searchButton;
    QVBoxLayout* searchLayout;
    QFrame* verticalLine;

    QLabel* resultsLabel;
    QPushButton* quitButton;
    QHBoxLayout* resquitLayout;
    QVBoxLayout* resultsLayout;
    QHBoxLayout* resultsModeLayout;
    QListWidget* results;
    QGroupBox* resultsModeBox;
    QRadioButton* pagerankOrderButton;
    QRadioButton* filenameOrderButton;
    QRadioButton* outlinksOrderButton;
    QRadioButton* inglinksOrderButton;
    QHBoxLayout* overallLayout;

 
    QWidget* display; //based on SIGNAL call display.show() or display.hide()
    QLabel*  displayTitle;
    QHBoxLayout* quitLayout;
    QPushButton* quitDisplayButton;
    QVBoxLayout* showLayout;
    QFrame* displayVerticalLine;
    QHBoxLayout* overallDisplayLayout;
    QTextEdit* displayedText;
    QVBoxLayout* linksLayout;
    QLabel* outglinksLabel;
    QHBoxLayout* outLinksOrderLayout;
    QGroupBox* outLinksGroupBox;
    QRadioButton* filenameOutLinksOrderButton;
    QRadioButton* outLinksOutLinksOrderButton;
    QRadioButton* incLinksOutLinksOrderButton;
    QListWidget* outgLinks;

    QLabel* incLinksLabel;
    QHBoxLayout* incLinksOrderLayout;
    QGroupBox* incLinksGroupBox;
    QRadioButton* filenameIncLinksOrderButton;
    QRadioButton* outLinksIncLinksOrderButton;
    QRadioButton* incLinksIncLinksOrderButton;
    QListWidget* incLinks;

    myset<WebPage*>  displayedOutLinks;
    myset<WebPage*>  displayedIncLinks;
    myset<WebPage*>  allPossibleWp;
    vector<WebPage*> displayedOutLinksVector;
    vector<WebPage*> displayedIncLinksVector;

    map<WebPage*,double> pagerankmap;
    double restartProbability_;
    int stepNumber_;
};


#endif
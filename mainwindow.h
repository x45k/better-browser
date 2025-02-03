#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QTabWidget>
#include <QLineEdit>
#include <QToolBar>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void navigateToUrl();
    void updateUrl(const QUrl &url);
    void createNewTab();
    void closeTab(int index);
    void goBack();
    void goForward(); // Declare the goForward slot
    void refreshPage();
    void updateUrlBar(int index); // Declare the updateUrlBar slot

private:
    QTabWidget *tabWidget;
    QLineEdit *urlLineEdit;
    QToolBar *toolBar;
    QPushButton *newTabButton;
    QPushButton *backButton;
    QPushButton *forwardButton; // Declare the forwardButton
    QPushButton *refreshButton;

    QWebEngineView* currentWebView();
};

#endif // MAINWINDOW_H

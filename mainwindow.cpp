#include "mainwindow.h"
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QUrl>
#include <QToolBar>
#include <QPushButton>
#include <QShortcut>
#include <QLineEdit>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Chrome");

    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    urlLineEdit = new QLineEdit(this);
    urlLineEdit->setPlaceholderText("Enter URL or search...");

    connect(urlLineEdit, &QLineEdit::returnPressed, this, &MainWindow::navigateToUrl);

    toolBar = new QToolBar(this);
    addToolBar(toolBar);

    newTabButton = new QPushButton("New Tab", this);
    connect(newTabButton, &QPushButton::clicked, this, &MainWindow::createNewTab);
    toolBar->addWidget(newTabButton);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::goBack);
    toolBar->addWidget(backButton);

    forwardButton = new QPushButton("Forward", this);
    connect(forwardButton, &QPushButton::clicked, this, &MainWindow::goForward);
    toolBar->addWidget(forwardButton);

    refreshButton = new QPushButton("Refresh", this);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshPage);
    toolBar->addWidget(refreshButton);

    toolBar->addWidget(urlLineEdit);

    createNewTab();

    setCentralWidget(tabWidget);

    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::updateUrlBar);

    QShortcut *newTabShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_T), this);
    connect(newTabShortcut, &QShortcut::activated, this, &MainWindow::createNewTab);

    QShortcut *closeTabShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_W), this);
    connect(closeTabShortcut, &QShortcut::activated, [this]() {
        closeTab(tabWidget->currentIndex());
    });

    QShortcut *refreshShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_R), this);
    connect(refreshShortcut, &QShortcut::activated, this, &MainWindow::refreshPage);

    for (int i = 1; i <= 9; ++i) {
        QShortcut *tabShortcut = new QShortcut(QKeySequence(Qt::CTRL | static_cast<Qt::Key>(Qt::Key_0 + i)), this);
        connect(tabShortcut, &QShortcut::activated, [this, i]() {
            int tabIndex = i - 1;
            if (i == 9) {
                if (tabWidget->count() > 0) {
                    tabWidget->setCurrentIndex(tabWidget->count() - 1);
                }
            } else if (tabIndex < tabWidget->count()) {
                tabWidget->setCurrentIndex(tabIndex);
            }
        });
    }
}

MainWindow::~MainWindow()
{
}

QWebEngineView* MainWindow::currentWebView()
{
    return qobject_cast<QWebEngineView*>(tabWidget->currentWidget());
}

void MainWindow::navigateToUrl()
{
    QString input = urlLineEdit->text().trimmed();

    QRegularExpression urlRegex(R"(^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$)");
    if (urlRegex.match(input).hasMatch()) {
        if (!input.startsWith("http://") && !input.startsWith("https://")) {
            input = "https://" + input;
        }
        if (currentWebView()) {
            currentWebView()->setUrl(QUrl(input));
        }
    } else {
        QString searchQuery = QUrl::toPercentEncoding(input);
        QString googleSearchUrl = "https://www.google.com/search?q=" + searchQuery;
        if (currentWebView()) {
            currentWebView()->setUrl(QUrl(googleSearchUrl));
        }
    }
}

void MainWindow::updateUrl(const QUrl &url)
{
    urlLineEdit->setText(url.toString());
}

void MainWindow::updateUrlBar(int index)
{
    if (index != -1) {
        QWebEngineView *webView = qobject_cast<QWebEngineView*>(tabWidget->widget(index));
        if (webView) {
            urlLineEdit->setText(webView->url().toString());
        }
    }
}

void MainWindow::createNewTab()
{
    QWebEngineView *webView = new QWebEngineView(this);
    webView->setUrl(QUrl("https://www.google.com"));
    int index = tabWidget->addTab(webView, "New Tab");
    tabWidget->setCurrentIndex(index);

    connect(webView, &QWebEngineView::urlChanged, [this, webView](const QUrl &url) {
        int index = tabWidget->indexOf(webView);
        if (index == tabWidget->currentIndex()) {
            urlLineEdit->setText(url.toString());
        }
    });

    connect(webView, &QWebEngineView::titleChanged, [this, webView](const QString &title) {
        int index = tabWidget->indexOf(webView);
        if (index != -1) {
            tabWidget->setTabText(index, title);
        }
    });
}

void MainWindow::closeTab(int index)
{
    QWidget *widget = tabWidget->widget(index);
    if (widget) {
        widget->deleteLater();
        tabWidget->removeTab(index);
    }
}

void MainWindow::goBack()
{
    if (currentWebView()) {
        currentWebView()->back();
    }
}

void MainWindow::goForward()
{
    if (currentWebView()) {
        currentWebView()->forward();
    }
}

void MainWindow::refreshPage()
{
    if (currentWebView()) {
        currentWebView()->reload();
    }
}

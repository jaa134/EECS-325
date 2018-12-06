/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QWidget *home;
    QPushButton *recordButton;
    QLabel *clientsLabel;
    QFrame *clientsGraph;
    QLabel *clientsMaxLine;
    QPushButton *reportButton;
    QLabel *durationLabel;
    QLabel *durationValue;
    QFrame *trafficGraph;
    QLabel *trafficMaxLine;
    QLabel *trafficLabel;
    QLabel *clientsMinValue;
    QLabel *clientsMaxValue;
    QLabel *trafficMinValue;
    QLabel *trafficMaxValue;
    QLabel *label_4;
    QWidget *transition;
    QLabel *transitionLabel;
    QProgressBar *progressBar;
    QWidget *report;
    QLabel *label_3;
    QFrame *clientsTrendGraph;
    QLabel *clientTrendMaxLine;
    QFrame *trafficTrendGraph;
    QLabel *trafficTrendMaxLine;
    QLabel *label_5;
    QLabel *label_2;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *clientDistinctClientsValue;
    QLabel *clientDistinctMacsValue;
    QLabel *clientDistinctIpsValue;
    QLabel *clientAverageValue;
    QLabel *clientNumPointsValue;
    QLabel *clientMaxValue;
    QLabel *clientMinValue;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_26;
    QLabel *trafficTcpPacketsValue;
    QLabel *trafficUdpPacketsValue;
    QLabel *trafficIpValue;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *trafficOtherPacketsValue;
    QLabel *trafficTcpLenValue;
    QLabel *trafficUdpLenValue;
    QWidget *error;
    QTextBrowser *textBrowser;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(420, 400);
        MainWindow->setMinimumSize(QSize(420, 400));
        MainWindow->setMaximumSize(QSize(420, 400));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 420, 400));
        home = new QWidget();
        home->setObjectName(QStringLiteral("home"));
        recordButton = new QPushButton(home);
        recordButton->setObjectName(QStringLiteral("recordButton"));
        recordButton->setGeometry(QRect(65, 350, 140, 35));
        QFont font;
        font.setPointSize(15);
        recordButton->setFont(font);
        clientsLabel = new QLabel(home);
        clientsLabel->setObjectName(QStringLiteral("clientsLabel"));
        clientsLabel->setGeometry(QRect(0, 5, 171, 40));
        QFont font1;
        font1.setPointSize(30);
        clientsLabel->setFont(font1);
        clientsLabel->setAlignment(Qt::AlignCenter);
        clientsGraph = new QFrame(home);
        clientsGraph->setObjectName(QStringLiteral("clientsGraph"));
        clientsGraph->setGeometry(QRect(40, 50, 375, 100));
        clientsGraph->setStyleSheet(QStringLiteral("background-color: #fff;"));
        clientsGraph->setFrameShape(QFrame::StyledPanel);
        clientsGraph->setFrameShadow(QFrame::Raised);
        clientsMaxLine = new QLabel(clientsGraph);
        clientsMaxLine->setObjectName(QStringLiteral("clientsMaxLine"));
        clientsMaxLine->setGeometry(QRect(1, 9, 373, 1));
        clientsMaxLine->setStyleSheet(QLatin1String("background-color: rgba(0, 0, 0, 0);\n"
"border: 1px dashed #000;"));
        reportButton = new QPushButton(home);
        reportButton->setObjectName(QStringLiteral("reportButton"));
        reportButton->setGeometry(QRect(215, 350, 140, 35));
        reportButton->setFont(font);
        durationLabel = new QLabel(home);
        durationLabel->setObjectName(QStringLiteral("durationLabel"));
        durationLabel->setGeometry(QRect(235, 25, 80, 20));
        QFont font2;
        font2.setPointSize(14);
        durationLabel->setFont(font2);
        durationValue = new QLabel(home);
        durationValue->setObjectName(QStringLiteral("durationValue"));
        durationValue->setGeometry(QRect(325, 25, 80, 20));
        durationValue->setFont(font2);
        durationValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficGraph = new QFrame(home);
        trafficGraph->setObjectName(QStringLiteral("trafficGraph"));
        trafficGraph->setGeometry(QRect(40, 230, 375, 100));
        trafficGraph->setStyleSheet(QStringLiteral("background-color: #fff;"));
        trafficGraph->setFrameShape(QFrame::StyledPanel);
        trafficGraph->setFrameShadow(QFrame::Raised);
        trafficMaxLine = new QLabel(trafficGraph);
        trafficMaxLine->setObjectName(QStringLiteral("trafficMaxLine"));
        trafficMaxLine->setGeometry(QRect(1, 9, 373, 1));
        trafficMaxLine->setStyleSheet(QLatin1String("background-color: rgba(0, 0, 0, 0);\n"
"border: 1px dashed #000;"));
        trafficLabel = new QLabel(home);
        trafficLabel->setObjectName(QStringLiteral("trafficLabel"));
        trafficLabel->setGeometry(QRect(0, 185, 171, 40));
        trafficLabel->setFont(font1);
        trafficLabel->setAlignment(Qt::AlignCenter);
        clientsMinValue = new QLabel(home);
        clientsMinValue->setObjectName(QStringLiteral("clientsMinValue"));
        clientsMinValue->setGeometry(QRect(5, 139, 30, 11));
        clientsMinValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clientsMaxValue = new QLabel(home);
        clientsMaxValue->setObjectName(QStringLiteral("clientsMaxValue"));
        clientsMaxValue->setGeometry(QRect(5, 49, 30, 20));
        clientsMaxValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficMinValue = new QLabel(home);
        trafficMinValue->setObjectName(QStringLiteral("trafficMinValue"));
        trafficMinValue->setGeometry(QRect(5, 319, 30, 11));
        trafficMinValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficMaxValue = new QLabel(home);
        trafficMaxValue->setObjectName(QStringLiteral("trafficMaxValue"));
        trafficMaxValue->setGeometry(QRect(5, 229, 30, 20));
        trafficMaxValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(home);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(170, 205, 140, 17));
        stackedWidget->addWidget(home);
        recordButton->raise();
        clientsLabel->raise();
        reportButton->raise();
        durationLabel->raise();
        durationValue->raise();
        trafficGraph->raise();
        trafficLabel->raise();
        clientsMinValue->raise();
        clientsMaxValue->raise();
        clientsGraph->raise();
        trafficMinValue->raise();
        trafficMaxValue->raise();
        label_4->raise();
        transition = new QWidget();
        transition->setObjectName(QStringLiteral("transition"));
        transitionLabel = new QLabel(transition);
        transitionLabel->setObjectName(QStringLiteral("transitionLabel"));
        transitionLabel->setGeometry(QRect(10, 130, 400, 50));
        QFont font3;
        font3.setPointSize(25);
        transitionLabel->setFont(font3);
        transitionLabel->setAlignment(Qt::AlignCenter);
        progressBar = new QProgressBar(transition);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(10, 200, 400, 30));
        QFont font4;
        font4.setPointSize(13);
        progressBar->setFont(font4);
        progressBar->setValue(0);
        stackedWidget->addWidget(transition);
        report = new QWidget();
        report->setObjectName(QStringLiteral("report"));
        label_3 = new QLabel(report);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(5, 295, 180, 20));
        QFont font5;
        font5.setPointSize(12);
        label_3->setFont(font5);
        clientsTrendGraph = new QFrame(report);
        clientsTrendGraph->setObjectName(QStringLiteral("clientsTrendGraph"));
        clientsTrendGraph->setGeometry(QRect(5, 205, 410, 80));
        clientsTrendGraph->setStyleSheet(QStringLiteral("background-color: #fff;"));
        clientsTrendGraph->setFrameShape(QFrame::StyledPanel);
        clientsTrendGraph->setFrameShadow(QFrame::Raised);
        clientTrendMaxLine = new QLabel(clientsTrendGraph);
        clientTrendMaxLine->setObjectName(QStringLiteral("clientTrendMaxLine"));
        clientTrendMaxLine->setGeometry(QRect(0, 7, 410, 1));
        clientTrendMaxLine->setStyleSheet(QLatin1String("background-color: rgba(0, 0, 0, 0);\n"
"border: 1px dashed #000;"));
        trafficTrendGraph = new QFrame(report);
        trafficTrendGraph->setObjectName(QStringLiteral("trafficTrendGraph"));
        trafficTrendGraph->setGeometry(QRect(5, 315, 410, 80));
        trafficTrendGraph->setStyleSheet(QStringLiteral("background-color: #fff;"));
        trafficTrendGraph->setFrameShape(QFrame::StyledPanel);
        trafficTrendGraph->setFrameShadow(QFrame::Raised);
        trafficTrendMaxLine = new QLabel(trafficTrendGraph);
        trafficTrendMaxLine->setObjectName(QStringLiteral("trafficTrendMaxLine"));
        trafficTrendMaxLine->setGeometry(QRect(0, 7, 410, 1));
        trafficTrendMaxLine->setStyleSheet(QLatin1String("background-color: rgba(0, 0, 0, 0);\n"
"border: 1px dashed #000;"));
        label_5 = new QLabel(report);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(5, 185, 180, 20));
        label_5->setFont(font5);
        label_2 = new QLabel(report);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(5, 35, 195, 20));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        label_6 = new QLabel(report);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(5, 55, 195, 20));
        label_6->setStyleSheet(QStringLiteral("background-color: rgb(211, 211, 211);"));
        label_8 = new QLabel(report);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(5, 95, 195, 20));
        label_8->setStyleSheet(QStringLiteral("background-color: rgb(211, 211, 211);"));
        label_9 = new QLabel(report);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(5, 75, 195, 20));
        label_9->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        label_10 = new QLabel(report);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(5, 5, 101, 25));
        QFont font6;
        font6.setPointSize(15);
        font6.setBold(true);
        font6.setWeight(75);
        label_10->setFont(font6);
        label_11 = new QLabel(report);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(220, 5, 161, 25));
        label_11->setFont(font6);
        label_12 = new QLabel(report);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(5, 115, 195, 20));
        label_12->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        label_13 = new QLabel(report);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(5, 135, 195, 20));
        label_13->setStyleSheet(QStringLiteral("background-color: rgb(211, 211, 211);"));
        label_14 = new QLabel(report);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(5, 155, 195, 20));
        label_14->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        clientDistinctClientsValue = new QLabel(report);
        clientDistinctClientsValue->setObjectName(QStringLiteral("clientDistinctClientsValue"));
        clientDistinctClientsValue->setGeometry(QRect(120, 155, 70, 20));
        clientDistinctClientsValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clientDistinctMacsValue = new QLabel(report);
        clientDistinctMacsValue->setObjectName(QStringLiteral("clientDistinctMacsValue"));
        clientDistinctMacsValue->setGeometry(QRect(120, 135, 70, 20));
        clientDistinctMacsValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clientDistinctIpsValue = new QLabel(report);
        clientDistinctIpsValue->setObjectName(QStringLiteral("clientDistinctIpsValue"));
        clientDistinctIpsValue->setGeometry(QRect(120, 115, 70, 20));
        clientDistinctIpsValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clientAverageValue = new QLabel(report);
        clientAverageValue->setObjectName(QStringLiteral("clientAverageValue"));
        clientAverageValue->setGeometry(QRect(120, 95, 70, 20));
        clientAverageValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clientNumPointsValue = new QLabel(report);
        clientNumPointsValue->setObjectName(QStringLiteral("clientNumPointsValue"));
        clientNumPointsValue->setGeometry(QRect(120, 75, 70, 20));
        clientNumPointsValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clientMaxValue = new QLabel(report);
        clientMaxValue->setObjectName(QStringLiteral("clientMaxValue"));
        clientMaxValue->setGeometry(QRect(120, 55, 70, 20));
        clientMaxValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clientMinValue = new QLabel(report);
        clientMinValue->setObjectName(QStringLiteral("clientMinValue"));
        clientMinValue->setGeometry(QRect(120, 35, 70, 20));
        clientMinValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(report);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(220, 35, 195, 20));
        label_15->setStyleSheet(QStringLiteral("background-color: rgb(211, 211, 211);"));
        label_16 = new QLabel(report);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(220, 55, 195, 20));
        label_16->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        label_17 = new QLabel(report);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(220, 135, 195, 20));
        label_17->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        label_26 = new QLabel(report);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(220, 155, 195, 20));
        label_26->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficTcpPacketsValue = new QLabel(report);
        trafficTcpPacketsValue->setObjectName(QStringLiteral("trafficTcpPacketsValue"));
        trafficTcpPacketsValue->setGeometry(QRect(325, 35, 80, 20));
        trafficTcpPacketsValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficUdpPacketsValue = new QLabel(report);
        trafficUdpPacketsValue->setObjectName(QStringLiteral("trafficUdpPacketsValue"));
        trafficUdpPacketsValue->setGeometry(QRect(325, 55, 80, 20));
        trafficUdpPacketsValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficIpValue = new QLabel(report);
        trafficIpValue->setObjectName(QStringLiteral("trafficIpValue"));
        trafficIpValue->setGeometry(QRect(280, 155, 125, 20));
        trafficIpValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_18 = new QLabel(report);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(220, 75, 195, 20));
        label_18->setStyleSheet(QStringLiteral("background-color: rgb(211, 211, 211);"));
        label_19 = new QLabel(report);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(220, 95, 195, 20));
        label_19->setStyleSheet(QStringLiteral("background-color: rgb(243, 243, 243);"));
        label_20 = new QLabel(report);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(220, 115, 195, 20));
        label_20->setStyleSheet(QStringLiteral("background-color: rgb(211, 211, 211);"));
        trafficOtherPacketsValue = new QLabel(report);
        trafficOtherPacketsValue->setObjectName(QStringLiteral("trafficOtherPacketsValue"));
        trafficOtherPacketsValue->setGeometry(QRect(325, 75, 80, 20));
        trafficOtherPacketsValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficTcpLenValue = new QLabel(report);
        trafficTcpLenValue->setObjectName(QStringLiteral("trafficTcpLenValue"));
        trafficTcpLenValue->setGeometry(QRect(325, 95, 80, 20));
        trafficTcpLenValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trafficUdpLenValue = new QLabel(report);
        trafficUdpLenValue->setObjectName(QStringLiteral("trafficUdpLenValue"));
        trafficUdpLenValue->setGeometry(QRect(325, 115, 80, 20));
        trafficUdpLenValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        stackedWidget->addWidget(report);
        label_3->raise();
        clientsTrendGraph->raise();
        trafficTrendGraph->raise();
        label_5->raise();
        label_2->raise();
        label_6->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();
        label_11->raise();
        label_12->raise();
        label_13->raise();
        label_14->raise();
        clientMinValue->raise();
        clientMaxValue->raise();
        clientNumPointsValue->raise();
        clientAverageValue->raise();
        clientDistinctIpsValue->raise();
        clientDistinctMacsValue->raise();
        clientDistinctClientsValue->raise();
        label_15->raise();
        label_16->raise();
        label_17->raise();
        label_26->raise();
        trafficTcpPacketsValue->raise();
        trafficUdpPacketsValue->raise();
        trafficIpValue->raise();
        label_18->raise();
        label_19->raise();
        label_20->raise();
        trafficOtherPacketsValue->raise();
        trafficTcpLenValue->raise();
        trafficUdpLenValue->raise();
        error = new QWidget();
        error->setObjectName(QStringLiteral("error"));
        textBrowser = new QTextBrowser(error);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 100, 400, 250));
        textBrowser->setFont(font2);
        label = new QLabel(error);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 40, 400, 51));
        label->setFont(font1);
        stackedWidget->addWidget(error);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Network Anlaysis", nullptr));
        recordButton->setText(QApplication::translate("MainWindow", "Record", nullptr));
        clientsLabel->setText(QApplication::translate("MainWindow", "CLIENTS", nullptr));
        clientsMaxLine->setText(QString());
        reportButton->setText(QApplication::translate("MainWindow", "Report", nullptr));
        durationLabel->setText(QApplication::translate("MainWindow", "Duration:", nullptr));
        durationValue->setText(QApplication::translate("MainWindow", "0s", nullptr));
        trafficMaxLine->setText(QString());
        trafficLabel->setText(QApplication::translate("MainWindow", "TRAFFIC", nullptr));
        clientsMinValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        clientsMaxValue->setText(QApplication::translate("MainWindow", "10", nullptr));
        trafficMinValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        trafficMaxValue->setText(QApplication::translate("MainWindow", "10", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "(packets per second)", nullptr));
        transitionLabel->setText(QApplication::translate("MainWindow", "Transition Message", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "IPv4 Traffic Trend", nullptr));
        clientTrendMaxLine->setText(QString());
        trafficTrendMaxLine->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "Client Connection Trend", nullptr));
#ifndef QT_NO_TOOLTIP
        label_2->setToolTip(QApplication::translate("MainWindow", "Minimum number of clients found", nullptr));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("MainWindow", "  Minimum:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_6->setToolTip(QApplication::translate("MainWindow", "Maximum number of clients found", nullptr));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("MainWindow", "  Maximum:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_8->setToolTip(QApplication::translate("MainWindow", "Average number of clients found", nullptr));
#endif // QT_NO_TOOLTIP
        label_8->setText(QApplication::translate("MainWindow", "  Average:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_9->setToolTip(QApplication::translate("MainWindow", "Number of data points collected", nullptr));
#endif // QT_NO_TOOLTIP
        label_9->setText(QApplication::translate("MainWindow", "  Data Points:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_10->setToolTip(QApplication::translate("MainWindow", "A client is defined as a MAC Address and IP Address pair", nullptr));
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("MainWindow", "Client Info", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Traffic Analysis", nullptr));
#ifndef QT_NO_TOOLTIP
        label_12->setToolTip(QApplication::translate("MainWindow", "Number of distinct IP Adresses found", nullptr));
#endif // QT_NO_TOOLTIP
        label_12->setText(QApplication::translate("MainWindow", "  Distinct IPs:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_13->setToolTip(QApplication::translate("MainWindow", "Number of distinct MAC Adresses found", nullptr));
#endif // QT_NO_TOOLTIP
        label_13->setText(QApplication::translate("MainWindow", "  Distinct MACs:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_14->setToolTip(QApplication::translate("MainWindow", "Number of distinct Clients found", nullptr));
#endif // QT_NO_TOOLTIP
        label_14->setText(QApplication::translate("MainWindow", "  Distinct Clients:", nullptr));
        clientDistinctClientsValue->setText(QApplication::translate("MainWindow", "0", nullptr));
#ifndef QT_NO_TOOLTIP
        clientDistinctMacsValue->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        clientDistinctMacsValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        clientDistinctIpsValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        clientAverageValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        clientNumPointsValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        clientMaxValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        clientMinValue->setText(QApplication::translate("MainWindow", "0", nullptr));
#ifndef QT_NO_TOOLTIP
        label_15->setToolTip(QApplication::translate("MainWindow", "Number of TCP packets captured", nullptr));
#endif // QT_NO_TOOLTIP
        label_15->setText(QApplication::translate("MainWindow", "  TCP:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_16->setToolTip(QApplication::translate("MainWindow", "Number of UDP packets captured", nullptr));
#endif // QT_NO_TOOLTIP
        label_16->setText(QApplication::translate("MainWindow", "  UDP:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_17->setToolTip(QApplication::translate("MainWindow", "The IP address asscoiated with the most packets", nullptr));
#endif // QT_NO_TOOLTIP
        label_17->setText(QApplication::translate("MainWindow", "  Most Packets by IP:", nullptr));
        label_26->setText(QString());
        trafficTcpPacketsValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        trafficUdpPacketsValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        trafficIpValue->setText(QApplication::translate("MainWindow", "255.255.255.255", nullptr));
#ifndef QT_NO_TOOLTIP
        label_18->setToolTip(QApplication::translate("MainWindow", "Number of Misc. packets captured", nullptr));
#endif // QT_NO_TOOLTIP
        label_18->setText(QApplication::translate("MainWindow", "  Other:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_19->setToolTip(QApplication::translate("MainWindow", "The average length of captured TCP packets", nullptr));
#endif // QT_NO_TOOLTIP
        label_19->setText(QApplication::translate("MainWindow", "  Avg TCP  Len:", nullptr));
#ifndef QT_NO_TOOLTIP
        label_20->setToolTip(QApplication::translate("MainWindow", "The average length of captured UDP packets", nullptr));
#endif // QT_NO_TOOLTIP
        label_20->setText(QApplication::translate("MainWindow", "  Avg UDP Len:", nullptr));
        trafficOtherPacketsValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        trafficTcpLenValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        trafficUdpLenValue->setText(QApplication::translate("MainWindow", "0", nullptr));
        label->setText(QApplication::translate("MainWindow", "System Error", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

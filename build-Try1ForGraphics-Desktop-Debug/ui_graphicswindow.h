/********************************************************************************
** Form generated from reading UI file 'graphicswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICSWINDOW_H
#define UI_GRAPHICSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphicsWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QSlider *verticalSlider;
    QVBoxLayout *verticalLayout_2;
    QGraphicsView *graphicsView;
    QPlainTextEdit *plainTextEdit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GraphicsWindow)
    {
        if (GraphicsWindow->objectName().isEmpty())
            GraphicsWindow->setObjectName(QString::fromUtf8("GraphicsWindow"));
        GraphicsWindow->resize(731, 235);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GraphicsWindow->sizePolicy().hasHeightForWidth());
        GraphicsWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(GraphicsWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        verticalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        verticalLayout->addWidget(pushButton_4);


        horizontalLayout->addLayout(verticalLayout);

        verticalSlider = new QSlider(centralwidget);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(verticalSlider);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);
        graphicsView->setMinimumSize(QSize(600, 100));
        graphicsView->setMaximumSize(QSize(1400, 1400));
        graphicsView->setLayoutDirection(Qt::LeftToRight);
        graphicsView->setAutoFillBackground(false);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        graphicsView->setResizeAnchor(QGraphicsView::NoAnchor);

        verticalLayout_2->addWidget(graphicsView);

        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        verticalLayout_2->addWidget(plainTextEdit);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        GraphicsWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(GraphicsWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        GraphicsWindow->setStatusBar(statusbar);

        retranslateUi(GraphicsWindow);

        QMetaObject::connectSlotsByName(GraphicsWindow);
    } // setupUi

    void retranslateUi(QMainWindow *GraphicsWindow)
    {
        GraphicsWindow->setWindowTitle(QApplication::translate("GraphicsWindow", "GraphicsWindow", nullptr));
        pushButton->setText(QApplication::translate("GraphicsWindow", "Cores", nullptr));
        pushButton_2->setText(QApplication::translate("GraphicsWindow", "RAM", nullptr));
        pushButton_3->setText(QApplication::translate("GraphicsWindow", "InTraffic", nullptr));
        pushButton_4->setText(QApplication::translate("GraphicsWindow", "Info", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphicsWindow: public Ui_GraphicsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICSWINDOW_H

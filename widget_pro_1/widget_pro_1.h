#pragma once

#include <QtWidgets/QWidget>
#include "ui_widget_pro_1.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <qevent.h>
#include <qcombobox.h>
#include <qtablewidget.h>
#include <qheaderview.h>
#include <qcolordialog.h>
#include <qcolor.h>
#include <qpalette.h>
#include <vector>
#include <qbrush.h>
#include <qstring.h>
#include <string>
#include <qboxlayout.h>
#include <qinputdialog.h>
#include <deque>
#include <qbrush.h>
#include <qpainter.h>
#include <qfile.h>
#include <qstringlist.h>
#include <qicon.h>

#include <qsystemtrayicon.h>
#include <qaction.h>
#include <qmenu.h>


//----------------------------------------------------------

class Content : public QWidget
{
    Q_OBJECT

private slots:
    void task_action(int);
    void add_task();
    void clear_this_action();

public:
    Content(QWidget* parent = nullptr);
    ~Content();
    QPushButton* add;
    QTableWidget* table;
    QGridLayout* grid;
    QPushButton* clear_this;
    QColorDialog* color_dialog;
    std::vector<QComboBox*> combobox_list;
    std::deque<QString> task;
    std::vector<QBrush> task_color_list;
    std::vector<QBrush> text_color_list;

private:
    Ui::widget_pro_1Class ui;
};

//----------------------------------------------------------

class widget_pro_1 : public QWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent* e) override;

private slots:
    void content_enable(int);
    void add_task_all_months();
    void clear_all_action();
    void close_in_tray();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

public:
    widget_pro_1(QWidget *parent = nullptr);
    ~widget_pro_1();
    QIcon* icon;
    QComboBox* months;
    QPushButton* add_all;
    QPushButton* clear_all;
    QVBoxLayout* main;
    QGridLayout* grid_head;
    std::vector<Content*> content_list;
    QInputDialog* input;
    //bool event(QEvent* e) override;
    bool yes_close;

    QSystemTrayIcon* tray_icon;

private:
    Ui::widget_pro_1Class ui;
};
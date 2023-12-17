#include "widget_pro_1.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

widget_pro_1::widget_pro_1(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->setStyleSheet("background-color:#3d404e; color: white; font-weight: bold;");

    yes_close = false;

    icon = new QIcon(QPixmap("pic/icon.ico"));

    setWindowIcon(*icon);

    main = new QVBoxLayout();

    grid_head = new QGridLayout();

    months = new QComboBox();
    months->addItem("jan");
    months->addItem("feb");
    months->addItem("mar");
    months->addItem("apr");
    months->addItem("may");
    months->addItem("jun");
    months->addItem("jul");
    months->addItem("aug");
    months->addItem("sep");
    months->addItem("oct");
    months->addItem("nov");
    months->addItem("dec");

    add_all = new QPushButton("++");
    add_all->setStyleSheet("QPushButton{background-color:#61677e; "
                                       //"border-style: outset; "
                                       //"border-width: 2px; "
                                       "border-radius: 10px; "
                                       //"border-color: black;"
                                       "font: bold 14px;"
                                       "min-width: 10em;"
                                       "padding: 6px; }"
                           "QPushButton:hover{background-color:#6d7fc1;}"
                           "QPushButton:pressed{background-color:#446cff; "
                                               "border - style: inset; }");
    clear_all = new QPushButton("Clear all");
    clear_all->setStyleSheet("QPushButton{background-color:#453638; "
                                         //"border-style: outset; "
                                         //"border-width: 2px; "
                                         "border-radius: 10px; "
                                         //"border-color: black;"
                                         "font: bold 14px;"
                                         "min-width: 10em;"
                                         "padding: 6px; }"
                             "QPushButton:hover{background-color:#643b40;}"
                             "QPushButton:pressed{background-color:#85222e; "
                                                 "border - style: inset; }");

    grid_head->addWidget(months, 0, 0);
    grid_head->addWidget(add_all, 0, 1);
    main->addLayout(grid_head);

    for (int i = 0; i < 12; i++) {
        content_list.push_back(new Content);
        content_list[i]->table->setVisible(false);
        content_list[i]->add->setVisible(false);
        content_list[i]->clear_this->setVisible(false);
        main->addLayout(content_list[i]->grid);
        connect(months, SIGNAL(activated(int)), this, SLOT(content_enable(int)));
    }

    auto now_date = to_simple_string(boost::gregorian::day_clock::local_day());
    auto month = QString(now_date.c_str()).sliced(5, 3).toLower();
    for (int i{}; i < 12; ++i) {
        if (months->itemText(i) == month) {
            months->setCurrentIndex(i);
            content_list[i]->table->setVisible(true);
            content_list[i]->add->setVisible(true);
            content_list[i]->clear_this->setVisible(true);
        }
    }

    main->addWidget(clear_all);
    setLayout(main);

    for (int i = 0; i < 12; i++) {
        QFile file("save/" + months->itemText(i) + ".txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        QString first_enter = in.readLine();
        QStringList task_in_file;
        QStringList colors_in_file;
        QStringList colors_in_file_for_text;
        if (!first_enter.isEmpty()) {
            task_in_file = first_enter.split(", ");
            colors_in_file = in.readLine().split(",");
            colors_in_file_for_text = in.readLine().split(",");
        }
        if (!task_in_file.isEmpty()){
            for (int task_index = 0; task_index < task_in_file.size(); task_index++) {
                bool ok;
                content_list[i]->combobox_list.push_back(new QComboBox());
                content_list[i]->combobox_list[content_list[i]->combobox_list.size() - 1]->addItem("Delete");
                content_list[i]->combobox_list[content_list[i]->combobox_list.size() - 1]->addItem("Color");
                content_list[i]->combobox_list[content_list[i]->combobox_list.size() - 1]->addItem("Text color");
                content_list[i]->table->setRowCount(task_in_file.size());
                content_list[i]->table->setItem(task_index, 0, new QTableWidgetItem(task_in_file[task_index]));
                content_list[i]->table->setCellWidget(task_index, 1, content_list[i]->combobox_list[task_index]);
                connect(content_list[i]->combobox_list[task_index], SIGNAL(activated(int)), content_list[i], SLOT(task_action(int)));
                content_list[i]->task_color_list.push_back(QBrush(QColor(colors_in_file[0].toInt(&ok), colors_in_file[1].toInt(&ok), colors_in_file[2].toInt(&ok))));
                content_list[i]->table->item(task_index, 0)->setBackground(content_list[i]->task_color_list[task_index]);
                content_list[i]->text_color_list.push_back(QBrush(QColor(colors_in_file_for_text[0].toInt(&ok), colors_in_file_for_text[1].toInt(&ok), colors_in_file_for_text[2].toInt(&ok))));
                content_list[i]->table->item(task_index, 0)->setForeground(content_list[i]->text_color_list[task_index]);
                colors_in_file.erase(colors_in_file.cbegin(), colors_in_file.cbegin() + 3);
                colors_in_file_for_text.erase(colors_in_file_for_text.cbegin(), colors_in_file_for_text.cbegin() + 3);
            }
        }
        file.close();
    }

    tray_icon = new QSystemTrayIcon(this);
    tray_icon->setIcon(*icon);

    QMenu* menu = new QMenu(this);
    QAction* view_window = new QAction("Open", this);
    QAction* quit_action = new QAction("Exit", this);

    connect(view_window, SIGNAL(triggered()), this, SLOT(show()));
    connect(quit_action, SIGNAL(triggered()), this, SLOT(close_in_tray()));

    menu->addAction(view_window);
    menu->addAction(quit_action);
    tray_icon->setContextMenu(menu);
    tray_icon->show();

    connect(tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    connect(add_all, SIGNAL(clicked()), this, SLOT(add_task_all_months()));
    connect(clear_all, SIGNAL(clicked()), this, SLOT(clear_all_action()));

}

void widget_pro_1::content_enable(int content_index) {
    for (int i = 0; i < 12; i++) {
        if (i != content_index) {
            content_list[i]->table->setVisible(false);
            content_list[i]->add->setVisible(false);
            content_list[i]->clear_this->setVisible(false);
        }
        else {
            content_list[i]->table->setVisible(true);
            content_list[i]->add->setVisible(true);
            content_list[i]->clear_this->setVisible(true);
        }
    }
}

void widget_pro_1::add_task_all_months() {
    bool make_task{ true };
    input = new QInputDialog();
    input->setLabelText("New task for all");
    QString value;
    int result = input->exec();
    if (result == QDialog::Accepted) {
        value = input->textValue();
    }
    else make_task = false;
    if (make_task){
        for (int i = 0; i < 12; i++) {
            content_list[i]->task.clear();
            
            for (int j = 0; j < content_list[i]->table->rowCount(); j++) {
                content_list[i]->task.push_back(content_list[i]->table->item(j, 0)->text());
            }
            content_list[i]->task.push_front(value);

            content_list[i]->combobox_list.push_back(new QComboBox());
            content_list[i]->combobox_list[content_list[i]->combobox_list.size() - 1]->addItem("Delete");
            content_list[i]->combobox_list[content_list[i]->combobox_list.size() - 1]->addItem("Color");
            content_list[i]->combobox_list[content_list[i]->combobox_list.size() - 1]->addItem("Text color");

            content_list[i]->table->setRowCount(content_list[i]->task.size());
            
            for (int f = 0; f < content_list[i]->task.size(); f++) {
                content_list[i]->table->setItem(f, 0, new QTableWidgetItem(content_list[i]->task[f]));
                content_list[i]->table->setCellWidget(f, 1, content_list[i]->combobox_list[f]);
                disconnect(content_list[i]->combobox_list[f], SIGNAL(activated(int)), content_list[i], SLOT(task_action(int)));
                connect(content_list[i]->combobox_list[f], SIGNAL(activated(int)), content_list[i], SLOT(task_action(int)));
                if (f == 0) {
                    content_list[i]->table->item(f, 0)->setBackground(QColor(20, 120, 0));
                    content_list[i]->table->item(f, 0)->setForeground(QColor(255, 255, 255));
                }
                else {
                    content_list[i]->table->item(f, 0)->setBackground(content_list[i]->task_color_list[f - 1]);
                    content_list[i]->table->item(f, 0)->setForeground(content_list[i]->text_color_list[f - 1]);
                }
            }
            content_list[i]->task_color_list.clear();
            for (int j = 0; j < content_list[i]->table->rowCount(); j++) {
                content_list[i]->task_color_list.push_back(content_list[i]->table->item(j, 0)->background());
                content_list[i]->text_color_list.push_back(content_list[i]->table->item(j, 0)->foreground());
            }
        }
    } 
}

void widget_pro_1::clear_all_action() {
    for (int i = 0; i < 12; i++){
        content_list[i]->task.clear();
        content_list[i]->task_color_list.clear();
        content_list[i]->text_color_list.clear();
        content_list[i]->table->setRowCount(0);
        content_list[i]->combobox_list.clear();
    }
}

void widget_pro_1::closeEvent(QCloseEvent* e) {
    for (int i = 0; i < 12; i++) {
        QFile file("save/" + months->itemText(i) + ".txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            continue;
        QTextStream out(&file);
        content_list[i]->task.clear();
        for (int j = 0; j < content_list[i]->table->rowCount(); j++) {
            if (!content_list[i]->table->item(j, 0)->text().isEmpty())
                content_list[i]->task.push_back(content_list[i]->table->item(j, 0)->text());
        }
        for (int task_index = 0; task_index < content_list[i]->task.size(); task_index++) {
            if (task_index != content_list[i]->task.size() - 1)
                out << content_list[i]->task[task_index] + ", ";
            else out << content_list[i]->task[task_index];
        }
        out << "\n";
        for (int color_index = 0; color_index < content_list[i]->task.size(); color_index++) {
            out << QString::number(content_list[i]->task_color_list[color_index].color().red()) + ",";
            out << QString::number(content_list[i]->task_color_list[color_index].color().green()) + ",";
            if (color_index != content_list[i]->task_color_list.size() - 1)
                out << QString::number(content_list[i]->task_color_list[color_index].color().blue()) + ",";
            else out << QString::number(content_list[i]->task_color_list[color_index].color().blue());
        }
        out << "\n";
        for (int color_index = 0; color_index < content_list[i]->task.size(); color_index++) {
            out << QString::number(content_list[i]->text_color_list[color_index].color().red()) + ",";
            out << QString::number(content_list[i]->text_color_list[color_index].color().green()) + ",";
            if (color_index != content_list[i]->text_color_list.size() - 1)
                out << QString::number(content_list[i]->text_color_list[color_index].color().blue()) + ",";
            else out << QString::number(content_list[i]->text_color_list[color_index].color().blue());
        }
        file.close();
    }

    if (yes_close) {
        e->accept();
        this->~widget_pro_1();
        tray_icon->~QSystemTrayIcon();
    }
        
    if (!yes_close) {
        e->ignore();
        this->hide();
    }
}

void widget_pro_1::close_in_tray() {

    yes_close = true;
    this->close();

}

void widget_pro_1::iconActivated(QSystemTrayIcon::ActivationReason reason) {

    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        if (!this->isVisible()) this->show();
        else this->hide();
        break;
    default:
        break;
    }

}

widget_pro_1::~widget_pro_1()
{
}

Content::Content(QWidget* parent)
    : QWidget(parent) 
{
    
    color_dialog = new QColorDialog();

    grid = new QGridLayout();
    add = new QPushButton("+");
    add->setStyleSheet("QPushButton{background-color:#61677e; "
                                   //"border-style: outset; "
                                   //"border-width: 2px; "
                                   "border-radius: 10px; "
                                   //"border-color: black;"
                                   "font: bold 14px;"
                                   "min-width: 10em;"
                                   "padding: 6px; }"
                       "QPushButton:hover{background-color:#6d7fc1;}"
                       "QPushButton:pressed{background-color:#446cff; "
                                           "border - style: inset; }");

    clear_this = new QPushButton("Clear this");
    clear_this->setStyleSheet("QPushButton{background-color:#453638; "
                                          //"border-style: outset; "
                                          //"border-width: 2px; "
                                          "border-radius: 10px; "
                                          //"border-color: black;"
                                          "font: bold 14px;"
                                          "min-width: 10em;"
                                          "padding: 6px; }"
                               "QPushButton:hover{background-color:#643b40;}"
                               "QPushButton:pressed{background-color:#85222e; "
                                                   "border - style: inset; }");

    table = new QTableWidget();
    table->setColumnCount(2);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->setHorizontalHeaderLabels(QStringList({ QString("TASK"), QString("EDIT") }));
    table->horizontalHeader()->setStyleSheet("font-size: 12pt; font-weight: bold;");

    table->setStyleSheet("QHeaderView::section {background-color:#484c5c }"
                         "QTableCornerButton::section{background-color: #484c5c}");

    grid->addWidget(add, 0, 1);
    grid->addWidget(table, 1, 0, 1, 2);
    grid->addWidget(clear_this, 2, 0, 1, 2);

    connect(add, SIGNAL(clicked()), this, SLOT(add_task()));
    connect(clear_this, SIGNAL(clicked()), this, SLOT(clear_this_action()));
}

Content::~Content()
{
}

void Content::add_task() {
    combobox_list.push_back(new QComboBox());
    table->setRowCount(table->rowCount() + 1);
    table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(""));
    table->setCellWidget(table->rowCount() - 1, 1, combobox_list[combobox_list.size() - 1]);
    combobox_list[combobox_list.size() - 1]->addItem("Delete");
    combobox_list[combobox_list.size() - 1]->addItem("Color");
    combobox_list[combobox_list.size() - 1]->addItem("Text color");
    connect(combobox_list[combobox_list.size() - 1], SIGNAL(activated(int)), this, SLOT(task_action(int)));
    task_color_list.push_back(QBrush(QColor(61, 64, 78)));
    text_color_list.push_back(QBrush(QColor(255, 255, 255)));
}

void Content::task_action(int combo_box_index) {
    for (int i = 0; i < combobox_list.size(); i++) {
        if (table->cellWidget(table->currentIndex().row(), table->currentIndex().column()) == combobox_list[i]) {
            if (combobox_list[i]->itemText(combo_box_index) == "Delete") {
                table->removeRow(table->currentIndex().row());
                combobox_list.erase(combobox_list.begin() + i);
            }
            else if (combobox_list[i]->itemText(combo_box_index) == "Color") {
                QColor color_buffer = color_dialog->getColor();
                table->item(table->currentIndex().row(), 0)->setBackground(color_buffer);
                task_color_list[table->currentIndex().row()] = QBrush(color_buffer);
            }
            else if (combobox_list[i]->itemText(combo_box_index) == "Text color") {
                QColor color_buffer_text = color_dialog->getColor();
                text_color_list[table->currentIndex().row()] = QBrush(color_buffer_text);
                table->item(table->currentIndex().row(), 0)->setForeground(color_buffer_text);
                qDebug() << text_color_list;
            }
        }
    }
}

void Content::clear_this_action() {
    task.clear();
    task_color_list.clear();
    text_color_list.clear();
    table->setRowCount(0);
    combobox_list.clear();
}
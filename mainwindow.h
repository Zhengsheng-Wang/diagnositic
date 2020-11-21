#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtwidgets>
#include "commander.h"

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   MainWindow();

   void decorate(); 
   void create_actions(); 
   //void create_toolbars();
   void create_manual();
   void create_auto();
   void create_query();


private slots:
    // 显示帮助信息
    void about();
    // 选择检测任务
    void choose_job();
    // 初始化时间间隔
    // 利用系统现有时间初始化
    void initial_interval(QComboBox*, QComboBox*, QComboBox*);

private:
    // 中枢
    Commander comm;

    QAction *about_action;
    QToolBar *auxiliary_toolbar;

    // 手动检测
    QGroupBox *manual_box;

    QLabel *manual_label;
    QLineEdit *manual_text;

    QPushButton *manual_browse;
    QPushButton *manual_confirm;
    // 自动检测
    QGroupBox *auto_box;

    // 卫星名
    QLabel *auto_sat_label;
    QLineEdit *auto_sat;
    // 起始时间
    QLabel *auto_start_time;
    // 年份
    QLabel *auto_start_year_label;
    QComboBox *auto_start_year;
    // 月份
    QLabel *auto_start_month_label;
    QComboBox *auto_start_month;
    // 日期
    QLabel *auto_start_date_label;
    QComboBox *auto_start_date;
    // 结束时间
    QLabel *auto_end_time; 
    // 年份
    QLabel *auto_end_year_label;
    QComboBox *auto_end_year;
    // 月份
    QLabel *auto_end_month_label;
    QComboBox *auto_end_month;
    // 日期
    QLabel *auto_end_date_label;
    QComboBox *auto_end_date;
    // 确定诊断时间
    QPushButton *auto_confirm_interval;
    
    // 显示一共需要检测的任务数
    QLabel *auto_nums_label;
    QLineEdit *auto_nums;
    // 确定
    QPushButton *auto_confirm;
    // 查询
    QGroupBox *query_box;
    // 卫星名
    QLabel *query_sat_label;
    QLineEdit *query_sat;
    // 起始时间
    QLabel *query_start_time;
    // 年份
    QLabel *query_start_year_label;
    QComboBox *query_start_year;
    // 月份
    QLabel *query_start_month_label;
    QComboBox *query_start_month;
    // 日期
    QLabel *query_start_date_label;
    QComboBox *query_start_date;
    // 结束时间
    QLabel *query_end_time; 
    // 年份
    QLabel *query_end_year_label;
    QComboBox *query_end_year;
    // 月份
    QLabel *query_end_month_label;
    QComboBox *query_end_month;
    // 日期
    QLabel *query_end_date_label;
    QComboBox *query_end_date;

    QPushButton *query_confirm;

    // 展示
    QGraphicsView *view;
    QGraphicsScene *scene;

private:
    QString job_path;
    // 根据年份、月份确定天数
    void set_days(QComboBox*, QComboBox*, QComboBox*);
};
//! [0]

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtwidgets>

#include "commander.h"
#include "diagram.h"

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
   MainWindow();

   void decorate(); 
   void create_actions(); 
    // Create manual operating console.
   void create_manual();
   // Create automatic operating console.
public:
   void create_auto();
   // Create queray diagnostic result console.
   void create_query();
   void create_system_viewer();
private:
    void show_CurDir();

private slots:
    // 显示帮助信息
    void about();
    // Choose a diagnostic task from local file system.
    void choose_job();
    void diagnose_tasks();
    // 初始化时间间隔
    // 利用系统现有时间初始化
    void initial_interval(QComboBox*, QComboBox*, QComboBox*);
    // Reset the view field of the QGraphicsView of diagnostic system.

private:
    // 中枢
    Commander comm;

    QAction *about_action;
    QToolBar *auxiliary_toolbar;

    /*
    Mannual diagnosis.
    We use a QLineEdit to input or choose a task file path.
    */
    QGroupBox *manual_box;
    // Task file path.
    QLabel *manual_label;
public:
    QLineEdit *manual_text;
private:
    QPushButton *manual_browse;
    QPushButton *manual_confirm;
    // Progress bar.
    QProgressBar *manual_progress_bar;

    /*
    In automatic diagnosis box, we use QComboBox choose the diagnosis time interval. 
    Diagnosis time interval is composed by a start time and a end time. When we confirm
    the time interval by click the "确认诊断时间" the number of tasks being diagnosed is displayed
    in the QLabel following "任务数" QLabel. Not until we clicke "开始自动诊断" QPushButton does the
    diagnosis process start.
    */
    QGroupBox *auto_box;
    // Start time.
    QLabel *auto_start_time;
    QLabel *auto_start_year_label;
    QComboBox *auto_start_year;
    QLabel *auto_start_month_label;
    QComboBox *auto_start_month;
    QLabel *auto_start_date_label;
    QComboBox *auto_start_date;
    // End time.
    QLabel *auto_end_time; 
    QLabel *auto_end_year_label;
    QComboBox *auto_end_year;
    QLabel *auto_end_month_label;
    QComboBox *auto_end_month;
    QLabel *auto_end_date_label;
    QComboBox *auto_end_date;
    // Comfirm button.
    QPushButton *auto_confirm_interval;
    // The number of tasks to be diagnosed.
    QLabel *auto_nums_label;
    QLineEdit *auto_nums;
    // Confirm button.
    QPushButton *auto_confirm;
    // Progress bar.
    QProgressBar *auto_progress_bar;

    /*
    Query diagnosis result stored in database.
    We choose the name of the queried satellite in a "QComboBox".
    Then we choose time interval using QComboBox and kick off the query by clicking a 
    QPushButton. Note the satellite name QComboBox and time QComboBox only show the information
    existing in database.
    */
    QGroupBox *query_box;
    // Name of satellite.
    QLabel *query_sat_label;
    QLineEdit *query_sat;
    // Start time.
    QLabel *query_start_time;
    QLabel *query_start_year_label;
    QComboBox *query_start_year;
    QLabel *query_start_month_label;
    QComboBox *query_start_month;
    QLabel *query_start_date_label;
    QComboBox *query_start_date;
    // End time. 
    QLabel *query_end_time; 
    QLabel *query_end_year_label;
    QComboBox *query_end_year;
    QLabel *query_end_month_label;
    QComboBox *query_end_month;
    QLabel *query_end_date_label;
    QComboBox *query_end_date;
    // Query confirm button.
    QPushButton *query_confirm;

    /*
    This box displays the profile of the receiving system in a QGraphicsView.
    We supply a button to reset the QGraphicsScene in the proper viewport position 
    of this QGraphicsView.
    */
    QGroupBox *viewer_box;
    Diagram *system;
    QPushButton *reset_scene;

private:
    // Diagnostic jobs' zip paths.
    QStringList diagnostic_jobPaths;
    // 根据年份、月份确定天数
    void set_days(QComboBox*, QComboBox*, QComboBox*);
};
//! [0]

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtwidgets>

#include "commander.h"
#include "diagram.h"

/*
"MainWindow" is the main window of this diagnostic application. It inherits "QMainWindow".

"MainWindow" is made up by 2 areas -- operational area and diaplay area.

The operational area is divided into 3 sections vertically which from top to the buttom are manual diagnostic, 
automatic diagnostic, diagnositic result query.  The display area is the diagram of satellite
data receiving system(System viewer).
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*
    Constructor. 

    Distribute widgets onto the "MainWindow" and build signal-slot connections.
    */
    MainWindow();

    /*
    Set outlooking of this software's window frame.
    */
    void decorate(); 
    void createActions(); 

    /*
    Set outlooking of manual diagnosis gauge.
    */
    void createManualDiagnosisGauge();
    /*
    Set outlooking of auto diagnosis gauge.
    */
    void createAutoDiagnosisGauge();
    /*
    Set outlooking of query diagnosis result gauge.
    */
    void createQueryDiagnosisResultGauge();
private slots:
    /*
    Show helping information.

    This function shows helping information by a "QMessageBox".
    */
    void about();

    /*
    Choose the zip path whose status parameters are used to diagnose.

    In the future we may develop a function to load task zip file from running log.)
    */
    void chooseJob();

    /*
    Diagnose a task indicated by "jobPath(Returned by chooseJob())".
    */
    QVector<QString> diagnose(const QString &jobPath);

    /*
    Initialize time interval.

    This function set the date "QComboBox"'s range according to current date.
    */
    void setEffectiveTimeInterval(QComboBox* years, QComboBox* months, QComboBox* dates);

    /*
    Determine the number of days of a specified month in a specified year.
    */
    void setNumberOfDaysInAMonth(QComboBox* years, QComboBox* months, QComboBox* dates);

private:
    // Outlooking widgets.
    QAction *aboutAction;
    QToolBar *auxiliaryToolbar;

    // Mannual diagnosis.
    // We use a QLineEdit to input or choose a task file path.
    QGroupBox *manualDiagnosisGauge;
    // Task file path.
    QLabel *manualChosenFileLabel;
    QLineEdit *manualChosenFilePath;
    QPushButton *manualBrowseFileButton;
    QPushButton *manualConfirmButton;

    // In automatic diagnosis box, we use QComboBox choose the diagnosis time interval. 
    // Diagnosis time interval is composed by a start time and a end time. When we confirm
    // the time interval by click the "确认诊断时间" the number of tasks being diagnosed is displayed
    // in the QLabel following "任务数" QLabel. Not until we clicke "开始自动诊断" QPushButton does the
    // diagnosis process start.
    QGroupBox *autoDiagnosisGauge;
    QLabel *autoDiagnosisStartTimeLabel;
    QLabel *autoDiagnosisStartYearLabel;
    QComboBox *autoDiagnosisStartYear;
    QLabel *autoDiagnosisStartMonthLabel;
    QComboBox *autoDiagnosisStartMonth;
    QLabel *autoDiagnosisStartDateLabel;
    QComboBox *autoDiagnosisStartDate;

    QLabel *autoDiagnosisEndTimeLabel; 
    QLabel *autoDiagnosisEndYearLabel;
    QComboBox *autoDiagnosisEndYear;
    QLabel *autoDiagnosisEndMonthLabel;
    QComboBox *autoDiagnosisEndMonth;
    QLabel *autoDiagnosisEndDateLabel;
    QComboBox *autoDiagnosisEndDate;

    QPushButton *autoConfirmEffectiveTimeButton;
    QLabel *autoJobsDiagnosedNumberLabel;
    QLineEdit *autoJobsDiagnosedNumber;
    QPushButton *autoStartDiagnoseButton;

    QLabel *autoDiagnosisProgressBarLabel;
    QProgressBar *autoDiagnosisProgressBar;

    // Query gauge.
    // Query diagnosis result stored in database.
    // We choose the name of the queried satellite in a "QComboBox".
    // Then we choose time interval using QComboBox and kick off the query by clicking a 
    // QPushButton. Note the satellite name QComboBox and time QComboBox only show the information
    // existing in database.
    QGroupBox *queryDiagnosisResultGauge;
    // Name of satellite.
    QLabel *querySatelliteNameLabel;
    QLineEdit *querySatelliteName;
    // Start time.
    QLabel *queryStartTimeLabel;
    QLabel *queryStartYearLabel;
    QComboBox *queryStartYear;
    QLabel *queryStartMonthLabel;
    QComboBox *queryStartMonth;
    QLabel *queryStartDateLabel;
    QComboBox *queryStartDate;
    // End time. 
    QLabel *queryEndTimeLabel; 
    QLabel *queryEndYearLabel;
    QComboBox *queryEndYear;
    QLabel *queryEndMonthLabel;
    QComboBox *queryEndMonth;
    QLabel *queryEndDateLabel;
    QComboBox *queryEndDate;
    // Query confirm button.
    QPushButton *queryConfirmButton;

    // Display area.
    // Render the abstract diagram of data satellite data receiving system.
    Diagram *diagram;
    QPushButton *resetSceneButton;
    QPushButton *selectTaskButton;

    // Diagnostic jobs' zip paths.
    QStringList pathsOfJobsDiagnosed;
};
//! [0]

#endif // MAINWINDOW_H

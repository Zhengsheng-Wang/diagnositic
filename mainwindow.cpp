#include <vector>
#include <algorithm>

#include <Python.h>

#include "mainwindow.h"
#include "element.h"

MainWindow::MainWindow(){
    decorate(); 
    createActions();


    createManualDiagnosisGauge();
    createAutoDiagnosisGauge();
    createQueryDiagnosisResultGauge();



    // Distribute widges.
    // Gauges.
    QVBoxLayout *gaugesLayout = new QVBoxLayout;

    gaugesLayout->addWidget(manualDiagnosisGauge);
    gaugesLayout->addWidget(autoDiagnosisGauge);
    gaugesLayout->addWidget(queryDiagnosisResultGauge);


    // Display area.
    QGridLayout *displayWidgetsLayout = new QGridLayout;

    diagram = new Diagram;
    selectTaskButton = new QPushButton(tr("选择展示任务"));
    resetSceneButton = new QPushButton(tr("重置显示区域"));

    displayWidgetsLayout->addWidget(diagram, 0, 0, 9, 3);
    displayWidgetsLayout->addWidget(selectTaskButton, 9, 0);
    displayWidgetsLayout->addWidget(resetSceneButton, 9, 2);


    // Ensemble gauge area and display area.
    QHBoxLayout *entireLayout = new QHBoxLayout;

    entireLayout->addLayout(gaugesLayout, 1);
    entireLayout->addLayout(displayWidgetsLayout, 2);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(entireLayout);
    setCentralWidget(centralWidget);



    // Add Signal&Slot.
    // Only one gauge can be activated each time.
    connect(manualDiagnosisGauge, &QGroupBox::clicked, this, [=]{
        autoDiagnosisGauge->setChecked(false);
        queryDiagnosisResultGauge->setChecked(false);
    });
    connect(autoDiagnosisGauge, &QGroupBox::clicked, this, [=]{
        manualDiagnosisGauge->setChecked(false);
        queryDiagnosisResultGauge->setChecked(false);
    });
    connect(queryDiagnosisResultGauge, &QGroupBox::clicked, this, [=]{
        autoDiagnosisGauge->setChecked(false);
        manualDiagnosisGauge->setChecked(false);
    });
    // Use crrent year and month to set initial effective receiving time 
    // interval for auto diagnosis or query.
    connect(autoDiagnosisGauge, &QGroupBox::clicked, this, [=]{
        setEffectiveTimeInterval(autoDiagnosisStartYear, autoDiagnosisStartMonth, autoDiagnosisStartDate);
        setEffectiveTimeInterval(autoDiagnosisEndYear, autoDiagnosisEndMonth, autoDiagnosisEndDate);
    });
    connect(queryDiagnosisResultGauge, &QGroupBox::clicked, this, [=]{
        setEffectiveTimeInterval(queryStartYear, queryStartMonth, queryStartDate);
        setEffectiveTimeInterval(queryEndYear, queryEndMonth, queryEndDate);
    });
    // Click "resetSceneButton" the system diagram is reset.
    connect(resetSceneButton, &QPushButton::clicked, diagram, &Diagram::resetSceneRect);
}

void MainWindow::decorate(){
    setWindowIcon(QIcon(":/images/icon.jpg"));
    setWindowTitle(tr("故障诊断系统"));
}

void MainWindow::createManualDiagnosisGauge(){
    // Gauge QGroupBox.
    manualDiagnosisGauge = new QGroupBox(tr("手动诊断"));
    manualDiagnosisGauge->setCheckable(true);


    // Distriute widgets.
    manualChosenFileLabel = new QLabel(tr("任务路径"));
    manualChosenFilePath = new QLineEdit;
    manualBrowseFileButton = new QPushButton(tr("浏览"));
    manualConfirmButton = new QPushButton(tr("确认"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(manualChosenFileLabel, 0, 0);
    layout->setColumnStretch(0, 1);
    layout->addWidget(manualChosenFilePath, 0, 1);
    layout->setColumnStretch(1, 5);
    layout->addWidget(manualBrowseFileButton, 0, 2);
    layout->setColumnStretch(2, 1);
    layout->addWidget(manualConfirmButton, 0, 3);
    layout->setColumnStretch(3, 1);
    manualDiagnosisGauge->setLayout(layout);

    connect(manualBrowseFileButton, &QPushButton::clicked, this, &MainWindow::chooseJob);
    connect(manualConfirmButton, &QPushButton::clicked, this, [=]()->void{
        if(pathsOfJobsDiagnosed.isEmpty()){
            QMessageBox msgBox;
            msgBox.setText(tr("没有任务压缩包可供诊断使用！"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;
        }
        diagnose(pathsOfJobsDiagnosed[0]);  // Diagnose the first task in the paths.
    });
}

void MainWindow::createAutoDiagnosisGauge(){
    // Gauge is a QGroupBox.
    autoDiagnosisGauge = new QGroupBox(tr("自动诊断"));
    autoDiagnosisGauge->setCheckable(true);
    autoDiagnosisGauge->setChecked(false);



    // Distribute widgets.
    QVBoxLayout *layout = new QVBoxLayout;


    // Effective time interval.
    // User can change the time by choosing year, month, date in QComboBox.  
    // These QComboBox can't be inserted to new value.
    autoDiagnosisStartTimeLabel = new QLabel(tr("起始时间："));
    autoDiagnosisStartYearLabel = new QLabel(tr("年份"));
    autoDiagnosisStartYear = new QComboBox;
    autoDiagnosisStartYear->setEditable(true);
    autoDiagnosisStartYear->setInsertPolicy(QComboBox::NoInsert);
    autoDiagnosisStartMonthLabel = new QLabel(tr("月份"));
    autoDiagnosisStartMonth = new QComboBox;
    autoDiagnosisStartMonth->setEditable(true);
    autoDiagnosisStartMonth->setInsertPolicy(QComboBox::NoInsert);
    autoDiagnosisStartDateLabel = new QLabel(tr("日期"));
    autoDiagnosisStartDate = new QComboBox;
    autoDiagnosisStartDate->setEditable(true);
    autoDiagnosisStartDate->setInsertPolicy(QComboBox::NoInsert);

    autoDiagnosisEndTimeLabel = new QLabel(tr("结束时间："));
    autoDiagnosisEndYearLabel = new QLabel(tr("年份"));
    autoDiagnosisEndYear = new QComboBox;
    autoDiagnosisEndYear->setEditable(true);
    autoDiagnosisEndYear->setInsertPolicy(QComboBox::NoInsert);
    autoDiagnosisEndMonthLabel = new QLabel(tr("月份"));
    autoDiagnosisEndMonth = new QComboBox;
    autoDiagnosisEndMonth->setEditable(true);
    autoDiagnosisEndMonth->setInsertPolicy(QComboBox::NoInsert);
    autoDiagnosisEndDateLabel = new QLabel(tr("日期"));
    autoDiagnosisEndDate = new QComboBox;
    autoDiagnosisEndDate->setEditable(true);
    autoDiagnosisEndDate->setInsertPolicy(QComboBox::NoInsert);

    autoConfirmEffectiveTimeButton = new QPushButton(tr("确认诊断时间"));

    QGridLayout *timeLayout = new QGridLayout;
    timeLayout->addWidget(autoDiagnosisStartTimeLabel, 0, 0);
    timeLayout->addWidget(autoDiagnosisStartYearLabel, 0, 1);
    timeLayout->addWidget(autoDiagnosisStartYear, 0, 2);
    timeLayout->addWidget(autoDiagnosisStartMonthLabel, 0, 3);
    timeLayout->addWidget(autoDiagnosisStartMonth, 0, 4);
    timeLayout->addWidget(autoDiagnosisStartDateLabel, 0, 5);
    timeLayout->addWidget(autoDiagnosisStartDate, 0, 6);

    timeLayout->addWidget(autoDiagnosisEndTimeLabel, 1, 0);
    timeLayout->addWidget(autoDiagnosisEndYearLabel, 1, 1);
    timeLayout->addWidget(autoDiagnosisEndYear, 1, 2);
    timeLayout->addWidget(autoDiagnosisEndMonthLabel, 1, 3);
    timeLayout->addWidget(autoDiagnosisEndMonth, 1, 4);
    timeLayout->addWidget(autoDiagnosisEndDateLabel, 1, 5);
    timeLayout->addWidget(autoDiagnosisEndDate, 1, 6);

    timeLayout->addWidget(autoConfirmEffectiveTimeButton, 2, 5, 1, 2);  // Span 1 row and 2 columns.


    // The number of tasks to be diagnosed.
    autoJobsDiagnosedNumberLabel = new QLabel(tr("任务数"));
    autoJobsDiagnosedNumber = new QLineEdit;

    QHBoxLayout *numberOfJobsDiagnosedLayout = new QHBoxLayout;
    numberOfJobsDiagnosedLayout->addWidget(autoJobsDiagnosedNumberLabel);
    numberOfJobsDiagnosedLayout->setStretch(0, 1);
    numberOfJobsDiagnosedLayout->addWidget(autoJobsDiagnosedNumber);
    numberOfJobsDiagnosedLayout->setStretch(1, 2);


    // Start diagnosis button.
    autoStartDiagnoseButton = new QPushButton(tr("开始自动诊断"));

    QHBoxLayout *startingDiagnosisButtonLayout = new QHBoxLayout;
    startingDiagnosisButtonLayout->addWidget(autoStartDiagnoseButton, 0, Qt::AlignRight);


    // Diagnosis progress bar.
    // Determine the maximum of progress bar by the number of paths of tasks to be diagnosed.
    autoDiagnosisProgressBarLabel = new QLabel(tr("进度"));
    autoDiagnosisProgressBar = new QProgressBar;
    autoDiagnosisProgressBar->setTextVisible(true);

    QHBoxLayout *progressBarLayout = new QHBoxLayout;
    progressBarLayout->addWidget(autoDiagnosisProgressBarLabel);
    progressBarLayout->addWidget(autoDiagnosisProgressBar);


    // Ensemble the gauge.
    QVBoxLayout *gaugeLayout = new QVBoxLayout;
    gaugeLayout->addLayout(timeLayout);
    gaugeLayout->addLayout(numberOfJobsDiagnosedLayout);
    gaugeLayout->addLayout(startingDiagnosisButtonLayout);
    gaugeLayout->addLayout(progressBarLayout);

    autoDiagnosisGauge->setLayout(gaugeLayout);



    // Add Slot&Signal.
    // When current year or month is changed the date range in QComboBox "autoDiagnosisStartDate" must be adjusted accordingly.
    connect(autoDiagnosisStartYear, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(autoDiagnosisStartYear, autoDiagnosisStartMonth, autoDiagnosisStartDate);
    });
    connect(autoDiagnosisStartMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(autoDiagnosisStartYear, autoDiagnosisStartMonth, autoDiagnosisStartDate);
    });
    connect(autoDiagnosisEndYear, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(autoDiagnosisEndYear, autoDiagnosisEndMonth, autoDiagnosisEndDate);
    });
    connect(autoDiagnosisEndMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(autoDiagnosisEndYear, autoDiagnosisEndMonth, autoDiagnosisEndDate);
    });
}

void MainWindow::createQueryDiagnosisResultGauge(){
    // Gauge is a QGroupBox.
    queryDiagnosisResultGauge = new QGroupBox(tr("查询"));
    queryDiagnosisResultGauge->setCheckable(true);
    queryDiagnosisResultGauge->setChecked(false);



    // Distribure widgets.
    // The name of indicated satellite.
    querySatelliteNameLabel = new QLabel(tr("卫星名"));
    querySatelliteName = new QLineEdit;

    QHBoxLayout *satelliteNameLayout = new QHBoxLayout;
    satelliteNameLayout->addWidget(querySatelliteNameLabel, 1);
    satelliteNameLayout->addWidget(querySatelliteName, 2);


    // Effective time interval.
    // User can change the time by choosing year, month, date in QComboBox.  
    // These QComboBox can't be inserted to new value.
    queryStartTimeLabel = new QLabel(tr("起始时间："));
    queryStartYearLabel = new QLabel(tr("年份"));
    queryStartYear = new QComboBox;
    queryStartYear->setEditable(true);
    queryStartYear->setInsertPolicy(QComboBox::NoInsert);
    queryStartMonthLabel = new QLabel(tr("月份"));
    queryStartMonth = new QComboBox;
    queryStartMonth->setEditable(true);
    queryStartMonth->setInsertPolicy(QComboBox::NoInsert);
    queryStartDateLabel = new QLabel(tr("日期"));
    queryStartDate = new QComboBox;
    queryStartDate->setEditable(true);
    queryStartDate->setInsertPolicy(QComboBox::NoInsert);

    queryEndTimeLabel = new QLabel(tr("结束时间："));
    queryEndYearLabel = new QLabel(tr("年份"));
    queryEndYear = new QComboBox;
    queryEndYear->setEditable(true);
    queryEndYear->setInsertPolicy(QComboBox::NoInsert);
    queryEndMonthLabel = new QLabel(tr("月份"));
    queryEndMonth = new QComboBox;
    queryEndMonth->setEditable(true);
    queryEndMonth->setInsertPolicy(QComboBox::NoInsert);
    queryEndDateLabel = new QLabel(tr("日期"));
    queryEndDate = new QComboBox;
    queryEndDate->setEditable(true);
    queryEndDate->setInsertPolicy(QComboBox::NoInsert);

    queryConfirmButton = new QPushButton(tr("确定"));

    QGridLayout *timeLayout = new QGridLayout;

    timeLayout->addWidget(queryStartTimeLabel, 0, 0);
    timeLayout->addWidget(queryStartYearLabel, 0, 1);
    timeLayout->addWidget(queryStartYear, 0, 2);
    timeLayout->addWidget(queryStartMonthLabel, 0, 3);
    timeLayout->addWidget(queryStartMonth, 0, 4);
    timeLayout->addWidget(queryStartDateLabel, 0, 5);
    timeLayout->addWidget(queryStartDate, 0, 6);

    timeLayout->addWidget(queryEndTimeLabel, 1, 0);
    timeLayout->addWidget(queryEndYearLabel, 1, 1);
    timeLayout->addWidget(queryEndYear, 1, 2);
    timeLayout->addWidget(queryEndMonthLabel, 1, 3);
    timeLayout->addWidget(queryEndMonth, 1, 4);
    timeLayout->addWidget(queryEndDateLabel, 1, 5);
    timeLayout->addWidget(queryEndDate, 1, 6);

    timeLayout->addWidget(queryConfirmButton, 2, 6); 


    // Ensemble gauge.
    QVBoxLayout *entireLayout = new QVBoxLayout;
    entireLayout->addLayout(satelliteNameLayout);
    entireLayout->addLayout(timeLayout);

    queryDiagnosisResultGauge->setLayout(entireLayout);



    // Add Signal&Slot.
    // When current year or month is changed the date range in QComboBox "autoDiagnosisStartDate" must be adjusted accordingly.
    connect(queryStartYear, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(queryStartYear, queryStartMonth, queryStartDate);
    });
    connect(queryStartMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(queryStartYear, queryStartMonth, queryStartDate);
    });
    connect(queryEndYear, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(queryEndYear, queryEndMonth, queryEndDate);
    });
    connect(queryEndMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        setNumberOfDaysInAMonth(queryEndYear, queryEndMonth, queryEndDate);
    });
}

void MainWindow::chooseJob(){
    // Pop up a QFileDialog to let user choose zip files.
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile); 
    fileDialog.setNameFilter(tr("任务压缩包 (*.zip)"));
    fileDialog.setViewMode(QFileDialog::Detail);

    if(fileDialog.exec()){
        pathsOfJobsDiagnosed = fileDialog.selectedFiles();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText(tr("接收任务压缩包载入失败！"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

QVector<QString> MainWindow::diagnose(const QString &jobPath){
    // Import diagnostic module.
    // We first try to load this module from memory.  
    // If it has not been in the memory(hasn't been imported), we import it.
    PyObject *pyModuleName = PyUnicode_DecodeFSDefault("call_DiagnosticModel");
    PyObject *pyModule = PyImport_GetModule(pyModuleName);  // Load from memory.
    if(pyModule == NULL){
        pyModule = PyImport_Import(pyModuleName);
    }
    Py_DECREF(pyModuleName);
    if(pyModule == NULL){
        QMessageBox msgBox;
        msgBox.setText(tr("诊断功能模块导入失败！"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return QVector<QString>(0);  // No valid diagnostic result.
    }

    // Get function.
    PyObject *pyDiagnose = PyObject_GetAttrString(pyModule, "call_model");
    if(pyDiagnose == NULL){
        QMessageBox msgBox;
        msgBox.setText(tr("Python函数提取失败！"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        Py_DECREF(pyModule);
        return QVector<QString>(0);  // No valid diagnostic result.
    }

    // Get Diagnostic result.
    std::string stdTaskPath = jobPath.toStdString();
    PyObject *pyJobPath = PyUnicode_FromString(stdTaskPath.c_str());
    if(pyJobPath == NULL){
        QMessageBox msgBox;
        msgBox.setText(tr("任务压缩包路径获取失败！"));
        msgBox.setInformativeText(tr("任务路径：") + jobPath);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        Py_DECREF(pyModule);
        Py_DECREF(pyDiagnose);
        return QVector<QString>(0);  // No valid diagnostic result.
    }

    // Call python diagnostic program.
    PyObject *pyArgs = PyTuple_New(1);
    PyTuple_SetItem(pyArgs, 0, pyJobPath);  // Note PyTuple_SetItem() takes over ownership so no need in decrementing pyJobPath.
    PyObject *pyFaultSPUs = PyObject_CallObject(pyDiagnose, pyArgs);
    Py_DECREF(pyArgs);
    if(pyFaultSPUs == NULL){
        QMessageBox msgBox;
        msgBox.setText(tr("诊断失败！"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        Py_DECREF(pyModule);
        Py_DECREF(pyDiagnose);
        Py_DECREF(pyJobPath);
        return QVector<QString>(0);
    }

    // Extract fault SPUs' names from python diagnositic function return value.
    QVector<QString> faultSPUs;
    for(int i = 0; i < PyList_Size(pyFaultSPUs); ++i){
        faultSPUs.append(QString::fromWCharArray(PyUnicode_AsWideCharString(PyList_GetItem(pyFaultSPUs, i), NULL)));
    }
    Py_DECREF(pyModule);
    Py_DECREF(pyDiagnose);
    Py_DECREF(pyJobPath);
    Py_DECREF(pyFaultSPUs);
    return faultSPUs;
}

void MainWindow::about(){
    QMessageBox::about(this, tr("故障诊断"), tr("中国遥感卫星地面站数据接收系统故障诊断系统"));
}

void MainWindow::setEffectiveTimeInterval(QComboBox *years, QComboBox *months, QComboBox *dates){
    // Retrieve current dates since we will use current date as the initial date shown in QComboBoxs.
    QDate currentDate = QDate::currentDate();


    // Clear all existing items in years, months, datess QComboBoxs.
    years->clear(); months->clear(); dates->clear();


    // Set year, month, date range of QComboBox.
    // Add last 20-year-items in years QComboBox.
    for(int i = currentDate.year(); i >= currentDate.year() - 19; --i){
        years->addItem(QString::number(i));
    }

    // Show months.
    for(int i = 1; i <= 12; ++i){
        months->addItem(QString::number(i));
    }
    months->setCurrentIndex(currentDate.month() - 1);  // Show current month.

    // Show dates.
    // For the reason that not every months has same number of days, we 
    // need to determine the number of days shown corresponding to each 
    // months dynamically.
    QCalendar calendar;
    for(int i = 1; i <= calendar.daysInMonth(currentDate.month(), currentDate.year()); ++i){
        dates->addItem(QString::number(i));
    }
    dates->setCurrentIndex(currentDate.day(calendar) - 1);
}

void MainWindow::setNumberOfDaysInAMonth(QComboBox *years, QComboBox *months, QComboBox *dates){
    // Determine the date range shown by "dates" QComboBox according to the year and 
    // month shown by "years" and "months".
    dates->clear();
    QCalendar calendar;
    int year = QString(years->currentText()).toInt();
    int month = QString(months->currentText()).toInt();
    int numberOfDays = calendar.daysInMonth(month, year);
    for(int i = 1; i <= numberOfDays; ++i){
        dates->addItem(QString::number(i));
    }
}

void MainWindow::createActions(){
    aboutAction = new QAction(tr("关于"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}
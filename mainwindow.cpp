#include "mainwindow.h"
#include <vector>
#include <algorithm>

MainWindow::MainWindow(){
    // 装饰
    decorate(); 
    // 工具栏
    create_actions();
    // 手动检测
    create_manual();
    // 自动检测
    create_auto();
    // 查询
    create_query();
    // 其中一个功能模块激活的同时，另2个模块变暗
    connect(manual_box, &QGroupBox::clicked, this, [=]{
        auto_box->setChecked(false);
        query_box->setChecked(false);
    });
    connect(auto_box, &QGroupBox::clicked, this, [=]{
        manual_box->setChecked(false);
        query_box->setChecked(false);
    });
    connect(query_box, &QGroupBox::clicked, this, [=]{
        auto_box->setChecked(false);
        manual_box->setChecked(false);
    });
    // 需要设置时间间隔的模块在第一次被激活的时候初始化间隔时间
    connect(auto_box, &QGroupBox::clicked, this, [=]{
        initial_interval(auto_start_year, auto_start_month, auto_start_date);
        initial_interval(auto_end_year, auto_end_month, auto_end_date);
    });
    connect(query_box, &QGroupBox::clicked, this, [=]{
        initial_interval(query_start_year, query_start_month, auto_start_date);
        initial_interval(query_end_year, query_end_month, auto_end_date);
    });
    // 展示窗口
    scene = new QGraphicsScene(this);
    scene->setSceneRect(QRectF(0, 0, 8000, 5000));
    view = new QGraphicsView(scene);

    // 设置配置文件夹
    comm.set_config_dir();
    // 读入已检测的卫星，查询部分需要用来建立list
    comm.read_sat_names();


    // 将界面的中心区域设计成左右结构
    QHBoxLayout *layout_h = new QHBoxLayout;
    //
    QVBoxLayout *layout_v = new QVBoxLayout;
    layout_v->addWidget(manual_box);
    layout_v->addWidget(auto_box);
    layout_v->addWidget(query_box);
    layout_h->addLayout(layout_v, 1);
    layout_h->addWidget(view, 2);

    // 中心控件为水平布局
    QWidget *central_widget = new QWidget;
    central_widget->setLayout(layout_h);
    setCentralWidget(central_widget);
}
// 装饰界面
void MainWindow::decorate(){
    setWindowIcon(QIcon(":/images/icon.jpg"));
    setWindowTitle(tr("故障诊断系统"));
}
// 创建工具按钮
void MainWindow::create_actions(){
    about_action = new QAction(tr("关于"), this);
    connect(about_action, &QAction::triggered, this, &MainWindow::about);
}
// 手动检测工具箱
void MainWindow::create_manual(){
    // 包含：标签、一行文本（显示任务路径）、浏览按钮
    manual_box = new QGroupBox(tr("手动检测"));
    manual_box->setCheckable(true);

    QGridLayout *layout = new QGridLayout;
    // 标签
    manual_label = new QLabel(tr("任务路径"));
    layout->addWidget(manual_label, 0, 0);
    layout->setColumnStretch(0, 1);
    // 文本
    manual_text = new QLineEdit;
    layout->addWidget(manual_text, 0, 1);
    layout->setColumnStretch(1, 5);
    // 浏览按钮
    manual_browse = new QPushButton(tr("浏览"));
    layout->addWidget(manual_browse, 0, 2);
    layout->setColumnStretch(2, 1);
    connect(manual_browse, &QPushButton::clicked, this, &MainWindow::choose_job);
    // 确认按钮，通过手动输入任务文件路径打开任务
    manual_confirm = new QPushButton(tr("确认"));
    layout->addWidget(manual_confirm, 0, 3);
    layout->setColumnStretch(3, 1);
    //connect(manual_confirm, &QPushButton::clicked, this, &MainWindow::);

    manual_box->setLayout(layout);
}
// 自动检测工具箱
void MainWindow::create_auto(){
    // 待检测任务数、检测结果保存路径
    auto_box = new QGroupBox(tr("自动检测"));
    auto_box->setCheckable(true);
    auto_box->setChecked(false);

    QVBoxLayout *layout_v = new QVBoxLayout;

    // 检测时间间隔
    QGridLayout *grid_interval= new QGridLayout;
    // 起始时间
    auto_start_time = new QLabel(tr("起始时间: "));
    grid_interval->addWidget(auto_start_time, 0, 0);
    // 年份
    auto_start_year_label = new QLabel(tr("年份"));
    grid_interval->addWidget(auto_start_year_label, 0, 1);
    auto_start_year = new QComboBox;
    auto_start_year->setEditable(true);
    auto_start_year->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(auto_start_year, 0, 2);

    grid_interval->setColumnStretch(2, 2);
    // 月份
    auto_start_month_label = new QLabel(tr("月份"));
    grid_interval->addWidget(auto_start_month_label, 0, 3);
    auto_start_month = new QComboBox;
    auto_start_month->setEditable(true);
    auto_start_month->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(auto_start_month, 0, 4);

    grid_interval->setColumnStretch(4, 1);
    // 日期
    auto_start_date_label = new QLabel(tr("日期"));
    grid_interval->addWidget(auto_start_date_label, 0, 5);
    auto_start_date = new QComboBox;
    auto_start_date->setEditable(true);
    auto_start_date->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(auto_start_date, 0, 6);
    // 调整可选择的日期
    connect(auto_start_year, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(auto_start_year, auto_start_month, auto_start_date);
    });
    connect(auto_start_month, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(auto_start_year, auto_start_month, auto_start_date);
    });

    grid_interval->setColumnStretch(6, 1);
    // 结束时间
    auto_end_time = new QLabel(tr("结束时间: "));
    grid_interval->addWidget(auto_end_time, 1, 0);
    // 年份
    auto_end_year_label = new QLabel(tr("年份"));
    grid_interval->addWidget(auto_end_year_label, 1, 1);
    auto_end_year = new QComboBox;
    auto_end_year->setEditable(true);
    auto_end_year->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(auto_end_year, 1, 2);
    // 月份
    auto_end_month_label = new QLabel(tr("月份"));
    grid_interval->addWidget(auto_end_month_label, 1, 3);
    auto_end_month = new QComboBox;
    auto_end_month->setEditable(true);
    auto_end_month->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(auto_end_month, 1, 4);
    // 日期
    auto_end_date_label = new QLabel(tr("日期"));
    grid_interval->addWidget(auto_end_date_label, 1, 5);
    auto_end_date = new QComboBox;
    auto_end_date->setEditable(true);
    auto_end_date->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(auto_end_date);
    // 调整天数
    connect(auto_end_year, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(auto_end_year, auto_end_month, auto_end_date);
    });
    connect(auto_end_month, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(auto_end_year, auto_end_month, auto_end_date);
    });

    layout_v->addLayout(grid_interval);
    // 确定诊断时间
    QHBoxLayout *layout_h_confirm_interval = new QHBoxLayout;
    auto_confirm_interval = new QPushButton(tr("确认诊断时间"));
    layout_h_confirm_interval->addWidget(auto_confirm_interval, 0, Qt::AlignRight);

    layout_v->addLayout(layout_h_confirm_interval);

    // 任务数显示
    QHBoxLayout *layout_h_nums = new QHBoxLayout;

    auto_nums_label = new QLabel(tr("任务数"));
    layout_h_nums->addWidget(auto_nums_label);
    layout_h_nums->setStretch(0, 1);
    auto_nums = new QLineEdit;
    layout_h_nums->addWidget(auto_nums);
    layout_h_nums->setStretch(1, 1);

    layout_h_nums->addStretch(2);

    // 确认按钮
    layout_v->addLayout(layout_h_nums);

    QHBoxLayout *layout_h_confirm = new QHBoxLayout;
    auto_confirm = new QPushButton(tr("开始自动诊断"));
    layout_h_confirm->addWidget(auto_confirm, 0, Qt::AlignRight);

    layout_v->addLayout(layout_h_confirm);

    // 总体布局
    auto_box->setLayout(layout_v);
}
// 查询
void MainWindow::create_query(){
    // 待查卫星、开始时间、结束时间
    query_box = new QGroupBox(tr("查询"));
    query_box->setCheckable(true);
    query_box->setChecked(false);

    QVBoxLayout *layout_v = new QVBoxLayout;
    // 卫星
    QHBoxLayout *layout_h_sat = new QHBoxLayout;
    query_sat_label = new QLabel(tr("卫星名"));
    layout_h_sat->addWidget(query_sat_label, 0);
    layout_h_sat->setStretch(0, 1);

    query_sat = new QLineEdit;
    layout_h_sat->addWidget(query_sat, 1);
    layout_h_sat->setStretch(1, 2);

    // 控制标签和文本行的占比
    layout_h_sat->addStretch(2);
    // 查询时间间隔
    QGridLayout *grid_interval= new QGridLayout;
    // 起始时间
    query_start_time = new QLabel(tr("起始时间: "));
    grid_interval->addWidget(query_start_time, 0, 0);
    // 年份
    query_start_year_label = new QLabel(tr("年份"));
    grid_interval->addWidget(query_start_year_label, 0, 1);
    query_start_year = new QComboBox;
    query_start_year->setEditable(true);
    query_start_year->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(query_start_year, 0, 2);

    grid_interval->setColumnStretch(2, 2);
    // 月份
    query_start_month_label = new QLabel(tr("月份"));
    grid_interval->addWidget(query_start_month_label, 0, 3);
    query_start_month = new QComboBox;
    query_start_month->setEditable(true);
    query_start_month->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(query_start_month, 0, 4);

    grid_interval->setColumnStretch(4, 1);
    // 日期
    query_start_date_label = new QLabel(tr("日期"));
    grid_interval->addWidget(query_start_date_label, 0, 5);
    query_start_date = new QComboBox;
    query_start_date->setEditable(true);
    query_start_date->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(query_start_date, 0, 6);
    // 调整天数
    connect(query_start_year, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(query_start_year, query_start_month, query_start_date);
    });
    connect(query_start_month, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(query_start_year, query_start_month, query_start_date);
    });

    grid_interval->setColumnStretch(6, 1);
    // 结束时间
    query_end_time = new QLabel(tr("结束时间: "));
    grid_interval->addWidget(query_end_time, 1, 0);
    // 年份
    query_end_year_label = new QLabel(tr("年份"));
    grid_interval->addWidget(query_end_year_label, 1, 1);
    query_end_year = new QComboBox;
    query_end_year->setEditable(true);
    query_end_year->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(query_end_year, 1, 2);
    // 月份
    query_end_month_label = new QLabel(tr("月份"));
    grid_interval->addWidget(query_end_month_label, 1, 3);
    query_end_month = new QComboBox;
    query_end_month->setEditable(true);
    query_end_month->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(query_end_month, 1, 4);
    // 日期
    query_end_date_label = new QLabel(tr("日期"));
    grid_interval->addWidget(query_end_date_label, 1, 5);
    query_end_date = new QComboBox;
    query_end_date->setEditable(true);
    query_end_date->setInsertPolicy(QComboBox::NoInsert);
    grid_interval->addWidget(query_end_date);
    // 调整天数
    connect(query_start_year, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(query_start_year, query_start_month, query_start_date);
    });
    connect(query_start_month, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        set_days(query_start_year, query_start_month, query_start_date);
    });

    // 确定
    query_confirm = new QPushButton(tr("确定"));
    QHBoxLayout *layout_h_confirm = new QHBoxLayout;
    layout_h_confirm->addWidget(query_confirm, 0, Qt::AlignRight);

    layout_v->addLayout(layout_h_sat);
    layout_v->addLayout(grid_interval);
    layout_v->addLayout(layout_h_confirm);

    query_box->setLayout(layout_v);
}

void MainWindow::choose_job(){
    QFileDialog d(this);
    d.setFileMode(QFileDialog::ExistingFile); 
    d.setNameFilter(tr("任务压缩文件 (*.zip)"));
    d.setViewMode(QFileDialog::Detail);
    d.setDirectory(tr("D:\\workspace\\project_station_code\\zips"));
    if(d.exec()){
        job_path = d.selectedFiles()[0];  //存储任务文件路径
    }
    manual_text->setText(job_path);  //显示选择的任务文件路径
}
void MainWindow::about(){
    QMessageBox::about(this, tr("故障诊断"), tr("中国遥感卫星地面站数据接收系统故障诊断系统"));
}
void MainWindow::initial_interval(QComboBox *y, QComboBox *m, QComboBox *d){
    // 已被初始化，直接返回
    if(y->count() > 0){
        return;
    }
    // 获取当前时间
    QDate current_date = QDate::currentDate();
    // 在ComboBox里面只显示前20年的年份
    for(int i = current_date.year(); i >= current_date.year() - 19; --i){
        y->addItem(QString::number(i));
    }
    // 显示1至12月
    for(int i = 1; i <= 12; ++i){
        m->addItem(QString::number(i));
    }
    // 显示1至31号
    for(int i = 1; i <= 31; ++i){
        d->addItem(QString::number(i));
    }
}
void MainWindow::set_days(QComboBox *y, QComboBox *m, QComboBox *d){
    // 清空日期
    d->clear();
    // 确定天数
    QCalendar c;
    int year = QString(y->currentText()).toInt();
    int month = QString(m->currentText()).toInt();
    int days = c.daysInMonth(month, year);
    // 添加日期
    for(int i = 1; i <= days; ++i){
        d->addItem(QString::number(i));
    }
}
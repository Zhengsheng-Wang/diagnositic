#ifndef COMMANDER_H
#define COMMANDER_H

#include <QtCore>

struct Commander{
    // 当前文件夹
    QDir cur_dir = QDir::current();
    // 配置文件夹
    QDir config_dir;
    // 卫星
    QStringList sat_names;

    // 设置配置文件夹
    void set_config_dir(const QString& d="config");
    void scan_sat_names();
    void read_sat_names();

    // Run Python code.
    void run_PythonCode();
private:
    void add_sat_names(const QStringList&);
};
#endif
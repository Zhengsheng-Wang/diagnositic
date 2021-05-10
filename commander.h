#ifndef COMMANDER_H
#define COMMANDER_H

#include <QtCore>

struct Commander{
    // 当前文件夹
    QDir currentDirectory = QDir::current();
    // 配置文件夹
    QDir configurationDirectory;
    // 卫星
    QStringList satelliteNames;

    // 设置配置文件夹
    void setConfigurationDirectory(const QString& d="config");
    void scanSatelliteNames();
    void readSatelliteNames();

    // Run Python code.
    void runPythonCode();
private:
    void addSatelliteNames(const QStringList&);
};
#endif
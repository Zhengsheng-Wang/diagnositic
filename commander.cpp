#include "commander.h"

void Commander::setConfigurationDirectory(const QString& configurationDirectoryName){
    /*
    Set configuration directory to store configuration information.
    
    Side effects.
    Build configuration directory "configrationDirectoryName" under the current working directory.
    */
    currentDirectory.mkdir(configurationDirectoryName);
    configurationDirectory = QDir(currentDirectory);
    configurationDirectory.cd(configurationDirectoryName);
}

void Commander::addSatelliteNames(const QStringList& names){
    /*
    */
}

void Commander::readSatelliteNames(){
    if(!satelliteNames.isEmpty()) return;

    // 读入卫星数量
    quint32 num;
    QFile file(configurationDirectory.filePath("sats_num"));
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> num;
    // 读入卫星名称
    // 按照卫星数量迭代读入卫星名
    QFile file1(configurationDirectory.filePath("satellites"));
    file1.open(QIODevice::ReadOnly);
    QDataStream in1(&file1);
    QString sat;
    for(quint32 i = 0; i < num; ++i){
        in1 >> sat;
        sat_names.append(sat);
    }
}

void Commander::runPythonCode(){
}
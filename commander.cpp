#include "commander.h"

void Commander::set_config_dir(const QString& d){
    // 若已经存在配置文件夹，则直接使用
    cur_dir.mkdir(d);
    config_dir = QDir(cur_dir);
    config_dir.cd(d);
}
// 添加卫星名称
void Commander::add_sat_names(const QStringList& names){
}
// 读入卫星名称，存入名称链表
void Commander::read_sat_names(){
    // 卫星是否已加载直接退出
    if(! sat_names.isEmpty()) return;
    // 读入卫星数量
    quint32 num;
    QFile file(config_dir.filePath("sats_num"));
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> num;
    // 读入卫星名称
    // 按照卫星数量迭代读入卫星名
    QFile file1(config_dir.filePath("satellites"));
    file1.open(QIODevice::ReadOnly);
    QDataStream in1(&file1);
    QString sat;
    for(quint32 i = 0; i < num; ++i){
        in1 >> sat;
        sat_names.append(sat);
    }
}

// ![]
void Commander::run_PythonCode(){
}
// ![]
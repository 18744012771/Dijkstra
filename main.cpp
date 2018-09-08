#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QSet>
#include "graph_dg.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QString s=QString("D:\\Documents\\Visual Studio 2013\\Projects\\day09\\Dijkstra\\map.txt");
    Graph_DG g(&w,s);
    g.print();
    g.createGraph();
    g.print();
//    g.Dijkstra(5);
//    g.print_path(5);
    g.computeAll();
    g.print_paths();
    return a.exec();
}

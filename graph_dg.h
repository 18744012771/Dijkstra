#ifndef GRAPH_DG_H
#define GRAPH_DG_H

#include <QObject>
#include <iostream>
#include <QFile>

#include <QDebug>
#include <QSet>
#include <QList>
#include <QVector>

struct Edge {
   Edge(int x1,int y1,float w1):x(x1),y(y1),w(w1){}
   int x;
   int y;
   float w;
} ;

/*
本程序是使用Dijkstra算法实现求解最短路径的问题
采用的邻接矩阵来存储图
*/
//记录起点到每个顶点的最短路径的信息
struct Dis {
    int start;
    int end;
    QString path;
    int value;
    bool visit;
    Dis() {
        visit = false;
        value = 0;
        path = "";
    }
};

struct Path{
    int start;
    int end;
    float w;
    QList<int> points;
    //Dis * dis;
};


class Graph_DG : public QObject
{
    Q_OBJECT
public:
      Graph_DG(QObject *parent  ,QString );
    //构造函数
   // Graph_DG(QObject *parent  ,int vexnum, int edge);
    //析构函数
    ~Graph_DG();
    // 判断我们每次输入的的边的信息是否合法
    //顶点从1开始编号
    bool check_edge_value(int start, int end, int weight);
    //创建图
    void createGraph();
    //打印邻接矩阵
    void print();
    //求最短路径
    void Dijkstra(int begin);
    //打印最短路径
    void print_path(int);
    void computeAll();

    void loaddMap(QString);

    void print_paths();
signals:

public slots:
private:
    int vexnum;   //图的顶点个数
    int edgenum;     //图的边数
    int **arc;   //邻接矩阵
    Dis * dis;   //记录各个顶点最短路径的信息
    QFile *file;
    QSet<int > points;
    QList<Edge> edges;
    QVector<QVector<Path>> paths;



};

#endif // GRAPH_DG_H

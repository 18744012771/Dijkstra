#include "graph_dg.h"
#include <qDebug>
Graph_DG::Graph_DG(QObject *parent,QString s="D:\\Documents\\Visual Studio 2013\\Projects\\day09\\Dijkstra\\map.txt" ) : QObject(parent)
{
    loaddMap(s);
    //初始化顶点数和边数
    qDebug()<<edgenum;
    //为邻接矩阵开辟空间和赋初值
    arc = new int*[this->vexnum];
    dis = new Dis[this->vexnum];
    for (int i = 0; i < this->vexnum; i++) {
        arc[i] = new int[this->vexnum];
        for (int k = 0; k < this->vexnum; k++) {
            //邻接矩阵初始化为无穷大
            arc[i][k] = INT_MAX;
        }
    }
}
////构造函数
//Graph_DG::Graph_DG(QObject *parent,int vexnum, int edgenum) : QObject(parent){

//    loaddMap();
//    //初始化顶点数和边数
//    this->vexnum = vexnum;
//    this->edgenum = edgenum;
//    //为邻接矩阵开辟空间和赋初值
//    arc = new int*[this->vexnum];
//    dis = new Dis[this->vexnum];
//    for (int i = 0; i < this->vexnum; i++) {
//        arc[i] = new int[this->vexnum];
//        for (int k = 0; k < this->vexnum; k++) {
//            //邻接矩阵初始化为无穷大
//            arc[i][k] = INT_MAX;
//        }
//    }
//}

//析构函数
Graph_DG::~Graph_DG() {
    delete[] dis;
    for (int i = 0; i < this->vexnum; i++) {
        delete this->arc[i];
    }
    delete arc;
}


// 判断我们每次输入的的边的信息是否合法
//顶点从1开始编号
bool Graph_DG::check_edge_value(int start, int end, int weight) {
    if (start<1 || end<1 || start>vexnum || end>vexnum || weight < 0) {
        return false;
    }
    return true;
}


void Graph_DG::createGraph() {
    qDebug() << "请输入每条边的起点和终点（顶点编号从1开始）以及其权重" ;
    int start;
    int end;
    int weight;
    int count = 0;


    //读map文件
    while (count != this->edgenum) {
        Edge edge=edges.front();
        edges.pop_front();

        //cin >> start >> end >> weight;
        //首先判断边的信息是否合法
        while (!this->check_edge_value(edge.x, edge.y, edge.w)) {
           qDebug() << "输入的边的信息不合法，请重新输入" ;
            return ;//cin >> start >> end >> weight;
        }
        //对邻接矩阵对应上的点赋值
        arc[edge.x - 1][edge.y - 1] = edge.w;
        //无向图添加上这行代码
        arc[edge.y - 1][edge.x - 1] = edge.w;
        ++count;
    }
}


void Graph_DG::print() {
    qDebug() << "图的邻接矩阵为：" ;
    int count_row = 0; //打印行的标签
    int count_col = 0; //打印列的标签
    //开始打印
    QList<QString> list;
    while (count_row != this->vexnum) {
        count_col = 0;
        while (count_col != this->vexnum) {
            if (arc[count_row][count_col] == INT_MAX)
                list << "∞" ;
            else
            {
                int k =arc[count_row][count_col];
                list << QString::number(k)  ;
            }

            ++count_col;
        }
        qDebug()<<list;
        list.clear();
        ++count_row;
    }
}

void Graph_DG::loaddMap(QString s){
    file = new QFile(s);
    if(file->open(QIODevice::ReadOnly | QIODevice::Text))
    {


        while(file->atEnd()==false)
        {
           QByteArray ba = file->readLine();
           QString s(ba);
           int index=s.section(" ",0,0).toInt();
           int x=s.section(" ",1,1).toInt();
           int y=s.section(" ",2,2).toInt();
           float w=s.section(" ",3,3).toFloat();
           qDebug()<<x<<y<<w;
           points.insert(x);
           points.insert(y);
           Edge edge(index,x,y,w);
           edges.append(edge);
           hash[QPair<int,int>(x,y)]=index;
           hash[QPair<int,int>(y,x)]=index;
           edgeCount[index]=0;

        }
        vexnum = points.size();

        edgenum= edges.size();
    }
    file->close();
}

void Graph_DG::Dijkstra(int begin){

    QVector<Path> vec;

    //首先初始化我们的dis数组
    int i;
    for (i = 0; i < this->vexnum; i++) {
        Path p;

        p.start=begin;
        p.end=i + 1;
        p.points<<begin<<i+1;

        //设置当前的路径
        dis[i].start=begin;
        dis[i].path = "v" + QString::number(begin) + "-->v" + QString::number(i + 1);
        dis[i].value = arc[begin - 1][i];
        p.w = arc[begin - 1][i];
       // qDebug()<<"初始化"<<dis[i].path;

        vec.push_back(p);
     }
    //设置起点的到起点的路径为0
    dis[begin - 1].value = 0;
    dis[begin - 1].visit = true;
    vec[begin - 1].w = 0;

    int count = 1;
    //计算剩余的顶点的最短路径（剩余this->vexnum-1个顶点）
    while (count != this->vexnum) {
        //temp用于保存当前dis数组中最小的那个下标
        //min记录的当前的最小值
        int temp = 0;
        int min = INT_MAX;
        for (i = 0; i < this->vexnum; i++) {
            if (!dis[i].visit && dis[i].value<min) {
                min = dis[i].value;
                temp = i;
            }
        }
        //cout << temp + 1 << "  "<<min << endl;
        //把temp对应的顶点加入到已经找到的最短路径的集合中
        dis[temp].visit = true;
        ++count;
        for (i = 0; i < this->vexnum; i++) {
            //注意这里的条件arc[temp][i]!=INT_MAX必须加，不然会出现溢出，从而造成程序异常
            if (!dis[i].visit && arc[temp][i] != INT_MAX && (dis[temp].value + arc[temp][i]) < dis[i].value) {
                //如果新得到的边可以影响其他为访问的顶点，那就就更新它的最短路径和长度
                dis[i].value = dis[temp].value + arc[temp][i];
                dis[i].path = dis[temp].path + "-->v" + QString::number(i + 1);

                vec[i].points=vec[temp].points;
                vec[i].points<<i+1;
                vec[i].w=vec[temp].w+arc[temp][i];

            }

            //qDebug()<<"最短路径"<<dis[i].path;

        }

 }

    paths.push_back(vec);

}






void Graph_DG::print_paths(){
    for(auto it=paths.begin();it!=paths.end();it++){
        for (int i=0;i<this->vexnum;i++){
            int start=(*it)[i].start;
            int end=(*it)[i].end;
            QList<int> points=(*it)[i].points;
            float w=(*it)[i].w;
            qDebug()<< start<< end<< points<< w;
            for(auto it=(points.constBegin()++);it!=points.constEnd();it++)
            {
                QPair<int,int>pair=QPair<int,int>(start,*it);
                if(hash.find(pair)!=hash.constEnd())
                    edgeCount[hash[pair]]++;
                //if()
            }
        }



    }
    for(auto it=edgeCount.constBegin();it!=edgeCount.constEnd();it++)
        qDebug()<<it.key()<<""<<it.value();

}

void Graph_DG::computeAll(){
    for(auto it = points.begin();it!=points.end();it++)
    {
         Dijkstra(*it);

         delete dis;
         dis = new Dis[this->vexnum];
    }

}

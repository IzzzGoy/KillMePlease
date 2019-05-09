#ifndef TABLE_H
#define TABLE_H
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <vector>


using namespace std;




class Table
{
private:
    QPixmap wall;
    QPixmap food;
    QPixmap no_food;
    QPixmap GG;
    QPixmap grey;
    QPixmap pinky;
    QPixmap blu;
    vector<QGraphicsPixmapItem*> players;
public:
    Table();
    void Drow(short int* grid,vector<double*> X,vector<double*> Y,QGraphicsScene* scene);
};

#endif // TABLE_H

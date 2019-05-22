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
    vector<QPixmap> playersTexture;
    vector<QGraphicsPixmapItem*> players;
    vector<QGraphicsPixmapItem*> table;
public:
    ~Table();
    Table();
    void Drow(short int* grid, vector<double*> X, vector<double*> Y, QGraphicsScene* scene, int id);
};

#endif // TABLE_H

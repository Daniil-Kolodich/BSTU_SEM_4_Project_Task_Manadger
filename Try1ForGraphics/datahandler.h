#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QPainter>
class DataHandler{
protected:
    const int max_size = 60;
    const int min_size = 10;
    float** data = nullptr;
    int size_of_data = 10;
    int dimensions = 3;
    int amountOfCores = 0;
    int current_dimension = 0;
public slots:
    void SecondPartOfGettingData ();
public:
    DataHandler();
    ~DataHandler();
    // todo
    void DrawBackgroundMarkup(QPainter *painter,int _x_start,int _y_start,int _width,int _height,int _columns,int _rows, int _offset, bool _is_CPU_Cores = false);
    void SetCpuGradient(QPainter *painter,QPointF _start,QPointF _end);
    void SetOptimalSizeForTable(int *_width,int *_height);
    void GraphDrawer(QPainter *painter);
//    void SetRamGradient(QPainter *painter,QPointF _start,QPointF _end);
//    void SetNetGradient(QPainter *painter,QPointF _start,QPointF _end);

    // ~~ done ~~
    void SetCurrentDimension(int value);
    void PrepareData();
    void FirstPartOfGettingData();
    void DataDrawer(QPainter *painter,QRect rect);
    void ExpandDataSet(int percent);
    void SetAmountOfCores();
    void SetPainterForCPUGraph(QPainter *painter);
    void SetPainterForRAMGraph(QPainter *painter);
};

#endif // DATAHANDLER_H

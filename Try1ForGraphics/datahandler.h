#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QPainter>
#include "enumForData.h"
class DataHandler{
protected:
    const int max_size = 240;
    const int min_size = 30;
    float** data = nullptr;
    int size_of_data = 10;
    int dimensions = 3;
    int amountOfCores = 0;
    int current_dimension = CPU;
    int cpu_grid_sizes[2] {4,4};
    double width_shrinkage = 0.1;
    double height_shrinkage = 0;
    QRect rect;

public:
//    void SetSizes(int width,int height);


    DataHandler();
    ~DataHandler();
    // todo
    void DrawCPUCoresInfo(QPainter *painter);
    void DrawBackgroundMarkup(QPainter *painter,int _columns,int _rows);
    void SetCpuGradient(QPainter *painter,QPointF _start,QPointF _end);
    void SetOptimalSizeForTable();
    void GraphDrawer(QPainter *painter, bool _isOutOfSize = false);
    void FirstPartOfDataUpdate();
    void SecondPartOfDataUpdate();
    // ~~ done ~~
    void SetCurrentDimension(int value);
    void PrepareData();
    void UpdateData();
    void DataDrawer(QPainter *painter,QRect rect);
    void ExpandDataSet(int percent);
    void SetAmountOfCores();
//    void SetPainterForCPUGraph(QPainter *painter);
//    void SetPainterForRAMGraph(QPainter *painter);
};

#endif // DATAHANDLER_H

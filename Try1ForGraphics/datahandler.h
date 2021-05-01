#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QPainter>
#include "enumForData.h"
class DataHandler{
protected:
    const int max_size = 240;
    const int min_size = 60;
    float** data = nullptr;
    int size_of_data = 10;
    int dimensions = 3;
    int amountOfCores = 0;
    int current_dimension = _CPU;
    int cpu_grid_sizes[2] {4,4};
    double width_shrinkage = 0.2;
    double height_shrinkage = 0;
    QRect rect;

public:
    DataHandler();
    ~DataHandler();
    // todo
    void SetCpuPainterForGraph(QPainter *painter);
    void SetRamPainterForGraph(QPainter *painter);
    void SetInTrafficPainterForGraph(QPainter *painter);
    void SetOutTrafficPainterForGraph(QPainter *painter);
    int GetMaxValueForGraph();
    double GetCoefForGraph();

    void SetCpuGradient(QPainter *painter,QPointF _start,QPointF _end);
    void GraphDrawer(QPainter *painter);
    // ~~ done ~~
    void DrawCPUCoresInfo(QPainter *painter);
    void DrawBackgroundMarkup(QPainter *painter,int _columns,int _rows);
    void SetOptimalSizeForTable();
    void FirstPartOfDataUpdate();
    void SecondPartOfDataUpdate();
    void SetCurrentDimension(int value);
    void PrepareData();
    void UpdateData();
    void DataDrawer(QPainter *painter,QRect rect);
    void ExpandDataSet(int percent);
    void SetAmountOfCores();
};

#endif // DATAHANDLER_H

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QPainter>
#include "enumForData.h"
#include <QColor>
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
    double width_shrinkage = 0.1;
//    double height_shrinkage =;
    QRect rect;
    // ram & swap
    float max_memory_values[2];
    // colors
    QColor colors[5];

public:
    DataHandler();
    ~DataHandler();
    // todo
    float GetMaxValueForGraph();
    double GetCoefForGraph(float max_value);
    void PresetColors();
    void SetColor(QPainter *painter);

    // drawer
    void Drawer(QPainter* painter,QRectF rect);
    void DrawGraph(QPainter* painter,QRectF rect,double coef);
    void DrawLoadBar(QPainter* painter,QRectF rect,float load_height);
    void DrawLoadMarkup(QPainter *painter,QRectF rect,int amount_of_marks);
    void DrawLoadMarkupText(QPainter *painter,QRectF rect,int amount_of_marks,float max_value);
    void DrawLoadText(QPainter *painter,QRectF rect,double coef);


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
    void SetSystemValues();
};

#endif // DATAHANDLER_H

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "clickhousehandler.h"
#include "enumForData.h"
#include <QPainter>
#include <fstream>

class DataHandler{
public:
    DataHandler();
    ~DataHandler();

    float GetMaxValueForGraph();
    double GetCoefForGraph(float max_value);
    void PresetColors();
    void SetColor(QPainter *painter);

    void DataDrawer(QPainter *painter,QRectF rect);
    void Drawer(QPainter* painter,QRectF rect);
    void DrawGraph(QPainter* painter,QRectF rect,double coef);
    void DrawGraphName(QPainter* painter,QRectF rect);
    void DrawLoadBar(QPainter* painter,QRectF rect,float load_height);
    void DrawLoadMarkup(QPainter *painter,QRectF rect,int amount_of_marks);
    void DrawLoadMarkupText(QPainter *painter,QRectF rect,int amount_of_marks,float max_value);
    void DrawLoadText(QPainter *painter,QRectF rect,double coef);
    void DrawCPUCoresInfo(QPainter *painter,QRectF rect);



    QString GetInfoText();

    void SetCurrentDimension(int value);

    void PrepareData();
    void SetSystemValues();
    void SetOptimalSizeForTable();
    void FirstPartOfDataUpdate();
    void SecondPartOfDataUpdate();
    void UpdateData();
    void ExpandDataSet(int percent);

private:
    ClickhouseHandler *db;
    const int max_size = 240;
    const int min_size = 60;
    float** data = nullptr;
    int size_of_data = 10;
    int dimensions = 3;
    int amountOfCores = 0;
    int current_dimension = _CPU;
    int cpu_grid_sizes[2] {4,4};
    float max_memory_values[2];
    QColor colors[5];
    const QString str_to_append[6]{
        "RAM",
        "SWAP",
        "InTraffic",
        "OutTraffic",
        "CPU",
        "Core[%1]"
    };
};

#endif // DATAHANDLER_H

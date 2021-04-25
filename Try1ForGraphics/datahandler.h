#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QPainter>
class DataHandler{
protected:
    const int max_size = 240;
    const int min_size = 10;
    int** data = nullptr;
    int size_of_data = 10;
    int dimensions = 3;
    int amountOfCores = 0;
    int current_dimension = 0;
public:
    DataHandler();
    DataHandler(int size_of_data,int dimensions);
    ~DataHandler();
    void SetCurrentDimension(int value);
    void PrepareData();
    void GetNewData();
    void DataDrawer(QPainter *painter,QRect rect);
    void ExpandDataSet(int percent);
    void SetAmountOfCores();
};

#endif // DATAHANDLER_H

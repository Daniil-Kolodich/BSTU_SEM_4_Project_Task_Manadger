#include "datahandler.h"
#include <QDebug>
#include <cmath>
#include <QPen>
#include <stdio.h>
#include <iostream>
#include <fstream>
DataHandler::DataHandler(){
    // ez
    SetAmountOfCores ();
    size_of_data = min_size;
    // + 3 cause totalCpu / RAM / (Wi-Fi * 2)
    this->dimensions = amountOfCores + 4;
    SetOptimalSizeForTable ();
    PrepareData ();
}

void DataHandler::SetAmountOfCores (){
    // get amount of cores & print to file
    system("nproc > amountOfCores.txt");
    // get data from file
    FILE* cores = fopen("amountOfCores.txt","r");
    fscanf(cores,"%d",&this->amountOfCores);
    fclose(cores);
    // delete file
    system ("rm amountOfCores.txt");
}

DataHandler::~DataHandler(){
    // deleting all the data that r used
    for (int j = 0 ; j < dimensions; j++)
        delete[] data[j];
    delete[] data;
}

void DataHandler::PrepareData(){
    // setting all data to zero
    data = new float*[dimensions];
    for (int i = 0 ; i < dimensions; i++)
        data[i] = new float[max_size];
    for (int j = 0 ; j < dimensions; j++)
        for (int i = 0 ; i < max_size; i++)
            data[j][i] = 0;
}

void DataHandler::FirstPartOfDataUpdate(){
    system("bash prepareData.sh");
}

void DataHandler::SecondPartOfDataUpdate(){
    system("bash 01_05_RAM.sh");
    system("bash 01_05_NET.sh");
    system("bash 01_05_CPU.sh");
}

void DataHandler::UpdateData (){
    float newData[dimensions];
    std::ifstream file;
    file.open ("data.txt");
    for (int i = 0 ; i < dimensions; i++)
        file >> newData[i];
    file.close ();
    for (int j= 0 ; j < dimensions; j++){
        for (int i = 0; i < max_size -1 ; i++)
            data[j][i] = data[j][i + 1];
    }

    for (int i = 0 ; i < dimensions; i++)
        data[i][max_size - 1] = newData[i];
}

void DataHandler::ExpandDataSet(int percent){
    // math
    size_of_data = percent * (max_size - min_size) * 0.01 + min_size;
}

void DataHandler::DrawCPUCoresInfo(QPainter *painter){
    int x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    int offset = 5;
    int columns;
    int rows;
    if (width > height){
        columns = cpu_grid_sizes[0];
        rows = cpu_grid_sizes[1];
    }
    else {
        columns = cpu_grid_sizes[1];
        rows = cpu_grid_sizes[0];
    }

    double x_grid_step = width / (double)columns;
    double y_grid_step = height / (double)rows;
    // ur problem it's boring part
    for (int i = 0; i < rows; i++)
        for (int j = 0 ; j < columns; j++){
            if (i * columns + j >= amountOfCores)
                return;
            QRectF current_rect(x_start + offset + j * x_grid_step,y_start + offset + i * y_grid_step,
                                x_grid_step - 2*offset,y_grid_step - 2*offset);

            int core_id = i * columns + j + 4;
            painter->setBrush (QBrush(QColor(2.55 * data[core_id][max_size-1],255 - 2.55 *data[core_id][max_size-1] ,0, 55 + 2 * data[core_id][max_size-1])));
            painter->drawRect (current_rect);


            QString str;
            str.setNum (data[core_id][max_size - 1],'p',2);
            QFont font;
            double min_grid_step = x_grid_step > y_grid_step ? y_grid_step : x_grid_step;
            font.setPixelSize (min_grid_step / str.length ());
            painter->setFont (font);
            QTextOption options;
            options.setAlignment (Qt::AlignCenter);
            painter->drawText (current_rect,str,options);
        }
}

void DataHandler::DrawBackgroundMarkup(QPainter *painter, int _columns,int _rows){
    int x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    width *= 1 - width_shrinkage;
    height *= 1 - height_shrinkage;


    double x_grid_step = width / (double)_columns,y_grid_step = height / (double)_rows;
    // ur problem it's boring part
    for (int i = 0; i < _rows; i++)
        for (int j = 0 ; j < _columns; j++)
            painter->drawRect (QRectF(x_start + j * x_grid_step,y_start + i * y_grid_step,
                                      x_grid_step,y_grid_step));
}

void DataHandler::SetCpuGradient(QPainter *painter,QPointF _start,QPointF _end){
    QLinearGradient grad;
    // set Start & End points
    grad.setStart (_start);
    grad.setFinalStop (_end);
    // Adding some colors during the range
    // Experiment with colours & alpha i bag u
    //    grad.setColorAt (0,QColor(0,255,0,100));
    //    grad.setColorAt (0.25,QColor(255,255,0,100));
    //    grad.setColorAt (0.75,QColor(255,0,100));
    //    grad.setColorAt (1,QColor(255,0,255,100));
    // к о ж з г с ф
    // rainbow
    // к ж з с ф
    int alpha = 255;
    grad.setColorAt (0,  QColor(255,0,0,alpha));
    //    grad.setColorAt (0.1,QColor(255,136,0,alpha));
    grad.setColorAt (0.1,QColor(255,255,0,alpha));
    grad.setColorAt (0.2,QColor(0,255,0,alpha));
    //    grad.setColorAt (0.4,QColor(0,255,255,alpha));
    grad.setColorAt (0.3,QColor(0,0,255,alpha));
    grad.setColorAt (0.4,QColor(255,0,255,alpha));
    grad.setColorAt (1,  QColor(0,255,255,alpha));
    // applying LinearGradient to painter
    painter->setBrush (grad);
}

void DataHandler::SetCurrentDimension(int value){
    current_dimension = value;
}

void DataHandler::SetOptimalSizeForTable(){
    for (int i = 1; i <= amountOfCores; i++)
        if (i >= amountOfCores / i){
            cpu_grid_sizes[0] = i;
            cpu_grid_sizes[1] = amountOfCores / i + (i * (amountOfCores / i) == amountOfCores ? 0 : 1);
            return;
        }
}

// add later
void DataHandler::SetCpuPainterForGraph(QPainter *painter){
    QPen pen(painter->pen ());
    pen.setJoinStyle (Qt::RoundJoin);
    pen.setWidth (2);
    pen.setColor (QColor(255,0,0,255));
    painter->setPen (pen);
    painter->setBrush (QBrush(QColor(255,0,0,50)));
}
void DataHandler::SetRamPainterForGraph(QPainter *painter){
    QPen pen(painter->pen ());
    pen.setJoinStyle (Qt::RoundJoin);
    pen.setWidth (2);
    pen.setColor (QColor(0,255,0,255));
    painter->setPen (pen);
    painter->setBrush (QBrush(QColor(0,255,0,50)));

}
void DataHandler::SetInTrafficPainterForGraph(QPainter *painter){
    QPen pen(painter->pen ());
    pen.setJoinStyle (Qt::RoundJoin);
    pen.setWidth (2);
    pen.setColor (QColor(0,255,255,255));
    painter->setPen (pen);
    painter->setBrush (QBrush(QColor(0,255,255,50)));
}
void DataHandler::SetOutTrafficPainterForGraph(QPainter *painter){
    QPen pen(painter->pen ());
    pen.setJoinStyle (Qt::RoundJoin);
    pen.setWidth (2);
    pen.setColor (QColor(255,0,255,255));
    painter->setPen (pen);
    painter->setBrush (QBrush(QColor(255,0,255,50)));
}

int DataHandler::GetMaxValueForGraph(){
    int max_value = -1;
    for (int i = 0; i < size_of_data; i++)
        if (data[current_dimension][i + max_size - size_of_data] > max_value)
            max_value = data[current_dimension][i + max_size - size_of_data];

    if (max_value == 0)
        return 1;
    return max_value;
}

double DataHandler::GetCoefForGraph(){
    if (current_dimension != _IN_TRAFFIC && current_dimension != _OUT_TRAFFIC)
        return 0.01;

//    qDebug() << "max_value";
    double coef = 1 / (1.3 * GetMaxValueForGraph());
    // controls _OUT_TRAFFIC graph & it's stretching
    // bigger value = bigger graph
    double coef_limit = 0.1;
    if (coef > coef_limit)
        coef = coef_limit;
    return coef;
}

void DataHandler::GraphDrawer(QPainter *painter){
    int x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);
    int original_width = width;
    int original_height = height;
    width *= 1 - width_shrinkage;
    height *= 1- height_shrinkage;

    double coef = GetCoefForGraph ();

    // here should be switch with presetting of painter values
    switch (current_dimension) {
    case _CPU:
        SetCpuPainterForGraph (painter);
        break;
    case _RAM:
        SetRamPainterForGraph (painter);
        break;
    case _IN_TRAFFIC:
        SetInTrafficPainterForGraph (painter);
        break;
    case _OUT_TRAFFIC:
        SetOutTrafficPainterForGraph (painter);
        break;
    }

    // drawing graph
    double x_step =width / (double)(size_of_data - 1);
    QPolygonF poly;
    qreal x,y;
    poly.append (QPointF(x_start,y_start + height));
    for (int i = 0 ; i < size_of_data; i++){
        x = i * x_step + x_start;
        y = height - (data[current_dimension][i + max_size - size_of_data] * height * coef - y_start);
        poly.append (QPointF(x,y));
    }
    poly.append (QPointF(x_start + width,y_start + height));
    painter->drawPolygon (poly);

    // drawing some good stuff :)
    {
        //    painter->setPen (QPen(QBrush(QColor(Qt::cyan)),5,Qt::SolidLine,Qt::SquareCap,Qt::RoundJoin));
        //    QPointF boundary_points[]{
        //        QPointF(x_start,y_start),
        //                QPointF(x_start,y_start + height),
        //                QPointF(x_start + width,y_start + height),
        //                QPointF(x_start + width,y_start),
        //                QPointF(x_start + original_width,y_start),
        //                QPointF(x_start + original_width,y_start + height)
        //    };
        //        painter->drawPolyline (boundary_points,4);

    }

    // load rect with set grad & pen
    painter->setPen (QPen(QBrush(QColor(Qt::cyan)),0.1,Qt::SolidLine,Qt::SquareCap,Qt::RoundJoin));
    QBrush curr_brush = painter->brush ();
    QColor curr_color = curr_brush.color ();
    QLinearGradient grad;
    grad.setStart (x_start + width,y_start + height);
    grad.setFinalStop (x_start + original_width,y_start);
                       //+ height - data[current_dimension][max_size - 1] * height * coef);
    curr_color.setAlpha (0);
    grad.setColorAt (0,QColor(100,100,100,50));
    curr_color.setAlpha (255);
    grad.setColorAt (1,curr_color);
    painter->setBrush (grad);
    QRectF load_rect;
    load_rect.setBottomLeft (QPointF(x_start + width,y_start + height));
    load_rect.setTopRight(QPointF(x_start + original_width,y_start + height - data[current_dimension][max_size - 1] * height * coef));
    painter->drawRect (load_rect);

    // preset of text rect & alignment
    QRectF text_load_rect;
    QTextOption options;
    if (y_start + height - data[current_dimension][max_size - 1] * height * coef <= y_start + 0.5 * height){
        text_load_rect.setBottomLeft (QPointF(x_start + width,y_start + height));
        text_load_rect.setTopRight(QPointF(x_start + original_width,y_start + height - data[current_dimension][max_size - 1] * height * coef));
        options.setAlignment (Qt::AlignHCenter | Qt::AlignTop);
    }
    else{
        text_load_rect.setBottomLeft (QPointF(x_start + width,y_start + height - data[current_dimension][max_size - 1] * height * coef));
        text_load_rect.setTopRight(QPointF(x_start + original_width,y_start));
        options.setAlignment (Qt::AlignBottom | Qt::AlignHCenter);
    }

    // drawing text with load data
    QString str;
    str.setNum (data[current_dimension][max_size - 1],'p',2);
    QFont font;
    int pixel_size = original_width - width;
    font.setPixelSize (pixel_size / str.length ());
    painter->setFont (font);
    painter->drawText (text_load_rect,str,options);
}

void DataHandler::DataDrawer(QPainter *painter,QRect rect){
    // getting sizes
    this->rect = rect;
    int x_start,y_start,width,height;
    this->rect.getRect (&x_start,&y_start,&width,&height);

    // IDK refactor those values
    int width_of_grid = sqrt(size_of_data);
    int height_of_grid = 10;
    // if core mode
    if (current_dimension == _CORES){
        DrawCPUCoresInfo (painter);
        return;
    }
//                else
//                    DrawBackgroundMarkup (painter,width_of_grid,height_of_grid);
    //            SetCpuGradient (painter,QPointF(x_start,y_start + height),QPointF(x_start,y_start));
    GraphDrawer (painter);
}

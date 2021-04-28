#include "datahandler.h"
#include <QDebug>
#include <cmath>
#include <QPen>
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
    system("bash firstPart.sh");
}

void DataHandler::SecondPartOfDataUpdate(){
    system("bash secondPart.sh");
}

void DataHandler::UpdateData (){
    FILE* file = fopen("data.txt","r");
    int size = 4 * (amountOfCores + 1) + 5;
    long int newData[size];
    for (int i = 0 ; i < size; i++)
        fscanf(file,"%ld",&newData[i]);
    fclose(file);
    float res[dimensions];
    for (int i = 0 ; i < amountOfCores + 1; i++){
        long int delta[] = {
            newData[i * 2] - newData[i * 2 + 2 * amountOfCores + 4],
            newData[i * 2]  + newData[i * 2 + 1] - newData[i * 2 + 2 * amountOfCores + 4] - newData[i * 2 + 2 * amountOfCores + 5]
        };
        res[i] = (float)((double)delta[0] / (double)delta[1]);
        res[i] *= 100;
    }
    res[dimensions - 1] = (newData[size - 2] - newData[size - 2 -  2 * amountOfCores - 4]) / 1000; // out Net
    res[dimensions - 2] = (newData[size - 3] - newData[size - 3 -  2 * amountOfCores - 4]) / 1000; // in Net
    res[dimensions - 3] = newData[size - 1]; // ram
    for (int j= 0 ; j < dimensions; j++){
        for (int i = 0; i < max_size -1 ; i++)
            data[j][i] = data[j][i + 1];
//        data[j][max_size - 1]= res[j];
    }

    data[0][max_size - 1] = res[0];
    data[1][max_size - 1] = res[dimensions - 3];
    data[2][max_size - 1] = res[dimensions - 2];
    data[3][max_size - 1] = res[dimensions - 1];
    for (int i = 0 ; i < amountOfCores; i++)
        data[4 + i][max_size - 1] = res[i + 1];

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
    double x_grid_step = width / (double)columns,y_grid_step = height / (double)rows;
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
            str.setNum (data[core_id][max_size - 1],'g',3);
            QFont font;
            double min_grid_step = x_grid_step > y_grid_step ? y_grid_step : x_grid_step;
            font.setPixelSize (min_grid_step / 2.5);
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

void DataHandler::SetOptimalSizeForTable(){
    for (int i = 1; i <= amountOfCores; i++)
        if (i >= amountOfCores / i){
            cpu_grid_sizes[0] = i;
            cpu_grid_sizes[1] = amountOfCores / i + (i * (amountOfCores / i) == amountOfCores ? 0 : 1);
            return;
        }
}

void DataHandler::GraphDrawer(QPainter *painter,bool _isOutOfSize){
    int x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);
    double coef_limit = 0.015;
    double coef = 0.01;
    float max_value = 100;

    coef = 1 / max_value;

    while (_isOutOfSize){
        max_value = -1;
        for (int i = 0; i < size_of_data; i++)
            if (data[current_dimension][i + max_size - size_of_data] > max_value)
                max_value = data[current_dimension][i + max_size - size_of_data];
        if (max_value == 0)
            max_value = 1;
        max_value *= 1.3;
        coef = 1 / max_value;

        if (coef > coef_limit)
            coef = coef_limit;
        break;
    }

    int original_width = width;
    int original_height = height;
    width *= 1 - width_shrinkage;
    height *= 1- height_shrinkage;


    QPen pen(painter->pen ());
    pen.setJoinStyle (Qt::RoundJoin);
    pen.setWidth (2);
    painter->setPen (pen);
    painter->setBrush (QBrush(QColor(255,150,200,50)));
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




    painter->setPen (QPen(QBrush(QColor(Qt::cyan)),5,Qt::SolidLine,Qt::SquareCap,Qt::RoundJoin));
    QPointF boundary_points[]{
        QPointF(x_start,y_start),
                QPointF(x_start,y_start + height),
                QPointF(x_start + width,y_start + height),
                QPointF(x_start + width,y_start),
                QPointF(x_start + original_width,y_start),
                QPointF(x_start + original_width,y_start + height)

    };
    //    painter->drawPolyline (boundary_points,6);



    painter->setPen (QPen(QBrush(QColor(Qt::cyan)),0,Qt::SolidLine,Qt::SquareCap,Qt::RoundJoin));
    SetCpuGradient (
                painter,
                QPointF(x_start + width, y_start + height),
                QPointF(x_start + width, y_start)
                );
    QRectF tmp_rect;
    tmp_rect.setBottomLeft (QPointF(x_start + width,y_start + height));
    tmp_rect.setTopRight(QPointF(x_start + original_width,y_start + height - data[current_dimension][max_size - 1] * height * coef));
    painter->drawRect (tmp_rect);

    QTextOption options;
    if (data[current_dimension][max_size - 1] >= 0.5*max_value){
        tmp_rect.setBottomLeft (QPointF(x_start + width,y_start + height));
        tmp_rect.setTopRight(QPointF(x_start + original_width,y_start + height - data[current_dimension][max_size - 1] * height * coef));
        options.setAlignment (Qt::AlignHCenter | Qt::AlignTop);
    }
    else{
        tmp_rect.setBottomLeft (QPointF(x_start + width,y_start + height - data[current_dimension][max_size - 1] * height * coef));
        tmp_rect.setTopRight(QPointF(x_start + original_width,y_start));
        options.setAlignment (Qt::AlignBottom | Qt::AlignHCenter);
    }

    QString str;
    str.setNum (data[current_dimension][max_size - 1],'p',0);
    QFont font;
    int pixel_size = original_width - width;

    font.setPixelSize (pixel_size / str.length ());
    painter->setFont (font);
    painter->drawText (
                tmp_rect,
                str,
                options
                );

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
    if (current_dimension == Cores){
        DrawCPUCoresInfo (painter);
        return;
    }
    //            else
    //                DrawBackgroundMarkup (painter,width_of_grid,height_of_grid);
//            SetCpuGradient (painter,QPointF(x_start,y_start + height),QPointF(x_start,y_start));
    GraphDrawer (painter, current_dimension == InTraffic || current_dimension == OutTraffic);
}

void DataHandler::SetCurrentDimension(int value){
    // used for switching between cpu / ram / NET graphs
    current_dimension = value;
}

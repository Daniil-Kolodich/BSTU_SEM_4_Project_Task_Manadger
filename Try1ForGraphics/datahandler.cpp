#include "datahandler.h"
#include <QDebug>
#include <cmath>
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
        data[j][max_size - 1]= res[j];
    }


    //    qDebug() << "0 : " << newData[0];
    //    << " , 1 : "<< data[2][max_size-1];
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

            int core_id = i * columns + j + 1;
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
    grad.setColorAt (0,QColor(0,255,0,255));
    grad.setColorAt (0.5,QColor(255,255,0,255));
    grad.setColorAt (1,QColor(255,0,0,100));
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


    double coef = 0.01;
    while (_isOutOfSize){
        float max_value = -1;
        for (int i = 0; i < size_of_data; i++)
            if (data[current_dimension][i + max_size - size_of_data] > max_value)
                max_value = data[current_dimension][i + max_size - size_of_data];
        if (max_value == 0)
            break;
        coef = 1 / (max_value * 1.5);
        qDebug() << coef << " FOR " << max_value;
        break;
    }



    double x_step =width / (double)(size_of_data - 1);
    QPolygonF poly;
    qreal x,y;
    // start_point
    poly.append (QPoint(x_start,y_start + height));
    for (int i = 0 ; i < size_of_data; i++){
        x = i * x_step + x_start;
        y = height - (data[current_dimension][i + max_size - size_of_data] * height * coef - y_start);
        poly.append (QPointF(x,y));
    }
    // end_point
    poly.append (QPointF(x_start + width,y_start + height));
    // hardcode points for using gradient
    painter->drawPolygon (poly);

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
    if (current_dimension == 1){
        DrawCPUCoresInfo (painter);
        return;
    }
    else
        DrawBackgroundMarkup (painter,width_of_grid,height_of_grid);
    SetCpuGradient (painter,QPointF(x_start,y_start + height),QPointF(x_start,y_start));
    GraphDrawer (painter, current_dimension >= dimensions - 2);
}

void DataHandler::SetCurrentDimension(int value){
    // used for switching between cpu / ram / NET graphs
    /*
    1 - Each Core
    0 - CPU
    -1 - RAM
    -2 - Wi-Fi
    */
    switch (value) {
    case 1:
        // cores mode
        current_dimension = 1;
        break;
    case 0:
        // cpu
        current_dimension = 0;
        break;
    case -1:
        // ram
        current_dimension = dimensions - 3;
        break;
    case -2:
        // in wifi
        current_dimension = dimensions - 2;
        break;
    case -3:
        // out wifi
        current_dimension = dimensions - 1;
        break;
    default:
        current_dimension = 0;
        break;
    }
}

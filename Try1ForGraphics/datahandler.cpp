#include "datahandler.h"
#include <QDebug>
#include <cmath>
DataHandler::DataHandler(){
    // ez
    SetAmountOfCores ();
    size_of_data = min_size;
    // + 3 cause totalCpu / RAM / Wi-Fi
    this->dimensions = amountOfCores + 3;
    // fill data
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
void DataHandler::FirstPartOfGettingData (){
    // look at bash script
    system ("bash firstPart.sh");
}
void DataHandler::SecondPartOfGettingData (){
    // look at bash script
    system ("bash secondPart.sh");
    // reading data from file
    FILE* file = fopen("data.txt","r");
    // cause we like have (old + new) * (amount of cores + 1) * ([1] + [2]) &&&&&& + 2 for RAM & Net
    int size = 4 * (amountOfCores + 1) + 2;
    long int newData[size];
    // reading data
    for (int i = 0 ; i < size; i++)
        fscanf(file,"%ld",&newData[i]);
    fclose(file);
    // some king of data calculations to get actual CPU loads
    float res[dimensions];
    for (int i = 0 ; i < amountOfCores + 1; i++){
        long int delta[] = {
            newData[i * 2] - newData[i * 2 + 2 * amountOfCores + 2],
            newData[i * 2]  + newData[i * 2 + 1] - newData[i * 2 + 2 * amountOfCores + 2] - newData[i * 2 + 2 * amountOfCores + 3]
        };
        res[i] = (float)((double)delta[0] / (double)delta[1]);
        res[i] *= 100;
    }
    // set Ram & net
    res[dimensions - 1] = newData[size - 1];
    res[dimensions - 2] = newData[size - 2];
    // move array one cell to left & set right most equal new data
    for (int j= 0 ; j < dimensions; j++){
        for (int i = 0; i < max_size -1 ; i++)
            data[j][i] = data[j][i + 1];
        data[j][max_size - 1]= res[j];
    }
}
void DataHandler::ExpandDataSet(int percent){
    // math
    size_of_data = percent * (max_size - min_size) * 0.01 + min_size;
}

// new
void DataHandler::DrawBackgroundMarkup(QPainter *painter,int _x_start,int _y_start,int _width,int _height,int _columns,int _rows, int _offset, bool _is_CPU_Cores){
    // x_step
    double x_grid_step = _width / (double)_columns,y_grid_step = _height / (double)_rows;
    // ur problem it's boring part
    for (int i = 0; i < _rows; i++)
        for (int j = 0 ; j < _columns; j++){
            if (_is_CPU_Cores && i * _columns + j >= amountOfCores)
                return;
            // draw rect IDK how but working :)
            painter->drawRect (QRectF(_x_start + _offset + j * x_grid_step,_y_start + _offset + i * y_grid_step,
                                      x_grid_step - 2*_offset,y_grid_step - 2*_offset));
            // SHIT
            if (_is_CPU_Cores){
                // in separate function ?
                int core_id = i * _columns + j + 1;
                //                qDebug() << "Core id : " << core_id;
                painter->setBrush (QBrush(QColor(0,0,255, 2.55 * data[core_id][max_size-1])));
                QString str;
                str.setNum (data[core_id][max_size - 1],'g',data[core_id][max_size - 1] < 10 ? 3 : 4);
                QFont font;
                font.setPixelSize (0.2 * (x_grid_step > y_grid_step ? y_grid_step : x_grid_step));
                painter->setFont (font);
                painter->drawRect (QRectF(_x_start + _offset + j * x_grid_step,_y_start + _offset + i * y_grid_step,
                                          x_grid_step - 2*_offset,y_grid_step - 2*_offset));
                painter->drawText (_x_start + _offset + j * x_grid_step,_y_start + _offset + i * y_grid_step + font.pixelSize (),str);

            }



        }
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
void DataHandler::SetOptimalSizeForTable(int *_width,int *_height,int *_offset){
    // IDK that's not actually working well
    // REFACTOR !
    *_offset = 5;
    //    int cores = 21;
    int size;
    for (size = 1 ; size < amountOfCores; size++)
        if (size > amountOfCores / size)
            break;
    *_height = size;
    *_width = amountOfCores / size + 1;
    // some useful staff from elder commits
    //    height_of_grid = size;
    //    width_of_grid = amountOfCores / size + 1;
    //    height_of_grid -= ((height_of_grid - 1) * width_of_grid == amountOfCores ? 1 : 0);
}
void DataHandler::GraphDrawer(QPainter *painter, int _x_start,int _y_start,int _width, int _height){
    // oh Fuck it's graph drawer
    // ez
    double x_step =_width / (double)(size_of_data - 1);
    QPolygonF poly;
    qreal x,y;
    // start_point
    poly.append (QPoint(_x_start,_y_start + _height));
    for (int i = 0 ; i < size_of_data; i++){
        x = i * x_step + _x_start;
        y = _height - (data[current_dimension][i + max_size - size_of_data] * _height * 0.01 - _y_start);
        poly.append (QPointF(x,y));
    }
    // end_point
    poly.append (QPointF(_x_start + _width,_y_start + _height));
    // hardcode points for using gradient
    painter->drawPolygon (poly);

}

void DataHandler::DataDrawer(QPainter *painter,QRect rect){
    // getting sizes
    int x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);
    // IDK refactor those values
    int width_of_grid = sqrt(size_of_data);
    int height_of_grid = 5;
//            = sqrt(size_of_data);
    // 0 for grid some value for cells
    int offset = 0;
    // if core mode
    if (current_dimension == 1)
        SetOptimalSizeForTable (&width_of_grid,&height_of_grid,&offset);
    // background & cells drawer
    DrawBackgroundMarkup (painter, x_start,y_start,width,height,width_of_grid,height_of_grid,offset,current_dimension == 1);
    // if cells drawer we done here
    if (current_dimension == 1)
        return;
    // setting cpu gradient
    SetCpuGradient (painter,QPointF(x_start,y_start + height),QPointF(x_start,y_start));
    // graph drawer
    GraphDrawer (painter,x_start,y_start,width,height);
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
    case -10:
        current_dimension = current_dimension == 1 ? 0 : 1;
        break;
    case 1:
        current_dimension = 1;
        break;
    case 0:
        current_dimension = 0;
        break;
    case -1:
        current_dimension = dimensions - 2;
        break;
    case -2:
        current_dimension = dimensions - 1;
        break;
    default:
        current_dimension = 0;
        break;
    }
}

#include "datahandler.h"
#include <QDebug>
#include <cmath>
DataHandler::DataHandler(){
    SetAmountOfCores ();
    // + 3 cause totalCpu / RAM / Wi-Fi
    this->dimensions = amountOfCores + 3;
    PrepareData ();
}
void DataHandler::SetAmountOfCores (){
    //    "nproc"
    system("nproc > amountOfCores.txt");
    FILE* cores = fopen("amountOfCores.txt","r");
    fscanf(cores,"%d",&this->amountOfCores);
    fclose(cores);
    system ("rm amountOfCores.txt");
}
DataHandler::DataHandler(int size_of_data,int dimensions){
    // Delete this later
    this->size_of_data = size_of_data;
    this->dimensions = dimensions;
    PrepareData ();
}
DataHandler::~DataHandler(){
    for (int j = 0 ; j < dimensions; j++)
        delete[] data[j];
    delete[] data;
}
void DataHandler::PrepareData(){
    // setting all data to zero
    data = new float*[dimensions];
    for (int i = 0 ; i < dimensions; i++)
        data[i] = new float[size_of_data];
    for (int j = 0 ; j < dimensions; j++)
        for (int i = 0 ; i < size_of_data; i++)
            data[j][i] = i % 2 ? 0 : 100;
}
void DataHandler::SecondPartOfGettingData (){
    system ("bash secondPart.sh");
    FILE* file = fopen("data.txt","r");
    int size = 4 * (amountOfCores + 1) + 2;
    double newData[size];
    for (int i = 0 ; i < size; i++)
        fscanf(file,"%lf",&newData[i]);
    fclose(file);
    float res[dimensions];
    for (int i = 0 ; i < amountOfCores + 1; i++){
        res[i] = (newData[i * 2] - newData[i * 2 + 2 * amountOfCores + 2]) / (newData[i * 2]  + newData[i * 2 + 1] - newData[i * 2 + 2 * amountOfCores + 2] - newData[i * 2 + 2 * amountOfCores + 3] );
        res[i] *= 100;
    }
    res[dimensions - 1] = newData[size - 1];
    res[dimensions - 2] = newData[size - 2];

//    system ("sleep 0.1s");
    for (int j= 0 ; j < dimensions; j++){
        for (int i = 0 ; i < size_of_data -1 ; i++)
            data[j][i] = data[j][i + 1];
        data[j][size_of_data - 1] = res[j];
    }
}
void DataHandler::FirstPartOfGettingData (){
    system ("bash firstPart.sh");
}
void DataHandler::ExpandDataSet(int percent){
    // percent is data from vertical slider
    // evaluating new size
    int needed_size = min_size + 0.01 * percent * (max_size - min_size);
    // making new array & copying data to it
    float** tmp_data = new float*[dimensions];
    for (int j = 0 ; j < dimensions; j++){
        tmp_data[j] = new float[needed_size];
        for (int i = 0 ; i < needed_size; i++)
            tmp_data[j][i] = 0;
        for (int i = 0; i < needed_size; i++){
            // IDK seems to be working
            if (size_of_data - i <= 0 || needed_size - i <= 0)
                continue;
            tmp_data[j][needed_size - i - 1] = data[j][size_of_data - i - 1];
        }
    }
    // delete all data
    for (int i = 0 ; i < dimensions; i++)
        delete[] data[i];
    delete[] data;
    // swap pointers
    data = tmp_data;
    size_of_data = needed_size;

}
void DataHandler::DataDrawer(QPainter *painter,QRect rect){
    // getting start coordinates & size
    int x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);
    /*
     Выводить график нагрузки на процессор в целом, а на каждое ядро в отдельности сделать небольшой квадрат с отображением
     .. процента загрузки и градиента в зависимости от процента
     */
    int width_of_grid = 0;
    int height_of_grid = 0;
    int offset = 0;
    double x_grid_step;
    double y_grid_step;
    if (current_dimension == 1){
        // drawing each core heat mark
        width_of_grid = 5;
        height_of_grid = 5;
        offset = 5;
    }
    else{
        // drawing background markup
        width_of_grid = 10;
        height_of_grid = 10;
//        offset = 2;
    }

    // grid drawing
    x_grid_step = width / (double)width_of_grid;
    y_grid_step = height / (double)height_of_grid;
    for (int i = 0 ; i < width_of_grid; i++)
        for (int j = 0 ;j < height_of_grid; j++){
               painter->setBrush (QBrush(QColor(0,0,255,current_dimension == 1 ? 255 : 0)));
               painter->drawRect (QRectF(x_start + offset + i * x_grid_step,y_start + offset + j * y_grid_step,x_grid_step - 2*offset,y_grid_step - 2*offset));
        }
    if (current_dimension == 1)
        return;
    // evaluating step on X axes to cover it by all data
    double x_step = width / (double)(size_of_data - 1);
    QPolygonF poly;
    qreal x,y;
    // start point at left bottom corner
    // used for Polygon instead of Polyline
    poly.append (QPoint(x_start,y_start + height));
    for (int i = 0 ; i < size_of_data; i++){
        x = i * x_step + x_start;
        y = height - (data[current_dimension][i] * height * 0.01 - y_start);
        // adding new points based on index & data
        poly.append (QPointF(x,y));
    }
    // last point at right bottom corner
    // still used only for Polygon instead of Polyline
    poly.append (QPointF(x_start + width,y_start + height));
    // making gradient
    QLinearGradient grad;
    // set Start & End points
    grad.setStart (x_start,y_start + height);
    grad.setFinalStop (x_start,y_start);
    // Adding some colors during the range
    // Experiment with colours & alpha i bag u
    grad.setColorAt (0,QColor(0,255,0,255));
    grad.setColorAt (0.5,QColor(255,255,0,255));
    grad.setColorAt (1,QColor(255,0,0,100));
    // applying LinearGradient to painter
    painter->setBrush (grad);
    // drawing Polygon
    painter->drawPolygon (poly);
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

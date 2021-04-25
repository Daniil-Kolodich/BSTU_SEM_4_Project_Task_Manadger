#include "datahandler.h"
#include <QDebug>
DataHandler::DataHandler(){
    SetAmountOfCores ();
    this->dimensions = amountOfCores + 2;
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
    data = new int*[dimensions];
    for (int i = 0 ; i < dimensions; i++)
        data[i] = new int[size_of_data];
    for (int j = 0 ; j < dimensions; j++)
        for (int i = 0 ; i < size_of_data; i++)
            data[j][i] = 0;
}
void DataHandler::GetNewData(){
    /*int newData[2];
    // calling system to get actual load of CPU & RAM
    system ("awk -v tmp=\"$(awk \'/cpu /{print $2+$4,$2+$4+$5}\' /proc/stat; sleep 0.3s)\" \'/cpu /{split(tmp,old,\" \"); print 100*($2+$4-old[1])/($2+$4+$5-old[2])}\' /proc/stat > cpu.txt");
    system ("free | awk \'/Mem: / {print 100 * $3 / $2}\' > ram.txt");
    // reading data
    // IDK why it's not working if i try to use only one file
    FILE* input = fopen("cpu.txt","r");
    fscanf (input,"%d",&newData[0]);
    fclose(input);
    input = fopen("ram.txt","r");
    fscanf (input,"%d",&newData[1]);
    fclose(input);*/

    // moving all data one index left
    // write new data into last cells
    system ("sleep 0.5s");
    for (int j= 0 ; j < dimensions; j++){
        for (int i = 0 ; i < size_of_data -1 ; i++)
            data[j][i] = data[j][i + 1];
        data[j][size_of_data - 1] = rand() % 100;
//                = newData[j];
    }

}
void DataHandler::ExpandDataSet(int percent){
    // percent is data from vertical slider
    // evaluating new size
    int needed_size = min_size + 0.01 * percent * (max_size - min_size);
    // making new array & copying data to it
    int** tmp_data = new int*[dimensions];
    for (int j = 0 ; j < dimensions; j++){
        tmp_data[j] = new int[needed_size];
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
    grad.setColorAt (0,QColor(0,255,0,100));
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
    0 - CPU
    -1 - RAM
    -2 - Wi-Fi
    */
    switch (value) {
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

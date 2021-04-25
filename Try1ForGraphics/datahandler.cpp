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
DataHandler::~DataHandler(){
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
void DataHandler::SecondPartOfGettingData (){
    system ("bash secondPart.sh");
    FILE* file = fopen("data.txt","r");
    int size = 4 * (amountOfCores + 1) + 2;
    long int newData[size];
    for (int i = 0 ; i < size; i++)
        fscanf(file,"%ld",&newData[i]);
    fclose(file);
    float res[dimensions];
    for (int i = 0 ; i < amountOfCores + 1; i++){
        long int delta[] = {
            newData[i * 2] - newData[i * 2 + 2 * amountOfCores + 2],
            newData[i * 2]  + newData[i * 2 + 1] - newData[i * 2 + 2 * amountOfCores + 2] - newData[i * 2 + 2 * amountOfCores + 3]
        };
        res[i] = (float)((double)delta[0] / (double)delta[1]);
        res[i] *= 100;
    }
    res[dimensions - 1] = newData[size - 1];
    res[dimensions - 2] = newData[size - 2];
    for (int j= 0 ; j < dimensions; j++){
        for (int i = 0; i < max_size -1 ; i++)
            data[j][i] = data[j][i + 1];
        data[j][max_size - 1]= res[j];
    }
}
void DataHandler::FirstPartOfGettingData (){
    system ("bash firstPart.sh");
}
void DataHandler::ExpandDataSet(int percent){
    size_of_data = percent * (max_size - min_size) * 0.01 + min_size;
}

// new
void DataHandler::DrawBackgroundMarkup(QPainter *painter,int _x_start,int _y_start,int _width,int _height,int _columns,int _rows, int _offset, bool _is_CPU_Cores){
    double x_grid_step = _width / (double)_columns,y_grid_step = _height / (double)_rows;
    for (int i = 0; i < _rows; i++)
        for (int j = 0 ; j < _columns; j++){
            if (_is_CPU_Cores && i * _columns + j >= amountOfCores)
                return;
            painter->drawRect (QRectF(_x_start + _offset + j * x_grid_step,_y_start + _offset + i * y_grid_step,
                                      x_grid_step - 2*_offset,y_grid_step - 2*_offset));
        }
}
void DataHandler::SetCpuGradient(QPainter *painter,QPointF _start,QPointF _end){}
void DataHandler::SetOptimalSizeForTable(int *_width,int *_height){}
void DataHandler::GraphDrawer(QPainter *painter){}

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
    //    int cores= 160;
    if (current_dimension == 1){
        // drawing each core heat mark
        int* grid;
        int size = 1;
        while(1){
            // amount of rowd with cells in them
            grid = new int[size];
            for (int i = 0 ; i < size; i++)
                grid[i] = amountOfCores / size;
            //            qDebug() << "Size = : " << size << " first el" << grid[0];
            if (size > grid[0]){
                delete[] grid;
                break;
            }
            //            system("sleep 5");
            delete[] grid;
            size++;
        }
        height_of_grid = size;
        width_of_grid = amountOfCores / size + 1;
        height_of_grid -= ((height_of_grid - 1) * width_of_grid == amountOfCores ? 1 : 0);
        offset = 5;
    }
    else{
        // drawing background markup
        width_of_grid = 10;
        height_of_grid = 10;
        //        offset = 2;
    }

    DrawBackgroundMarkup (painter, x_start,y_start,width,height,10,10,offset,false);

    // grid drawing
    /*
    x_grid_step = width / (double)width_of_grid;
    y_grid_step = height / (double)height_of_grid;
    qDebug() << "MAIN : " << x_start << " , " << y_start << " , " << width << " , " << height << " , " << x_grid_step << " ," << y_grid_step;

    for (int i = 0 ; i < width_of_grid; i++)
        for (int j = 0 ;j < height_of_grid; j++){
            if (current_dimension == 1){
                if (j * width_of_grid + i >= amountOfCores)
                    break;
                int core_id = j * width_of_grid + i + 1;
                //                qDebug() << "Core id : " << core_id;
                painter->setBrush (QBrush(QColor(0,0,2.55 * data[core_id][max_size - 1],100)));
                QString str;
                str.setNum (data[core_id][max_size - 1],'g',data[core_id][max_size - 1] < 10 ? 3 : 4);
                //                str = str.arg (data[core_id][max_size - 1]);
                //                painter->drawText ();
                QFont font;
                font.setPixelSize (0.2 * (x_grid_step > y_grid_step ? y_grid_step : x_grid_step));
                painter->setFont (font);
                painter->drawText (x_start + offset + i * x_grid_step,y_start + offset + j * y_grid_step + font.pixelSize (),str);
            }
            if (j * width_of_grid + i >= amountOfCores && current_dimension == 1)
                break;
            painter->drawRect (QRectF(x_start + offset + i * x_grid_step,y_start + offset + j * y_grid_step,x_grid_step - 2*offset,y_grid_step - 2*offset));
        }
        */
    if (current_dimension == 1)
        return;
    // evaluating step on X axes to cover it by all data
    double x_step = width / (double)(size_of_data - 1);

    //    qDebug() << "Data segment size : " << size_of_data;
    //    qDebug() << "For width = " << width << " we have sizes : " << x_step;



    QPolygonF poly;
    qreal x,y;
    // start point at left bottom corner
    // used for Polygon instead of Polyline
    poly.append (QPoint(x_start,y_start + height));
    for (int i = 0 ; i < size_of_data; i++){
        x = i * x_step + x_start;
        y = height - (data[current_dimension][i + max_size - size_of_data] * height * 0.01 - y_start);
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

#include "datahandler.h"
#include <stdlib.h>
#include <unistd.h>
QString DataHandler::GetInfoText (){
    system("bash getText.sh");
    std::ifstream file("info.txt");
    QString res;
    while(1){
        char *str = new char[1000];
        file.getline (str,1000);
        res.append (str);
        res.append ('\n');
        delete str;
        if (file.eof())
            break;
    }
    file.close ();
    return res;
}

void DataHandler::Drawer(QPainter* painter,QRectF rect){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    float max_value = GetMaxValueForGraph ();
    double coef = GetCoefForGraph (max_value);


    SetColor(painter);


    float graph_size = 0.85;
    QRectF graph_rect = QRectF(x_start,y_start,graph_size * width,height);
    DrawGraph (painter,graph_rect,coef);
    DrawGraphName (painter,graph_rect);

    QRectF load_rect = QRectF(x_start + graph_size * width,y_start,width * (1 - graph_size),height);
    DrawLoadBar (painter,load_rect,data[current_dimension][max_size - 1] * coef * height);

    int amount_of_marks = 5;
    DrawLoadMarkup (painter,load_rect,amount_of_marks);

    painter->setPen (QPen(QBrush(QColor(Qt::white)),5,Qt::SolidLine,Qt::SquareCap,Qt::RoundJoin));
    DrawLoadMarkupText (painter,load_rect,amount_of_marks,max_value);

    float text_size = 0.3;
    QRectF text_rect = QRectF(x_start + (graph_size - text_size) * width,y_start,text_size * width,height);
    DrawLoadText (painter,text_rect,coef);
}

void DataHandler::DrawGraphName (QPainter *painter, QRectF rect){
    QFont font = painter->font ();
    QString str = str_to_append[current_dimension];
    font.setPixelSize (rect.width () / 10);
    painter->setFont (font);
    painter->drawText (rect,str,QTextOption(Qt::AlignTop | Qt::AlignLeft));
}

void DataHandler::DrawGraph(QPainter* painter,QRectF rect,double coef){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    double x_step = width / (double)(size_of_data - 1);

    QPolygonF poly;
    poly.append (QPointF(x_start,y_start + height));
    for (int i = 0 ; i < size_of_data; i++)
        poly.append (QPointF(
                         i * x_step + x_start,
                         y_start + height * (1 - data[current_dimension][i + max_size - size_of_data] * coef)));
    poly.append (QPointF(x_start + width,y_start + height));

    painter->drawPolygon (poly);
}

void DataHandler::DrawLoadBar(QPainter* painter,QRectF rect,float load_height){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    int start_alpha = 50;
    int end_alpha = 255;

    QColor curr_color = painter->brush ().color ();
    QLinearGradient grad;
    grad.setStart (x_start,y_start + height);
    grad.setFinalStop (x_start,y_start);
    curr_color.setAlpha (start_alpha);
    grad.setColorAt (0,curr_color);
    curr_color.setAlpha (end_alpha);
    grad.setColorAt (1,curr_color);

    painter->setBrush (grad);
    painter->drawRect (QRectF(x_start,y_start + height - load_height,width,load_height));
}

void DataHandler::DrawLoadMarkup(QPainter *painter,QRectF rect,int amount_of_marks){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    QPolygonF poly;

    for (int i = 0; i <= amount_of_marks; i++){
        poly.append (QPointF(x_start + width * (i % 2 ? 1 : 0),y_start + height - height * i / amount_of_marks));
        poly.append (QPointF(x_start + width * (i % 2 ? 0 : 1),y_start + height - height * i / amount_of_marks));
    }

    painter->drawPolyline (poly);
}

void DataHandler::DrawLoadMarkupText(QPainter *painter,QRectF rect,int amount_of_marks,float max_value){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    float y_step = height / amount_of_marks;

    QString mark;
    mark.setNum (max_value,'p',1);
    QFont mark_font;
    mark_font.setPixelSize (width / mark.length ());
    painter->setFont (mark_font);
    for (int i =1 ; i <= amount_of_marks;i++){
        mark.setNum ( (i / (float)amount_of_marks) * max_value,'p',1);
        QRectF mark_rect(QPointF(x_start,y_start + height - i * y_step),
                         QPointF(x_start + width,y_start + height - (i-1) * y_step));
        painter->drawText (mark_rect,mark,QTextOption(Qt::AlignHCenter | Qt::AlignTop));
    }
}

void DataHandler::DrawLoadText(QPainter *painter,QRectF rect, double coef){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    bool isUpper = data[current_dimension][max_size - 1] * coef < 0.5;
    QRectF text_load_rect = QRectF(
                x_start, y_start + (isUpper ? 0 : height * (1 - data[current_dimension][max_size - 1] * coef)),
            width, (isUpper ? -1 : 1) * height * data[current_dimension][max_size - 1] * coef + (isUpper ? height : 0));

    QString str;
    str.setNum (data[current_dimension][max_size - 1],'p',2);
    QString add_string = current_dimension == _CPU ? "%" : (current_dimension < _IN_TRAFFIC ? "\nGB" : "\nKb/s");
    str.append (add_string);
    QFont font;
    font.setPixelSize (width / str.length ());
    painter->setFont (font);
    painter->drawText (text_load_rect,str,QTextOption(Qt::AlignRight | (isUpper ? Qt::AlignBottom : Qt::AlignTop)));
}

void DataHandler::DataDrawer(QPainter *painter,QRectF rect){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);
    if (current_dimension == _CORES){
        DrawCPUCoresInfo (painter,rect);
        return;
    }
    Drawer (painter,rect);
}

void DataHandler::PresetColors (){
    colors[_RAM] = QColor(90,255,1);
    colors[_SWAP] = QColor(255,0,66);
    colors[_IN_TRAFFIC] = QColor(1,238,255);
    colors[_OUT_TRAFFIC] = QColor(225,0,254);
    colors[_CPU] = QColor(255,78,0);
}

void DataHandler::SetColor(QPainter *painter){
    QPen pen;
    int width = 6;
    int background_alpha = 37;
    int line_alpha = 255;

    QColor color;
    color.setAlpha (line_alpha);
    color = colors[current_dimension];

    pen.setJoinStyle (Qt::RoundJoin);
    pen.setWidth (width);
    pen.setColor (color);
    painter->setPen (pen);

    color.setAlpha (background_alpha);
    painter->setBrush (QBrush(color));
}

float DataHandler::GetMaxValueForGraph(){
    if (current_dimension == _RAM || current_dimension == _SWAP)
        return max_memory_values[current_dimension];
    if (current_dimension == _CPU)
        return 100;
    float max_value = -1;
    for (int i = 0; i < size_of_data; i++)
        if (data[current_dimension][i + max_size - size_of_data] > max_value)
            max_value = data[current_dimension][i + max_size - size_of_data];

    if (max_value == 0)
        return 1;
    return max_value;
}

double DataHandler::GetCoefForGraph(float max_value){
    if (current_dimension < _IN_TRAFFIC)
        return 1 / max_value;
    // controls _OUT_TRAFFIC graph & it's stretching
    //     bigger value = bigger graph
    double coef = 1 /  max_value;
    double coef_limit = 0.01;
    if (coef > coef_limit)
        coef = coef_limit;
    return coef;
}

DataHandler::DataHandler(){
    SetSystemValues ();
    size_of_data = min_size;
    db = new ClickhouseHandler();
    // + 3 cause totalCpu / RAM / swap / (Wi-Fi * 2)
    this->dimensions = amountOfCores + 5;
    SetOptimalSizeForTable ();
    PrepareData ();
    PresetColors ();
}

void DataHandler::SetSystemValues (){
    // get amount of cores & print to file
    system("bash presetting.sh");
    // get data from file
    std::ifstream file;
    file.open ("preset.txt");
    file >> amountOfCores;
    for (int i = 0 ; i < 2; i++)
        file >> max_memory_values[i];
    file.close ();
    // delete file
    system ("rm preset.txt");
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
    system("bash 01_05_RAM.sh && bash 01_05_NET.sh && bash 01_05_CPU.sh");
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

    QString res;
    for (int i = 0 ; i < dimensions; i++){
        QString str;
        QString num;
        str = (i < dimensions - amountOfCores) ? str_to_append[i] : str_to_append[5].arg (i - dimensions + amountOfCores);
        res.append (str);
        res.append ("\t");
        num = num.setNum (newData[i],'p',2);
        res.append (num);
        if (i != dimensions - 1)
            res.append ("\n");
    }
    db->InsertData (res);
}

void DataHandler::ExpandDataSet(int percent){
    size_of_data = percent * (max_size - min_size) * 0.01 + min_size;
}

void DataHandler::DrawCPUCoresInfo(QPainter *painter,QRectF rect){
    qreal x_start,y_start,width,height;
    rect.getRect (&x_start,&y_start,&width,&height);

    int offset = 15,columns,rows;
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
    for (int i = 0; i < rows; i++)
        for (int j = 0 ; j < columns; j++){
            if (i * columns + j >= amountOfCores)
                return;
            int core_id = i * columns + j + dimensions - amountOfCores;
            QPen pen(QBrush(QColor(2.55 * data[core_id][max_size-1],255 - 2.55 *data[core_id][max_size-1] ,0, 30 + 2 * data[core_id][max_size-1])),offset);
            painter->setPen (pen);
            QRectF current_rect(x_start + offset + j * x_grid_step,y_start + offset + i * y_grid_step,
                                x_grid_step - 2*offset,y_grid_step - 2*offset);

            painter->setBrush (QBrush(QColor(2.55 * data[core_id][max_size-1],255 - 2.55 *data[core_id][max_size-1] ,0, 55 + 2 * data[core_id][max_size-1])));
            painter->drawRect (current_rect);
            QString str;
            str.setNum (data[core_id][max_size - 1],'p',2);
            QFont font;
            double min_grid_step = x_grid_step > y_grid_step ? y_grid_step : x_grid_step;
            font.setPixelSize (min_grid_step / str.length ());
            painter->setFont (font);
            painter->setPen (QColor(Qt::white));
            painter->drawText (current_rect,str,QTextOption(Qt::AlignCenter));
        }
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

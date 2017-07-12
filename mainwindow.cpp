#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QtMath"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit_info->insertPlainText("----- START -----\n");
    ui->label_text->setText("image location is not selected");
    ui->checkBox_preview->setChecked(true);
    ui->scrollArea_image->hide();

    prev = true;
    scale = 1;

    //preparing to DAUBECHIES
    lpf[0] = (1+sqrt(3))/(4*sqrt(2));
    lpf[1] = (3+sqrt(3))/(4*sqrt(2));
    lpf[2] = (3-sqrt(3))/(4*sqrt(2));
    lpf[3] = (1-sqrt(3))/(4*sqrt(2));

    hpf[0] =   lpf[3];
    hpf[1] = - lpf[2];
    hpf[2] =   lpf[1];
    hpf[3] = - lpf[0];

    //выделение памяти под матрицу преобразованных коэффициентов
    current_image = new float**[512];
    processed_image = new float**[512];
    reduced_image = new float**[512];

    for(int i = 0; i < 512; i++)
    {
        processed_image[i] = new float*[512];
        reduced_image[i] = new float*[512];
        current_image[i] = new float*[512];

        for(int j = 0; j < 512; j++)
        {
        processed_image[i][j] = new float[3];
        reduced_image[i][j] = new float[3];
        current_image[i][j] = new float[3];
        };

    };




    /*// connect buttons
    connect(ui->pushButton_tryagain, SIGNAL(pressed()), this, SLOT(handle_try_again()));*/
    connect(ui->pushButton_select, SIGNAL(pressed()), this, SLOT(handle_open_file()));
    connect(ui->pushButton_load, SIGNAL(pressed()), this, SLOT(handle_load()));
    connect(ui->pushButton_process, SIGNAL(pressed()), this, SLOT(handle_process()));
    connect(ui->pushButton_restore, SIGNAL(pressed()), this, SLOT(handle_restore()));
    connect(ui->pushButton_decimate, SIGNAL(pressed()), this, SLOT(handle_decimate()));
    connect(ui->pushButton_antinoise, SIGNAL(pressed()), this, SLOT(handle_antinoise()));
    connect(ui->pushButton_hfdivision, SIGNAL(pressed()), this, SLOT(handle_hfdivision()));
    connect(ui->pushButton_lhtonulls, SIGNAL(pressed()), this, SLOT(handle_lhtonulls()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::handle_open_file(){
    QString str;
    str = QFileDialog::getOpenFileName(0, "Select Image", "..//", "*.jpg *.png *.bmp");
    if (!str.isEmpty())
    {
        imgLocation = str;
        ui->label_text->setText(imgLocation);
    };
    //else do nothing

};

void MainWindow::handle_load(){
    if (!imgLocation.isEmpty())
    {
        img_orig.load(imgLocation);
        img_orig.convertToFormat(QImage::Format_RGB32);
        img_new = img_orig;

        imagelabel = new QLabel;
        imagelabel->setPixmap(QPixmap::fromImage(img_new, Qt::AutoColor));

        ui->scrollArea_image->setBackgroundRole(QPalette::Dark);
        ui->scrollArea_image->setWidget(imagelabel);
        ui->scrollArea_image->show();/**/

        refresh();
    }
    else
    {
        ui->plainTextEdit_info->insertPlainText("image is not selected\n");

    };

};


void MainWindow::refresh(){
    QString str;

    if (prev)
    {
     img_viev = img_new;
     w_pixmap = scale*img_new.width();
     h_pixmap = scale*img_new.height();

     str = "Current size is " + QString::number(img_new.width());
     str+="x"+QString::number(img_new.height());
    }
     else
    {
     img_viev = img_orig;
     w_pixmap = scale*img_orig.width();
     h_pixmap = scale*img_orig.height();

     str = "Current size is " + QString::number(img_orig.width());
     str+="x"+QString::number(img_orig.height());

    };


     img_viev = img_viev.scaled(w_pixmap,h_pixmap);


     imagelabel->setPixmap(QPixmap::fromImage(img_viev, Qt::AutoColor));

     ui->label_text->setText(str);
};

void MainWindow::daubechies_horizontal(int array_size){
    int size = array_size / 2;
    float p0, p1, p2, p3;
    float val_red_h, val_grn_h, val_blu_h;
    float val_red_l, val_grn_l, val_blu_l;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < array_size; j++)
        {
         p0 = current_image[i*2                   ][j][0];
         p1 = current_image[i*2 +1                ][j][0];
         p2 = current_image[(i*2 + 2) % array_size][j][0];
         p3 = current_image[(i*2 + 3) % array_size][j][0];


         val_red_h = hpf[0]*p0  + hpf[1]*p1   + hpf[2]*p2  + hpf[3]*p3;
         val_red_l = lpf[0]*p0  + lpf[1]*p1   + lpf[2]*p2  + lpf[3]*p3;

         p0 = current_image[i*2                   ][j][1];
         p1 = current_image[i*2 +1                ][j][1];
         p2 = current_image[(i*2 + 2) % array_size][j][1];
         p3 = current_image[(i*2 + 3) % array_size][j][1];


         val_grn_h = hpf[0]*p0  + hpf[1]*p1   + hpf[2]*p2  + hpf[3]*p3;
         val_grn_l = lpf[0]*p0  + lpf[1]*p1   + lpf[2]*p2  + lpf[3]*p3;

         p0 = current_image[i*2                   ][j][2];
         p1 = current_image[i*2 +1                ][j][2];
         p2 = current_image[(i*2 + 2) % array_size][j][2];
         p3 = current_image[(i*2 + 3) % array_size][j][2];


         val_blu_h = hpf[0]*p0  + hpf[1]*p1   + hpf[2]*p2  + hpf[3]*p3;
         val_blu_l = lpf[0]*p0  + lpf[1]*p1   + lpf[2]*p2  + lpf[3]*p3;

         processed_image[i+size][j][0] = val_red_h;
         processed_image[i+size][j][1] = val_grn_h;
         processed_image[i+size][j][2] = val_blu_h;

         processed_image[i][j][0] = val_red_l;
         processed_image[i][j][1] = val_grn_l;
         processed_image[i][j][2] = val_blu_l;

        }; // end for j
    } // end for i
};


void MainWindow::daubechies_vertical(int array_size){
    int size = array_size / 2;
    float p0, p1, p2, p3;
    float val_red_h, val_grn_h, val_blu_h;
    float val_red_l, val_grn_l, val_blu_l;

    for(int j = 0; j < array_size; j++)
    {
        for(int i = 0; i < size; i++)
        {
         p0 = current_image[j][i*2                   ][0];
         p1 = current_image[j][i*2 +1                ][0];
         p2 = current_image[j][(i*2 + 2) % array_size][0];
         p3 = current_image[j][(i*2 + 3) % array_size][0];


         val_red_h = hpf[0]*p0  + hpf[1]*p1   + hpf[2]*p2  + hpf[3]*p3;
         val_red_l = lpf[0]*p0  + lpf[1]*p1   + lpf[2]*p2  + lpf[3]*p3;

         p0 = current_image[j][i*2                   ][1];
         p1 = current_image[j][i*2 +1                ][1];
         p2 = current_image[j][(i*2 + 2) % array_size][1];
         p3 = current_image[j][(i*2 + 3) % array_size][1];


         val_grn_h = hpf[0]*p0  + hpf[1]*p1   + hpf[2]*p2  + hpf[3]*p3;
         val_grn_l = lpf[0]*p0  + lpf[1]*p1   + lpf[2]*p2  + lpf[3]*p3;

         p0 = current_image[j][i*2                   ][2];
         p1 = current_image[j][i*2 +1                ][2];
         p2 = current_image[j][(i*2 + 2) % array_size][2];
         p3 = current_image[j][(i*2 + 3) % array_size][2];


         val_blu_h = hpf[0]*p0  + hpf[1]*p1   + hpf[2]*p2  + hpf[3]*p3;
         val_blu_l = lpf[0]*p0  + lpf[1]*p1   + lpf[2]*p2  + lpf[3]*p3;

         processed_image[j][i+size][0] = val_red_h;
         processed_image[j][i+size][1] = val_grn_h;
         processed_image[j][i+size][2] = val_blu_h;

         processed_image[j][i][0] = val_red_l;
         processed_image[j][i][1] = val_grn_l;
         processed_image[j][i][2] = val_blu_l;

        }; // end for j
    } // end for i
};

void MainWindow::qImage_to_float(){
    int width, height;
    width = img_orig.width();
    height = img_orig.height();

    if (width > 512)  width = 512;
    if (height > 512) height = 512;

    int red, grn, blu;

    for(int i = 0; i < width; i++)
    for(int j = 0; j < height; j++)
    {
        red = img_orig.pixelColor(i, j).red();
        grn = img_orig.pixelColor(i, j).green();
        blu = img_orig.pixelColor(i, j).blue();

        current_image[i][j][0] = red;
        current_image[i][j][1] = grn;
        current_image[i][j][2] = blu;

    };
};

void MainWindow::copy_processed_to_current(int size){

    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    {
        current_image[i][j][0] = processed_image[i][j][0];
        current_image[i][j][1] = processed_image[i][j][1];
        current_image[i][j][2] = processed_image[i][j][2];
    };

};

void MainWindow::convert_processed_to_new_image(){
    img_new.scaled(512, 512);
    int red, blu, grn;
    QColor one_point;

    for(int i = 0; i < 512; i++)
    for(int j = 0; j < 512; j++)
    {
        red = floor(abs(processed_image[i][j][0]));
        grn = floor(abs(processed_image[i][j][1]));
        blu = floor(abs(processed_image[i][j][2]));

        if (red > 255) red = 255;
        if (grn > 255) grn = 255;
        if (blu > 255) blu = 255;

        if (red < 0) red = 0;
        if (grn < 0) grn = 0;
        if (blu < 0) blu = 0;

        one_point.setRgb(red, grn, blu);

        img_new.setPixelColor(i, j, one_point);
    };
};

void MainWindow::handle_process(){
    qImage_to_float();

    QString name;
    //set name
    name = "/home/amata/proc_imgs/Original.bmp";
    img_new.save(name,0,100);


    int size = 512;
    int steps = ui->spinBox_steps->value();
    last_steps = steps;

    for(int curr_step = 0; curr_step < steps; curr_step ++)
    {
        last_size = size;

        //daubechies horizontal
        daubechies_horizontal(size);

        copy_processed_to_current(size);

        convert_processed_to_new_image();

        //set name
        name = "/home/amata/proc_imgs/Processed_step_";
        name += QString::number(curr_step);
        name += "_a_horizontal.bmp";

        img_new.save(name,0,100);

        //daubecies vertical
        daubechies_vertical(size);

        copy_processed_to_current(size);

        convert_processed_to_new_image();

        //set name
        name = "/home/amata/proc_imgs/Processed_step_";
        name += QString::number(curr_step);
        name += "_b_vertical.bmp";

        img_new.save(name,0,100);

        size = size / 2;

    }; // end of main cycle

};

void MainWindow::restore_horizontal(int array_size){
    int size = array_size;
    int half = size/2;

    float p0, p1, p2, p3;
    float val_red_h, val_grn_h, val_blu_h;
    float val_red_l, val_grn_l, val_blu_l;

    mix_rows(array_size);

    for(int i = 0; i < half; i++)
    {
        for(int j = 0; j < array_size; j++)
        {

         p0 = current_image[(i*2-2+size) % size   ][j][0];
         p1 = current_image[(i*2-1+size) % size   ][j][0];
         p2 = current_image[ i*2     ][j][0];
         p3 = current_image[ i*2 + 1 ][j][0];


         val_red_h = hpf[0]*p0  + hpf[3]*p1   + hpf[2]*p2  + hpf[1]*p3;
         val_red_l = lpf[2]*p0  + lpf[1]*p1   + lpf[0]*p2  + lpf[3]*p3;

         p0 = current_image[(i*2-2+size) % size   ][j][1];
         p1 = current_image[(i*2-1+size) % size   ][j][1];
         p2 = current_image[ i*2     ][j][1];
         p3 = current_image[ i*2 + 1 ][j][1];


         val_grn_h = hpf[0]*p0  + hpf[3]*p1   + hpf[2]*p2  + hpf[1]*p3;
         val_grn_l = lpf[2]*p0  + lpf[1]*p1   + lpf[0]*p2  + lpf[3]*p3;

         p0 = current_image[(i*2-2+size) % size   ][j][2];
         p1 = current_image[(i*2-1+size) % size   ][j][2];
         p2 = current_image[ i*2     ][j][2];
         p3 = current_image[ i*2 + 1 ][j][2];


         val_blu_h = hpf[0]*p0  + hpf[3]*p1   + hpf[2]*p2  + hpf[1]*p3;
         val_blu_l = lpf[2]*p0  + lpf[1]*p1   + lpf[0]*p2  + lpf[3]*p3;

         processed_image[i*2+1][j][0] = val_red_h;
         processed_image[i*2+1][j][1] = val_grn_h;
         processed_image[i*2+1][j][2] = val_blu_h;

         processed_image[i*2][j][0] = val_red_l;
         processed_image[i*2][j][1] = val_grn_l;
         processed_image[i*2][j][2] = val_blu_l;

        }; // end for j
    }; // end for i
};


void MainWindow::restore_vertical(int array_size){
    int size = array_size;
    int half = size/2;

    float p0, p1, p2, p3;
    float val_red_h, val_grn_h, val_blu_h;
    float val_red_l, val_grn_l, val_blu_l;

    mix_columns(array_size);

    for(int j = 0; j < array_size; j++)
    {
        for(int i = 0; i < half; i++)
        {

         p0 = current_image[j][(i*2-2+size) % size   ][0];
         p1 = current_image[j][(i*2-1+size) % size   ][0];
         p2 = current_image[j][ i*2     ][0];
         p3 = current_image[j][ i*2 + 1 ][0];


         val_red_h = hpf[0]*p0  + hpf[3]*p1   + hpf[2]*p2  + hpf[1]*p3;
         val_red_l = lpf[2]*p0  + lpf[1]*p1   + lpf[0]*p2  + lpf[3]*p3;

         p0 = current_image[j][(i*2-2+size) % size   ][1];
         p1 = current_image[j][(i*2-1+size) % size   ][1];
         p2 = current_image[j][ i*2     ][1];
         p3 = current_image[j][ i*2 + 1 ][1];


         val_grn_h = hpf[0]*p0  + hpf[3]*p1   + hpf[2]*p2  + hpf[1]*p3;
         val_grn_l = lpf[2]*p0  + lpf[1]*p1   + lpf[0]*p2  + lpf[3]*p3;

         p0 = current_image[j][(i*2-2+size) % size   ][2];
         p1 = current_image[j][(i*2-1+size) % size   ][2];
         p2 = current_image[j][ i*2     ][2];
         p3 = current_image[j][ i*2 + 1 ][2];


         val_blu_h = hpf[0]*p0  + hpf[3]*p1   + hpf[2]*p2  + hpf[1]*p3;
         val_blu_l = lpf[2]*p0  + lpf[1]*p1   + lpf[0]*p2  + lpf[3]*p3;

         processed_image[j][i*2+1][0] = val_red_h;
         processed_image[j][i*2+1][1] = val_grn_h;
         processed_image[j][i*2+1][2] = val_blu_h;

         processed_image[j][i*2][0] = val_red_l;
         processed_image[j][i*2][1] = val_grn_l;
         processed_image[j][i*2][2] = val_blu_l;

        }; // end for j
    }; // end for i

};


void MainWindow::handle_restore(){

    QString name;


    int size = last_size;
    int steps = last_steps;

    last_steps = steps;

    for(int curr_step = 0; curr_step < steps; curr_step ++)
    {
        //restore vertical
        restore_vertical(size);

        copy_processed_to_current(size);

        convert_processed_to_new_image();

        //set name
        name = "/home/amata/proc_imgs/Restored_step_";
        name += QString::number(curr_step);
        name += "_a_vertical.bmp";

        img_new.save(name,0,100);


        //restore horizontal
        restore_horizontal(size);

        copy_processed_to_current(size);

        convert_processed_to_new_image();

        //set name
        name = "/home/amata/proc_imgs/Restored_step_";
        name += QString::number(curr_step);
        name += "_b_horizontal.bmp";

        img_new.save(name,0,100);

        size *= 2;

    }; // end of main cycle

};


void MainWindow::mix_rows(int array_size){
    int size = array_size /2;

    for(int i = 0; i < size; i++)
        for(int j = 0; j < array_size; j++)
        for(int k = 0; k < 3; k++)
        {
            processed_image[i*2  ][j][k] = current_image[i     ][j][k];
            processed_image[i*2+1][j][k] = current_image[i+size][j][k];

        };

    copy_processed_to_current(array_size);
};


void MainWindow::mix_columns(int array_size){
    int size = array_size /2;

    for(int j = 0; j < array_size; j++)
        for(int i = 0; i < size; i++)
        for(int k = 0; k < 3; k++)
        {
            processed_image[j][i*2  ][k] = current_image[j][i     ][k];
            processed_image[j][i*2+1][k] = current_image[j][i+size][k];

        };

    copy_processed_to_current(array_size);
};


void MainWindow::handle_decimate(){
    float level = ui->doubleSpinBox_decimate->value();
    float val;
    int size = last_size / 2;
    int counter = 0;

    for(int i = size; i < 512; i++)
    for(int j = 0; j < 512; j++)
    for(int k = 0; k < 3; k++)
    {
        val = current_image[i][j][k];

        if (abs(val) < level)
        {
            current_image[i][j][k] = 0;
            counter += 1;
        };

    };

    for(int i = 0; i < size; i++)
    for(int j = size; j < 512; j++)
    for(int k = 0; k < 3; k++)
    {
        val = current_image[i][j][k];

        if (abs(val) < level)
        {
            current_image[i][j][k] = 0;
            counter += 1;
        };
    };

    QString str = "There are ";
    str += QString::number(counter);
    str += " nulls now\n";
    ui->plainTextEdit_info->insertPlainText("Decimation finished\n");
    ui->plainTextEdit_info->insertPlainText(str);

    str = "Nulls: ";
    int percent = counter*100 / (512*512*3);
    str += QString::number(percent);
    str +="% \n";

    ui->plainTextEdit_info->insertPlainText(str);

};

void MainWindow::handle_antinoise(){
    float level = ui->doubleSpinBox_antinoise->value();
    float val;
    int size = last_size / 2;
    int counter = 0;

    for(int i = size; i < 512; i++)
    for(int j = 0; j < 512; j++)
    for(int k = 0; k < 3; k++)
    {
        val = current_image[i][j][k];

        if (abs(val) > level)
        {
            current_image[i][j][k] = val*(level/abs(val));
            counter += 1;
        };

    };

    for(int i = 0; i < size; i++)
    for(int j = size; j < 512; j++)
    for(int k = 0; k < 3; k++)
    {
        val = current_image[i][j][k];

        if (abs(val) < level)
        {
            current_image[i][j][k] = 0;
            counter += 1;
        };
    };

    QString str = "There are ";
    str += QString::number(counter);
    str += " leveled coeffs\n";
    ui->plainTextEdit_info->insertPlainText("Anti-noise filtration finished\n");
    ui->plainTextEdit_info->insertPlainText(str);


};




void MainWindow::handle_hfdivision(){
    float level = ui->doubleSpinBox_hfdivision->value();
    float val;
    int size = last_size / 2;
    int counter = 0;

    for(int i = size; i < 512; i++)
    for(int j = 0; j < 512; j++)
    for(int k = 0; k < 3; k++)
    {
        val = current_image[i][j][k];


        current_image[i][j][k] = val/level;
        counter += 1;


    };

    for(int i = 0; i < size; i++)
    for(int j = size; j < 512; j++)
    for(int k = 0; k < 3; k++)
    {
        val = current_image[i][j][k];

        if (abs(val) < level)
        {
            current_image[i][j][k] = 0;
            counter += 1;
        };
    };

    QString str = "There are ";
    str += QString::number(counter);
    str += " divided coeffs\n";
    ui->plainTextEdit_info->insertPlainText("Division filtration finished\n");
    ui->plainTextEdit_info->insertPlainText(str);


};

void MainWindow::handle_lhtonulls(){
    int size = last_size / 2;
    int counter = 0;

    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    for(int k = 0; k < 3; k++)
    {
        current_image[i][j][k] = 255;
        counter += 1;
    };


    QString str = "There are ";
    str += QString::number(counter);
    str += " turned to 255 lh coeffs\n";
    ui->plainTextEdit_info->insertPlainText("LH filtration finished\n");
    ui->plainTextEdit_info->insertPlainText(str);


};

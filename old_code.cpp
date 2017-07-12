#include "old_code.h"

old_code::old_code()
{

}


/*JUST SOME OLD CODE*/

/* OLD CODE */

/*  int size = array_size / 2;
    QColor p0, p1, p2, p3;
    float val_red, val_grn, val_blu;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < array_size; j++)
        {
         p0 = current_image[i*2][j][0];
         p1 = current_image[i*2][j][0];
         p0 = current_image[i*2][j][0];
         p0 = current_image[i*2][j][0];
         p1 = img_orig.pixel(i*2 + 1 ,                j);
         p2 = img_orig.pixel((i*2 + 2) % array_size , j);
         p3 = img_orig.pixel((i*2 + 3) % array_size , j);

         val_red = hpf[0]*p0.red()   + hpf[1]*p1.red()   + hpf[2]*p2.red()   + hpf[3]*p3.red();
         val_grn = hpf[0]*p0.green() + hpf[1]*p1.green() + hpf[2]*p2.green() + hpf[3]*p3.green();
         val_blu = hpf[0]*p0.blue()  + hpf[1]*p1.blue()  + hpf[2]*p2.blue()  + hpf[3]*p3.blue();

         processed_image[i+size][j][0] = val_red;
         processed_image[i+size][j][0] = val_grn;
         processed_image[i+size][j][0] = val_blu;

         val_red = lpf[0]*p0.red()   + lpf[1]*p1.red()   + lpf[2]*p2.red()   + lpf[3]*p3.red();
         val_grn = lpf[0]*p0.green() + lpf[1]*p1.green() + lpf[2]*p2.green() + lpf[3]*p3.green();
         val_blu = lpf[0]*p0.blue()  + lpf[1]*p1.blue()  + lpf[2]*p2.blue()  + lpf[3]*p3.blue();

         processed_image[i][j][0] = val_red;
         processed_image[i][j][0] = val_grn;
         processed_image[i][j][0] = val_blu;

        }; // end for j
    } // end for i*/


/*
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < array_size; j++)
        {
         p0 = current_image[(i*2 -1 + array) % array][j       ][0];
         p1 = current_image[(i*2 -1 + array) % array][j + size][0];
         p2 = current_image[i*2                    ][j       ][0];
         p3 = current_image[i*2                    ][j + size][0];


         val_red_h = hpf[2]*p0  + hpf[1]*p1   + hpf[0]*p2  + hpf[3]*p3;
         val_red_l = lpf[0]*p0  + lpf[3]*p1   + lpf[2]*p2  + lpf[1]*p3;

         p0 = current_image[(i*2 -1 + array) % array       ][j][1];
         p1 = current_image[(i*2 -1 + array) % array + size][j ][1];
         p2 = current_image[i*2                    ][j][1];
         p3 = current_image[i*2 + size             ][j][1];


         val_grn_h = hpf[2]*p0  + hpf[1]*p1   + hpf[0]*p2  + hpf[3]*p3;
         val_grn_l = lpf[0]*p0  + lpf[3]*p1   + lpf[2]*p2  + lpf[1]*p3;;

         p0 = current_image[(i*2 -1 + array) % array       ][j][2];
         p1 = current_image[(i*2 -1 + array) % array + size][j][2];
         p2 = current_image[i*2                    ][j][2];
         p3 = current_image[i*2 + size             ][j][2];


         val_blu_h = hpf[2]*p0  + hpf[1]*p1   + hpf[0]*p2  + hpf[3]*p3;
         val_blu_l = lpf[0]*p0  + lpf[3]*p1   + lpf[2]*p2  + lpf[1]*p3;

         processed_image[i+size][j][0] = val_red_h;
         processed_image[i+size][j][1] = val_grn_h;
         processed_image[i+size][j][2] = val_blu_h;

         processed_image[i][j][0] = val_red_l;
         processed_image[i][j][1] = val_grn_l;
         processed_image[i][j][2] = val_blu_l;
*/

/*for(int j = 0; j < array_size; j++)
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
    }; // end for i*/


/*
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

};*/

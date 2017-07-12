#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QString>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
//image viewing options
    bool prev;
    float scale;

    int w_pixmap;
    int h_pixmap;

    int last_size;
    int last_steps;

    void refresh();

    QImage img_new;
    QImage img_orig;
    QImage img_viev;

    QLabel* imagelabel;
    QString imgLocation;


    Ui::MainWindow *ui;

//daubechies transform
    float *** current_image; //processed without reduction
    float *** processed_image; //processed without reduction
    float *** reduced_image; //processed with reduction

    double lpf[4];
    double hpf[4];

    void daubechies_horizontal(int array_size);
    void daubechies_vertical(int array_size);
    void qImage_to_float();
    void copy_processed_to_current(int array_size = 512);
    void convert_processed_to_new_image();

    void restore_horizontal(int array_size);
    void restore_vertical(int array_size);

    void mix_rows(int array_size);
    void mix_columns(int array_size);

private slots:

    void handle_open_file();
    void handle_load();
    void handle_process();
    void handle_restore();
    void handle_decimate();
    void handle_antinoise();
    void handle_hfdivision();
    void handle_lhtonulls();
};

#endif // MAINWINDOW_H

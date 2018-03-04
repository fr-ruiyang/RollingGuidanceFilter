#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QFileDialog>
#include "RollingGuidanceFilter.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("图像处理.exe"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked() //浏览(获取并显示图片及其路径)
{
        QImage* img = new QImage;
        QString filename;
        filename=QFileDialog::getOpenFileName(this,
                                              tr("选择图像"),
                                              "",
                                              tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
        if(filename.isEmpty())
        {
             return;
        }
        else
        {
            if(! ( img->load(filename) ) ) //加载图像
            {
                QMessageBox::information(this,
                                         tr("打开图像失败"),
                                         tr("打开图像失败!"));
                delete img;
                return;
            }
            QImage* imgScaled = new QImage;
            *imgScaled=img->scaled(ui->picture->width(),
                                   ui->picture->height(),
                                   Qt::KeepAspectRatio);
            ui->picture->setPixmap(QPixmap::fromImage(*imgScaled));
            res = cv::imread(filename.toStdString());
            cv::cvtColor(res, res, CV_BGR2RGB);
            ui->lineEdit->setText(filename);
        }
}

void Widget::on_pushButton_1_clicked() //处理
{
        QImage* newimg = new QImage;
        bool ok;
        int iteration = ui->lineEdit_1->text().toInt(&ok,10);
        float sigma_s= ui->lineEdit_2->text().toFloat();
        float sigma_r= ui->lineEdit_3->text().toFloat();
        QString filename=ui->lineEdit->text();
        cv::Mat img = cv::imread(filename.toStdString());
        res = RollingGuidanceFilter::filter(img,sigma_s,sigma_r,iteration);
        cv::cvtColor(res, res, CV_BGR2RGB);
        *newimg = QImage((const unsigned char*)res.data,  res.cols, res.rows,  res.cols * res.channels(),QImage::Format_RGB888);
        QImage* newimgScaled = new QImage;
        *newimgScaled=newimg->scaled(ui->picture->width(),ui->picture->height(),Qt::KeepAspectRatio);
        ui->picture->setPixmap(QPixmap::fromImage(*newimgScaled));
}

void Widget::on_pushButton_2_clicked() //重置
{
    ui->lineEdit_1->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_1->setFocus();
}

void Widget::on_pushButton_3_clicked()  //保存
{
    QImage* newimg = new QImage;
    /*bool ok;
    int iteration = ui->lineEdit_1->text().toInt(&ok,10);
    float sigma_s= ui->lineEdit_2->text().toFloat();
    float sigma_r= ui->lineEdit_3->text().toFloat();
    QString filename=ui->lineEdit->text();
    cv::Mat img = cv::imread(filename.toStdString());
    Mat res = RollingGuidanceFilter::filter(img,sigma_s,sigma_r,iteration);
    cv::cvtColor(res, res, CV_BGR2RGB);*/
    *newimg = QImage((const unsigned char*)res.data,  res.cols, res.rows,  res.cols * res.channels(),QImage::Format_RGB888);
//    QImage* newimgScaled = new QImage;
//    *newimgScaled=newimg->scaled(ui->picture->width(),ui->picture->height(),Qt::KeepAspectRatio);
//    ui->picture->setPixmap(QPixmap::fromImage(*newimgScaled));
    QString newfilename;
    newfilename=QFileDialog::getSaveFileName(this,
                                             tr("另存为？"),
                                             "untitled.png",
                                             tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
        if(! ( newimg->save(newfilename) ) ) //保存图像
        {
            QMessageBox::warning(this, tr("提示"),tr("保存失败"));
        }
        else
        {
            QMessageBox::warning(this, tr("提示"),tr("保存成功"));
            //ui->lineEdit->setText(newfilename);
        }
}

#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QTimer::singleShot(0,[=](){
        MyFileDialog* dlg = new MyFileDialog(this);
        QString path = dlg->getOpenFileName(this,"打开文件",QCoreApplication::applicationDirPath());
        if(!path.isEmpty())
            QMessageBox::information(this,"",path,"确定");
    });
}

Widget::~Widget()
{
}


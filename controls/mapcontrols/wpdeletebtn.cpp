#include "wpdeletebtn.h"

WPDeleteBtn::WPDeleteBtn(QWidget *parent):QPushButton(parent)
{
    setText("X");
    setGeometry(0,0,60,30);
    connect(this, SIGNAL(clicked()), this, SLOT(btn_clicked()));
}

int WPDeleteBtn::setRow(const int& row){
    m_row = row;
    return true;
}

int WPDeleteBtn::addRow(){
    m_row++;
    return true;
}

int WPDeleteBtn::subtractRow(){
    m_row--;
    return true;
}

void WPDeleteBtn::btn_clicked(){
    emit signal_deleteClicked(m_row);
}


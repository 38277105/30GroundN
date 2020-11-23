#include "wpmvbtn.h"

wpmvbtn::wpmvbtn(WPMVBTN_TYPE m_type, QWidget *parent):m_type(m_type),QPushButton(parent)
{
    if(m_type == WPMVBTN_TYPE(0)){
        this->setText("上移");
    }else if(m_type == WPMVBTN_TYPE(1)){
        this->setText("下移");
    }
    connect(this, SIGNAL(clicked()), this, SLOT(btn_clicked()));
}

int wpmvbtn::setRow(const int& row){
    m_row = row;
    return true;
}

int wpmvbtn::addRow(){
    m_row++;
    return true;
}

int wpmvbtn::subtractRow(){
    m_row--;
    return true;
}

void wpmvbtn::btn_clicked(){
    switch (m_type) {
    case MV_UP:
        emit signal_up(m_row);
        break;
    case MV_DOWN:
        emit signal_down(m_row);
        break;
    default:
        break;
    }
}


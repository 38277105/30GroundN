#include "frmmain.h"
#include <QApplication>
#include "controls/myhelper.h"
#include "controls/commoncontrol/loadingsplash.h"
#include "messagetrans.h"
#include "DbgHelp.h"
#pragma comment(lib, "DbgHelp" )

LoadingSplash * InstallFlash();
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);//程式异常捕获

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<mavlink_message_t>("mavlink_message_t");
    myHelper::SetUTF8Code();
    myHelper::SetStyle("black");//黑色风格
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    SetUnhandledExceptionFilter(ApplicationCrashHandler);

    MessageTrans tran;
    tran.Init();

    frmMain w;
//    w.showMaximized();
    w.show();

    return a.exec();
}

LoadingSplash * InstallFlash()
{
    LoadingSplash *splash = new LoadingSplash(QPixmap(QStringLiteral(":/markers/images/splash.jpg")));
    splash->setMinimumWidth(580);
    splash->setMinimumHeight(386);
    splash->show(); // 显示
    splash->setGeometry(700,300,580,386);

    splash->setProgress(10);// 显示%30
    QThread::msleep(500);

    splash->setProgress(20);
    QThread::msleep(300);

    splash->setProgress(33);// 显示%30
     QThread::msleep(300);

    splash->setProgress(39);
    QThread::msleep(300);
    splash->setProgress(51);// 显示%30
    QThread::msleep(300);

    splash->setProgress(63);
    QThread::msleep(300);
    splash->setProgress(71);// 显示%30
    QThread::msleep(300);

    splash->setProgress(85);
    QThread::msleep(300);
    splash->setProgress(90);// 显示%30
    QThread::msleep(300);

    splash->setProgress(97);
    QThread::msleep(300);
    return splash;

}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){
    //程式异常捕获
    /****保存数据代码****/
    //创建 Dump 文件
    wchar_t wfilepath[1024];
    memset(wfilepath,0,sizeof(wchar_t)*1024);
    QString sTmp=QTime::currentTime().toString("HHmmss");
    sTmp="error_"+sTmp+".dmp";
    sTmp.toWCharArray(wfilepath);
    qDebug()<<wfilepath;
    HANDLE hDumpFile = CreateFile(wfilepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hDumpFile != INVALID_HANDLE_VALUE){
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QString errorcontent= qt_error_string(record->ExceptionCode);

    QMessageBox::critical(NULL,"program crash","<FONT size=4><div><b>sorry to crash !!!</b><br/></div>"+
        QString("<div>error code:%1</div><div>error address:%2</div><div>error descript:%3</div></FONT>").arg(errCode).arg(errAdr).arg(errorcontent),
        QMessageBox::Ok);


    return EXCEPTION_EXECUTE_HANDLER;
}

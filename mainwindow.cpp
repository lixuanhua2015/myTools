#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>
#include <QtMath>
#include <QValidator>
#include <limits>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
    m_timer->start(1000);

    // 禁止最大化按钮
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    // 进制拖动窗口大小
    setFixedSize(this->width(), this->height());

    connect(ui->pushButton_floatToHex, SIGNAL(clicked(bool)), this, SLOT(onFloatToHexSlot()));
    connect(ui->pushButton_HexToFloat, SIGNAL(clicked(bool)), this, SLOT(onHexToFloatSlot()));
    connect(ui->lineEdit_floatHex, SIGNAL(textEdited(QString)), this, SLOT(onFloatHexToDoubleHex()));
    connect(ui->lineEdit_DecNum, SIGNAL(textEdited(QString)), this, SLOT(onDecNumToOtherNum()));
    connect(ui->lineEdit_BinNum, SIGNAL(textEdited(QString)), this, SLOT(onBinNumToOtherNum()));
    connect(ui->lineEdit_OctNum, SIGNAL(textEdited(QString)), this, SLOT(onOctNumToOtherNum()));
    connect(ui->lineEdit_HexNum, SIGNAL(textEdited(QString)), this, SLOT(onHexNumToOtherNum()));
    connect(ui->lineEdit_num, SIGNAL(textEdited(QString)), this, SLOT(onNumToASCIICode()));
    connect(ui->lineEdit_asciiCode, SIGNAL(textEdited(QString)), this, SLOT(onASCIICodeToNum()));
    ui->pushButton_floatToHex->click();
    initMainWindow();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
}

void MainWindow::initMainWindow()
{
    onTimerUpdate();
    // 自适应text的大小
    ui->label_dynamicTime->adjustSize();
    onDecNumToOtherNum();
    onNumToASCIICode();
    // 限制lineEdit输入
    {
        // 限制lineEdit_float输入为double，只显示12为小数
        {
            double maxDouble = numeric_limits<double>::max();
            double minDouble = numeric_limits<double>::max();
            MY_DEBUG << maxDouble << minDouble;
            QDoubleValidator *doubleValidator = new QDoubleValidator (minDouble, maxDouble, 12, this);
            ui->lineEdit_float->setValidator(doubleValidator);
        }
        {
            long maxLong = numeric_limits<long>::max();
            long minLong = numeric_limits<long>::min();
            MY_DEBUG << minLong << maxLong;
            QIntValidator *intValidator = new QIntValidator (minLong, maxLong, this);
            ui->lineEdit_DecNum->setValidator(intValidator);
        }
    }
}

void MainWindow::stringToOtherString(const QString &inputStr, const int &inputTypeInt,
                                     QString &outputStr, const int &outputTypeInt)
{
    long tempLong = 0;
    QByteArray inputByteArr = inputStr.toLatin1();
    int strSize = inputByteArr.size();
    for(int i = 0; i < strSize; ++i) {
        // qPow(x,y)接口是求x的y次方
        char eachValChar = inputByteArr.data()[strSize - 1 - i];
        long eachValLong = (eachValChar - '0');
        if (eachValLong > 9) {
            if (eachValChar >= 'A' && eachValChar <= 'F') {
                eachValLong = eachValChar - 'A' + 10;
            } else if (eachValChar >= 'a' && eachValChar <= 'f') {
                eachValLong = eachValChar - 'a' + 10;
            }
        }
        tempLong += eachValLong * (qPow(inputTypeInt, i));
    }
    outputStr = QString::number(tempLong, outputTypeInt);
}

void MainWindow::onFloatToHexSlot()
{
    // 方法二
    /*
     *float a = ui->lineEdit_float->text().toFloat();
     *char b[4];
     *memcpy(b, &a, 4);
    */
    // 将浮点数用float和double存储
    tempFloat.a = ui->lineEdit_float->text().toFloat();
    tempDouble.a = ui->lineEdit_float->text().toDouble();
    // float转换16进制
    QString floatToHex = "";
    for (int i = 0; i < sizeof(float); ++i) {
        uchar tempChar = tempFloat.b[sizeof(float) - 1 - i];
        if (tempChar < 16) {
            floatToHex += "0";
        }
        floatToHex += QString::number(tempChar, 16);
    }
    // double转换16进制
    QString doubleToHex = "";
    for (int i = 0; i < sizeof(double); ++i) {
        uchar tempChar = tempDouble.b[sizeof(double) - 1 - i];
        if (tempChar < 16) {
            doubleToHex += "0";
        }
        doubleToHex += QString::number(tempChar, 16);
    }
    ui->lineEdit_floatHex->setText(floatToHex.toUpper());
    ui->lineEdit_doubleHex->setText(doubleToHex.toUpper());
    MY_DEBUG << floatToHex << doubleToHex;
}

void MainWindow::onHexToFloatSlot()
{
    QByteArray floatBtArr = QByteArray::fromHex(ui->lineEdit_floatHex->text().toLatin1());
    for(int i = 0; i < floatBtArr.size(); ++i) {
        tempFloat.b[floatBtArr.size() - 1 - i] = floatBtArr.data()[i];
    }
    // 将float显示为12位有效小数
    ui->lineEdit_float->setText(QString::number(tempFloat.a, 'f', 12));
}

void MainWindow::onTimerUpdate()
{
    QDateTime curTime = QDateTime::currentDateTime();
    QString curTimeStr = curTime.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->label_dynamicTime->setText(curTimeStr);
}

void MainWindow::onFloatHexToDoubleHex()
{
    QByteArray floatBtArr = QByteArray::fromHex(ui->lineEdit_floatHex->text().toLatin1());
    for(int i = 0; i < floatBtArr.size(); ++i) {
        tempFloat.b[floatBtArr.size() - 1 - i] = floatBtArr.data()[i];
    }
    tempDouble.a = QString::number(tempFloat.a, 'f', 12).toDouble();
    // double转换16进制
    QString doubleToHex = "";
    for (int i = 0; i < sizeof(double); ++i) {
        uchar tempChar = tempDouble.b[sizeof(double) - 1 - i];
        if (tempChar < 16) {
            doubleToHex += "0";
        }
        doubleToHex += QString::number(tempChar, 16);
    }
    ui->lineEdit_doubleHex->setText(doubleToHex.toUpper());
}

void MainWindow::onDecNumToOtherNum()
{
    long decNumLong = ui->lineEdit_DecNum->text().toLong();
    // 十进制转二进制
    ui->lineEdit_BinNum->setText(QString::number(decNumLong, 2));
    // 十进制转八进制
    ui->lineEdit_OctNum->setText(QString::number(decNumLong, 8));
    // 十进制转十六进制
    ui->lineEdit_HexNum->setText(QString::number(decNumLong, 16));
}

void MainWindow::onBinNumToOtherNum()
{
    QString binNumStr = ui->lineEdit_BinNum->text();
    // 二进制转换十进制
    QString decNum = "";
    stringToOtherString(binNumStr, 2, decNum, 10);
    ui->lineEdit_DecNum->setText(decNum);
    // 二进制转换八进制
    QString OctNum = "";
    stringToOtherString(binNumStr, 2, OctNum, 8);
    ui->lineEdit_OctNum->setText(OctNum);
    // 二进制转换十六进制
    QString hexNum = "";
    stringToOtherString(binNumStr, 2, hexNum, 16);
    ui->lineEdit_HexNum->setText(hexNum);
}

void MainWindow::onOctNumToOtherNum()
{
    QString octNumStr = ui->lineEdit_OctNum->text();
    // 八进制转换十进制
    QString decNum = "";
    stringToOtherString(octNumStr, 8, decNum, 10);
    ui->lineEdit_DecNum->setText(decNum);
    // 八进制转换二进制
    QString OctNum = "";
    stringToOtherString(octNumStr, 8, OctNum, 2);
    ui->lineEdit_BinNum->setText(OctNum);
    // 八进制转换十六进制
    QString hexNum = "";
    stringToOtherString(octNumStr, 8, hexNum, 16);
    ui->lineEdit_HexNum->setText(hexNum);
}

void MainWindow::onHexNumToOtherNum()
{
    QString octNumStr = ui->lineEdit_HexNum->text();
    // 十六进制转换十进制
    QString decNum = "";
    stringToOtherString(octNumStr, 16, decNum, 10);
    ui->lineEdit_DecNum->setText(decNum);
    // 十六进制转换二进制
    QString OctNum = "";
    stringToOtherString(octNumStr, 16, OctNum, 2);
    ui->lineEdit_BinNum->setText(OctNum);
    // 十六进制转换八进制
    QString hexNum = "";
    stringToOtherString(octNumStr, 16, hexNum, 8);
    ui->lineEdit_OctNum->setText(hexNum);
}

void MainWindow::onNumToASCIICode()
{
    char numChar = ui->lineEdit_num->text().toInt() & 0xff;
    if (numChar <= 127) {
        QByteArray numByteArr(1,numChar);
        ui->lineEdit_asciiCode->setText(numByteArr);
    }

}

void MainWindow::onASCIICodeToNum()
{
    QByteArray asciiCodeByteArr = ui->lineEdit_asciiCode->text().toLatin1();
    char numChar = asciiCodeByteArr.data()[0];
    ui->lineEdit_num->setText(QString::number(numChar,10));
}

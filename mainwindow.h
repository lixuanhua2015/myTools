#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "parambase.h"

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
    /**
     * @brief initMainWindow 初始化主窗口
     */
    void initMainWindow();
    /**
     * @brief stringToOtherString 数值的进制转换
     * @param inputStr 需要转换的数值字符串
     * @param inputTypeInt 需要转换的进制类型
     * @param outputStr 转换后的数字字符串
     * @param outputTypeInt 转换后的进制类型
     */
    void stringToOtherString(const QString &inputStr, const int &inputTypeInt,
                             QString &outputStr, const int &outputTypeInt);

private slots:
    /**
     * @brief onFloatToHexSlot float转换为hex的槽函数
     */
    void onFloatToHexSlot();
    /**
     * @brief onHexToFloatSlot hex转换为float的槽函数
     */
    void onHexToFloatSlot();
    /**
     * @brief onTimerUpdate 更新当前时间的槽函数
     */
    void onTimerUpdate();
    /**
     * @brief onFloatHexToDoubleHex float十六进制转换为double十六进制
     */
    void onFloatHexToDoubleHex();
    /**
     * @brief onDecNumToOtherNum 十进制数转换为其他进制数
     */
    void onDecNumToOtherNum();
    /**
     * @brief onBinNumToOtherNum 二进制数转换为其他进制数
     */
    void onBinNumToOtherNum();
    /**
     * @brief onOctNumToOtherNum 八进制数转换为其他进制数
     */
    void onOctNumToOtherNum();
    /**
     * @brief onHexNumToOtherNum 十六进制数转换为其他进制数
     */
    void onHexNumToOtherNum();
    /**
     * @brief onNumToASCIICode 数值转ASCII码
     */
    void onNumToASCIICode();
    /**
     * @brief onASCIICodeToNum ASCII码转数值
     */
    void onASCIICodeToNum();

private:
    Ui::MainWindow *ui;
    /**
     * @brief m_timer 控制更新系统时间的定时器
     */
    QTimer *m_timer;
    /**
     * @brief The Anonymous:1 union 一个float转hex通用的共同体
     */
    union {
        float a;
        uchar b[4];
    }tempFloat;
    /**
     * @brief The Anonymous:1 union 一个double转hex通用共同体
     */
    union {
        double a;
        uchar b[8];
    }tempDouble;
};

#endif // MAINWINDOW_H

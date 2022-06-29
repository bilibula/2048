#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <random>
#include <QKeyEvent>
#include <QDebug>
#include <QTest>
#include <QTime>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <cmath>
#include <QFile>
#include <QCoreApplication>
#include <QPainter>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QSoundEffect>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QDesktopWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


private:
    //基础信息
    QString workingPath;                                //游戏工作路径
    const QString myTab = "    ";                       //Tab
    int windowWidth, windowHeight, volume, textSize;    //游戏设置记录
    char* prevBoard;                                    //用来记录过去的局面，最多记录两千步
    int* prevScore;                                     //用来记录过去的分数，最多记录两千步
    int startNum = 0;                                   //游戏开始次数
    int turn = 0;                                       //游戏进行的轮数
    int recordPos = 0;                                  //prevBoard、prevScore中记录到的步数
    int achieved = 0;                                   //记录哪些成就完成了，是什么颜色。每个成就占据两位，
                                                        //第一位代表是否完成过，第二位代表完成的颜色
    bool isBack = false;                                //本次对局中是否用过撤销


    //游戏整体布局控件
    QStackedWidget* stackWidget;                        //分页控件
    QWidget* gameWidget;                                //桌面页
    QWidget* setWidget;                                 //设置页
    QWidget* helpWidget;                                //帮助页
    QWidget* settingWidget;                             //游戏设置页
    QWidget* achievementWidget;                         //成就页


    //桌面控件
    char* numBoard;                                     //记录当前游戏局面
    QLabel* viewBoard;                                  //显示当前游戏局面
    int score = 0;                                      //记录当前分数
    const QString scorePrefix = "Score           ";     //游戏中打印分数的前缀
    QLabel* scoreLabel;                                 //显示分数的标签
    int highScore = 0;                                  //记录最高分数
    const QString highScoreProfix = "High Score   ";    //游戏中打印最高分数的前缀
    QLabel* highScoreLabel;                             //显示最高分数的标签
    QPushButton* backBtn;                               //撤回按钮
    QPushButton* achievementBtn;                        //成就按钮
    QPushButton* setBtn;                                //设置按钮
    QLabel* achievementCompleteLabel;                   //成就完成时显示的标签


    //设置页面控件
    QPushButton* resumeBtn;                             //继续游戏按钮
    QPushButton* restartBtn;                            //重新开始按钮
    QPushButton* settingBtn;                            //游戏设置按钮
    QPushButton* helpBtn;                               //游戏帮助按钮
    QPushButton* quitBtn;                               //退出游戏按钮


    //帮助页面控件
    QLabel* helpLabel;                                  //显示游戏帮助的标签
    QPushButton* helpReturnBtn;                         //从帮助页面返回设置页面的按钮


    //游戏设置页面控件
    QLabel* resolutionLabel;                            //提示分辨率调整的标签
    QComboBox* resolutionBox;                           //调整分辨率的控件
    QLabel* textSizeLabel;                              //提示字体大小调整的标签
    QComboBox* textSizeBox;                             //调整字体大小的控件
    QLabel* soundVolumeLabel;                           //提示声音大小调整的标签
    QSpinBox* soundVolumeBox;                           //调整声音大小的控件
    QSlider* soundVolumeSlider;                         //调整声音大小的控件
    QPushButton* settingReturnBtn;                      //从游戏设置页面返回设置页面的按钮


    //游戏成就页面控件
    QLabel* achievementLabel;                           //提示成就页面功能的标签
    QListWidget* achievementList;                       //成就列表
    QPushButton* achievementReturnBtn;                  //从成就页面返回设置页面的按钮


    //游戏声音
    QSoundEffect* composeSound;                         //方块合成时的音效
    QSoundEffect* moveSound;                            //方块移动时的音效
    QSoundEffect* gameOverSound;                        //游戏结束时的音效
    QSoundEffect* cantMoveSound;                        //无法移动时的音效
public:
    explicit MainWindow(QWidget *parent = nullptr);     //构造函数
    ~MainWindow();                                      //析构函数

    void createBoard();                                 //构造游戏桌面
    QPixmap image(int no = 0);                          //返回适当大小的数字图片
    void randomFill(int num = 1);                       //在棋盘随机的num个位置填入随机的2或4
    void keyPressEvent(QKeyEvent* event);               //重载键盘按下的事件
    int move(char* board);                              //移动的核心代码。将游戏盘向右移动，并返回得分。
                                                        //向其他方向移动时先将棋盘旋转，调用该函数之后再旋转回去。
    std::pair<bool, int> moveRight();                   //向右移动，返回棋盘是否变化和得分
    std::pair<bool, int> moveLeft();                    //原理同上
    std::pair<bool, int> moveUp();                      //原理同上
    std::pair<bool, int> moveDown();                    //原理同上
    void recordBoard();                                 //玩家行动完一步之后记录当前局面
    bool isEnd();                                       //判断游戏是否结束
    void end();                                         //游戏结束时调用的函数
    void restart();                                     //重新开始游戏
    void updateViewBoard();                             //更新玩家看到的游戏盘
    void addScore(int n);                               //加分
    void back();                                        //撤回
    void paintEvent(QPaintEvent*);                      //重载程序开始时画图的事件
    void closeEvent(QCloseEvent*);                      //重载用户按下红叉结束程序时的事件
    void quit();                                        //退出游戏
    void loadFile(QString filePath);                    //加载设置文件与存档文件
    void setResolution(QString text);                   //设置分辨率
    void setVolume(int soundVolume);                    //设置声音大小
    void setTextSize(int size);                         //设置字体大小
    QByteArray numberCode(QByteArray num);              //返回加密后的数字
    QByteArray numberDecode(QByteArray num);            //返回解密后的数字
    bool isAchieved(int no);                            //返回第no个成就是否已完成
    void showAchievementComplete(int no, bool isGold);  //成就完成后的展示

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //设置游戏标题、图片
    setWindowTitle("2048");
    setWindowIcon(QIcon(":/2048image/windowIcon.png"));


    //读取当前电脑分辨率、当前程序运行位置
    int currentScreenHeight = QApplication::desktop()->height();
    workingPath = QCoreApplication::applicationDirPath();


    //读入设置文件中的内容
    QString settingPath = workingPath + "//settings.txt";
    QFile settingFile(settingPath);
    if(!settingFile.exists()){
        settingFile.open(QIODevice::WriteOnly | QIODevice::Text);
        settingFile.write(QString::number(currentScreenHeight * 9 / 20).toUtf8() + "\n");
        settingFile.write(QString::number(currentScreenHeight / 5 * 4).toUtf8() + "\n");
        settingFile.write("11\n100");
        settingFile.close();
    }
    settingFile.open(QIODevice::ReadOnly);
    windowWidth = settingFile.readLine().toInt();
    windowHeight = settingFile.readLine().toInt();
    setFixedSize(windowWidth, windowHeight);
    textSize = settingFile.readLine().toInt();
    volume = settingFile.readLine().toInt();


    //构建总的分页系统以及五个页面
    stackWidget = new QStackedWidget(this);
    stackWidget->setFixedSize(width(), height());
    gameWidget = new QWidget();
    setWidget = new QWidget();
    helpWidget = new QWidget();
    settingWidget = new QWidget();
    achievementWidget = new QWidget();
    gameWidget->setFixedSize(width(), height());
    setWidget->setFixedSize(width(), height());
    helpWidget->setFixedSize(width(), height());
    settingWidget->setFixedSize(width(), height());
    achievementWidget->setFixedSize(width(), height());
    stackWidget->addWidget(gameWidget);
    stackWidget->addWidget(setWidget);
    stackWidget->addWidget(helpWidget);
    stackWidget->addWidget(settingWidget);
    stackWidget->addWidget(achievementWidget);
    stackWidget->setCurrentIndex(0);


    //构造桌面上除了游戏盘的其他部分
    scoreLabel = new QLabel(gameWidget);
    scoreLabel->setText(scorePrefix + "0");
    scoreLabel->setFixedSize(height() / 2, height() / 15);
    scoreLabel->move(width() / 2 - height() / 5, height() / 5 - height() / 90);
    scoreLabel->setFont(QFont("Arial", textSize, QFont::Bold));

    highScoreLabel = new QLabel(gameWidget);
    highScoreLabel->setText(highScoreProfix + "0");
    highScoreLabel->setFixedSize(height() / 2, height() / 15);
    highScoreLabel->move(width() / 2 - height() / 5, height() / 4 - height() / 90);
    highScoreLabel->setFont(QFont("Arial", textSize, QFont::Bold));

    backBtn = new QPushButton("撤回", gameWidget);
    backBtn->setFixedSize(height() / 20 * 3 - height() / 90, height() / 15);
    backBtn->move(width() / 2 - height() / 5 + height() / 180, height() / 19 * 6);
    backBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    backBtn->setFocusPolicy(Qt::NoFocus);
    connect(backBtn, &QPushButton::clicked,this,&MainWindow::back);

    achievementBtn = new QPushButton("成就", gameWidget);
    achievementBtn->setFixedSize(height() / 20 * 3 - height() / 90, height() / 15);
    achievementBtn->move(width() / 2 - height() / 20 + height() / 400, height() / 19 * 6);
    achievementBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    achievementBtn->setFocusPolicy(Qt::NoFocus);
    connect(achievementBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(4);});

    setBtn = new QPushButton("设置", gameWidget);
    setBtn->setFixedSize(height() / 20 * 3 - height() / 90, height() / 15);
    setBtn->move(width() / 3 * 2 + height() / 180, height() / 19 * 6);
    setBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    setBtn->setFocusPolicy(Qt::NoFocus);
    connect(setBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(1);});

    achievementCompleteLabel = new QLabel(gameWidget);
    achievementCompleteLabel->setFixedSize(width() / 5 * 4, width() / 15 * 4);
    achievementCompleteLabel->move(width() / 10, height() / 7 * 6);
    QPixmap achievementCompletePm(":/2048image/achievementComplete.png");
    achievementCompletePm = achievementCompletePm.scaled(achievementCompleteLabel->size());
    QBrush achievementCompleteBrush(achievementCompletePm);
    QPalette achievementCompletePalette(this->palette());
    achievementCompletePalette.setBrush(QPalette::Window, achievementCompleteBrush);
    achievementCompleteLabel->setAutoFillBackground(true);
    achievementCompleteLabel->setPalette(achievementCompletePalette);
    achievementCompleteLabel->setFont(QFont("Arial", textSize, QFont::Bold));
    achievementCompleteLabel->setHidden(true);


    //构造设置页面
    resumeBtn = new QPushButton("继续游戏", setWidget);
    restartBtn = new QPushButton("重新开始", setWidget);
    settingBtn = new QPushButton("游戏设置", setWidget);
    helpBtn = new QPushButton("游戏帮助", setWidget);
    quitBtn = new QPushButton("退出游戏", setWidget);
    resumeBtn->setFixedSize(width() / 2, height() / 10);
    restartBtn->setFixedSize(width() / 2, height() / 10);
    settingBtn->setFixedSize(width() / 2, height() / 10);
    helpBtn->setFixedSize(width() / 2, height() / 10);
    quitBtn->setFixedSize(width() / 2, height() / 10);
    resumeBtn->move(width() / 4, height() / 4);
    restartBtn->move(width() / 4, height() / 4 + height() / 25 * 3);
    settingBtn->move(width() / 4, height() / 4 + height() / 25 * 6);
    helpBtn->move(width() / 4, height() / 4 + height() / 25 * 9);
    quitBtn->move(width() / 4, height() / 4 + height() / 25 * 12);
    settingBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    helpBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    resumeBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    restartBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    quitBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    connect(resumeBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(0);});
    connect(restartBtn, &QPushButton::clicked, this, [=](){if(QMessageBox::question(this, "", "确认要重新开始吗？") == QMessageBox::Yes){restart();}});
    connect(settingBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(3);});
    connect(helpBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(2);});
    connect(quitBtn, &QPushButton::clicked, this, &MainWindow::quit);


    //构造帮助页面
    helpLabel = new QLabel(helpWidget);
    helpLabel->setFixedSize(width() / 5 * 4, height() / 4 * 3);
    helpLabel->move(width() / 10, height() / 10);
    helpLabel->setText("按“↑”、“↓”、“←”、“→”或\n“A”、“W”、“S”、“D”进行游戏\n\n按“H”或“？”打开帮助\n\n按“B”撤回一步操作\n\n按“R”重新开始游戏");
    helpLabel->setFont(QFont("Arial", textSize, QFont::Bold));

    helpReturnBtn = new QPushButton("返回", helpWidget);
    helpReturnBtn->setFixedSize(width() / 2, height() / 10);
    helpReturnBtn->move(width() / 4, height() / 10 * 7);
    helpReturnBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    connect(helpReturnBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(1);});


    //构造游戏设置界面
    resolutionLabel = new QLabel(settingWidget);
    resolutionLabel->setFixedSize(width() / 3, height() / 15);
    resolutionLabel->move(width() / 8, height() / 4);
    resolutionLabel->setText("分辨率");
    resolutionLabel->setFont(QFont("Arial", textSize, QFont::Bold));

    resolutionBox = new QComboBox(settingWidget);
    QStringList resolutionList;
    resolutionList << QString::number(currentScreenHeight * 9 / 20) + " * " + QString::number(currentScreenHeight * 4 / 5) << "2430 * 4320" << "1215 * 2160" << "810 * 1440" << "675 * 1200" << "608 * 1080" << "591 * 1050" << "506 * 900" << "450 * 800" << "270 * 480" ;
    resolutionBox->addItems(resolutionList);
    resolutionBox->setCurrentText(QString::number(width()) + " * " + QString::number(height()));
    resolutionBox->setFixedSize(width() / 2, height() / 15);
    resolutionBox->move(width() / 5 * 2, height() / 4);
    resolutionBox->setFont(QFont("Arial",textSize,QFont::Bold));
    resolutionBox->setFrame(true);
    resolutionBox->setFocusPolicy(Qt::NoFocus);
    connect(resolutionBox, &QComboBox::currentTextChanged, this, &MainWindow::setResolution);

    textSizeLabel = new QLabel("字体大小",settingWidget);
    textSizeLabel->setFixedSize(width() / 3, height() / 15);
    textSizeLabel->move(width() / 10, height() / 4 + height() / 8);
    textSizeLabel->setFont(QFont("Arial", textSize, QFont::Bold));
    textSizeBox = new QComboBox(settingWidget);

    QStringList textSizeList;
    textSizeList << "8" << "9" << "10" << "11" << "12" << "14" << "16" <<"18" << "20" << "22" << "24" << "26" << "28" << "36" << "48" <<"72";
    textSizeBox->addItems(textSizeList);
    textSizeBox->setCurrentText(QString::number(textSize));
    textSizeBox->setFixedSize(width() / 2, height() / 15);
    textSizeBox->move(width() / 5 * 2, height() / 4 + height() / 8);
    textSizeBox->setFont(QFont("Arial",textSize,QFont::Bold));
    textSizeBox->setFrame(true);
    textSizeBox->setFocusPolicy(Qt::NoFocus);
    connect(textSizeBox, &QComboBox::currentTextChanged, this, [=](QString size){setTextSize(size.toInt());});

    soundVolumeLabel = new QLabel(settingWidget);
    soundVolumeLabel->setFixedSize(width() / 3, height() / 15);
    soundVolumeLabel->move(width() / 8, height() / 4 + height() / 4);
    soundVolumeLabel->setText("音量");
    soundVolumeLabel->setFont(QFont("Arial",textSize,QFont::Bold));

    soundVolumeSlider = new QSlider(settingWidget);
    soundVolumeSlider->setFixedSize(width() / 3, height() / 15);
    soundVolumeSlider->move(width() / 5 * 2, height() / 4 + height() / 4);
    soundVolumeSlider->setOrientation(Qt::Horizontal);
    soundVolumeSlider->setRange(0, 100);
    soundVolumeSlider->setValue(volume);

    soundVolumeBox = new QSpinBox(settingWidget);
    soundVolumeBox->setFixedSize(width() / 6, height() / 15);
    soundVolumeBox->move(width() / 5 * 2 + width() / 3 + width() / 100, height() / 4 + height() / 4);
    soundVolumeBox->setRange(0, 100);
    soundVolumeBox->setValue(volume);
    soundVolumeBox->setFont(QFont("Arial",textSize,QFont::Bold));
    connect(soundVolumeSlider, &QSlider::valueChanged, soundVolumeBox,[=](int value){setVolume(value);});
    void (QSpinBox::*soundValueChangedSignal)(int) = &QSpinBox::valueChanged;
    connect(soundVolumeBox, soundValueChangedSignal, soundVolumeSlider, [=](int value){setVolume(value);});

    settingReturnBtn = new QPushButton("返回", settingWidget);
    settingReturnBtn->setFixedSize(width() / 2, height() / 10);
    settingReturnBtn->move(width() / 4, height() / 10 * 7);
    settingReturnBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    connect(settingReturnBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(1);});


    //构造成就页面
    achievementLabel = new QLabel("成就列表", achievementWidget);
    achievementLabel->setFixedSize(height() / 2, height() / 15);
    achievementLabel->move(width() / 2 - height() / 5, height() / 5 - height() / 90);
    achievementLabel->setFont(QFont("Arial", textSize, QFont::Bold));

    achievementList = new QListWidget(achievementWidget);
    QStringList achievementStringList;
    achievementStringList << "新的开始\n累计开始一局游戏\n"
                          << "初窥门径\n累计开始十局游戏\n"
                          << "登堂入室\n累计开始一百局游戏\n"
                          << "臻至化境\n累计开始一千局游戏\n"
                          << "玩的不错！\n合成一次512\n"
                          << "2048！\n合成一次2048\n"
                          << "万元户！\n合成一次16384\n"
                          << "我要最大的！\n合成一次131072\n"
                          << "四星连珠\n在同一行或同一列有4个\n相同的数字\n"
                          << "诶，就不合\n在桌面上同时存在3个及\n以上大于等于512的相同\n数字\n"
                          << "良好的开局\n在80步内合成128\n"
                          << "进展不错\n在400步内合成512\n"
                          << "速通\n在1600步内合成2048\n"  ;
    achievementList->addItems(achievementStringList);
    achievementList->setFixedSize(width() / 5 * 4, height() / 2);
    achievementList->move(width() / 8, height() / 4);
    int achievementNum = achievementList->count();
    for(int i = 0; i < achievementNum; i++){
        achievementList->item(i)->setBackgroundColor(QColor(Qt::gray));
        achievementList->item(i)->setFont(QFont("Arial", textSize, QFont::Bold));
    }

    achievementReturnBtn = new QPushButton("返回", achievementWidget);
    achievementReturnBtn->setFixedSize(width() / 2, height() / 10);
    achievementReturnBtn->move(width() / 4, height() / 5 * 4);
    achievementReturnBtn->setFont(QFont("Arial", textSize, QFont::Bold));
    connect(achievementReturnBtn, &QPushButton::clicked, [=](){stackWidget->setCurrentIndex(0);});


    //声音设置
    cantMoveSound = new QSoundEffect( this);
    cantMoveSound->setSource(QUrl::fromLocalFile(":/2048sound/cantMoveSound.wav"));
    moveSound = new QSoundEffect(this);
    moveSound->setSource(QUrl::fromLocalFile(":/2048sound/moveSound.wav"));
    composeSound = new QSoundEffect(this);
    composeSound->setSource(QUrl::fromLocalFile(":/2048sound/composeSound.wav"));
    gameOverSound = new QSoundEffect(this);
    gameOverSound->setSource(QUrl::fromLocalFile(":/2048sound/gameOverSound.wav"));


    //读入存档，构造游戏盘
    QString savePath = workingPath + "//save.txt";
    QFile saveFile(savePath);
    if(!saveFile.exists()) {
        createBoard();
        randomFill(2);
        recordBoard();
        if(!(achieved & 2) && isAchieved(0)){
            showAchievementComplete(0, true);
        }
    }
    else {
        createBoard();
        loadFile(savePath);
    }

}

MainWindow::~MainWindow()
{
    delete []prevBoard;
    delete [] prevScore;
    delete numBoard;
    delete ui;
}


void MainWindow::createBoard(){
    numBoard = new char[16]{};
    viewBoard = new QLabel[16]();
    for(int i = 0; i < 16; i++){
        viewBoard[i].setParent(gameWidget);
        viewBoard[i].setFixedSize(height() / 10, height() / 10);
        int leftupx = width() / 2 - height() / 5;
        int leftupy = height() / 5 * 2;
        viewBoard[i].move(leftupx + i % 4 * (height() / 10 + height() / 180), leftupy + i / 4 * (height() / 10 + height() / 180));
        viewBoard[i].setPixmap(image());
    }
    prevBoard = new char[32000]{};
    prevScore = new int[2000]{};
    startNum = 1;
    turn = 0;
    recordPos = 0;
    isBack = false;
}

QPixmap MainWindow::image(int no){
    QPixmap pm(":/2048image/" + QString::number(no) + ".png");
    pm = pm.scaled(height() / 10, height() / 10);
    return pm;
}

void MainWindow::randomFill(int num){
    for(int i = 0; i < num; i++){

        QTime t = QTime::currentTime();
        int seed = t.msec();
        std::default_random_engine e(seed);
        int pos = static_cast<int>(e() % 16);
        while(numBoard[pos]){
            pos = static_cast<int>(e() % 16);
        }
        int twoOrFour = static_cast<int>(e() % 2);
        numBoard[pos] = static_cast<char>(twoOrFour + 1);
        viewBoard[pos].setPixmap(image(twoOrFour + 1));
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event){
    achievementCompleteLabel->setHidden(true);
    std::pair<bool, int> flagAndScore;
    bool tryMoved = false;
    if(stackWidget->currentIndex() == 0){
        if(event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
            flagAndScore = moveRight();
            tryMoved = true;
        }
        if(event->key() == Qt::Key_Left || event->key() == Qt::Key_A){
            flagAndScore = moveLeft();
            tryMoved = true;
        }
        if(event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
            flagAndScore = moveUp();
            tryMoved = true;
        }
        if(event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
            flagAndScore = moveDown();
            tryMoved = true;
        }
        if(event->key() == Qt::Key_B){
            back();
        }
        if(event->key() == Qt::Key_R){
            if(QMessageBox::question(this, "", "确认要重新开始吗？") == QMessageBox::Yes){restart();}
        }
    }
    if(event->key() == Qt::Key_Escape){
        stackWidget->setCurrentIndex(1);
    }

    if(event->key() == Qt::Key_H || event->key() == Qt::Key_Question){
        qDebug() <<1;
        stackWidget->setCurrentIndex(2);
    }

    if(flagAndScore.first) {
        updateViewBoard();
        if(flagAndScore.second)composeSound->play();
        else moveSound->play();
        addScore(flagAndScore.second);
        randomFill();
        if(isEnd())end();
        recordBoard();
        for(int i = 4; i <= 12; i++){
            if(!((achieved >> (2 * i + 1)) & 1) && isAchieved(i)){
                showAchievementComplete(i, !isBack);
            }
        }
    }
    else if(tryMoved) {
        cantMoveSound->play();
    }
}

int MainWindow::move(char *board){
    int curScore = 0;
    for(int i = 0; i < 4; i++){
        int pos = 3;
        for(int j = 3; j >= 0; j--){
            if(board[i * 4 + j] != 0){
                board[i * 4 + pos] = board[i * 4 + j];
                pos--;
            }
        }
        for(int j = 0; j <= pos; j++){
            board[i * 4 + j] = 0;
        }
        for(int j = 3; j > 0; j--){
            if(board[i * 4 + j] != 0 && board[i * 4 + j] == board[i * 4 + j - 1]){
                board[i * 4 + j]++;
                curScore += pow(2, static_cast<int>(board[i * 4 + j]));
                for(int k = j - 1; k > 0; k--){
                    board[i * 4 + k] = board[i * 4 + k - 1];
                }
                board[i * 4 + 0] = 0;
            }
        }
    }
    return curScore;
}


std::pair<bool, int> MainWindow::moveRight(){
    char tempboard[16]{};
    for(int i = 0; i < 16; i++){
        tempboard[i] = numBoard[i];
    }
    int curScore = move(tempboard);
    bool flag = false;
    for(int i = 0; i < 16; i++){
        if(tempboard[i] != numBoard[i]){
            flag = true;
            numBoard[i] = tempboard[i];
        }
    }
    return std::make_pair(flag, curScore);
}

std::pair<bool, int> MainWindow::moveLeft(){
    char tempboard[16]{};
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tempboard[i * 4 + j] = numBoard[i * 4 + 3 - j];
        }
    }
    int curScore = move(tempboard);
    bool flag = false;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tempboard[i * 4 + j] != numBoard[i * 4 + 3 - j]){
                flag = true;
                numBoard[i * 4 + 3 - j] = tempboard[i * 4 + j];
            }
        }
    }
    return std::make_pair(flag, curScore);
}

std::pair<bool, int> MainWindow::moveDown(){
    char tempboard[16]{};
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tempboard[i * 4 + j] = numBoard[j * 4 + 3 - i];
        }
    }
    int curScore = move(tempboard);
    bool flag = false;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tempboard[i * 4 + j] != numBoard[j * 4 + 3 - i]){
                flag = true;
                numBoard[j * 4 + 3 - i] = tempboard[i * 4 + j];
            }
        }
    }
    return std::make_pair(flag, curScore);
}

std::pair<bool, int> MainWindow::moveUp(){
    char tempboard[16]{};
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tempboard[i * 4 + j] = numBoard[(3 - j) * 4 + i];
        }
    }
    int curScore = move(tempboard);
    bool flag = false;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tempboard[i * 4 + j] != numBoard[(3 - j) * 4 + i]){
                flag = true;
                numBoard[(3 - j) * 4 + i] = tempboard[i * 4 + j];
            }
        }
    }
    return std::make_pair(flag, curScore);
}


void MainWindow::recordBoard(){
    for(int i = recordPos * 16; i < recordPos * 16 + 16; i++){
        prevBoard[i] = numBoard[i - recordPos * 16];
    }
    prevScore[recordPos] = score;
    turn++;
    recordPos++;
    if(recordPos >= 2000 && recordPos % 1000 == 0){
        for(int i = 0; i < 1000; i++){
            for(int j = 0; j < 16; j++){
                prevBoard[i * 16 + j] = prevBoard[(i + 1000) * 16 + j];
                prevBoard[(i + 1000) * 16 + j] = 0;
            }
            prevScore[i] = prevScore[i + 1000];
            prevScore[i + 1000] = 0;
        }
        recordPos -= 1000;
    }
}

bool MainWindow::isEnd(){
    char tempboard[16]{};
    for(int i = 0; i < 16; i++) {
        tempboard[i] = numBoard[i];
    }
    if(!moveUp().first && !moveLeft().first && !moveDown().first && !moveRight().first){
        return true;
    }
    else{
        for(int i = 0; i < 16; i++) {
           numBoard[i] = tempboard[i];
        }
        return false;
    }
}

void MainWindow::end(){
    gameOverSound->play();
    if(QMessageBox::information(this, "游戏结束", "游戏结束！", QMessageBox::Retry | QMessageBox::Ok, QMessageBox::Retry) == QMessageBox::Retry){
        restart();
    }
}

void MainWindow::restart(){

    stackWidget->setCurrentIndex(0);
    memset(numBoard,0,sizeof (char) * 16);
    memset(prevBoard,0,sizeof (char) * 32000);
    startNum++;
    turn = 0;
    recordPos = 0;
    score = 0;
    isBack = false;
    scoreLabel->setText(scorePrefix + "0");
    updateViewBoard();
    randomFill(2);
    recordBoard();
    for(int i = 1; i <= 3; i++){
        if(!((achieved >> (i * 2 + 1)) & 1) && isAchieved(i)){
            showAchievementComplete(i, true);
        }
    }
}

void MainWindow::updateViewBoard(){
    for(int i = 0; i < 16; i++){
        viewBoard[i].setPixmap(image(numBoard[i]));
    }
}


void MainWindow::back(){
    isBack = true;
    if(recordPos > 1) {
        recordPos--;
        turn--;
        for(int i = 0; i < 16; i++){
            numBoard[i] = prevBoard[(recordPos - 1) * 16 + i];
        }
        updateViewBoard();
        score = prevScore[recordPos - 1];
        scoreLabel->setText(scorePrefix + QString::number(score));
    }

}


void MainWindow::addScore(int n){
    score += n;
    scoreLabel->setText(scorePrefix + QString::number(score));
    if(score > highScore){
        highScore = score;
        highScoreLabel->setText(highScoreProfix + QString::number(highScore));
    }
}


void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pm(":/2048image/gameback.png");
    painter.drawPixmap(0, 0, width(), height() ,pm);
}


void MainWindow::closeEvent(QCloseEvent *){
    quit();
}


void MainWindow::quit(){
    QString savePath = workingPath + "//save.txt";
    QFile saveFile(savePath);
    saveFile.open(QIODevice::WriteOnly | QIODevice::Text);
    saveFile.write(numberCode(QString::number(startNum).toUtf8()) + "\n");
    saveFile.write(numberCode(QString::number(turn).toUtf8()) + "\n");
    saveFile.write(numberCode(QString::number(recordPos).toUtf8()) + "\n");
    saveFile.write(numberCode(QString::number(achieved).toUtf8()) + "\n");
    saveFile.write(numberCode(QString::number(isBack).toUtf8()) + "\n");
    saveFile.write(prevBoard, recordPos * 16);
    saveFile.putChar('\n');
    for(int i = 0; i < recordPos; i++){
        saveFile.write(numberCode(QString::number(prevScore[i]).toUtf8()) + "\n");
    }
    saveFile.write(numberCode(QString::number(highScore).toUtf8()) + "\n");
    saveFile.close();
    QString settingPath = workingPath + "//settings.txt";
    QFile settingFile(settingPath);
    settingFile.open(QIODevice::WriteOnly | QIODevice::Text);
    settingFile.write(QString::number(windowWidth).toUtf8() + "\n");
    settingFile.write(QString::number(windowHeight).toUtf8() + "\n");
    settingFile.write(QString::number(textSize).toUtf8() + "\n");
    settingFile.write(QString::number(volume).toUtf8() + "\n");
    settingFile.close();
    close();
}


void MainWindow::loadFile(QString filePath){
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    startNum = numberDecode(file.readLine().chopped(2)).toInt();
    turn = numberDecode(file.readLine().chopped(2)).toInt();
    recordPos = numberDecode(file.readLine().chopped(2)).toInt();
    achieved = numberDecode(file.readLine().chopped(2)).toInt();
    isBack = numberDecode(file.readLine().chopped(2)).toInt();
    qDebug()<<1;
    for(int i = 0; i <= 12; i++){
        if((achieved >> (i * 2)) & 1){
            if((achieved >> (i * 2 + 1)) & 1)achievementList->item(i)->setBackgroundColor(Qt::yellow);
            else achievementList->item(i)->setBackgroundColor(Qt::white);
        }
    }
    QByteArray loadPrevBoard = file.readLine();
    for(int i = 0; i < recordPos * 16; i++){
        prevBoard[i] = loadPrevBoard[i];
    }
    for(int i = 0; i < recordPos; i++){
        prevScore[i] = numberDecode(file.readLine().chopped(2)).toInt();
    }
    score = prevScore[recordPos - 1];
    highScore = numberDecode(file.readLine().chopped(2)).toInt();
    file.close();
    for(int i = (recordPos - 1) * 16; i < recordPos * 16; i++){
        numBoard[i - (recordPos - 1) * 16] = prevBoard[i];
    }
    updateViewBoard();
    scoreLabel->setText(scorePrefix + QString::number(score));
    highScoreLabel->setText(highScoreProfix + QString::number(highScore));
}

void MainWindow::setResolution(QString text){
    int len = text.size();
    int pos;
    for(pos = 0; pos < len; pos++){
        if(text[pos] == '*')break;
    }
    windowWidth = text.mid(0, pos - 1).toInt();
    windowHeight = text.mid(pos + 2, len - 1).toInt();
    QMessageBox::information(settingWidget,"提示","重启以启用设置");
}


void MainWindow::setVolume(int soundVolume){
    volume = soundVolume;
    soundVolumeBox->setValue(soundVolume);
    soundVolumeSlider->setValue(soundVolume);
    moveSound->setVolume(static_cast<double>(0.01 * static_cast<double>(soundVolume)));
    composeSound->setVolume(static_cast<double>(0.01 * static_cast<double>(soundVolume)));
    cantMoveSound->setVolume(static_cast<double>(0.01 * static_cast<double>(soundVolume)));
    gameOverSound->setVolume(static_cast<double>(0.01 * static_cast<double>(soundVolume)));
}

void MainWindow::setTextSize(int size){
    textSize = size;
    QMessageBox::information(settingWidget,"提示","重启以启用设置");
}


QByteArray MainWindow::numberCode(QByteArray num){
    int len = num.size();
    for(int i = 0; i < len; i++){
        num[i] =  num[i] - '0';
    }
    return num;
}


QByteArray MainWindow::numberDecode(QByteArray num){
    int len = num.size();
    for(int i = 0; i < len; i++){
        num[i] =  num[i] + '0';
    }
    return num;
}


bool MainWindow::isAchieved(int no){
    switch (no) {
    case 0:return true;
    case 1:
        if(startNum == 10){
            return true;
        }
        else return false;
    case 2:
        if(startNum == 100){
            return true;
        }
        else return false;
    case 3:
        if(startNum == 1000){
            return true;
        }
        else return false;
    case 4:
        for(int i = 0; i < 16; i++){
            if(numBoard[i] == 9)return true;
        }
        return false;
    case 5:
        for(int i = 0; i < 16; i++){
            if(numBoard[i] == 11)return true;
        }
        return false;
    case 6:
        for(int i = 0; i < 16; i++){
            if(numBoard[i] == 14)return true;
        }
        return false;
    case 7:
        for(int i = 0; i < 16; i++){
            if(numBoard[i] == 17)return true;
        }
        return false;
    case 8:
        qDebug() << 1;
        for(int i = 0; i < 4; i++){
            if(numBoard[i * 4] && numBoard[i * 4] == numBoard[i * 4 + 1] && numBoard[i * 4] == numBoard[i * 4 + 2] && numBoard[i * 4] == numBoard[i * 4 + 3])return true;
        }
        for(int i = 0; i < 4; i++){
            if(numBoard[i] && numBoard[i] == numBoard[i + 4] && numBoard[i] == numBoard[i + 8] && numBoard[i] == numBoard[i + 12])return true;
        }
        return false;
    case 9:{
        int num[18]{};
        for(int i = 0; i < 16; i++){
            num[static_cast<int>(numBoard[i])]++;
        }
        for(int i = 9; i < 18; i++){
            if(num[i] >= 3)return true;
        }
        return false;
    }
    case 10:
        if(turn <= 80){
            for(int i = 0; i < 16; i++){
                if(numBoard[i] == 7)return true;
            }
        }
        return false;
    case 11:
        if(turn <= 400){
            for(int i = 0; i < 16; i++){
                if(numBoard[i] == 9)return true;
            }
        }
        return false;
    case 12:
        if(turn <= 1600){
            for(int i = 0; i < 16; i++){
                if(numBoard[i] == 11)return true;
            }
        }
        return false;
    default:return false;
    }
}



void MainWindow::showAchievementComplete(int no, bool isGold){
    if(((achieved >> (no * 2)) & 1) && !isGold)return;
    QListWidgetItem* item = achievementList->item(no);
    if(isGold)item->setBackgroundColor(Qt::yellow);
    else item->setBackgroundColor(Qt::white);
    QString text = item->text();
    int len = text.size();
    int enterPos = 0;
    for(enterPos = 0; enterPos < len; enterPos++){
        if(text[enterPos] == '\n')break;
    }
    text = text.mid(0, enterPos);
    if(isGold)achievementCompleteLabel->setText("成就 " + text + " 已达成(金)");
    else achievementCompleteLabel->setText("成就 " + text + " 已达成(银)");
    achievementCompleteLabel->setHidden(false);
    achieved |= (1 << (no * 2));
    if(isGold)achieved |= (1 << (no * 2 + 1));
}

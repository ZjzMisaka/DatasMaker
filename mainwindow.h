#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include "QHBoxLayout"
#include "QSpacerItem"
#include "QLabel"
#include "QStringList"
#include "QDir"
#include "canclickedqlabel.h"
#include "QProcess"
#include "iostream"
#include "outputviewer.h"
#include "QInputDialog"
#include "QComboBox"

#include "QDebug"

//QsciScintilla作为QWidget的控件，需要添加该控件的头文件
#include <Qsci/qsciscintilla.h>
//以python语法作为例子，该语法分析器的头文件
#include <Qsci/qscilexerjava.h>
//设置代码提示功能，依靠QsciAPIs类实现
#include <Qsci/qsciapis.h>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QsciScintilla *createQsciScintillaToLayout(QsciScintilla *newQsciScintilla, QLayout *layout, QString ruleFile);
    QStringList getExtraClassesList();
    void resetClassesList();
    void createFile();
    void getSettings();
    void refreshEnvironment();

    QsciScintilla *qs_editorImport;
    QsciScintilla *qs_editorDeclare;
    QsciScintilla *qs_editorFun;
    QsciScintilla *qs_editorExtraClass;
    QHBoxLayout horizontalLayout_button;
    QWidget *widget_button;
    QWidget *widget_importareaisshow;
    QWidget *widget_declareareaisshow;
    QPushButton *pb_resetExtraClass;
    QPushButton *pb_loadExtraClass;
    QPushButton *pb_addExtraClass;
    QPushButton *pb_importareaisshow;
    QPushButton *pb_declareareaisshow;
    QLabel *label_import;
    QLabel *label_declare;
    CanClickedQLabel *label;
    CanClickedQLabel *selectedLabel;
    QList<CanClickedQLabel *> labelList;
    QStringList *extraClassesList;
    QString commandCompile;
    QString commangRun;
    OutputViewer *outputViewer;
    QProcess *pro;
    QStringList *settings;

signals:
    void sendOutput(QString output);
    void sendError(QString error);

private slots:
    void onIsUseSshChanged();
    void changeSettingWidgetIsShow();
    void changeImportAreaIsShow();
    void changeDeclareAreaIsShow();
    void run();
    void onTabWidgetCurrentChanged(int index);
    void selectLabel(CanClickedQLabel *selectedLabel);
    void deleteClass();
    void readOutput();
    void readError();
    void processStop();
    void refreshSettings();
    void deleteSetting();
    void saveSetting();
    void onSettingChoosed();
    void showOutput();
};

#endif // MAINWINDOW_H

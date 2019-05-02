#include "outputviewer.h"
#include "ui_outputviewer.h"

OutputViewer::OutputViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputViewer)
{
    ui->setupUi(this);

    historyThisRun = new QString();
    allHistory = new  QStringList();

    connect(ui->pb_stop, &QPushButton::clicked, this, &OutputViewer::sendStopSignal);
    connect(ui->pb_clear, &QPushButton::clicked, this, &OutputViewer::clearOutput);
}

void OutputViewer::getOutput(QString output)
{
    QString showText;
    for(QString history : *allHistory)
    {
        showText.append(history).append("<br />=============== <br />");
    }
    *historyThisRun = *historyThisRun + output + "<br />";
    ui->te_outputviewer->setText(showText + *historyThisRun);
    QScrollBar *vScrollBar = ui->te_outputviewer->verticalScrollBar();
    if (vScrollBar != nullptr)
    {
        vScrollBar->setValue(vScrollBar->maximum());
    }
}

void OutputViewer::getError(QString output)
{
    QString showText;
    for(QString history : *allHistory)
    {
        showText.append(history + "<br />=============== <br />");
    }
    *historyThisRun = "<font style='color:#FF0000;'>" + *historyThisRun + output + "</font> <br />";
    ui->te_outputviewer->setText(showText + *historyThisRun);
    QScrollBar *vScrollBar = ui->te_outputviewer->verticalScrollBar();
    if (vScrollBar != nullptr)
    {
        vScrollBar->setValue(vScrollBar->maximum());
    }
}

void OutputViewer::onFinished()
{
    *historyThisRun = "<font style='color:#C0C0C0;'>" + *historyThisRun + "</font> <br />";
    allHistory->append(*historyThisRun);
    ui->te_outputviewer->setText("");
}

void OutputViewer::sendStopSignal()
{
    emit processStop();
}

void OutputViewer::clearOutput()
{
    historyThisRun = new QString();
    allHistory = new QStringList();
}

OutputViewer::~OutputViewer()
{
    delete ui;
}

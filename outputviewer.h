#ifndef OUTPUTVIEWER_H
#define OUTPUTVIEWER_H

#include <QWidget>
#include "QScrollBar"
#include "QCloseEvent"

namespace Ui {
class OutputViewer;
}

class OutputViewer : public QWidget
{
    Q_OBJECT

public:
    explicit OutputViewer(QWidget *parent = nullptr);
    ~OutputViewer();

private:
    Ui::OutputViewer *ui;
    QString *historyThisRun;
    QStringList *allHistory;

signals:
    void processStop();

public slots:
    void getOutput(QString output);
    void getError(QString error);
    void onFinished();
    void sendStopSignal();
    void clearOutput();
};

#endif // OUTPUTVIEWER_H

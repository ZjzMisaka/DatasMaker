#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    outputViewer = new OutputViewer();

    widget_importareaisshow = new QWidget();
    widget_importareaisshow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    widget_importareaisshow->setLayout(new QHBoxLayout());
    ui->tab_importanddeclare->layout()->addWidget(widget_importareaisshow);
    pb_importareaisshow = new  QPushButton();
    pb_importareaisshow->setMaximumSize(15, 15);
    pb_importareaisshow->setText("-");
    label_import = new QLabel();
    label_import->setText("导入");
    widget_importareaisshow->layout()->addWidget(pb_importareaisshow);
    widget_importareaisshow->layout()->addWidget(label_import);
    widget_importareaisshow->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    widget_importareaisshow->layout()->setMargin(0);

    qs_editorImport  = createQsciScintillaToLayout(qs_editorImport, ui->tab_importanddeclare->layout(), "./rules/ClassRules.txt");
    qs_editorImport->setText("import com.datasmaker.datasmaker.DatasMaker;\nimport com.datasmaker.datasmaker.DatasMaker.DBType;\nimport com.datasmaker.utils.SelectUtil;\n\nimport java.util.Random;\n");

    widget_declareareaisshow = new QWidget();
    widget_declareareaisshow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    widget_declareareaisshow->setLayout(new QHBoxLayout());
    ui->tab_importanddeclare->layout()->addWidget(widget_declareareaisshow);
    pb_declareareaisshow = new  QPushButton();
    pb_declareareaisshow->setMaximumSize(15, 15);
    pb_declareareaisshow->setText("-");
    label_declare = new QLabel();
    label_declare->setText("声明");
    widget_declareareaisshow->layout()->addWidget(pb_declareareaisshow);
    widget_declareareaisshow->layout()->addWidget(label_declare);
    widget_declareareaisshow->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    widget_declareareaisshow->layout()->setMargin(0);

    qs_editorDeclare = createQsciScintillaToLayout(qs_editorDeclare, ui->tab_importanddeclare->layout(), nullptr);

    qs_editorFun = createQsciScintillaToLayout(qs_editorFun, ui->tab_fun->layout(), "./rules/FunctionRules.txt");
    qs_editorFun->setText("if(hasFinishedTurnLastInvoke && !hasSucceedLastTurn){\n\t\n}\n");
    qs_editorExtraClass = createQsciScintillaToLayout(qs_editorFun, ui->tab_extraclass->layout(), nullptr);
    qs_editorExtraClass->setText("package com.datasmaker.extra;\n\t\n\tpublic class NewExtra {\n\t\t\n\t}\n}\n");

    widget_button = new QWidget();
    widget_button->setLayout(new QHBoxLayout());
    ui->tab_extraclass->layout()->addWidget(widget_button);

    widget_button->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    widget_button->layout()->setMargin(0);
    pb_resetExtraClass = new  QPushButton();
    pb_resetExtraClass->setText("重置");
    pb_resetExtraClass->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    widget_button->layout()->addWidget(pb_resetExtraClass);
    pb_loadExtraClass = new  QPushButton();
    pb_loadExtraClass->setText("导入类");
    pb_loadExtraClass->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    widget_button->layout()->addWidget(pb_loadExtraClass);
    pb_addExtraClass = new  QPushButton();
    pb_addExtraClass->setText("生成类");
    pb_addExtraClass->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    widget_button->layout()->addWidget(pb_addExtraClass);

    ui->vl_viewclass->setSpacing(0);
    ui->vl_viewclass->setMargin(0);
    ui->vl_viewclass->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    ui->tabWidget->setCurrentWidget(ui->tab_importanddeclare);

    connect(ui->pb_output, &QPushButton::clicked, this, &MainWindow::showOutput);
    connect(ui->cb_ssh, &QCheckBox::clicked, this, &MainWindow::onIsUseSshChanged);
    connect(ui->pb_changesettingisshow, &QPushButton::clicked, this, &MainWindow::changeSettingWidgetIsShow);
    connect(pb_importareaisshow, &QPushButton::clicked, this, &MainWindow::changeImportAreaIsShow);
    connect(pb_declareareaisshow, &QPushButton::clicked, this, &MainWindow::changeDeclareAreaIsShow);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabWidgetCurrentChanged);
    connect(ui->pb_deleteclass, &QPushButton::clicked, this, &MainWindow::deleteClass);
    connect(ui->pb_run, &QPushButton::clicked, this, &MainWindow::run);
    connect(this, &MainWindow::sendOutput, outputViewer, &OutputViewer::getOutput);
    connect(this, &MainWindow::sendError, outputViewer, &OutputViewer::getError);
    connect(outputViewer, &OutputViewer::processStop, this, &MainWindow::processStop);
    connect(ui->cb_choosesetting, &QComboBox::currentTextChanged, this, &MainWindow::onSettingChoosed);
    connect(ui->pb_savesetting, &QPushButton::clicked, this, &MainWindow::saveSetting);
    connect(ui->pb_deletesetting, &QPushButton::clicked, this, &MainWindow::deleteSetting);

    refreshSettings();
    refreshEnvironment();
}

QsciScintilla *MainWindow::createQsciScintillaToLayout(QsciScintilla *newQsciScintilla, QLayout *layout, QString ruleFile)
{
    newQsciScintilla = new QsciScintilla();

    //设置语法
    QsciLexerJava *textLexer = new QsciLexerJava;//创建一个词法分析器
    newQsciScintilla->setLexer(textLexer);//给QsciScintilla设置词法分析器

    //行号提示
    newQsciScintilla->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
    newQsciScintilla->setMarginLineNumbers(0,true);//对该页边启用行号
    newQsciScintilla->setMarginWidth(0,15);//设置页边宽度
    //代码提示
    QsciAPIs *apis = new QsciAPIs(textLexer);
    apis->add(QString("import"));
    apis->add(QString("package"));
    apis->add(QString("lastTurnDataTotalCount"));
    if(ruleFile != nullptr)
    {
        apis->load(ruleFile);
    }
    apis->prepare();

    newQsciScintilla->setAutoCompletionSource(QsciScintilla::AcsAll);   //设置源，自动补全所有地方出现的
    newQsciScintilla->setAutoCompletionCaseSensitivity(true);   //设置自动补全大小写敏感
    newQsciScintilla->setAutoCompletionThreshold(1);    //设置每输入一个字符就会出现自动补全的提示*/

    layout->addWidget(newQsciScintilla);

    return newQsciScintilla;
}

void MainWindow::onIsUseSshChanged()
{
    if(ui->cb_ssh->isChecked())
    {
        ui->le_sshport->setEnabled(true);
        ui->le_localport->setEnabled(true);
        ui->le_sshusername->setEnabled(true);
        ui->le_sshpassword->setEnabled(true);
    }
    else
    {
        ui->le_sshport->setEnabled(false);
        ui->le_localport->setEnabled(false);
        ui->le_sshusername->setEnabled(false);
        ui->le_sshpassword->setEnabled(false);
    }
}

void MainWindow::changeSettingWidgetIsShow()
{
    if(ui->pb_changesettingisshow->text() == "隐藏设置")
    {
        ui->pb_changesettingisshow->setText("显示设置");
        ui->widget_setting->hide();
        setMinimumWidth(320);
    }
    else
    {
        ui->pb_changesettingisshow->setText("隐藏设置");
        ui->widget_setting->show();
        setMinimumWidth(915);
    }
}

void MainWindow::changeImportAreaIsShow()
{
    if(pb_importareaisshow->text() == "-")
    {
        pb_importareaisshow->setText("+");
        qs_editorImport->hide();
        if(pb_declareareaisshow->text() == "+")
        {
            pb_declareareaisshow->setText("-");
            qs_editorDeclare->show();
        }
    }
    else
    {
        pb_importareaisshow->setText("-");
        qs_editorImport->show();
    }
}

void MainWindow::changeDeclareAreaIsShow()
{
    if(pb_declareareaisshow->text() == "-")
    {
        pb_declareareaisshow->setText("+");
        qs_editorDeclare->hide();
        if(pb_importareaisshow->text() == "+")
        {
            pb_importareaisshow->setText("-");
            qs_editorImport->show();
        }
    }
    else
    {
        pb_declareareaisshow->setText("-");
        qs_editorDeclare->show();
    }
}

void MainWindow::showOutput()
{
    outputViewer->show();
}

QStringList MainWindow::getExtraClassesList()
{
    QDir dir("./src/com/datasmaker/extra");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QStringList extraClassesList;

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        extraClassesList.append(fileInfo.filePath());
    }

    return extraClassesList;
}

void MainWindow::resetClassesList()
{
    for(CanClickedQLabel *label : labelList)
    {
        if(label)
        {
            labelList.removeOne(label);
            delete label;
            label=nullptr;
        }
    }
}

void MainWindow::onTabWidgetCurrentChanged(int index)
{
    if(index == 0)
    {
    }
    else if(index == 1)
    {
    }
    else if(index == 2)
    {
    }
    else if(index == 3)
    {
        resetClassesList();
        extraClassesList = new QStringList();
        *extraClassesList = getExtraClassesList();
        for(int i = 0; i < extraClassesList->count(); ++i)
        {
            label = new CanClickedQLabel(extraClassesList->at(i).mid(extraClassesList->at(i).lastIndexOf("/") + 1));
            connect(label, &CanClickedQLabel::clicked, this, &MainWindow::selectLabel);
            ui->vl_viewclass->insertWidget(0, label);
            labelList.push_back(label);
        }
        if(labelList.count() >= 1)
        {
            selectLabel(labelList.at(labelList.count() - 1));
        }
    }
}

/**
 * @brief FileSetter::selectLabel
 * @param selectedLabel
 * 选中高亮一个配置label
 */
void MainWindow::selectLabel(CanClickedQLabel *selectedLabel)
{
    labelList = ui->widget_viewclasses->findChildren<CanClickedQLabel *>();
    for (CanClickedQLabel * i : labelList)
    {
        i->setStyleSheet("background-color: none");
    }
    this->selectedLabel = selectedLabel;
    selectedLabel->setStyleSheet("background-color: #0078d7; color: #F0F0F0;");

    QString classContent;
    for(int i = 0; i < extraClassesList->count(); ++i)
    {
        if(extraClassesList->at(i).contains(selectedLabel->text()))
        {
            QFile file(extraClassesList->at(i));
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                return;
            }

            QTextStream in(&file);
            QString line = in.readLine();
            while (!line.isNull())
            {
                classContent.append(line + "\n");
                line = in.readLine();
            }
            ui->te_classcontent->clear();
            ui->te_classcontent->setPlainText(classContent);
        }
    }
}

void MainWindow::deleteClass()
{
    for(int i = 0; i < extraClassesList->count(); ++i)
    {
        if(extraClassesList->at(i).contains(selectedLabel->text()))
        {
            QFile fileTemp(extraClassesList->at(i));
            CanClickedQLabel *labelTemp = selectedLabel;
            if(fileTemp.remove())
            {
                labelList.removeOne(labelTemp);
                delete labelTemp;
                labelTemp=nullptr;

                if(labelList.count() >= 1)
                {
                    selectLabel(labelList.at(labelList.count() - 1));
                }

                return;
            }
        }
    }
}

void MainWindow::createFile()
{
    QStringList classContent;
    QFile sourceFile("./src/MakeDatas.java");
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&sourceFile);
    QString line = in.readLine();
    while (!line.isNull())
    {
        classContent.append(line);
        line = in.readLine();
    }

    for (int i = 0; i < classContent.count(); ++i)
    {
        if(classContent.at(i).contains("/*import start*/"))
        {
            classContent.insert(++i, qs_editorImport->text());
        }
        else if(classContent.at(i).contains("/*init start*/"))
        {
            classContent.insert(++i, qs_editorDeclare->text());
        }
        else if(classContent.at(i).contains("/*DatasMaker para start*/"))
        {
            QString para;
            QString dbType;
            if(ui->cb_dbtype->currentText() == "MySQL")
            {
                dbType = "DBType.MySQL";
            }
            else  if(ui->cb_dbtype->currentText() == "Oracle")
            {
                dbType = "DBType.Oracle";
            }
            QString ip = "\"" + ui->le_ip->text() + "\"";
            QString dbPort = ui->le_port->text();
            QString dbName = "\"" + ui->le_dbname->text() + "\"";
            QString dbUsername = "\"" + ui->le_dbusername->text() + "\"";
            QString dbPassword = "\"" + ui->le_dbpassword->text() + "\"";
            QString tableName = "\"" + ui->le_tablename->text() + "\"";

            QString sshPort;
            QString localPort;
            QString sshUserName;
            QString sshPassword;

            if(!ui->cb_ssh->isChecked())
            {
                sshPort = ui->le_sshport->text();
                localPort = ui->le_localport->text();
                sshUserName = "\"" + ui->le_sshusername->text() + "\"";
                sshPassword = "\"" + ui->le_sshpassword->text() + "\"";
                para.append(dbType + "," + ip + "," + dbPort + "," + dbName + "," + dbUsername + "," + dbPassword + "," + tableName);
            }
            else
            {
                para.append(dbType + "," + ip + "," + sshPort + "," + localPort + "," + dbPort + "," + sshUserName + "," + sshPassword + "," + dbName + "," + dbUsername + "," + dbPassword + "," + tableName);
            }
            classContent.insert(++i, para);
        }
        else if(classContent.at(i).contains("/*makeDatas para start*/"))
        {
            QString para;
            QString allDataTotalCount = ui->le_alldatascount->text();
            QString oneTurnDataTotalCount = ui->le_oneturndatascount->text();
            QString fields = "\"" + ui->le_fields->text() + "\"";
            QString callerClassName = "\"com.datasmaker.work.MakeDatas\"";
            QString methodName = "\"makeData\"";
            if(oneTurnDataTotalCount != "")
            {
                para.append(allDataTotalCount + "," + oneTurnDataTotalCount + "," + fields + "," + callerClassName + "," + methodName);
            }
            else
            {
                para.append(allDataTotalCount + "," + fields + "," + callerClassName + "," + methodName);
            }
            classContent.insert(++i, para);
        }
        else if(classContent.at(i).contains("/*function body start*/"))
        {
            classContent.insert(++i, qs_editorFun->text());
        }
    }

    QFile file("./src/com/datasmaker/work/MakeDatas.java");
    file.open( QIODevice::WriteOnly );
    for(QString line : classContent)
    {
        file.write(line.toLatin1() + "\n");
    }
    file.close();
}

void MainWindow::run()
{
    createFile();

    QString currentPath = QCoreApplication::applicationDirPath();
    currentPath = currentPath.replace("/debug", "");

    QStringList extraList = getExtraClassesList();
    QString extraStr;
    for(QString extra : extraList)
    {
        extraStr.append(" \"" + extra + "\"");
    }

    QString javac = "\"" + ui->le_javac->text() + "\"";
    if(javac == "\"\"")
    {
        javac = "javac";
    }
    QString commandJavac;
    commandJavac = javac + " -encoding utf-8 -cp \"" + currentPath + "/jars/jsch-0.1.55.jar;" + currentPath + "/jars/mysql-connector-java-8.0.15.jar;" + currentPath + "/jars/ojdbc6.jar\" \"" + currentPath + "/src/com/datasmaker/utils/SelectUtil.java\" \"" + currentPath + "/src/com/datasmaker/datasmaker/DatasMaker.java\" \"" + currentPath + "/src/com/datasmaker/work/MakeDatas.java\"" + extraStr;

    QString commandCd0 = currentPath.mid(0, 2);
    QString commandCd1 = "cd " + currentPath + "/src";

    QString java = "\"" + ui->le_java->text() + "\"";
    if(java == "\"\"")
    {
        java = "java";
    }
    QString commandJava;
    commandJava = java + " -cp \".;" + currentPath + "/jars/jsch-0.1.55.jar;" + currentPath + "/jars/mysql-connector-java-8.0.15.jar;" + currentPath + "/jars/ojdbc6.jar\" \"com.datasmaker.work.MakeDatas\"";

    pro = new QProcess;
    connect(pro, &QProcess::readyReadStandardOutput, this, &MainWindow::readOutput);
    connect(pro, &QProcess::readyReadStandardError, this, &MainWindow::readError);
    connect(pro, &QProcess::aboutToClose, outputViewer, &OutputViewer::onFinished);
    outputViewer->show();
    pro->start("cmd", QIODevice::ReadWrite);
    //pro->waitForStarted();
    //pro->waitForReadyRead();
    pro->write(commandJavac.toLatin1() + "\n");
    pro->write(commandCd0.toLatin1() + "\n");
    pro->write(commandCd1.toLatin1() + "\n");
    pro->write(commandJava.toLatin1() + "\n");
}

void MainWindow::readOutput()
{
    QString output = QString::fromLocal8Bit(pro->readAllStandardOutput());
    emit sendOutput(output);
}

void MainWindow::readError()
{
    QString error = QString::fromLocal8Bit(pro->readAllStandardError());
    emit sendError(error);
}

void MainWindow::processStop()
{
    pro->close();
}

void MainWindow::getSettings()
{
    QFile file("./settings.cfg");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    settings = new QStringList();

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull())
    {
        settings->append(line);
        line = in.readLine();
    }
}

void MainWindow::refreshSettings()
{
    getSettings();
    int count = ui->cb_choosesetting->count();
    for (int i = 1; i < count; --count)
    {
        ui->cb_choosesetting->removeItem(i);
    }
    QStringList sort;
    QStringList paras;
    for (QString line : *settings)
    {
        sort = line.split("[sort]");
        ui->cb_choosesetting->addItem(sort.at(0));
    }
}

void MainWindow::onSettingChoosed()
{
    if(ui->cb_choosesetting->currentIndex() == 0)
    {
        ui->cb_ssh->setChecked(false);
        ui->cb_dbtype->setCurrentIndex(0);
        ui->le_ip->setText("");
        ui->le_port->setText("");
        ui->le_dbname->setText("");
        ui->le_tablename->setText("");
        ui->le_dbusername->setText("");
        ui->le_dbpassword->setText("");
        ui->le_sshport->setText("");
        ui->le_localport->setText("");
        ui->le_sshusername->setText("");
        ui->le_sshpassword->setText("");
        ui->le_alldatascount->setText("");
        ui->le_oneturndatascount->setText("");
        ui->le_fields->setText("");
        return;
    }

    QStringList sort;
    QStringList paras;
    for (QString line : *settings)
    {
        sort = line.split("[sort]");
        if(sort.at(0) == ui->cb_choosesetting->currentText())
        {
            paras = sort.at(1).split("[para]");
            for(int i = 0; i < ui->cb_dbtype->count(); ++i)
            {
                if(ui->cb_dbtype->itemText(i) == paras.at(0))
                {
                    ui->cb_dbtype->setCurrentIndex(i);
                }
            }
            ui->le_ip->setText(paras.at(1));
            ui->le_port->setText(paras.at(2));
            ui->le_dbname->setText(paras.at(3));
            ui->le_tablename->setText(paras.at(4));
            ui->le_dbusername->setText(paras.at(5));
            ui->le_dbpassword->setText(paras.at(6));
            ui->le_sshport->setText(paras.at(7));
            ui->le_localport->setText(paras.at(8));
            ui->le_sshusername->setText(paras.at(9));
            ui->le_sshpassword->setText(paras.at(10));
            paras = sort.at(2).split("[para]");
            ui->le_alldatascount->setText(paras.at(0));
            ui->le_oneturndatascount->setText(paras.at(1));
            ui->le_fields->setText(paras.at(2));
            paras = sort.at(3).split("[para]");
            if(paras.at(0) == "true")
            {
                ui->cb_ssh->setChecked(true);
            }
            else
            {
                ui->cb_ssh->setChecked(false);
            }

            return;
        }
    }
}

void MainWindow::saveSetting()
{
    bool isOk;
    QString settingName = QInputDialog::getText(nullptr, "设置名字", "输入名字", QLineEdit::Normal, "", &isOk);
    if(isOk)
    {
        QString isSsh;
        if(ui->cb_ssh->isChecked())
        {
            isSsh = "true";
        }
        else
        {
            isSsh = "false";
        }

        QString data = settingName + "[sort]" + ui->cb_dbtype->currentText() + "[para]" + ui->le_ip->text() + "[para]" + ui->le_port->text() + "[para]" + ui->le_dbname->text() + "[para]" + ui->le_tablename->text() + "[para]" + ui->le_dbusername->text() + "[para]" + ui->le_dbpassword->text() + "[para]" + ui->le_sshport->text() + "[para]" + ui->le_localport->text() + "[para]" + ui->le_sshusername->text() + "[para]" + ui->le_sshpassword->text() + "[sort]" + ui->le_alldatascount->text() + "[para]" + ui->le_oneturndatascount->text() + "[para]" + ui->le_fields->text() + "[sort]" + isSsh;

        QFile file("./settings.cfg");

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }

        QTextStream out(&file);

        for(QString line : *settings)
        {
            if(line.split("[sort]").at(0) != settingName)
            {
                out << line << endl;
            }
        }

        file.close();

        if(!file.open(QIODevice::Append | QIODevice::Text))
        {
            return;
        }

        out << data << endl;

        file.close();
        refreshSettings();
    }
}

void MainWindow::deleteSetting()
{
    QFile file("./settings.cfg");
    QTextStream out(&file);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    for(QString line : *settings)
    {
        if(line.split("[sort]").at(0) != ui->cb_choosesetting->currentText())
        {
            out << line << endl;
        }
    }

    file.close();
    refreshSettings();
}

void MainWindow::refreshEnvironment()
{
    QFile file("./environment.cfg");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QStringList *lines = new QStringList();

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull())
    {
        lines->append(line);
        line = in.readLine();
    }

    for(QString line : *lines)
    {
        QStringList type = line.split(',');
        if(type.at(0) == "javac")
        {
            ui->le_javac->setText(type.at(1));
        }
        else if(type.at(0) == "java")
        {
                ui->le_java->setText(type.at(1));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

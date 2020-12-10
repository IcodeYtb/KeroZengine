#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NULLBYTE 0

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(400, 200);

    this->statusBar()->setSizeGripEnabled(false);
    this->createMenus();
    this->createWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    this->createActions();

    m_fileMenu = new QMenu("File", this);
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_saveFileAction);
    m_fileMenu->addSeparator();
}

void MainWindow::createActions()
{
    m_saveFileAction = new QAction(tr("&Save as..."), this);
    m_saveFileAction->setShortcuts(QKeySequence::Save);
    m_saveFileAction->setStatusTip(tr("Save Talisman"));
    connect(m_saveFileAction, &QAction::triggered, this, &MainWindow::saveToFile);
}

void MainWindow::createWidgets()
{
    m_ID = new QComboBox(this);
    m_ID->setGeometry(0, 20, 130, 20);

    std::string temp_str;

    std::fstream ID_file("Ids.txt", std::ios::in);
    if(ID_file.is_open())
    {
        while(std::getline(ID_file, temp_str))
        {
            m_ID->addItem(QString::fromStdString(temp_str));
        }
    }
    else
        m_ID->addItem("Error couldnt load talisman file");
    m_slots = new QComboBox(this);
    m_slots->setGeometry(160,20,60,20);

    m_slots->addItem("0 Slots");m_slots->addItem("1 Slots");m_slots->addItem("2 Slots");m_slots->addItem("3 Slots");

    for(int i = 0; i<2; i++)
    {
        m_talent[i] = new QComboBox(this);
        m_talent_value[i] = new QComboBox(this);
        m_talent[i]->setGeometry(50+i*150, 100, 120, 30);
        std::fstream talent_file("talents.dat", std::ios::in);
        if(talent_file.is_open())
        {
            while (std::getline(talent_file, temp_str)) {
                m_talent[i]->addItem(QString::fromStdString(temp_str));
            }
            talent_file.close();
        }
        else
            m_talent[i]->addItem("ERROR COULDNT LOAD TALENTS FILE");

        for(int j=0; j<255; j++)
            m_talent_value[i]->addItem(QString::number(j));

        m_talent_value[i]->setGeometry(50+i*150, 140, 60, 30);
    }
}

void MainWindow::saveTalismanHexValues(int& _tslots,int& _ttalent1,int& _ttalent2,int& _ttalent1_value,int& _ttalent2_value,int& _tid)
{
    _tid = m_ID->currentIndex();
    _tslots = m_slots->currentIndex();
    _ttalent1 = m_talent[0]->currentIndex();
    _ttalent1_value = m_talent_value[0]->currentIndex();
    _ttalent2 = m_talent[1]->currentIndex();
    _ttalent2_value = m_talent_value[1]->currentIndex();
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Talisman"), "",
            tr("Equipment Box (*.eqp);;All Files (*)"));

    if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            file.close();
            int tslots, ttalent1, ttalent2, ttalent1_value, ttalent2_value, tid, tal=0x06;
            saveTalismanHexValues(tslots, ttalent1, ttalent2, ttalent1_value, ttalent2_value, tid);

            std::fstream output_File(fileName.toStdString().c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
            output_File.write((char*)&tal, 1);
            output_File.write((char*)&tslots, 1);
            output_File.write((char*)&tid, 1);
            output_File.seekg(0x0C, std::_S_beg);
            output_File.write((char*)&ttalent1, 1);
            output_File.seekg(0x0E, std::_S_beg);
            output_File.write((char*)&ttalent1_value, 1);
            output_File.seekg(0x10, std::_S_beg);
            output_File.write((char*)&ttalent2, 1);
            output_File.seekg(0x12, std::_S_beg);
            output_File.write((char*)&ttalent2_value, 1);
            output_File.close();

    }
}


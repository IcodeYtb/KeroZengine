#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QComboBox>
#include <QDebug>

#include <fstream>
#include <cstdlib>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createMenus();
    void createActions();
    void createWidgets();

    void saveTalismanHexValues(int& _tslots,int& _ttalent1,int& _ttalent2,int& _ttalent1_value,int& _ttalent2_value,int& _tid);

private slots:
    void saveToFile();

private:
    Ui::MainWindow *ui;

    QComboBox* m_slots, * m_ID,
             * m_talent[2], * m_talent_value[2];

    QMenu* m_fileMenu; QAction* m_saveFileAction;
};
#endif // MAINWINDOW_H

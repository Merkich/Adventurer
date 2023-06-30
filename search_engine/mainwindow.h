#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>

#include "converterjson.h"
#include "exceptions.h"
#include "qapplication.h"
#include "searchserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_requestButton_clicked();
    void on_pathButton_clicked();

private:
    Ui::MainWindow *ui;
    ConverterJSON js;
    InvertedIndex index;


    bool makeConverterClass();

    enum ResultOfTask
    {
        waiting = 0,
        noError,
        withError,
        fatalError
    };
    ResultOfTask taskResult;
    // Отображение критических ошибок
    void shorErrors(QString);
public slots:
    // Отображение не критических ошибок
    void slotShowError(QString);

};

#endif // MAINWINDOW_H

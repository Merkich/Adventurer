#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textResult->setWordWrapMode(QTextOption::WordWrap);
    ui->textError->setWordWrapMode(QTextOption::WordWrap);
    taskResult = waiting;
    connect(&js, SIGNAL(showError(QString)), this, SLOT(slotShowError(QString)));
    ui->requestButton->setEnabled(false);
    ui->requestButton->setStyleSheet("background-color: #e1e1e1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotShowError(QString str)
{
    QString errorStr = ui->textError->toPlainText();
    ui->textError->setText(errorStr + str + "\n");
    taskResult = withError;
}

void MainWindow::shorErrors(QString str)
{
    QString errorStr = ui->textError->toPlainText();
    ui->textError->setText(errorStr + str + "\n");
    taskResult = fatalError;
}

void MainWindow::on_requestButton_clicked()
{


    makeConverterClass();
    switch (taskResult) {
    case noError:
        ui->textResult->setText("<font color=green>Запросы выполнены без ошибок</font>");
        break;
    case withError:
        ui->textResult->setText("Запросы выполнены с ошибками");
        break;
    case fatalError:
        ui->textResult->setText("<font color=red>Запросы выполнены с критическими ошибками</font>");
        break;
    default:
        break;
    }
}

bool MainWindow::makeConverterClass()
{
    bool result = true;
    ui->textError->clear();
    taskResult = noError;

    try {
        index.UpdateDocumentBase(js.GetTextDocuments());
    }
    catch (const NoConfigFile &exception)
    {
        shorErrors("Файл config.json не найден");
        result = false;
        cerr << exception.what() << endl;
    }
    catch (const ConfigFileNotOpen &exception)
    {
        shorErrors("Файл config.json не открыт");
        result = false;
        cerr << exception.what() << endl;
    }
    catch (const NoConfigField &exception)
    {
        shorErrors("В файле config.json нет поля config");
        result = false;
        cerr << exception.what() << endl;
    }
    catch (const WrongStructConfig &exception)
    {
        shorErrors("В файле config.json поля заполнены неверно");
        result = false;
        cerr << exception.what() << endl;
    }


    if(result)
    {
        SearchServer server(index);

        try {
            server.setMaxResponce(js.GetResponsesLimit());
            QVector<QVector<RelativeIndex> > answerVector = server.search(js.GetRequests());

            QVector<QVector<QPair<int, float>>> vectorForConverter;

            for(auto &vector : answerVector)
            {
                QVector<QPair<int, float>> timeVector;
                for(int i = 0; i < vector.size() && i < (js.GetResponsesLimit()); i++)
                {
                    QPair<int, float> p(make_pair(vector.at(i).doc_id, vector.at(i).rank));
                    timeVector.push_back(p);
                }
                vectorForConverter.push_back(timeVector);
            }

            js.putAnswers(vectorForConverter);
        }
        catch (const NoRequestsFile &exception)
        {
            shorErrors("Файл requests.json не найден");
            result = false;
            cerr << exception.what() << endl;
        }
        catch (const RequestsFileNotOpen &exception)
        {
            shorErrors("Файл requests.json не открыт");
            result = false;
            cerr << exception.what() << endl;
        }
        catch (const NoRequestField &exception)
        {
            shorErrors("В файле requests.json нет поля requests");
            result = false;
            cerr << exception.what() << endl;
        }
        catch (const WrongStructRequest &exception)
        {
            shorErrors("В файле requests.json поля заполнены неверно");
            result = false;
            cerr << exception.what() << endl;
        }
    }

    return result;
}

void MainWindow::on_pathButton_clicked()
{

    js.setConfigPath(QFileDialog::getOpenFileName(nullptr,
                                            "Find config file",
                                            QDir::currentPath(),
                                            "(*.json)"));
    if(!js.getConfigPath().isEmpty())
    {
        ui->requestButton->setEnabled(true);
        ui->requestButton->setStyleSheet(ui->pathButton->styleSheet());
    }
}


#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Добавляем варианты алгоритмов в QComboBox
    ui->comboBox->addItem("DFS");
    ui->comboBox->addItem("BFS");
    ui->comboBox->addItem("Dijkstra");

    // Соединяем событие нажатия кнопки с обработчиком
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::visualizeGraph);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::visualizeGraph()
{
    // Получаем текст из QTextEdit для ввода вершин и ребер
    QString graphData = ui->textEdit->toPlainText();

    // Получаем выбранный пользователем алгоритм
    QString selectedAlgorithm = ui->comboBox->currentText();

    // Здесь вы можете добавить код для обработки введенных данных
    // и вызова соответствующего алгоритма

    // Пример обработки выбранного алгоритма
    if (selectedAlgorithm == "DFS") {
        runDFS(graphData);
    } else if (selectedAlgorithm == "BFS") {
        runBFS(graphData);
    } else if (selectedAlgorithm == "Dijkstra") {
        runDijkstra(graphData);
    }
}

void Widget::runDFS(const QString &graphData)
{
    // Здесь должен быть код для выполнения алгоритма DFS
    // Например:
    QMessageBox::information(this, "DFS", "Выполняется алгоритм DFS...");
}

void Widget::runBFS(const QString &graphData)
{
    // Здесь должен быть код для выполнения алгоритма BFS
    // Например:
    QMessageBox::information(this, "BFS", "Выполняется алгоритм BFS...");
}

void Widget::runDijkstra(const QString &graphData)
{
    // Здесь должен быть код для выполнения алгоритма Дейкстры
    // Например:
    QMessageBox::information(this, "Dijkstra", "Выполняется алгоритм Дейкстры...");
}

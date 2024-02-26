#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <QStringList>
#include <QGraphicsTextItem>
#include <QRandomGenerator>
#include <QSet>
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
    // Получаем список вершин и ребер из строки данных
    QStringList lines = graphData.split("\n", Qt::SkipEmptyParts);
    QGraphicsScene *scene = new QGraphicsScene;

    // Словарь для хранения координат вершин
    QMap<QString, QPointF> verticesCoords;

    // Сет для хранения уже добавленных ребер
    QSet<QPair<QString, QString>> addedEdges;

    // Обход каждой строки данных
    foreach(const QString &line, lines) {
        QStringList vertices = line.split(" ", Qt::SkipEmptyParts);
        QString parentVertex = vertices.at(0);
        QPointF parentVertexPos = verticesCoords.value(parentVertex, QPointF());
        if (parentVertexPos.isNull()) {
            parentVertexPos = QPointF(qrand() % 500, qrand() % 500); // Генерируем случайные координаты для вершины
            verticesCoords.insert(parentVertex, parentVertexPos);
            scene->addEllipse(parentVertexPos.x(), parentVertexPos.y(), 20, 20);
            scene->addText(parentVertex)->setPos(parentVertexPos.x(), parentVertexPos.y() - 20);
        }

        // Обход смежных вершин для текущей вершины
        for (int i = 1; i < vertices.size(); ++i) {
            QString childVertex = vertices.at(i);
            QPointF childVertexPos = verticesCoords.value(childVertex, QPointF());
            if (childVertexPos.isNull()) {
                childVertexPos = QPointF(qrand() % 500, qrand() % 500); // Генерируем случайные координаты для вершины
                verticesCoords.insert(childVertex, childVertexPos);
                scene->addEllipse(childVertexPos.x(), childVertexPos.y(), 20, 20);
                scene->addText(childVertex)->setPos(childVertexPos.x(), childVertexPos.y() - 20);
            }

            // Проверяем, не добавлено ли ребро между этими вершинами ранее
            if (!addedEdges.contains(qMakePair(parentVertex, childVertex))) {
                scene->addLine(parentVertexPos.x() + 10, parentVertexPos.y() + 10,
                               childVertexPos.x() + 10, childVertexPos.y() + 10);
                // Добавляем ребро в сет уже добавленных ребер
                addedEdges.insert(qMakePair(parentVertex, childVertex));
                addedEdges.insert(qMakePair(childVertex, parentVertex)); // Добавляем обратное ребро
            }
        }
    }

    // Создаем вид сцены и отображаем его
    QGraphicsView *view = new QGraphicsView(scene);
    view->show();
}

void Widget::runBFS(const QString &graphData)
{
    // Получаем список вершин и ребер из строки данных
    QStringList lines = graphData.split("\n", Qt::SkipEmptyParts);
    QGraphicsScene *scene = new QGraphicsScene;

    int x = 0;
    int y = 0;

    // Словарь для хранения координат вершин
    QMap<QString, QPointF> verticesCoords;

    // Обход каждой строки данных
    foreach(const QString &line, lines) {
        QStringList vertices = line.split(" ", Qt::SkipEmptyParts);
        QString parentVertex = vertices.at(0);
        QPointF parentVertexPos(x, y);
        verticesCoords.insert(parentVertex, parentVertexPos);

        // Добавляем вершину на сцену
        QGraphicsEllipseItem *item = scene->addEllipse(x, y, 20, 20);
        QGraphicsTextItem *text = scene->addText(parentVertex);
        text->setPos(x, y - 20);

        // Обход смежных вершин для текущей вершины
        for (int i = 1; i < vertices.size(); ++i) {
            QString childVertex = vertices.at(i);
            QPointF childVertexPos;
            // Если координаты дочерней вершины уже были определены, используем их
            if (verticesCoords.contains(childVertex)) {
                childVertexPos = verticesCoords.value(childVertex);
            } else {
                // Иначе создаем новую координату для дочерней вершины
                x += 50;
                childVertexPos.setX(x);
                childVertexPos.setY(y);
                verticesCoords.insert(childVertex, childVertexPos);

                // Добавляем вершину на сцену
                item = scene->addEllipse(x, y, 20, 20);
                text = scene->addText(childVertex);
                text->setPos(x, y - 20);
            }

            // Добавляем ребро между вершинами
            scene->addLine(parentVertexPos.x() + 10, parentVertexPos.y() + 10,
                           childVertexPos.x() + 10, childVertexPos.y() + 10);

        }
        y += 50;
        x = 0;
    }

    // Создаем вид сцены и отображаем его
    QGraphicsView *view = new QGraphicsView(scene);
    view->show();
}

void Widget::runDijkstra(const QString &graphData)
{
    // Получаем список вершин и ребер из строки данных
    QStringList lines = graphData.split("\n", Qt::SkipEmptyParts);
    QGraphicsScene *scene = new QGraphicsScene;

    int x = 0;
    int y = 0;

    // Словарь для хранения координат вершин
    QMap<QString, QPointF> verticesCoords;

    // Обход каждой строки данных
    foreach(const QString &line, lines) {
        QStringList vertices = line.split(" ", Qt::SkipEmptyParts);
        QString parentVertex = vertices.at(0);
        QPointF parentVertexPos(x, y);
        verticesCoords.insert(parentVertex, parentVertexPos);

        // Добавляем вершину на сцену
        QGraphicsEllipseItem *item = scene->addEllipse(x, y, 20, 20);
        QGraphicsTextItem *text = scene->addText(parentVertex);
        text->setPos(x, y - 20);

        // Обход смежных вершин для текущей вершины
        for (int i = 1; i < vertices.size(); ++i) {
            QString childVertex = vertices.at(i);
            QPointF childVertexPos;
            // Если координаты дочерней вершины уже были определены, используем их
            if (verticesCoords.contains(childVertex)) {
                childVertexPos = verticesCoords.value(childVertex);
            } else {
                // Иначе создаем новую координату для дочерней вершины
                x += 50;
                childVertexPos.setX(x);
                childVertexPos.setY(y);
                verticesCoords.insert(childVertex, childVertexPos);

                // Добавляем вершину на сцену
                item = scene->addEllipse(x, y, 20, 20);
                text = scene->addText(childVertex);
                text->setPos(x, y - 20);
            }

            // Добавляем ребро между вершинами
            scene->addLine(parentVertexPos.x() + 10, parentVertexPos.y() + 10,
                           childVertexPos.x() + 10, childVertexPos.y() + 10);

        }
        y += 50;
        x = 0;
    }

    // Создаем вид сцены и отображаем его
    QGraphicsView *view = new QGraphicsView(scene);
    view->show();
}


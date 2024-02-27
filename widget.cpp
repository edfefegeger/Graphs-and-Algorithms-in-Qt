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
#include <QQueue>

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
    // Получаем начальную и конечную точки из QTextEdit
    QString startVertex = ui->textEdit_2->toPlainText().trimmed();
    QString endVertex = ui->textEdit_3->toPlainText().trimmed();

    // Реализуйте алгоритм поиска кратчайшего пути здесь

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
                // Если вершины соответствуют начальной и конечной точке, рисуем красную линию
                if ((parentVertex == startVertex && childVertex == endVertex) || (parentVertex == endVertex && childVertex == startVertex)) {
                    QPen redPen(Qt::red);
                    scene->addLine(parentVertexPos.x() + 10, parentVertexPos.y() + 10,
                                   childVertexPos.x() + 10, childVertexPos.y() + 10, redPen);
                } else {
                    // Иначе рисуем обычную линию
                    scene->addLine(parentVertexPos.x() + 10, parentVertexPos.y() + 10,
                                   childVertexPos.x() + 10, childVertexPos.y() + 10);
                }

                // Добавляем ребро в сет уже добавленных ребер
                addedEdges.insert(qMakePair(parentVertex, childVertex));
                addedEdges.insert(qMakePair(childVertex, parentVertex)); // Добавляем обратное ребро
            }
        }
    }

    // Создаем вид сцены и отображаем его
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->show();
}
void Widget::runBFS(const QString &graphData)
{
    // Получаем начальную и конечную точки из QTextEdit
    QString startVertex = ui->textEdit_2->toPlainText().trimmed();
    QString endVertex = ui->textEdit_3->toPlainText().trimmed();

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
            if (parentVertex == startVertex)
                scene->addEllipse(parentVertexPos.x(), parentVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(Qt::green)); // Зеленый круг для начальной точки
            else
                scene->addEllipse(parentVertexPos.x(), parentVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(QColor("#5b87b0"))); // Голубой круг
            scene->addText(parentVertex)->setPos(parentVertexPos.x(), parentVertexPos.y() - 20);
        }

        // Обход смежных вершин для текущей вершины
        for (int i = 1; i < vertices.size(); ++i) {
            QString childVertex = vertices.at(i);
            QPointF childVertexPos = verticesCoords.value(childVertex, QPointF());
            if (childVertexPos.isNull()) {
                childVertexPos = QPointF(qrand() % 500, qrand() % 500); // Генерируем случайные координаты для вершины
                verticesCoords.insert(childVertex, childVertexPos);
                scene->addEllipse(childVertexPos.x(), childVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(QColor("#5b87b0"))); // Голубой круг
                scene->addText(childVertex)->setPos(childVertexPos.x(), childVertexPos.y() - 20);
            }

            // Проверяем, не добавлено ли ребро между этими вершинами ранее
            if (!addedEdges.contains(qMakePair(parentVertex, childVertex))) {
                // Если вершины соответствуют начальной и конечной точке, рисуем красную линию
                if ((parentVertex == startVertex && childVertex == endVertex) || (parentVertex == endVertex && childVertex == startVertex)) {
                    QPen redPen(Qt::red);
                    redPen.setWidth(3); // Устанавливаем толщину красной линии
                    scene->addLine(parentVertexPos.x() + 12, parentVertexPos.y() + 12,
                                   childVertexPos.x() + 12, childVertexPos.y() + 12, redPen);
                } else {
                    // Иначе рисуем обычную линию
                    QPen pen(Qt::black);
                    pen.setWidth(2); // Устанавливаем толщину линии
                    scene->addLine(parentVertexPos.x() + 12, parentVertexPos.y() + 12,
                                   childVertexPos.x() + 12, childVertexPos.y() + 12, pen);
                }

                // Добавляем ребро в сет уже добавленных ребер
                addedEdges.insert(qMakePair(parentVertex, childVertex));
                addedEdges.insert(qMakePair(childVertex, parentVertex)); // Добавляем обратное ребро
            }
        }
    }

    // Используем BFS для поиска всех кратчайших путей между startVertex и endVertex
    QList<QList<QString>> allShortestPaths = findShortestPathsBFS(startVertex, endVertex, lines);

    // Визуализация всех кратчайших путей красным цветом
    foreach (const QList<QString> &path, allShortestPaths) {
        for (int i = 0; i < path.size() - 1; ++i) {
            QString currentVertex = path[i];
            QString nextVertex = path[i + 1];
            QPointF currentVertexPos = verticesCoords.value(currentVertex);
            QPointF nextVertexPos = verticesCoords.value(nextVertex);
            if (!currentVertexPos.isNull() && !nextVertexPos.isNull()) {
                QPen redPen(Qt::red);
                redPen.setWidth(3); // Устанавливаем толщину красной линии
                scene->addLine(currentVertexPos.x() + 12, currentVertexPos.y() + 12,
                               nextVertexPos.x() + 12, nextVertexPos.y() + 12, redPen);
            }
        }
    }

    // Создаем кружок для финишной точки
    QPointF endVertexPos = verticesCoords.value(endVertex);
    if (!endVertexPos.isNull())
        scene->addEllipse(endVertexPos.x(), endVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(Qt::red)); // Красный круг для финишной точки

    // Создаем вид сцены и отображаем его
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->show();
}


QList<QList<QString>> Widget::findShortestPathsBFS(const QString &startVertex, const QString &endVertex, const QStringList &lines)
{
    QList<QList<QString>> allShortestPaths;

    // Создаем QMap для хранения предков вершин в кратчайших путях
    QMap<QString, QStringList> parentMap;

    // Создаем QMap для хранения уровней вершин в графе
    QMap<QString, int> levelMap;

    // Создаем очередь для BFS
    QQueue<QString> queue;

    // Начальная вершина имеет уровень 0
    levelMap.insert(startVertex, 0);

    // Добавляем начальную вершину в очередь
    queue.enqueue(startVertex);

    // Пока очередь не пуста
    while (!queue.isEmpty()) {
        QString currentVertex = queue.dequeue();
        int currentLevel = levelMap.value(currentVertex);

        // Если мы дошли до конечной вершины, строим кратчайший путь
        if (currentVertex == endVertex) {
            QList<QString> path;
            QString vertex = endVertex;

            // Восстанавливаем кратчайший путь от конечной к начальной вершине
            while (!vertex.isEmpty()) {
                path.prepend(vertex);
                vertex = parentMap.value(vertex).isEmpty() ? "" : parentMap.value(vertex).first();
            }

            // Добавляем кратчайший путь в список всех кратчайших путей
            allShortestPaths.append(path);
        }

        // Обходим смежные вершины текущей вершины
        foreach(const QString &line, lines) {
            QStringList vertices = line.split(" ", Qt::SkipEmptyParts);
            QString parentVertex = vertices.at(0);

            // Если текущая вершина совпадает с родительской вершиной
            if (parentVertex == currentVertex) {
                // Обходим смежные вершины
                for (int i = 1; i < vertices.size(); ++i) {
                    QString childVertex = vertices.at(i);

                    // Если дочерняя вершина еще не посещена
                    if (!levelMap.contains(childVertex)) {
                        // Устанавливаем уровень дочерней вершины
                        levelMap.insert(childVertex, currentLevel + 1);

                        // Добавляем дочернюю вершину в очередь
                        queue.enqueue(childVertex);

                        // Добавляем текущую вершину в список предков дочерней вершины
                        QStringList parents = parentMap.value(childVertex);
                        parents.append(parentVertex);
                        parentMap.insert(childVertex, parents);
                    }
                }
            }
        }
    }

    return allShortestPaths;
}


// Функция для поиска всех кратчайших путей между startVertex и endVertex
QList<QList<QString>> Widget::findShortestPathsDijkstra(const QString &startVertex, const QString &endVertex, const QStringList &lines)
{
    QList<QList<QString>> allShortestPaths;

    // Создаем QMap для хранения предков вершин в кратчайших путях
    QMap<QString, QStringList> parentMap;

    // Создаем QMap для хранения уровней вершин в графе
    QMap<QString, int> levelMap;

    // Создаем очередь для BFS
    QQueue<QString> queue;

    // Начальная вершина имеет уровень 0
    levelMap.insert(startVertex, 0);

    // Добавляем начальную вершину в очередь
    queue.enqueue(startVertex);

    // Пока очередь не пуста
    while (!queue.isEmpty()) {
        QString currentVertex = queue.dequeue();
        int currentLevel = levelMap.value(currentVertex);

        // Если мы дошли до конечной вершины, строим кратчайший путь
        if (currentVertex == endVertex) {
            QList<QString> path;
            QString vertex = endVertex;

            // Восстанавливаем кратчайший путь от конечной к начальной вершине
            while (!vertex.isEmpty()) {
                path.prepend(vertex);
                vertex = parentMap.value(vertex).isEmpty() ? "" : parentMap.value(vertex).first();
            }

            // Добавляем кратчайший путь в список всех кратчайших путей
            allShortestPaths.append(path);
        }

        // Обходим смежные вершины текущей вершины
        foreach(const QString &line, lines) {
            QStringList vertices = line.split(" ", Qt::SkipEmptyParts);
            QString parentVertex = vertices.at(0);

            // Если текущая вершина совпадает с родительской вершиной
            if (parentVertex == currentVertex) {
                // Обходим смежные вершины
                for (int i = 1; i < vertices.size(); ++i) {
                    QString childVertex = vertices.at(i);

                    // Если дочерняя вершина еще не посещена
                    if (!levelMap.contains(childVertex)) {
                        // Устанавливаем уровень дочерней вершины
                        levelMap.insert(childVertex, currentLevel + 1);

                        // Добавляем дочернюю вершину в очередь
                        queue.enqueue(childVertex);

                        // Добавляем текущую вершину в список предков дочерней вершины
                        QStringList parents = parentMap.value(childVertex);
                        parents.append(parentVertex);
                        parentMap.insert(childVertex, parents);
                    }
                }
            }
        }
    }

    return allShortestPaths;
}



void Widget::runDijkstra(const QString &graphData)
{
    // Получаем начальную и конечную точки из QTextEdit
    QString startVertex = ui->textEdit_2->toPlainText().trimmed();
    QString endVertex = ui->textEdit_3->toPlainText().trimmed();

    // Получаем список вершин и ребер из строки данных
    QStringList lines = graphData.split("\n", Qt::SkipEmptyParts);
    QGraphicsScene *scene = new QGraphicsScene;

    // Словарь для хранения координат вершин
    QMap<QString, QPointF> verticesCoords;

    // Обход каждой строки данных для построения графа на сцене
    foreach(const QString &line, lines) {
        QStringList vertices = line.split(" ", Qt::SkipEmptyParts);
        QString parentVertex = vertices.at(0);
        QPointF parentVertexPos = verticesCoords.value(parentVertex, QPointF());
        if (parentVertexPos.isNull()) {
            parentVertexPos = QPointF(qrand() % 500, qrand() % 500); // Генерируем случайные координаты для вершины
            verticesCoords.insert(parentVertex, parentVertexPos);
            if (parentVertex == startVertex)
                scene->addEllipse(parentVertexPos.x(), parentVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(Qt::green)); // Зеленый круг для начальной точки
            else if (parentVertex == endVertex)
                scene->addEllipse(parentVertexPos.x(), parentVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(Qt::red)); // Красный круг для конечной точки
            else
                scene->addEllipse(parentVertexPos.x(), parentVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(QColor("#5b87b0"))); // Голубой круг
            scene->addText(parentVertex)->setPos(parentVertexPos.x(), parentVertexPos.y() - 20);
        }

        // Обход смежных вершин для текущей вершины
        for (int i = 1; i < vertices.size(); ++i) {
            QString childVertex = vertices.at(i);
            QPointF childVertexPos = verticesCoords.value(childVertex, QPointF());
            if (childVertexPos.isNull()) {
                childVertexPos = QPointF(qrand() % 500, qrand() % 500); // Генерируем случайные координаты для вершины
                verticesCoords.insert(childVertex, childVertexPos);
                scene->addEllipse(childVertexPos.x(), childVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(QColor("#5b87b0"))); // Голубой круг
                scene->addText(childVertex)->setPos(childVertexPos.x(), childVertexPos.y() - 20);
            }

            // Добавляем ребро между вершинами с более толстой кистью
            QPen pen;
            pen.setWidth(2); // Устанавливаем толщину линии
            scene->addLine(parentVertexPos.x() + 12, parentVertexPos.y() + 12,
                           childVertexPos.x() + 12, childVertexPos.y() + 12, pen);

        }
    }

    // Используем BFS для поиска всех кратчайших путей между startVertex и endVertex
    QList<QList<QString>> allShortestPaths = findShortestPathsDijkstra(startVertex, endVertex, lines);

    // Визуализация всех кратчайших путей красным цветом с более толстой линией
    foreach (const QList<QString> &path, allShortestPaths) {
        for (int i = 0; i < path.size() - 1; ++i) {
            QString currentVertex = path[i];
            QString nextVertex = path[i + 1];
            QPointF currentVertexPos = verticesCoords.value(currentVertex);
            QPointF nextVertexPos = verticesCoords.value(nextVertex);
            if (!currentVertexPos.isNull() && !nextVertexPos.isNull()) {
                QPen redPen(Qt::red);
                redPen.setWidth(3); // Устанавливаем толщину красной линии
                scene->addLine(currentVertexPos.x() + 12, currentVertexPos.y() + 12,
                               nextVertexPos.x() + 12, nextVertexPos.y() + 12, redPen);
            }
        }
    }
    // Создаем кружок для финишной точки
    QPointF endVertexPos = verticesCoords.value(endVertex);
    if (!endVertexPos.isNull())
        scene->addEllipse(endVertexPos.x(), endVertexPos.y(), 25, 25, QPen(Qt::black), QBrush(Qt::red)); // Красный круг для финишной точки

    // Создаем вид сцены и отображаем его
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->show();
}



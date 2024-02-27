#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void visualizeGraph();
    void runDFS(const QString &graphData);
    void runBFS(const QString &graphData);
    void runDijkstra(const QString &graphData);

private:
    Ui::Widget *ui;
    QList<QList<QString>> findShortestPathsDijkstra(const QString &startVertex, const QString &endVertex, const QStringList &lines);
    QList<QList<QString>> findShortestPathsBFS(const QString &startVertex, const QString &endVertex, const QStringList &lines);
    QList<QList<QString>> findAllPathsDFS(const QString &startVertex, const QString &endVertex, const QStringList &lines);
    void findAllPathsDFSRecursive(const QString &currentVertex, const QString &endVertex, const QStringList &lines, QSet<QString> &visitedVertices, QList<QString> &currentPath, QList<QList<QString>> &allPaths);
};

#endif // WIDGET_H

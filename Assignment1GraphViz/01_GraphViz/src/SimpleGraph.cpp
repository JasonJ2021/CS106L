#include <iostream>

#include <QtGui>
#include <QWidget>
#include <QApplication>
#include <algorithm>
#include <QCoreApplication>
#include <QObject>
#include <QSemaphore>
#include <fstream>
#include <sstream>
#include <cmath>

#include "SimpleGraph.h"
#undef main

using namespace std;
const double kPi = 3.14159265358979323;

MyWidget &MyWidget::getInstance()
{
    static MyWidget instance;
    return instance;
}

const int kCircleRadius = 7;
const int kCircleDiameter = 2 * kCircleRadius;
const int kWindowWidth = 600;
const int kWindowHeight = 600;
const QString kBackgroundColor = "#000000";
const QString kCircleFill = "#92FCFF";
const QString kCircleLine = "#0d0d0d";
const QString kLineColor = "#606060";

/* global semaphore to ensure only one update call at a time.
 * Prevents overload of update calls */

// TODO: This was 0 but breaks in Qt 5.10. Invesitgate
QSemaphore semaphore{1};

void InitGraphVisualizer(SimpleGraph &userGraph)
{
    MyWidget &g = MyWidget::getInstance();
    QObject::connect(&userGraph, SIGNAL(graphUpdated(SimpleGraph)),
                     &g, SLOT(repaint()));
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), QColor(kBackgroundColor));
    if (!userGraph.nodes.empty())
    {
        SimpleGraph copy;
        copy.nodes = userGraph.nodes;
        copy.edges = userGraph.edges;
        auto getX = [](const Node &a, const Node &b)
        { return a.x < b.x; };
        double maxX = std::max_element(copy.nodes.begin(), copy.nodes.end(), getX)->x;
        double minX = std::min_element(copy.nodes.begin(), copy.nodes.end(), getX)->x;

        auto getY = [](const Node &a, const Node &b)
        { return a.y < b.y; };
        double maxY = std::max_element(copy.nodes.begin(), copy.nodes.end(), getY)->y;
        double minY = std::min_element(copy.nodes.begin(), copy.nodes.end(), getY)->y;

        // To avoid division by zero in the one node case
        if (minX == maxX)
        {
            maxX += 1;
        }

        if (minY == maxY)
        {
            maxY += 1;
        }

        auto scale = [maxX, maxY, minX, minY](const Node &a) -> Node
        {
            return {(a.x - minX) * (kWindowWidth - kCircleDiameter) / (-minX + maxX) + kCircleRadius,
                    (a.y - minY) * (kWindowHeight - kCircleDiameter) / (-minY + maxY) + kCircleRadius};
        };

        std::transform(copy.nodes.begin(), copy.nodes.end(), copy.nodes.begin(), scale);

        painter.setPen(QColor(kLineColor));
        for (Edge &e : copy.edges)
        {
            Node start = copy.nodes[e.start];
            Node end = copy.nodes[e.end];
            painter.drawLine(start.x, start.y, end.x, end.y);
        }

        painter.setPen(QColor(kCircleLine));
        painter.setBrush(QColor(kCircleFill));
        for (Node &n : copy.nodes)
        {
            painter.drawEllipse(n.x - kCircleRadius, n.y - kCircleRadius, kCircleDiameter, kCircleDiameter);
        }
    }

    //last_run = QTime::currentTime();
    semaphore.release();
}

class WorkerThread : public QThread
{
    void run()
    {
        int _userMain();
        _userMain();
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MyWidget &myWidget = MyWidget::getInstance();
    //    myWidget.resize(600, 600);
    myWidget.resize(kWindowWidth, kWindowHeight);
    myWidget.show();
    qRegisterMetaType<SimpleGraph>(); //allows use of SimpleGraph in signals/slots
    WorkerThread x;
    x.start();
    return app.exec();
}

void SimpleGraph::drawGraph(SimpleGraph &graph)
{
    if (!semaphore.tryAcquire())
        return;
    MyWidget &m = MyWidget::getInstance();
    m.userGraph.nodes = graph.nodes;
    m.userGraph.edges = graph.edges;
    emit graphUpdated(graph);
}

void DrawGraph(SimpleGraph &userGraph)
{
    userGraph.drawGraph(userGraph);
}
int getInteger()
{
    while (true)
    {
        /* First we get a line of input from the user. */
        string line;
        std::getline(cin, line);
        /*
         * We'll again use an istringstream because we only want to pull data
         * out of the stream once we have put the string's contents in it.
         */
        istringstream converter(line);
        /*
         * Try getting an int from the stream. If this is not succesful
         * then user input was not a valid input.
         */
        int result;
        if (converter >> result)
        {
            /*
             * See if we can extract a char from the stream.
             * If so, the user had junk after a valid int in their input.
             */
            char remaining;
            if (converter >> remaining)
            {
                cout << "Unexpected character. Try again." << endl;
            }
            else
            {
                /*
                 * Input was succesfully converted to int with no
                 * trailing stuff at the end.
                 */
                return result;
            }
        }
        else
        {
            cout << "Not a valid integer. Try again." << endl;
        }
    }
}

int GetInteger()
{

    int result;
    result = getInteger();
    return result;
}
void readGraph(const char *filename , SimpleGraph &graph)
{
    std::ifstream graphstream(filename);
    if (!graphstream.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }
    int nodes;
    graphstream >> nodes;
    for (int i = 0; i < nodes; i++)
    {
        struct Node node;
        graph.nodes.push_back(node);
    }
    while (true)
    {
        int start, end;
        graphstream >> start;
        graphstream >> end;
        if (graphstream.fail())
            break;
        Edge edge;
        edge.start = start;
        edge.end = end;
        graph.edges.push_back(edge);
    }
}
void positionInit(SimpleGraph &graph)
{
    int n = graph.nodes.size();
    for (int k = 1; k <= n; k++)
    {
        (graph.nodes)[k - 1].x = cos(2 * kPi * k / n);
        (graph.nodes)[k - 1].y = sin(2 * kPi * k / n);
    }
}
void computeForce(SimpleGraph &graph)
{
    int n = graph.nodes.size();
    double deltax[n];
    double deltay[n];
    for(int i = 0 ; i < n ; i++){
        deltax[i] = 0 ;
    }
    for(int i = 0 ; i < n ; i++){
        deltay[i] = 0 ;
    }
    double krel = 0.001;
    double karc = 0.001;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double F = krel / sqrt(pow(graph.nodes[j].y - graph.nodes[i].y, 2) + pow(graph.nodes[j].x - graph.nodes[i].x, 2));
            double angle = atan2(graph.nodes[j].y - graph.nodes[i].y, graph.nodes[j].x - graph.nodes[i].x);
            deltax[i] -= F * cos(angle);
            deltay[i] -= F * sin(angle);
            deltax[j] += F * cos(angle);
            deltay[j] += F * sin(angle);
        }
    }
    for (int i = 0; i < graph.edges.size(); i++)
    {
        double x0 = graph.nodes[graph.edges[i].start].x;
        double y0 = graph.nodes[graph.edges[i].start].x;
        double x1 = graph.nodes[graph.edges[i].end].x;
        double y1 = graph.nodes[graph.edges[i].end].y;
        double F = karc * (pow(y1 - y0, 2) + pow(x1 - x0, 2));
        double angle = atan2(y1 - y0, x1 - x0);
        deltax[graph.edges[i].start] += F * cos(angle);
        deltay[graph.edges[i].start] += F * sin(angle);
        deltax[graph.edges[i].end] -= F * cos(angle);
        deltay[graph.edges[i].end] -= F * sin(angle);
    }
    for(int i = 0 ; i < n ; i++){
        graph.nodes[i].x +=deltax[i];
        graph.nodes[i].y +=deltay[i];
    }
}

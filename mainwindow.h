#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QImage>
#include <QTimer>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateFps();

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    Ui::MainWindow *ui;
    QList<QImage> m_RawImages;
    QTimer m_Timer;
    quint64 m_FramesDrawn;
    quint64 m_SampleCount;
    quint64 m_CurrentFps;
    quint64 m_AvgFps;
    quint32 m_DrawPos;
};

#endif // MAINWINDOW_H

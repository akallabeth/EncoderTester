#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include <QImage>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_DrawPos(0),
    m_FramesDrawn(0),
    m_SampleCount(0),
    m_CurrentFps(0),
    m_AvgFps(0)
{
    ui->setupUi(this);

    showFullScreen();

    QString path = QApplication::applicationDirPath();

    QDir dir(path, "*.png", QDir::IgnoreCase, QDir::Files);
    QFileInfoList files = dir.entryInfoList();

    foreach(QFileInfo file, files) {
        QImage img;
        if (img.load(file.absoluteFilePath())) {
            m_RawImages.append(img);
        }
    }

    qDebug() << "start";
    connect(&m_Timer, &QTimer::timeout, this, &MainWindow::updateFps);
    m_Timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFps()
{
    m_CurrentFps = m_FramesDrawn;
    m_FramesDrawn = 0;

    m_AvgFps = (m_CurrentFps + m_SampleCount * m_AvgFps) / (m_SampleCount + 1);
    m_SampleCount++;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if (m_RawImages.size()) {
        QImage img = m_RawImages.at(m_DrawPos);
        int x = 0;
        int y = 0;

        if (img.width() < width()) {
            x = (width() - img.width()) / 2;
        }
        if (img.height() < height()) {
            y = (height() - img.height()) / 2;
        }

        QString fpsOverlay = QString("FPS: %1").arg(m_CurrentFps);
        QString avgOverlay = QString("AVG: %1").arg(m_AvgFps);

        painter.drawImage(x, y, m_RawImages.at(m_DrawPos));

        painter.save();

        QFont font = painter.font();
        font.setPointSize(24);
        painter.setFont(font);
        painter.setPen(Qt::green);
        painter.setBrush(Qt::red);

        QFontMetrics fMetrics = painter.fontMetrics();
        QSize fpsMetrics = fMetrics.size( Qt::TextSingleLine, fpsOverlay );
        QRectF fpsRect( QPoint(0, 0), fpsMetrics );
        QSize avgMetrics = fMetrics.size( Qt::TextSingleLine, avgOverlay );
        QRectF avgRect( QPoint(0, fpsRect.height()), avgMetrics );
        painter.drawText(fpsRect, Qt::TextSingleLine, fpsOverlay);
        painter.drawText(avgRect, Qt::TextSingleLine, avgOverlay);
        painter.restore();

        m_DrawPos++;
        m_DrawPos %= m_RawImages.size();
        m_FramesDrawn ++;
    }

    update();
}

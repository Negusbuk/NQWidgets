#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include <nqtokenedit.h>
#include <nqcolorwheel.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void tokensChanged(const QStringList&);
    void colorChanged(const QColor&);

protected:

    NQTokenEdit* tokenedit_;
    QListWidget* tokenlist_;

    NQColorWheel* colorwheel_;
    QWidget* colordisplay_;
};

#endif // MAINWINDOW_H

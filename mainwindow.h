#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <sqlite3.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    sqlite3* DB;
private slots:
    void handleFileButton();
    void handleQuery();
    void handleFisAra();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

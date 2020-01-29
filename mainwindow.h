#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindowpresenter.h"
#include "sudokumatrix.h"

#include <QGridLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    MainWindowPresenter *presenter;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initSudokuGrid(QGridLayout *grid);
    QLineEdit* getWidgetWithContent(const QString& text);

    QLineEdit* getCellAt(int index);

    void enableSudokuCells(bool enable);

private slots:
    void onSudokuMatrixChanged(const SudokuMatrix& matrix);
    void onGridCellTextChanged(const QString& input);

    void onStartNewGameAction();
    void onAboutAction();

    void onLoadGameBtn();
    void onSaveGameBtn();

    void onErrorMsgBox(const QString& msg);

    void onGameEndedMsg();
};

#endif // MAINWINDOW_H

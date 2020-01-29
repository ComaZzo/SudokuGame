#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include "sudokumatrix.h"

#include <QObject>
#include <QFile>
#include <QTextStream>

class MainWindowModel : public QObject
{
    Q_OBJECT
private:
    SudokuMatrix *matrix;

public:
    MainWindowModel();
    ~MainWindowModel();

    void setAt(int index, const QString& text);
    int getAt(int index) const;

    void saveGame(const QString& filePath);
    void loadGame(const QString& filePath);

    void startNewGame(int filledCells);

private slots:
    void onMatrixChanged(const SudokuMatrix& matrix);
    void onCellWrongInput(int i, int j);

    void onErrorMsgBox(const QString& msg);

    void onGameEnded();

signals:
    void matrixChanged(const SudokuMatrix& matrix);
    void cellWrongInput(int i, int j);

    void errorMsgBox(const QString& msg);

    void gameEnded();
};

#endif // MAINWINDOWMODEL_H

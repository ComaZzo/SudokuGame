#ifndef MAINWINDOWPRESENTER_H
#define MAINWINDOWPRESENTER_H

#include "mainwindowmodel.h"

#include <QLineEdit>
#include <QObject>

class MainWindowPresenter : public QObject
{
    Q_OBJECT
private:
    MainWindowModel *model;
    QVector<QVector<QLineEdit*>> sudokuFields;

public:
    MainWindowPresenter();
    ~MainWindowPresenter();

    void startNewGame(int filledCells);

    void setSudokuField(int i, int j, QLineEdit* field);

    void setAt(int index, const QString& text);
    int getAt(int index) const;

    void saveGame(const QString& filePath);
    void loadGame(const QString& filePath);

private:

private slots:
    void onMatrixChanged(const SudokuMatrix& matrix);

    void onErrorMsgBox(const QString& msg);

    void onGameEnded();

signals:
    void matrixChanged(const SudokuMatrix& matrix);

    void errorMsgBox(const QString& msg);

    void gameEnded();
};

#endif // MAINWINDOWPRESENTER_H

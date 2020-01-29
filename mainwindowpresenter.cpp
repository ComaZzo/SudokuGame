#include "mainwindowpresenter.h"

MainWindowPresenter::MainWindowPresenter() :
    model(new MainWindowModel),
    sudokuFields(QVector<QVector<QLineEdit*>>(9, QVector<QLineEdit*>(9)))
{
    connect(model, &MainWindowModel::matrixChanged, this, &MainWindowPresenter::onMatrixChanged);
    connect(model, &MainWindowModel::errorMsgBox, this, &MainWindowPresenter::onErrorMsgBox);

    connect(model, &MainWindowModel::gameEnded, this, &MainWindowPresenter::onGameEnded);
}

MainWindowPresenter::~MainWindowPresenter()
{
    delete model;
}

void MainWindowPresenter::startNewGame(int filledCells)
{
    model->startNewGame(filledCells);
}

void MainWindowPresenter::setSudokuField(int i, int j, QLineEdit *field)
{
    sudokuFields[i][j] = field;
}

void MainWindowPresenter::setAt(int index, const QString &text)
{
    model->setAt(index, text);
}

int MainWindowPresenter::getAt(int index) const
{
    return model->getAt(index);
}

void MainWindowPresenter::saveGame(const QString &filePath)
{
    model->saveGame(filePath);
}

void MainWindowPresenter::loadGame(const QString &filePath)
{
    model->loadGame(filePath);
}

void MainWindowPresenter::onMatrixChanged(const SudokuMatrix &matrix)
{
    emit matrixChanged(matrix);
}

void MainWindowPresenter::onErrorMsgBox(const QString &msg)
{
    emit errorMsgBox(msg);
}

void MainWindowPresenter::onGameEnded()
{
    emit gameEnded();
}

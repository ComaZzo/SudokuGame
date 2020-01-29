#include "mainwindowmodel.h"

MainWindowModel::MainWindowModel() :
    matrix(new SudokuMatrix)
{
    connect(matrix, &SudokuMatrix::matrixChanged, this, &MainWindowModel::onMatrixChanged);
    connect(matrix, &SudokuMatrix::errorMsgBox, this, &MainWindowModel::onErrorMsgBox);

    connect(matrix, &SudokuMatrix::gameEnded, this, &MainWindowModel::onGameEnded);
}

MainWindowModel::~MainWindowModel()
{
    delete matrix;
}

void MainWindowModel::setAt(int index, const QString &text)
{
    matrix->setAt(index, text);
}

int MainWindowModel::getAt(int index) const
{
    return matrix->getAt(index);
}

void MainWindowModel::saveGame(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream ts(&file);
        ts << matrix->toQString();
    }
    file.close();
}

void MainWindowModel::loadGame(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream ts(&file);
        auto allStr = ts.readAll();
        matrix->fromQString(allStr);
    }
    file.close();
}

void MainWindowModel::startNewGame(int filledCells)
{
    matrix->generate(filledCells);
}

void MainWindowModel::onMatrixChanged(const SudokuMatrix &matrix)
{
    emit matrixChanged(matrix);
}

void MainWindowModel::onCellWrongInput(int i, int j)
{
    emit cellWrongInput(i, j);
}

void MainWindowModel::onErrorMsgBox(const QString &msg)
{
    emit errorMsgBox(msg);
}

void MainWindowModel::onGameEnded()
{
    emit gameEnded();
}

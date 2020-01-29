#include "sudokumatrix.h"
#include <QDebug>

SudokuMatrix::SudokuMatrix() :
    matrix(QVector<QVector<int>>(9, QVector<int>(9, 0))),
    isCorrectMatrix(QVector<QVector<bool>>(9, QVector<bool>(9, true))),
    regexp(QRegExp("[1-9]{0,1}"))
{

}

SudokuMatrix::~SudokuMatrix()
{

}

void SudokuMatrix::setAt(int index, const QString &text)
{
    int i = index / 9;
    int j = index - i * 9;

    int val = text.toInt();

    if(regexp.exactMatch(text))
    {
        isCorrectMatrix[i][j] = true;
    }
    else
    {
        isCorrectMatrix[i][j] = false;
        val = matrix[i][j];
    }
    matrix[i][j] = val;

    checkSudokuCorrect();

    emit matrixChanged(*this);
}

int SudokuMatrix::getAt(int index) const
{
    int i = index / 9;
    int j = index - i * 9;
    return matrix[i][j];
}

bool SudokuMatrix::isCorrectAt(int index) const
{
    int i = index / 9;
    int j = index - i * 9;
    return isCorrectMatrix[i][j];
}

void SudokuMatrix::generate(int filledCells)
{
    resetCorrectMatrix();
    resetMatrix();


    emit matrixChanged(*this);
}

QString SudokuMatrix::toQString() const
{
    QString str;

    str.append('[');
    for(int i = 0 ; i < 9; i++)
    {
        for(int j = 0 ; j < 9; j++)
        {
            str.append(QString::number(matrix[i][j]));
        }
    }
    str.append(']');

    str.append('[');
    for(int i = 0 ; i < 9; i++)
    {
        for(int j = 0 ; j < 9 ; j++)
        {
            str.append(isCorrectMatrix[i][j] ? '1' : '0');
        }
    }
    str.append(']');
    return str;
}

void SudokuMatrix::fromQString(const QString &str)
{
    auto strs = str.split(QRegExp("\\[|\\]"), QString::SplitBehavior::SkipEmptyParts);

    if(strs.count() != 2 || strs[0].count() != 81 || strs[1].count() != 81)
    {
        emit errorMsgBox("Error file for loading game");
        return;
    }

    for(int i = 0 ; i < 81; i++)
    {
        int a = i / 9;
        int b = i - a * 9;
        int val = QString(strs[0][i]).toInt();
        int valIsCorrect = strs[1][i] == '1' ? true : false;
        matrix[a][b] = val;
        isCorrectMatrix[a][b] = valIsCorrect;
    }

    emit matrixChanged(*this);
}

void SudokuMatrix::setSubSqrCorrect(int i, int j, bool isCorrect)
{
    for(int a = 0 ; a < 3 ; a++)
    {
        for(int b = 0 ; b < 3 ; b++)
        {
            int n = i * 3 + a;
            int m = j * 3 + b;
            isCorrectMatrix[n][m] = isCorrect;
        }
    }
}

void SudokuMatrix::checkSudokuCorrect()
{
    resetCorrectMatrix();
    rowsCheck();
    columnsCheck();
    subSqrCheck();

    if(isGameEnded()) emit gameEnded();
}

void SudokuMatrix::resetCorrectMatrix()
{
    for(int i = 0 ; i < 9 ; i++)
    for(int j = 0 ; j < 9 ; j++)
    {
        isCorrectMatrix[i][j] = true;
    }
}

void SudokuMatrix::resetMatrix()
{
    for(int i = 0 ; i < 9 ; i++)
    for(int j = 0 ; j < 9 ; j++)
    {
        matrix[i][j] = 0;
    }
}

void SudokuMatrix::rowsCheck()
{
    int count = 0;
    int temp = 0;
    for(int i = 0 ; i < 9; i++)
    {
        for(int j = 0 ; j < 9; j++)
        {
            count = 0;
            temp = matrix[i][j];
            if(temp != 0)
            {
                for(int k = 0 ; k < 9; k++)
                {
                    if(matrix[i][k] == temp)
                    {
                        count++;
                    }
                }
            }
            if(count > 1)
            {
                isCorrectMatrix[i][j] = false;
            }
        }
    }
}

void SudokuMatrix::columnsCheck()
{
    int count = 0;
    int temp = 0;
    for(int i = 0 ; i < 9; i++)
    {
        for(int j = 0 ; j < 9; j++)
        {
            count = 0;
            temp = matrix[i][j];
            if(temp != 0)
            {
                for(int k = 0 ; k < 9; k++)
                {
                    if(matrix[k][j] == temp)
                    {
                        count++;
                    }
                }
            }
            if(count > 1)
            {
                isCorrectMatrix[i][j] = false;
            }
        }
    }
}

void SudokuMatrix::subSqrCheck()
{
    int temp = 0;
    int count = 0;
    bool subSquareInvalid;

    for (int i = 0; i < 9; i++)
    {
        subSquareInvalid = false;
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                temp = matrix[j+(i/3)*3][k+(i%3)*3];
                count = 0;
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++)
                    {
                        int tempIn = matrix[m+(i/3)*3][n+(i%3)*3];
                        if (tempIn && tempIn == temp)
                        {
                            count++;
                        }
                        if (count > 1)
                        {
                            int a = i / 3;
                            int b = i - a * 3;
                            setSubSqrCorrect(a, b, false);
                            subSquareInvalid = true;
                            break;
                        }
                    }
                    if (subSquareInvalid)
                        break;
                }
                if (subSquareInvalid)
                    break;
            }
            if (subSquareInvalid)
                continue;
        }
    }
}

bool SudokuMatrix::isGameEnded() const
{
    for(int i = 0 ; i < 81; i++)
    {
        int a = i / 9;
        int b = i - a * 9;
        if(!isCorrectMatrix[a][b] || matrix[a][b] == 0)
            return false;
    }

    return true;
}

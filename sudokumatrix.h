#ifndef SUDOKUMATRIX_H
#define SUDOKUMATRIX_H

#include <QObject>
#include <QVector>

class SudokuMatrix : public QObject
{
    Q_OBJECT
private:
    QVector<QVector<int>> matrix;
    QVector<QVector<bool>> isCorrectMatrix;
    QRegExp regexp;

public:
    SudokuMatrix();
    ~SudokuMatrix();

    void setAt(int index, const QString& text);

    int getAt(int index) const;

    bool isCorrectAt(int index) const;

    void generate(int filledCells = 30);

    QString toQString() const;
    void fromQString(const QString& str);


private:
    void setSubSqrCorrect(int i, int j, bool isCorrectMatrix = true);

    void checkSudokuCorrect();

    void resetCorrectMatrix();
    void resetMatrix();

    void rowsCheck();
    void columnsCheck();
    void subSqrCheck();

    bool isGameEnded() const;

signals:
    void matrixChanged(const SudokuMatrix& matrix);
    void cellWrongInput(int i, int j);

    void errorMsgBox(const QString& msg);

    void gameEnded();
};

#endif // SUDOKUMATRIX_H

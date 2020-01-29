#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QLayoutItem>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    presenter(new MainWindowPresenter)
{
    ui->setupUi(this);
    initSudokuGrid(ui->sudokuGrid);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutAction);
    connect(ui->actionExit, &QAction::triggered, QApplication::instance(), &QApplication::exit);

    connect(presenter, &MainWindowPresenter::matrixChanged, this, &MainWindow::onSudokuMatrixChanged);

    connect(presenter, &MainWindowPresenter::errorMsgBox, this, &MainWindow::onErrorMsgBox);

    connect(ui->saveGameBtn, &QPushButton::clicked, this, &MainWindow::onSaveGameBtn);
    connect(ui->loadGameBtn, &QPushButton::clicked, this, &MainWindow::onLoadGameBtn);
    connect(ui->startNewBtn, &QPushButton::clicked, this, &MainWindow::onStartNewGameAction);

    connect(presenter, &MainWindowPresenter::gameEnded, this, &MainWindow::onGameEndedMsg);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete presenter;
}

void MainWindow::initSudokuGrid(QGridLayout *grid)
{
    QString defName("gridCell");
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            auto tempLineEdit = new QLineEdit(this);
            tempLineEdit->setObjectName((defName+"%1%2").arg(QString::number(i), QString::number(j)));
            tempLineEdit->setFixedSize(QSize(18,18));
            connect(tempLineEdit, &QLineEdit::textChanged, this, &MainWindow::onGridCellTextChanged);
            grid->addWidget(tempLineEdit, i, j);
            presenter->setSudokuField(i,j,tempLineEdit);
        }
    }
}

QLineEdit *MainWindow::getCellAt(int index)
{
    return static_cast<QLineEdit*>(ui->sudokuGrid->itemAt(index)->widget());
}

void MainWindow::enableSudokuCells(bool enable)
{
    for(int i = 0 ; i < ui->sudokuGrid->count(); i++)
    {
        ui->sudokuGrid->itemAt(i)->widget()->setEnabled(enable);
    }
}

void MainWindow::onSudokuMatrixChanged(const SudokuMatrix &matrix)
{
    for(int i = 0; i < 9 * 9; i++)
    {
        auto cell = getCellAt(i);
        int matrVal = matrix.getAt(i);
        if(matrVal)
            cell->setText(QString::number(matrix.getAt(i)));
        else
            cell->setText("");
        QPalette plt;

        if(matrix.isCorrectAt(i))
        {
            plt.setColor(QPalette::Base, Qt::white);
        }
        else
        {
            plt.setColor(QPalette::Base, Qt::red);
        }
        cell->setPalette(plt);
    }
}

void MainWindow::onGridCellTextChanged(const QString &input)
{
    auto widget = static_cast<QLineEdit*>(sender());
    if(widget)
    {
        auto index = ui->sudokuGrid->indexOf(widget);
        presenter->setAt(index, input);
    }
}

void MainWindow::onStartNewGameAction()
{
    enableSudokuCells(true);
    bool resDlg = false;
    auto answerDlg = QInputDialog::getInt(this, "Start game dialog", "Enter count of filled cells", 10, 0, 81, 1, &resDlg);

    if(resDlg)
    {
        presenter->startNewGame(answerDlg);
    }
}

void MainWindow::onAboutAction()
{
    QMessageBox::about(this, "About", "[Form, C++] (40 баллов) Судоку. Реализовать оконное приложение для игры в"
                       "\nсудоку. В основном игровом окне должно находиться меню с пунктом для"
                       "\nзапуска новой игры (исходная позиция должна вычитываться из файла формата"
                       "\n*.sud, выбор имени файла происходит через компонент QFileDialog). Формат"
                       "\nфайла – на Ваше усмотрение, должна быть реализована валидация файла."
                       "\nПосле загрузки из файла исходной позиции на игровое поле, пользователь"
                       "\nможет заполнять игровое поле цифрами от 1 до 9. После завершения"
                       "\nзаполнения, при правильном решении головоломки, должно появляться"
                       "\nсообщение о выигрыше."
                       "\n[Bonus] (10 баллов) Реализовать подсветку ошибочно выставленных цифр"
                       "\n(цифры не могут повторяться в пределах одной строки, столбца, квадрата"
                       "\n3x3 – более подробно см. правила игры в судоку)."
                       "\n[Bonus] (20 баллов) Реализовать основную задачу в архитектурном"
                       "\nпаттерне (MVC, MVP, MVVM)."
                       "\n[Bonus] (30 баллов) Реализовать генератор исходных позиций для игры в"
                       "\nсудоку, с возможностью создания исходной позиции через меню игры.");
}

void MainWindow::onLoadGameBtn()
{
    auto filePath = QFileDialog::getOpenFileName(this, "Choose file for loading game", "", "*.sud");
    presenter->loadGame(filePath);
}

void MainWindow::onSaveGameBtn()
{
    auto filePath = QFileDialog::getSaveFileName(this, "Choose or create file for saving game", "", "*.sud");
    if(!filePath.endsWith(".sud"))
        filePath += ".sud";
    presenter->saveGame(filePath);
}

void MainWindow::onErrorMsgBox(const QString &msg)
{
    QMessageBox::critical(this, "Error", msg);
}

void MainWindow::onGameEndedMsg()
{
    enableSudokuCells(false);
    QMessageBox::information(this, "YOU WON", "Congratulations! You can start new game and enjoy!");
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtextcodec.h>
#include <QStringList>
#include "ui_mainWindow.h"
#include "AsphericSurface.h"
#include "Converter.h"
#include "help.h"


class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QMainWindow *parent = 0);
	~mainWindow();
private slots:
	// добавить ячейку для нового коэффициента
	void addColumn();
	// удалить ячейку с коэффициента
	void deleteColumn();
	// очистить ячейки с коэффициентами
	void clearColumn();
	// Конвертировать
	void convert();
	// скрыть/раскрыть настройки для МНК
	void parameterMNK();

	void changeHeadersTable();

    void changeConvertEquation(int i);

    void changeSourceEquation(int i);
    // Открыть справку
    //void openHelpWindow();
private:
	Ui::MainWindowClass ui;
	Help* myHelpWindow;
};

#endif // MAINWINDOW_H

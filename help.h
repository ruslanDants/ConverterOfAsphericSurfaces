#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include "ui_help.h"

class Help : public QWidget
{
	Q_OBJECT

public:
	Help(QWidget *parent = 0);
	~Help();

private:
	Ui::Help ui;
};

#endif // HELP_H

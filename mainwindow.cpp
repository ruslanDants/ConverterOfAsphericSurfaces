#include "mainWindow.h"

mainWindow::mainWindow(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_AddCoefficient, SIGNAL(clicked()), this, SLOT(addColumn()));
	connect(ui.pushButton_DeleteCoefficient, SIGNAL(clicked()), this, SLOT(deleteColumn()));
	connect(ui.pushButton_Clear, SIGNAL(clicked()), this, SLOT(clearColumn()));
	connect(ui.pushButton_Calc, SIGNAL(clicked()), this, SLOT(convert()));
	connect(ui.radioButton_OLS, SIGNAL(toggled(bool)), this, SLOT(parameterMNK()));
	connect(ui.comboBox_SourceEquation, SIGNAL(currentIndexChanged(int)), this, SLOT(changeConvertEquation(int)));
	connect(ui.comboBox_ConvertEquation, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSourceEquation(int)));
	connect(ui.menu, SIGNAL(aboutToShow()), this, SLOT(openHelpWindow()));
//	QTextCodec* codec = QTextCodec::codecForName("cp1251");
//	QTextCodec::setCodecForTr(codec);
//	QTextCodec::setCodecForCStrings(codec);
//	QTextCodec::setCodecForLocale(codec);
}
void mainWindow::parameterMNK()
{
	ui.label_Approximation->setEnabled(ui.radioButton_OLS->isChecked());
	ui.spinBox_PointsApproximation->setEnabled(ui.radioButton_OLS->isChecked());
	
}
void mainWindow::changeConvertEquation(int i)
{
	ui.comboBox_ConvertEquation->setCurrentIndex(i^1);
	changeHeadersTable();
}
void mainWindow::changeSourceEquation(int i)
{
	ui.comboBox_SourceEquation->setCurrentIndex(i^1);
	changeHeadersTable();
}
void mainWindow::changeHeadersTable()
{
	QString headers;
	for (int i = 0; i < ui.tableWidget_SourceCoefficients->columnCount(); i++)
	{
		if (ui.comboBox_SourceEquation->currentIndex() == 0)
		{
			headers.append("z^");
			headers.append(QString::number(i+3));
		}
		else
		{
			headers.append("r^");
			headers.append(QString::number(6 + i*2));
		}
		headers.append(";");
	}
	ui.tableWidget_SourceCoefficients->setHorizontalHeaderLabels(headers.split(";"));
}
void mainWindow::addColumn()
{
	int k = ui.tableWidget_SourceCoefficients->columnCount();
	ui.tableWidget_SourceCoefficients->insertColumn(k);
	ui.tableWidget_SourceCoefficients->setItem(0, k, new QTableWidgetItem("0.00"));
	changeHeadersTable();
}
void mainWindow::deleteColumn()
{
	int x = ui.tableWidget_SourceCoefficients->currentIndex().column();
	ui.tableWidget_SourceCoefficients->removeColumn(x);
	changeHeadersTable();
}
void mainWindow::clearColumn()
{
	for (int i = 0; i < ui.tableWidget_SourceCoefficients->columnCount(); i++)
	{
		ui.tableWidget_SourceCoefficients->item(0, i)->setText("0.00");
	}
}
void mainWindow::openHelpWindow()
{
	myHelpWindow = new Help();
	myHelpWindow->show();
}
void mainWindow::convert()
{
	// Получение радиуса из текстового поля
	double radius = ui.lineEdit_Radius->text().toDouble();
	// Получение эксцентриситета из текстового поля
	double eccentricity = ui.lineEdit_Eccentricity->text().toDouble();
	// Получение световой высоты из текстового поля
	double height = ui.lineEdit_LightHeight->text().toDouble();
	// Получение номера начальной формулы
	int id_f = ui.comboBox_SourceEquation->currentIndex();
	AsphericSurface *Surf1 = SetFormula(id_f);
	// Получение номера конечной формулы
	id_f = ui.comboBox_ConvertEquation->currentIndex();
	AsphericSurface *Surf2 = SetFormula(id_f);
	// Задаем общие параметры асферики
	Surf1->SetParametres(radius, eccentricity, height);
	Surf2->SetParametres(radius, eccentricity, height);
	// Получение коэффициентов исходной формулы
	Surf1->Coefficients.resize(ui.tableWidget_SourceCoefficients->columnCount());
	for (int i = 0; i < ui.tableWidget_SourceCoefficients->columnCount(); i++)
	{
		Surf1->Coefficients[i] = ui.tableWidget_SourceCoefficients->item(0, i)->text().toDouble();
	}
	Converter Plan;
	if (ui.radioButton_OLS->isChecked())
		Plan.SetMethod(Converter::MHK);
	else
		Plan.SetMethod(Converter::Analytical);
	// количество получаемых коэффициентов
	int kCoeffs = ui.spinBox_CountOutCoefficients->text().toInt();
	// количество точек для апроксимации
	int kPoints = ui.spinBox_PointsApproximation->text().toInt();

	Plan.Settings(kCoeffs, kPoints);
	// Выполнить пересчет
	Plan.Run(Surf1, Surf2);
	
	QString textResult = "Полученные коэффициенты:\n";
	
	for (int i = 0; i < Surf2->Coefficients.size(); i++)
	{
		if (id_f == 1)
		{
			textResult.append("r^");
			textResult.append(QString::number(6+i*2));
		}
		else
		{
			textResult.append("z^");
			textResult.append(QString::number(3+i));
			
		}
		textResult.append("= ");
		textResult.append(QString::number(Surf2->Coefficients[i], 'E', 25));
		textResult.append("\n");
	}
	int PointsStdev = ui.spinBox_PointsStdev->text().toInt();
	double stdev = Plan.CalcStdev(Surf1, Surf2, PointsStdev);
	textResult.append("Среднее квадратичное отклонение = ");
	textResult.append(QString::number(stdev, 'E', 25));
	textResult.append("\n");
	ui.textEdit_Result->append(textResult);

}
mainWindow::~mainWindow()
{
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QPushButton>
#include <QDebug>
#include <algorithm>
#include <random>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QtXml>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QTimer *timer;
	size_t counter {0};
	QString weather;
	QString city;
	int temperature;
	static constexpr size_t size {10};
	QVector<QPushButton*> buttons_vector;
	QString color_free;
	QString color_current;
	QVector<QString> colors = {"white",
								"black",
								"brown",
								"darkBlue",
								"blue",
								"red",
								"yellow",
								"green",
								"gray",
								"pink"};
	QVector<QString> buttons_colors_vector;
	std::vector<int> array_index {0,1,2,3,4,5,6,7,8,9};

	void parsingWeather(const QString& _current_weather);

private slots:
	void slotTimerAlarm();
	void slotButtonClicked();

};
#endif // MAINWINDOW_H

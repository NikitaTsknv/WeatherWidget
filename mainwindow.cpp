#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->label_time->setText(QString("Current time - %1").arg(QTime::currentTime().toString("hh:mm:ss")));
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
	timer->start(1000);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("https://api.openweathermap.org/data/2.5/weather?"
															 "id=524894&units=metric&lang=eng&mode=xml&appid=ea9ce9d1ff5f3497b6730a7286a7cf4b")));
	QEventLoop loop;
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();
	QByteArray data = reply->readAll();
	weather = QString::fromUtf8(data);
	reply->deleteLater();
	manager->deleteLater();
	this->parsingWeather(weather);

	ui->label_weather->setText(QString("Temperature in %1 - %2 °C").arg(city, QString::number(temperature)));

	buttons_vector.push_back(ui->pushButton_0);
	buttons_vector.push_back(ui->pushButton_1);
	buttons_vector.push_back(ui->pushButton_2);
	buttons_vector.push_back(ui->pushButton_3);
	buttons_vector.push_back(ui->pushButton_4);
	buttons_vector.push_back(ui->pushButton_5);
	buttons_vector.push_back(ui->pushButton_6);
	buttons_vector.push_back(ui->pushButton_7);
	buttons_vector.push_back(ui->pushButton_8);

	auto rd = std::random_device {};
	auto rng = std::default_random_engine { rd() };
	std::shuffle(array_index.begin(), array_index.end(), rng);

	for (int i = 0; i < size; i++)
	{
		if (i == size - 1)
		{
			color_free = colors[array_index[i]];
		}
		else
		{
			buttons_vector[i]->setStyleSheet(QString("background-color: %1; color: #00FF00;").arg(colors[array_index[i]]));
			buttons_vector[i]->setText(colors[array_index[i]]);
			buttons_colors_vector.push_back(colors[array_index[i]]);
		}
	}

	connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
	connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));

}

void MainWindow::parsingWeather(const QString& _current_weather)
{
	QXmlStreamReader reader(_current_weather);

	while(!reader.atEnd())
	{
		if (reader.isStartElement())
		{
			QString name = reader.name().toString();

			if (name == "city")
			{
				QXmlStreamAttributes attributes = reader.attributes();
				if (attributes.hasAttribute("name"))
				{
					city = attributes.value("name").toString();
				}
			}

			if (name == "temperature")
			{
				QXmlStreamAttributes attributes = reader.attributes();
				double max = 0.0, min = 0.0;
				if (attributes.hasAttribute("max"))
				{
					max = attributes.value("max").toDouble();
				}
				if (attributes.hasAttribute("min"))
				{
					min = attributes.value("min").toDouble();
				}
				temperature = (max + min) / 2;
				qDebug() << "temperature" << temperature;
			}
		}

		reader.readNext();
	}
}

void MainWindow::slotTimerAlarm()
{
	ui->label_time->setText(QString("Current time - %1").arg(QTime::currentTime().toString("hh:mm:ss")));
}

void MainWindow::slotButtonClicked()
{
	counter++;
	ui->label_counter->setText(QString::number(counter));

	QObject* obj = sender();

	for (int i = 0; i < size - 1; i++)
	{
		if (obj == buttons_vector[i])
		{
			color_current = buttons_colors_vector[i];
			buttons_vector[i]->setStyleSheet(QString("background-color: %1; color: #00FF00;").arg(color_free));
			buttons_vector[i]->setText(color_free);
			buttons_colors_vector[i] = color_free;
			color_free = color_current;
			break;
		}
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}


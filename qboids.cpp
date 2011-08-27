#include <QApplication>
#include <QSettings>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	QApplication *app = new QApplication(argc, argv);

	MainWindow *mainWindow = new MainWindow();
	mainWindow->showMaximized();

	QSettings settings( "NoodleSoft", "qboids" );

	return app->exec();
}

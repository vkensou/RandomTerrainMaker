#include "makerdialog.h"
#include <QApplication>
#include "random.h"
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>

int main(int argc, char *argv[])
{
    log4cpp::PatternLayout *playout = new log4cpp::PatternLayout();
    playout->setConversionPattern("%d: %p %c %x: %m%n");
    log4cpp::Appender* fileAppender = new log4cpp::FileAppender("fileAppender", "terrainmaker.log", false);
    fileAppender->setLayout(playout);
    log4cpp::Category& root =log4cpp::Category::getRoot().getInstance("TerrainMaker2D");
    root.addAppender(fileAppender);
    root.setPriority(log4cpp::Priority::DEBUG);

    root.info("TerrainMaker start");

	srand(time(0));
    QApplication a(argc, argv);
    MakerDialog w;
    w.show();
    a.exec();

    root.info("TerrainMaker end");
    log4cpp::Category::shutdown();

    return 0;
}

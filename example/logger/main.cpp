/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void SyncLog()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << CURRENT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		auto logger = std::make_shared<tinyToolkit::logger::SyncLogger>();

		auto fileSink = std::make_shared<tinyToolkit::logger::FileSink>("fileSink", "syncFileSink.log", true);
		auto syslogSink = std::make_shared<tinyToolkit::logger::SyslogSink>("syslogSink");
		auto consoleSink = std::make_shared<tinyToolkit::logger::ConsoleSink>("consoleFile");
		auto dailyFileSink = std::make_shared<tinyToolkit::logger::DailyFileSink>("dailyFileSink", "syncDailyFileSink.log");
		auto rotatingFileLogSink = std::make_shared<tinyToolkit::logger::RotatingFileSink>("rotatingFileSink", "syncRotatingFileSink.log", 1000, 10);

		auto simpleLayout = std::make_shared<tinyToolkit::logger::SimpleLayout>();
		auto patternLayout = std::make_shared<tinyToolkit::logger::PatternLayout>("[%d.%A][%N][%P][%t] %V%n");

		auto regexFilter = std::make_shared<tinyToolkit::logger::RegexFilter>(".*Critical.*");
		auto priorityFilter = std::make_shared<tinyToolkit::logger::PriorityFilter>("DEBUG");
		auto priorityBelowFilter = std::make_shared<tinyToolkit::logger::PriorityBelowFilter>("INFO");
		auto priorityAboveFilter = std::make_shared<tinyToolkit::logger::PriorityAboveFilter>("NOTICE");

		fileSink->SetLayout(simpleLayout)->AddFilter(priorityFilter);
		syslogSink->SetLayout(simpleLayout)->AddFilter(priorityAboveFilter);
		consoleSink->SetLayout(patternLayout)->AddFilter(regexFilter);
		dailyFileSink->SetLayout(patternLayout)->AddFilter(regexFilter);
		rotatingFileLogSink->SetLayout(simpleLayout)->AddFilter(priorityBelowFilter);

		logger->AddSink(fileSink);
		logger->AddSink(syslogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			logger->Debug("Sync Message {}", "Debug");
			logger->Info("Sync Message {}", "Info");
			logger->Notice("Sync Message {}", "Notice");
			logger->Warning("Sync Message {}", "Warning");
			logger->Error("Sync Message {}", "Error");
			logger->Critical("Sync Message {}", "Critical");
			logger->Alert("Sync Message {}", "Alert");
			logger->Fatal("Sync Message {}", "Fatal");
		}

		logger->Wait();

		logger->Close();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void AsyncLog()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << CURRENT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		auto logger = std::make_shared<tinyToolkit::logger::AsyncLogger>();

		auto fileSink = std::make_shared<tinyToolkit::logger::FileSink>("fileSink", "asyncFileSink.log", true);
		auto syslogSink = std::make_shared<tinyToolkit::logger::SyslogSink>("syslogSink");
		auto consoleSink = std::make_shared<tinyToolkit::logger::ConsoleSink>("consoleFile");
		auto dailyFileSink = std::make_shared<tinyToolkit::logger::DailyFileSink>("dailyFileSink", "asyncDailyFileSink.log");
		auto rotatingFileLogSink = std::make_shared<tinyToolkit::logger::RotatingFileSink>("rotatingFileSink", "asyncRotatingFileSink.log", 1000, 10);

		auto simpleLayout = std::make_shared<tinyToolkit::logger::SimpleLayout>();
		auto patternLayout = std::make_shared<tinyToolkit::logger::PatternLayout>("[%d.%A][%N][%P][%t] %V%n");

		auto regexFilter = std::make_shared<tinyToolkit::logger::RegexFilter>(".*Critical.*");
		auto priorityFilter = std::make_shared<tinyToolkit::logger::PriorityFilter>("DEBUG");
		auto priorityBelowFilter = std::make_shared<tinyToolkit::logger::PriorityBelowFilter>("INFO");
		auto priorityAboveFilter = std::make_shared<tinyToolkit::logger::PriorityAboveFilter>("NOTICE");

		fileSink->SetLayout(simpleLayout)->AddFilter(priorityFilter);
		syslogSink->SetLayout(simpleLayout)->AddFilter(priorityAboveFilter);
		consoleSink->SetLayout(patternLayout)->AddFilter(regexFilter);
		dailyFileSink->SetLayout(patternLayout)->AddFilter(regexFilter);
		rotatingFileLogSink->SetLayout(simpleLayout)->AddFilter(priorityBelowFilter);

		logger->AddSink(fileSink);
		logger->AddSink(syslogSink);
		logger->AddSink(consoleSink);
		logger->AddSink(dailyFileSink);
		logger->AddSink(rotatingFileLogSink);

		for (int i = 0; i < 10; ++i)
		{
			logger->Debug("Async Message {}", "Debug");
			logger->Info("Async Message {}", "Info");
			logger->Notice("Async Message {}", "Notice");
			logger->Warning("Async Message {}", "Warning");
			logger->Error("Async Message {}", "Error");
			logger->Critical("Async Message {}", "Critical");
			logger->Alert("Async Message {}", "Alert");
			logger->Fatal("Async Message {}", "Fatal");
		}

		logger->Wait();

		logger->Close();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	SyncLog();
	AsyncLog();

	return 0;
}

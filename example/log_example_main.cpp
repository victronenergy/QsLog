// Copyright (c) 2013, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include "QsLog.h"
#include "QsLogDest.h"
#include "log_example_shared.h"
#include <QLibrary>
#include <QCoreApplication>
#include <QDir>
#include <iostream>

void logFunction(const QString &message, QsLogging::Level level)
{
    std::cout << "From log function: " << qPrintable(message) << " " << static_cast<int>(level)
              << std::endl;
}

// This small example shows how QsLog can be used inside a project.
int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   using namespace QsLogging;

   // 1. init the logging mechanism
   Logger& logger = Logger::instance();
   logger.setLoggingLevel(QsLogging::TraceLevel);
   const QString sLogPath(QDir(a.applicationDirPath()).filePath("log.txt"));

   // 2. add two destinations
   DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
     sLogPath, EnableLogRotation, MaxSizeBytes(512), MaxOldLogCount(2)));
   DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
   DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));
   logger.addDestination(debugDestination);
   logger.addDestination(fileDestination);
   logger.addDestination(functorDestination);

   // 3. start logging
   QLOG_INFO() << "Program started";
   QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();

   QLOG_TRACE() << "Here's a" << QString::fromUtf8("trace") << "message";
   QLOG_DEBUG() << "Here's a" << static_cast<int>(QsLogging::DebugLevel) << "message";
   QLOG_WARN()  << "Uh-oh!";
   qDebug() << "This message won't be picked up by the logger";
   QLOG_ERROR() << "An error has occurred";
   qWarning() << "Neither will this one";
   QLOG_FATAL() << "Fatal error!";

   logger.setLoggingLevel(QsLogging::OffLevel);
   for (int i = 0;i < 10000000;++i) {
       QLOG_ERROR() << QString::fromUtf8("this message should not be visible");
   }
   logger.setLoggingLevel(QsLogging::TraceLevel);

   // 4. log from a shared library - should automatically share the same log instance as above
   QLibrary myLib("log_example_shared");
   typedef LogExampleShared* (*LogExampleGetter)();
   typedef void(*LogExampleDeleter)(LogExampleShared*);
   LogExampleGetter fLogCreator = (LogExampleGetter) myLib.resolve("createExample");
   LogExampleDeleter fLogDeleter = (LogExampleDeleter)myLib.resolve("destroyExample");
   LogExampleShared *logFromShared = 0;
   if (fLogCreator && fLogDeleter) {
       logFromShared = fLogCreator();
       logFromShared->logSomething();
       fLogDeleter(logFromShared);
   } else if (!fLogCreator || !fLogDeleter) {
       QLOG_ERROR() << "could not resolve shared library function(s)";
   }

   QLOG_DEBUG() << "Program ending";

   QsLogging::Logger::destroyInstance();
   return 0;
}

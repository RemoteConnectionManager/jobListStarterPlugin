#ifndef pqJobListStarter_h
#define pqJobListStarter_h

// Qt
#include <QObject>

//Paraview
#include "pqLiveInsituManager.h"


class pqJobListStarter : public QObject
{
  Q_OBJECT
  typedef QObject Superclass;

public:
  pqJobListStarter(QObject* p = 0);
  ~pqJobListStarter();

  // Callback for shutdown.
  void onShutdown();

  // Callback for startup.
  void onStartup();

public slots:
  void onConnect(pqServer* displaySession);

private:
  Q_DISABLE_COPY(pqJobListStarter)
  QString m_filename;
  int     m_job_id;
};

#endif

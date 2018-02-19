// Qt
#include <QProcess>
#include <QPointer>
#include <QtDebug>
#include <QProgressDialog>
#include <QApplication>
#include <QDesktopWidget>

// Server Manager Includes.
#include "pqLiveInsituVisualizationManager.h"
#include "pqLiveInsituVisualizationManager.h"
#include "pqApplicationCore.h"
#include "pqCoreUtilities.h"
#include "pqObjectBuilder.h"
#include "pqOutputPort.h"
#include "pqPipelineSource.h"
#include "pqServer.h"
#include "pqServerManagerModel.h"

// vtk
#include "vtkCommand.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkSMLiveInsituLinkProxy.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMSessionProxyManager.h"
#include "vtkWeakPointer.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"

#include "jobListDialog.h"
#include "pqJobListStarter.h"


//-----------------------------------------------------------------------------
void centerWidget(QWidget *widget) {
    QRect screenGeometry = QApplication::desktop()->screenGeometry(0);
    int x = (screenGeometry.width() - widget->width()) / 2;
    int y = (screenGeometry.height() - widget->height()) / 2;
    widget->move(x, y);
}

//-----------------------------------------------------------------------------
pqJobListStarter::pqJobListStarter(QObject* p /*=0*/)
  : QObject(p)
{
    pqLiveInsituManager* insitu_manager = pqLiveInsituManager::instance();
    connect(insitu_manager, SIGNAL(connectionInitiated(pqServer*)), SLOT(onConnect(pqServer*)));
    m_filename = "";
    m_job_id = -1;
}

//-----------------------------------------------------------------------------
pqJobListStarter::~pqJobListStarter()
{
}

//-----------------------------------------------------------------------------
void pqJobListStarter::onConnect(pqServer* displaySession)
{

    QProgressDialog *prog_dlg = new QProgressDialog("Querying the scheduler...", "Cancel", 0, 0);
    prog_dlg->setModal(Qt::WindowModal);
    prog_dlg->setWindowFlags(Qt::WindowStaysOnTopHint);
    centerWidget(prog_dlg);

    QProcess* process = new QProcess();
    QString cmd = QString("squeue --user=%1").arg(getenv("USER"));

    connect(process, SIGNAL(finished(int)), prog_dlg, SLOT(close()));

    process->start(cmd);
    prog_dlg->exec();

    QByteArray out = process->readAllStandardOutput();
    QString out_str(out);
    QStringList out_str_list = out_str.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    jobListDialog *dialog = new jobListDialog();
    dialog->setItems(&out_str_list);
    dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    centerWidget(dialog);
    dialog->exec();

    m_job_id = dialog->getJobSelectedId();

    QString host = getenv("HOSTNAME");

    pqLiveInsituManager* insitu_manager = pqLiveInsituManager::instance();
    pqLiveInsituVisualizationManager* insituviz_manager = insitu_manager->managerFromDisplay(displaySession);
    vtkSMLiveInsituLinkProxy* adaptor = insituviz_manager->getProxy();
    int port = vtkSMPropertyHelper(static_cast<vtkSMProxy*>(adaptor), "InsituPort").GetAsInt();

    QString path = getenv("HOME");
    path += "/.insitu/";
    m_filename = path;
    m_filename += "run_";
    m_filename += QString::number(m_job_id);
    m_filename += ".txt";

    QFile inputFile(m_filename);
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }
    if (inputFile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&inputFile);
        out << host << ":" << port;
    }

    delete prog_dlg;
    delete process;
    delete dialog;
}

//-----------------------------------------------------------------------------
void pqJobListStarter::onStartup()
{
    qWarning() << "pqJobListStarter Started";
}

//-----------------------------------------------------------------------------
void pqJobListStarter::onShutdown()
{
  qWarning() << "pqJobListStarter Shutting down";

  QFile inputFile(m_filename);
  if(inputFile.exists()) {
      inputFile.remove();
  }
}

/* Copyright 2013-2021 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "InjectAuthlib.h"
#include <launch/LaunchTask.h>
#include <minecraft/MinecraftInstance.h>
#include <FileSystem.h>
#include <Env.h>
InjectAuthlib::InjectAuthlib(LaunchTask *parent, AuthlibInjectorPtr* injector) : LaunchStep(parent)
{
  m_injector = injector;
}

void InjectAuthlib::executeTask()
{

  if (m_aborted)
  {
    emitFailed(tr("Task aborted."));
    return;
  }

  auto latestVersionInfo = QString("https://authlib-injector.yushi.moe/artifact/latest.json");
  auto netJob = new NetJob("Injector versions info download");
  MetaEntryPtr entry = ENV.metacache()->resolveEntry("injectors", "version.json");
  entry->setStale(true);
  auto task = Net::Download::makeCached(QUrl(latestVersionInfo), entry);
  netJob->addNetAction(task);

  jobPtr.reset(netJob);
  QObject::connect(netJob, &NetJob::succeeded, this, &InjectAuthlib::onVersionDownloadSucceeded);
  QObject::connect(netJob, &NetJob::failed, this, &InjectAuthlib::onDownloadFailed);
  jobPtr->start();
}

void InjectAuthlib::onVersionDownloadSucceeded()
{

  QByteArray data;
  try
  {
    data = FS::read(QDir("injectors").absoluteFilePath("version.json"));
  }
  catch (const Exception &e)
  {
    qCritical() << "Translations Download Failed: index file not readable";
    jobPtr.reset();
    emitFailed("Error while parsing JSON response from InjectorEndpoint");
    return;
  }

  qDebug() << "Version got\n";
  QJsonParseError parse_error;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_error);
  if (parse_error.error != QJsonParseError::NoError)
  {
    qCritical() << "Error while parsing JSON response from InjectorEndpoint at " << parse_error.offset << " reason: " << parse_error.errorString();
    qCritical() << data;
    jobPtr.reset();
    emitFailed("Error while parsing JSON response from InjectorEndpoint");
    return;
  }

  if (!doc.isObject())
  {
    qCritical() << "Error while parsing JSON response from InjectorEndpoint root is not object";
    qCritical() << data;
    jobPtr.reset();
    emitFailed("Error while parsing JSON response from InjectorEndpoint");
    return;
  }

  auto downloadUrlValue = doc["download_url"];
  if (!downloadUrlValue.isString())
  {
    qCritical() << "Error while parsing JSON response from InjectorEndpoint download url is not string";
    qCritical() << data;
    jobPtr.reset();
    emitFailed("Error while parsing JSON response from InjectorEndpoint");
    return;
  }

  QString downloadUrl = downloadUrlValue.toString();

  QFileInfo fi(downloadUrl);
  m_versionName = fi.fileName();

  qDebug() << "Authlib injector version:" << m_versionName;

  auto netJob = new NetJob("Injector download");
  MetaEntryPtr entry = ENV.metacache()->resolveEntry("injectors", m_versionName);
  entry->setStale(true);
  auto task = Net::Download::makeCached(QUrl(downloadUrl), entry);
  netJob->addNetAction(task);

  jobPtr.reset(netJob);
  QObject::connect(netJob, &NetJob::succeeded, this, &InjectAuthlib::onDownloadSucceeded);
  QObject::connect(netJob, &NetJob::failed, this, &InjectAuthlib::onDownloadFailed);
  jobPtr->start();
}

void InjectAuthlib::onDownloadSucceeded()
{
  QString injector = QString("-javaagent:%1=%2").arg(QDir("injectors").absoluteFilePath(m_versionName)).arg("http://localhost:6000");

  qDebug()
      << "Injecting " << injector;
  auto inj = new AuthlibInjector(injector);
  qDebug() << "aAA" << inj;
  m_injector->reset(inj);

  jobPtr.reset();
  emitSucceeded();
}

void InjectAuthlib::onDownloadFailed(QString reason)
{
  jobPtr.reset();
  emitFailed(reason);
}

void InjectAuthlib::proceed()
{
}

// void InjectAuthlib::updateFinished()
// {
//   // if (m_updateTask->wasSuccessful())
//   // {
//   //   m_updateTask.reset();
//   //   emitSucceeded();
//   // }
//   // else
//   // {
//   //   QString reason = tr("Instance update failed because: %1\n\n").arg(m_updateTask->failReason());
//   //   m_updateTask.reset();
//   //   emit logLine(reason, MessageLevel::Fatal);
//   //   emitFailed(reason);
//   // }
// }

bool InjectAuthlib::canAbort() const
{
  if (jobPtr)
  {
    return jobPtr->canAbort();
  }
  return true;
}

bool InjectAuthlib::abort()
{
  m_aborted = true;
  if (jobPtr)
  {
    if (jobPtr->canAbort())
    {
      return jobPtr->abort();
    }
  }
  return true;
}

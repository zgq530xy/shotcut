/*
 * Copyright (c) 2012-2014 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include "jobs/abstractjob.h"
#include <QStandardItemModel>
#include <QMutex>

class JobQueue : public QStandardItemModel
{
    Q_OBJECT
protected:
    enum ColumnRole {
        COLUMN_OUTPUT,
        COLUMN_STATUS,
        COLUMN_COUNT
    };
    JobQueue(QObject *parent);
    void startNextJob();

public:
    static JobQueue& singleton(QObject* parent = 0);
    void cleanup();
    AbstractJob* add(AbstractJob *job);
    AbstractJob* jobFromIndex(const QModelIndex& index) const;
    void pause();
    void resume();
    bool isPaused() const;
    bool hasIncomplete() const;

signals:
    void jobAdded();

public slots:
    void onMessageAvailable(AbstractJob* job);
    void onFinished(AbstractJob* job, bool isSuccess);

private:
    QList<AbstractJob*> m_jobs;
    QMutex m_mutex; // protects m_jobs
    bool m_paused;
};

#define JOBS JobQueue::singleton()

#endif // JOBQUEUE_H

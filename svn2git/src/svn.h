/*
 *  Copyright (C) 2007  Thiago Macieira <thiago@kde.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SVN_H
#define SVN_H

#include <QDebug>
#include <QHash>
#include <QList>
#include <QString>
#include "ruleparser.h"

class Repository;

struct mergeinfo {
    QString from;
    int rev = -1;
    QString to;
};

inline QDebug operator<<(QDebug debug, const mergeinfo &mi)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << mi.from << "@" << mi.rev << " -> " << mi.to;

    return debug;
}

inline QTextStream& operator<<(QTextStream& out, const mergeinfo &mi)
{
    out << ".from = \"" << mi.from << "\", .rev = " + QString::number(mi.rev) + ", .to = \"" << mi.to << "\"";

    return out;
}

inline bool operator==(const mergeinfo &m1, const mergeinfo &m2)
{
    return m1.from == m2.from
        && m1.rev == m2.rev
        && m1.to == m2.to;
}

inline bool operator<(const mergeinfo &m1, const mergeinfo &m2)
{
    return m1.from < m2.from ||
        (m1.from == m2.from && m1.rev < m2.rev) ||
        (m1.rev == m2.rev && m1.to < m2.to);
}

inline uint qHash(const mergeinfo &key, uint seed)
{
    return qHash(key.from, seed) ^ key.rev ^ qHash(key.to, seed);
}

class SvnPrivate;
class Svn
{
public:
    static void initialize();

    Svn(const QString &pathToRepository);
    ~Svn();

    void setMatchRules(const QList<QList<Rules::Match> > &matchRules);
    void setRepositories(const QHash<QString, Repository *> &repositories);
    void setIdentityMap(const QHash<QByteArray, QByteArray> &identityMap);
    void setIdentityDomain(const QString &identityDomain);

    int youngestRevision();
    bool exportRevision(int revnum);

private:
    SvnPrivate * const d;
};

#endif

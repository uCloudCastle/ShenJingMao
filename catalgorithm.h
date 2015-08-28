#ifndef CATALGORITHM_H
#define CATALGORITHM_H

#include <QObject>
#include "helper.h"

enum HardLevel{
    Eazy = 1,
    Normal,
    Hard
};

enum NormalDirMode
{
    DirLeftFirst,
    DirRandom
};

class CatAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit CatAlgorithm(QObject *parent = 0);
    Q_PROPERTY(int hardLv READ hardLv WRITE setHardLv NOTIFY hardLvChanged)
    Q_INVOKABLE int getNextDirection(int catPos, QList<int> map);

    int hardLv() const { return m_hardLv; }
    void setHardLv(int putin) {
        m_hardLv = putin;
        if ( m_hardLv != Eazy && m_hardLv != Normal && m_hardLv != Hard ) {
            m_hardLv = Eazy;
        }
        emit hardLvChanged();
    }

signals:
    void hardLvChanged();

private:
    int EazyMode(int catPos, QList<int>& list);
    int NormalMode(int catPos, QList<int>& list, NormalDirMode dm);
    int HardMode(int catPos, QList<int>& list);

    int m_hardLv;
    QScopedPointer<Helper> m_helper;
};

#endif // CATALGORITHM_H

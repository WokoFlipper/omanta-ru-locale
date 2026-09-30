#include "LocaleManager.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QSettings>
#include <QLocale>

LocaleManager::LocaleManager(QQmlEngine *engine, QObject *parent)
    : QObject(parent), m_engine(engine)
{
    m_current = QSettings().value(QStringLiteral("ui/locale"), QString()).toString();
    apply(m_current);
}

QStringList LocaleManager::availableLocales() const
{
    return { QString(), QStringLiteral("en"), QStringLiteral("ru") };
}

void LocaleManager::setLocale(const QString &code)
{
    if (code == m_current)
        return;
    m_current = code;
    QSettings().setValue(QStringLiteral("ui/locale"), code);
    apply(code);
    Q_EMIT localeChanged();
}

void LocaleManager::apply(const QString &code)
{
    QGuiApplication::removeTranslator(&m_translator);
    QString target = code;
    if (target.isEmpty())
        target = QLocale::system().name(); // e.g. "ru_RU"
    const QStringList candidates = { target, target.section(u'_', 0, 0) };
    const QStringList searchPaths = {
        QStringLiteral("/usr/share/omanta/translations"),
        QStringLiteral("/usr/local/share/omanta/translations"),
        QStringLiteral(":/i18n"),
    };
    for (const QString &loc : candidates) {
        if (loc.startsWith(QStringLiteral("en")))
            break; // built-in English, no file needed
        for (const QString &path : searchPaths) {
            const QString file = QStringLiteral("omanta_") + loc;
            if (m_translator.load(file, path)) {
                QGuiApplication::installTranslator(&m_translator);
                qDebug() << "omanta-i18n: loaded" << (path + QLatin1Char('/') + file + QStringLiteral(".qm"))
                         << "for" << target;
                if (m_engine)
                    m_engine->retranslate();
                return;
            }
            qDebug() << "omanta-i18n: miss" << (path + QLatin1Char('/') + file + QStringLiteral(".qm"));
        }
        if (loc.startsWith(QStringLiteral("en")))
            break;
    }
    qDebug() << "omanta-i18n: no translation file, built-in English; system"
             << QLocale::system().name() << "saved" << m_current;
    if (m_engine)
        m_engine->retranslate();
}

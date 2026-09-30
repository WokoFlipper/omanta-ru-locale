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
    return { QString(), QStringLiteral("en"), QStringLiteral("ru"),
             QStringLiteral("fr"), QStringLiteral("de"),
             QStringLiteral("es") };
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
    // The shipped files use full names (omanta_ru_RU.qm), while Preferences
    // stores short codes ("ru"). Try both directions so either form resolves:
    // "ru_RU" -> {"ru_RU", "ru"}, "ru" -> {"ru", "ru_RU"}.
    QStringList candidates = { target };
    const QString shortName = target.section(u'_', 0, 0);
    if (shortName != target) {
        candidates << shortName;
    } else {
        const QString fullName = shortName + u'_' + shortName.toUpper();
        if (fullName != target)
            candidates << fullName;
    }
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

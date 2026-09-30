#pragma once
// LocaleManager: runtime UI language switch without restart.
// "" = follow system locale (default, current behaviour), "en" = built-in
// English, "ru" = Russian .qm. Persisted via QSettings "ui/locale".

#include <QObject>
#include <QTranslator>

class QQmlEngine;

class LocaleManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentLocale READ currentLocale NOTIFY localeChanged)
public:
    explicit LocaleManager(QQmlEngine *engine, QObject *parent = nullptr);

    QString currentLocale() const { return m_current; }
    Q_INVOKABLE void setLocale(const QString &code);
    Q_INVOKABLE QStringList availableLocales() const;

Q_SIGNALS:
    void localeChanged();

private:
    void apply(const QString &code);

    QQmlEngine *m_engine = nullptr;
    QTranslator m_translator;
    QString m_current;
};

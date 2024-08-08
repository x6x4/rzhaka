#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>

#include "runtime/runtime.hpp"

class Listener : public QObject {
    Q_OBJECT
public:
    explicit Listener(QObject *parent = nullptr);

    void openServer(const QString& name);

signals:
    void newStateRecieved(Orient dir, Cell cur);

private slots:
    void aquireState();
    void newConnection();
private:
    QLocalServer* m_server;
};

#endif // LISTENER_HPP

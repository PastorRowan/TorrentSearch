
#pragma once

#include "Aria2c/Aria2cRpc.h"
#include "Aria2c/Types.h"

#include <QObject>
#include <QString>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QProcess>
#include <functional>
#include <variant>

class Aria2c : public QObject {

    Q_OBJECT

    private:

        // empty = no token auth (aria2c --rpc-secret not set)
        QString secret = "my_secret";

        QProcess* aria2cProcess;

        QString aria2cBinFileLocation = QCoreApplication::applicationDirPath() + "/bin/windows/aria2c.exe";

        QStringList aria2Arguments = {
            "--enable-rpc=true",
            "--enable-dht=true",
            "--enable-peer-exchange=true",
            "--bt-enable-lpd=true",
            "--rpc-listen-all=false",
            "--rpc-listen-port=6800",
            QString("--rpc-secret=%1").arg(secret)
        };

        QNetworkAccessManager* networkAccessManager;

        QString rpcUrl = "http://127.0.0.1:6800/jsonrpc";

    public:

        explicit Aria2c(
            QObject* parent = nullptr
        );

        void setSecret(const QString& s) {
            secret = s;
        };

        void setRpcUrl(const QString& u) {
            rpcUrl = u;
        };

        bool isNotRunning() const;

        bool isStarting() const;

        bool isRunning() const;

        void start();

        void stop();

        void request(
            const RpcMethod method,
            const QJsonArray& params,
            QObject* context,
            std::function<void(std::variant<QJsonValue, Aria2Error>)> callback
        );

        #define DECLARE_PARAMS_ARG_Y(name) const name##Params& params,
        #define DECLARE_PARAMS_ARG_N(name)

        #define DECLARE_PARAMS_ARG(name, hasParams) \
            DECLARE_PARAMS_ARG_##hasParams(name)

        #define DECLARE_CALLBACK_ARG(name) \
            std::function<void(std::variant<name##Response, Aria2Error>)> cb

        // Named public API — generated from the table.
        #define X(function, method, name, hasParams) \
            void function( \
                DECLARE_PARAMS_ARG(name, hasParams) \
                QObject* context, \
                DECLARE_CALLBACK_ARG(name) \
            );

        RPC_METHODS

        #undef X
        #undef DECLARE_PARAMS_ARG_Y
        #undef DECLARE_PARAMS_ARG_N
        #undef DECLARE_PARAMS_ARG
        #undef DECLARE_CALLBACK_ARG

    // Leave public: here after macro definition beceause
    // It stops compiling and IDE bugs for some reason

    public:

    signals:

        void started();

        void stopped();

        void processError();

};

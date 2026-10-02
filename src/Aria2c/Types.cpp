
#include "Aria2c/Types.h"

// ================================================================
// Shared helper structs
// ================================================================

Aria2Uri Aria2Uri::fromQJsonObject(const QJsonObject& obj) {
    Aria2Uri result;
    result.uri    = obj.value(QStringLiteral("uri")).toString();
    result.status = obj.value(QStringLiteral("status")).toString();
    return result;
};

Aria2File Aria2File::fromQJsonObject(const QJsonObject& obj) {
    Aria2File result;
    result.index           = obj.value(QStringLiteral("index")).toString();
    result.path            = obj.value(QStringLiteral("path")).toString();
    result.length          = obj.value(QStringLiteral("length")).toString();
    result.completedLength = obj.value(QStringLiteral("completedLength")).toString();
    result.selected        = obj.value(QStringLiteral("selected")).toString();

    const QJsonArray urisArray = obj.value(QStringLiteral("uris")).toArray();
    for (const QJsonValue& uriValue : urisArray) {
        result.uris.append(Aria2Uri::fromQJsonObject(uriValue.toObject()));
    }

    return result;
};

Aria2Peer Aria2Peer::fromQJsonObject(const QJsonObject& obj) {
    Aria2Peer result;
    result.peerId        = obj.value(QStringLiteral("peerId")).toString();
    result.ip            = obj.value(QStringLiteral("ip")).toString();
    result.port          = obj.value(QStringLiteral("port")).toString();
    result.bitfield      = obj.value(QStringLiteral("bitfield")).toString();
    result.amChoking     = obj.value(QStringLiteral("amChoking")).toString();
    result.peerChoking   = obj.value(QStringLiteral("peerChoking")).toString();
    result.downloadSpeed = obj.value(QStringLiteral("downloadSpeed")).toString();
    result.uploadSpeed   = obj.value(QStringLiteral("uploadSpeed")).toString();
    result.seeder        = obj.value(QStringLiteral("seeder")).toString();
    return result;
};

Aria2Server Aria2Server::fromQJsonObject(const QJsonObject& obj) {
    Aria2Server result;
    result.uri           = obj.value(QStringLiteral("uri")).toString();
    result.currentUri    = obj.value(QStringLiteral("currentUri")).toString();
    result.downloadSpeed = obj.value(QStringLiteral("downloadSpeed")).toString();
    return result;
};

Aria2FileServers Aria2FileServers::fromQJsonObject(const QJsonObject& obj) {
    Aria2FileServers result;
    result.index = obj.value(QStringLiteral("index")).toString();

    const QJsonArray serversArray = obj.value(QStringLiteral("servers")).toArray();
    for (const QJsonValue& serverValue : serversArray) {
        result.servers.append(Aria2Server::fromQJsonObject(serverValue.toObject()));
    }

    return result;
};

Aria2BittorrentInfo Aria2BittorrentInfo::fromQJsonObject(const QJsonObject& obj) {
    Aria2BittorrentInfo result;
    result.comment      = obj.value(QStringLiteral("comment")).toString();
    result.creationDate = obj.value(QStringLiteral("creationDate")).toString();
    result.mode         = obj.value(QStringLiteral("mode")).toString();

    const QJsonObject infoObj = obj.value(QStringLiteral("info")).toObject();
    result.name = infoObj.value(QStringLiteral("name")).toString();

    const QJsonArray announceListArray = obj.value(QStringLiteral("announceList")).toArray();
    for (const QJsonValue& tierValue : announceListArray) {
        QVector<QString> tier;
        const QJsonArray tierArray = tierValue.toArray();
        for (const QJsonValue& uriValue : tierArray) {
            tier.append(uriValue.toString());
        }
        result.announceList.append(tier);
    }

    return result;
};


// ================================================================
// Aria2AddUri
// ================================================================

QJsonArray Aria2AddUriParams::toQJsonArray() const {
    QJsonArray array;

    QJsonArray urisArray;
    for (const QString& uri : uris) {
        urisArray.append(uri);
    };
    array.append(urisArray);

    if (tail1) {
        array.append(tail1->options);
        if (tail1->position) {
            array.append(*tail1->position);
        };
    };

    return array;
};

Aria2AddUriResponse Aria2AddUriResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2AddUriResponse result;
    result.gid = value.toString();
    return result;
};


// ================================================================
// Aria2AddTorrent
// ================================================================

QJsonArray Aria2AddTorrentParams::toQJsonArray() const {
    QJsonArray array;
    array.append(QString::fromLatin1(torrent.toBase64()));

    if (tail1) {
        QJsonArray urisArray;
        for (const QString& uri : tail1->uris) {
            urisArray.append(uri);
        };
        array.append(urisArray);

        if (tail1->tail2) {
            array.append(tail1->tail2->options);
            if (tail1->tail2->position) {
                array.append(*tail1->tail2->position);
            };
        };
    };

    return array;
};

Aria2AddTorrentResponse Aria2AddTorrentResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2AddTorrentResponse result;
    result.gid = value.toString();
    return result;
};


// ================================================================
// Aria2AddMetalink
// ================================================================

QJsonArray Aria2AddMetalinkParams::toQJsonArray() const {
    QJsonArray array;
    array.append(QString::fromLatin1(metalink.toBase64()));

    if (tail1) {
        array.append(tail1->options);
        if (tail1->position) {
            array.append(*tail1->position);
        };
    };

    return array;
};

Aria2AddMetalinkResponse Aria2AddMetalinkResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2AddMetalinkResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& gidValue : array) {
        result.gids.append(gidValue.toString());
    }
    return result;
};


// ================================================================
// Aria2Remove
// ================================================================

QJsonArray Aria2RemoveParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2RemoveResponse Aria2RemoveResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2RemoveResponse result;
    result.gid = value.toString();
    return result;
};


// ================================================================
// Aria2ForceRemove
// ================================================================

QJsonArray Aria2ForceRemoveParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2ForceRemoveResponse Aria2ForceRemoveResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ForceRemoveResponse result;
    result.gid = value.toString();
    return result;
};


// ================================================================
// Aria2Pause
// ================================================================

QJsonArray Aria2PauseParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2PauseResponse Aria2PauseResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2PauseResponse result;
    result.gid = value.toString();
    return result;
};


// ================================================================
// Aria2PauseAll
// ================================================================

Aria2PauseAllResponse Aria2PauseAllResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2PauseAllResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2ForcePause
// ================================================================

QJsonArray Aria2ForcePauseParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2ForcePauseResponse Aria2ForcePauseResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ForcePauseResponse result;
    result.gid = value.toString();
    return result;
};


// ================================================================
// Aria2ForcePauseAll
// ================================================================

Aria2ForcePauseAllResponse Aria2ForcePauseAllResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ForcePauseAllResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2Unpause
// ================================================================

QJsonArray Aria2UnpauseParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2UnpauseResponse Aria2UnpauseResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2UnpauseResponse result;
    result.gid = value.toString();
    return result;
};


// ================================================================
// Aria2UnpauseAll
// ================================================================

Aria2UnpauseAllResponse Aria2UnpauseAllResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2UnpauseAllResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2TellStatus
// ================================================================

QJsonArray Aria2TellStatusParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);

    if (keys) {
        QJsonArray keysArray;
        for (const QString& key : *keys) {
            keysArray.append(key);
        };
        array.append(keysArray);
    };

    return array;
};

Aria2TellStatusResponse Aria2TellStatusResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2TellStatusResponse result;
    const QJsonObject obj = value.toObject();

    result.gid                    = obj.value("gid").toString();
    result.status                 = obj.value("status").toString();
    result.totalLength            = obj.value("totalLength").toString();
    result.completedLength        = obj.value("completedLength").toString();
    result.uploadLength           = obj.value("uploadLength").toString();
    result.bitfield               = obj.value("bitfield").toString();
    result.downloadSpeed          = obj.value("downloadSpeed").toString();
    result.uploadSpeed            = obj.value("uploadSpeed").toString();
    result.infoHash               = obj.value("infoHash").toString();
    result.numSeeders             = obj.value("numSeeders").toString();
    result.seeder                 = obj.value("seeder").toString();
    result.pieceLength            = obj.value("pieceLength").toString();
    result.numPieces              = obj.value("numPieces").toString();
    result.connections            = obj.value("connections").toString();
    result.errorCode              = obj.value("errorCode").toString();
    result.errorMessage           = obj.value("errorMessage").toString();
    result.following              = obj.value("following").toString();
    result.belongsTo              = obj.value("belongsTo").toString();
    result.dir                    = obj.value("dir").toString();
    result.verifiedLength         = obj.value("verifiedLength").toString();
    result.verifyIntegrityPending = obj.value("verifyIntegrityPending").toString();

    const QJsonArray followedByArray = obj.value(QStringLiteral("followedBy")).toArray();
    for (const QJsonValue& gidValue : followedByArray) {
        result.followedBy.append(gidValue.toString());
    }

    const QJsonArray filesArray = obj.value(QStringLiteral("files")).toArray();
    for (const QJsonValue& fileValue : filesArray) {
        result.files.append(Aria2File::fromQJsonObject(fileValue.toObject()));
    }

    if (obj.contains(QStringLiteral("bittorrent"))) {
        result.bittorrent = Aria2BittorrentInfo::fromQJsonObject(
            obj.value(QStringLiteral("bittorrent")).toObject()
        );
    }

    return result;
};


// ================================================================
// Aria2GetUris
// ================================================================

QJsonArray Aria2GetUrisParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2GetUrisResponse Aria2GetUrisResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetUrisResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& uriValue : array) {
        result.uris.append(Aria2Uri::fromQJsonObject(uriValue.toObject()));
    }
    return result;
};


// ================================================================
// Aria2GetFiles
// ================================================================

QJsonArray Aria2GetFilesParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2GetFilesResponse Aria2GetFilesResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetFilesResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& fileValue : array) {
        result.files.append(Aria2File::fromQJsonObject(fileValue.toObject()));
    }
    return result;
};


// ================================================================
// Aria2GetPeers
// ================================================================

QJsonArray Aria2GetPeersParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2GetPeersResponse Aria2GetPeersResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetPeersResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& peerValue : array) {
        result.peers.append(Aria2Peer::fromQJsonObject(peerValue.toObject()));
    }
    return result;
};


// ================================================================
// Aria2GetServers
// ================================================================

QJsonArray Aria2GetServersParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2GetServersResponse Aria2GetServersResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetServersResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& fileValue : array) {
        result.fileServers.append(Aria2FileServers::fromQJsonObject(fileValue.toObject()));
    }
    return result;
};


// ================================================================
// Aria2TellActive
// ================================================================

Aria2TellActiveResponse Aria2TellActiveResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2TellActiveResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& statusValue : array) {
        result.downloads.append(Aria2TellStatusResponse::fromQJsonValue(statusValue));
    }
    return result;
};


// ================================================================
// Aria2TellWaiting
// ================================================================

QJsonArray Aria2TellWaitingParams::toQJsonArray() const {
    QJsonArray array;
    array.append(offset);
    array.append(num);

    if (keys) {
        QJsonArray keysArray;
        for (const QString& key : *keys) {
            keysArray.append(key);
        };
        array.append(keysArray);
    };

    return array;
};

Aria2TellWaitingResponse Aria2TellWaitingResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2TellWaitingResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& statusValue : array) {
        result.downloads.append(Aria2TellStatusResponse::fromQJsonValue(statusValue));
    }
    return result;
};


// ================================================================
// Aria2TellStopped
// ================================================================

QJsonArray Aria2TellStoppedParams::toQJsonArray() const {
    QJsonArray array;
    array.append(offset);
    array.append(num);

    if (keys) {
        QJsonArray keysArray;
        for (const QString& key : *keys) {
            keysArray.append(key);
        };
        array.append(keysArray);
    };

    return array;
};

Aria2TellStoppedResponse Aria2TellStoppedResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2TellStoppedResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& statusValue : array) {
        result.downloads.append(Aria2TellStatusResponse::fromQJsonValue(statusValue));
    }
    return result;
};


// ================================================================
// Aria2ChangePosition
// ================================================================

QJsonArray Aria2ChangePositionParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    array.append(pos);
    array.append(HOW_ENUM_TO_QSTRING_MAP[howToUnsignedInt(how)]);
    return array;
};

Aria2ChangePositionResponse Aria2ChangePositionResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ChangePositionResponse result;
    result.index = value.toInt();
    return result;
};


// ================================================================
// Aria2ChangeUri
// ================================================================

QJsonArray Aria2ChangeUriParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    array.append(fileIndex);

    QJsonArray delUrisArray;
    for (const QString& uri : delUris) {
        delUrisArray.append(uri);
    };
    array.append(delUrisArray);

    QJsonArray addUrisArray;
    for (const QString& uri : addUris) {
        addUrisArray.append(uri);
    };
    array.append(addUrisArray);

    if (position) {
        array.append(*position);
    };

    return array;
};

Aria2ChangeUriResponse Aria2ChangeUriResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ChangeUriResponse result;
    const QJsonArray array = value.toArray();
    if (!array.isEmpty()) {
        result.index = array.at(0).toInt();
        for (int i = 1; i < array.size(); ++i) {
            result.uris.append(array.at(i).toString());
        }
    }
    return result;
};


// ================================================================
// Aria2GetOption
// ================================================================

QJsonArray Aria2GetOptionParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2GetOptionResponse Aria2GetOptionResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetOptionResponse result;
    result.options = value.toObject();
    return result;
};


// ================================================================
// Aria2ChangeOption
// ================================================================

QJsonArray Aria2ChangeOptionParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    array.append(options);
    return array;
};

Aria2ChangeOptionResponse Aria2ChangeOptionResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ChangeOptionResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2GetGlobalOption
// ================================================================

Aria2GetGlobalOptionResponse Aria2GetGlobalOptionResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetGlobalOptionResponse result;
    result.options = value.toObject();
    return result;
};


// ================================================================
// Aria2ChangeGlobalOption
// ================================================================

QJsonArray Aria2ChangeGlobalOptionParams::toQJsonArray() const {
    QJsonArray array;
    array.append(options);
    return array;
};

Aria2ChangeGlobalOptionResponse Aria2ChangeGlobalOptionResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ChangeGlobalOptionResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2GetGlobalStat
// ================================================================

Aria2GetGlobalStatResponse Aria2GetGlobalStatResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetGlobalStatResponse result;
    const QJsonObject obj = value.toObject();

    result.downloadSpeed = obj.value(QStringLiteral("downloadSpeed")).toString();
    result.uploadSpeed   = obj.value(QStringLiteral("uploadSpeed")).toString();
    result.numActive     = obj.value(QStringLiteral("numActive")).toString();
    result.numWaiting    = obj.value(QStringLiteral("numWaiting")).toString();
    result.numStopped    = obj.value(QStringLiteral("numStopped")).toString();

    return result;
};


// ================================================================
// Aria2PurgeDownloadResult
// ================================================================

Aria2PurgeDownloadResultResponse Aria2PurgeDownloadResultResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2PurgeDownloadResultResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2RemoveDownloadResult
// ================================================================

QJsonArray Aria2RemoveDownloadResultParams::toQJsonArray() const {
    QJsonArray array;
    array.append(gid);
    return array;
};

Aria2RemoveDownloadResultResponse Aria2RemoveDownloadResultResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2RemoveDownloadResultResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2GetVersion
// ================================================================

Aria2GetVersionResponse Aria2GetVersionResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetVersionResponse result;
    const QJsonObject obj = value.toObject();

    result.version = obj.value(QStringLiteral("version")).toString();

    const QJsonArray featuresArray = obj.value(QStringLiteral("enabledFeatures")).toArray();
    for (const QJsonValue& featureValue : featuresArray) {
        result.enabledFeatures.append(featureValue.toString());
    }

    return result;
};


// ================================================================
// Aria2GetSessionInfo
// ================================================================

Aria2GetSessionInfoResponse Aria2GetSessionInfoResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2GetSessionInfoResponse result;
    result.sessionId = value.toObject()
                            .value(QStringLiteral("sessionId"))
                            .toString();
    return result;
};


// ================================================================
// Aria2Shutdown
// ================================================================

Aria2ShutdownResponse Aria2ShutdownResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ShutdownResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2ForceShutdown
// ================================================================

Aria2ForceShutdownResponse Aria2ForceShutdownResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2ForceShutdownResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// Aria2SaveSession
// ================================================================

Aria2SaveSessionResponse Aria2SaveSessionResponse::fromQJsonValue(const QJsonValue& value) {
    Aria2SaveSessionResponse result;
    result.ok = (value.toString() == QStringLiteral("OK"));
    return result;
};


// ================================================================
// SystemMulticall
// ================================================================

QJsonArray SystemMulticallParams::toQJsonArray() const {
    QJsonArray array;
    for (const SystemMulticallMethod& method : methods) {
        QJsonObject entry;
        entry[QStringLiteral("methodName")] = method.methodName;
        entry[QStringLiteral("params")]     = method.params;
        array.append(entry);
    };
    return array;
};

bool SystemMulticallResponse::Entry::isSuccess() const {
    return !isFault;
};

QJsonValue SystemMulticallResponse::Entry::resultValue() const {
    return value;
};

SystemMulticallResponse SystemMulticallResponse::fromQJsonValue(const QJsonValue& value) {
    SystemMulticallResponse result;
    const QJsonArray array = value.toArray();

    for (const QJsonValue& entryValue : array) {
        Entry entry;

        // The entry can be either a one-item array or a fault struct.
        if (entryValue.isArray()) {
            const QJsonArray inner = entryValue.toArray();
            if (!inner.isEmpty()) {
                entry.value = inner.at(0);
            }
            entry.isFault = false;
        } else if (entryValue.isObject()) {
            const QJsonObject obj = entryValue.toObject();
            if (obj.contains(QStringLiteral("fault"))) {
                const QJsonObject fault = obj.value(QStringLiteral("fault")).toObject();
                entry.isFault      = true;
                entry.faultCode    = fault.value(QStringLiteral("code")).toInt();
                entry.faultMessage = fault.value(QStringLiteral("message")).toString();
                entry.value        = fault;
            };
        };

        result.entries.append(entry);
    };

    return result;
};


// ================================================================
// SystemListMethods
// ================================================================

SystemListMethodsResponse SystemListMethodsResponse::fromQJsonValue(const QJsonValue& value) {
    SystemListMethodsResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& methodValue : array) {
        result.methods.append(methodValue.toString());
    }
    return result;
};


// ================================================================
// SystemListNotifications
// ================================================================

SystemListNotificationsResponse SystemListNotificationsResponse::fromQJsonValue(const QJsonValue& value) {
    SystemListNotificationsResponse result;
    const QJsonArray array = value.toArray();
    for (const QJsonValue& notificationValue : array) {
        result.notifications.append(notificationValue.toString());
    }
    return result;
};

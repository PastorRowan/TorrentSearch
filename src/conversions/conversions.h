
#pragma once

#include "Aria2c/Types.h"
#include "download/TorrentDownloadData.h"

namespace conversions {

    TorrentDownloadData aria2TellStatusResponseToTorrentDownloadData(
        const Aria2TellStatusResponse& aria2TellStatusResponse,
        TorrentDownloadData torrentDownloadData = {}
    );

};

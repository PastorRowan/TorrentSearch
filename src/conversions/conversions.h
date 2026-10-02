
#pragma once

#include "Aria2c/Types.h"
#include "download/TorrentDownloadData.h"

namespace conversions {

    TorrentDownloadData aria2TellStatusResponse(
        const Aria2TellStatusResponse& aria2TellStatusResponse
    );

};

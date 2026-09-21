
#include "providers/PirateBayProvider.h"

PirateBayProvider::PirateBayProvider(
    QObject* parent
): TorrentProvider(parent) {

};

void PirateBayProvider::search(
    const unsigned int searchId,
    const QString& query
) const {
    return TorrentSearchResults{
        {
            .name = "PirateBayProvider 1 - " + query
        },
        {
            .name = "PirateBayProvider 2 - " + query
        }
    };
};
